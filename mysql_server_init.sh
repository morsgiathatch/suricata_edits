#!/bin/sh

host=localhost  # unused
user=root
pass=password

command="CREATE DATABASE IF NOT EXISTS dnslookup; 
USE dnslookup; 
CREATE TABLE IF NOT EXISTS ipaddresses (ipaddress BIGINT, PRIMARY KEY (ipaddress)); 
CREATE TABLE IF NOT EXISTS ipaddresses_mem (ipaddress BIGINT, PRIMARY KEY (ipaddress)) ENGINE = MEMORY AS SELECT * FROM ipaddresses;" 

if [[ $(mysql --user="$user" --password="$pass" --execute="$command") -ne 0 ]]
then
	echo "An error occured initializing mysql server"
fi



