#include "idxops_test.h"

using namespace ebl;

void idxops_test::setUp() {
}

void idxops_test::tearDown() {
}

void idxops_test::test_idx_min_max() {
  idx<double> m2(2, 2);
  idx_clear(m2);
  m2.set(0.0, 0, 0);
  m2.set(42.0, 0, 1);
  m2.set(1.0, 1, 0);
  m2.set(-42.0, 1, 1);
  CPPUNIT_ASSERT(idx_max(m2) == 42.0);
  CPPUNIT_ASSERT(idx_min(m2) == -42.0);
}

void idxops_test::test_idx_sums() {
		idx<double> m(3, 4);
		idx<double> m0;
		idx<unsigned char> mc(3, 4);
		idx<unsigned char> mc0;
		idx_clear(m);
		idx_clear(m0);
		idx_clear(mc);
		idx_clear(mc0);
		double i;
		i = -1;
		idx_aloop2(lm, m, double, lmc, mc, unsigned char) {
			*lm = i;
			*lmc = (unsigned char) i;
			i++;
		}
		mc.set(0, 0, 0);
		//m.printElems();
		//mc.printElems();
		CPPUNIT_ASSERT(idx_sum(m) == 54);
		CPPUNIT_ASSERT(idx_sumsqr(m) == 386);
		CPPUNIT_ASSERT(idx_sumacc(m, m0) == 54);
		CPPUNIT_ASSERT(m0.get() == 54);
		CPPUNIT_ASSERT(idx_sumacc(m, m0) == 108);
		CPPUNIT_ASSERT(m0.get() == 108);
		// Testing non double or float versions.
		CPPUNIT_ASSERT((ubyte)ROUND(idx_sumacc(mc, mc0)) == 55);
		CPPUNIT_ASSERT((ubyte)ROUND(mc0.get()) == 55);
		CPPUNIT_ASSERT((ubyte)ROUND(idx_sumacc(mc, mc0)) == 110);
		CPPUNIT_ASSERT((ubyte)ROUND(mc0.get()) == 110);
		// TODO: add tests for non contiguous idx and order of 1
}

void idxops_test::test_idx_sortdown() {
	idx<double> m(5);
	idx<unsigned char> mc(5);
	double i;
	i = 1;
	idx_aloop2(lm, m, double, lmc, mc, unsigned char) {
		*lm = i;
		*lmc = 5 - (unsigned char) i;
		i++;
	}
//	m.printElems();
//	mc.printElems();
	idx_sortdown(m, mc);
//	m.printElems();
//	mc.printElems();
	CPPUNIT_ASSERT_EQUAL((double) 5, m.get(0));
	CPPUNIT_ASSERT_EQUAL((double) 4, m.get(1));
	CPPUNIT_ASSERT_EQUAL((double) 3, m.get(2));
	CPPUNIT_ASSERT_EQUAL((double) 2, m.get(3));
	CPPUNIT_ASSERT_EQUAL((double) 1, m.get(4));
	CPPUNIT_ASSERT_EQUAL((unsigned char) 0, mc.get(0));
	CPPUNIT_ASSERT_EQUAL((unsigned char) 1, mc.get(1));
	CPPUNIT_ASSERT_EQUAL((unsigned char) 2, mc.get(2));
	CPPUNIT_ASSERT_EQUAL((unsigned char) 3, mc.get(3));
	CPPUNIT_ASSERT_EQUAL((unsigned char) 4, mc.get(4));
}

void idxops_test::test_idx_sqrdist() {
	idx<double> m(5);
	idx<double> mc(5);
	double i;
	i = 1;
	idx_aloop2(lm, m, double, lmc, mc, double) {
		*lm = i;
		*lmc = 5.0 - i;
		i++;
	}
	//m.printElems();
	//mc.printElems();
	i = idx_sqrdist(m, mc);
	//cout << "sqrdist: " << i << endl;
	CPPUNIT_ASSERT_EQUAL((double) 45, i);
}

void idxops_test::test_idx_exp() {
	  idx<double> m2(2, 2);
	  idx_clear(m2);
	  m2.set(0.0, 0, 0);
	  m2.set(1.0, 0, 1);
	  m2.set(-1, 1, 0);
	  m2.set(log((double)2), 1, 1);
	  idx_exp(m2);
	  CPPUNIT_ASSERT(m2.get(0,0) == 1);
	  CPPUNIT_ASSERT(m2.get(0,1) == exp((double)1));
	  CPPUNIT_ASSERT(m2.get(1,0) == 1/exp((double)1.0));
	  CPPUNIT_ASSERT(m2.get(1,1) == 2);
}

void idxops_test::test_idx_m2oversample() {
	idx<double> m(2, 2);
	idx<double> o(4, 4);
	m.set(1.0, 0, 0);
	m.set(2.0, 0, 1);
	m.set(3.0, 1, 0);
	m.set(4.0, 1, 1);
	//m.printElems(); cout << endl;
	idx_m2oversample(m, 2, 2, o);
	//o.printElems(); cout << endl;
	CPPUNIT_ASSERT_EQUAL(1.0, o.get(0, 0));
	CPPUNIT_ASSERT_EQUAL(1.0, o.get(0, 1));
	CPPUNIT_ASSERT_EQUAL(2.0, o.get(0, 2));
	CPPUNIT_ASSERT_EQUAL(2.0, o.get(0, 3));
	CPPUNIT_ASSERT_EQUAL(1.0, o.get(1, 0));
	CPPUNIT_ASSERT_EQUAL(1.0, o.get(1, 1));
	CPPUNIT_ASSERT_EQUAL(2.0, o.get(1, 2));
	CPPUNIT_ASSERT_EQUAL(2.0, o.get(1, 3));
	CPPUNIT_ASSERT_EQUAL(3.0, o.get(2, 0));
	CPPUNIT_ASSERT_EQUAL(3.0, o.get(2, 1));
	CPPUNIT_ASSERT_EQUAL(4.0, o.get(2, 2));
	CPPUNIT_ASSERT_EQUAL(4.0, o.get(2, 3));
	CPPUNIT_ASSERT_EQUAL(3.0, o.get(3, 0));
	CPPUNIT_ASSERT_EQUAL(3.0, o.get(3, 1));
	CPPUNIT_ASSERT_EQUAL(4.0, o.get(3, 2));
	CPPUNIT_ASSERT_EQUAL(4.0, o.get(3, 3));
}

void idxops_test::test_idx_m2squdotm1() {
	idx<double> m1(2, 2);
	idx<double> m2(2);
	idx<double> m3(2);
	m1.set(2.0, 0, 0);
	m1.set(2.0, 0, 1);
	m1.set(3.0, 1, 0);
	m1.set(3.0, 1, 1);
	m2.set(1.0, 0);
	m2.set(2.0, 1);
	idx_clear(m3);
	//m1.printElems(); cout << endl;
	//m2.printElems(); cout << endl;
	idx_m2squdotm1(m1, m2, m3);
	//m3.printElems(); cout << endl;
	CPPUNIT_ASSERT_EQUAL(12.0, m3.get(0));
	CPPUNIT_ASSERT_EQUAL(27.0, m3.get(1));
}

void idxops_test::test_idx_m2extm2acc() {
	idx<double> m1(2, 2);
	idx<double> m2(2, 2);
	idx<double> m3(2, 2, 2, 2);
	m1 = m1.transpose(0, 1);
	m2 = m2.transpose(0, 1);
	m3 = m3.transpose(1, 2);
	m1.set(1.0, 0, 0);
	m1.set(2.0, 0, 1);
	m1.set(3.0, 1, 0);
	m1.set(4.0, 1, 1);
	idx_fill(m2, 2.0);
	idx_fill(m3, 1.0);
//	m1.printElems(); cout << endl;
//	m2.printElems(); cout << endl;
//	m3.printElems(); cout << endl;
	idx_m2extm2acc(m1, m2, m3);
//	m3.printElems(); cout << endl;
	CPPUNIT_ASSERT_EQUAL(3.0, m3.get(0, 0, 0, 0));
	CPPUNIT_ASSERT_EQUAL(3.0, m3.get(0, 0, 0, 1));
	CPPUNIT_ASSERT_EQUAL(3.0, m3.get(0, 0, 1, 0));
	CPPUNIT_ASSERT_EQUAL(3.0, m3.get(0, 0, 1, 1));
	CPPUNIT_ASSERT_EQUAL(5.0, m3.get(0, 1, 0, 0));
	CPPUNIT_ASSERT_EQUAL(5.0, m3.get(0, 1, 0, 1));
	CPPUNIT_ASSERT_EQUAL(5.0, m3.get(0, 1, 1, 0));
	CPPUNIT_ASSERT_EQUAL(5.0, m3.get(0, 1, 1, 1));
	CPPUNIT_ASSERT_EQUAL(7.0, m3.get(1, 0, 0, 0));
	CPPUNIT_ASSERT_EQUAL(7.0, m3.get(1, 0, 0, 1));
	CPPUNIT_ASSERT_EQUAL(7.0, m3.get(1, 0, 1, 0));
	CPPUNIT_ASSERT_EQUAL(7.0, m3.get(1, 0, 1, 1));
	CPPUNIT_ASSERT_EQUAL(9.0, m3.get(1, 1, 0, 0));
	CPPUNIT_ASSERT_EQUAL(9.0, m3.get(1, 1, 0, 1));
	CPPUNIT_ASSERT_EQUAL(9.0, m3.get(1, 1, 1, 0));
	CPPUNIT_ASSERT_EQUAL(9.0, m3.get(1, 1, 1, 1));
}

void idxops_test::test_idx_copy(){
	idx<double> m1(3, 5, 4);
	idx<double> m2(3, 5, 4);
	idx<float> m3(3, 5, 4);
	idx<intg> m4(3, 5, 4);
	idx<ubyte> m5(3, 5, 4);

	dseed(2);
	idx_bloop1(i, m1, double){
	  idx_bloop1(ii, i, double){
	    idx_bloop1(iii, ii, double){
	      iii.set(255*drand());
	    }
	  }
	}

	idx_copy(m1, m2);
	for(int i = 0; i < 3; i++)
	  for(int j = 0; j < 5; j++)
	    for(int k = 0; k < 4; k++)
	      CPPUNIT_ASSERT_EQUAL(m1.get(i,j,k), m2.get(i,j,k));

	idx_copy(m1, m3);
	for(int i = 0; i < 3; i++)
	  for(int j = 0; j < 5; j++)
	    for(int k = 0; k < 4; k++)
	      CPPUNIT_ASSERT_EQUAL((float)m1.get(i,j,k), m3.get(i,j,k));

	idx_copy(m1, m4);
	for(int i = 0; i < 3; i++)
	  for(int j = 0; j < 5; j++)
	    for(int k = 0; k < 4; k++)
	      CPPUNIT_ASSERT_EQUAL((int)floor(m1.get(i,j,k)), 
				   (int)m4.get(i,j,k));

	idx_copy(m1, m5);
	for(int i = 0; i < 3; i++)
	  for(int j = 0; j < 5; j++)
	    for(int k = 0; k < 4; k++)
	      CPPUNIT_ASSERT_EQUAL((ubyte)floor(m1.get(i,j,k)), m5.get(i,j,k));

	void* m6 = (void*) new idx<double>(3, 5, 4);
	idx_copy(m1, *(static_cast<idx<double>*>(m6)));
	for(int i = 0; i < 3; i++)
	  for(int j = 0; j < 5; j++)
	    for(int k = 0; k < 4; k++)
	      CPPUNIT_ASSERT_EQUAL(m1.get(i,j,k), 
				   static_cast<idx<double>*>(m6)->get(i,j,k));

	idx<ubyte> m7(3, 5, 4);
	idx_copy(*(static_cast<idx<double>*>(m6)), m7);
	for(int i = 0; i < 3; i++)
	  for(int j = 0; j < 5; j++)
	    for(int k = 0; k < 4; k++)
	      CPPUNIT_ASSERT_EQUAL(m7.get(i,j,k), 
				   (ubyte) static_cast<idx<double>*>
				   (m6)->get(i,j,k));


	idx<double>m8(3, 5, 2);
	m8 = m1.narrow(2,2,2);
	idx<ubyte>m9(3, 5, 2);
	idx_clear(m9);
	idx_copy(m8, m9);
	//	m8.printElems();
	//	printf("\n ********************************************* \n");
	//	m9.printElems();
	for(int i = 0; i < 3; i++)
	  for(int j = 0; j < 5; j++)
	    for(int k = 0; k < 2; k++)
	      CPPUNIT_ASSERT_EQUAL((ubyte)m8.get(i,j,k), m9.get(i,j,k));

	idx<ubyte>m10(3, 5, 4);
	idx<ubyte>m11(3, 5, 2);
	m11 = m10.narrow(2, 2, 2);
	idx_copy(m8, m11);
	for(int i = 0; i < 3; i++)
	  for(int j = 0; j < 5; j++)
	    for(int k = 0; k < 2; k++)
	      CPPUNIT_ASSERT_EQUAL((ubyte)m8.get(i,j,k), m11.get(i,j,k));
	delete static_cast<idx<double>*>(m6);
}

void idxops_test::test_idx_copy2(){
  idx<double> m1(5, 3, 4);

  dseed(2);
  idx_bloop1(i, m1, double){
    idx_bloop1(ii, i, double){
      idx_bloop1(iii, ii, double){
	iii.set(255*drand());
      }
    }
  }

  idx<double>m8(5, 3, 2);
  m8 = m1.narrow(2,2,0);
  idx<ubyte>m9(5, 3, 2);

  idx_copy(m8, m9);
  //m8.printElems();
  //printf("\n ********************************************* \n");
  //m9.printElems();
  for(int i = 0; i < 5; i++)
    for(int j = 0; j < 3; j++)
      for(int k = 0; k < 2; k++)
	CPPUNIT_ASSERT_EQUAL((ubyte)m8.get(i,j,k), m9.get(i,j,k));

}

void idxops_test::test_idx_abs() {
  {
    idx<double> m2(20, 20);
    idx<double> m2a(20, 20);
    idx_clear(m2);
    idx_clear(m2a);

    dseed(2);
    { idx_bloop1(i,m2,double){
      idx_bloop1(ii,i,double){
	ii.set(drand(-2,2));
      }
      }}
    idx_abs(m2,m2a);
    { idx_bloop2(i,m2,double,j,m2a,double){
      idx_bloop2(ii,i,double,jj,j,double){
	if(ii.get() < 0.0){
	  CPPUNIT_ASSERT(-(ii.get()) == jj.get());
	}else{
	  CPPUNIT_ASSERT(ii.get() == jj.get());
	}
      }
      }}
  }
  {
    // repeat for int
    idx<int> m2(20, 20);
    idx<int> m2a(20, 20);
    idx_clear(m2);
    idx_clear(m2a);

    dseed(2);
    { idx_bloop1(i,m2,int){
      idx_bloop1(ii,i,int){
	ii.set((int)drand(-2,2));
      }
      }}
    idx_abs(m2,m2a);
    { idx_bloop2(i,m2,int,j,m2a,int){
      idx_bloop2(ii,i,int,jj,j,int){
	if(ii.get() < 0.0){
	  CPPUNIT_ASSERT(-(ii.get()) == jj.get());
	}else{
	  CPPUNIT_ASSERT(ii.get() == jj.get());
	}
      }
      }}
  }
}

void idxops_test::test_idx_m2dotm1() {
  idx<double> d2(2, 5);
  idx<double> d1(5);
  idx<double> out(2);
  
  idx_fill(d2, 2.0);
  idx_fill(d1, 1.0);
  idx_m2dotm1(d2, d1, out);
  idx_bloop1(o, out, double)
    CPPUNIT_ASSERT_EQUAL(10.0, o.get());
}

void idxops_test::test_huge_vec() {

  // this would not run on many systems
  // disable until a cmake trick is implemented
  //   intg size = 0;
  //   if (LONG_MAX > INT_MAX){
  //     size = INT_MAX/2; // for now leave like this, I can't make things run on humair now.
  //   }
  //   else{
  //     size = INT_MAX/2;
  //   }
  //   idx<double> hv(size);
  //   idx_fill(hv, 1.0);
  //   // dot with itself
  //   double dd = idx_dot(hv,hv);
  //   CPPUNIT_ASSERT((int)dd == hv.footprint());
}


// int testBlas01() {
// 	printf("\n****************************************************************\n");
// 	printf("*** testBlas01\n");

// 	idx<double> m1(2, 5);
// 	idx<double> m2(2, 5);
// #if USING_STL_ITERS
// 	int ii = 0;
// 	idx_aloop2(lm1,m1,double,lm2,m2,double) {*lm1 = *lm2 = ++ii;}
// #else
// 	idx_aloop2(lm1, m1, double, lm2, m2, double) {
// 		*lm1 = *lm2 = (double)lm1.i;
// 	}
// #endif
// 	printf("idx_dot(m1,m2)=%g (should be 285)\n", idx_dot(m1, m2));

// 	// non contiguous
// 	idx<double> nm1 = m1.narrow(1, 3, 1);
// 	idx<double> nm2 = m2.narrow(1, 3, 1);
// 	printf("idx_dot(nm1,nm2)=%g (should be 163)\n", idx_dot(nm1, nm2));

// 	printf("==== norm_columns(m1)\n");
// 	norm_columns(m1);
// 	idx_bloop1(lm, m1, double) {
// 		idx_bloop1(llm, lm, double) {
// 			printf("%g, ", llm.get());
// 		}
// 		printf("\n");
// 	}

// 	return 0;
// }

// int testBlas02() {
// 	printf("\n****************************************************************\n");
// 	printf("*** testBlas02\n");

// 	{
// 		printf("==== idx_m2dotm1(m,x,y)\n");
// 		double mdata[] = { 1000, 100, 10, 1, -1000, -100, -10, -1, 1e7, 1e6,
// 				1e5, 1e4 };
// 		// idx<double> z(3,6);
// 		// idx<double> m = z.narrow(1,4,1);
// 		idx<double> z(4, 3);
// 		idx<double> m = z.transpose(0, 1);
// 		double xdata[] = { 2, 3, 4, 6 };
// 		idx<double> x(4);
// 		idx<double> y(3);
// 		int i;
// 		i = 0;
// 		idx_aloop1(lm, m, double) {
// 			*lm = mdata[i++];
// 		}
// 		i = 0;
// 		idx_aloop1(lx, x, double) {
// 			*lx = xdata[i++];
// 		}
// 		m.pretty(stdout);
// 		idx_m2dotm1(m, x, y);
// 		y.pretty(stdout);
// 		idx_aloop1(ly, y, double) {
// 			printf("%g, ", *ly);
// 		}
// 	}

// 	{
// 		printf("==== idx_m1extm1(x,y,m)\n");
// 		idx<double> m(3, 4);
// 		idx<double> x(3);
// 		idx<double> y(4);
// 		double i;
// 		i = 1;
// 		idx_aloop1(lx, x, double) {
// 			*lx = i++;
// 		}
// 		i = 1;
// 		idx_aloop1(ly, y, double) {
// 			*ly = i++;
// 		}
// 		idx_m1extm1(x, y, m);
// 		idx_aloop1(lm, m, double) {
// 			printf("%g, ", *lm);
// 		}
// 		printf("\n");
// 	}
// 	return 0;
// }
