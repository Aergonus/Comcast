#!/bin/bash
# Basch Script to run Comcast
clear 

#echo "Cleaning and Rebuilding Comcast"
#make clean
#make

echo "Welcome to Comcast (´·w·`)"

echo "Running Test Case 0"
./Comcast.exe -i ./input/test_case_0.json -o TC0_Output.csv -d TC0_debug.csv

echo "Running Test Case 1"
./Comcast.exe -i ./input/test_case_1.json -o TC1_Output.csv -d TC1_debug.csv

echo "Running Test Case 2"
./Comcast.exe -i ./input/test_case_2.json -o TC2_Output.csv -d TC2_debug.csv

echo "Comcast has successfully extorted you for your $ $ $"
echo "If you would like to donate more to Comcast please execute"
echo "		./Comcast.exe [-i input_file] [-o output_file] [-d debug_log_file]"
echo "Goodbye."