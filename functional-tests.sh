#!/usr/bin/env bash
echo "When program called without arguments error message printed to stdout"
output="$(./nessie)"
expected="Error: no arguments provided"
[ "$output" = "$expected" ]  && echo "pass" || echo "fail"
