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
		fprintf(stderr, "Faiure constructing mysql_con handle: %s\n", mysql_error(mysql_con));
		// NEED TO DETERMINE HOW TO HANDLE A CONSTRUCTION ERROR
		exit(1);
	}

	if (mysql_real_connect(mysql_con, "localhost", "root", "password", 
		  NULL, 0, NULL, 0) == NULL) 
	{
		fprintf(stderr, "Failure connecting to mysql server: %s\n", mysql_error(mysql_con));
		mysql_close(mysql_con);
		// NEED TO DETERMINE HOW TO HANDLE A CONNECTION ERROR
		exit(1);
	}
	// Create database 
	if (mysql_query(mysql_con, "CREATE DATABASE IF NOT EXISTS dnslookup;")) 
	{
		fprintf(stderr, "Failure creating dnslookup database: %s\n", mysql_error(mysql_con));
		mysql_close(mysql_con);
		exit(1);
	}
	
	if (mysql_query(mysql_con, "USE dnslookup;"))
	{
		fprintf(stderr, "Failure using dnslookup database: %s\n", mysql_error(mysql_con));
		mysql_close(mysql_con);
		exit(1);
	}

	// Create table if non-existent
	if (mysql_query(mysql_con, "CREATE TABLE IF NOT EXISTS ipaddresses (ipaddress BIGINT, PRIMARY KEY (ipaddress));")) 
	{
		fprintf(stderr, "Failure creating table ipaddresses: %s\n", mysql_error(mysql_con));
		mysql_close(mysql_con);
		exit(1);
	}

	// Create memory table to speed up checks
	if (mysql_query(mysql_con, "CREATE TABLE IF NOT EXISTS ipaddresses_mem (ipaddress BIGINT, PRIMARY KEY (ipaddress)) ENGINE = MEMORY;")) 
	{
		fprintf(stderr, "Failure creating ipaddresses_mem table: %s\n", mysql_error(mysql_con));
		mysql_close(mysql_con);
		exit(1);
	}

	// Load non-volatile table into memory table
	if (mysql_query(mysql_con, "INSERT IGNORE INTO ipaddresses_mem (ipaddress) SELECT ipaddress FROM ipaddresses;")) 
	{
		fprintf(stderr, "Failure creating ipaddresses_mem table: %s\n", mysql_error(mysql_con));
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
		fprintf(stderr, "Failure selecting from ipaddresses_mem: %s\n", mysql_error(mysql_con));
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
			fprintf(stderr, "Failure inserting into ipaddresses: %s\n", mysql_error(mysql_con));
			mysql_close(mysql_con);
			exit(1);
		}

		query_ = "INSERT IGNORE INTO ipaddresses_mem VALUES (";
		snprintf(query, quer_len + 34, "%s%d);", query_, addr);
		if (mysql_query(mysql_con, query)) 
		{
			fprintf(stderr, "Failure inserting into ipaddresses_mem: %s\n", mysql_error(mysql_con));
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

	// Create prepared statments
	if (mysql_query(mysql_con, "PREPARE ipaddress_query FROM 'SELECT * FROM ipaddresses_mem WHERE ipaddress = ?';")){
		fprintf(stderr, "Failure creating ipaddress_query prepared statement: %s\n", mysql_error(mysql_con));
		mysql_close(mysql_con);
		exit(1);
	}

	// Create prepared statments
	if (mysql_query(mysql_con, "PREPARE ipaddress_insert FROM 'INSERT INTO ipaddresses_mem VALUES (?)';")){
		fprintf(stderr, "Failure creating ipaddress_insert prepared statment: %s\n", mysql_error(mysql_con));
		mysql_close(mysql_con);
		exit(1);
	}

	// Copy new ipaddresses from ipaddresses_mem into ipaddresses for later use
	if (mysql_query(mysql_con, "INSERT IGNORE INTO ipaddresses (ipaddress) SELECT ipaddress FROM ipaddresses_mem;")){
		fprintf(stderr, "Failrue copying ipaddresses_mem into ipaddresses: %s\n", mysql_error(mysql_con));
		exit(1);
	}

	// Deallocate prepared statements
	if (mysql_query(mysql_con, "DEALLOCATE PREPARE ipaddress_query;")){
		fprintf(stderr, "Failure deallocating ipaddress_query prepared statement: %s\n", mysql_error(mysql_con));
		exit(1);
	}
	if (mysql_query(mysql_con, "DEALLOCATE PREPARE ipaddress_insert;")){
		fprintf(stderr, "Failure deallocating ipaddress_query prepared statement: %s\n", mysql_error(mysql_con));
		exit(1);
	}

	mysql_close(mysql_con);
	exit(0);
}
