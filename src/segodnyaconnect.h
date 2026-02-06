#ifndef _SEGODNYA_CONNECT_H
#define _SEGODNYA_CONNECT_H

// segodnyaconnect.c
int check(int argc, char* argv[], int* ptr_add_folder);
int add_directory(int argc, char* argv[], int optind);
int add_to_config(const char* config_path, const char* dir_path);

// logs
void log_no_file_war(char const* file_path);
void log_mkdir_error(const char* path);
void log_pwd_war();
void log_pwd_error();
#endif