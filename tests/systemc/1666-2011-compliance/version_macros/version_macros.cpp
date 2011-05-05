// Process control method throw_it

#define SC_INCLUDE_DYNAMIC_PROCESSES

#include <systemc>

using namespace sc_core;
using std::cout;
using std::endl;

int sc_main(int argc, char* argv[])
{
  sc_assert(  SC_VERSION_MAJOR        == sc_version_major );
  sc_assert(  SC_VERSION_MINOR        == sc_version_minor );
  sc_assert(  SC_VERSION_PATCH        == sc_version_patch );
  sc_assert(  SC_VERSION_ORIGINATOR   == sc_version_originator );
  sc_assert(  SC_VERSION_RELEASE_DATE == sc_version_release_date );
  sc_assert(  SC_VERSION_PRERELEASE   == sc_version_prerelease );
  sc_assert(  SC_IS_PRERELEASE        == sc_is_prerelease );
  sc_assert(  SC_VERSION              == sc_version_string );
  sc_assert(  SC_COPYRIGHT            == sc_copyright_string );

  cout << "IEEE_1666_SYSTEMC = "       << IEEE_1666_SYSTEMC << endl;
  cout << endl;
  cout << "sc_version_major = "        << sc_version_major << endl;  
  cout << "sc_version_minor = "        << sc_version_minor << endl;  
  cout << "sc_version_patch = "        << sc_version_patch << endl;  
  cout << "sc_version_originator = "   << sc_version_originator << endl;  
  cout << "sc_version_release_date = " << sc_version_release_date << endl;  
  cout << "sc_version_prerelease = "   << sc_version_prerelease << endl;  
  cout << "sc_is_prerelease = "        << sc_is_prerelease << endl;  
  cout << "sc_version_string = "       << sc_version_string << endl;  
  cout << "sc_copyright_string = "     << sc_copyright_string << endl;  
  cout << endl;
  cout << "sc_release() = "            << sc_release() << endl;  
  cout << "sc_version() = "            << sc_version() << endl;  
  cout << "sc_copyright() = "          << sc_copyright() << endl;  
  
  //sc_start();
 
  cout << endl << "Success" << endl;
  return 0;
}
  
