# GCC flags from the syllabus
CFLAGS = -std=c99 -Wall -pedantic -Wextra -Werror -O2 -Wstrict-prototypes -Wold-style-definition -g

CFILES = main.c 21tenmo.c
OFILES = main.o 21tenmo.o

21TENMO_OFILES = main.o 21tenmo.o utility.o
TESTS_OFILE = tests.o utility.o

CHECK_TEST_FILES = suites/21tenmo_suite.o suites/test_utils.o utility.o 21tenmo.c

# Uses pkg-config to retrieve package information about check -- Unit Testing Framework
CHECK_LIBS = $(shell pkg-config --cflags --libs check)

.PHONY: run-tests 21tenmo run-gdb check-tests main-tests ofiles clean

# Compiles & Links Student Code - Students must execute their own code "./21tenmo ......etc"
21tenmo: ofiles
	./verify.sh
	gcc $(CFLAGS) $(21TENMO_OFILES) -o 21tenmo

# Execute Tests
run-tests: main-tests check-tests
	./main-tests test_invalid_args
	./main-tests test_invalid_name
	./main-tests test_database_full

# Testing with check library
check-tests:
	gcc $(CFLAGS) $(CHECK_TEST_FILES) $(CHECK_LIBS) -o check_tests
	./check_tests $(TEST)

# Compiles Main Method Tests
main-tests: 21tenmo
	./verify.sh
	gcc $(TESTS_OFILE) -o main-tests

# Only able to gdb the check_tests
run-gdb: check-tests
	CK_FORK=no gdb --args ./check_tests $(TEST)

ofiles:
	gcc $(CFLAGS) $(CFILES) -c

clean:
	rm -f 21tenmo main-tests check_tests $(OFILES)
