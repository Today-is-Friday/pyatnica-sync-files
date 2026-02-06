#include "segodnyaconnectd.h"

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <sys/syslog.h>
#include <unistd.h>

#include "file_handle.h"

int main() {
  openlog("mydaemon", LOG_PID | LOG_CONS, LOG_DAEMON);

  int fd;
  int* wd;
  char buf[4096] __attribute__((aligned(__alignof__(struct inotify_event))));

  size_t count_dirs = 0;

  // starts inotify
  if ((fd = inotify_init()) == 0) log_inotify_error();

  // reads user dirs
  char** dirs;
  check_configs();
  read_user_dirs(&dirs, &count_dirs);
  wd = (int*)calloc(count_dirs, sizeof(int));
  for (size_t i = 0; i < count_dirs && wd && dirs[i]; i++) {
    printf("file '%s'\n", dirs[i]);
    wd[i] = inotify_add_watch(fd, dirs[i],
                              IN_MODIFY | IN_CLOSE_WRITE | IN_MOVE_SELF);

    // can't watch a file
    if (wd[i] == -1) log_watch_war(dirs[i]);
  }

  for (;;) {
    ssize_t len;
    char* ptr;
    len = read(fd, buf, sizeof(buf));
    if (len == -1 && errno != EAGAIN) {
      perror("read");
      exit(EXIT_FAILURE);
    }

    if (len == 0) sleep(1);

    for (ptr = buf; ptr < buf + len;) {
      struct inotify_event* event = (struct inotify_event*)ptr;

      // direct modify 'echo "" >> file'
      if (event->mask & IN_MODIFY) syslog(LOG_INFO, "Config file modified!");

      // override old file. text editors use this method
      else if (event->mask & IN_CLOSE_WRITE) {
        syslog(LOG_INFO, "Config file was overrided!");
        for (size_t i = 0; i < count_dirs; ++i)
          if (event->wd == wd[i]) {
            inotify_rm_watch(fd, wd[i]);
            sleep(1);  // make sure that file is already created (bad decision
                       // need to fix)
            wd[i] = inotify_add_watch(
                fd, dirs[i], IN_MODIFY | IN_CLOSE_WRITE | IN_MOVE_SELF);

            if (wd[i] == -1) log_watch_war(dirs[i]);
          }
      }

      // file moved or deleted
      else if (event->mask & IN_MOVE_SELF) {
        syslog(LOG_INFO, "Config file deleted!");
      }

      ptr += sizeof(struct inotify_event) + event->len;
    }
  }

  close(fd);
  free(wd);

  closelog();

  return 0;
}

int check_configs() {
  int state = config_file_exist(USER_DIRS_FILE_NAME);
  if (state == 0) state = fix_config_file(USER_DIRS_FILE_NAME);

  return state;
}

int read_user_dirs(char** dirs[], size_t* count) {
  int state = 1;
  int path_length = get_path_length(USER_DIRS_FILE_NAME);
  char path[path_length];
  if ((state = get_file_path(path, USER_DIRS_FILE_NAME)))
    *count = read_from_file(path, dirs);

  return state;
}

// int validate_dirs(char const **dirs, int count) {
//   for (int i = 0; i < count; ++i)
//     if (!config_file_exist(dirs[i])) log_mkdir_error("");

//   return 1;
// }

size_t read_from_file(char const* path, char** line_ptr[]) {
  size_t n = 0;
  char** lines = (char**)malloc(sizeof(char*));
  char buffer[256];
  FILE* file = fopen(path, "r");
  if (file && lines)
    while (fgets(buffer, 256, file)) {
      char** tmp = realloc(lines, (n + 1) * sizeof(char*));
      char* line = (char*)malloc(sizeof(char) * 257);
      if (line && tmp) {
        buffer[strcspn(buffer, "\n")] = '\0';
        strcpy(line, buffer);
        lines = tmp;
        lines[n++] = line;
      } else {
        log_cant_read_war();
        free(line);
        free(tmp);
      }
    }

  *line_ptr = lines;

  return n;
}

// LOGS

void log_cant_read_war() {
  syslog(LOG_WARNING, "Can not allocate a memory. Not enough memory!: %s",
         strerror(errno));
}

void log_no_file_war(char const* file_path) {
  syslog(LOG_ERR, "Can not open file '%s': %s\n", file_path, strerror(errno));
}

void log_mkdir_error(const char* path) {
  if (errno != EEXIST) {
    syslog(LOG_ERR, "Failed to create config directory '%s': %s\n", path,
           strerror(errno));
    closelog();
    exit(EXIT_FAILURE);
  }
}

void log_pwd_war() {
  syslog(LOG_WARNING, "Getpwuid failed for UID %d\n", getuid());
}

void log_watch_war(char* file_path) {
  syslog(LOG_WARNING, "Cannot watch '%s': %s\n", file_path, strerror(errno));
}

void log_pwd_error() {
  syslog(LOG_ERR, "No passwd entry found for UID %d\n", getuid());
  closelog();
  exit(EXIT_FAILURE);
}

void log_inotify_error() {
  syslog(LOG_ERR, "Inotify has occured with a problem: %s\n", strerror(errno));
  closelog();
  exit(EXIT_FAILURE);
}

void log_watching_warn(char* file_path) {
  syslog(LOG_WARNING, "Can not add watch for a file '%s': %s", file_path,
         strerror(errno));
}