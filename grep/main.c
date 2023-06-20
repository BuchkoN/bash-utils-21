#include "grep.h"

int main(int argc, char **argv) {
  options_s options = INIT_OPTIONS;
  bool err_opt = false, err_patt = false;
  if (argc > 1) {
    int file_index = parse_args(&options, &err_opt, &err_patt, argc, argv);
    parse_files(&options, argv, argc, file_index);
    if (options.count_files && options.count_patterns && !err_opt &&
        !err_patt) {
      enum_files(&options);
      free_mass(options.files, options.count_files);
      free_mass(options.patts, options.count_patterns);
    } else {
      fprintf(stderr, "s21_grep: %s\n",
              err_opt ? "invalid option" : "missing pattern");
      free_mass(options.files, options.count_files);
      free_mass(options.patts, options.count_patterns);
    }
  }
  // // Дебаг парсинга
  // for (int i = 0; i < options.count_files; i++) {
  //   printf("Файл %s\n", options.files[i]);
  // }
  // for (int i = 0; i < options.count_patterns; i++) {
  //   printf("Шаблон %s\n", options.patts[i]);
  // }
  return 0;
}

int parse_args(options_s *opts, bool *err_opt, bool *err_patt, int argc,
               char **argv) {
  struct option long_options[] = {};
  int opt = 0, long_opt = 0;
  opterr = 0;
  while ((opt = getopt_long(argc, argv, GREP_OPTS, long_options, &long_opt)) !=
         -1) {
    if (opt == 'e') {
      opts->enable_opt_e = true;
      add_item_mass(optarg, &opts->patts, opts->count_patterns);
      opts->count_patterns = opts->patts != NULL ? opts->count_patterns + 1 : 0;
    } else if (opt == 'f') {
      opts->enable_opt_f = true;
      parse_opt_f(optarg, opts);
    } else if (opt == 'i') {
      opts->enable_opt_i = true;
    } else if (opt == 'v') {
      opts->enable_opt_v = true;
    } else if (opt == 'c') {
      opts->enable_opt_c = true;
    } else if (opt == 'l') {
      opts->enable_opt_l = true;
    } else if (opt == 'n') {
      opts->enable_opt_n = true;
    } else if (opt == 'h') {
      opts->enable_opt_h = true;
    } else if (opt == 's') {
      opts->enable_opt_s = true;
    } else if (opt == 'o') {
      opts->enable_opt_o = true;
    } else if (opt == ':') {
      *err_patt = 1;
    } else if (opt == '?') {
      *err_opt = 1;
    }
  }
  if (opts->count_patterns == 0) {
    add_item_mass(argv[optind], &opts->patts, opts->count_patterns);
    opts->count_patterns = opts->patts != NULL ? opts->count_patterns + 1 : 0;
    optind++;
  }
  return optind;
}

void parse_files(options_s *opts, char **argv, int argc, int file_index) {
  for (int i = file_index; i < argc; i++) {
    add_item_mass(argv[i], &opts->files, opts->count_files);
    opts->count_files = opts->files != NULL ? opts->count_files + 1 : 0;
  }
}

void add_item_mass(char *item, char ***mass, int len_mass) {
  char **buffer = *mass;
  *mass = (char **)realloc(*mass, (len_mass + 1) * sizeof(char *));
  if (*mass == NULL) {
    free_mass(buffer, len_mass);
  } else {
    (*mass)[len_mass] = strdup(item);
  }
  if (*mass != NULL && (*mass)[len_mass] == NULL) {
    free_mass(buffer, len_mass);
  }
}

void parse_opt_f(char *filename, options_s *opts) {
  size_t size_line = 0;
  char *line = NULL;

  FILE *file = fopen(filename, "r");
  if (file != NULL) {
    while (getline(&line, &size_line, file) != -1) {
      if (strchr(line, '\n') != NULL) {
        line[strlen(line) - 1] = '\0';
      }
      add_item_mass(line, &opts->patts, opts->count_patterns);
      opts->count_patterns = opts->patts != NULL ? opts->count_patterns + 1 : 0;
    }
    free(line);
    fclose(file);
  }
}

void free_mass(char **mass, int len_mass) {
  for (int i = 0; i < len_mass; i++) {
    free(mass[i]);
  }
  free(mass);
}