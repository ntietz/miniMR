# A makefile for building the capstone project.
# 
# SYNOPSIS:
# 
#   make [all]		- makes everything and runs the tests
#	make compile	- just builds the source
#	make run_tests	- just runs the tests
#	make TARGET		- makes the given target
#	make clean		- removes all files generated by make

COMPILER=g++
# COMPILER=clang
COMPILE_OPTS= -std=c++11 -I${SRC} -Ilib -Wall
SRC=src
BIN=bin

GTEST_DIR = lib/gtest-1.6.0
TEST_DIR = test
TEST_FLAGS = ${COMPILE_OPTS} -I${GTEST_DIR} -I${GTEST_DIR}/include
GTEST_HEADERS = ${GTEST_DIR}/include/gtest/*.h \
				${GTEST_DIR}/include/gtest/internal/*.h
GTEST_SRCS = ${GTEST_DIR}/src/*.cc ${GTEST_DIR}/src/*.h ${GTEST_HEADERS}
TESTS = mapper_test.o reducer_test.o

all : compile tests run_tests

init :
	mkdir bin

compile : clean init mapreduce

run_tests : tests
	bin/tests.out

mapreduce : mapper reducer inputreader outputwriter job partition keyvaluepair

mapper : 
	${COMPILER} ${COMPILE_OPTS} ${SRC}/mapper.cpp -c -o ${BIN}/mapper.o

reducer : 
	${COMPILER} ${COMPILE_OPTS} ${SRC}/reducer.cpp -c -o ${BIN}/reducer.o

inputreader : 
	${COMPILER} ${COMPILE_OPTS} ${SRC}/inputreader.cpp -c -o ${BIN}/inputreader.o

outputwriter : 
	${COMPILER} ${COMPILE_OPTS} ${SRC}/outputwriter.cpp -c -o ${BIN}/outputwriter.o

job : 
	${COMPILER} ${COMPILE_OPTS} ${SRC}/job.cpp -c -o ${BIN}/job.o

partition : 
	${COMPILER} ${COMPILE_OPTS} ${SRC}/partition.cpp -c -o ${BIN}/partition.o

keyvaluepair : 
	${COMPILER} ${COMPILE_OPTS} ${SRC}/keyvaluepair.cpp -c -o ${BIN}/keyvaluepair.o




gtest-all.o : ${GTEST_SRCS}
	${COMPILER} ${TEST_FLAGS} -c ${GTEST_DIR}/src/gtest-all.cc

gtest_main.o : ${GTEST_SRCS}
	${COMPILER} ${TEST_FLAGS} -c ${GTEST_DIR}/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

mapper_test.o : ${TEST_DIR}/mapper_test.cpp mapreduce
	${COMPILER} ${TEST_FLAGS} -c ${TEST_DIR}/mapper_test.cpp

reducer_test.o : ${TEST_DIR}/reducer_test.cpp mapreduce
	${COMPILER} ${TEST_FLAGS} -c ${TEST_DIR}/reducer_test.cpp

LINKS = bin/*.o
tests : ${TESTS} gtest_main.a 
	${COMPILER} ${TEST_FLAGS} -lpthread $^ bin/*.o -o bin/$@.out



clean :
	rm -f */*/*.h.gch
	rm -rf bin
	rm -f *.o
	rm -f *.a

