# Makefile / interpreter

CC = g++

EXCLUDE_FILES = test.cpp

FILES = $(shell find *.cpp -not -name ${EXCLUDE_FILES})

FLAGS= $(FILES) -o interpreter -O2 -Iinclude -Wall

all: compile run


compile:
	$(CC) $(FLAGS)

run:
	./interpreter

run_test_file:
	./interpreter scripts/test.lang
