#include <check.h>
#include <stdio.h>
#include <string.h>

#include "test.h"

START_TEST(add_to_config_test) {
  remove("test_1");

  char dir[] = "~";

  // test func
  int result = add_to_config("test_1", "~");
  ck_assert_int_eq(result, 1);

  FILE* file = fopen("test_1", "r");
  ck_assert_ptr_nonnull(file);
  // compare content
  char content[sizeof(dir)];
  fgets(content, sizeof(dir), file);
  ck_assert_str_eq(content, dir);

  // delete file
  remove("test_1");
}
END_TEST

START_TEST(add_to_config_test_2) {
  // test func
  int result = add_to_config("", "");
  ck_assert_int_eq(-1, result);
}
END_TEST

START_TEST(add_to_config_test_3) {
  const char* file = "test_3";
  remove(file);

  // test func
  int result = add_to_config(file, "");
  ck_assert_int_eq(1, result);

  // compare content
  FILE* f = fopen(file, "r");
  ck_assert_ptr_nonnull(f);
  char result1[2];
  fgets(result1, 2, f);
  ck_assert_str_eq("\n", result1);
  fclose(f);

  // test func
  result = add_to_config(file, "some_path");
  ck_assert_int_eq(1, result);
  result = add_to_config(file, "some_path_2");
  ck_assert_int_eq(1, result);

  // compare content
  char true_result[] =
      "\nsome_path\nsome_path_2\n";  // after use add_to_config it adds \n on
                                     // the end of the file
  char text[4096] = "";
  char buf[4096] = "";
  f = fopen(file, "r");
  ck_assert_ptr_nonnull(f);
  while (fgets(buf, sizeof(buf), f)) strcat(text, buf);
  ck_assert_str_eq(true_result, text);
  fclose(f);
  remove(file);
}
END_TEST

START_TEST(get_dir_path_test) {
  // test func
  char dir[1024];
  char match_1[] = "/home/";
  char match_2[] = "/.config/segodnyaconnect/";
  ck_assert_int_eq(1, get_dir_path(dir));
  ck_assert_ptr_nonnull(strstr(dir, match_1));
  ck_assert_ptr_nonnull(strstr(dir, match_2));
}
END_TEST

Suite* test_config_file() {
  Suite* s = suite_create("\033[45mFILE_CONFIG\033[0m");
  TCase* tc = tcase_create("file_config");

  tcase_add_test(tc, add_to_config_test);
  tcase_add_test(tc, add_to_config_test_2);
  tcase_add_test(tc, add_to_config_test_3);
  tcase_add_test(tc, get_dir_path_test);

  suite_add_tcase(s, tc);
  return s;
}