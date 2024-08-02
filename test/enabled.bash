#!/bin/bash

IP=$1
PORT=$2
PIN=$3
[ "$IP" == "" ] && { echo "Fatal: missing IP address"; exit; } || echo "ip: $IP" 
[ "$PORT" == "" ] && { echo "Fatal: missing port"; exit; } || echo "port: $PORT" 
[ "$PIN" == "" ] && { echo "Fatal: missing pin"; exit; } || echo "pin: $PIN" 


output=$(curl --silent --request POST --header "Content-Type: application/json" --data '{"pin":'${PIN}',"mode":"offup","name":"onboard led","scheduler":0,"timer":0}' http://${IP}/config/device/${PORT})
echo "Configure pin ${PIN} in ${PORT}: ${output}"

sleep 1

output=$(curl --silent --request POST --header "Content-Type: application/json"  http://${IP}/device/set/${PORT}/enabled)
echo "Enable device: ${output}"


