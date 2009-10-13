#include "systemc.h"

void dump_signals(const char* module_name)
{
	sc_simcontext* c_p = sc_get_curr_simcontext();
	unsigned int   mod_name_len = strlen(module_name);
	sc_object*     obj_p;
	const char*    obj_name_p;


	for (obj_p = c_p->first_object(); obj_p; obj_p=c_p->next_object())
	{
		obj_name_p = obj_p->name();
		if ( strlen(obj_name_p) > mod_name_len && 
		     !strncmp(obj_p->name(),module_name, mod_name_len) &&
			 !strcmp(obj_p->kind(),"sc_signal") )
		{
			cout << "   " << obj_p->name() << ": " << obj_p->kind() << endl;
		}
	}
	cout << endl;
}

SC_MODULE(A)
{
	SC_CTOR(A) : a("a"), b("b"), c("c")
	{
	}
	sc_signal<int>     a, b, c;
	sc_in_clk      m_clk;
	sc_signal<int> x, y, z;
};


SC_MODULE(TB)
{
	SC_CTOR(TB)
	{
		SC_METHOD(sync);
		sensitive << m_clk.pos();
		dont_initialize();
	}
	void sync()
	{
	cout << "Internal: " << endl;
		dump_signals("a");
	}
	sc_in_clk      m_clk;
};

int sc_main(int argc, char* argv[])
{
	sc_clock        clock;
	sc_signal<int>  dummy;
	A               module_a("a");
	TB              tb("tb");
	module_a.m_clk(clock);
	tb.m_clk(clock);

	cout << "External: " << endl;
	dump_signals(module_a.name());
	sc_start(1);


	cerr << "Program completed" << endl;
	return 0;
}
