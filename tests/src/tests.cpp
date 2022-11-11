#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this
                          // in one cpp file
#include "catch.hpp"

extern "C" {
#include "btree.h"
#include "linked_list.h"
}

// See Catch2's documentation:
// https://github.com/catchorg/Catch2/blob/devel/docs/tutorial.md#scaling-up

TEST_CASE("linked_list") {
  // exercise 2
  {
    int sum;
    node *ns = make_node(
        1, make_node(2, make_node(3, make_node(4, make_node(5, NULL)))));

    sum = sum_squares(ns);
    REQUIRE(sum == 55);
    free_list(ns);
    // dangling pointer we need to set it to null
    ns = NULL;

    sum = sum_squares(ns);
    REQUIRE(sum == 0);

    ns = make_node(1, NULL);
    sum = sum_squares(ns);
    REQUIRE(sum == 1);
    free_list(ns);
    ns = NULL;
  }

  // exercise 3
  {
    int sum;
    node *ns = make_node(1, NULL);
    // ns contains one node with the value 1
    node *mns = map(ns, square);
    sum = sum_squares(mns);

    REQUIRE(sum == 1);
    free_list(ns);
    free_list(mns);
    // dangling pointer we need to set it to null
    ns = NULL;
    mns = NULL;

    ns = make_node(1, make_node(2, make_node(3, NULL)));
    // ns is 1->2->3
    mns = map(ns, square);
    free_list(ns);
    ns = NULL;
    // mns is 1->4->9
    sum = sum_squares(mns);

    free_list(mns);
    mns = NULL;

    // (1^2)^2 1 + (2^2)^2 + (3^2)^2 = 1^4 + 2^4 + 3^4 = 98
    REQUIRE(sum == 98);
  }
}

TEST_CASE("btree") {

  /*
//               20
//             /    \
//            /      \
//           5       30
//         /   \     /\
//        /     \   /  \
//       1      15 25  40
//            /          \
//           /            \
//          9             45
//        /   \          /
//       /     \        /
//      7      12      42
*/
  struct tree_node *root = NULL;

  root = Initialize(root);
  root = Insert(20, root);
  root = Insert(5, root);
  root = Insert(1, root);
  root = Insert(15, root);
  root = Insert(9, root);
  root = Insert(7, root);
  root = Insert(12, root);
  root = Insert(30, root);
  root = Insert(25, root);
  root = Insert(40, root);
  root = Insert(45, root);
  root = Insert(42, root);

  REQUIRE(Contains(20, root) == 1);
  REQUIRE(Contains(5, root) == 1);
  REQUIRE(Contains(1, root) == 1);
  REQUIRE(Contains(15, root) == 1);
  REQUIRE(Contains(9, root) == 1);
  REQUIRE(Contains(7, root) == 1);
  REQUIRE(Contains(12, root) == 1);
  REQUIRE(Contains(30, root) == 1);
  REQUIRE(Contains(25, root) == 1);
  REQUIRE(Contains(40, root) == 1);
  REQUIRE(Contains(45, root) == 1);
  REQUIRE(Contains(42, root) == 1);
  REQUIRE(Contains(2, root) == 0);
  REQUIRE(Contains(3, root) == 0);

  // (A)
  REQUIRE(Empty(Initialize(NULL)));

  // (B) and (C)

  root = Insert(3, root);

  REQUIRE(Contains(3, root) == 1);

  root = Remove(3, root);

  REQUIRE(Contains(20, root) == 1);
  REQUIRE(Contains(5, root) == 1);
  REQUIRE(Contains(1, root) == 1);
  REQUIRE(Contains(15, root) == 1);
  REQUIRE(Contains(9, root) == 1);
  REQUIRE(Contains(7, root) == 1);
  REQUIRE(Contains(12, root) == 1);
  REQUIRE(Contains(30, root) == 1);
  REQUIRE(Contains(25, root) == 1);
  REQUIRE(Contains(40, root) == 1);
  REQUIRE(Contains(45, root) == 1);
  REQUIRE(Contains(42, root) == 1);

  REQUIRE(Contains(2, root) == 0);
  REQUIRE(Contains(3, root) == 0);

  // (D) and (E)
  root = Insert(-1, root);
  root = Insert(-1, root);
  root = Remove(-1, root);
  REQUIRE(Contains(-1, root) == 1);
  root = Remove(-1, root);
  REQUIRE(Contains(-1, root) == 0);

  root = Remove(45, root);
  root = Remove(42, root);
  root = Insert(16, root);

  REQUIRE(Contains(20, root) == 1);
  REQUIRE(Contains(5, root) == 1);
  REQUIRE(Contains(1, root) == 1);
  REQUIRE(Contains(15, root) == 1);
  REQUIRE(Contains(9, root) == 1);
  REQUIRE(Contains(7, root) == 1);
  REQUIRE(Contains(12, root) == 1);
  REQUIRE(Contains(30, root) == 1);
  REQUIRE(Contains(25, root) == 1);
  REQUIRE(Contains(40, root) == 1);
  REQUIRE(Contains(45, root) == 0);
  REQUIRE(Contains(42, root) == 0);
  REQUIRE(Contains(16, root) == 1);

  //REQUIRE(Full(root) == 1);

  root = Remove(7, root);

  REQUIRE(Contains(16, root) == 1);
  REQUIRE(Contains(20, root) == 1);
  REQUIRE(Contains(5, root) == 1);
  REQUIRE(Contains(1, root) == 1);
  REQUIRE(Contains(15, root) == 1);
  REQUIRE(Contains(9, root) == 1);
  REQUIRE(Contains(7, root) == 0);
  REQUIRE(Contains(12, root) == 1);
  REQUIRE(Contains(30, root) == 1);
  REQUIRE(Contains(25, root) == 1);
  REQUIRE(Contains(40, root) == 1);
  REQUIRE(Contains(45, root) == 0);

  root = Remove(1, root);
  root = Remove(7, root);
  root = Remove(12, root);
  root = Remove(9, root);
  root = Remove(15, root);
  root = Remove(5, root);
  root = Remove(42, root);
  root = Remove(45, root);
  root = Remove(25, root);
  root = Remove(40, root);
  root = Remove(30, root);
  root = Remove(20, root);
  root = Remove(16, root);

  free(root);
}
