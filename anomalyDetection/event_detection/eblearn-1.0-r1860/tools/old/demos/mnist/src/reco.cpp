#include "libeblearn.h"

#ifdef __GUI__
#include "libeblearngui.h"
#endif

using namespace std;
using namespace ebl; // all eblearn objects are under the ebl namespace

// NOTE: this is one way to execute a trained network on single input images.
//   In this simple program, one has to provide the correct input size based
//   on the convolution and subsampling kernels.
//   With a cscscf network with 5-convolutions and 2-subsamplings, the valid
//   input sizes are given by the following formula, given the output size (x,y)
//     in_x = (((out_x + 4) * 2) + 4) * 2 + 4
//     in_y = (((out_y + 4) * 2) + 4) * 2 + 4
//   A better way uses the classifier2D class which provides multi-scale
//   analysis, automatic input/output size computation,
//   a better answer inference and returns bounding boxes.
//   One should preferably use the Classifier2D for recognition.
//   Classifier2D is however currently under restructuration to work on generic
//   sizes.
//   TODO-0: finish classifier 2D generic version.


// argv[1] is expected to contain the directory of the mnist dataset
#ifdef __GUI__
MAIN_QTHREAD(int, argc, char **, argv) { // macro to enable multithreaded gui
#else
int main(int argc, char **argv) { // regular main without gui
#endif
  cout << "* MNIST recognition demo" << endl;
  if (argc > 3) {
    cout << "Usage: ./mnist_reco <trained parameter> <image file>" << endl;
    eblerror("expected trained parameter filename and image filename");
  }
  typedef float t_net;
  string paramfname = "mnist_trained_network.mat";
  string imagefname = "../data/mnist/2_grid_64x64.pnm";
  if (argc > 1) paramfname = argv[1];
  if (argc > 2) imagefname = argv[2];
  cout << "Looking for digits in image " << imagefname;
  cout << " using trained network " << paramfname << endl;

  // load trained network
  parameter<fs(t_net)> theparam;
  lenet5<fs(t_net)> l5(theparam, 32, 32, 5, 5, 2, 2, 5, 5, 2, 2, 120, 10);
  theparam.load_x(paramfname.c_str());

  // load image
  idx<ubyte> image = load_image<ubyte>(imagefname);
  // only keep 1 color channel since we trained with greyscale images
  image = image.select(2, 0);

  // copy 1 color layer of the image into input state_idx
  fstate_idx<t_net> stin(1, image.dim(0), image.dim(1)), stout(1,1,1);
  idx<t_net> inx = stin.x.select(0, 0); // pointer to 2D input
  idx_copy(image, inx); // convert and copy ubyte 2D image to double 2D input

  // fprop input throught the network
  l5.fprop(stin, stout);

  // infer answers
  cout << "min " << idx_min(stout.x) << " max " << idx_max(stout.x) << endl;
  t_net threshold = (t_net) .93;
  idx_eloop1(outx, stout.x, t_net) {
    idx_eloop1(out, outx, t_net) {
      if (idx_max(out) >= threshold)
	cout << "the image contains a " << idx_indexmax(out) << endl;
    }
  }

  // display internals of the fprop
#ifdef __GUI__
  module_1_1_gui l5gui;
  idx_threshold(stout.x, threshold, (t_net) -1.0);
  l5gui.display_fprop(l5, stin, stout);
  secsleep(1);
#endif
  
  return 0;
}
