// Simple mysql test for functionality. To run, use 
//    $ sudo ./test
// when pointing to the directory. 


#include <my_global.h>
#include <mysql.h>
#include "stdio.h"


int main(){
	printf("MySQL client version: %s\n", mysql_get_client_info());
	// Construct mysql connection
	MYSQL *mysql_con = mysql_init(NULL);
	if (mysql_con == NULL) 
	{
		fprintf(stderr, "%s\n", mysql_error(mysql_con));
		// NEED TO DETERMINE HOW TO HANDLE A CONSTRUCTION ERROR
		exit(1);
	}

	if (mysql_real_connect(mysql_con, "localhost", "root", "password", 
		  NULL, 0, NULL, 0) == NULL) 
	{
		fprintf(stderr, "Error:%s\n", mysql_error(mysql_con));
		mysql_close(mysql_con);
		// NEED TO DETERMINE HOW TO HANDLE A CONNECTION ERROR
		exit(1);
	}
	// Create database 
	if (mysql_query(mysql_con, "CREATE DATABASE IF NOT EXISTS dnslookup;")) 
	{
		fprintf(stderr, "%s\n", mysql_error(mysql_con));
		mysql_close(mysql_con);
		exit(1);
	}
	
	if (mysql_query(mysql_con, "USE dnslookup;"))
	{
		fprintf(stderr, "%s\n", mysql_error(mysql_con));
		mysql_close(mysql_con);
		exit(1);
	}

	// Create table if non-existent
	if (mysql_query(mysql_con, "CREATE TABLE IF NOT EXISTS ipaddresses (ipaddress BIGINT, PRIMARY KEY (ipaddress));")) 
	{
		fprintf(stderr, "%s\n", mysql_error(mysql_con));
		mysql_close(mysql_con);
		exit(1);
	}

	// Load table into memory table to speed up checks
	if (mysql_query(mysql_con, "CREATE TABLE IF NOT EXISTS ipaddresses_mem (ipaddress BIGINT, PRIMARY KEY (ipaddress)) ENGINE = MEMORY AS SELECT * FROM ipaddresses;")) 
	{
		fprintf(stderr, "%s\n", mysql_error(mysql_con));
		mysql_close(mysql_con);
		exit(1);
	}

	// insert into table
	uint32_t addr = 19816811;
	char * query_ = "SELECT * FROM ipaddresses_mem WHERE ipaddress = ";
	int quer_len = strlen(query_);
	char query[quer_len + 34];
	snprintf(query, quer_len + 34, "%s%d;", query_, addr);

	if (mysql_query(mysql_con, query)){
		fprintf(stderr, "%s\n", mysql_error(mysql_con));
		mysql_close(mysql_con);
		exit(1);
	}

	MYSQL_RES *result = mysql_store_result(mysql_con);
	int num_rows = mysql_num_rows(result);
	if (num_rows == 0){
		// Must copy ipaddress into both tables
		char * query_ = "INSERT INTO ipaddresses VALUES (";
		snprintf(query, quer_len + 34, "%s%d);", query_, addr);
		if (mysql_query(mysql_con, query)) 
		{
			fprintf(stderr, "%s\n", mysql_error(mysql_con));
			mysql_close(mysql_con);
			exit(1);
		}

		query_ = "INSERT INTO ipaddresses_mem VALUES (";
		snprintf(query, quer_len + 34, "%s%d);", query_, addr);
		if (mysql_query(mysql_con, query)) 
		{
			fprintf(stderr, "%s\n", mysql_error(mysql_con));
			mysql_close(mysql_con);
			exit(1);
		}

	} else {
		MYSQL_ROW row;
		int num_fields = mysql_num_fields(result);
		while ((row = mysql_fetch_row(result))) 
		{ 
			for(int i = 0; i < num_fields; i++) 
			{ 
				printf("%s ", row[i] ? row[i] : "NULL"); 
			} 
				printf("\n"); 
		}
	}

	mysql_close(mysql_con);
	exit(0);

}
