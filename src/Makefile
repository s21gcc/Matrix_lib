#------------------------------------------------#
#   INGREDIENTS                                  #
#------------------------------------------------#
# SRCS      source files
# OBJS      object files
#
# CC        compiler
# CFLAGS    compiler flags


SRCS        := s21_matrix_oop.cc
TEST_SRCS	:= matrix_tests.cc
OBJS        := $(SRCS:.cc=.o)
TEST_OBJS	:= $(TEST_SRCS:.cc=.o)
EXE			:= s21_matrix_oop
LIB			:= s21_matrix_oop.a

CC          := gcc
CFLAGS      := -lstdc++ -std=c++17 -Wall -Werror -Wextra

all: test clean

$(LIB): $(SRCS)
	$(CC) $(CFLAGS) -c $(SRCS)
	ar rcs $(LIB) $(OBJS)

test: $(LIB)
	$(CC) $(CFLAGS) -lgtest $(LIB) $(TEST_SRCS) -o test
	./test

style:
	cp ../materials/linters/.clang-format .
	clang-format -n *.cc *.h
	rm ./.clang-format

clean:
	rm -rf *.o *.a $(EXE)
	rm -rf test
