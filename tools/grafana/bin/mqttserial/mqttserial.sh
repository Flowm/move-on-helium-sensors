#!/usr/bin/env bash

topic=sensors-raw

if [ -n "$1" ]; then
    topic=$1
fi

mosquitto_sub -t $topic
