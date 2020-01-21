#!/bin/sh

# Update suricata rules
#suricata-update
# Start cron
#crond
# Add cronjob
#crontab /etc/crontabs/suricata-update-cron

service mysql start
suricata -i enp5s0f1


#FOR DEMO PERFORMANCE REASONS, SURICATA IS RUNNING WITH NO RULES.

# NOTE THESE MESSAGES ARE EXPECTED
#This is Suricata version 5.0.2-dev (2c050187a 2020-01-20) running in SYSTEM mode
#<Warning> (ProcessSigFiles) -- [ERRCODE: SC_ERR_NO_RULES(42)] 
#- No rule files match the pattern /etc/suricata/rules/suricata.rules
#<Warning> (SigLoadSignatures) -- [ERRCODE: SC_ERR_NO_RULES_LOADED(43)] 
#- 1 rule files specified, but no rule was loaded at all!

#FOR PERFORMANCE REASONS, SURICATA IS RUNNING WITH NO RULES.
