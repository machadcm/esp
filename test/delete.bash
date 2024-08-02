#!/bin/bash

IP=$1
PORT=$2
[ "$IP" == "" ] && { echo "Fatal: missing IP address"; exit; } || echo "ip: $IP" 
[ "$PORT" == "" ] && { echo "Fatal: missing port"; exit; } || echo "port: $PORT" 

output=$(curl --silent --request POST --header "Content-Type: application/json"  http://${IP}/device/set/${PORT}/disable)
echo "Disable device: ${output}"
output=$(curl --silent --request GET --header "Content-Type: application/json"  http://${IP}/status/device/${PORT})
echo "Get device id 0: ${output}"

sleep 2

output=$(curl --silent --request POST --header "Content-Type: application/json"  http://${IP}/device/del/${PORT} )
echo "Delete device id 0: ${output}"

sleep 2

output=$(curl --silent --request GET --header "Content-Type: application/json"  http://${IP}/status/devices )
echo "List all devices: ${output}"

exit


