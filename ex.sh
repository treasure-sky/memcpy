#!/bin/bash

FILENAME="/proc/cpuinfo"

echo "" > log_cur
echo "" > log_old
echo "" > log_movs
echo "" > log_custom

for ((i=0;i<10;i++))
do
        ./cur ${FILENAME} 	>> log_cur
        ./old ${FILENAME} 	>> log_old
        ./movs ${FILENAME} 	>> log_movs
        ./custom ${FILENAME} 	>> log_custom
        done


echo "" > RESULT
echo "log_old" >> RESULT
cat ./log_old |grep -i elapsed  >> RESULT
echo "log_cur" >> RESULT
cat ./log_cur |grep -i elapsed  >> RESULT
echo "log_movs" >> RESULT
cat ./log_movs |grep -i elapsed  >> RESULT
echo "log_custom" >> RESULT
cat ./log_custom |grep -i elapsed  >> RESULT

