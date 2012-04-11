#ifndef EBL_PREPROCESSING_TEST_H_
#define EBL_PREPROCESSING_TEST_H_

#include <cppunit/extensions/HelperMacros.h>
#include "ebl_preprocessing.h"
#include "ebl_layers.h"
#include "ebl_tester.h"

//! Test class for Ebm class
class ebl_preprocessing_test : public CppUnit::TestFixture  {
  CPPUNIT_TEST_SUITE(ebl_preprocessing_test);
  CPPUNIT_TEST(test_rgb_to_yp);
  CPPUNIT_TEST_SUITE_END();

private:
  // member variables

public:
  //! This function is called before each test function is called.
  void setUp();
  //! This function is called after each test function is called.
  void tearDown();

  // Test functions
  void test_rgb_to_yp();
};

#endif /* EBL_PREPROCESSING_TEST_H_ */
