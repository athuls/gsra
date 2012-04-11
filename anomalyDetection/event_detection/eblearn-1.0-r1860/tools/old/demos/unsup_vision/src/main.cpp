#include "libeblearn.h"

#ifdef __GUI__
#include "libeblearngui.h"
#endif

using namespace std;
using namespace ebl; // all eblearn objects are under the ebl namespace

#ifdef __GUI__
MAIN_QTHREAD(int, argc, char **, argv) { // macro to enable multithreaded gui
#else
int main(int argc, char **argv) { // regular main without gui
#endif
  cout << "* unsupervised vision demo *" << endl;
  init_drand(time(NULL)); // initialize random seed

  //! load MNIST datasets: trize for training set and tesize for testing set
  mnist_datasource<ubyte,ubyte> train_ds, test_ds;
  load_mnist_dataset(argv[1], train_ds, test_ds, 2000, 1000);

  //! create the network weights, network and trainer
  parameter theparam(60000); // create trainable parameter
  linear_bias_tanh_d encoder();
  linear_module decoder(parameter, in, out);
  double alphae = 1.0, alphaz = 1.0, alphad = 1.0, l1_deriv_thres = 0.000001;
  codec_lone ed(&encoder, &decoder, alphae, alphaz, alphad, l1_deriv_thres);
  stochastic_gd_trainer<ubyte,ubyte> thetrainer(ed, theparam);

  //! a classifier-meter measures classification errors
  classifier_meter trainmeter, testmeter;

  //! initialize the network weights
  forget_param_linear fgp(1, 0.5);
  thenet.forget(fgp);

  // learning parameters
  gd_param gdp(/* double leta*/ 0.0001,
	       /* double ln */ 	0.0,
	       /* double l1 */ 	0.0,
	       /* double l2 */ 	0.0,
	       /* int dtime */ 	0,
	       /* double iner */0.0, 
	       /* double a_v */ 0.0,
	       /* double a_t */ 0.0,
	       /* double g_t*/ 	0.0);
  infer_param infp;
	
#ifdef __GUI__
  supervised_trainer_gui<ubyte, ubyte> stgui;
  stgui.display_datasource(thetrainer, test_ds, infp, 10, 10);
  stgui.display_internals(thetrainer, test_ds, infp, 2);
#endif

  // now do training iterations 
  cout << "Training network on MNIST with " << train_ds.size();
  cout << " training samples and " << test_ds.size() << " test samples:" << endl;
  for (int i = 0; i < 100; ++i) {
    thetrainer.train(train_ds, trainmeter, gdp, 1);	         // train
    thetrainer.test(train_ds, trainmeter, infp);	         // test
    thetrainer.test(test_ds, testmeter, infp);	                 // test
#ifdef __GUI__
    stgui.display_datasource(thetrainer, test_ds, infp, 10, 10); // display
    stgui.display_internals(thetrainer, test_ds, infp, 2);       // display
#endif
    thetrainer.compute_diaghessian(train_ds, 100, 0.02); // recompute 2nd der
  }
  return 0;
}
