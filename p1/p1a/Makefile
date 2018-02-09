zip:
	gcc -o my-zip my-zip.c -Wall -Werror -g
test-zip:
	~cs537-1/tests/bin/test-my-zip.csh -c 
unzip:
	gcc -o my-unzip my-unzip.c -Wall -Werror -g
test-unzip:
	~cs537-1/tests/bin/test-my-unzip.csh -v 
test-all:
	~cs537-1/tests/bin/test-my-zip.csh -c
	~cs537-1/tests/bin/test-my-cat.csh -c
	~cs537-1/tests/bin/test-my-grep.csh -c
	~cs537-1/tests/bin/test-my-unzip.csh -c