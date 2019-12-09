#!/bin/bash

# TESTS FOR PART 2
####################################################################################################   SORTED

./generate.sh ./data/henry_spell_dict testing_dict.txt testing_query.txt 10 10 sorted 100
time ./speller_darray -d testing_dict.txt -v -m 0 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 1 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 2 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 3 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 4 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 5 testing_query.txt

./generate.sh ./data/henry_spell_dict testing_dict.txt testing_query.txt 1000 1000 sorted 100
time ./speller_darray -d testing_dict.txt -v -m 0 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 1 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 2 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 3 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 4 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 5 testing_query.txt

./generate.sh ./data/henry_spell_dict testing_dict.txt testing_query.txt 10000 10000 sorted 100
time ./speller_darray -d testing_dict.txt -v -m 0 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 1 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 2 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 3 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 4 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 5 testing_query.txt
clear ##############################################################################################   RANDOM
./generate.sh ./data/henry_spell_dict testing_dict.txt testing_query.txt 10 10 random 100
time ./speller_darray -d testing_dict.txt -v -m 0 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 1 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 2 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 3 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 4 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 5 testing_query.txt

./generate.sh ./data/henry_spell_dict testing_dict.txt testing_query.txt 1000 1000 random 100
time ./speller_darray -d testing_dict.txt -v -m 0 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 1 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 2 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 3 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 4 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 5 testing_query.txt

./generate.sh ./data/henry_spell_dict testing_dict.txt testing_query.txt 10000 10000 random 100
time ./speller_darray -d testing_dict.txt -v -m 0 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 1 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 2 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 3 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 4 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 5 testing_query.txt
clear ##############################################################################################   REVERSED
./generate.sh ./data/henry_spell_dict testing_dict.txt testing_query.txt 10 10 reverse 100
time ./speller_darray -d testing_dict.txt -v -m 0 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 1 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 2 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 3 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 4 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 5 testing_query.txt

./generate.sh ./data/henry_spell_dict testing_dict.txt testing_query.txt 1000 1000 reverse 100
time ./speller_darray -d testing_dict.txt -v -m 0 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 1 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 2 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 3 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 4 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 5 testing_query.txt

./generate.sh ./data/henry_spell_dict testing_dict.txt testing_query.txt 10000 10000 reverse 100
time ./speller_darray -d testing_dict.txt -v -m 0 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 1 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 2 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 3 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 4 testing_query.txt
time ./speller_darray -d testing_dict.txt -v -m 5 testing_query.txt
