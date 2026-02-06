#ifndef FILE_HANDLE_H_
#define FILE_HANDLE_H_

#define DIR_PATH_LENGTH 26
#define DIR_PATH "/.config/segodnyaconnect/"

int add_to_config(char const* config_path, char const* dir_path);
int config_file_exist(char const* file);
int fix_config_file(char const* file_name);
int get_path_length(char const* file_name);
int get_dir_path(char dir_path[]);
int get_file_path(char file_path[], char const* file_name);
int remove_from_config(char const* config_path, char const* dir_path);

// Log handle
void log_mkdir_error(const char* path);
void log_no_file_war(char const* file_path);
void log_pwd_war();
void log_pwd_error();

#endif