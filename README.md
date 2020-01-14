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

