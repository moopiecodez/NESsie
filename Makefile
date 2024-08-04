#Nessie makefile

.PHONY: clean

loadRom: loadROM.c
	gcc loadROM.c -o $@ -Wall

testInstructions: testAllInstructions.o cpu.o
	gcc testAllInstructions.o cpu.o testpowercpu.o testArithmetic.o testBranch.o testFlagChanges.o testINC-DEC.o testJump.o testLDA.o testLogical.o testShifts.o testStack.o testStore.o testSysFunctions.o testTransfer.o -o $@ -lcheck -lm -lsubunit -pthread -lrt -Wall

testAllInstructions.o: testAllInstructions.c testpowercpu.o testArithmetic.o testBranch.o testFlagChanges.o testINC-DEC.o testJump.o testLDA.o testLogical.o testShifts.o testStack.o testStore.o testSysFunctions.o testTransfer.o
	gcc -c testAllInstructions.c -o $@ -lcheck -lm -lsubunit -pthread -lrt -Wall

testpowercpu.o: testpowercpu.c
	gcc -c testpowercpu.c -o $@ -lcheck -lm -lsubunit -pthread -lrt -Wall

testArithmetic.o: testArithmetic.c
	gcc -c testArithmetic.c -o $@ -lcheck -lm -lsubunit -pthread -lrt -Wall

testBranch.o: testBranch.c
	gcc -c testBranch.c -o $@ -lcheck -lm -lsubunit -pthread -lrt -Wall

testFetch.o: testFetch.c
	gcc -c testFetch.c -o $@ -lcheck -lm -lsubunit -pthread -lrt -Wall

testFlagChanges.o: testFlagChanges.c
	gcc -c testFlagChanges.c -o $@ -lcheck -lm -lsubunit -pthread -lrt -Wall

testINC-DEC.o: testINC-DEC.c
	gcc -c testINC-DEC.c -o $@ -lcheck -lm -lsubunit -pthread -lrt -Wall

testJump.o: testJump.c
	gcc -c testJump.c -o $@ -lcheck -lm -lsubunit -pthread -lrt -Wall

testLDA.o: testLDA.c
	gcc -c testLDA.c -o $@ -lcheck -lm -lsubunit -pthread -lrt -Wall

testLogical.o: testLogical.c
	gcc -c testLogical.c -o $@ -lcheck -lm -lsubunit -pthread -lrt -Wall

testShifts.o: testShifts.c
	gcc -c testShifts.c -o $@ -lcheck -lm -lsubunit -pthread -lrt -Wall

testStack.o: testStack.c
	gcc -c testStack.c -o $@ -lcheck -lm -lsubunit -pthread -lrt -Wall

testStore.o: testStore.c
	gcc -c testStore.c -o $@ -lcheck -lm -lsubunit -pthread -lrt -Wall

testSysFunctions.o: testSysFunctions.c
	gcc -c testSysFunctions.c -o $@ -lcheck -lm -lsubunit -pthread -lrt -Wall

testTransfer.o: testTransfer.c
	gcc -c testTransfer.c -o $@ -lcheck -lm -lsubunit -pthread -lrt -Wall

cpu.o: cpu.c
	gcc -c cpu.c -o $@ -Wall

clean:
# '-' at start of line means continue beyond error
	rm -f testInstructions loadRom *.o
