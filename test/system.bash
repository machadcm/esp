#!/bin/bash

IP=$1
[ "$IP" == "" ] && { echo "Fatal: missing IP address"; exit; } || echo "ip: $IP" 

output=$(curl --silent --request GET --header "Content-Type: application/json"  http://${IP}/status/system )
echo "List all devices: ${output}"

exit


