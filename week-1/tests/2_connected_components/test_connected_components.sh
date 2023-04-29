#!/bin/bash

gcc -g -pipe -O0 -std=c11 ../../../week-1/connected_components.c -lm &&
for i in {01..03}
do
    result=$(cat cases/$i | ./a.out)
    test=$(cat cases/$i.a)
    if [ "$result" != "$test" ]; then
        echo the case $i failed.
        echo $(cat cases/$i)
        echo your result =\> $result
        echo test result =\> $test
        printf "\n"
        exit
    else
        echo passed $i
    fi
done
echo All test passed.
