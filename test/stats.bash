#!/bin/bash

./test/manyclients.bash > test/stats.txt
while read i;
do  
    (( sum+=i ));
    (( count+=1 )); 
done <./test/stats.txt;
echo $count 'connections handled';
echo 'Total time to handle connection with 5O clients is:' $((sum))µs
echo 'Average time to handle 1 connection is:' $((sum / count))µs
rm -r test/stats.txt test/client