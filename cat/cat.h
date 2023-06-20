#ifndef CAT_H
#define CAT_H
#define SHOTR_OPTIONS "bneEstTv"
#define COUNT_OPTIONS 6
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

char *num_to_str(int num);

int find_index_first_file(int argc, char *argv[]);
void available_flags(int argc, char *argv[], int *flags, int *is_correct_flag, int index_file);
void print_cat(int *options, FILE *fp);
void flag_b(char **line, ssize_t *len_line);
void flag_n(char **line, ssize_t *len_line);
void flag_e(char **line, ssize_t *len_line);
void flag_s(char **line, ssize_t *len_line);
void flag_t(char **line, ssize_t *len_line);
void flag_v(char **line, ssize_t *len_line);

#endif