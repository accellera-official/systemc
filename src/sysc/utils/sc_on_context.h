/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_on_context.h -- Job handler to run on context

  Original Author: Mark burton

  CHANGE LOG AT END OF FILE
 *****************************************************************************/

#ifndef SC_ON_CONTEXT_H
#define SC_ON_CONTEXT_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <future>
#include <chrono>

#include <sysc/kernel/sc_simcontext.h>
#include <sysc/kernel/sc_event.h>

namespace sc_core {

class sc_on_context : public sc_core::sc_module
{
protected:
    class AsyncJob
    {
    public:
        using Ptr = std::shared_ptr<AsyncJob>;

    private:
        std::packaged_task<void()> m_task;

        bool m_cancelled = false;

        void run_job() { m_task(); }

    public:
        AsyncJob(std::function<void()>&& job): m_task(job) {}

        AsyncJob(std::function<void()>& job): m_task(job) {}

        AsyncJob() = delete;
        AsyncJob(const AsyncJob&) = delete;

        void operator()() { run_job(); }

        /**
         * @brief Cancel a job
         *
         * @details Cancel a job by setting m_cancelled to true and by
         * resetting the task. Any waiter will then be unblocked immediately.
         */
        void cancel()
        {
            m_cancelled = true;
            m_task.reset();
        }

        void wait()
        {
            auto future = m_task.get_future();

                while (!m_cancelled &&
                    future.wait_for(std::chrono::milliseconds(500))==std::future_status::timeout)
                    {}

            if (!m_cancelled) {
                future.get();
            }
        }

        bool is_cancelled() const { return m_cancelled; }
    };

    
    sc_core::sc_simcontext *m_simc=nullptr;

    /* Async job queue */
    std::queue<AsyncJob::Ptr> m_async_jobs;
    AsyncJob::Ptr m_running_job;
    std::mutex m_async_jobs_mutex;

    sc_event m_jobs_handler_event; // NB this event must be threadsafe.
    std::atomic<bool> running = true;

    // Process inside a thread incase the job calls wait
    void jobs_handler()
    {
        m_simc=sc_core::sc_get_curr_simcontext();

        std::unique_lock<std::mutex> lock(m_async_jobs_mutex);
        running = true;
        for (; running;) {
            while (running && !m_async_jobs.empty()) {
                m_running_job = m_async_jobs.front();
                m_async_jobs.pop();

                lock.unlock();
                sc_core::sc_unsuspendable(); // a wait in the job will cause systemc time to advance
                (*m_running_job)();
                sc_core::sc_suspendable();
                lock.lock();

                m_running_job.reset();
            }
            lock.unlock();
            wait(m_jobs_handler_event);
            lock.lock();
        }
        SC_REPORT_WARNING("sc_on_context", "Stopped");
        sc_core::sc_stop();
    }

    void cancel_pendings_locked()
    {
        while (!m_async_jobs.empty()) {
            m_async_jobs.front()->cancel();
            m_async_jobs.pop();
        }
    }

public:
    sc_on_context(const sc_core::sc_module_name& n = sc_core::sc_module_name("sc_on_ctx"))
        : sc_module(n)
    {
        SC_THREAD(jobs_handler);
    }

    /**
     * @brief Cancel all pending jobs
     *
     * @detail Cancel all the pending jobs. The callers will be unblocked
     *         if they are waiting for the job.
     */
    void cancel_pendings()
    {
        std::lock_guard<std::mutex> lock(m_async_jobs_mutex);

        cancel_pendings_locked();
    }

    /**
     * @brief Cancel all pending and running jobs
     *
     * @detail Cancel all the pending jobs and the currently running job.
     *         The callers will be unblocked if they are waiting for the
     *         job. Note that if the currently running job is resumed, the
     *         behaviour is undefined. This method is meant to be called
     *         after simulation has ended.
     */
    void cancel_all()
    {
        std::lock_guard<std::mutex> lock(m_async_jobs_mutex);

        cancel_pendings_locked();

        if (m_running_job) {
            m_running_job->cancel();
            m_running_job.reset();
        }
    }
    void stop()
    {
        running = false;
        m_jobs_handler_event.notify(sc_core::SC_ZERO_TIME);
    }

    void end_of_simulation()
    {
        running = false;
        cancel_all();
    }

    void fork(std::function<void()> job_entry) { run(job_entry, false); }

    /**
     * @brief Run a job on the SystemC kernel thread
     *
     * @param[in] job_entry The job to run
     * @param[in] wait If true, wait for job completion
     *
     * @return true if the job has been succesfully executed or if `wait`
     *         was false, false if it has been cancelled (see
     *         `Sc_on_context::cancel_all`).
     */
    bool run(std::function<void()> job_entry, bool wait = true)
    {
        if (!running) return false;
        if (is_on_sysc()) {
            job_entry();
            return true;
        } else {
            AsyncJob::Ptr job(new AsyncJob(job_entry));

            {
                std::lock_guard<std::mutex> lock(m_async_jobs_mutex);
                if (running) {
                    m_async_jobs.push(job);
                } else {
                    return false;
                }
            }

            m_jobs_handler_event.notify();

            if (wait) {
                /* Wait for job completion */
                try {
                    job->wait();
                } catch (std::runtime_error const& e) {
                    /* Report unknown runtime errors, without causing a futher excetion */
                    auto old = sc_core::sc_report_handler::set_actions(sc_core::SC_ERROR,
                                                                       sc_core::SC_LOG | sc_core::SC_DISPLAY);
                    SC_REPORT_ERROR(
                        "Sc_on_context",
                        ("Run on systemc received a runtime error from job: " + std::string(e.what())).c_str());
                    sc_core::sc_report_handler::set_actions(sc_core::SC_ERROR, old);
                    stop();
                    return false;
                } catch (const std::exception& exc) {
                    if (sc_core::sc_report_handler::get_count(sc_core::SC_ERROR) == 0) {
                        /* Report exceptions that were not caused by SC_ERRORS (which have already been reported)*/
                        auto old = sc_core::sc_report_handler::set_actions(sc_core::SC_ERROR,
                                                                           sc_core::SC_LOG | sc_core::SC_DISPLAY);
                        SC_REPORT_ERROR(
                            "Sc_on_context",
                            ("Run on systemc received an exception from job: " + std::string(exc.what())).c_str());
                        sc_core::sc_report_handler::set_actions(sc_core::SC_ERROR, old);
                    }
                    stop();
                    return false;
                } catch (...) {
                    auto old = sc_core::sc_report_handler::set_actions(sc_core::SC_ERROR,
                                                                       sc_core::SC_LOG | sc_core::SC_DISPLAY);
                    SC_REPORT_ERROR("Sc_on_context", "Run on systemc received an unknown exception from job");
                    sc_core::sc_report_handler::set_actions(sc_core::SC_ERROR, old);
                    stop();
                    return false;
                }

                return !job->is_cancelled();
            }

            return true;
        }
    }

    /**
     * @return Whether we are on the right SystemC context
     */
    bool is_on_sysc() const { return sc_core::sc_get_curr_simcontext()==m_simc; }
};
}
#endif // SC_ON_CONTEXT_H