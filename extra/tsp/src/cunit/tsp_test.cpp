#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>

#include "../tsp.h"


void assert_cyclic_equal(int *tuple1, int *tuple2, int count)
{
  int i;
  bool flag1, flag2;
  flag1 = flag2 = false;
  for(i = 0; i < count; i++)
  {
    if(tuple1[i] != tuple2[i])
    {
      flag1 = true;
      break;
    }
  }
  for(i = 0; i < count; i++)
  {
    if(tuple1[i] != tuple2[count - i - 1])
    {
      flag2 = true;
      break;
    }
  }
  if(flag1 && flag2)
  {
    CU_FAIL("Output Tuple is not equal to expected tuple");
  }
  else
  {
    CU_PASS();
  }
}

// Initialising Input values
float DISTANCE1[][40] = {
                        {0,1,1,1.414},
                        {1,0,1.414,1},
                        {1,1.414,0,1},
                        {1.414,1,1,0}
};

void test_tsp()
{
  char *err_msg, *original_err_msg;
  err_msg = original_err_msg = (char *)"No error";
  int ids[4] = {0,1,2,3};
  int total_tuples = 4;
  int result;
  float fit = 0;
  int source = 0;
  int expected_output[4] = {0,2,3,1};

  result = find_tsp_solution(total_tuples, DISTANCE1, ids, source, &fit, err_msg);
  CU_ASSERT_EQUAL(result, EXIT_SUCCESS);
  CU_ASSERT_EQUAL(err_msg, original_err_msg);
  assert_cyclic_equal(ids, expected_output, total_tuples);
  return;
}

int main()
{
  if(CU_initialize_registry() != CUE_SUCCESS)
  {
    printf("Cannot initialize test registry");
    return CU_get_error();
  }
  CU_pSuite pSuite = CU_add_suite("Tsp", NULL, NULL);
  CU_pTest pTest = CU_add_test(pSuite, "test_tsp", test_tsp);
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  //FIXME: cunit documentation uses `return CU_get_error();` but CU_get_error() always
  // returns 0 even if a test fails.
  if(CU_get_number_of_failures() != 0)
  {
    CU_cleanup_registry();
    return 1;
  }
  CU_cleanup_registry();
  return 0;
}
