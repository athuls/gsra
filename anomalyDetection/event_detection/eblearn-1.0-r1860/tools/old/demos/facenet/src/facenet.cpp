#include "facenet.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

using namespace std;
using namespace ebl;


//////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////

Idx<intg> custom_table(int in, int out){
  int out2 = out-2;
  Idx<intg> tbl((3.5 * out2)+(2*in), 2);
  for(int i = 0; i < out2/2; i++){
    for(int j = 0; j <= 2; j++){
      tbl.set(i, (3*i)+j, 1);
      tbl.set((i+j)% in, 3*i +j, 0);
    }
  }
  for(int i = out2/2; i < out2; i++){
    for(int j = 0; j <= 3; j++){
      tbl.set(i, i*4 - out2/2 + j, 1);
      tbl.set((i+j)% in,  i*4 - out2/2 + j, 0);
    }
  }
  for(int j = 0; j < in; j++){
    tbl.set(j, 3.5*out2 + j, 0);
    tbl.set(out2,  3.5*out2 + j, 1);
  }
  for(int j = 0; j < in; j++){
    tbl.set(j, 3.5*out2 + j + in, 0);
    tbl.set(out-1,  3.5*out2 + j + in, 1);
  }
  return tbl;
}


Idx<double> mexican_hat(double s, int n){
  Idx<double> m(n, n);
  double vinv = 1/(s*s);
  double total = 0;
  int cx = n/2;
  int cy = n/2;
  for(int x = 0; x < n; x++){
    for(int y = 0; y < n; y++){
      int dx = x - cx;
      int dy = y - cy;
      m.set( -exp(-sqrt(vinv*(dx*dx + dy*dy))) , x, y);
      total += m.get(x, y);
    }
  }
  //! set center valus so it's zero sum
  m.set( m.get(cx, cy) - total, cx, cy);
  //! normalize so that energy is 1
  double energy = sqrt(idx_sumsqr(m));
  idx_dotc(m, 1/energy, m);
  return m;
}


//////////////////////////////////////////////////////////////////

facenet_datasource::facenet_datasource(Idx<double> *inputs, Idx<ubyte> *labels, double m, double d, bool pretreatment) :
  LabeledDataSource<double, ubyte>(*inputs, *labels)
{
  height = data.dim(1);
  width = data.dim(2);
  mymean = m;
  mydeviation = d;
  if(pretreatment){
    mexican_filter(2, 5);
    mymean = idx3_mean(&data);
    mydeviation = idx3_coef(&data, mymean);
    normalize();
  }
}


void facenet_datasource::mexican_filter(double s, int n){
  Idx<double> filter = mexican_hat(s, n);
  for(int layer = 0; layer < data.dim(0); layer++){
    Idx<double> big(height + 2 * floor(n/2), width + 2 * floor(n/2));
    idx_fill(big, 0.0);
    Idx<double> bla = data.select(0, layer);
    Idx<double> bla2 = big.narrow(0, height, floor(n/2)).narrow(1, width, floor(n/2));
    idx_copy( bla, bla2);
    idx_2dconvol(big, filter, bla);
  }
  printf("High-pass filtering performed \n");
}

void facenet_datasource::save(string dir, string name){
  string s1 = dir + "/" + name + "_inputs.mat";
  save_matrix(data, s1.c_str());
  string s2 = dir + "/" + name + "_labels.mat";
  save_matrix(labels, s2.c_str());
}

void facenet_datasource::normalize(){
  for(int layer = 0; layer < data.dim(0); layer++){
    Idx<double> bla = data.select(0, layer);
    Idx<double> bla2(height, width);
    idx_addc(bla, -mymean, bla2);
    idx_dotc(bla2, 1/mydeviation, bla);
  }
  printf("Normalization performed \n");
}


/////////////////////////////////////////////////////////////////

facenet::facenet(const char *paramfile){
  theparam = new parameter(60000);

  int nclasses = 2;
  //! the target values for mean-squared error training
  //! are +target for the output corresponding to the class
  //! being shown and -target for the other outputs.
  double target = 1;
  labels = new Idx<ubyte>(nclasses);
  targets = new Idx<double>(nclasses, nclasses);
  idx_fill( *targets, -target);
  for(int i = 0; i < nclasses; i++){
    targets->set(target, i, i);
    labels->set(i, i);
  }
  idx_dotc(*targets, 1.5, *targets);

  table0 = full_table(1, 6);
  table1 = custom_table(6, 16);
  table2 = custom_table(16, 80);

  //! initializes the net (cscscf) and charges the weights if needed
  init(*theparam, 42, 42, 7, 7, table0, 2, 2, 7, 7, table1, 2, 2, 6, 6, table2, 2);
  if((paramfile != NULL) && (theparam->load(paramfile))){
    printf("Weights loaded. \n");
  }
  else{
    printf("Weights loaded \n");
  }

  //! creates the kernels for smoothing and high-pass filtering
  double sker[3][3] = {{0.3, 0.5, 0.3}, {0.5, 1, 0.5}, {0.3, 0.5, 0.3}};
  smoothing_kernel = Idx<double>(3, 3);
  memcpy(smoothing_kernel.idx_ptr(), sker, sizeof (sker));

  highpass_kernel = mexican_hat(2, 5);
}

facenet::~facenet() {
  delete theparam;
  { idx_bloop3(in, inputs, void*, out, outputs, void*, r, results, void*) {
      delete((state_idx*) in.get());
      delete((state_idx*) out.get());
      delete((Idx<double>*) r.get());
    }}
  delete labels;
  delete targets;

}


void facenet::build_databases( int sz, const string directory, float ratio){
  Idx<double> dataface(1,1,1), datanonface(1,1,1);
  Idx<ubyte> lblface(1), lblnonface(1);

  //! loads the various images idxs
  string s1 = directory + "/data_face.mat";
  load_matrix( dataface, s1.c_str());
  string s2 = directory + "/labels_face.mat";
  load_matrix( lblface, s2.c_str());
  string s3 = directory + "/data_nonface.mat";
  load_matrix( datanonface, s3.c_str());
  string s4 = directory + "/labels_nonface.mat";
  load_matrix( lblnonface, s4.c_str());

  //! initializes the various sets of images and according labels
  Idx<double> training_data( dataface.dim(0)+ datanonface.dim(0), sz, sz);
  Idx<double> validation_data( dataface.dim(0)+ datanonface.dim(0), sz, sz);
  Idx<ubyte> training_lbl( lblface.dim(0)+ lblnonface.dim(0));
  Idx<ubyte> validation_lbl( lblface.dim(0)+ lblnonface.dim(0));
  int n_train = 0, n_valid = 0;
  int imax = min(dataface.dim(0)-1, datanonface.dim(0) - 1);

  //! distributes randomly the images into the training and validation datasets, according to the ratio
  dseed(0);
  for(int i = 0; i <= imax; i++){
    if(drand() < ratio){
      Idx<double> bla = datanonface.select(0,i);
      Idx<double> bla2 = training_data.select(0, n_train);
      idx_copy( bla, bla2);
      training_lbl.set( lblnonface.get(i) , n_train);
      n_train++;
      bla = dataface.select(0,i);
      bla2 = training_data.select(0, n_train);
      idx_copy( bla, bla2);
      training_lbl.set( lblface.get(i) , n_train);
      n_train++;
    } else {
      Idx<double> bla = datanonface.select(0,i);
      Idx<double> bla2 = validation_data.select(0, n_valid);
      idx_copy( bla, bla2);
      validation_lbl.set( lblnonface.get(i) , n_valid);
      n_valid++;
      bla = dataface.select(0,i);
      bla2 = validation_data.select(0, n_valid);
      idx_copy( bla, bla2);
      validation_lbl.set( lblface.get(i) , n_valid);
      n_valid++;
    }
  }

  training_data = training_data.narrow(0, n_train, 0);
  training_lbl = training_lbl.narrow(0, n_train, 0);
  validation_data = validation_data.narrow(0, n_valid, 0);
  validation_lbl = validation_lbl.narrow(0, n_valid, 0);

  //! saves the idxs for future use
  facenet_datasource* trainingdb = new facenet_datasource( &training_data, &training_lbl, 0, 0, true);
  facenet_datasource* testingdb = new facenet_datasource( &validation_data, &validation_lbl, 0, 0, true);

  trainingdb->save(directory, "training");
  testingdb->save(directory, "testing");

  printf("Databases built \n");

}


void facenet::train(const string directory){
  //! create the network
  idx3_supervised_module* thenet = new idx3_supervised_module(this, new edist_cost(labels, 1, 1, targets), new max_classer(labels));
  //! create the trainer
  supervised_gradient* thetrainer = new supervised_gradient( thenet, theparam);
  //! a classifier-meter measures classification errors
  classifier_meter* trainmeter = new classifier_meter();
  classifier_meter* testmeter = new classifier_meter();

  //! initialize the network weights
  forget_param_linear forgetparam(1, 1/2);
  init_drand(0);
  thenet->machine->forget(forgetparam);

  //! load the training and testing databases
  Idx<double> training_input(1,1,1);
  Idx<ubyte> training_label(1);
  Idx<double> testing_input(1,1,1);
  Idx<ubyte> testing_label(1);

  string s1 = directory + "/training_inputs.mat";
  load_matrix(training_input, s1.c_str());
  string s2 = directory + "/training_labels.mat";
  load_matrix(training_label, s2.c_str());
  string s3 = directory + "/testing_inputs.mat";
  load_matrix(testing_input, s3.c_str());
  string s4 = directory + "/testing_labels.mat";
  load_matrix(testing_label, s4.c_str());

  facenet_datasource* trainingdb = new facenet_datasource(&training_input, &training_label);
  facenet_datasource* testingdb = new facenet_datasource( &testing_input, &testing_label);
  printf("Databases loaded \n");

  //! do training iterations (here only one)
  printf("training with %d training samples and %d test samples\n", trainingdb->size(), testingdb->size());
  gd_param* gdp = new gd_param(0.0001, 0, 0, 0, 0, 0, 0, 0, 0);

  //! perform only one pass to check that the training is working
  train_online(thetrainer, trainingdb, testingdb, trainmeter, testmeter, 1, gdp, 0, directory);

  //! basic training scheme
  //train_online(thetrainer, trainingdb, testingdb, trainmeter, testmeter, 3, gdp, 0, directory);
  //gdp->eta = 0.00003;
  //train_online(thetrainer, trainingdb, testingdb, trainmeter, testmeter, 2, gdp, 1, directory);
  //gdp->eta = 0.00001;
  //train_online(thetrainer, trainingdb, testingdb, trainmeter, testmeter, 2, gdp, 2, directory);


}


void facenet::train_online(supervised_gradient* trainer, facenet_datasource* trainingdb, facenet_datasource* testingdb, classifier_meter* trainmeter, classifier_meter* testmeter, int n, gd_param* eta, int pass, const string directory){
  int i = 0;
  for(int j = 0; j < n; j++){
    time_t t = clock();
    //! estimate second derivative on 100 iterations, using mu=0.02 and set individual espilons
    printf("computing diagonal hessian and learning rates\n");
    trainer->compute_diaghessian(trainingdb, 100, 0.02);
    trainer->train(trainingdb, trainmeter, eta, 0);
    time_t t2 = clock() - t;
    printf("time : %li \n", (int)t2/CLOCKS_PER_SEC);
    printf("training: ");
    trainer->test(trainingdb, trainmeter);
    trainmeter->display();
    printf(" testing: ");
    trainer->test(testingdb, testmeter);
    testmeter->display();
    //! save the generated weights
    std::stringstream ss;
    ss << pass << i;
    string s = directory + "/weights" + ss.str() + ".mat";
    save_matrix( theparam->x, s.c_str());
    i++;
  }
}



Idx<double> facenet::detect(Idx<ubyte> *img, int h, int w, Idx<int> &sz, float zoom, double threshold, int objsize) {
  height = h;
  width = w;
  grabbed = Idx<ubyte>(height, width);

  sizes = sz;
  //! initialize input and output states and result matrices for each size
  inputs = Idx<void*>(sizes.nelements());
  outputs = Idx<void*>(sizes.nelements());
  results = Idx<void*>(sizes.nelements());
  { idx_bloop4(size, sizes, int, in, inputs, void*, out, outputs, void*, r, results, void*) {
      in.set((void*) new state_idx(2, 38 + (size.get() * 4), 38 + (size.get() * 4)));
      out.set((void*) new state_idx(labels->nelements(), size.get(), size.get()));
      r.set((void*) new Idx<double>(size.get(), size.get(), 2)); // (class,score)
    }}

  //! prepares the multiple scales
  Idx<ubyte> display = this->multi_res_prep(img, 0.5);
  //! performs the fprop
  Idx<double> res = this->multi_res_fprop(threshold, objsize);
  //! draws the rectangles around the hot spots detected
  { idx_bloop1(re, res, double) {
      image_draw_box(display, (ubyte)255,
		     (unsigned int) (zoom * (re.get(3) - (0.5 * re.get(5)))),
		     (unsigned int) (zoom * (re.get(2) - (0.5 * re.get(4)))),
		     (unsigned int) (zoom * re.get(5)),
		     (unsigned int) (zoom * re.get(4)));
    }}

  idx_copy(grabbed, *img);
  return res;
}




void facenet::mark_maxima(Idx<double> &in, Idx<double> &inc, Idx<double> &res, double threshold) {
  idx_clear(res);
  int tr[] = { 1, 2, 0 };
  Idx<double> s(inc.transpose(tr));
  Idx<double> z(in.transpose(tr));
  z = z.unfold(0, 3, 1);
  z = z.unfold(1, 3, 1);
  { idx_bloop3(se, s, double, ze, z, double, re, res, double) {
      { idx_bloop3(see, se, double, zee, ze, double, ree, re, double)  {
	  // find winning class
	  intg w = 1;
	  double c = see.get(w);
	  // look if above threshold and local maximum
	  ree.set(-1.0, 0),
	    ree.set(-100.0, 1);
	  if ((c > threshold) &&
	      (c > zee.get(w, 0, 0)) && (c > zee.get(w, 0, 1)) && (c > zee.get(w, 0, 2)) &&
	      (c > zee.get(w, 1, 0)) && (c > zee.get(w, 1, 2)) &&
	      (c > zee.get(w, 2, 0)) && (c > zee.get(w, 2, 1)) && (c > zee.get(w, 2, 2))) {
	    ree.set(w, 0);
	    ree.set(c, 1);
	  }
	}}
    }}
}

//! produce a score between 0 and 1 for each class at each location
Idx<double> facenet::postprocess_output(double threshold, int objsize) {
  //! find candidates at each scale
  { idx_bloop2(out, outputs, void*, resu, results, void*) {
      Idx<double> outx = ((state_idx*) out.get())->x;
      int c = outx.dim(0);
      int h = outx.dim(1);
      int w = outx.dim(2);
      Idx<double> in(c, 2 + w, 2 + h);
      Idx<double> inc(in);
      inc = inc.narrow(1, w, 1);
      inc = inc.narrow(2, h, 1);
      Idx<double> m(c, h, w);
      idx_fill(in, 0.0);
      idx_clip(outx, 0.0, inc);
      //! smooth
      { idx_bloop2(ie, in, double, me, m, double) {
	  idx_2dconvol(ie, smoothing_kernel, me);
	}}
      idx_copy(m, inc);
      //! find points that are local maxima spatial and class-wise
      //! write result in m. rescale result to [0 1]
      mark_maxima(in, inc, *((Idx<double>*) resu.get()), threshold);
    }}
  //! now prune by scale and make a list
  Idx<double> rlist(1, 6);
  rlist.resize(0, rlist.dim(1));
  Idx<double> in0x(((state_idx*) inputs.get(0))->x);
  intg s0j = in0x.dim(2);
  { idx_bloop3(input, inputs, void*, output, outputs, void*, r, results, void*) {
      Idx<double> inx(((state_idx*) input.get())->x);
      intg sj = inx.dim(2);
      int i = 0, j = 0;
      double scale = (double) (s0j / sj);
      { idx_bloop1(re, *((Idx<double>*) r.get()), double) {
	  j = 0;
	  { idx_bloop1(ree, re, double) {
	      if (ree.get(1) > threshold) {
		intg ri = rlist.dim(0);
		rlist.resize(ri + 1, rlist.dim(1));
		rlist.set(ree.get(0), ri, 0);
		rlist.set(ree.get(1), ri, 1);
		rlist.set((23 + (4 * j)) * scale, ri, 2);
		rlist.set((23 + (4 * i)) * scale, ri, 3);
		rlist.set(objsize * scale, ri, 4);
		rlist.set(objsize * scale, ri, 5);
	      }
	      j++;
	    }}
	  i++;
	}}
    }}
  return rlist;
}

Idx<ubyte> facenet::multi_res_prep(Idx<ubyte> *img, float zoom) {
  //! copy input images locally
  idx_copy(*img, grabbed);
  //! prepare multi resolutions input
  Idx<double> inx;
  int ni = ((state_idx*) inputs.get(0))->x.dim(1);
  int nj = ((state_idx*) inputs.get(0))->x.dim(2);
  int zi = max(ni, (int) (zoom * grabbed.dim(0)));
  int zj = max(nj, (int) (zoom * grabbed.dim(1)));
  int oi = (zi - ni) / 2;
  int oj = (zj - nj) / 2;
  Idx<ubyte> im = image_resize(grabbed, zj, zi, 1);
  im = im.narrow(0, ni, oi);
  im = im.narrow(1, nj, oj);
  //! for display
  idx_clear(grabbed);
  Idx<ubyte> display(grabbed.narrow(0, im.dim(0), 0));
  display = display.narrow(1, im.dim(1), 0);
  idx_copy(im, display);
  { idx_bloop1(in, inputs, void*) {
      inx = ((state_idx*) in.get())->x;
      ni = inx.dim(1);
      nj = inx.dim(2);
      Idx<ubyte> imres = image_resize(im, nj, ni, 1);
      Idx<double> inx0 = inx.select(0, 0);
      Idx<double> inx1 = inx.select(0, 1);
      idx_copy(imres, inx0);
      idx_copy(imres, inx1);
      //! high-pass filtering
      for(int layer = 0; layer <= 1; layer++){
	Idx<double> big(ni + 4, nj + 4);
	idx_fill(big, 0.0);
	Idx<double> bla = inx.select(0, layer);
	Idx<double> bla2 = big.narrow(0, ni, 2).narrow(1, nj, 2);
	idx_copy( bla, bla2);
	idx_2dconvol(big, highpass_kernel, bla);
      }
      //! normalization
      double bias = idx3_mean(&inx);
      double coeff = idx3_coef(&inx, bias);
      idx_addc(inx, -bias, inx);
      idx_dotc(inx, 1/coeff, inx);
    }}
  return display;
}

Idx<double> facenet::multi_res_fprop(double threshold, int objsize) {
  //! fprop network on different resolutions
  { idx_bloop2(in, inputs, void*, out, outputs, void*) {
      fprop(*((state_idx*) in.get()), *((state_idx*) out.get()));
    }}
  //! post process outputs
  Idx<double> res = postprocess_output(threshold, objsize);
  //! prune results
  res = prune(res);
  //! print results
  cout << " results: \n";
  { idx_bloop1(re, res, double) {
      re.printElems();
      cout << " " << labels->get((int)re.get(0));
    }}
  if (res.dim(0) == 0) cout << "no object found.";
  cout << "\n";
  return res;
}

Idx<double> facenet::prune(Idx<double> &res, float ratio) {
  //! prune a list of detections.
  //! only keep the largest scoring within an area
  Idx<double> rlist(1, res.dim(1));
  rlist.resize(0, rlist.dim(1));
  { idx_bloop1(re, res, double) {
      double score = re.get(1);
      int px = re.get(2);
      int py = re.get(3);
      int bx = re.get(4);
      int by = re.get(5);
      //! if the rectangle overlaps an other rectangle with higher score more than ratio, kill it
      bool ok = true;
      { idx_bloop1(o, res, double) {
	  double scoreo = o.get(1);
	  int pxo = o.get(2);
	  int pyo = o.get(3);
	  int bxo = o.get(4);
	  int byo = o.get(5);
	  if ((score < scoreo) &&
	      ((common_area(pxo - (0.5 * bxo),	pyo - (0.5 * byo), bxo, byo, px - 0.5 * bx, py - 0.5 * by, bx, by) > ratio)
	       || (common_area(px - 0.5 * bx, py - 0.5 * by, bx, by, pxo - (0.5 * bxo),	pyo - (0.5 * byo), bxo, byo) > ratio)))
	    ok = false;
	}}
      if (ok) {
	intg ri = rlist.dim(0);
	rlist.resize(ri + 1, rlist.dim(1));
	Idx<double> out(rlist.select(0, ri));
	idx_copy(re, out);
      }
    }}
  return rlist;
}

Idx<int> facenet::calc_sizes(int height, int width){
  int h = (height - 38) / 4;
  int w = (width - 38) / 4;
  int s = min(h, w);
  int sizes[50];
  int n = 0;
  while( s != 0){
    sizes[n]= s;
    s = s / sqrt(2);
    n++;
  }
  Idx<int> sz(n);
  for(int i = 0; i < n; i++) sz.set(sizes[i], i);
  return sz;
}

void facenet::show_net(bool kernel, bool prop){
  // 	if(kernel){
  // 		ebbox* kernels = new ebbox(0, "kernels");
  // 		Idx_Gui* c0kernel = new Idx_Gui(&(this->c0_module->convol->kernel->x), DOUBLE, "c0-layer");
  // 		kernels->add_box( c0kernel);
  // 		Idx_Gui* c1kernel = new Idx_Gui(&(this->c1_module->convol->kernel->x), DOUBLE, "c1-layer");
  // 		kernels->add_box( c1kernel);
  // 		Idx_Gui* c2kernel = new Idx_Gui(&(this->c2_module->convol->kernel->x), DOUBLE, "c2-layer");
  // 		kernels->add_box( c2kernel);
  // 		kernels->show();
  // 	}
  // 	if(prop){
  // 		ebbox* eprop = new ebbox(0, "propagation");
  // 		Idx_Gui* c0prop = new Idx_Gui((void*)&(this->c0_state->x), DOUBLE, "c0-prop");
  // 		eprop->add_box( c0prop);
  // 		Idx_Gui* c1prop = new Idx_Gui((void*)&(this->c1_state->x), DOUBLE, "c1-prop");
  // 		eprop->add_box( c1prop);
  // 		Idx_Gui* c2prop = new Idx_Gui((void*)&(this->c2_state->x), DOUBLE, "c2-prop");
  // 		eprop->add_box( c2prop);
  // 		Idx_Gui* oprop = new Idx_Gui((void*)&(((state_idx*)(outputs.get(0)))->x), DOUBLE, "outputs");
  // 		eprop->add_box( oprop);
  // 		eprop->show();
  // 	}
  eblerror("not implemented");
}


//////////////////////////////////////////////////////

void extract_faces(int height, int width, const string imgDir, const string coordFile, int LineToSkip, char delim, string output, const string outputcoordfile){
  string myfilename = imgDir + "/" + coordFile;
  ifstream myfile(myfilename.c_str());
  if(!myfile.is_open()){
    cout << "Could not open the file :" << myfilename <<"\n";
    return;
  }
  Idx<ubyte> out(1, height, width);
  if((output != "")&&(!load_matrix(out, output.c_str()))){
    cout << "Could not open the output matrix :" << output <<"\n";
    return;
  }
  int index = out.dim(0)-1;

  ofstream myoutputfile;
  myoutputfile.open(outputcoordfile.c_str(), ios_base::ate);

  char* newline = new char[256];
  // skip the irrelevent first lines
  for(int i = 0; i < LineToSkip; i++) myfile.getline(newline, 256);

  char* namefile, *nameandpath;
  int x_centereye, y_centereye, x_centermouth, y_centermouth;
  int x_centereye_o, y_centereye_o, x_centermouth_o, y_centermouth_o;

  dseed(time(0));
  int blabla = 0;
  while(!myfile.eof()){
    newline = new char[256];
    myfile.getline(newline, 256);
    namefile = new char[256];
    int* points = lineparser(newline, delim, namefile);
    if(points == NULL) continue;

    Idx<ubyte> image(1,1,1);
    nameandpath = new char[256];
    string bla = imgDir + "/";
    strcpy(nameandpath, bla.c_str());
    strcat(nameandpath, namefile);
    // open the image in an Idx<ubyte>
    if(image_read_rgbx(nameandpath, image)){
      cout << blabla++ << endl;
      x_centereye_o = x_centereye = points[0];
      y_centereye_o = y_centereye = points[1];
      x_centermouth_o = x_centermouth = points[2];
      y_centermouth_o = y_centermouth = points[3];

      // calculate the angle to verticality of the face, and its size.
      // coeff is a subsampling coefficient used if the face is to big
      // facesize is the nb of pixels between the eyes and the mouth in the final training images.
      int facesize = 8;
      int dist = (x_centermouth - x_centereye)*(x_centermouth - x_centereye) + (y_centermouth - y_centereye)*(y_centermouth - y_centereye);
      double temp1 = 3.1415 / 180 *(x_centermouth - x_centereye)/ sqrt((double)dist);
      double angle = asin(temp1);
      double coeff = facesize / sqrt((double)dist);
      int nsub = 0;
      while(coeff < 0.5){
	Idx<ubyte> image2 = image_subsample(image, 2, 2);
	image.resize(image2.dim(0), image2.dim(1), image2.dim(2));
	idx_copy(image2, image);
	x_centereye /= 2;
	y_centereye /= 2;
	x_centermouth /= 2;
	y_centermouth /=2;
	dist = (x_centermouth - x_centereye)*(x_centermouth - x_centereye) + (y_centermouth - y_centereye)*(y_centermouth - y_centereye);
	coeff = facesize / sqrt((double)dist);
	nsub++;
      }

      // for each face, 10 images will be created by scaling and rotating the face randomly,
      // and 10 more by flipping the first 10
      for(int j = 0; j < 10; j++){
	double rand_angle = drand(30 - abs((int)angle));
	double rand_coeff = drand(1/sqrt(sqrt((double)2)), sqrt(sqrt((double)2)));
	Idx<intg> wh(2);
	Idx<ubyte> bg(4);
	Idx<double> cxcy(2);
	// rotate (up to a 30 angle) and rescale (from a sqrt(sqrt(2)) factor to a 1/sqrt(sqrt(2)) factor) the image
	image_rotscale_rect((int)image.dim(1), (int)image.dim(0), (double)x_centereye, (double)y_centereye, rand_angle + angle, rand_coeff * coeff, wh, cxcy);
	Idx<ubyte> img_rotscaled(wh.get(1), wh.get(0), 3);
	image_rotscale(image, img_rotscaled, x_centereye, y_centereye, cxcy.get(0), cxcy.get(1), rand_angle + angle, rand_coeff * coeff, bg);
	int x_topleft = max((int)cxcy.get(0) - width/2 + (int)drand(2), 0);
	int y_topleft = max((int)cxcy.get(1) - height/2 + (int)drand(2), 0);
	// if a width*height image can be cropped from the resulting image, then crop it and save the according data
	if((x_topleft + width < wh.get(0))&&(y_topleft + height < wh.get(1))){
	  Idx<ubyte> img_cropped = image_crop(img_rotscaled, x_topleft, y_topleft, width, height);
	  if(index == out.dim(0)) out.resize(out.dim(0) + 2, out.dim(1), out.dim(2));
	  Idx<ubyte> bla1 = img_cropped.select( 2, 0);
	  Idx<ubyte> bla2 = out.select(0, index);
	  idx_copy(bla1, bla2);
	  if(outputcoordfile != ""){
	    myoutputfile << index << " " << nameandpath;
	    myoutputfile << " " << x_centereye_o ;
	    myoutputfile << " " << y_centereye_o;
	    myoutputfile << " " << x_centermouth_o;
	    myoutputfile << " " << y_centermouth_o;
	    myoutputfile << " " << (rand_angle + angle);
	    myoutputfile << " " << (coeff * sqrt(nsub) * rand_coeff);
	    myoutputfile << " false\n ";
	  }
	  index++;
	  Idx<ubyte> img_flipped(img_cropped.dim(0), img_cropped.dim(1));
	  for(int i = 0; i < img_cropped.dim(0); i++)
	    for(int j = 0; j < img_cropped.dim(1); j++){
	      img_flipped.set(img_cropped.get(i, (width - 1) - j, 0), i, j);
	    }
	  if(index == out.dim(0)) out.resize(out.dim(0) + 1, out.dim(1), out.dim(2));
	  Idx<ubyte> bla3 = out.select(0, index);
	  idx_copy(img_flipped, bla3);
	  if(outputcoordfile != ""){
	    myoutputfile << index << " " << nameandpath;
	    myoutputfile << " " << x_centereye_o ;
	    myoutputfile << " " << y_centereye_o;
	    myoutputfile << " " << x_centermouth_o;
	    myoutputfile << " " << y_centermouth_o;
	    myoutputfile << " " << (rand_angle + angle) * (nsub + 1);
	    myoutputfile << " " << (coeff * sqrt(nsub) * rand_coeff);
	    myoutputfile << " true\n ";
	  }
	  index++;
	}
      }

    }
    delete [] newline;
    //delete [] namefile;
    delete [] nameandpath;
  }
  myfile.close();
  if(outputcoordfile != "") myoutputfile.close();
  if(output != "") save_matrix(out, output.c_str());
  else save_matrix(out, "data/database_faces.mat");

}

int* lineparser(const char* newline, char delim, char* namefile){
  //! this parser has been written to extract information from lines of the following type :
  //! "name_of_the_imge_file x_lefteye ylefteye x_righteye y_righteye x_nose y_nose x_leftcornermouth y_leftcornermouth x_centermouth y_centermouth x_rightcornermouth y_rightcornermouth"
  int x_centereye, y_centereye, x_centermouth, y_centermouth;

  //! find the name of the image to open
  const char* mychar2 = newline;
  char* mychar = strchr(mychar2,delim);
  if(mychar == NULL) return NULL;
  int longueur = mychar - mychar2;
  strncpy(namefile, mychar2, longueur);
  *(namefile + longueur) = 0;
  mychar2 = ++mychar;

  //! find the various points needed to extract the image
  int x_lefteye, y_lefteye, x_righteye, y_righteye;
  {
    mychar = strchr(mychar2,delim);
    if(mychar == NULL) return NULL;
    longueur = mychar - mychar2;
    if(longueur > 256) return NULL;
    char temp[256];
    strncpy(temp, mychar2, longueur);
    *(temp+longueur) = 0;
    x_lefteye = atoi(temp);
    mychar2 = ++mychar;
  }
  {
    mychar = strchr(mychar2,delim);
    if(mychar == NULL) return NULL;
    longueur = mychar - mychar2;
    if(longueur > 256) return NULL;
    char temp[256];
    strncpy(temp, mychar2, longueur);
    *(temp+longueur) = 0;
    y_lefteye = atoi(temp);
    mychar2 = ++mychar;
  }
  {
    mychar = strchr(mychar2,delim);
    if(mychar == NULL) return NULL;
    longueur = mychar - mychar2;
    if(longueur > 256) return NULL;
    char temp[256];
    strncpy(temp, mychar2, longueur);
    *(temp+longueur) = 0;
    x_righteye = atoi(temp);
    mychar2 = ++mychar;
  }
  {
    mychar = strchr(mychar2,delim);
    if(mychar == NULL) return NULL;
    longueur = mychar - mychar2;
    if(longueur > 256) return NULL;
    char temp[256];
    strncpy(temp, mychar2, longueur);
    *(temp+longueur) = 0;
    y_righteye = atoi(temp);
    mychar2 = ++mychar;
  }

  x_centereye = (x_lefteye + x_righteye)/2;
  y_centereye = (y_lefteye + y_righteye)/2;

  for(int i = 5; i<9; i++){
    mychar = strchr(mychar2,delim);
    if(mychar == NULL) return NULL;
    mychar2 = ++mychar;
  }

  {
    mychar = strchr(mychar2,delim);
    if(mychar == NULL) return NULL;
    longueur = mychar - mychar2;
    if(longueur > 256) return NULL;
    char temp[256];
    strncpy(temp, mychar2, longueur);
    *(temp+longueur) = 0;
    x_centermouth = atoi(temp);
    mychar2 = ++mychar;
  }

  {
    char temp[5];
    if(strlen(mychar2) <= 4) return NULL;
    strncpy(temp, mychar2, 4);
    *(temp+4) = 0;
    y_centermouth = atoi(temp);
  }

  int* bla = new int[4];
  bla[0] = x_centereye;
  bla[1] = y_centereye;
  bla[2] = x_centermouth;
  bla[3] = y_centermouth;
  return bla;
}


