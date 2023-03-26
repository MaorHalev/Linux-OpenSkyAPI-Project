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
curl -u "Halevi:0121596" -s "https://opensky-network.org/api/flights/departure?airport=$airport&begin=$yesterday&end=$now" | sed 's/,/\n/g' | awk -F: '{print $2 }' | sed -z 's/\n/,/g' | sed 's/}/\n/g' | sed 's/[][ \t"]//g' | sed 's/^[,]//g' > "$airport.dpt"
curl -u "Halevi:0121596" -s "https://opensky-network.org/api/flights/arrival?airport=$airport&begin=$yesterday&end=$now" | sed 's/,/\n/g' | awk -F: '{print $2 }' | sed -z 's/\n/,/g' | sed 's/}/\n/g' | sed 's/[][ \t"]//g' | sed 's/^[,]//g'  > "$airport.arv"
echo "icao24,firstSeen,estDepartueAirport,lastSeen,estArrivalAirport,callsign,estDepartueAirportHorizDistance,estDepartureAirportVertDistance,estArrivalAirportHorizDistance,estArrivalAirportVertDistance,departueAirportCandidatesCount,arrivalAirportCandidatesCount" | cat - "$airport.dpt" > tmp && mv tmp "$airport.dpt"
echo "icao24,firstSeen,estDepartueAirport,lastSeen,estArrivalAirport,callsign,estDepartueAirportHorizDistance,estDepartureAirportVertDistance,estArrivalAirportHorizDistance,estArrivalAirportVertDistance,departueAirportCandidatesCount,arrivalAirportCandidatesCount" | cat - "$airport.arv" > tmp && mv tmp "$airport.arv"
cd ..
done