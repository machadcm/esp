#!/bin/bash

IP=$1
[ "$IP" == "" ] && { echo "Fatal: missing IP address"; exit; } || echo "IP:$IP" 


output=$(curl --silent --request POST --header "Content-Type: application/json" --data '{"type":10}' http://${IP}/config/device/add)
echo "Add device id: ${output}"


