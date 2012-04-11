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

#include <string>
#include "similar_patches.h"

using namespace std;

namespace ebl {

  similar_patches::similar_patches(unsigned int maxcurrent, 
				   unsigned int maxsimilar,
				   unsigned int ph, unsigned int pw,
				   unsigned int ih, unsigned int iw) 
    : max_similar_patches(maxsimilar), 
      pheight(ph), pwidth(pw), iheight(ih), iwidth(iw), display_index(0),
      dataset(),
      current_patches(maxcurrent, NULL), 
      max_current_patches(maxcurrent), 
      current_patches_xy(maxcurrent) {
#ifdef __GUI__
    wdisplay = new_window("similar_patches");
#endif
  }

  similar_patches::~similar_patches() {
  }

  bool similar_patches::add_similar_patch(idx<ubyte> &im, int h, 
					  int w, unsigned int index) {
      int h0 = h - pheight / 2;
      int w0 = w - pwidth / 2;
      // don't add patches outsides of the image
      if ((h0 < 0) || (w0 < 0) 
	  || (h0 + pheight >= iheight) || (w0 + pwidth >= iwidth))
	return false;
      if (index >= max_current_patches)
	return false; // the index is wrong
      // make a copy of the patch
      idx<ubyte>* patch = new idx<ubyte>(pheight, pwidth);
      idx<ubyte> im2 = im.narrow(0, pheight, h0);
      im2 = im2.narrow(1, pwidth, w0);
      idx_copy(im2, *patch);
      // if current slot is NULL, create a new vector
      if (current_patches[index] == NULL)
	current_patches[index] = new vector<idx<ubyte>*>;
      vector<idx<ubyte>*> *patchlist = current_patches[index];
      patchlist->push_back(patch);
      current_patches_xy[index].first = w;
      current_patches_xy[index].second = h;
      // we reached the maximum number of patches, add group in
      // database and free spot for a new patch
      if (patchlist->size() >= max_similar_patches) {
	// add group of similar patches to dataset
	dataset.push_back(patchlist);
	// free up patch spot
	current_patches[index] = NULL;
      }
      return true;
    }

  bool similar_patches::current_patch_empty(unsigned int index) {
    if (index >= current_patches.size())
      return false;
    return (current_patches[index] == NULL) ? true : false;
  }

  void similar_patches::display_dataset(unsigned int maxh, unsigned int maxw) {
#ifdef __GUI__
    select_window(wdisplay);
    disable_window_updates();
    clear_window();
    unsigned int h = 0, w = 0;
    idx<ubyte> *patch = NULL;
    for ( ; (display_index < dataset.size()) && (h < maxh); ++display_index) {
      vector<idx<ubyte>*> *plist = dataset[display_index];
      if (plist != NULL) {
	vector<idx<ubyte>*>::iterator k = plist->begin();
	for ( ; (k != plist->end()) && (h < maxh); ++k) {
	  patch = *k;
	  if (w + patch->dim(1) > maxw) {
	    w = 0;
	    h += patch->dim(0);
	  }
	  draw_matrix(*patch, h, w);
	  w += patch->dim(1);
	}
      }
    }
    enable_window_updates();
#endif
  }

  bool similar_patches::save_dataset(const char *directory) {
    idx<ubyte> patches(dataset.size() * max_similar_patches, 
		       1, pheight, pwidth);
    idx<int> labels(patches.dim(0));
    int npairs = choose(max_similar_patches, 2);
    int ipairs = 0;
    int icurrent = 0;
    idx<int> pairs(npairs * dataset.size(), 2);
    idx<ubyte> p;
    idx<ubyte> *patch = NULL;
    // filenames
    string pairs_filename = directory;
    pairs_filename += "/pairs.mat";
    string patches_filename = directory;
    patches_filename += "/patches.mat";
    string labels_filename = directory;
    labels_filename += "/labels.mat";
    // messages
    cout << "saving " << patches.dim(0) << " patches in ";
    cout << patches_filename.c_str() << "..." << endl;
    cout << "saving " << dataset.size() << " different labels in ";
    cout << labels_filename.c_str() << "..." << endl;
    cout << "saving " << pairs.dim(0) << " pairs of patches in ";
    cout << pairs_filename.c_str() << "..." << endl;
    cout << "(" << npairs << " combinations of pairs in " << dataset.size();
    cout << " sets of ";
    cout << max_similar_patches << " similar patches)" << endl;

    // convert dataset list to a big idx
    for (unsigned int i = 0; i < dataset.size(); ++i) {
      vector<idx<ubyte>*> *plist = dataset[i];
      for (unsigned int k = 0; k < plist->size(); ++k) {
	icurrent = i * max_similar_patches + k;
	// patch
	patch = (*plist)[k];
	p = patches.select(0, icurrent);
	p = p.select(0, 0);
	idx_copy(*patch, p);
	// label
	labels.set(i, icurrent);
	// pairs
	for (int l = 1; k + l < plist->size(); ++l) {
	  pairs.set(icurrent, ipairs, 0);
	  pairs.set(icurrent + l, ipairs, 1);
	  ipairs++;
	}
      }
    }
    // save files
    if (!save_matrix(patches, patches_filename.c_str()))
      return false;
    if (!save_matrix(labels, labels_filename.c_str()))
      return false;
    if (!save_matrix(pairs, pairs_filename.c_str()))
      return false;
    cout << "saving done." << endl;
    return true;
  }

  } // end namespace ebl

