#!/bin/bash

dataPath=../data_gene/data_gen_results/dataGen_regex_javaclassconcate100.txt
sum_time=0
sum_pos=0
# Read each line of the replacements file and replace "REGEX" in target.cpp with the current line
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
done < /home/fudoyusea/tyy_work/data_real/lyl/java_classes_trans_for_ragel.data

echo "matching total time:$sum_time ms"
echo "matching total pos:$sum_pos"

