#!/bin/bash

dataPath=../data_gene/data_gen_results/dataGen_regex_javaclassconcate50000.txt
sum_time=0
total_pos=0
test_times=100.0
# Read each line of the replacements file and replace "REGEX" in target.cpp with the current line
for i in `seq 100`
do
    sum_pos=0
    while read line; do
        cp ragel_test_c.rl ragel_test_c_temp.rl
        sed -i "s/REGEX/$line/g" ragel_test_c_temp.rl
        ragel -G2 ragel_test_c_temp.rl
        output=$(gcc ragel_test_c_temp.c -o ragel_test_G2 && ./ragel_test_G2 $dataPath)
        result_time=$(echo "$output" | head -n 1)
        result_pos=$(echo "$output" | tail -n 1)
        sum_time=$(echo "$sum_time + $result_time" | bc)
        sum_pos=$(echo "$sum_pos + $result_pos" | bc)
        echo "scanning"
    done < /home/fudoyusea/tyy_work/regex/single_char_concate_ragel.data
    total_pos=$sum_pos
done

average_time=$(echo "scale=6;$sum_time / $test_times" | bc)

echo "matching total time:$sum_time ms"
echo "matching average time:$average_time ms"
echo "matching total pos:$total_pos"

