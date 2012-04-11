/***************************************************************************
 *   Copyright (C) 2009 by Pierre Sermanet   *
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

#ifndef SIMILAR_PATCHES_H_
#define SIMILAR_PATCHES_H_

#include "libidx.h"
#ifdef __GUI__
#include "libidxgui.h"
#endif

#include <vector>

using namespace std;

namespace ebl {

  ////////////////////////////////////////////////////////////////
  // similar_patches

  class similar_patches {
  private:
    unsigned int			max_similar_patches;
    unsigned int			pheight;
    unsigned int			pwidth;
    unsigned int			iheight;
    unsigned int			iwidth;
    unsigned int                        wdisplay;
    unsigned int                        display_index;
    vector< vector< idx<ubyte>* > *>	dataset;
    vector< vector< idx<ubyte>* > *>	current_patches;
  public:
    unsigned int			max_current_patches;
    vector< pair<int, int> >		current_patches_xy;

    similar_patches(unsigned int maxcurrent, unsigned int maxsimilar,
		    unsigned int ph, unsigned int pw,
		    unsigned int ih, unsigned int iw);
    virtual ~similar_patches();
    bool add_similar_patch(idx<ubyte> &im, int h, 
			   int w, unsigned int index);
    bool current_patch_empty(unsigned int index);
    void display_dataset(unsigned int maxh, unsigned int maxw);
    bool save_dataset(const char *directory);
  };

} // namespace ebl {

#endif /* SIMILAR_PATCHES_H_ */
