#include <asm-generic/errno-base.h>  // mkdir error list
#include <getopt.h>                  // optind
#include <pwd.h>                     // getuid(), getpwuid()
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/syslog.h>
#include <sys/types.h>
#include <syslog.h>  // log write
#include <unistd.h>  // getopt_long() sysconf

#include "file_handle.h"

int add_to_config(char const* config_path, char const* dir_path) {
  int state = 1;
  FILE* config_file = fopen(config_path, "a");
  if (config_file != NULL) {
    fputs(dir_path, config_file);
    fputc('\n', config_file);
    fclose(config_file);
  } else
    state = 0;

  return state;
}

// 1 - path removed
// 0 - path wasn't exist
// -1 - file doesn't exist
// int remove_from_config(char const *config_path, char const *dir_path) {
//   int state = 0;
//   char line[256];
//   FILE *config_file = fopen(config_path, "a+");
//   if (config_file != NULL) {
//     while (fgets(line, sizeof(line), config_file) && !state) {
//       if (strcmp(line, dir_path) == 0) state = 1;
//     }
//   } else
//     state = -1;

//   return state;
// }

// 1 - file exists.
// 0 - dir / file doesn't exist.
// -1 - user has issue (no auto fix).
int config_file_exist(char const* file_name) {
  int state = 1;
  // Gets path length
  int path_length = get_path_length(file_name);
  char path[path_length];
  // Checks file state
  state = get_file_path(path, file_name);
  if (state) {
    if (access(path, F_OK) != 0) state = 0;
  } else
    state = -1;

  return state;
}

// Calls if config_file_exist() returned 0
int fix_config_file(char const* file_name) {
  int state = 1;
  // Gets path length
  int path_length = get_path_length(file_name);
  char dir_path[path_length];
  char path[path_length];
  // If directory doesn't exist it create it
  state = get_dir_path(dir_path);
  if (state)
    if (mkdir(dir_path, 0755) == -1) log_mkdir_error(dir_path);

  // Tries recreate a file
  if ((state = get_file_path(path, file_name))) {
    FILE* file = fopen(path, "a");
    if (file != NULL)
      fclose(file);
    else
      state = 0;
  }
  return state;
}

int get_path_length(char const* file_name) {
  int path_length = 0;
  // gets max user name length
  long user_length = sysconf(_SC_LOGIN_NAME_MAX);
  int file_name_length = strlen(file_name);
  // sysconf didn't found any limit
  if (user_length == -1)
    path_length = file_name_length + DIR_PATH_LENGTH + 32;
  else
    path_length = file_name_length + DIR_PATH_LENGTH + user_length;

  return path_length + 1;
}

int get_dir_path(char dir_path[]) {
  int state = 1;
  const struct passwd* pwd = getpwuid(getuid());

  // Can find the user
  if (pwd) {
    sprintf(dir_path, "%s%s", pwd->pw_dir, DIR_PATH);
  } else {
    state = 0;
  }

  return state;
}

int get_file_path(char file_path[], char const* file_name) {
  // Gets dir path firs
  int state = get_dir_path(file_path);

  if (state) {
    strcat(file_path, file_name);
  }

  return state;
}
