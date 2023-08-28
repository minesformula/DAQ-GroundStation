#!/bin/bash
  
if [ -z "$1" ]
 then	
  echo "Usage: ./run_fileship <fileLocation> <OptionalCSVSaveName>"
  exit

 else
  echo "Shipping data from $1"
  #influx delete -b daqactual --start 2010-01-02T23:00:00Z --stop 2100-01-02T23:00:00Z
  curl --request POST http://104.131.85.212:8086/api/v2/delete?org=minesformula\&bucket=daqactual \
  --header 'Authorization: Token 0kv-XjSwMu0ZUAngy4UkM-DoemSKZz1YVGVpaHgDIdF2LI_eHS8-K82SC6_0oWL-CFd_PFFTjFEhfq4ACHJMFA==' \
  --header 'Content-Type: application/json' \
  --data '{
    "start": "2010-01-01T00:00:00Z",
    "stop": "2200-01-01T00:00:00Z"
  }'
  ./fileShipper $1

  if [ -z "$2" ]
  then
   echo "No Filename Specified for Raw CSV Data File"
  else
  curl -XPOST http://104.131.85.212:8086/api/v2/query?org=minesformula\&bucket=daqactual -sS \
  -H 'Authorization: Token 0kv-XjSwMu0ZUAngy4UkM-DoemSKZz1YVGVpaHgDIdF2LI_eHS8-K82SC6_0oWL-CFd_PFFTjFEhfq4ACHJMFA=='\
  -H 'Accept:application/csv' \
  -H 'Content-type:application/vnd.flux' \
  -d 'from(bucket:"daqactual")
        |> range(start:-7d)' > ./csv_raw/"$2.csv"

  echo "Wrote InfluxDB Raw CSV File to ./csv_raw/{$2}"
  fi
fi
