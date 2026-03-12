#!/bin/bash

# ERGO M575SP の not connected なデバイスをすべてアンペアリングする

addresses=$(blueutil --paired \
  | grep 'not connected' \
  | grep 'ERGO M575SP' \
  | awk -F'address: ' '{print $2}' \
  | awk -F',' '{print $1}')

if [ -z "$addresses" ]; then
  echo "対象デバイスなし"
  exit 0
fi

count=$(echo "$addresses" | wc -l | tr -d ' ')
echo "削除対象: ${count} 件"

echo "$addresses" | while read -r addr; do
  echo "Unpairing: $addr"
  blueutil --unpair "$addr"
done

echo "完了"
