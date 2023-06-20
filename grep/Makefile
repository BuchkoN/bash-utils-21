GCC=gcc -Wall -Wextra -Werror
CFLAG=-c

all: s21_grep

s21_grep: grep_main.o grep.o
	$(GCC) grep_main.o grep.o -o s21_grep -g

grep_main.o: main.c
	$(GCC) $(CFLAG) main.c -o grep_main.o

grep.o: grep.c
	$(GCC) $(CFLAG) grep.c

clean:
	rm -rf *.o
	rm -rf s21_grep

rebuild: clean all