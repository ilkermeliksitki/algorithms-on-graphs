#!/bin/bash

# compile the C code with debugging symbols
gcc -g -pipe -O0 -std=c11 ../../../week-3/bipartite.c -Wall -lm -o a.out &&

# define an array of test case numbers
test_cases=("01" "02" "03" "04" "05")

# colors
RED='\033[0;31m'
RED_BOLD='\033[1;31m'
GREEN_BOLD='\033[1;32m'
NC='\033[0m'    # No Color

# iterate over the test case files
for i in "${test_cases[@]}"; do
    # check that the test file exists or not
    if ! [[ -e cases/$i ]]; then
        echo "there is no test file called $i" 1>&2
        continue
    fi

    # execute the program using Valgrind and capture the output
    valgrind_output=$(valgrind --leak-check=full ./a.out < "cases/$i" 2>&1)

    # check if valgrind reported any memory leaks
    if [[ "$valgrind_output" =~ "All heap blocks were freed -- no leaks are possible" ]]; then
        memory_leak_info="${GREEN_BOLD}without memory leak${NC}"
    else
        memory_leak_info="${RED_BOLD}with memory leak${NC}"
    fi

    # check if valgrind reports any conditional jump based on uninitialised value(s)
    if ! [[ "$valgrind_output" =~ "Conditional jump or move depends on uninitialised value(s)" ]]; then
        conditional_jump=""
    else
        conditional_jump=" - ${RED_BOLD}with conditional jump based on uninitialised value${NC}"
    fi

    # check if valgrind reports any invalid write to "unallocated memory"
    if ! [[ "$valgrind_output" =~ "Invalid write of size" ]]; then
        invalid_write=""
    else
        invalid_write=" - ${RED_BOLD}invalid write to unallocated memory${NC}"
    fi
    
    # execute the program again and capture the program's output
    result=$(./a.out < "cases/$i")

    # read the expected output from the file
    expected=$(cat "cases/$i.a")

    # compare the result and the expected output
    # if there is a trailing space, it will appear as a red block character.
    if [ "$result" != "$expected" ]; then
        echo -e "Test case $i failed ${memory_leak_info}${conditional_jump}${invalid_write}"
        echo "Input:"
        cat "cases/$i"
        echo -e "\nYour result:"
        echo -e "$result" | sed -E "s/[[:space:]]+$/$(printf "${RED}█${NC}")/"
        echo -e "\nExpected result:"
        echo -e "$expected" | sed -E "s/[[:space:]]+$/$(printf "${RED}█${NC}")/"
        echo
        exit 1
    else
        echo -e "Test case $i passed ${memory_leak_info}${conditional_jump}${invalid_write}"
    fi
done

echo "All tests passed."

