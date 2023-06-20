#include "cat.h"

void print_cat(int *flags_list, FILE *fp) {
  char *line = NULL;
  size_t size_line = 0;
  ssize_t len_line = 0;
  int is_succes_alloc = 1;
  void (*flags_func[COUNT_OPTIONS])(char **line, ssize_t *len_line) = {
      flag_s, flag_t, flag_b, flag_e, flag_n, flag_v};

  while ((len_line = getline(&line, &size_line, fp)) != -1 && is_succes_alloc) {
    for (int i = 0; i < COUNT_OPTIONS && is_succes_alloc; i++) {
      if (flags_list[i] == 1) {
        flags_func[i](&line, &len_line);
        is_succes_alloc = (line != NULL);
      }
    }
    if (is_succes_alloc) {
      for (int i = 0; i < len_line; i++) {
        printf("%c", line[i]);
      }
    } else {
      fprintf(stderr, "Memory allocation failed\n");
    }
    free(line);
    line = NULL;
  }
  free(line);
  line = NULL;
  flag_n(&line, &len_line);
  flag_s(&line, &len_line);
}

void flag_v(char **line, ssize_t *len_line) {
  char *buffer = NULL;
  for (int i = 0; i < (int)(*len_line - 1) && *line != NULL; i++) {
    if ((*line)[i] < -96) {
      buffer = *line;
      *line = realloc(*line, *len_line + 3);
      if (*line != NULL) {
        memmove(*line + i + 2, *line + i, *len_line - i + 1);
        (*line)[i + 2] = (int)((*line)[i] & 127);
        memcpy(*line + i, "M-", 2);
        *len_line += 2;
      } else {
        free(buffer);
        buffer = NULL;
      }
    } else if (((*line)[i] >= 0 && (*line)[i] <= 31 && (*line)[i] != '\t') ||
               (*line)[i] == 127) {
      buffer = *line;
      *line = realloc(*line, *len_line + sizeof('^') + sizeof('\0'));
      if (*line != NULL) {
        *len_line += 1;
        memmove(*line + i + 1, *line + i, *len_line - i + 1);
        (*line)[i + 1] = (*line)[i] == 127 ? '?' : (*line)[i] + 64;
        (*line)[i] = '^';
      } else {
        free(buffer);
        buffer = NULL;
      }
    }
  }
}

void flag_t(char **line, ssize_t *len_line) {
  char *buffer = NULL;
  for (int i = 0; i < *len_line - 1 && *line != NULL; i++) {
    if (*line != NULL && (*line)[i] == '\t') {
      buffer = *line;
      *line = realloc(*line, *len_line + sizeof('^') + sizeof('\0'));
      if (*line != NULL) {
        *len_line += 1;
        memmove(*line + i + 1, *line + i, *len_line - i + 1);
        memcpy(*line + i, "^I", 2);
      } else {
        free(buffer);
        buffer = NULL;
      }
    }
  }
}

void flag_s(char **line, ssize_t *len_line) {
  static int is_empty_line = 0;

  if (*line != NULL && (*line)[0] == '\n' && is_empty_line == 0) {
    is_empty_line = 1;
  } else if (*line != NULL && (*line)[0] == '\n' && is_empty_line == 1) {
    (*line)[0] = '\0';
    *len_line = 0;
  } else {
    is_empty_line = 0;
  }
}

void flag_e(char **line, ssize_t *len_line) {
  if (*line != NULL && *len_line != 0 && (*line)[*len_line - 1] == '\n') {
    char *buffer = *line;
    *line = realloc(*line, *len_line + sizeof('$') + sizeof('\0'));
    if (*line != NULL) {
      *len_line += 1;
      memcpy(*line + *len_line - 2, "$\n", 2);
    } else {
      free(buffer);
      buffer = NULL;
    }
  }
}

void flag_b(char **line, ssize_t *len_line) {
  if ((*line)[0] != '\n') {
    flag_n(line, len_line);
  }
}

void flag_n(char **line, ssize_t *len_line) {
  static int num_line = 1;
  char *buffer = NULL, *str_num_line = NULL;

  num_line = *line == NULL ? 1 : num_line;
  if (*len_line >= 1) {
    if ((str_num_line = num_to_str(num_line)) != NULL) {
      buffer = *line;
      *line = realloc(*line, *len_line + strlen(str_num_line) + sizeof('\0'));
    } else {
      free(*line);
      *line = NULL;
    }
    if (*line != NULL) {
      int len_num = strlen(str_num_line);
      *len_line += len_num;
      memmove(*line + len_num, *line, *len_line - len_num + sizeof('\0'));
      memcpy(*line, str_num_line, strlen(str_num_line));
      num_line++;
    } else {
      free(buffer);
      buffer = NULL;
    }
    free(str_num_line);
    str_num_line = NULL;
  }
}

char *num_to_str(int num) {
  char *strnum = NULL;
  if ((strnum = malloc(8 * sizeof(char))) != NULL) {
    sprintf(strnum, "%6d	", num);
  }
  return strnum;
}