#!/bin/bash
now=$(date +%s)
yesterday=$(date --date="yesterday" +"%s")
curl -u "Halevi:0121596" -s "https://opensky-network.org/api/flights/departure?airport=$1&begin=$yesterday&end=$now" > dpts
sed -i 's/[{}]/\n/g' dpts   #delete {}
sed -i 's/\[//g' dpts       #delete [
sed -i 's/\]//g' dpts       #delete ]
sed -i 's/^[ \t,]*//' dpts  #delete line starts with , \t
awk -F',;' '{ for (i=2;i<=NF;i+=2) print $i }' dpts

sed -i '/^$/d' dpts         #delete empty lines