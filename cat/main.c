#include "cat.h"

int main(int argc, char *argv[]) {
  int options[COUNT_OPTIONS] = {0};
  int is_correct_flag = 1, index_file = 0;

  index_file = find_index_first_file(argc, argv);
  available_flags(argc, argv, options, &is_correct_flag, index_file);
  if (index_file == -1) {
    fprintf(stderr, "s21_cat: missing filename\n");
  } else if (is_correct_flag == 0) {
    fprintf(stderr, "s21_cat: illegal flag\n");
  } else {
    for (int i = index_file; i < argc; i++) {
      FILE *fp = fopen(argv[i], "r");
      if (fp != NULL) {
        print_cat(options, fp);
        fclose(fp);
      } else {
        fprintf(stderr, "s21_cat: no such file %s\n", argv[i]);
      }
    }
  }
  return 0;
}

int find_index_first_file(int argc, char *argv[]) {
  int index = 1;
  int is_find_filename = 0;
  while (index < argc && !is_find_filename) {
    if (argv[index][0] != '-') {
      is_find_filename = 1;
    } else {
      index++;
    }
  }
  return is_find_filename ? index : -1;
}

void available_flags(int argc, char *argv[], int *options, int *is_correct_flag,
                     int index_file) {
  char option = '0';
  int long_index = 0;
  struct option long_options[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
  };
  opterr = 0;

  option = getopt_long(argc, argv, SHOTR_OPTIONS, long_options, &long_index);
  while (option != -1 && (optind - 2) < index_file) {
    if (option == 'b') {
      options[2] = 1;
      options[4] = 0;
    } else if (option == 'e') {
      options[3] = 1;
      options[5] = 1;
    } else if (option == 'E') {
      options[3] = 1;
    } else if (option == 'n' && options[2] == 0) {
      options[4] = 1;
    } else if (option == 's') {
      options[0] = 1;
    } else if (option == 't') {
      options[1] = 1;
      options[5] = 1;
    } else if (option == 'T') {
      options[1] = 1;
    } else if (option == 'v') {
      options[5] = 1;
    } else if (option == '?') {
      *is_correct_flag = 0;
    }
    option = getopt_long(argc, argv, SHOTR_OPTIONS, long_options, &long_index);
  }
}