#include "systemc.h"


int main()
{
	char arg0[] = "0";
	char arg1[] = "1";
	char arg2[] = "2";
	char arg3[] = "3";
	char arg4[] = "4";
	char* argv[] = { arg0, arg1, arg2, arg3, arg4 };
	return sc_elab_and_sim( sizeof(argv)/sizeof(char*), argv );
}

int sc_main(int argc, char* argv[])
{
	if ( argc != sc_argc() ) 
	{
	    cout << "sc_argc mismatch: expected " << argc << " got " << sc_argc()
			<< endl;
	}
	for ( int argi = 0; argi < argc; argi++ )
	{
		if ( strcmp( argv[argi], sc_argv()[argi] ) )
		{
			cout << "sc_argv()[" << argi << "] mismatch: expected: '" 
			     << argv[argi] << "' got: '" << sc_argv()[argi] << "'" << endl;
		}
	}

	cerr << "Program completed" << endl;

    return 0;
}

