/***************************************************************************
 *   Copyright (C) 2008 by Pierre Sermanet *
 *   pierre.sermanet@gmail.com *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Redistribution under a license not approved by the Open Source
 *       Initiative (http://www.opensource.org) must display the
 *       following acknowledgement in all advertising material:
 *        This product includes software developed at the Courant
 *        Institute of Mathematical Sciences (http://cims.nyu.edu).
 *     * The names of the authors may not be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ThE AUTHORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ***************************************************************************/

#include "dataset_generation.h"
#include <algorithm>

#ifdef __BOOST__
#include "boost/filesystem.hpp"
#include "boost/regex.hpp"
using namespace boost::filesystem;
using namespace boost;
#endif

#ifdef __GUI__
#include "libidxgui.h"
#include "libeblearngui.h"
#endif

using namespace std;

#ifdef __BOOST__

namespace ebl {

  ////////////////////////////////////////////////////////////////
  // Utility functions to prepare a dataset from raw images.

  // take the first slot of letf_images as input image, and append
  // <deformations> deformed copies of it and set corresponding labels
  // to <label>.
  void add_deformations(idx<float> &left_images, idx<int> &current_labels,
			int label, int deformations,
			idx<int> &current_deformid, int &ndefid) {
    idx<float> original = left_images[0];
    idx<float> dst;
    idx<int> lab;
    idx<int> defid = current_deformid[0];
    defid.set(++ndefid); // increment and set deformation id for this set
    // add <deformations> deformations
    for (int i = 1; i <= deformations; ++i) {
      // set label
      lab = current_labels[i];
      lab.set(label);
      // set deformation id
      defid = current_deformid[i];
      defid.set(ndefid);
      // perturbe image
      dst = left_images[i];
      image_deformation_ranperspective(original, dst, original.dim(0) / 4,
				       original.dim(1) / 4, (float) 0.0);
    }
  }

  // convert the input image <src> into the <channels_mode> format.
  void convert_image(idx<float> &src, idx<float> &dst, const int channels_mode,
		     unsigned int fkernel_size) {
    idxdim d = idxdim(src), d2 = idxdim(src);
    d2.setdim(2, dst.dim(2));
    idx<float> in, out, src2(d), tmp(d2), tmp2;
    if (fkernel_size > 0) {
      src2 = src.narrow(0, src.dim(0) - fkernel_size + 1,
			floor(fkernel_size / 2));
      src2 = src2.narrow(1, src.dim(1) - fkernel_size + 1,
			 floor(fkernel_size / 2));
      tmp2 = tmp.narrow(0, tmp.dim(0) - fkernel_size + 1,
			floor(fkernel_size / 2));
      tmp2 = tmp2.narrow(1, tmp.dim(1) - fkernel_size + 1,
			 floor(fkernel_size / 2));
    }
    idx_clear(dst);
    // narrow dst to fit src if smaller
    if (src2.dim(0) < dst.dim(0))
      dst = dst.narrow(0, src2.dim(0), floor((dst.dim(0) - src2.dim(0)) / 2));
    if (src2.dim(1) < dst.dim(1))
      dst = dst.narrow(1, src2.dim(1), floor((dst.dim(1) - src2.dim(1)) / 2));
    // converting
    switch (channels_mode) {
    case 0: // RGB
      idx_copy(src2, dst);
      image_global_normalization(dst);
      break ;
    case 1: // YUV
      rgb_to_yuv(src, tmp);
      in = tmp.select(2, 0);
      d = idxdim(in);
      out = idx<float>(d);      
      image_mexican_filter(in, out, 6, fkernel_size);
      idx_copy(out, in);
      idx_copy(tmp2, dst);
      image_global_normalization(dst);      
      break ;
    case 3: { // Y only 
      rgb_to_y(src, tmp);
      in = tmp.select(2, 0);
      d = idxdim(in);
      out = idx<float>(d);      
      image_global_normalization(in);
      image_local_normalization(in, out, fkernel_size);      
      idx_copy(out, in);
      idx_copy(tmp2, dst);
    }
      break ;
    case 4: // YH3
      rgb_to_yh3(src, tmp);
      in = tmp.select(2, 0);
      d = idxdim(in);
      out = idx<float>(d);      
      image_mexican_filter(in, out, 6, fkernel_size);
      image_global_normalization(out);
      idx_copy(out, in);
      idx_copy(tmp2, dst);
      break ;
    case 5: // VpH2SV
      rgb_to_vph2sv(src, tmp, 6, fkernel_size);
      idx_copy(tmp2, dst);
      break ;
    default:
      cerr << "unknown channel mode: " << channels_mode << endl;
      eblerror("unknown channel mode");
    }
  }

  unsigned int draw_layer(idx<float> &img, int dimn, int layern, const char *s,
			  unsigned int h, unsigned int w) {
#ifdef __GUI__
    idx<float> layer = img.select(dimn, layern);
    draw_matrix(layer, s, h, w, 1.0, 1.0, (float)-1.0, (float)1.0);
#endif
    return img.dim(1) + 3;
  }
  
  // display original and new images.
  void display_images(idx<ubyte> &classes, int label, idx<float> &original,
		      idx<float> &new_images, const int channels_mode,
		      idx<ubyte> &ds_names, int current_ds) {
#ifdef __GUI__
    unsigned int h = 0, w = 0;
    static unsigned int cnt = 0;
    idx<float> layer;

    // only display every 15 images
    if (cnt++ % 15 != 0) return ;
    
    // reset window and do a batch display
    disable_window_updates();
    clear_window();
    
    // original image (RGB)
    static string s;
    s = "RGB"; s += " - "; s += (char *) classes[label].idx_ptr();
    draw_matrix(original, s.c_str(), h, w);
    w = 0; h += new_images.dim(1) + 5;
    gui << at(h, w) << black_on_white();
    gui << ds_names[current_ds].idx_ptr() << " dataset:";
    h += 15;
    
    // new images
    idx_bloop1(image, new_images, float) {
      switch (channels_mode) {
      case 1: // YpUV
	w += draw_layer(image, 2, 0, "Yp", h, w);
	w += draw_layer(image, 2, 1, "U", h, w);
	w += draw_layer(image, 2, 2, "V", h, w);
	break ;
      case 3: // Yp
	w += draw_layer(image, 2, 0, "Yp", h, w);
	break ;
      case 4: { // YpH3
	w += draw_layer(image, 2, 0, "Yp", h, w);
	w += draw_layer(image, 2, 1, "H3", h, w);
	idx_addc(layer, (float)1.0, layer);
	idx_dotc(layer, (float)210.0, layer);
	w += layer.dim(1) + 5;
	static idx<float> rgb(layer.dim(0), layer.dim(1), 3);
	h3_to_rgb(layer, rgb);
	draw_matrix(rgb, "H3 (colored)", h, w);
	break ;
      }
      case 5: // VpH2SV
	w += draw_layer(image, 2, 0, "Vp", h, w);
	w += draw_layer(image, 2, 1, "H1", h, w);
	w += draw_layer(image, 2, 2, "H2", h, w);
	w += draw_layer(image, 2, 3, "S", h, w);
	w += draw_layer(image, 2, 4, "V", h, w);
	w = 0; h += image.dim(1) + 5;
	break ;
      }
    }
    enable_window_updates();
#endif
  }
  
  //! Recursively goes through dir, looking for files matching extension ext.
  void process_dir(const char *dir, const char *ext, const char* leftp, 
		   const char *rightp, unsigned int width,
		   unsigned int fwidth, idx<float> &images,
		   idx<int> &labels, int label, bool silent, bool display,
		   bool *binocular, 
		   const int channels_mode, const int channels_size,
		   idx<ubyte> &classes, unsigned int &counter,
		   idx<unsigned int> &counters_used, idx<int> &ds_assignment,
		   unsigned int fkernel_size, int deformations,
		   idx<int> &deformid, int &ndefid,
		   idx<ubyte> &ds_names) {
    regex eExt(ext);
    string el(".*");
    idx<float> limg(1, 1, 1);
    idx<float> rimg(1, 1, 1);
    idx<float> tmp, left_images;
    idx<int> current_labels;
    idx<int> current_deformid;
    idx<float> tmp2;
    int current_ds;
    unsigned int current_used;
    if (leftp) {
      el += leftp;
      el += ".*";
    }
    regex eLeft(el);
    cmatch what;
    path p(dir);
    if (!exists(p))
      return ;
    directory_iterator end_itr; // default construction yields past-the-end
    for (directory_iterator itr(p); itr != end_itr; ++itr) {
      if (is_directory(itr->status())) {
	process_dir(itr->path().string().c_str(), ext, leftp, rightp, width,
		    fwidth, images, labels, label, silent, display, binocular,
		    channels_mode, channels_size, classes, counter,
		    counters_used, ds_assignment, fkernel_size, deformations,
		    deformid, ndefid, ds_names);
      } else if (regex_match(itr->leaf().c_str(), what, eExt)) {
	if (regex_match(itr->leaf().c_str(), what, eLeft)) {
	  current_ds = ds_assignment.get(counter);
	  if (current_ds != -1) {
	    current_used = counters_used.get(current_ds);
	    // found left image
	    // increase example number
	    labels.set(label, current_ds, counters_used.get(current_ds));
	    // check for right image
	    if (rightp != NULL) {
	      regex reg(leftp);
	      string rp(rightp);
	      string s = regex_replace(itr->leaf(), reg, rp);
	      string sfull = itr->path().branch_path().string();
	      sfull += "/";
	      sfull += s;
	      path r(sfull);
	      if (exists(r)) {
		// found right image
		*binocular = true;
		if (image_read_rgbx(r.string().c_str(), rimg)) {
		  // resize stereo dimension to twice the channels_size
		  if (images.dim(3) == channels_size)
		    images.resize(images.dim(0), images.dim(1), 
				  images.dim(2), images.dim(3) * 2);
		  // take the right most square of the image
		  if (rimg.dim(0) <= rimg.dim(1))
		    rimg = rimg.narrow(1, rimg.dim(0),
				       rimg.dim(1) - rimg.dim(0));
		  else
		    rimg = rimg.narrow(0, rimg.dim(1),
				       rimg.dim(0) - rimg.dim(1));
		  // resize image to target width
		  rimg = image_resize(rimg, width, width, 1);
		  tmp = images[current_ds];
		  tmp = tmp[counters_used.get(current_ds)];
		  tmp = tmp.narrow(2, channels_size, channels_size);
		  // finally copy right images to idx
		  convert_image(rimg, tmp, channels_mode, fkernel_size);
		}
		if (!silent)
		  cout << "Processing (right): " << sfull << endl;
	      }
	    }
	    if (!silent) {
	      cout << counter << "/" << ds_assignment.dim(0) << ": ";
	      cout << itr->path().string().c_str() << endl;
	    }
	    // process left image
	    if (image_read_rgbx(itr->path().string().c_str(), limg)) {
	      // take the left most square of the image
	      int h = limg.dim(0), w = limg.dim(1), newh, neww;
	      if (h > w) {
		newh = width;
		neww = (newh / (float) h) * w;
	      } else {
		neww = width;
		newh = (neww / (float) w) * h;
	      }
	      // resize image to target width
	      limg = image_resize(limg, neww, newh, 1);
	      left_images = images[current_ds];
	      left_images = left_images.narrow(3, channels_size, 0);
	      left_images = left_images.narrow(0, 1 + (deformations<=0?0:
						       deformations),
					       current_used);
	      current_labels = labels[current_ds];
	      current_labels = current_labels.narrow(0, current_labels.dim(0) -
						     current_used,current_used);
	      current_deformid = deformid[current_ds];
	      current_deformid = current_deformid.
		narrow(0, current_deformid.dim(0) - current_used, current_used);
	      tmp = left_images[0];
	      // convert and copy image into images buffer
	      convert_image(limg, tmp, channels_mode, fkernel_size);
	      // if adding to dataset 0, add deformations if deformations > 0
	      if ((current_ds == 0) && (deformations > 0)) {
		add_deformations(left_images, current_labels, label,
				 deformations, current_deformid, ndefid);
		counters_used.set(counters_used.get(current_ds) + deformations,
				  current_ds);
	      } else // no deformations
		left_images = left_images.narrow(0, 1, 0);
	      // display
	      if (display) display_images(classes, label, limg, left_images,
					  channels_mode, ds_names, current_ds);
	    // increment counter for dataset current_ds
	    counters_used.set(counters_used.get(current_ds) + 1, current_ds);
	    }
	  }
	  counter++;
	}
      }
    }
  }

  // Recursively goes through dir, looking for files matching extension ext.
  void count_matches(const char *dir, const char *pattern,
		     unsigned int &nimages) {
    regex eExt(pattern);
    cmatch what;
    path p(dir);
    if (!exists(p))
      return ;
    directory_iterator end_itr; // default construction yields past-the-end
    for (directory_iterator itr(p); itr != end_itr; ++itr) {
      if (is_directory(itr->status()))
	count_matches(itr->path().string().c_str(), pattern, nimages);
      else if (regex_match(itr->leaf().c_str(), what, eExt))
	nimages++;
    }
  }

  //! Return an idx of dimensions Nx2 containing all possible N similar pairs.
  idx<int> make_pairs(idx<int> &labels) {
    // allocate maximum number of pairs
    idx<int> pairs((labels.dim(0) * (labels.dim(0) - 1)) / 2, 2);
    int n = 0, r = 0;
    // for each label, loop over all following labels to find pairs
    for (int i = 0; i < labels.dim(0) - 1; ++i) {
      for (int j = i + 1; j < labels.dim(0); ++j) {
	if (labels.get(i) == labels.get(j)) {
	  r = drand(0.0, 1.0); // randomize distribution as 1st or 2nd element
	  pairs.set(i, n, (r > 0.5) ? 0 : 1);
	  pairs.set(j, n, (r > 0.5) ? 1 : 0);
	  n++;
	}
      }
    }
    pairs.resize(n, pairs.dim(1));
    return pairs;
  }

#endif 

  ////////////////////////////////////////////////////////////////

  bool imagedir_to_idx(const char *imgDir, unsigned int width,
		       const int channels_mode,
		       const char *imgExtension, const char *imgPatternLeft, 
		       const char *outDir, const char *imgPatternRight, 
		       bool silent, bool display, const char *prefix,
		       const char *name_, idx<int> *max_per_class_,
		       idx<ubyte> *datasets_names_,
		       unsigned int fkernel_size, int deformations) {
    // local variables
    idx<ubyte>          datasets_names = datasets_names_ ? *datasets_names_
      : idx<ubyte>(1, 1);
    if (!datasets_names_) datasets_names.set('\0', 0, 0);
    int                 ndatasets = datasets_names.dim(0);
    unsigned int	nimages;	// number of available images
    unsigned int        nimages_used;	// final number of images per dataset
    unsigned int	counter;	// counter of available images
    idx<unsigned int>	counters_used(ndatasets);// counters for used images
    int			nclasses;       // number of classes
    int			channels_size;  // number of channels
    idx<ubyte>		classes;        // strings of classes
    idx<float>		images;         // full image buffer
    idx<int>		labels;         // full labels buffer
    idx<int>		deformid;       // index of original for deformations
    int                 ndefid = -1;         // id for each deformed set
    idx<ubyte>		tmp;
    int                 i;
    string              name = (name_ == NULL) ? "dataset" : name_;
    idx<int>            class_ranges;
    idx<int>            ds_assignment;
    if (fkernel_size > 0) width += fkernel_size - 1;
    unsigned int        fwidth = fkernel_size == 0 ? width :
      width - fkernel_size + 1;
    idx<int> max_per_class(datasets_names.dim(0));
    if (max_per_class_)
      idx_copy(*max_per_class_, max_per_class);
    else
      idx_fill(max_per_class, -1);
    int max_max_per_class = idx_max(max_per_class);
    if (!silent) cout << "Target image width: " << fwidth << endl;
    //    if (silent) display = false;
#ifdef __GUI__
    if (display) new_window("Dataset Compiler");
#endif
#ifdef __BOOST__
    // check root directory
    path imgp(imgDir);
    bool binocular = false;
    if (!exists(imgp)) {
      cerr << "cannot find path: " << imgp.string() << endl;
      eblerror("imagedir_to_idx: path does not exist");
      return false;
    }

    // select channels size
    channels_size = 0;
    switch (channels_mode) {
    case 0: channels_size = 3; break;
    case 1: channels_size = 3; break;
    case 2: channels_size = 3; break;
    case 3: channels_size = 1; break;      
    case 4: channels_size = 2; break;
    case 5: channels_size = 5; break;
    default: cerr << "unknown channels mode " << channels_mode << endl;
      eblerror("unknown channels mode");
    }
    
    // find classes from first-level directories
    if (!silent) cout << "Scanning classes... ";
    idx_clear(classes);
    nclasses = 0;
    cmatch what;
    regex hidden_dir(".svn");
    directory_iterator end_itr; // default construction yields past-the-end
    for (directory_iterator itr(imgp); itr != end_itr; ++itr) {
      if (is_directory(itr->status()) 
	  && !regex_match(itr->leaf().c_str(), what, hidden_dir)) {
	nclasses++;
      }
    }
    if (nclasses == 0) {
      eblerror("ImageDirToidx: no classes found");
      return false;
    }
    // collect class names
    classes = idx<ubyte>(nclasses, 128); // Nclasses x 128
    i = 0;
    for (directory_iterator itr(imgp); itr != end_itr; ++itr) {
      if (is_directory(itr->status()) 
	  && !regex_match(itr->leaf().c_str(), what, hidden_dir)) {
	tmp = classes.select(0, i);
	// copy class name
	memcpy(tmp.idx_ptr(), itr->leaf().c_str(), 
	       min((size_t) 128, itr->leaf().length() + 1) * sizeof (ubyte));
	++i;
      }
    }
    
    // find number of images
    if (!silent) cout << "Counting images... ";
    nimages = 0;
    class_ranges = idx<int>(nclasses, 2);
    i = 0;
    for (directory_iterator itr(imgp); itr != end_itr; itr++) {
      if (is_directory(itr->status())
	  && !regex_match(itr->leaf().c_str(), what, hidden_dir)) {
	class_ranges.set(nimages, i, 0);
	// recursively search each directory
	count_matches(itr->path().string().c_str(), imgExtension, nimages);
	class_ranges.set(nimages - 1, i, 1);
	if ((max_max_per_class > 0) &&
	    (class_ranges.get(i, 1) + 1 - class_ranges.get(i, 0) <
	     max_max_per_class)) {
	  cout << "warning in dataset_generation: class ";
	  cout << classes[i].idx_ptr() << " has ";
	  cout << class_ranges.get(i, 1) + 1 - class_ranges.get(i, 0);
	  cout << " elements but max_per_class = " << max_max_per_class << endl;
	}
	++i;
      }
    }
    if (!silent) {
      cout << nimages << " images found." << endl;
      cout << nclasses << " classes found:" << endl;
      i = 0;
      idx_bloop2(classe, classes, ubyte, range, class_ranges, int) { 
	cout << "  " << classe.idx_ptr() << " (";
	cout << range.get(1) - range.get(0) + 1 << ")" << endl;
	i++; 
      }
    }

    // allocate memory
    nimages_used = (max_max_per_class <= 0) ?
      nimages : nclasses * max_max_per_class;
    nimages_used += (deformations <= 0) ? 0 : nimages_used * deformations;
    if (!silent) {
      cout << "Allocating memory for " << ndatasets << "x";
      cout << nimages_used << "x" << channels_size;
      cout << "x" << fwidth << "x" << fwidth << " image buffer (";
      cout << (ndatasets * nimages_used * fwidth * fwidth
	       * channels_size * sizeof (float)) / (1024*1024);
      cout << " Mb)..." << endl;
    }
    // N x w x w x channels_size
    images = idx<float>(ndatasets, nimages_used, fwidth, fwidth, channels_size);
    labels = idx<int>(ndatasets, nimages_used); // N
    deformid = idx<int>(ndatasets, nimages_used); // N
    ds_assignment = idx<int>(nimages);

    if (!silent) { cout << "Shuffling image collection..." << endl; }
    dynamic_init_drand(); // initialize random seed
    // for each class, assign images to training or testing set
    if (max_max_per_class == -1)
      idx_fill(ds_assignment, 0); // put all images in dataset 0
    else {
      int total_missing = 0;
      idx_fill(ds_assignment, -1); // -1 means no assignment
      idx_bloop2(range, class_ranges, int, classe, classes, ubyte) {
	int id = 0;
	idx_bloop2(dsname, datasets_names, ubyte, mpc, max_per_class, int) {
	  int j;
	  int k;
	  int pos;
	  for (k = 0; k < mpc.get(); ++k) {
	    pos = (int) drand(range.get(0), range.get(1));
	    // if conflict, assign next available slot
	    if (ds_assignment.get(pos) != -1) {
	      for (j = pos + 1; j != pos; j++) {
		if (j >= range.get(1) + 1) // reach end, go to beginning
		  j = range.get(0);
		if (j == pos)
		  break ;
		if (ds_assignment.get(j) == -1) {
		  pos = j;
		  break ;
		}
	      }
	      if (ds_assignment.get(pos) != -1) { // did not find an image
		if (!silent) {
		  cout << "warning: only " << k << "/" << mpc.get();
		  cout << " available images for class ";
		  cout << classe.idx_ptr() << " in ";
		  cout << dsname.idx_ptr();
		  cout << " dataset." << endl;
		}
		total_missing += mpc.get() - k;
		break ;
	      }
	    }
	    ds_assignment.set(id, pos); // assign to dataset 0
	  }
	  id++;
	}
      }
      if (!silent) {
	cout << "Total missing images for all datasets: ";
	cout << total_missing << endl;
      }
    }

     // collect images
    if (!silent) cout << "Collecting images..." << endl;
    counter = 0;
    idx_bloop1(cused, counters_used, unsigned int) {
      cused.set(0);
    }
    i = 0;
    for (directory_iterator itr(imgp); itr != end_itr; itr++) {
      if (is_directory(itr->status())
	  && !regex_match(itr->leaf().c_str(), what, hidden_dir)) {
	// process subdirs to extract images into the single image idx
	process_dir(itr->path().string().c_str(), imgExtension, imgPatternLeft,
		    imgPatternRight, width, fwidth, images, labels, i, silent,
		    display, &binocular, channels_mode, channels_size, classes,
		    counter, counters_used, ds_assignment, fkernel_size,
		    deformations, deformid, ndefid, datasets_names);
	++i; // increment only for directories
      }
    }

    if (!silent) {
      cout << "Collected " << idx_sum(counters_used) << " images:" << endl;
      idx_bloop2(cused, counters_used, unsigned int,
		 dsname, datasets_names, ubyte) {
	cout << "  collected " << cused.get() << " images for ";
	cout << dsname.idx_ptr() << " dataset." << endl;
      }
      cout << "Saving images, labels, classes and pairs in ";
      cout << (outDir != NULL ? outDir: imgDir) << ":" << endl;
    }

    // put layers dimension first, easier for neural nets
    int tr[5] = { 0, 1, 4, 2, 3 };
    images = images.transpose(tr);

    // save each dataset
    i = 0;
    idx_bloop5(dsname, datasets_names, ubyte,
	       dsimages, images, float,
	       dslabels, labels, int,
	       dsdefid, deformid, int,
	       cntused, counters_used, unsigned int) {
      // filenames
      string cular(binocular? "_bino" : "");
      string dsetimages(outDir != NULL ? outDir: imgDir);
      dsetimages += "/"; dsetimages += name;
      dsetimages += cular;
      if (prefix) { dsetimages += "_"; dsetimages += prefix; }
      dsetimages += "_"; dsetimages += (const char *) dsname.idx_ptr();
      dsetimages += "_images.mat";
      string dsetlabels(outDir != NULL ? outDir: imgDir);
      dsetlabels += "/"; dsetlabels += name;
      dsetlabels += cular;
      if (prefix) { dsetlabels += "_"; dsetlabels += prefix; }
      dsetlabels += "_"; dsetlabels += (const char *) dsname.idx_ptr();
      dsetlabels += "_labels.mat";
      string dsetclasses(outDir != NULL ? outDir: imgDir);
      dsetclasses += "/"; dsetclasses += name;
      dsetclasses += cular;
      if (prefix) { dsetclasses += "_"; dsetclasses += prefix; }
      dsetclasses += "_"; dsetclasses += (const char *) dsname.idx_ptr();
      dsetclasses += "_classes.mat";
      string dsetclasspairs(outDir != NULL ? outDir: imgDir);
      dsetclasspairs += "/"; dsetclasspairs += name;
      dsetclasspairs += cular;
      if (prefix) { dsetclasspairs += "_"; dsetclasspairs += prefix; }
      dsetclasspairs += "_"; dsetclasspairs += (const char *) dsname.idx_ptr();
      dsetclasspairs += "_classpairs.mat";
      string dsetdefpairs(outDir != NULL ? outDir: imgDir);
      dsetdefpairs += "/"; dsetdefpairs += name;
      dsetdefpairs += cular;
      if (prefix) { dsetdefpairs += "_"; dsetdefpairs += prefix; }
      dsetdefpairs += "_"; dsetdefpairs += (const char *) dsname.idx_ptr();
      dsetdefpairs += "_defpairs.mat";

      // narrow datasets to actual number of collected images
      idx<float> dsimages_used = dsimages.narrow(0, cntused.get(), 0);
      idx<int> dslabels_used = dslabels.narrow(0, cntused.get(), 0);
      idx<int> dsdefid_used = dsdefid.narrow(0, cntused.get(), 0);
      
      // shuffle datasets
      if (!silent) cout << "Shuffling dataset..." << endl;
      idx_shuffle_together(dsimages_used, dslabels_used, dsdefid_used, 0);

      // make pairs (only after shuffling)
      idx<int> classpairs, defpairs;
      if (i == 0) {
	if (!silent) cout << "Computing class and deformation pairs..." << endl;
	classpairs = make_pairs(dslabels_used);
	if (deformations > 0) defpairs = make_pairs(dsdefid_used);
	if (!silent) {
	  cout << "  Found " << classpairs.dim(0) << " class pairs." << endl;
	  cout << "  Found " << ((deformations > 0) ? defpairs.dim(0) : 0);
	  cout << " deformation pairs." << endl;
	  cout << "  Shuffling pairs..." << endl;
	}
	// shuffle
	idx_shuffle(classpairs, 0);
	if (deformations > 0)
	  idx_shuffle(defpairs, 0);
      }

      // saving files
      if (!silent) cout << "Saving " << dsetimages << endl;
      save_matrix(dsimages_used, dsetimages.c_str());
      if (!silent) cout << "Saving " << dsetlabels << endl;
      save_matrix(dslabels_used, dsetlabels.c_str());
      if (!silent) cout << "Saving " << dsetclasses << endl;
      save_matrix(classes, dsetclasses.c_str());
      if (i == 0) { // pairs exist only for training set
	if (!silent) cout << "Saving " << dsetclasspairs << endl;
	save_matrix(classpairs, dsetclasspairs.c_str());
	if (deformations > 0) {
	  if (!silent) cout << "Saving " << dsetdefpairs << endl;
	  save_matrix(defpairs, dsetdefpairs.c_str());
	}
      }
      i++;
    }
#endif /* __BOOST__ */
#ifdef __GUI__
    if (display) quit_gui();
#endif
    return true;
  }

} // end namespace ebl
