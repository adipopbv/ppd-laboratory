#!/usr/bin/env bash

echo "$1" > "$2"

NUMBER="$(($RANDOM%10))"
while [ $NUMBER -eq 0 ]; do
  NUMBER="$(($RANDOM%10))";
done

for _ in $(seq 2 "$1"); do
  NUMBER+="$(($RANDOM % 10))";
done
echo $NUMBER >> "$2";
