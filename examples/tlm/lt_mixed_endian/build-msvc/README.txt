Command line
~~~~~~~~~~~~
If you run using nmake and the Makefile, the example should pass.

Visual C++ GUI
~~~~~~~~~~~~~~
If you run using the supplied Visual C++ .sln (solution), you will need to supply a command
line input of ..\results\input.txt and also filter out any lines in the run.log file
that *DO NOT* contain the string "::" (without the quotes) before comparing to the expected.log file.

To supply a command line input in Visual C++

1. click on the project lt in the solution explorer

2. right click on project and select Properties

3. In the lt Project Properties page, browse to Configuration Properties > Debugging

4. Click in the Command Arguments box and type in

< ..\results\input.txt

and click OK.

Now you can run the program.

