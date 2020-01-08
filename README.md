# suricata_edits
This repo is to keep track of file changes to the Suricata library. 

## Building and Running

Using a VM of Ubuntu 18.04.3, run the following to get everything up and running from a fresh OS.
```bash
sudo apt update;
sudo apt-get upgrade;
sudo apt-get install libtool libpcre3-dev python3-distutils libyaml-dev libpcap0.8-dev libcap-ng-dev libnspr4-dev libnss3-dev libmagic-dev liblz4-dev libhtp-dev libz-dev libssl-dev;
sudo apt install autoconf m4 pkg-config	pkgconf libjansson-dev rustc cargo mysql-server libmysqlclient-dev;
```

Even though the upgrade may take awhile, it is necessary to get an upgraded version of `sudo`. 
After this, `cd` to the desired directory to clone the repo and run
```bash
git clone https://github.com/morsgiathatch/suricata_edits.git;
cd suricata_edits;
git clone https://github.com/OISF/libhtp;

```
After this, build the software.
```bash
sudo ./autogen.sh; 
sudo ./configure; 
sudo make; 
sudo make install;
```
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
