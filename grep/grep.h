#ifndef GREP_H
#define GREP_H
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>
#include <getopt.h>
#define INIT_OPTIONS {false, false, false, false, false, false, false, false, false, false, 0, 0, NULL, NULL}
#define GREP_OPTS "e:ivclnhsf:o"

typedef struct options {
	bool enable_opt_e;
	bool enable_opt_i;
	bool enable_opt_v;
	bool enable_opt_c;
	bool enable_opt_l;
	bool enable_opt_n;
	bool enable_opt_h;
	bool enable_opt_s;
	bool enable_opt_f;
	bool enable_opt_o;
	int count_files;
	int count_patterns;
	char ** patts;
	char ** files;
} options_s;

int parse_args(options_s *opts, bool *err_opt, bool *err_patt, int argc, char **argv);
void add_item_mass(char *item, char ***mass, int len_mass);
void parse_opt_f(char *filename, options_s *opts);
void parse_files(options_s *opts, char **argv, int argc, int file_index);

void grep_file(FILE *file, char *filename, options_s *opts);
void enum_files(options_s *opts);
int match_search(options_s *opts, char *line);
int match_flag_o(options_s *opts, char *line, char **line_flag_o);

void print_line(options_s *opts, char *line, int num_line, char *filename);
void print_file(options_s *opts, char *filename, int match_line);
void free_mass(char **mass, int len_mass);

#endif