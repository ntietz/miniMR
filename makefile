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

all : compile 

init :
	mkdir bin

compile : clean init mapreduce

mapreduce : ${SRC}/job.hpp
	${COMPILER} ${COMPILE_OPTS} ${SRC}/job.hpp -c -o ${BIN}/mapreduce.o

clean :
	rm -rf bin

