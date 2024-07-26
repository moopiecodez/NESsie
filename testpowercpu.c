#include <stdio.h>
#include <check.h>
#include "testAll.h"

START_TEST(test_Pwr) {
    cpu.P = cpu.A = cpu.X = cpu.Y = 0x12u;
    BYTE expectedP, expectedA, expectedX, expectedY;
    expectedP = 0x04u;
    expectedA = expectedX = expectedY = 0x00u;
    uint16_t expectedPC = 0xFFFCu;
    BYTE expectedS = 0xFFu;    

    power_cpu(&cpu);

    ck_assert_msg(cpu.P == expectedP, "incorrect P register value");
    ck_assert_msg(cpu.A == expectedA, "incorrect A register value");
    ck_assert_msg(cpu.X == expectedX, "incorrect X register value");
    ck_assert_msg(cpu.Y == expectedY, "incorrect Y register value");
    ck_assert_msg(cpu.PC == expectedPC, "incorrect PC register value");
    ck_assert_msg(cpu.S == expectedS, "incorrect S register value");
}
END_TEST

Suite *Power_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Power CPU");

    /*Core test case*/
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_Pwr);
    
    suite_add_tcase(s, tc_core);

    return s;
}
