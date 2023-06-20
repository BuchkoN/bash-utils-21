#include "grep.h"

void enum_files(options_s *opts) {
  for (int i = 0; i < opts->count_files; i++) {
    FILE *file = fopen(opts->files[i], "r");
    if (file) {
      grep_file(file, opts->files[i], opts);
      fclose(file);
    } else if (!file && !opts->enable_opt_s) {
      fprintf(stderr, "s21_grep: %s: No such file or directory\n",
              opts->files[i]);
    }
  }
}

void grep_file(FILE *file, char *filename, options_s *opts) {
  size_t size_line = 0;
  int len_line = 0, num_line = 0, match = 0, match_line = 0;
  char *line = NULL, *line_flag_o = NULL, *pline = NULL;

  while (match != -1 && (len_line = getline(&line, &size_line, file)) != -1) {
    if (!opts->enable_opt_o) {
      match = match_search(opts, line);
    } else {
      match = match_flag_o(opts, line, &line_flag_o);
    }
    if (!opts->enable_opt_v) {
      match_line += match > 0 ? 1 : 0;
    } else {
      match_line += match == 0 ? 1 : 0;
    }
    num_line++;
    if (!opts->enable_opt_c && !opts->enable_opt_l) {
      pline = line;
      if (match > 0 && !opts->enable_opt_v) {
        pline = opts->enable_opt_o ? line_flag_o : line;
        // Если совпадения есть и поиск не инвертирован
        print_line(opts, pline, num_line, filename);
      } else if (match == 0 && opts->enable_opt_v) {
        // Если совпадений нет, но поиск инвертирован
        print_line(opts, pline, num_line, filename);
      } else if (match == -1 && !opts->enable_opt_s) {
        fprintf(stderr, "s21_grep: error regomp\n");
      }
    }
    free(line_flag_o);
    line_flag_o = NULL;
  }
  free(line);
  line = NULL;
  if ((opts->enable_opt_c || opts->enable_opt_l) && match != -1) {
    print_file(opts, filename, match_line);
  }
}

int match_search(options_s *opts, char *line) {
  regmatch_t pmatch = {0};
  regex_t preg = {0};
  size_t nmatch = 1;
  int match = 0, res_comp = 0, mod_regcomp = 0;

  mod_regcomp = opts->enable_opt_i ? REG_ICASE : REG_EXTENDED;
  for (int i = 0; i < opts->count_patterns && !res_comp && match != -1; i++) {
    res_comp = regcomp(&preg, opts->patts[i], mod_regcomp);
    if (res_comp) {
      match = -1;
    } else if (regexec(&preg, line, nmatch, &pmatch, 0) == 0 &&
               line[0] != '\n') {
      match++;
    }
    regfree(&preg);
  }

  return match;
}

int match_flag_o(options_s *opts, char *line, char **line_flag_o) {
  regmatch_t pmatch = {0};
  regex_t preg = {0};
  size_t nmatch = 1;
  int match = 0, res_comp = 0, res_regex = 0, mod_regcomp = 0,
      size_line_flag_o = 0;
  char *buff = NULL;

  mod_regcomp = opts->enable_opt_i ? REG_ICASE : REG_EXTENDED;
  for (int i = 0; i < opts->count_patterns && !res_comp; i++) {
    res_comp = regcomp(&preg, opts->patts[i], mod_regcomp);
    res_regex = 0;
    while (!res_comp && !res_regex) {
      if ((res_regex = regexec(&preg, line, nmatch, &pmatch, 0)) == 0) {
        buff = *line_flag_o;
        *line_flag_o = realloc(*line_flag_o,
                               size_line_flag_o + strlen(opts->patts[i]) + 2);
        if (*line_flag_o == NULL) {
          free(buff);
        } else {
          memcpy(*line_flag_o + size_line_flag_o, line + pmatch.rm_so,
                 pmatch.rm_eo - pmatch.rm_so);
          size_line_flag_o += (pmatch.rm_eo - pmatch.rm_so + 1);
          memcpy(*line_flag_o + size_line_flag_o - 1, "\n\0", 2);
        }
        line = line + pmatch.rm_eo;
        match++;
      }
    }
    regfree(&preg);
  }
  return match;
}

void print_line(options_s *opts, char *line, int num_line, char *filename) {
  if (opts->count_files == 1) {
    opts->enable_opt_h = true;
  }
  if (!opts->enable_opt_h) {
    printf("%s:", filename);
  }
  if (opts->enable_opt_n) {
    printf("%d:", num_line);
  }
  printf("%s", line);
  if (strchr(line, '\n') == NULL) {
    printf("\n");
  }
}

void print_file(options_s *opts, char *filename, int match_line) {
  match_line = opts->enable_opt_l && match_line > 1 ? 1 : match_line;
  if (opts->count_files == 1) {
    opts->enable_opt_h = true;
  }
  if (!opts->enable_opt_h && opts->enable_opt_c) {
    printf("%s:%d\n", filename, match_line);
  } else if (opts->enable_opt_c) {
    printf("%d\n", match_line);
  }
  if (opts->enable_opt_l && match_line) {
    printf("%s\n", filename);
  }
}