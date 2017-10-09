#!/bin/bash

tides=3
if [ ! -z $1 ]; then
	tides=$2
fi

# 12 = SIGUSR2 TIDE RISE
for i in $(seq 1 $tides)
do
	sleep 2
	echo "TIDE RISE kill -12 $1"
	kill -12 $1
done

sleep 10

# 31 SIGUNUSED
for i in $(seq 1 $tides)
do
	sleep 2
	echo "TIDE DECREASE kill -31 $1"
	kill -31 $1
done

