#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>

#include "../dijkstra.h"

#define ASSERT_PATHS_EQUAL(path1, path2, path_count) do \
{ \
  int i; \
  for(i = 0; i < path_count; i++) \
  { \
	CU_ASSERT_EQUAL(path1[i].vertex_id, path2[i].vertex_id); \
	CU_ASSERT_EQUAL(path1[i].edge_id, path2[i].edge_id); \
	CU_ASSERT_EQUAL(path1[i].cost, path2[i].cost); \
  } \
}while(0);

edge_t *generate_test_edges()
{
  // The caller of this function is given the responsibility to free the edges memory
  edge_t *edges = (edge_t *)malloc(sizeof(edge_t) * 8);
  int i;
  edges[0] = { 1, 1, 2, 5, 5};
  edges[1] = { 2, 3, 1, 9, 9};
  edges[2] = { 3, 2, 6,10,10};
  edges[3] = { 4, 6, 3,10,10};
  edges[4] = { 5, 3, 4, 8, 8};
  edges[5] = { 6, 4, 5,10,10};
  edges[6] = { 7, 4, 2,10,100000};
  edges[7] = { 8, 5, 1,150,100000};
  return edges;
}

path_element **load_expected_output()
{
  path_element **op = (path_element **)malloc(sizeof(path_element *) * 6);

  //1->5 directed=false reverse_cost=false
  op[0] = (path_element *)malloc(sizeof(path_element) * 4);
  op[0][0] = {1,1,5};
  op[0][1] = {2,7,10};
  op[0][2] = {4,6,10};
  op[0][3] = {5,-1,0};

  //5->1 directed=false reverse_cost=false
  op[1] = (path_element *)malloc(sizeof(path_element) * 4);
  op[1][0] = {5,6,10};
  op[1][1] = {4,7,10};
  op[1][2] = {2,1,5};
  op[1][3] = {1,-1,0};

  //1->5 directed=true reverse_cost=false
  op[2] = (path_element *)malloc(sizeof(path_element) * 6);
  op[2][0] = {1,1,5};
  op[2][1] = {2,3,10};
  op[2][2] = {6,4,10};
  op[2][3] = {3,5,8};
  op[2][4] = {4,6,10};
  op[2][5] = {5,-1,0};

  //5->1 directed=true reverse_cost=false
  op[3] = (path_element *)malloc(sizeof(path_element) * 2);
  op[3][0] = {5,8,150};
  op[3][1] = {1,-1,0};

  //1->5 directed=true reverse_cost=true
  op[4] = (path_element *)malloc(sizeof(path_element) * 4);
  op[4][0] = {1,2,9};
  op[4][1] = {3,5,8};
  op[4][2] = {4,6,10};
  op[4][3] = {5,-1,0};

  //5->1 directed=true reverse_cost=true
  op[5] = (path_element *)malloc(sizeof(path_element) * 4);
  op[5][0] = {5,6,10};
  op[5][1] = {4,7,10};
  op[5][2] = {2,1,5};
  op[5][3] = {1,-1,0};
  return op;
}

void test_dijkstra()
{
  edge_t *edges = generate_test_edges();
  path_element_t *path;
  path_element_t **expected_path = load_expected_output();
  int path_count;
  char *err_msg, *original_err_msg;
  err_msg = original_err_msg = (char *)"No error";
  int result;

  result = boost_dijkstra(edges, 8, 1, 5, false, false, &path, &path_count, &err_msg);
  CU_ASSERT(result == EXIT_SUCCESS && err_msg == original_err_msg);
  ASSERT_PATHS_EQUAL(path, expected_path[0], path_count);

  result = boost_dijkstra(edges, 8, 5, 1, false, false, &path, &path_count, &err_msg);
  CU_ASSERT(result == EXIT_SUCCESS && err_msg == original_err_msg);
  ASSERT_PATHS_EQUAL(path, expected_path[1], path_count);

  result = boost_dijkstra(edges, 8, 1, 5, true, false, &path, &path_count, &err_msg);
  CU_ASSERT(result == EXIT_SUCCESS && err_msg == original_err_msg);
  ASSERT_PATHS_EQUAL(path, expected_path[2], path_count);

  result = boost_dijkstra(edges, 8, 5, 1, true, false, &path, &path_count, &err_msg);
  CU_ASSERT(result == EXIT_SUCCESS && err_msg == original_err_msg);
  ASSERT_PATHS_EQUAL(path, expected_path[3], path_count);

  result = boost_dijkstra(edges, 8, 1, 5, true, true, &path, &path_count, &err_msg);
  CU_ASSERT(result == EXIT_SUCCESS && err_msg == original_err_msg);
  ASSERT_PATHS_EQUAL(path, expected_path[4], path_count);

  result = boost_dijkstra(edges, 8, 5, 1, true, true, &path, &path_count, &err_msg);
  CU_ASSERT(result == EXIT_SUCCESS && err_msg == original_err_msg);
  ASSERT_PATHS_EQUAL(path, expected_path[5], path_count);

  free(edges);
  return;
}

int main()
{
  if(CU_initialize_registry() != CUE_SUCCESS)
  {
    printf("Cannot initialize test registry");
    return CU_get_error();
  }
  CU_pSuite pSuite = CU_add_suite("Dijkstra", NULL, NULL);
  CU_pTest pTest = CU_add_test(pSuite, "test_dijkstra", test_dijkstra);
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
