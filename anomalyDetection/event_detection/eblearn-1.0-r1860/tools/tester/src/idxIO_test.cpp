#include "idxIO_test.h"
#include "libidx.h"

#ifdef __WINDOWS__
#define TEST_FILE "C:/Windows/Temp/eblearn_tester_matrix.mat"
#else
#define TEST_FILE "/tmp/eblearn_tester_matrix.mat"
#endif

extern string *gl_data_dir;
extern string *gl_data_errmsg;

using namespace std;
using namespace ebl;

void idxIO_test::setUp() {
}

void idxIO_test::tearDown() {
}

template<class T> void test_save_load_matrix() {
  idx<T> m(9, 9);
  string fname= TEST_FILE;

  // initialize values
  double v = 0.1;
  { idx_aloop1(i, m, T) {
      *i = (T) v;
      v++;
    }
  }
  //m.printElems(); cout << endl;
  try {
    save_matrix(m, fname);
    idx<T> l = load_matrix<T>(fname);
    // test values
    v = 0.1;
    { idx_aloop1(i, l, T) {
	CPPUNIT_ASSERT_EQUAL((T)v, *i);
	v++;
      }
    }
  } catch(string &err) {
    cerr << err << endl;
    CPPUNIT_ASSERT(false); // error
  }
}

void idxIO_test::test_save_load_matrix_ubyte() {
  test_save_load_matrix<ubyte>();
}

void idxIO_test::test_save_load_matrix_int() {
  test_save_load_matrix<int>();
}

void idxIO_test::test_save_load_matrix_float() {
  test_save_load_matrix<float>();
}

void idxIO_test::test_save_load_matrix_double() {
  test_save_load_matrix<double>();
}

void idxIO_test::test_save_load_matrix_long() {
  idx<intg> test(3,3);
  test.set(2147483647, 0, 0);
  test.set(2147483646, 0, 1);
  test.set(2147483645, 0, 2);
  test.set(4, 1, 0);
  test.set(5, 1, 1);
  test.set(6, 1, 2);
  test.set(-2147483647, 2, 0);
  test.set(-2147483646, 2, 1);
  test.set(-2147483645, 2, 2);
  try {
    save_matrix(test, TEST_FILE);
    idx<intg> m = load_matrix<intg>(TEST_FILE);
    { idx_aloop2(i, test, intg, j, m, intg) {
	CPPUNIT_ASSERT_EQUAL((intg) *j, (intg) *i);
      }
    }
  } catch(string &err) {
    cerr << err << endl;
    CPPUNIT_ASSERT(false); // err
  }
}

void idxIO_test::test_save_load_matrix_matrix() {
  try {
    CPPUNIT_ASSERT_MESSAGE(*gl_data_errmsg, gl_data_dir != NULL);
    string root = *gl_data_dir;
    string sim1, sim2, sim3, sall;
    sim1 << root << "/barn.png";
    sim2 << root << "/lena.png";
    sim3 << root << "/2008_007714.jpg";
    sall << root << "/all.mat";
    idx<float> im1 = load_image<float>(sim1);
    idx<float> im2 = load_image<float>(sim2);
    idx<float> im3 = load_image<float>(sim3);
    
    idxs<float> all(3);
    all.set(im1, 0);
    all.set(im2, 1);
    all.set(im3, 2);
    
    save_matrices(all, sall);
    idxs<float> all2 = load_matrices<float>(sall, false);
    
    idx<float> m1 = all.get(0);
    idx<float> m2 = all.get(1);
    idx<float> m3 = all.get(2);
    float sum = idx_sum(im1) - idx_sum(m1) + idx_sum(im2) - idx_sum(m2)
      + idx_sum(im3) - idx_sum(m3);
    CPPUNIT_ASSERT_EQUAL(sum, (float) 0);
  } catch(string &err) {
    cerr << err << endl;
    CPPUNIT_ASSERT(false); // err
  }
}

void idxIO_test::test_save_load_matrices() {
  try {
    CPPUNIT_ASSERT_MESSAGE(*gl_data_errmsg, gl_data_dir != NULL);
    string root;
    root << *gl_data_dir << "/tables";
    string sim1, sim2, sim3, sall;
    sim1 << root <<"/table_2_6_connect_6_fanin_0_density_0.5_uv0_u3_v6.mat";
    sim2 << root <<"/table_32_96_connect_1920_fanin_20_density_0.62_random.mat";
    sim3 << root <<"/table_38_68_connect_2040_fanin_30_density_0.79_random.mat";
    sall << root << "/all.mat";
    list<string> l;
    l.push_back(sim1);
    l.push_back(sim2);
    l.push_back(sim3);
    save_matrices<float>(l, sall);
    idxs<float> all2 = load_matrices<float>(sall, true);
    // check 1st matrix is ok
    idx<float> m1 = load_matrix<float>(sim1);
    idx<float> m2 = all2.get(0);
    CPPUNIT_ASSERT_EQUAL(idx_sum(m1) - idx_sum(m2), (float) 0);
    // check 2nd matrix is ok
    m1 = load_matrix<float>(sim2);
    m2 = all2.get(1);
    CPPUNIT_ASSERT_EQUAL(idx_sum(m1) - idx_sum(m2), (float) 0);
    // check 3rd matrix is ok
    m1 = load_matrix<float>(sim3);
    m2 = all2.get(2);
    CPPUNIT_ASSERT_EQUAL(idx_sum(m1) - idx_sum(m2), (float) 0);
    // check has_multiple_matrices
    CPPUNIT_ASSERT_EQUAL(has_multiple_matrices(sall.c_str()), true);
    CPPUNIT_ASSERT_EQUAL(has_multiple_matrices(sim1.c_str()), false);
  } catch(string &err) {
    cerr << err << endl;
    CPPUNIT_ASSERT(false); // err
  }
}
