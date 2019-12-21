import threading
import time
import matplotlib.pyplot as plt
import numpy as np
import subprocess
import random
from ipaddress import IPv4Address, IPv6Address
import sys


# Insert into ipsett without pause
def insert_random_ip_into_ipset_v4(continue_inserting, total_inserts):
	while continue_inserting[0]: 
		bashCommand = "ipset add test_set_v4 " + str(IPv4Address(random.getrandbits(32)))
		process = subprocess.Popen(bashCommand.split(), stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
		total_inserts[0] += 1
	

def insert_random_ip_into_ipset_v6(continue_inserting, total_inserts):
	while continue_inserting[0]: 
		bashCommand = "ipset add test_set_v6 " + str(IPv6Address(random.getrandbits(128)))
		process = subprocess.Popen(bashCommand.split(), stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
		total_inserts[0] += 1


if __name__ == '__main__':
	num_test_hit_entries = 100
	test_hit_entries_v4 = []
	test_hit_entries_v6 = []
	total_inserts = [0]
	continue_inserting = [True]

	# First create test tables
	bashCommand = "ipset create test_set_v4 hash:net family inet"
	process = subprocess.Popen(bashCommand.split(), stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
	bashCommand = "ipset create test_set_v6 hash:net family inet6"
	process = subprocess.Popen(bashCommand.split(), stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

	# Generate and store 100 ipaddresses for possible use
	for i in range(0, num_test_hit_entries):
		test_hit_entries_v4.append(IPv4Address(random.getrandbits(32)))
		test_hit_entries_v6.append(IPv6Address(random.getrandbits(128)))

	# add ipaddresses to ipsets
	for i in range(0, num_test_hit_entries):
		bashCommand = "ipset add test_set_v4 " + str(test_hit_entries_v4[i])
		process = subprocess.Popen(bashCommand.split(), stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
		bashCommand = "ipset add test_set_v6 " + str(test_hit_entries_v6[i])
		process = subprocess.Popen(bashCommand.split(), stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

	# Start separate threads for inserting
	insert_thread_v4 = threading.Thread(target=insert_random_ip_into_ipset_v4, args=(continue_inserting, total_inserts))
	insert_thread_v6 = threading.Thread(target=insert_random_ip_into_ipset_v6, args=(continue_inserting, total_inserts))
	inserting_time_start = time.time()
	insert_thread_v4.start()
	insert_thread_v6.start()

	query_times_v4 = []
	query_times_v6 = []

	# For now, do equal amounts of hits and misses
	ipv4_flag = True
	for i in range(0, num_test_hit_entries * 2):
		if ipv4_flag:
			if i % 2 == 0:
				bashcommand = "ipset test test_set_v4 " + str(test_hit_entries_v4[i // 2])
			else:
				bashcommand = "ipset test test_set_v4 " + str(IPv4Address(random.getrandbits(32)))
			start = time.time()
			process = subprocess.Popen(bashCommand.split(), stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
			end = time.time()
			query_times_v4.append(end - start)	
			ipv4_flag = False
		else:
			if i % 2 == 0:
				bashcommand = "ipset test test_set_v6 " + str(test_hit_entries_v6[i // 2])
			else:
				bashcommand = "ipset test test_set_v6 " + str(IPv6Address(random.getrandbits(128)))
			start = time.time()
			process = subprocess.Popen(bashCommand.split(), stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
			end = time.time()
			query_times_v6.append(end - start)	
			ipv4_flag = True
		time.sleep(0.25)
		sys.stdout.write("\rProcessed %d / %d queries after %d inserts" % (i * 2 + 2, 4 * num_test_hit_entries, total_inserts[0]))
		sys.stdout.flush()	

	# Join insert thread
	continue_inserting[0] = False
	inserting_time_end = time.time()
	insert_thread_v4.join()
	insert_thread_v6.join()

	# Lastly delete test ipset
	bashCommand = "ipset destroy test_set_v4"
	process = subprocess.Popen(bashCommand.split(), stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
	bashCommand = "ipset destroy test_set_v6"
	process = subprocess.Popen(bashCommand.split(), stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

	# Print results
	print("\nAverage query time for IPv4 query during inserts was %.16f" % (np.mean(query_times_v4)))
	print("Average query time for IPv6 query during inserts was %.16f" % (np.mean(query_times_v6)))
	print("Average number of inserts per second was %.16f" % (total_inserts[0] / (inserting_time_end - inserting_time_start)))

else:
	exit(1)

