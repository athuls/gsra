#include "ebl_machines_test.h"

#ifdef __GUI__
#include "libeblearngui.h"
#endif

using namespace std;
using namespace ebl;

extern string *gl_data_dir;
extern string *gl_mnist_dir;
extern string *gl_mnist_errmsg;

void ebl_machines_test::setUp() {
}

void ebl_machines_test::tearDown() {
}

void ebl_machines_test::test_lenet5_mnist() {
  typedef double Tnet;
  typedef ubyte Tdata;
  typedef ubyte Tlab;
  bool display = true;
  uint ninternals = 1;
  cout << endl;
  // for testing purposes, we always initialize the randomization with 0 so 
  // that we know the exact results. 
  // in the real world, init_drand should be called with time(NULL) as argument.
  CPPUNIT_ASSERT_MESSAGE(*gl_mnist_errmsg, gl_mnist_dir != NULL);
  
  // load MNIST dataset
  mnist_datasource<Tnet,Tdata,Tlab>
    test_ds(gl_mnist_dir->c_str(), false, 1000),
    train_ds(gl_mnist_dir->c_str(), true, 2000);
  train_ds.set_balanced(true);
  train_ds.set_shuffle_passes(true);
  // set 2nd argument to true for focusing on hardest examples
  //  train_ds.set_weigh_samples(true, false, false, 0.01);
  train_ds.set_weigh_samples(false, false, false, 0.01);
  train_ds.set_epoch_show(100); // show progress every 100 samples
  train_ds.ignore_correct(true);
  
  // create 1-of-n targets with target 1.0 for shown class, -1.0 for the rest
  idx<Tnet> targets =
    create_target_matrix<Tnet>(1+idx_max(train_ds.labels), 1.0);
  uint nclasses = targets.dim(0);

  // create the network weights, network and trainer
  idxdim dims(train_ds.sample_dims()); // get order and dimensions of sample
  parameter<Tnet> theparam(60000); // create trainable parameter
  lenet5<Tnet> net(theparam, 32, 32, 5, 5, 2, 2, 5, 5, 2, 2, 120,
		   nclasses, true, false, true, false);
  cout << net.describe() << endl;

  l2_energy<Tnet> energy;
  class_answer<Tnet,Tdata,Tlab> answer(nclasses);
  trainable_module<Tnet,Tdata,Tlab> trainable(energy, net, NULL, NULL, &answer);
  supervised_trainer<Tnet, ubyte, ubyte> thetrainer(trainable, theparam);

#ifdef __GUI__
  //  labeled_datasource_gui<Tnet, ubyte, ubyte> dsgui(true);
//   dsgui.display(test_ds, 10, 10);
  supervised_trainer_gui<Tnet,Tdata,Tlab> stgui;
#endif

  // a classifier-meter measures classification errors
  classifier_meter trainmeter;
  classifier_meter testmeter;
   
  // initialize the network weights
  forget_param_linear fgp(1, 0.5);
  trainable.forget(fgp);

  // gradient parameters
  gd_param gdp(/* double eta*/ 0.0001,
	       /* double ln */ 	0.0,
	       /* double l1 */ 	0.0,
	       /* double l2 */ 	0.0,
	       /* intg dtime */ 	0,
	       /* double iner */0.0, 
	       /* double anneal_value */ 0.001,
	       /* intg anneal_period */ 2000,
	       /* double g_t*/ 	0.0);
  cout << gdp << endl;
  infer_param infp;
  fixed_init_drand(); // fixed randomization

#ifdef __DEBUGMEM__
  pretty_memory();
#endif
  // estimate second derivative on 100 iterations, using mu=0.02
  // and set individual espilons
  //printf("computing diagonal hessian and learning rates\n");
  thetrainer.compute_diaghessian(train_ds, 100, 0.02);
//   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.985363, 
// 			       idx_min(thetrainer.param.epsilons), 0.000001);
//   CPPUNIT_ASSERT_DOUBLES_EQUAL(49.851524, 
// 			       idx_max(thetrainer.param.epsilons), 0.000001);

	
#ifdef __GUI__
  if (display) {
    stgui.display_datasource(thetrainer, test_ds, infp, 10, 10);
    stgui.display_internals(thetrainer, test_ds, infp, gdp, ninternals);
  }
#endif
  
  // do training iterations
  cout << "training with " << train_ds.size() << " training samples and ";
  cout << test_ds.size() << " test samples" << endl;
	
  thetrainer.test(train_ds, trainmeter, infp);
  thetrainer.test(test_ds, testmeter, infp);
#ifdef __GUI__
  if (display) {
    stgui.display_datasource(thetrainer, test_ds, infp, 10, 10);
    stgui.display_internals(thetrainer, test_ds, infp, gdp, ninternals);
  }
#endif
  // this goes at about 25 examples per second on a PIIIM 800MHz
  for (int i = 0; i < 5; ++i) {
#ifdef __DEBUGMEM__
    pretty_memory();
#endif
    thetrainer.train(train_ds, trainmeter, gdp, 1, infp);
    // ostringstream name(""); name << "pickings_" << i+1;
    // train_ds.save_pickings(name.str().c_str());
    thetrainer.test(train_ds, trainmeter, infp);
    thetrainer.test(test_ds, testmeter, infp);
    thetrainer.compute_diaghessian(train_ds, 100, 0.02);

#ifdef __GUI__
    if (display) {
      //      dsgui.display_pickings(train_ds, 3, 3);
      stgui.display_datasource(thetrainer, test_ds, infp, 10, 10);
      stgui.display_datasource(thetrainer, test_ds, infp, 10, 10);
      stgui.display_internals(thetrainer, test_ds, infp, gdp, ninternals);
    }
#endif
  }
  CPPUNIT_ASSERT_DOUBLES_EQUAL(100.0, // old: 97.00
			       ((trainmeter.total_correct * 100) 
				/ (double) trainmeter.size), 1.5);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(95.4262, // 96.4630, // old: 95.90
			       ((testmeter.total_correct * 100) 
				/ (double) testmeter.size), 1.5);
}


void ebl_machines_test::test_full_table() {
  idx<intg> m = full_table(2, 3);
  CPPUNIT_ASSERT_EQUAL((intg) 0, m.get(0, 0));
  CPPUNIT_ASSERT_EQUAL((intg) 0, m.get(0, 1));
  CPPUNIT_ASSERT_EQUAL((intg) 1, m.get(1, 0));
  CPPUNIT_ASSERT_EQUAL((intg) 0, m.get(1, 1));
  CPPUNIT_ASSERT_EQUAL((intg) 0, m.get(2, 0));
  CPPUNIT_ASSERT_EQUAL((intg) 1, m.get(2, 1));
  CPPUNIT_ASSERT_EQUAL((intg) 1, m.get(3, 0));
  CPPUNIT_ASSERT_EQUAL((intg) 1, m.get(3, 1));
  CPPUNIT_ASSERT_EQUAL((intg) 0, m.get(4, 0));
  CPPUNIT_ASSERT_EQUAL((intg) 2, m.get(4, 1));
  CPPUNIT_ASSERT_EQUAL((intg) 1, m.get(5, 0));
  CPPUNIT_ASSERT_EQUAL((intg) 2, m.get(5, 1));
}
