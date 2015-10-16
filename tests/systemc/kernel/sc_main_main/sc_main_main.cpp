#include "systemc.h"

int main()
{
    char *argv[] = { strdup("0"), strdup("1"), strdup("2"), strdup("3"),
                     strdup("4") };
    return sc_elab_and_sim( sizeof argv / sizeof argv[0], argv );
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
