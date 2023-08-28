#!/bin/bash

if [ -z "$1" ]
 then
  echo "Usage: ./fileDownload.sh <SaveFileName.csv>"
 else
  curl -XPOST http://104.131.85.212:8086/api/v2/query?org=minesformula\&bucket=daqactual -sS \
  -H 'Authorization: Token RD4WZeqTA2m7XJYeGDvbHewr7QXrSDMITW9C6OINQFxl-_DRlpx9erxwMUvB8CPktKaXVxY6YMbXHHqpEvaXdg=='\
  -H 'Accept:application/csv' \
  -H 'Content-type:application/vnd.flux' \
  -H 'Connection: close' \
  -d 'from(bucket:"daqactual")
        |> range(start:-7d)' > ./csv_raw/"$1.csv"
  echo "File Downloaded to ./csv_raw/$1.csv"
fi