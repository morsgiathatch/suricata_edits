# suricata_edits
This repo is to keep track of file changes to the Suricata library. 

## Tests
The tests are for me to test functionality of the C/mySQL interface
functionality. If you care, point to the root of the directory and run
```bash
	$ cd mysql_tests; make; sudo ./test
```
in order to use the tests. 

## Dependencies

In order to use the software, you must have `mysql server` installed. To install
on ubuntu, use:
```bash
	sudo apt update
	sudo apt install mysql-server
```
There are additional installations that are recommended, however I did not use them. They are 
```bash
	sudo mysql_secure_installation
```
If you install the additional software, there are prompts that let you decide how to set up `mysql-server`.
After this, build the software below. As it stands, no passwords or users need to be set for the database
for this project. All that is needed is root access and the hope that you do not have a databasse called `dnslookup`
in your local mysql-server.


## Building

First `cd` to the Suricata root directory. Then run
```bash
	git clone https://github.com/OISF/libhtp
```
This should allow the following commands to run smoothely.
```bash
	./autogen.sh; ./configure; make; make install
```
You may need root access for a portion of the commands.

## Current Issues

We have not decided how to gracefully handle mysql-database errors. Additionally, I have no idea why
I get errors when I run
```bash
	sudo suricata -c suricata.yaml -s signatures.rules -i eth0
```
These are the errors:
```bash
[3040] 14/12/2019 -- 16:50:03 - (suricata.c:1103) <Notice> (LogVersion) -- This is Suricata version 5.0.1-dev (80fec43 2019-12-14) running in SYSTEM mode
[3040] 14/12/2019 -- 16:50:03 - (detect-engine-loader.c:230) <Warning> (ProcessSigFiles) -- [ERRCODE: SC_ERR_NO_RULES(42)] - No rule files match the pattern /usr/local/etc/suricata/rules/suricata.rules
[3040] 14/12/2019 -- 16:50:03 - (detect-engine-loader.c:230) <Warning> (ProcessSigFiles) -- [ERRCODE: SC_ERR_NO_RULES(42)] - No rule files match the pattern signatures.rules
[3040] 14/12/2019 -- 16:50:03 - (detect-engine-loader.c:345) <Warning> (SigLoadSignatures) -- [ERRCODE: SC_ERR_NO_RULES_LOADED(43)] - 2 rule files specified, but no rule was loaded at all!
[3040] 14/12/2019 -- 16:50:03 - (source-af-packet.c:1681) <Error> (AFPGetDevLinktype) -- [ERRCODE: SC_ERR_AFP_CREATE(190)] - Unable to find type for iface "eth0": No such device
[3040] 14/12/2019 -- 16:50:03 - (tm-threads.c:2165) <Notice> (TmThreadWaitOnThreadInit) -- all 4 packet processing threads, 4 management threads initialized, engine started.
[3043] 14/12/2019 -- 16:50:03 - (source-af-packet.c:1665) <Error> (AFPGetIfnumByDev) -- [ERRCODE: SC_ERR_AFP_CREATE(190)] - Unable to find iface eth0: No such device
[3043] 14/12/2019 -- 16:50:03 - (source-af-packet.c:1507) <Error> (ReceiveAFPLoop) -- [ERRCODE: SC_ERR_AFP_CREATE(190)] - Couldn't init AF_PACKET socket, fatal error
[3040] 14/12/2019 -- 16:50:03 - (tm-threads.c:2082) <Error> (TmThreadCheckThreadState) -- [ERRCODE: SC_ERR_FATAL(171)] - thread W#01-eth0 failed

```

