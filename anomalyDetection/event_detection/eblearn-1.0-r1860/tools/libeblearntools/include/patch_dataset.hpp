/***************************************************************************
 *   Copyright (C) 2010 by Pierre Sermanet   *
 *   pierre.sermanet@gmail.com   *
 *   All rights reserved.
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

#ifndef PATCH_DATASET_HPP_
#define PATCH_DATASET_HPP_

#include <algorithm>

#ifdef __BOOST__
#include "boost/filesystem.hpp"
#include "boost/regex.hpp"
using namespace boost::filesystem;
using namespace boost;
#endif

using namespace std;

namespace ebl {

  ////////////////////////////////////////////////////////////////
  // constructors & initializations

  template <class Tdata>
  patch_dataset<Tdata>::patch_dataset(const char *name_,
				      const char *inroot_,
				      const char *outdir_,
				      uint max_folders_)
    : dataset<Tdata>(name_, inroot_) {
    outdir = outdir_;
    cout << "Output directory: " << outdir << endl;
    cout << "Saving " << max_folders_ << " patch per image "
	 << "for each scale at random position. " << endl;
    max_folders = max_folders_;
    data_cnt = 0;
    save_mode = "mat";
  }

  template <class Tdata>
  patch_dataset<Tdata>::~patch_dataset() {
  }

  ////////////////////////////////////////////////////////////////
  // data extraction

  template <class Tdata>
  bool patch_dataset<Tdata>::
  add_data(idx<Tdata> &img, const t_label label,
	   const string *class_name, const char *filename, const rect<int> *r,
	   pair<int,int> *center, const rect<int> *visr,
	   const rect<int> *cropr, const vector<object*> *objs,
	   const jitter *jittforce) {
    vector<rect<int> > patch_bboxes;
    vector<rect<int> >::iterator ibb;
    string cname = "patch";
    ostringstream fname;
    vector<idx<Tdata> > patches;
    
    // check for capacity
    if (this->full(label)) // reached full capacity
      return false;
    // for each scale, find patches and save them
    for (vector<double>::iterator i = scales.begin(); i != scales.end(); ++i) {
      patches.clear();
      patch_bboxes.clear();
      // rescale original bboxes
      uint outh = (uint) (outdims.dim(0) * *i);
      uint outw = (uint) (outdims.dim(1) * *i);
      float ratio = std::max(img.dim(0) / (float) outh, 
			     img.dim(1) / (float) outw);
      uint inh = (uint) (img.dim(0) / ratio);
      uint inw = (uint) (img.dim(1) / ratio);
      // do not upsample to avoid creating artefacts
      // and ignore sizes smaller than outdims
      if (inh > (uint) img.dim(0) || inw > (uint) img.dim(1)
	  || inh < (uint) outdims.dim(0) || inw < (uint) outdims.dim(1)) {
	cerr << "warning: ignoring scale " << *i << " (" << inh << "x"
	     << inw << ") to avoid upsampling original (" << img
	     << ") or downsampling below target (" << outdims << ")." << endl;
      	continue ; // do nothing for this scale
      }
      // preprocess image
      rect<int>  r(0, 0, img.dim(0), img.dim(1));
      rect<int>  outr;
      idx<Tdata> im =
	this->preprocess_data(img, &cname, filename, &r, *i, &outr);
      // extract all non overlapping patches with dimensions outdims that
      // do not overlap with bounding boxes
      rect<int>  patch(0, 0, outdims.dim(0), outdims.dim(1));
      for (patch.h0 = outr.h0; patch.h0 + patch.height <= outr.h0 + outr.height;
	   patch.h0 += patch.height) {
	for (patch.w0 = outr.w0; patch.w0 + patch.width <= outr.w0 + outr.width;
	     patch.w0 += patch.width) {
	  // add patch
	  idx<Tdata> p = im.narrow(0, patch.height, patch.h0);
	  p = p.narrow(1, patch.width, patch.w0);
	  patches.push_back(p);
	  patch_bboxes.push_back(patch);
	}
      }
#ifdef __GUI__
      if (display_extraction) {
	uint h = 63, w = 0;
	//	disable_window_updates();
// 	// draw original image
// 	draw_matrix(im, h, w, 1.0, 1.0, (Tdata) -1, (Tdata) 1);
	// draw patches boxes
	for (ibb = patch_bboxes.begin(); ibb != patch_bboxes.end(); ++ibb)
	  draw_box(h + ibb->h0, w + ibb->w0,
		   ibb->height, ibb->width, 0, 255, 0);
	//	enable_window_updates();
	if (sleep_display)
	  millisleep((long) sleep_delay);
      }
#endif
      save_patches(patches, outdir, max_folders, filename, *i, label);
    }
    return true;
  }
  
  ////////////////////////////////////////////////////////////////
  // save patches

  template <class Tdata>
  void patch_dataset<Tdata>::save_patches(vector<idx<Tdata> > &patches,
					  const string &outdir,
					  uint max_folders,
					  const string &filename,
					  double scale, const t_label label) {
    ostringstream folder, fname;
    try {
      mkdir_full(outdir.c_str());
      uint i;
      // shuffle randomly vector of patches to avoid taking top left corner
      // as first patch every time
      random_shuffle(patches.begin(), patches.end());
      // loop on patches
      for (i = 0; (i < patches.size()) && (i < max_folders); ++i) {
	// create folder if doesn't exist
	folder.str("");
	folder << outdir << "/" << "bg" << i+1 << "/";
	mkdir_full(folder.str().c_str());
	// save patch in folder
	// switch saving behavior
	fname.str("");
	fname << folder.str() << "img_" << setw(5) << setfill('0') << data_cnt 
	      << "_bg" << i+1 << "_scale" << scale;
	if (!strcmp(save_mode.c_str(), "mat")) { // lush matrix mode
	  fname << MATRIX_EXTENSION;
	  if (!save_matrix(patches[i], fname.str()))
	    throw fname.str();
	} else { // image file mode
	  fname << "." << save_mode;
	  idx<Tdata> tmp = patches[i];
	  // scale image to 0 255 if preprocessed
	  if (strcmp(ppconv_type.c_str(), "RGB")) {
	    idx_addc(tmp, (Tdata) 1.0, tmp);
	    idx_dotc(tmp, (Tdata) 127.5, tmp);
	  }
	  save_image(fname.str(), tmp, save_mode.c_str());
	}
	cout << data_cnt << ": saved " << fname.str().c_str()
	     << "(" << patches[i] << ")" << endl;
	// increase global counter
	data_cnt++;
	// check for capacity
	if (this->full(label)) // reached full capacity
	  break ;
      }
//       if (i < patches.size()) // reached max_folders, fill-up last one
// 	for ( ; i < patches.size(); ++i) {
// 	  // save patch in folder
// 	  fname.str("");
// 	  fname << folder.str() << filename << ".bg" << i+1 << ".mat";
// 	  if (!save_matrix(patches[i], fname.str()))
// 	    throw fname.str();
// 	  cout << data_cnt++ << ": saved " << fname.str().c_str() << endl;
// 	}
    } catch (const string &err) {
      cerr << "error: failed to save patch in " << err << endl;
    }
  }

} // end namespace ebl

#endif /* PATCH_DATASET_HPP_ */
