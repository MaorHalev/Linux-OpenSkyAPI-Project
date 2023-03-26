#!/bin/bash

rm -r flightsDB 2>/dev/null
mkdir -p flightsDB
cd flightsDB
now=$(date +%s)
yesterday=$(date --date="yesterday" +"%s")
PARAMS="$@"
for code in $PARAMS
do
mkdir "$code"
cd "$code"
curl -u "Halevi:0121596" -s "https://opensky-network.org/api/flights/departure?airport=$code&begin=$yesterday&end=$now" > dptsTmp
sed -i 's/,/\n/g' dptsTmp 
awk -F: '{print $2 }' dptsTmp > "$code.dpt" 
sed -i -z 's/\n/,/g' "$code.dpt" 
sed -i 's/}/\n/g' "$code.dpt" 
sed -i 's/"//g' "$code.dpt" 
sed -i 's/[ \t]//' "$code.dpt" 
sed -i '$ d' "$code.dpt" 
sed -i 's/^[,]//g' "$code.dpt" 
rm dptsTmp
curl -u "Halevi:0121596" -s "https://opensky-network.org/api/flights/arrival?airport=$code&begin=$yesterday&end=$now" > arvTmp
sed -i 's/,/\n/g' arvTmp 
awk -F: '{print $2 }' arvTmp > "$code.arv" 
sed -i -z 's/\n/,/g' "$code.arv" 
sed -i 's/}/\n/g' "$code.arv" 
sed -i 's/"//g' "$code.arv" 
sed -i 's/[ \t]//' "$code.arv" 
sed -i '$ d' "$code.arv" 
sed -i 's/^[,]//g' "$code.arv" 
rm arvTmp
cd ..
done