#Nessie makefile
CC = gcc
CFLAGS = -Wall -Wpedantic
TESTFLAGS = -lcheck -lm -lsubunit -pthread -lrt
TESTSUITES = testpowercpu.o	testArithmetic.o testBranch.o testFlagChanges.o\
			testINC-DEC.o testJump.o testLDA.o testLogical.o testShifts.o\
			testStack.o testStore.o testSysFunctions.o testTransfer.o

.PHONY: clean

nessie: nessie.c cpu.c cartridge.c bus.c
	$(CC) $(CFLAGS) nessie.c cpu.c cartridge.c bus.c -o $@

loadRom: loadROM.c
	$(CC) $(CFLAGS) loadROM.c -o $@

testcycles: testCycleFunctions.o
	$(CC) $(CFLAGS) $^ -o $@ $(TESTFLAGS)

testInstructions: testAllInstructions.o $(TESTSUITES)
	$(CC) $(CFLAGS) $^ -o $@ $(TESTFLAGS)

testCycleFunctions.o: testCycleFunctions.c
	$(CC) $(CFLAGS) -c $< -o $@

testAllInstructions.o: testAllInstructions.c $(TESTSUITES)
	$(CC) $(CFLAGS) -c $< -o $@

$(TESTSUITES) cpu.o: %.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -f testInstructions testcycles loadRom nessie *.o
