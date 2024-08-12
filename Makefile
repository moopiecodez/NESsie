#Nessie makefile
CC = gcc
CFLAGS = -Wall
TESTFLAGS = -lcheck -lm -lsubunit -pthread -lrt
TESTSUITES = testpowercpu.o	testArithmetic.o testBranch.o testFlagChanges.o\
			testINC-DEC.o testJump.o testLDA.o testLogical.o testShifts.o\
			testStack.o testStore.o testSysFunctions.o testTransfer.o

.PHONY: clean

loadRom: loadROM.c
	$(CC) $(CFLAGS) loadROM.c -o $@

testInstructions: testAllInstructions.o cpu.o $(TESTSUITES)
	$(CC) $(CFLAGS) $^ -o $@ $(TESTFLAGS)

testAllInstructions.o: testAllInstructions.c $(TESTSUITES)
	$(CC) $(CFLAGS) -c $< -o $@

$(TESTSUITES) cpu.o: %.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -f testInstructions loadRom *.o
