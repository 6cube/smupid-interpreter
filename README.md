# smupid.h
This header contains the smupid interpreter itself, plus some crucial functions.

    - int smupid_setup()
    - int smupid(char* word)
    - void smupid_cleanup()

smupid.h also has some extra functions

	- void to_smupid(const char* text, char* buf)

To save on executable size, these can be excluded by using

	#define SMUP_ESSENTIALS_ONLY

**smupid_setup()**
This function is responsible for allocating the memory that smupid needs. Unexpected behavior will occur if smupid is called before smupid_setup(). If successful, this function will return 0.

*example/recommended usage:*

	if (smupid_setup() != 0)
	{
	    fputs("Memory allocation failed!", stderr);
        return 1;
    }

**smupid_cleanup()**

This function frees up the memory when the program is ready to exit. Make sure the program must call this before ending or memory will leak!

*example:*

    smupid_cleanup();
    return 0;

**smupid()**

This is the interpreter function. It takes the current term from the program that is being processed. It returns the amount that it wants the index of the program being processed to change

*example:*

	// this program interprets code from args
	#define SMUP_ESSENTIALS_ONLY
	#include "smupid.h"
	int main(int argc, char* argv[])
	{
		if (smupid_setup() != 0)
		{
			fputs("Memory allocation failed!", stderr);
			return 1;
		} // setup
		
		int i = 0;
		while (i >= 0 && i < argc) // smupid() either moves the index forward or back. do not let it read past the min or max
		{
			i += smupid(argv[i]);
		}
		smupid_cleanup();
		return 0;
	}
**to_smupid()**

This function converts text to smupid code, leaving it in the buffer string.

*example:*
  
	#include <stdio.h>
	#include "smupid.h"
	
	int main()
	{
		char text[64];
		char buf[512];
		
		scanf("%63s", text);
		to_smupid(text, buf);
		
		puts(buf);
		return 0;
	}

 # smupid executable
 Usage:

     smupid <filename>
filename: name of file to execute.
