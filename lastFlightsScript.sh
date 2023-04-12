#!/bin/bash

rm -r flightsDB 2>/dev/null
mkdir -p flightsDB
cd flightsDB
now=$(date +%s)
yesterday=$(date --date="yesterday" +"%s")
PARAMS="$@"
for airport in $PARAMS;
do
dptUrl="https://opensky-network.org/api/flights/departure?airport=$airport&begin=$yesterday&end=$now";
arvUrl="https://opensky-network.org/api/flights/arrival?airport=$airport&begin=$yesterday&end=$now";
response=$(curl -s -o /dev/null -w "%{http_code}" $dptUrl)
#we will check if page exists
if [ $response == "200" ]; then
    mkdir "$airport"
cd "$airport"
curl -u "Halevi:0121596" -s $dptUrl | sed 's/,/\n/g' | awk -F: '{print $2 }' | sed -z 's/\n/,/g' | sed 's/}/\n/g' | sed 's/[][ \t"]//g' | sed 's/^[,]//g' > "$airport.dpt"
curl -u "Halevi:0121596" -s $arvUrl | sed 's/,/\n/g' | awk -F: '{print $2 }' | sed -z 's/\n/,/g' | sed 's/}/\n/g' | sed 's/[][ \t"]//g' | sed 's/^[,]//g'  > "$airport.arv"
echo "icao24,firstSeen,estDepartueAirport,lastSeen,estArrivalAirport,callsign,estDepartueAirportHorizDistance,estDepartureAirportVertDistance,estArrivalAirportHorizDistance,estArrivalAirportVertDistance,departueAirportCandidatesCount,arrivalAirportCandidatesCount" | cat - "$airport.dpt" > tmp && mv tmp "$airport.dpt"
echo "icao24,firstSeen,estDepartueAirport,lastSeen,estArrivalAirport,callsign,estDepartueAirportHorizDistance,estDepartureAirportVertDistance,estArrivalAirportHorizDistance,estArrivalAirportVertDistance,departueAirportCandidatesCount,arrivalAirportCandidatesCount" | cat - "$airport.arv" > tmp && mv tmp "$airport.arv"
cd ..
else
    echo "ICAO $airport does not exist";
fi
done