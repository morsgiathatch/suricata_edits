# suricata_edits
This repo is to keep track of file changes to the Suricata library. 

## Tests
The tests are for me to test functionality of the C/mySQL interface
functionality. If you care, point to the root of the directory and run
```bash
	$ cd mysql_tests; make; sudo ./test
```
in order to use the tests. 

## Building

First `cd` to the Suricata root directory. Then run
```bash
	./autogen.sh; ./configure; make; make install
```
You may need root access for a portion of the commands.
