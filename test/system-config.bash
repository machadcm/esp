#!/bin/bash

IP=$1
[ "$IP" == "" ] && { echo "Fatal: missing IP address"; exit; } || echo "ip: $IP" 

output=$(curl --silent --request GET --header "Content-Type: application/json"  http://${IP}/status/system )
echo "List all devices: ${output}"

curl --request POST --header "Content-Type: application/json" --data '{"name":"Development device","target":"http://192.168.1.157:8080"}' http://${IP}/config/system; echo

sleep 2

curl --request POST --header "Content-Type: application/json" http://${IP}/system/save; echo

exit


