FROM ubuntu:18.04
#ENV container docker

# Huge thank you to Victor Julien (github.com/inliniac) and all contributors of the Suricata project.
# These "syn_detector" suricata modifications were created by github.com/morsgiathatch and github.com/1stOctet. 

# Suricata on docker made possible by https://github.com/chongpohkit/suricata-docker/blob/master/Dockerfile/Dockerfile
# Docker container is ubuntu:18.04

# Host system is a new/clean Ubuntu 19.10 install
# lsb_release -a
# Description:	Ubuntu 19.10
# Release:	19.10
# Codename:	eoan
# Helpful commands for getting up and running. All of these packages may not be needed.
# sudo apt install apt-transport-https ca-certificates 
# sudo apt install curl 
# sudo apt install gnupg-agent 
# sudo apt software-properties-common
# sudo apt install docker-ce
# sudo apt install docker-ce-cli 
# sudo apt install git
# sudo apt install docker-compose 
# sudo docker-compose build
# sudo docker-compose build --no-cache
# sudo docker rmi -f $(sudo docker images -q -a)
# sudo docker rm $(sudo docker ps -a -q)
# sudo docker exec -it careful_synner bash

# cat .env file
# syn_detector=ubuntu

#FOR DEMO / PERFORMANCE REASONS, SURICATA IS RUNNING WITH NO RULES.
# THESE MESSAGES ARE EXPECTED
#This is Suricata version 5.0.2-dev (2c050187a 2020-01-20) running in SYSTEM mode
#<Warning> (ProcessSigFiles) -- [ERRCODE: SC_ERR_NO_RULES(42)] 
#- No rule files match the pattern /etc/suricata/rules/suricata.rules
#<Warning> (SigLoadSignatures) -- [ERRCODE: SC_ERR_NO_RULES_LOADED(43)] 
#- 1 rule files specified, but no rule was loaded at all!

RUN apt-get update
RUN apt -y install libpcre3 libpcre3-dev build-essential autoconf wget apt-utils \
    automake libtool libpcap-dev libnet1-dev libyaml-0-2 libyaml-dev liblua5.1-dev \
    zlib1g zlib1g-dev libmagic-dev libcap-ng-dev libjansson-dev pkg-config \
    libnetfilter-queue-dev geoip-bin geoip-database geoipupdate apt-transport-https \
    clang libelf-dev software-properties-common apt-utils llvm-6.0-dev llvm-6.0 git-core \
    python3-distutils libnspr4-dev libnss3-dev rustc cargo mysql-server libmysqlclient-dev \
    libmysql++-dev libssl-dev net-tools
RUN ln -s /usr/lib/llvm-6.0/bin/llc /usr/bin/llc

RUN cd /tmp/ && git clone https://github.com/OISF/suricata.git 

#Add careful_synner modifications
ADD flow-util-mods.c /tmp/suricata/src/flow-util.c
ADD output-json-dns-mods.c /tmp/suricata/src/output-json-dns.c
ADD suricata-mods.c /tmp/suricata/src/suricata.c
ADD output-json-dns-mods.h /tmp/suricata/src/output-json-dns.h


# sha256sum of the above files.
# 1stOctet@ubuntu19:Dockerfile$ sha256sum *.c
# 1fa74fcb62a33792d6ebbc9cce850647874cd916f32dbe3b4e0dd1e8913f15eb  flow-util-mods.c
# 7d32bf640d26fbd7ded8333194e78658f7dfb27f8be064ac24732e547915e7b7  output-json-dns-mods.c
# 604264e839e0453ce917b20bef00259a2c00c40f8c5a97c1a4051ccdb5fa725b  suricata-mods.c

# 1stOctet@ubuntu19:Dockerfile$ sha256sum *.h
# 5b8fcce879f41cf910aab49b5b447d3922dfd212701193f771735c50a63868ac  output-json-dns-mods.h

#add CFLAGS in suricata for mysql support.
RUN sed -i.bak /^AC_SUBST\(CFLAGS\)/s/^/"CFLAGS=\"\${CFLAGS} \`mysql_config --cflags\`\""\\n/ /tmp/suricata/configure.ac
RUN sed -i.bak2 /^AC_SUBST\(CFLAGS\)/s/^/"LDFLAGS=\"\${LDFLAGS} \`mysql_config --libs\`\""\\n/ /tmp/suricata/configure.ac

# if enabled, grep output below should be md5sum edbf5185a988fa64e266e21759856afc 
#RUN grep mysql_config /tmp/suricata/configure.ac | grep CF | md5sum
# if enabled, output below should be md5sum 6e6518e95b4ea7e88eac5ace9b50e051
#RUN grep mysql_config /tmp/suricata/configure.ac | grep LD | md5sum
# \we \sed \and \escape \gud.

RUN cd /tmp/suricata && git clone https://github.com/OISF/libhtp.git -b 0.5.x
RUN cd /tmp/suricata/ && ./autogen.sh && \
    CC=clang ./configure --prefix=/usr --sysconfdir=/etc --localstatedir=/var 
#--enable-nfqueue (may add later)

RUN cd /tmp/suricata/ \
    && make \
    && make install \
    && ldconfig
RUN cd /tmp/suricata/libhtp/ \
    && make install \
    && ldconfig

RUN mkdir -p /etc/suricata/
RUN cp /tmp/suricata/suricata.yaml /etc/suricata/suricata.yaml
RUN cp /tmp/suricata/threshold.config /etc/suricata/threshold.config
RUN cp -r /tmp/suricata/etc/* /etc/suricata/

RUN suricata -V

RUN suricata --build-info
COPY docker-entrypoint.sh /tmp/docker-entrypoint.sh
RUN chmod +x /tmp/docker-entrypoint.sh

## IMPORTANT!  You are responsible for ensuring the MYSQL Data is not lost.
## Shutting down docker by typing ctrl-c for example will cause the data to be lost.
## https://stackoverflow.com/questions/39175194/docker-compose-persistent-data-mysql

RUN echo "How to make sure MYSQL DATA is preserved on docker."
RUN echo "https://stackoverflow.com/questions/39175194/docker-compose-persistent-data-mysql"


