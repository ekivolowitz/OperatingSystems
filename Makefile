zip:
	gcc -o my-zip my-zip.c -Wall -Werror
test-zip:
	~cs537-1/tests/bin/test-my-zip.csh -v