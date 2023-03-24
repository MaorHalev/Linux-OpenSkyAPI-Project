#!/bin/bash
now=$(date +%s)
yesterday=$(date --date="yesterday" +"%s")
curl -u "Halevi:0121596" -s "https://opensky-network.org/api/flights/departure?airport=$1&begin=$yesterday&end=$now" > file
sed -i 's/,/\n/g' file
awk -F: '{print $2}' file 
#sed -i '/^$/d' file         #delete empty lines