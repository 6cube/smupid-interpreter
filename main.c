#include <stdio.h>
#include <signal.h>
#include "smupid.h"

FILE* f;

static int strcomp(char* str1, char* str2)
{
	int x = 0;
	while (*str1 != 0 || *str2 != 0)
	{
		x += *str1 - *str2;
		str1++;
		str2++;
	}
	return x;
}

static void prev_word(FILE* file) {
	long pos;

	// Step 1: Get the current position
	pos = ftell(file);

	// Step 2: Move backward and skip trailing whitespace
	while (pos > 0) {
		fseek(file, --pos, SEEK_SET);
		int ch = fgetc(file);

		if (ch != 32) { // !isspace(ch)
			break;  // Found the end of a word
		}
	}

	// Step 3: Continue moving backward until start of the word is found
	while (pos > 0) {
		fseek(file, --pos, SEEK_SET);
		int ch = fgetc(file);

		if (ch == 32) { // isspace(ch)
			pos++;  // Move one step forward to land on the first character of the word
			break;
		}
	}

	// Move the file pointer to the calculated position
	fseek(file, pos, SEEK_SET);
}

static void sigint_handler(int sig_num) {
	puts("\nProgram abort. Cleaning up...");
	fclose(f);
	smupid_cleanup();
	exit(0);
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		fputs("The syntax of the command is incorrect.\n", stderr);
		return 1;
	}
	if (strcomp(argv[1], "--version") == 0 || strcomp(argv[1], "--v") == 0 || strcomp(argv[1], "-version") == 0)
	{
		puts("smupid.h CLI v1.0 -> smupid.h C interpreter v1.0 -> smupid spec 1\n64-bit, 64KiB memory.");
		return 0;
	}

	fopen_s(&f, argv[1], "r");
	if (f == NULL)
	{
		fprintf(stderr, "File \"%s\" not found.\n", argv[1]);
		return 2;
	}
	char term[32];

	if (smupid_setup() == NULL)
	{
		fputs("Error allocating memory.", stderr);
		return 3;
	}

	if (signal(SIGINT, sigint_handler) == SIG_ERR) {
		perror("Error establishing SIGINT.");
		return 4;
	}

	fscanf_s(f, "%s", term, 32);
	while (ftell(f) > 0 && !feof(f))
	{
		switch (smupid(term))
		{
		case -1:
			prev_word(f);
			prev_word(f);
			break;
		case 0:
			prev_word(f);
			break;
		}
		fscanf_s(f, "%s", term, 32);
	}

	fclose(f);
	smupid_cleanup();
	return 0;
}
