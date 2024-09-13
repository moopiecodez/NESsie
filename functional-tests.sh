#!/usr/bin/env bash
echo "When program called without arguments error message printed to stdout"
output="$(./nessie)"
expected="Error: no arguments provided"
[ "$output" = "$expected" ]  && echo "pass" || echo "fail"

echo "When program called withmore than two arguments error message printed to stdout"
output="$(./nessie burgle fish)"
expected="Error: too many arguments, please specify just one file"
[ "$output" = "$expected" ]  && echo "pass" || echo "fail"

