# suricata_edits
This repo is to keep track of file changes to the Suricata library. 

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

## Building and Running

First `cd` to the Suricata root directory. Then run
```bash
git clone https://github.com/OISF/libhtp
```
This should allow the following commands to run smoothely.
```bash
sudo ./autogen.sh; sudo ./configure; sudo make; sudo make install
```
You may need root access for a portion of the commands.
Warnings will appear when building the object files for `src/suricata.c` and
`src/stream-tcp.c`. These are harmless for our current purposes.

After this, to avoid issues with the `dnslookup` table from previous commits/builds,
run `sudo mysql` followed by
```mysql
DROP DATABASE dnslookup;
QUIT;
```

Since I use wifi interface `wlp2s0`, I use
```bash
sudo suricata -i wlp2s0

```
to run and `CTRL-C` to quit. Sample output using the above is 
```bash 
me@mycomp:~/Desktop/suricata_edits$ sudo suricata -i wlp2s0
[19374] 30/12/2019 -- 04:47:44 - (suricata.c:1103) <Notice> (LogVersion) -- This is Suricata version 5.0.1-dev (a37bc4f 2019-12-14) running in SYSTEM mode
[19374] 30/12/2019 -- 04:47:44 - (detect-engine-loader.c:230) <Warning> (ProcessSigFiles) -- [ERRCODE: SC_ERR_NO_RULES(42)] - No rule files match the pattern /usr/local/etc/suricata/rules/suricata.rules
[19374] 30/12/2019 -- 04:47:44 - (detect-engine-loader.c:345) <Warning> (SigLoadSignatures) -- [ERRCODE: SC_ERR_NO_RULES_LOADED(43)] - 1 rule files specified, but no rule was loaded at all!
[19374] 30/12/2019 -- 04:47:44 - (util-ioctl.c:324) <Warning> (SetEthtoolValue) -- [ERRCODE: SC_ERR_SYSCALL(50)] - Failure when trying to set feature via ioctl for 'wlp2s0': Operation not supported (95)
[19374] 30/12/2019 -- 04:47:44 - (tm-threads.c:2165) <Notice> (TmThreadWaitOnThreadInit) -- all 4 packet processing threads, 4 management threads initialized, engine started.
Initiailizing local mysql server
Inserting 209.197.3.24 to database
Inserting 2001:4de0:ac19:0000:0000:0001:000b:001a to database
Inserting 2001:4de0:ac19:0000:0000:0001:000b:001b to database
Inserting 2001:4de0:ac19:0000:0000:0001:000b:003a to database
Inserting 2001:4de0:ac19:0000:0000:0001:000b:002b to database
Inserting 2001:4de0:ac19:0000:0000:0001:000b:002a to database
Inserting 2001:4de0:ac19:0000:0000:0001:000b:003b to database
Address 209.197.3.24 in table!
Address 104.28.11.111 in table!
Address 172.217.11.238 in table!
Inserting 138.197.224.101 to database
Address 104.28.24.86 in table!
Address 104.28.24.86 in table!
Address 104.28.24.86 in table!
Address 104.28.24.86 in table!
Address 104.28.24.86 in table!
Address 104.28.24.86 in table!
Address 172.217.11.238 in table!
Inserting 172.217.214.189 to database
Inserting 2607:f8b0:4001:0c02:0000:0000:0000:00bd to database
^C[19374] 30/12/2019 -- 04:48:29 - (suricata.c:3039) <Notice> (SuricataMainLoop) -- Signal Received.  Stopping engine.
[19374] 30/12/2019 -- 04:48:30 - (util-device.c:360) <Notice> (LiveDeviceListClean) -- Stats for 'wlp2s0':  pkts: 375, drop: 0 (0.00%), invalid chksum: 0
[19374] 30/12/2019 -- 04:48:30 - (util-ioctl.c:324) <Warning> (SetEthtoolValue) -- [ERRCODE: SC_ERR_SYSCALL(50)] - Failure when trying to set feature via ioctl for 'wlp2s0': Operation not supported (95)
```

## ipset Test
This test looks at average insertion time in milliseconds while threads are inserting into ipsets. 
First, `ipset` must be installed. To do this, use 
```bash
sudo apt update 
sudo apt-get install ipset
```
If the above fails (which worked fine for me), an older guide can be found here <https://community.scaleway.com/t/installing-and-using-ipset-for-blacklisting/1853>.
Now to run the test, `cd` to the directory `suricata_edits/ipset_tests` and run
```bash
sudo python3 ./ipset_insert_test.py
```
The test randomly adds IPv6 or IPv4 entries to two separate ipset tables in two separate threads.
During this, occasionally query hits and misses are made and the query times are stored. Afterwards,
a printout shows average query time in seconds and average inserts per second into both tables. Running 
on a laptop with specs:
```bash
Architecture:        x86_64
CPU op-mode(s):      32-bit, 64-bit
Byte Order:          Little Endian
Address sizes:       39 bits physical, 48 bits virtual
CPU(s):              4
On-line CPU(s) list: 0-3
Thread(s) per core:  2
Core(s) per socket:  2
Socket(s):           1
NUMA node(s):        1
Vendor ID:           GenuineIntel
CPU family:          6
Model:               142
Model name:          Intel(R) Core(TM) i7-7500U CPU @ 2.70GHz
Stepping:            9
CPU MHz:             2900.064
CPU max MHz:         3500.0000
CPU min MHz:         400.0000
BogoMIPS:            5808.00
Virtualization:      VT-x
L1d cache:           32K
L1i cache:           32K
L2 cache:            256K
L3 cache:            4096K
NUMA node0 CPU(s):   0-3

```
I get the following results
```bash
Processed 400 / 400 queries after 39062 inserts
Average query time for IPv4 query during inserts was 0.0036564040184021
Average query time for IPv6 query during inserts was 0.0034817528724670
Average number of inserts per second was 766.6718794172512617
```
