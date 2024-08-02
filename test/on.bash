#!/bin/bash

IP=$1
PORT=$2
[ "$IP" == "" ] && { echo "Fatal: missing IP address"; exit; } || echo "ip: $IP" 
[ "$PORT" == "" ] && { echo "Fatal: missing port"; exit; } || echo "port: $PORT" 

output=$(curl --silent --request POST --header "Content-Type: application/json"  http://${IP}/device/set/${PORT}/on )
echo "Set device ON: ${output}"

exit


