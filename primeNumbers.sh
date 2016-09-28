#!/bin/bash

total=0
lower=2
upper=131072
startTime=`date +%s`

for (( i=2;i<$upper;i++ ))
do
   isprime=1
   for (( j=2;j<i;j++ ))
     do
     moduloOp=$(( $i % $j ))
     if [ $moduloOp = 0 ]
        then
          isprime=0
          break
     fi
     done
  total=`expr $total + $isprime`
done
echo $total
endTime=`date +%s`
elapsedTime=`expr $endTime - $startTime`
echo $elapsedTime

