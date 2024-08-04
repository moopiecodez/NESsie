#Nessie makefile
CC = gcc
CFLAGS = -Wall
TESTFLAGS = -lcheck -lm -lsubunit -pthread -lrt

.PHONY: clean

loadRom: loadROM.c
	$(CC) $(CFLAGS) loadROM.c -o $@ 

testInstructions: testAllInstructions.o cpu.o
	$(CC) $(CFLAGS) testAllInstructions.o cpu.o testpowercpu.o testArithmetic.o testBranch.o testFlagChanges.o testINC-DEC.o testJump.o testLDA.o testLogical.o testShifts.o testStack.o testStore.o testSysFunctions.o testTransfer.o -o $@ $(TESTFLAGS)

testAllInstructions.o: testAllInstructions.c testpowercpu.o testArithmetic.o testBranch.o testFlagChanges.o testINC-DEC.o testJump.o testLDA.o testLogical.o testShifts.o testStack.o testStore.o testSysFunctions.o testTransfer.o
	$(CC) $(CFLAGS) -c testAllInstructions.c -o $@ 

testpowercpu.o: testpowercpu.c
	$(CC) $(CFLAGS) -c testpowercpu.c -o $@ 

testArithmetic.o: testArithmetic.c
	$(CC) $(CFLAGS) -c testArithmetic.c -o $@ 

testBranch.o: testBranch.c
	$(CC) $(CFLAGS) -c testBranch.c -o $@ 

testFetch.o: testFetch.c
	$(CC) $(CFLAGS) -c testFetch.c -o $@ 

testFlagChanges.o: testFlagChanges.c
	$(CC) $(CFLAGS) -c testFlagChanges.c -o $@  

testINC-DEC.o: testINC-DEC.c
	$(CC) $(CFLAGS) -c testINC-DEC.c -o $@  

testJump.o: testJump.c
	$(CC) $(CFLAGS) -c testJump.c -o $@  

testLDA.o: testLDA.c
	$(CC) $(CFLAGS) -c testLDA.c -o $@  

testLogical.o: testLogical.c
	$(CC) $(CFLAGS) -c testLogical.c -o $@  

testShifts.o: testShifts.c
	$(CC) $(CFLAGS) -c testShifts.c -o $@  

testStack.o: testStack.c
	$(CC) $(CFLAGS) -c testStack.c -o $@  

testStore.o: testStore.c
	$(CC) $(CFLAGS) -c testStore.c -o $@  

testSysFunctions.o: testSysFunctions.c
	$(CC) $(CFLAGS) -c testSysFunctions.c -o $@  

testTransfer.o: testTransfer.c
	$(CC) $(CFLAGS) -c testTransfer.c -o $@  

cpu.o: cpu.c
	$(CC) $(CFLAGS) -c cpu.c -o $@ 

clean:
# '-' at start of line means continue beyond error
	rm -f testInstructions loadRom *.o
