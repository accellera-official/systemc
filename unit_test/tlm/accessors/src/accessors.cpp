
#include <systemc>
using namespace sc_core;
using namespace std;

enum tlm_command {
    TLM_READ_COMMAND,
    TLM_WRITE_COMMAND
};

enum tlm_burst_mode {
    TLM_INCREMENT_BURST,
    TLM_STREAMING_BURST,
    TLM_WRAPPING_BURST
};

enum tlm_response_status {
    TLM_OK_RESP,
    TLM_GENERIC_ERROR_RESP,
    TLM_ADDRESS_ERROR_RESP,
    TLM_COMMAND_ERROR_RESP,
    TLM_BURST_ERROR_RESP,
    TLM_INCOMPLETE_RESP
};

class tlm_generic_payload {
public:

    tlm_command          m_command;
    sc_dt::uint64        m_address;
    unsigned char*       m_data;
    unsigned int         m_burst_length;
    unsigned int         m_burst_data_size;
    tlm_burst_mode       m_burst_mode;
    tlm_response_status  m_response_status;

    tlm_command         get_command()         const {return m_command;}
    sc_dt::uint64       get_address()         const {return m_address;}
    unsigned int        get_burst_length()    const {return m_burst_length;}
    unsigned int        get_burst_data_size() const {return m_burst_data_size;}
    tlm_burst_mode      get_burst_mode()      const {return m_burst_mode;}
    unsigned char*      get_data_ptr()        const {return m_data;}
    tlm_response_status get_response_status() const {return m_response_status;}

    void set_command(const tlm_command command)
        {m_command = command;}
    void set_address(const sc_dt::uint64 address)
        {m_address = address;}
    void set_burst_length(const unsigned int burst_length)
        {m_burst_length = burst_length;}
    void set_burst_data_size(const unsigned int burst_data_size)
        {m_burst_data_size = burst_data_size;}
    void set_burst_mode(const tlm_burst_mode burst_mode)
        {m_burst_mode = burst_mode;}
    void set_data_ptr(unsigned char* data)
        {m_data = data;}
    void set_response_status(const tlm_response_status response_status)
        {m_response_status = response_status;}

};


struct Config
{
	unsigned int repeats;
	bool         setget;
	bool         context_switch;
	Config() : repeats(1), setget(0), context_switch(0) {}
};

Config config;


SC_MODULE(Top)
{
	SC_CTOR(Top)
	{
		SC_THREAD(T1);
	}

	tlm_generic_payload* t;
	int mem[1000];

	void T1()
	{
		t = new tlm_generic_payload;
		unsigned char c;
		for (unsigned int i = 0; i < config.repeats; i++)
		{
			if (config.setget)
			{
				t->set_command( TLM_WRITE_COMMAND );
				t->set_address( i );
				c = i % 256;
				t->set_data_ptr( &c );
				t->set_burst_length( 1 );
				t->set_burst_data_size( 4 );
				t->set_burst_mode( TLM_INCREMENT_BURST );
			}
			else
			{
				t->m_command      = TLM_WRITE_COMMAND;
				t->m_address      = i;
				c                 = i % 256;
				t->m_data         = &c;
				t->m_burst_length = 1;
				t->m_burst_data_size = 4;
				t->m_burst_mode   = TLM_INCREMENT_BURST;
			}
			P(t);
			if (config.context_switch)
			  if ((i % 10) == 0) wait(SC_ZERO_TIME);
		}
		int sum = 0;
		for (int j = 0; j < 1000; j++)
			sum += mem[j];
		cout << "Checksum = " << sum << endl;

	}
	void P(tlm_generic_payload* t)
	{
		if (config.setget)
		{
			if (t->get_command() == TLM_WRITE_COMMAND && t->get_burst_mode() == TLM_INCREMENT_BURST)
			{
				for (unsigned int j = 0; j < t->get_burst_length(); j++)
					mem[(t->get_address() + j) % 1000] = *(t->get_data_ptr() + j);
				if (t->get_address() == config.repeats-1) sc_stop();
			}
		}
		else
		{
			if (t->m_command == TLM_WRITE_COMMAND && t->m_burst_mode == TLM_INCREMENT_BURST)
			{
				for (unsigned int j = 0; j < t->m_burst_length; j++)
					mem[(t->m_address + j) % 1000] = *(t->m_data + j);
				if (t->m_address == config.repeats-1) sc_stop();
			}
		}
	}
};

int sc_main( int argc, char* argv[])
{
	assert(argc == 4);
	config.repeats        = atoi(argv[1]); // Number of repeats
	config.setget         = atoi(argv[2]); // 0 => .field, 1 => setters and getters
	config.context_switch = atoi(argv[3]); // 0 => no waits, 1 => wait

	cout << "# repeats = " << config.repeats
	     << " setget = " << config.setget
	     << " context switch = " << config.context_switch << endl;

	Top top("top");
	sc_start();
	return 0;
}