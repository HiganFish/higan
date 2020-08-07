#!/bin/bash
flatname=$1
roomname=$2
exoutput=$3


response=$(curl -s --location --request POST 'http://xxx.xxx.xxx.xxx/hydropower/index.php?m=PayWeChat&c=IndexKd&a=find&schoolcode=13579' \
    --header 'X-Requested-With:  XMLHttpRequest' \
    --header 'Content-Type:  application/x-www-form-urlencoded' \
    --data-raw "flatname=${flatname}&roomname=${roomname}")

degree=$(echo "$response" | sed -r 's/(.*dushu\"\:)([0-9.]*)(.*)/\2/')

date=$(date '+%Y/%m/%d %H:%M:%S')

printf "%-25s %6.2f  %s\n" "$date" $degree $exoutput >> "/usr/local/web/df-5100/df-$flatname$roomname.txt"

printf "%-15s %6.2f  %s" $flatname$roomname $degree $exoutput
