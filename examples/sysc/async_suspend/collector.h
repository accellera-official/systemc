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

  Original Author: Mark Burton, GreenSocs

 *****************************************************************************/

#ifndef ASYNCTESTCOLLECTOR_H
#define ASYNCTESTCOLLECTOR_H

//#define WITHMATPLOT
#ifdef WITHMATPLOT
#include "matplotlibcpp.h"
namespace plt = matplotlibcpp;
#endif

/* thread-safe collector to enable pretty-printing the output of the
 * simulation as a csv style output (import it into a spreadsheet and draw some
 * nice graphs !*/
class collector
{
private:
    std::unordered_map<const char *, sc_time> names;
    std::mutex lock;
    std::vector<std::pair<const char *, const sc_time>> times;

public:
    void add(const char *name, const sc_time mytime)
    {
        std::lock_guard<std::mutex> guard(lock);
        if (names.find(name) == names.end())
        {
            names.insert(std::make_pair(name, SC_ZERO_TIME));
        }
        times.push_back(std::make_pair(name, mytime));
    }

    void csvreport()
    {
        cout << "event";
        for (auto kv : names)
        {
            cout << ", " << kv.first;
        }
        cout << "\n";
        int i = 0;
        for (std::vector<std::pair<const char *, const sc_time>>::iterator it = times.begin();
             it != times.end(); ++it)
        {
            names[std::get<0>(*it)] = std::get<1>(*it);
            cout << i++;
            for (auto kv : names)
            {
                cout << ", " << kv.second.to_seconds() * 1000000000;
            }
            cout << "\n";
        }
    }

#ifdef WITHMATPLOT

    void matplot()
    {
        std::unordered_map<const char *, std::vector<double>> vecs;
        for (auto kv : names)
        {
            vecs.insert(std::make_pair(kv.first, std::vector<double>()));
        }
        for (std::vector<std::pair<const char *, const sc_time>>::iterator it = times.begin();
             it != times.end(); ++it)
        {
            names[std::get<0>(*it)] = std::get<1>(*it);
            for (auto kv : names)
            {
                vecs[kv.first].push_back(kv.second.to_seconds() * 1000000000);
            }
        }
        for (auto kv : names)
        {
            plt::plot(vecs[kv.first]);
        }

        plt::save("./output.png");
    }

    void report()
    {
        matplot();
    }

#else

    void report()
    {
        csvreport();
    }

#endif

};

#endif