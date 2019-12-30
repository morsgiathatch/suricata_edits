# suricata_edits
This repo is to keep track of file changes to the Suricata library. 

## MySQL Test
The tests are for me to test functionality of the C/mySQL interface
functionality. If you care, point to the root of the directory and run
```bash
cd mysql_tests; make; sudo ./test
```
in order to use the tests. 

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
./autogen.sh; ./configure; make; make install
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
to run and `CTRL-C` to quit.


