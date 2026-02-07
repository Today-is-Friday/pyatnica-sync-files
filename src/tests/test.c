#include "test.h"

#include <stdio.h>

int main(void) {
  int failed = 0;
  int total = 0;

  Suite* tests[] = {test_config_file(), NULL};

  for (int i = 0; tests[i] != NULL; i++) {
    SRunner* sr = srunner_create(tests[i]);

    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_NORMAL);

    failed += srunner_ntests_failed(sr);
    total += srunner_ntests_run(sr);
    srunner_free(sr);
  }

  printf("\033[41m========= FAILED : %-3d =========\033[0m\n", failed);
  printf("\033[42m\033[30m========= SUCCESS: %-3d =========\033[0m\n",
         total - failed);
  printf("\033[42m\033[30m========= TOTAL: %-3d =========\033[0m\n", total);

  return failed == 0 ? 0 : 1;
}