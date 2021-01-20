#!/bin/bash

GEM5_BASE=  ~/gem5
SRC_DIR=    ${GEM5_BASE}/tests/test-progs/cs251a-microbench
RESULT_DIR= ${GEM5_BASE}/m5out/result
TARGET=     ${GEM5_BASE}configs/example/se.py

cd $GEM5_BASE

CACHE_TYPE=Random
for FILE in mm lfsr merge sieve; do
    for FREQ in 1GHz 1.5GHz 2GHz 2.3GHz; do
        for ASSOC in 1 2 4 8 16; do
            build/X86/gem5.opt \
                --outdir=${RESULT_DIR}/${FILE}/${CACHE_TYPE}_${FREQ}_${ASSOC}Assoc \
                ${TARGET} \
                --cmd=${SRC_DIR}/${FILE} \
                --cpu-type=DerivO3CPU \
                --l1d_size=64kB --l1i_size=64kB --l1d_assoc=${ASSOC} l1d_repl=${CACHE_TYPE} --caches \
                --l2_size=2MB --l2_repl=${CACHE_TYPE} --l2cache \
                --sys-clock=${FREQ} --cpu-clock=${FREQ} \
                --mem-type=DDR3_1600_8x8
        done
    done
done

CACHE_TYPE=NMRU
for FILE in mm lfsr merge sieve; do
    for FREQ in 1GHz 1.5GHz 2GHz; do
        for ASSOC in 1 2 4 8; do
            build/X86/gem5.opt \
                --outdir=${RESULT_DIR}/${FILE}/${CACHE_TYPE}_${FREQ}_${ASSOC}Assoc \
                ${TARGET} \
                --cmd=${SRC_DIR}/${FILE} \
                --cpu-type=DerivO3CPU \
                --l1d_size=64kB --l1i_size=64kB --l1d_assoc=${ASSOC} l1d_repl=${CACHE_TYPE} --caches \
                --l2_size=2MB --l2_repl=${CACHE_TYPE} --l2cache \
                --sys-clock=${FREQ} --cpu-clock=${FREQ} \
                --mem-type=DDR3_1600_8x8
        done
    done
done

CACHE_TYPE=LIP
for FILE in mm lfsr merge sieve; do
    for FREQ in 1GHz 1.5GHz; do
        for ASSOC in 1 2 4 8; do
            build/X86/gem5.opt \
                --outdir=${RESULT_DIR}/${FILE}/${CACHE_TYPE}_${FREQ}_${ASSOC}Assoc \
                ${TARGET} \
                --cmd=${SRC_DIR}/${FILE} \
                --cpu-type=DerivO3CPU \
                --l1d_size=64kB --l1i_size=64kB --l1d_assoc=${ASSOC} l1d_repl=${CACHE_TYPE} --caches \
                --l2_size=2MB --l2_repl=${CACHE_TYPE} --l2cache \
                --sys-clock=${FREQ} --cpu-clock=${FREQ} \
                --mem-type=DDR3_1600_8x8
        done
    done
done
