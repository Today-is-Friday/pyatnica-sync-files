#ifndef _SEGODNYA_CONNECT_D_H
#define _SEGODNYA_CONNECT_D_H
#define USER_DIRS_FILE_NAME "user_directories.conf"
#include <stddef.h>

int check_configs();
size_t read_from_file(char const *path, char **line_ptr[]);
int read_config(char const *path, char **line_ptr[]);
int read_user_dirs(char **dirs[], size_t *count);
void handle_events(int fd, char *buf);

// LOG
void log_cant_read_war();
void log_inotify_error();
void log_watch_war(char *file_path);
#endif  // _SEGODNYA_CONNECT_D_H