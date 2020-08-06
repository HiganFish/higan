#!/bin/bash
flatname=$1
roomname=$2

response=$(curl -s --location --request POST 'http://xxx.xxx.xxx.xxx/hydropower/index.php?m=PayWeChat&c=IndexKd&a=find&schoolcode=13579' \
    --header 'X-Requested-With:  XMLHttpRequest' \
    --header 'Content-Type:  application/x-www-form-urlencoded' \
    --data-raw "flatname=${flatname}&roomname=${roomname}")

degree=$(echo "$response" | sed -r 's/(.*dushu\"\:)([0-9.]*)(.*)/\2/')

date=$(date '+%Y/%m/%d %H:%M:%S')

echo -e "$date\t$degree" >> "/usr/local/web/df-5100/df-$flatname$roomname.txt"
echo -e "$flatname$roomname\t$data\t$degree"
