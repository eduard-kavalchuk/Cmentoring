#include <stdio.h>
#include <libgen.h>

/**
 * Print a name of its binary.
 * 
 * Clause 5.1.2.2.1 of the C standard states the following:
	If the value of argc is greater than zero, the string pointed to by argv[0] represents the program name; argv[0][0] shall be the null character if the program name is not available from the host environment.
   The problem is that standard says that argv[0] "represents" program's name. It says nothing about how the name should be represented. In Linux envorinment argv[0] may contain different things depending on how the program was invoked:
   >> ./a.out  -->  ./a.out     When called from a local directory a program name is returned prepended with a relative path
   >> ~/a.out  -->  /home/ed/a.out   When called with an absolute path the name returned will contain a full path to the binary
   If a soft link to a binary  with a name *myprog* is created in /bin directory (or any other directory visible via PATH) then calling the program via the name of its link will return the name of its link (which is not we want):
   >> myprog  --> myprog
   
   The only reliable way that I see to get the binary name is to make use of Linux basename(1) function.
   Now the program returns the name of its binary (including file extension) no matter whether it is called with a relative or full path to that binary:
   >> ./a.out  -->  a.out
   >> ~/a.out  -->  a.out
   
 * @param[in] void
 * @return Return type
 */
int main(int argc, char** argv) {
	printf("%s\n", basename(argv[0]));
	
	return 0;
}
