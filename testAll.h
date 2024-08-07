#include <check.h>
#include <stdint.h>
#include "cpu.h"

extern CPU cpu;
extern BYTE memory[];

void setup(void);
void teardown(void);

Suite *Arithmetic_suite(void);
Suite *Branch_suite(void);
Suite *Flag_suite(void);
Suite *INC_DEC_suite(void);
Suite *Jump_suite(void);
Suite *Load_suite(void);
Suite *Logical_suite(void);
Suite *Power_suite(void);
Suite *Shift_suite(void);
Suite *Stack_suite(void);
Suite *Store_suite(void);
Suite *System_suite(void);
Suite *Transfer_suite(void);
