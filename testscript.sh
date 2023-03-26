#!/bin/bash

rm -r flightsDB 2>/dev/null
mkdir -p flightsDB
cd flightsDB
now=$(date +%s)
yesterday=$(date --date="yesterday" +"%s")
curl -u "Halevi:0121596" -s "https://opensky-network.org/api/flights/departure?airport=$1&begin=$yesterday&end=$now" > dptsTmp
sed -i 's/,/\n/g' dptsTmp 
awk -F: '{print $2 }' dptsTmp > "$1.dpt" 
sed -i -z 's/\n/,/g' "$1.dpt" 
sed -i 's/}/\n/g' "$1.dpt" 
sed -i 's/"//g' "$1.dpt" 
sed -i 's/[ \t]//' "$1.dpt" 
sed -i '$ d' "$1.dpt" 
sed -i 's/^[,]//g' "$1.dpt" 
rm dptsTmp