#!/bin/bash

PROCESSES=()

./build/NodeEmulator ./configs/receiver0.properties &
PROCESSES+=($!)

./build/NodeEmulator ./configs/requester.properties &
PROCESSES+=($!)


read -p "Press enter to continue"

kill $(ps -A | grep Node | cut -d " " -f 2)





