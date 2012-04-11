/***************************************************************************
 *   Copyright (C) 2009 by Pierre Sermanet *
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

#define MAXWIDTH 1000

using namespace std;

namespace ebl {
  
  ////////////////////////////////////////////////////////////////

#define WEIGHTED_STD_MODULE_GUI(name, T, op)				\
  template <typename T, class Tstate>					\
  void weighted_std_module_gui::name(weighted_std_module<T,Tstate> &wsm, \
				     Tstate &in, Tstate &out,		\
				     unsigned int &h0,			\
				     unsigned int &w0,			\
				     double zoom, T vmin, T vmax,	\
				     bool show_out, bool run) {		\
    /* run it */							\
    if (run)								\
      wsm.op(in, out);							\
    uint h = h0, w = w0;						\
    /* display text */							\
    gui << gui_only() << at(h, w) << "inmean out:" << wsm.inmean.T	\
	<< at(h + 15, w) << "min:" << idx_min(wsm.inmean.T)		\
	<< at(h + 30, w) << "max:" << idx_max(wsm.inmean.T);		\
    w += 150;								\
    /* display image */							\
    {idx_bloop1(m, wsm.inmean.T, T) {					\
	if (w - w0 < MAXWIDTH) {					\
	  draw_matrix(m, h, w, zoom, zoom, vmin, vmax);			\
	  w += (uint) (m.dim(1) * zoom + 1);				\
	}								\
      }									\
      h0 += (uint) (m.dim(0) * zoom + 1);				\
    }									\
    w = w0;								\
    h = h0;								\
    /* display text */							\
    gui << gui_only() << at(h, w) << "inzmeansq out:" << wsm.inzmeansq.T \
	<< at(h + 15, w) << "min:" << idx_min(wsm.inzmeansq.T)		\
	<< at(h + 30, w) << "max:" << idx_max(wsm.inzmeansq.T);		\
    w += 150;								\
    /* display image */							\
    {idx_bloop1(m, wsm.inzmeansq.T, T) {				\
	if (w - w0 < MAXWIDTH) {					\
	  draw_matrix(m, h, w, zoom, zoom, vmin, vmax);			\
	  w += (uint) (m.dim(1) * zoom + 1);				\
	}								\
      }									\
      h0 += (uint) (m.dim(0) * zoom + 1);				\
    }									\
    w = w0;								\
    h = h0;								\
    /* display text */							\
    gui << gui_only() << at(h, w) << "invar out:" << wsm.invar.T	\
	<< at(h + 15, w) << "min:" << idx_min(wsm.invar.T)		\
	<< at(h + 30, w) << "max:" << idx_max(wsm.invar.T);		\
    w += 150;								\
    /* display image */							\
    {idx_bloop1(m, wsm.invar.T, T) {					\
	if (w - w0 < MAXWIDTH) {					\
	  draw_matrix(m, h, w, zoom, zoom, vmin, vmax);			\
	  w += (uint) (m.dim(1) * zoom + 1);				\
	}								\
      }									\
      h0 += (uint) (m.dim(0) * zoom + 1);				\
    }									\
    w = w0;								\
    h = h0;								\
    /* display text */							\
    gui << gui_only() << at(h, w) << "instd out:" << wsm.instd.T	\
	<< at(h + 15, w) << "min:" << idx_min(wsm.instd.T)		\
	<< at(h + 30, w) << "max:" << idx_max(wsm.instd.T);		\
    w += 150;								\
    /* display image */							\
    {idx_bloop1(m, wsm.instd.T, T) {					\
	if (w - w0 < MAXWIDTH) {					\
	  draw_matrix(m, h, w, zoom, zoom, vmin, vmax);			\
	  w += (uint) (m.dim(1) * zoom + 1);				\
	}								\
      }									\
      h0 += (uint) (m.dim(0) * zoom + 1);				\
    }									\
    w = w0;								\
    h = h0;								\
    /* display text */							\
    gui << gui_only() << at(h, w) << "thstd out:" << wsm.thstd.T	\
	<< at(h + 15, w) << "min:" << idx_min(wsm.thstd.T)		\
	<< at(h + 30, w) << "max:" << idx_max(wsm.thstd.T);		\
    w += 150;								\
    /* display image */							\
    {idx_bloop1(m, wsm.thstd.T, T) {					\
	if (w - w0 < MAXWIDTH) {					\
	  draw_matrix(m, h, w, zoom, zoom, vmin, vmax);			\
	  w += (uint) (m.dim(1) * zoom + 1);				\
	}								\
      }									\
      h0 += (uint) (m.dim(0) * zoom + 1);				\
    }									\
    w = w0;								\
    h = h0;								\
    /* display text */							\
    gui << gui_only() << at(h, w) << "invstd out:" << wsm.invstd.T	\
	<< at(h + 15, w) << "min:" << idx_min(wsm.invstd.T)		\
	<< at(h + 30, w) << "max:" << idx_max(wsm.invstd.T);		\
    w += 150;								\
    /* display image */							\
    {idx_bloop1(m, wsm.invstd.T, T) {					\
	if (w - w0 < MAXWIDTH) {					\
	  draw_matrix(m, h, w, zoom, zoom, vmin, vmax);			\
	  w += (uint) (m.dim(1) * zoom + 1);				\
	}								\
      }									\
      h0 += (uint) (m.dim(0) * zoom + 1);				\
    }									\
    w = w0;								\
    h = h0;								\
  }

  WEIGHTED_STD_MODULE_GUI(display_fprop, x, fprop)
  WEIGHTED_STD_MODULE_GUI(display_bprop, dx, bprop)
  WEIGHTED_STD_MODULE_GUI(display_bbprop, ddx, bbprop)
  
} // end namespace ebl
