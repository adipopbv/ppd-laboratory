#!/bin/env bash

EXECUTABLE_PATH=$1
INPUT_DIR=$2
OUTPUT_FILE=$3
THREADS=$4
RETRIES=$5
INDEX=1

RESULTS=()

if [ -f "$OUTPUT_FILE" ]; then
    echo "$OUTPUT_FILE exists. Output will be appended"
    INDEX=$(tail -n1 $OUTPUT_FILE | cut -d"," -f1)
    INDEX=$((INDEX+1))
else 
    echo "$OUTPUT_FILE does not exist. A new file will be createdi"
    echo "trial_no,average_time,threads" > $OUTPUT_FILE
fi

for i in $(seq 1 $RETRIES); do 
	echo "Trial no:" $i "for" $EXECUTABLE_PATH "with" $THREADS "threads."
      
	result=$(/usr/bin/java $EXECUTABLE_PATH $INPUT_DIR $THREADS)
        RESULTS+=($result)
done

total=0
sum=0
for i in "${RESULTS[@]}"
do
	sum=`expr $sum + $i`
        total=`expr $total + 1`
done
average=`expr $sum / $total`
echo $INDEX","$average","$THREADS >> $OUTPUT_FILE
