#Nessie makefile

.PHONY: clean

loadRom: loadROM.c
	gcc loadROM.c -o loadRom -Wall

testInstructions: testAllInstructions.o cpu.o
	gcc testAllInstructions.o cpu.o testpowercpu.o testArithmetic.o testBranch.o testFlagChanges.o testINC-DEC.o testJump.o testLDA.o testLogical.o testShifts.o testStack.o testStore.o testSysFunctions.o testTransfer.o -o testInstructions -lcheck -lm -lsubunit -pthread -lrt -Wall

testAllInstructions.o: testAllInstructions.c testpowercpu.o testArithmetic.o testBranch.o testFlagChanges.o testINC-DEC.o testJump.o testLDA.o testLogical.o testShifts.o testStack.o testStore.o testSysFunctions.o testTransfer.o
	gcc -c testAllInstructions.c -o testAllInstructions.o -lcheck -lm -lsubunit -pthread -lrt -Wall
testpowercpu.o: testpowercpu.c
	gcc -c testpowercpu.c -o testpowercpu.o -lcheck -lm -lsubunit -pthread -lrt -Wall
testArithmetic.o: testArithmetic.c
	gcc -c testArithmetic.c -o testArithmetic.o -lcheck -lm -lsubunit -pthread -lrt -Wall
testBranch.o: testBranch.c
	gcc -c testBranch.c -o testBranch.o -lcheck -lm -lsubunit -pthread -lrt -Wall
testFetch.o: testFetch.c
	gcc -c testFetch.c -o testFetch.o -lcheck -lm -lsubunit -pthread -lrt -Wall
testFlagChanges.o: testFlagChanges.c
	gcc -c testFlagChanges.c -o testFlagChanges.o -lcheck -lm -lsubunit -pthread -lrt -Wall
testINC-DEC.o: testINC-DEC.c
	gcc -c testINC-DEC.c -o testINC-DEC.o -lcheck -lm -lsubunit -pthread -lrt -Wall
testJump.o: testJump.c
	gcc -c testJump.c -o testJump.o -lcheck -lm -lsubunit -pthread -lrt -Wall
testLDA.o: testLDA.c
	gcc -c testLDA.c -o testLDA.o -lcheck -lm -lsubunit -pthread -lrt -Wall
testLogical.o: testLogical.c
	gcc -c testLogical.c -o testLogical.o -lcheck -lm -lsubunit -pthread -lrt -Wall
testShifts.o: testShifts.c
	gcc -c testShifts.c -o testShifts.o -lcheck -lm -lsubunit -pthread -lrt -Wall
testStack.o: testStack.c
	gcc -c testStack.c -o testStack.o -lcheck -lm -lsubunit -pthread -lrt -Wall
testStore.o: testStore.c
	gcc -c testStore.c -o testStore.o -lcheck -lm -lsubunit -pthread -lrt -Wall
testSysFunctions.o: testSysFunctions.c
	gcc -c testSysFunctions.c -o testSysFunctions.o -lcheck -lm -lsubunit -pthread -lrt -Wall
testTransfer.o: testTransfer.c
	gcc -c testTransfer.c -o testTransfer.o -lcheck -lm -lsubunit -pthread -lrt -Wall

cpu.o: cpu.c
	gcc -c cpu.c -Wall

clean:
# '-' at start of line means continue beyond error
	rm -f testInstructions loadRom *.o
