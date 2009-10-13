#include "systemc.h"

int main(int argc, char* argv[])
{
	sc_elab_and_sim( argc, argv );
}

int sc_main(int argc, char* argv[])
{
	const char* release_string_p;

	release_string_p = sc_release();
	if (strncmp(release_string_p, SC_RELEASE_STRING, strlen(SC_RELEASE_STRING)))
	{
		cout <<  "sc_release() mismatch expected: '" << SC_RELEASE_STRING << "' got: '" << release_string_p << "'" << endl;
	}
	cerr << "Program completed" << endl;

    return 0;
}

