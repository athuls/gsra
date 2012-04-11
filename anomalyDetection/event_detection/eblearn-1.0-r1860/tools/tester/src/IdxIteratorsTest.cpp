#include "IdxIteratorsTest.h"
#include <iostream>
#include <ctime>
//#include <parallel/algorithm>
#include <iterator>

using namespace std;
using namespace ebl;

void IdxIteratorsTest::setUp() {
}

void IdxIteratorsTest::tearDown() {
}

void IdxIteratorsTest::test_scalariter() {
#if USING_STL_ITERS == 1
  idx<double> m1(2, 2);
  idx<double> m2(2, 2);
  double c = 42;
  idx_clear(m1);
  idx_clear(m2);
  m2.set(0.0, 0, 0);
  m2.set(42.0, 0, 1);
  m2.set(1.0, 1, 0);
  m2.set(-42.0, 1, 1);
  ScalarIter<double> pinp(m2); ScalarIter<double> pout(m1);
  idx_aloop2_on(pinp,m2,pout,m1) { *pout = *pinp + c; }
  //m1.printElems(); cout << endl;
  //m2.printElems(); cout << endl;
  CPPUNIT_ASSERT_EQUAL(42.0, m1.get(0, 0));
  CPPUNIT_ASSERT_EQUAL(84.0, m1.get(0, 1));
  CPPUNIT_ASSERT_EQUAL(43.0, m1.get(1, 0));
  CPPUNIT_ASSERT_EQUAL(0.0,  m1.get(1, 1));
#endif
}

void IdxIteratorsTest::test_ElemIter_shifting(){
  typedef double real;
  idx<real> m(3, 5, 2);
  int ii = 0, jj = 0, kk = 0;

  for( idx<real>::scalar_iterator iter(m); iter.notdone(); ++iter){
    *iter = ii++;
  }

  int tpInds[3] = {1,0,2}; 
  idx<real> idx2= m.narrow(2,1,0).transpose(tpInds); // .narrow(1,3,1);

  ii = 0;
  jj = 0;
  kk = 0;
  for( idx<real>::scalar_iterator iter(idx2); iter.notdone(); ++iter){
    CPPUNIT_ASSERT_EQUAL(idx2.get(ii, jj, kk), *iter);
    if (++kk >= idx2.dim(2)) {
      ++jj;
      kk = 0;
    }
    if (jj >= idx2.dim(1)) {
      ++ii;
      jj = 0;
    }
  }

  ii = idx2.dim(0) - 1;
  jj = idx2.dim(1) - 1;
  kk = idx2.dim(2) - 1;
  for( idx<real>::reverse_scalar_iterator iter(idx2); iter.notdone(); ++iter){
    CPPUNIT_ASSERT_EQUAL(idx2.get(ii, jj, kk), *iter);
    if (--kk < 0) {
      --jj;
      kk = idx2.dim(2) - 1;
    }
    if (jj < 0) {
      --ii;
      jj = idx2.dim(1) -1;
    }
  }
	
  ii = 0;
  jj = 0;
  kk = 0;
  int iir = idx2.dim(0) - 1;
  int jjr = idx2.dim(1) - 1;
  int kkr = idx2.dim(2) - 1;
  idx<real>::reverse_scalar_iterator riter(idx2);
  idx<real>::scalar_iterator iter(idx2);
  for( ; riter.notdone(); ++riter, ++iter ){

    // forward iter
    CPPUNIT_ASSERT_EQUAL(idx2.get(ii, jj, kk), *iter);
    if (++kk >= idx2.dim(2)) {
      ++jj;
      kk = 0;
    }
    if (jj >= idx2.dim(1)) {
      ++ii;
      jj = 0;
    }

    // reverse iter
    CPPUNIT_ASSERT_EQUAL(idx2.get(iir, jjr, kkr), *riter);
    if (--kkr < 0) {
      --jjr;
      kkr = idx2.dim(2) - 1;
    }
    if (jjr < 0) {
      --iir;
      jjr = idx2.dim(1) -1;
    }
  }
	
  ii = 0;
  jj = 0;
  kk = 0;
  for( idx<real>::scalar_iterator sii(idx2), sjj(idx2); 
       sii.notdone(); ++sii, ++sjj ){
    // forward iter
    CPPUNIT_ASSERT_EQUAL(idx2.get(ii, jj, kk), *sii);
    CPPUNIT_ASSERT_EQUAL(idx2.get(ii, jj, kk), *sjj);
    if (++kk >= idx2.dim(2)) {
      ++jj;
      kk = 0;
    }
    if (jj >= idx2.dim(1)) {
      ++ii;
      jj = 0;
    }
  }
}

void IdxIteratorsTest::test_ElemIter_parallelism(){
//   idx<unsigned char> idx1(50, 1024, 1024); // 50 meg image stack
	
//   clock_t startTime = clock();
//   std::generate( idx1.scalars_begin(), idx1.scalars_end(), &std::rand );
//   clock_t duration = clock() - startTime;
//   cout<<"executed in "<<double(duration)/CLOCKS_PER_SEC<<endl;
//   CPPUNIT_ASSERT(false); // TODO: create automatic test
}

void IdxIteratorsTest::test_DimensionIterator_shifting(){
  idx<double> idx1(2,3,4);
  for( int ii = 0; ii < idx1.dim(0); ++ii ){
    for( int jj = 0; jj < idx1.dim(1); ++jj ){
      for( int kk = 0; kk < idx1.dim(2); ++kk ){
	idx1.set( ii*100 + jj*10 + kk, ii,jj,kk);
      }
    }
  }
	
  typedef idx<double>::dimension_iterator DIter;
  for( DIter ii(idx1,0); ii.notdone(); ++ii){
    CPPUNIT_ASSERT_EQUAL(3, (int) ii->dim(0));
    CPPUNIT_ASSERT_EQUAL(4, (int) ii->dim(1));
  }

  for( DIter jj(idx1,1); jj.notdone(); ++jj){
    CPPUNIT_ASSERT_EQUAL(2, (int) jj->dim(0));
    CPPUNIT_ASSERT_EQUAL(4, (int) jj->dim(1));
  }
	
  for( DIter kk(idx1, 2); kk.notdone(); ++kk){
    CPPUNIT_ASSERT_EQUAL(2, (int) kk->dim(0));
    CPPUNIT_ASSERT_EQUAL(3, (int) kk->dim(1));
  }
}
