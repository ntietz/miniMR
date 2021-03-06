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
COMPILE_OPTS= -std=c++11 -I${SRC} -Ilib -Wall -O3 -g
SRC=src
BIN=bin

GTEST_DIR = lib/gtest-1.6.0
TEST_DIR = test
TEST_FLAGS = ${COMPILE_OPTS} -I${GTEST_DIR} -I${GTEST_DIR}/include
GTEST_HEADERS = ${GTEST_DIR}/include/gtest/*.h \
				${GTEST_DIR}/include/gtest/internal/*.h
GTEST_SRCS = ${GTEST_DIR}/src/*.cc ${GTEST_DIR}/src/*.h ${GTEST_HEADERS}
TESTS = mapper_test.o reducer_test.o context_test.o job_test.o diskcache_test.o keyvaluepair_test.o

all : compile tests run_tests

init :
	mkdir bin

compile : clean init mapreduce

samples : compile wordcount beer_prepare beer_stats

run_tests : tests
	bin/tests.out

mapreduce : mapper reducer inputreader outputwriter job keyvaluepair diskcache impls

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

keyvaluepair : 
	${COMPILER} ${COMPILE_OPTS} ${SRC}/keyvaluepair.cpp -c -o ${BIN}/keyvaluepair.o

diskcache :
	${COMPILER} ${COMPILE_OPTS} ${SRC}/diskcache.cpp -c -o ${BIN}/diskcache.o

impls :
	${COMPILER} ${COMPILE_OPTS} ${SRC}/impl/inputreaders.cpp -c -o ${BIN}/inputreaders.o

wordcount :
	${COMPILER} ${COMPILE_OPTS} samples/wordcount.cpp bin/*.o -o ${BIN}/wordcount.out

beer_stats :
	${COMPILER} ${COMPILE_OPTS} -Isamples samples/beer_stats.cpp bin/*.o -o ${BIN}/beer_stats.out

beer_prepare :
	${COMPILER} ${COMPILE_OPTS} -Isamples samples/beer_prepare.cpp bin/*.o -o ${BIN}/beer_prepare.out

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

context_test.o : ${TEST_DIR}/context_test.cpp mapreduce
	${COMPILER} ${TEST_FLAGS} -c ${TEST_DIR}/context_test.cpp

job_test.o : ${TEST_DIR}/job_test.cpp mapreduce
	${COMPILER} ${TEST_FLAGS} -c ${TEST_DIR}/job_test.cpp

diskcache_test.o : ${TEST_DIR}/diskcache_test.cpp mapreduce
	${COMPILER} ${TEST_FLAGS} -c ${TEST_DIR}/diskcache_test.cpp

keyvaluepair_test.o : ${TEST_DIR}/keyvaluepair_test.cpp mapreduce
	${COMPILER} ${TEST_FLAGS} -c ${TEST_DIR}/keyvaluepair_test.cpp

LINKS = bin/*.o
tests : ${TESTS} gtest_main.a 
	${COMPILER} ${TEST_FLAGS} -lpthread $^ bin/*.o -o bin/$@.out



clean :
	rm -f */*/*.h.gch
	rm -rf bin
	rm -f *.o
	rm -f *.a

