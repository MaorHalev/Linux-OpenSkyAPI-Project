#!/bin/bash

rm -r flightsDB 2>/dev/null
mkdir -p flightsDB
cd flightsDB
now=$(date +%s)
yesterday=$(date --date="yesterday" +"%s")
PARAMS="$@"
for airport in $PARAMS
do
mkdir "$airport"
cd "$airport"
curl -u "Halevi:0121596" -s "https://opensky-network.org/api/flights/departure?airport=$airport&begin=$yesterday&end=$now" > dptsTmp
sed -i 's/,/\n/g' dptsTmp 
awk -F: '{print $2 }' dptsTmp > "$airport.dpt" 
sed -i -z 's/\n/,/g' "$airport.dpt" 
sed -i 's/}/\n/g' "$airport.dpt" 
sed -i 's/"//g' "$airport.dpt" 
sed -i 's/[ \t]//' "$airport.dpt"
sed -i '$ d' "$airport.dpt" 
sed -i 's/^[,]//g' "$airport.dpt" 
rm dptsTmp
curl -u "Halevi:0121596" -s "https://opensky-network.org/api/flights/arrival?airport=$airport&begin=$yesterday&end=$now" > arvTmp
sed -i 's/,/\n/g' arvTmp 
awk -F: '{print $2 }' arvTmp > "$airport.arv" 
sed -i -z 's/\n/,/g' "$airport.arv" 
sed -i 's/}/\n/g' "$airport.arv" 
sed -i 's/"//g' "$airport.arv" 
sed -i 's/[ \t]//' "$airport.arv" 
sed -i '$ d' "$airport.arv" 
sed -i 's/^[,]//g' "$airport.arv" 
rm arvTmp
cd ..
done