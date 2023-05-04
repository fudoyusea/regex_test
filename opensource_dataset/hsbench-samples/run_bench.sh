#!/bin/sh -e

# Script to run Hyperscan performance measurements. Pass in the path to your
# `hsbench` binary as an argument.

if [ $# -ne 1 ]; then
    echo "Usage: ./run_bench.sh <hsbench binary>"
    exit 1
fi

HSBENCH_BIN=$1

if [ ! -e ${HSBENCH_BIN} ]; then
    echo "Can't find hsbench binary: ${HSBENCH_BIN}"
    exit 1
fi


echo "\n*** Snort literals against HTTP traffic, block mode.\n"
taskset 1 ${HSBENCH_BIN} -e pcre/snort_literals -c corpora/alexa200.db -N

echo "\n*** Snort PCREs against HTTP traffic, block mode.\n"
taskset 1 ${HSBENCH_BIN} -e pcre/snort_pcres -c corpora/alexa200.db -N
echo

echo "\n*** Teakettle synthetic patterns against Gutenberg text, streaming mode.\n"
taskset 1 ${HSBENCH_BIN} -e pcre/teakettle_2500 -c corpora/gutenberg.db
echo
