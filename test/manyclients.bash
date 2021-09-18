#!/bin/bash

gcc test/client.c -o test/client
for N in {1..50}
do
    ./test/client localhost 8081 &
done
wait