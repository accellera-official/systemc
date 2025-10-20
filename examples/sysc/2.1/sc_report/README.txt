

                    EXCEPTION REPORTING


Based on SystemC Verification Standard Specification,
Feb, 2003.


The exception reporting facility provides a common and configurable API to
report an exceptional situation.

The facility is presented by two classes: sc_report_handler and sc_report.
The former provides configuration and report generation calls.
The latter just contains the report related information.

The application defines an exceptional situation by using one of SC_REPORT_
macros to generate a report. The report is identified by its severity
(represented by sc_severity enum type) and the message type. The message
type is a string of characters, uniquely identifing a specific type of the
exception.

This sc_severity describes the severity of a report:

enum sc_severity { SC_INFO, SC_WARNING, SC_ERROR, SC_FATAL };

SC_INFO     The report is informative only.

SC_WARNING  The report indicates a potentially incorrect condition.

SC_ERROR    The report indicates a definite problem during execution.
            The default configuration forces a throw of a C++
            exception sc_exception with the corresponding report
            information attached.

SC_FATAL    The report indicates a problem which cannot be recovered
            from. In default configuration, the simulation is
            terminated immediately using an abort() call after
            reporting a SC_FATAL report.

The application can define actions to be taken for a generated report.
Whereas a usual reaction on a exceptional situation includes just printing a
message, more complex scenarios could involve a logging of the report into a
file, throwing a C++ exception or drop in the debugger.
The enum type sc_actions describes such a set of operations.

There are several predefined values for this type:

enum {
    SC_UNSPECIFIED  = 0x00,
    SC_DO_NOTHING   = 0x01,
    SC_THROW        = 0x02,
    SC_LOG          = 0x04,
    SC_DISPLAY      = 0x08,
    SC_CACHE_REPORT = 0x10,
    SC_INTERRUPT    = 0x20,
    SC_STOP         = 0x40,
    SC_ABORT        = 0x80
};

SC_UNSPECIFIED  Take the action specified by a configuration rule of a lower
                precedence.

SC_DO_NOTHING   Don't take any actions for the report, the action will be
                ignored, if other actions are given.

SC_THROW        Throw a C++ exception (sc_exception) that represents the
                report. The method sc_exception::get_report() can be used to
                access the report instance later.

SC_LOG          Print the report into the report log, typically a file on
                disk. The actual behavior is defined by the report handler
                function described below.

SC_DISPLAY      Display the report to the screen, typically by writing it in
                to the standard output channel using std::cout.

SC_INTERRUPT    Interrupt simulation if simulation is not being run in batch
                mode. Actual behavior is implementation defined, the
                default configuration calls sc_interrupt_here(...) debugging
                hook and has no further side effects.

SC_CACHE_REPORT Save a copy of the report. The report could be read later
                using sc_report_handler::get_cached_report(). The reports
                saved by different processes do not overwrite each other.

SC_STOP         Call sc_stop(). See sc_stop() manual for further detail.

SC_ABORT        The action requests the report handler to call abort().

The report handler, a function known to the class sc_report_handler, takes
the responsibility of execution the requested actions. Application is able
to redefine the report handler to take additional steps on execution of a
specific action or extend the default set of possible actions.
As the report handler is responsible for all predefined actions it can also
be used to redefine the behavior of predefined actions.

Each exception report can be configured to take one or more sc_actions.
Multiple actions can be specified using bit-wise OR. When SC_DO_NOTHING is
combined with any thing other than SC_UNSPECIFIED, the bit is ignored by the
facility.
In addition to the actions specified within the sc_actions enum, via
sc_actions, the exception API also can take two additional actions. The
first action is always taken: the sc_stop_here() function is called for
every report, thus providing users a convenient location to set breakpoints
to detect error reports, warning reports, etc. The second action that can be
taken is to force SC_STOP in the set of the actions to be executed. The
action is configured via the stop_after() method described below, which
allows users to set specific limits on the number of reports of various
types that will usually cause simulation to call sc_stop().


The configuration and report generation API is contained within the
sc_report_handler class.

The sc_report_handler Class 

The class provides only static API. The user cannot construct an
instance of the class.


    void report(
       sc_severity severity,
       const char* msg_type,
       const char* msg,
       const char* file,
       int         line
    );

Generate a report instance, which will cause the facility to take
the appropriate actions based on the current configuration.
The call will configure a not known before exception of msg_type to take
default set of actions for given severity.
The first occurence of the particular msg_type starts its stop_after()
counter.


    sc_actions set_actions(
       sc_severity severity,
       sc_actions actions = SC_UNSPECIFIED
    );

Configure the set of actions to take for reports of the given severity
(lowest precedence match). The previous actions set for this severity is
returned as the result. SC_UNSPECIFIED is returned if there was no previous
actions set for this severity.
TODO: what if to be taken actions are SC_UNSPECIFIED and there is now
highe precedence match? do nothing?


    sc_actions set_actions(
       const char* msg_type,
       sc_actions  actions = SC_UNSPECIFIED
    );

Configure the set of actions to take for reports of the given message type
(middle precedence match). The previous actions set for this message type is
returned as the result. SC_UNSPECIFIED is returned if there was no previous
actions set for this message type.


    sc_actions set_actions(
        const char* msg_type,
        sc_severity severity,
        sc_actions  actions = SC_UNSPECIFIED
    );

Configure the set of actions to take for reports having both the given
message type and severity (high precedence match). The previous actions set
for this message type and severity  is returned as the result.
SC_UNSPECIFIED is returned if there was no previous actions set for
this message type and severity.


The functions stop_after(...) modify only the limit, they do not affect the
counter of the number of reports. Setting the limit below the number of
already occured reports will cause sc_stop() for the next matching report.


    int stop_after(
       sc_severity severity,
       int limit = -1
    );

Call sc_stop() after encountering limit number of reports of the given
severity (lowest precedence match). If limit is set to one, the first
occurrence of a matching report will cause the abort. If limit is 0, abort
will never be taken due to a matching report. If limit is negative, abort
will never be taken for non-fatal error, and abort will be taken for the
first occurrence of a fatal error. The previous limit for this severity is
returned as the result. The stop_after() call will return UINT_MAX (int -1)
in the case where no previous corresponding stop_after() call was made.


    int stop_after(
       const char* msg_type,
       int limit = -1
    );

Call sc_stop() after encountering limit number of reports of the given
message type (middle precedence match). The previous limit for this message
type is returned as the result. If limit is 0, abort will never be taken due
to a matching report. If limit is negative, the limit specified by a lower
precedence rule is used. The stop_after() call will return UINT_MAX in the
case where no previous corresponding stop_after() call was made.


    int stop_after(
       sc_msg_type msg_type,
       sc_severity severity,
       int limit = -1
    );

Call sc_stop() after encountering limit number of reports having both the
given message type and severity.  (Highest precedence match.) If limit is 0,
abort will never be taken due to a matching report. If limit is negative,
the limit specified by a lower precedence rule is used. The previous limit
for this message type and severity is returned as the result. The call will
return UINT_MAX in the case where no previous corresponding stop_after()
call was made.


    sc_actions suppress(
        sc_actions actions
    );

Suppress specified actions for subsequent reports regardless of
configuration and clears previous calls to suppress(). The return value is
the actions that were suppressed prior to this call. The suppressed actions
are still active if they are mentioned by force(sc_actions) call.

    sc_actions suppress();

Restore default behavior by clearing previous calls to suppress(). The
return value is the actions that were suppressed prior to this call.
The default behavior does not suppress any actions.

    sc_actions force(
        sc_actions actions
    );

Force specified actions to be taken for subsequent reports in addition to
the actions specified in the current configuration and clears previous calls
to force(). The return value is the actions that were forced prior to this
call.
The actions given by this call override similar setting in suppress().

    sc_actions force();

Restore default behavior by clearing previous calls to force(). The return
value is the actions that were forced prior to this call.
There is no forced actions in the default configurations.


    sc_actions get_new_action_id();

Return an unused sc_actions value. Returns a different value each time it
is called (returns SC_UNSPECIFIED if no more unique values are available).
Used when establishing user-defined actions, interpreted by a non-default
report handler.
It is implementation defined whether the call could be used in the global
constructors.

    const char* get_log_file_name();

Return the log file name currently in effect. Return NULL if no logging is
active at the moment.
It is implementation defined whether the returned string actually represents
a file.

    void set_log_file_name(
        const char* name
    );

Set the log file name. The current handler implementation is responsible for
interpretation of the given argument. The name may be unused until first
SC_LOG action has occured.
The default implementation provides a plain text file logging. The file will
be opened after as the first SC_LOG action. The default implementation
provides a global function for closing the log file:
sc_report_close_default_log(). The logging is deactivated after calling the
function and subsequent calls to get_log_file_name() will return NULL.
The report handler is reponsible for proper terminating of the logging
facility at the end.


    const sc_report* get_cached_report();

Return pointer to the recent report for which was an SC_CACHE_REPORT action
defined. In the default configuration reports of severities SC_ERROR and
SC_FATAL are cached.

    void clear_cached_report();

Clear cached report for the current process (if any).

    void initialize();

Initializes default configuration.
The call shall reset the limit counters.
The call may not remove or reconfigure messages.
The call may not affect logging.
The call does not affect cached reports.

    void release();

Releases the resource possibly allocated by the exception reporting
implementation. The facility may not be used after this call. Whether the
facility could be used after subsequent initialize() call is defined by the
implementation. The default implementation removes all user defined and/or
configured messages and close the log file. Configured predefined messages
will be not reset.


    void set_handler(
        sc_report_handler_proc handler
    );

typedef void (*sc_report_handler_proc)(const sc_report&, const sc_actions&);

Specify the report handler function. The handler functions get an instance
of the generated report and can use the methods of sc_report to access the
needed information. The set of requested actions is passed through the
second argument.


    void default_handler(
        const sc_report& report,
        const sc_actions& actions
    );

The function is the default handler of the facility provided by the given
SystemC implementation.


The force() and suppress() methods provide a brute-force way to override the
current configuration. For example, force(SC_LOG) could be called during
debugging to cause all reports to be logged regardless of  the current
configuration. As another example, suppress(suppress() | SC_THROW); could be
called by code that is not C++ throw-safe when it starts execution, and then
suppress(prev) would be called when it completes execution.


The class sc_report the report representation.

An instance of the class could be accessed either through its cached copy or
in the catch block of C++ "try {} catch (const sc_exception & ex) {}"
construction. To access the report the method sc_exception::get_report() is
available.
Use sc_report_handler::get_cached_report() to access the cached copy of the
report.
Instances of the sc_report can be copied by copy constructor and assignment
operator means. It is not allowed to create an empty report.


The sc_report Class


    sc_severity get_severity() const;

Return the severity of a report object.


    const char* get_msg_type() const;

Get message type of a report object.
The returned string is guaranteed to persist until
sc_report_handler::release() is called.


    const char* get_msg() const;

Get message contents of a report object.
The lifetime of the returned pointer is that of the report instance.


    const char* get_file_name() const;

Get file name that generated report object.
Please see the definition of the SC_REPORT_ macros for the exact contents of
the returned value.


    int get_line_number() const;

Get line number that generated report object. See also: get_file_name().


    sc_time get_time() const;

Get the simulation time when then report object was generated.


    const sc_process_b* get_process() const;

Get the process that generated the report object. The process name could be
retrieved by calling the name() of the returned pointer.


When a report is logged to a file, the current simulation time and current
process name will automatically be included within the report.

The implementation defines following actions in the default configuration:

Severity    Actions

INFO       SC_LOG | SC_DISPLAY
WARNING    SC_LOG | SC_DISPLAY
ERROR      SC_LOG | SC_CACHE_REPORT | SC_THROW
FATAL      SC_LOG | SC_DISPLAY | SC_CACHE_REPORT | SC_ABORT

The error level reports are displayed by the default handler of sc_exception
type exceptions.

The following macros are globally visible as part of the standard and should
be used to generate reports:

#define SC_REPORT_INFO(msg_type, msg) \
sc_report_handler::report( SC_INFO, msg_type, msg, __FILE__, __LINE__ )
#define SC_REPORT_WARNING(msg_type, msg) \
sc_report_handler::report( SC_WARNING, msg_type, msg, __FILE__, __LINE__ )
#define SC_REPORT_ERROR(msg_type, msg) \
sc_report_handler::report( SC_ERROR, msg_type, msg, __FILE__, __LINE__ )
#define SC_REPORT_FATAL(msg_type, msg) \
sc_report_handler::report( SC_FATAL, msg_type, msg, __FILE__, __LINE__ )


The following examples illustrates how the exception API might be custom
configured and how reports are generated. Note that message types are best
captured within one or more header files, where they are declared using
#define macros. This technique insures that strings representing message
types are only declared once and that any typos that might occur when
message types are specified in the SC_REPORT_* macros are caught by the
compiler.

#define PCI_RPT_PROTOCOL_EXCEPTION    "PCI Protocol Exception"
const char PCI_RPT_PROTOCOL_READ_RETRY[] = "PCI Read Retry";

int sc_main(int, char**)
{
    // stop after having seen 10 error-level reports
    sc_report_handler::stop_after(SC_ERROR, 10);

    // make the PCI_RPT_PROTOCOL_EXCEPTION error non-critical
    // Note that 10 this errors will still cause a stop, as
    // configured by previous statement.
    sc_report_handler::set_actions(PCI_RPT_PROTOCOL_EXCEPTION,
                                   SC_ERROR,
                                   SC_DISPLAY);

    // disable the report PCI_RPT_PROTOCOL_READ_RETRY
    sc_report_handler::set_actions(PCI_RPT_PROTOCOL_READ_RETRY, SC_DO_NOTHING);

    sc_start(1, SC_MS);

    // allow the report PCI_RPT_PROTOCOL_READ_RETRY to be displayed
    sc_report_handler::set_actions(PCI_RPT_PROTOCOL_READ_RETRY, SC_DISPLAY);

    sc_start(1, SC_MS);

    // PCI_RPT_PROTOCOL_READ_RETRY reports will now be configured to
    // SC_UNSPECIFIED. Therefore, a lower precedence rule applies and the
    // actions in SC_DEFAULT_..._ACTIONS will take effect for the report.
    sc_report_handler::set_actions(PCI_RPT_PROTOCOL_READ_RETRY);

    sc_start(1, SC_MS);
}

void foo()
{
    sc_time max_time(500, SC_NS);

    if (...)
        SC_REPORT_ERROR(PCI_RPT_PROTOCOL_EXCEPTION, 
                        "PCI burst read exceeded max time limit of " + max_time.to_string());

    if (...)
        SC_REPORT_INFO(PCI_RPT_PROTOCOL_READ_RETRY, 
                       "PCI read retry at time " + sc_time_stamp().to_string());
}

The following example illustrates how reports using SC_CACHE_REPORT actions
can be accessed:

    ...
    sc_report_handler::set_actions(PCI_RPT_PROTOCOL_READ_RETRY,
                                   SC_INFO,
                                   SC_CACHE_REPORT|SC_LOG);
    ...

    void module::do_something()
    {
        if (...)
            SC_REPORT_INFO(PCI_RPT_PROTOCOL_READ_RETRY, "...");
    }
    void module::foo()
    {
        sc_report_handler::clear_cached_report();
        do_something();

        sc_report* rp = sc_report_handler::get_cached_report();

        if ( rp ) {
            cout << rp->get_msg() << endl;
        }
    }

The following example illustrates how reports using SC_THROW actions can be
accessed:

    ...
    sc_report_handler::set_actions(PCI_RPT_PROTOCOL_EXCEPTION,
                                   SC_ERROR,
                                   SC_THROW);
    ...

    void module::do_something()
    {
        if (...)
            SC_REPORT_ERROR(PCI_RPT_PROTOCOL_EXCEPTION, "...");
    }
    void module::bar()
    {
        try
        {
            do_something();
        }
        catch (const sc_exception & e)
        {
            cerr << e.get_report().get_msg() << endl;
        }
    }


