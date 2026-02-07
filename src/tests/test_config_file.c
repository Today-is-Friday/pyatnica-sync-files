#include <check.h>
#include <stdio.h>

#include "test.h"

START_TEST(add_to_config_test) {
  // create file
  FILE* recreate_file = fopen("test_1", "w");
  ck_assert_ptr_nonnull(recreate_file);
  fclose(recreate_file);

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

Suite* test_config_file() {
  Suite* s = suite_create("\033[45mFILE_CONFIG\033[0m");
  TCase* tc = tcase_create("file_config");

  tcase_add_test(tc, add_to_config_test);

  suite_add_tcase(s, tc);
  return s;
}