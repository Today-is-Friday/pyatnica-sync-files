#include "segodnyaconnect.h"

#include <asm-generic/errno-base.h>  // mkdir error list
#include <errno.h>                   // error handle
#include <getopt.h>                  // optind
#include <pwd.h>                     // getuid(), getpwuid()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <syslog.h>  // log write
#include <unistd.h>  // getopt_long() sysconf

#include "file_handle.h"
#define USER_DIRS_FILE_NAME "user_directories.conf"

int main(int argc, char* argv[]) {
  int _add_folder;
  check(argc, argv, &_add_folder);
  if (_add_folder) add_folder(argc, argv, optind);

  return 0;
}

int check(int argc, char* argv[], int* ptr_add_folder) {
  struct option long_options[] = {{"--add-folder", 0, NULL, 'a'}};
  int flag;
  while ((flag = getopt_long(argc, argv, "a", long_options, 0)) != -1) {
    switch (flag) {
      case 'a':
        *ptr_add_folder = 1;
        break;
      default:
        perror("Try ./segodnyaconnect --help for more information.");
        break;
    }
  }
  return 1;
}

int add_folder(int argc, char* argv[], int optind) {
  while (optind != argc) {
    const char* dir_name = argv[optind];
    struct stat statbuf;
    if (stat(dir_name, &statbuf) != 0)
      perror("directory doesn't exist");
    else if (S_ISDIR(statbuf.st_mode)) {
      printf("directory found\n");
      add_to_config(USER_DIRS_FILE_NAME, dir_name);  // need to fix
    } else
      perror("this is not directory");
    optind++;
  }

  return 1;
}

int add_to_config(const char* config_path, const char* dir_path) {
  int state = 1;
  FILE* file = fopen(config_path, "a");
  if (file) {
    state = fputs(dir_path, file);
    fclose(file);
  } else
    state = 0;

  return state;
}

// LOGS

void log_no_file_war(char const* file_path) {
  printf("Can not open file '%s': %s\n", file_path, strerror(errno));
}

void log_mkdir_error(const char* path) {
  if (errno != EEXIST) {
    printf("Failed to create config directory '%s': %s\n", path,
           strerror(errno));
    exit(EXIT_FAILURE);
  }
}

void log_pwd_war() { printf("getpwuid failed for UID %d\n", getuid()); }

void log_pwd_error() {
  printf(LOG_ERR, "No passwd entry found for UID %d\n", getuid());
  exit(EXIT_FAILURE);
}
