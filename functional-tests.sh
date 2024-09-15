#!/usr/bin/env bash
echo "When program called without arguments error message printed to stdout"
output="$(./nessie)"
expected="Error: no arguments provided"
[ "$output" = "$expected" ]  && echo "pass" || (echo "fail" && echo $output && echo $expected)

echo "When program called with more than two arguments error message printed to stdout"
output="$(./nessie burgle fish)"
expected="Error: too many arguments, please specify just one file"
[ "$output" = "$expected" ]  && echo "pass" || (echo "fail" && echo $output && echo $expected)

echo "When program called with invalid file error message printed to stdout"
output="$(./nessie coconuttree.txt)"
expected="Error: file could not be opened"
[ "$output" = "$expected" ]  && echo "pass" || (echo "fail" && echo $output && echo $expected)

echo "When program called with file not in iNES format error message printed to stdout"
output="$(./nessie ./roms/bubbles.nes)"
expected="Error: file not in iNES format"
[ "$output" = "$expected" ]  && echo "pass" || (echo "fail" && echo $output && echo $expected)

echo "When program called with iNES file format number of mapper, PRG and CHR banks and mirroring printed to stdout"
output="$(./nessie ./roms/nestest.nes)"
expected="ROM file contains 1 PRG and 1 CHR banks
Vertical arrangement, horizontal mirrored
Mapper number is: 0"
[ "$output" = "$expected" ]  && echo "pass" || (echo "fail" && echo $output && echo $expected)

