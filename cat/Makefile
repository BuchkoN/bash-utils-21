GCC =gcc -Wall -Werror -Wextra
CFLAG = -c

all: s21_cat

s21_cat: cat_main.o cat.o
	$(GCC) cat_main.o cat.o -o s21_cat

cat_main.o: main.c
	$(GCC) $(CFLAG) main.c -o cat_main.o

cat.o: cat.c
	$(GCC) $(CFLAG) cat.c

clean:
	rm -rf *.o
	rm -rf s21_cat

rebuild: clean all