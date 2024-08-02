#!/bin/bash

IP="192.168.1.71"
#IP="192.168.1.195"

output=$(curl --silent --request GET --header "Content-Type: application/json"  http://${IP}/status/device/0)
echo "Get device id 0: ${output}"

sleep 2

output=$(curl --silent --request POST --header "Content-Type: application/json" --data '{"type":10}' http://${IP}/config/device/add)
echo "Add device id: ${output}"

sleep 2

output=$(curl --silent --request POST --header "Content-Type: application/json" --data '{"pin":2,"mode":1,"name":"onboard led","scheduler":10,"timer":2}' http://${IP}/config/device/0)
echo "Configure pin 2: ${output}"

sleep 2

output=$(curl --silent --request POST --header "Content-Type: application/json"  http://${IP}/device/set/0/enabled)
echo "Enable device: ${output}"
output=$(curl --silent --request GET --header "Content-Type: application/json"  http://${IP}/status/device/0)
echo "Get device id 0: ${output}"

sleep 62

#output=$(curl --silent --request POST --header "Content-Type: application/json"  http://${IP}/device/set/0/on )
#echo "Set device ON: ${output}"
#output=$(curl --silent --request GET --header "Content-Type: application/json"  http://${IP}/status/device/0)
#echo "Get device id 0: ${output}"

sleep 2

#output=$(curl --silent --request POST --header "Content-Type: application/json"  http://${IP}/device/set/0/off )
#echo "Set device OFF: ${output}"
#output=$(curl --silent --request GET --header "Content-Type: application/json"  http://${IP}/status/device/0)
#echo "Get device id 0: ${output}"

sleep 2

output=$(curl --silent --request POST --header "Content-Type: application/json"  http://${IP}/device/set/0/disable)
echo "Disable device: ${output}"
output=$(curl --silent --request GET --header "Content-Type: application/json"  http://${IP}/status/device/0)
echo "Get device id 0: ${output}"

sleep 2

output=$(curl --silent --request POST --header "Content-Type: application/json"  http://${IP}/device/del/0 )
echo "Delete device id 0: ${output}"

sleep 2

output=$(curl --silent --request GET --header "Content-Type: application/json"  http://${IP}/status/devices )
echo "List all devices: ${output}"

exit


