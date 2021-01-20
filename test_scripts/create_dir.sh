#!/bin/bash

GEM5_BASE=~/gem5

for FILE in mm lfsr merge sieve; do
    # Random
    for FREQ in 1GHz 1.5GHz 2GHz 2.3GHz; do
        for ASSOC in 1 2 4 8 16; do
            mkdir -p ${GEM5_BASE}/m5out/result/${FILE}/Random_${FREQ}_${ASSOC}Assoc
        done
    done

    # NMRU
    for FREQ in 1GHz 1.5GHz 2GHz; do
        for ASSOC in 1 2 4 8; do
            mkdir -p ${GEM5_BASE}/m5out/result/${FILE}/NMRU_${FREQ}_${ASSOC}Assoc
        done
    done

    # LIP
    for FREQ in 1GHz 1.5GHz; do
        for ASSOC in 1 2 4 8; do
            mkdir -p ${GEM5_BASE}/m5out/result/${FILE}/LIP_${FREQ}_${ASSOC}Assoc
        done
    done
done