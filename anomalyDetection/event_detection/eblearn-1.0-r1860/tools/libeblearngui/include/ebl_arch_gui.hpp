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

#ifndef ELB_ARCH_GUI_HPP_
#define ELB_ARCH_GUI_HPP_

#include <ostream>
#include <typeinfo>

#include "ebl_layers_gui.h"
#include "ebl_basic_gui.h"

using namespace std;

namespace ebl {

  ////////////////////////////////////////////////////////////////
  // module_1_1_gui

#define DISPLAY_1_1(name1, name2)					\
  template<typename T, class Tstate>					\
  void module_1_1_gui::name1(module_1_1<T,Tstate> &m,			\
			     Tstate &in, Tstate &out,			\
			     unsigned int h0, unsigned int w0,		\
			     double zoom, T vmin, T vmax,		\
			     bool show_out, int wid,			\
			     const char *wname) {			\
    name2(m, in, out, h0, w0, zoom, vmin, vmax, show_out, wid, wname);	\
  }

#define DISPLAY2_1_1(name1, name2, op, T)				\
  template<typename T, class Tstate>					\
  void module_1_1_gui::name2(module_1_1<T,Tstate> &m,			\
			     Tstate &in, Tstate &out,			\
			     unsigned int &h0, unsigned int &w0,	\
			     double zoom, T vmin, T vmax,		\
			     bool show_out, int wid,			\
			     const char *wname) {			\
    display_wid_fprop = (wid >= 0) ? wid :				\
      new_window((wname ? wname : "module_1_1 display"));		\
    select_window(display_wid_fprop);					\
    gui << black_on_white(255, 0) << gui_only();			\
    									\
    if (dynamic_cast< layers<T,Tstate>* >(&m)) {			\
      /* layers */							\
      layers_gui::name1(*this, dynamic_cast< layers<T,Tstate>& >(m),	\
			in, out, h0, w0, zoom, vmin, vmax, show_out);	\
      /*    } else if(dynamic_cast< layers_2_gen<Tstate,Tstate,Tstate >* >(&m)){ */ \
      /* layers_2 */							\
      /*      layers_2_gui::						\
	      name1(*this, dynamic_cast< layers_2<T,Tstate>& >(m),	\
	      in, out, h0, w0, zoom, vmin, vmax, show_out); */		\
    } else if (dynamic_cast< full_layer<T,Tstate>* >(&m)) {		\
      /* full_layer */							\
      full_layer_gui::name1(dynamic_cast< full_layer<T,Tstate>& >(m), in, \
			    out, h0, w0, zoom, vmin, vmax,show_out);	\
    } else if (dynamic_cast< convolution_layer<T,Tstate>* >(&m)) {	\
      /* convolution_layer */						\
      convolution_layer_gui::						\
	name1(dynamic_cast< convolution_layer<T,Tstate>& >(m),		\
	      in, out, h0, w0, zoom, vmin, vmax, show_out);		\
    } else if (dynamic_cast< convabsnorm_layer<T,Tstate>* >(&m)) {	\
      /* convabsnorm_layer */						\
      convabsnorm_layer_gui::						\
	name1(dynamic_cast< convabsnorm_layer<T,Tstate>& >(m),		\
	      in, out, h0, w0, zoom, vmin, vmax, show_out);		\
    } else if (dynamic_cast< subsampling_layer<T,Tstate>* >(&m)) {	\
      /* subsampling_layer */						\
      subsampling_layer_gui::						\
	name1(dynamic_cast< subsampling_layer<T,Tstate>& >(m),		\
	      in, out, h0, w0, zoom, vmin, vmax, show_out);		\
    } else if (dynamic_cast< convolution_module_replicable<T,Tstate>* >(&m)) { \
      convolution_module_gui::						\
	name1(dynamic_cast< convolution_module<T,Tstate>& >(m),		\
	      in, out, h0, w0, zoom, vmin, vmax, show_out);		\
    } else if (dynamic_cast< subsampling_module_replicable<T,Tstate>* >(&m)) { \
      subsampling_module_gui::						\
	name1(dynamic_cast< subsampling_module<T,Tstate>& >(m),		\
	      in, out, h0, w0, zoom, vmin, vmax, show_out);		\
    } else {								\
      unsigned int h = h0, w = w0;					\
      /* display text */						\
      gui << gui_only() << at(h, w) << m.name() << " in:" << in.T	\
	  << at(h + 15, w) << "min:" << idx_min(in.T)			\
	  << at(h + 30, w) << "max:" << idx_max(in.T);			\
      w += 150;								\
      /* display inputs */						\
      idx_bloop1(inp, in.T, T) {					\
	if (w - w0 < MAXWIDTH) {					\
	  draw_matrix(inp, h, w, zoom, zoom, vmin, vmax);		\
	  w += (uint) (inp.dim(1) * zoom + 1);				\
	}								\
      }									\
      h += (uint) (inp.dim(0) * zoom + 1);				\
      /* run it */							\
      ((module_1_1<T,Tstate>&)m).op(in, out);				\
      w = w0;								\
      /* display text */						\
      gui << gui_only() << at(h, w) << m.name() << " out:" << out.T	\
	  << at(h + 15, w) << "min:" << idx_min(out.T)			\
	  << at(h + 30, w) << "max:" << idx_max(out.T);			\
      w += 150;								\
      /* display outputs */						\
      idx_bloop1(outp, out.T, T) {					\
	if (w - w0 < MAXWIDTH) {					\
	  draw_matrix(outp, h, w, zoom, zoom, vmin, vmax);		\
	  w += (uint) (outp.dim(1) * zoom + 1);				\
	}								\
      }									\
      h0 = h + (uint) (outp.dim(0) * zoom + 1);				\
    }}
  /*  else {								\
    	cerr << "Warning: unknown display function for module_1_1 object"; \
    	cerr << " (" << typeid(m).name() << ")." << endl;		   \
    	}								\
    	}*/
  
  DISPLAY_1_1(display_fprop, display_fprop2)
  DISPLAY2_1_1(display_fprop, display_fprop2, fprop, x)
  DISPLAY_1_1(display_bprop, display_bprop2)
  DISPLAY2_1_1(display_bprop, display_bprop2, bprop, dx)
  DISPLAY_1_1(display_bbprop, display_bbprop2)
  DISPLAY2_1_1(display_bbprop, display_bbprop2, bbprop, ddx)

  template<typename T, class Tstate>					
  void module_1_1_gui::
  display_internals(module_1_1<T,Tstate> &m, unsigned int &h0, unsigned int &w0,
		    double zoom, T vmin, T vmax, int wid,
		    const char *wname, uint maxwidth) {
    display_wid_fprop = (wid >= 0) ? wid :				
      (display_wid_fprop >= 0) ? display_wid_fprop :
      new_window((wname ? wname : "module_1_1 display"));		
    select_window(display_wid_fprop);
    
    gui << black_on_white(255, 0) << gui_only();			
    unsigned int h = h0, w = w0;					
    /* display internals */
    for (uint i = 0; i < m.internals.size(); ++i) {
      idx<T> internal = m.internals[i];
      string s;
      if (i < m.internals_str.size())
	s = m.internals_str[i];
      // expect 3d internal
      if (internal.order() != 3)
	eblerror("only 3d buffers currently supported, found " << internal);
      // show internal's text
      gui << gui_only() << at(h, w) << m.name() << " " << s << " " << internal
	<< at(h + 15, w) << "min:" << idx_min(internal)			
	<< at(h + 30, w) << "max:" << idx_max(internal);			
      w += TEXT_MARGIN;      
      // show each slice of internal
      idx_bloop1(in, internal, T) {
	if (w - w0 >= maxwidth) {
	  h += (uint) (in.dim(0) * zoom + 1);
	  w = w0 + TEXT_MARGIN;
	}
	if (h < MAXHEIGHT) {
	  draw_matrix(in, h, w, zoom, zoom, vmin, vmax);		
	  w += (uint) (in.dim(1) * zoom + 1);
	}
      }
      w = w0;
      h += (uint) (internal.dim(1) * zoom + 1);
    }
    if (!m.internals.empty())
      h0 = std::max(h0 + TEXT_MIN_HEIGHT, h + MODULES_HSPACE);
  }

  ////////////////////////////////////////////////////////////////
  // module_2_1_gui

#define DISPLAY_2_1(name1)						\
  template<typename T, class Tstate, class Tin2, class Tout>		\
  void module_2_1_gui::name1(module_2_1<T,Tstate,Tstate,Tout> &m,	\
			     Tstate &in1, Tin2 &in2, Tout &out,		\
			     unsigned int &h0, unsigned int &w0,	\
			     double zoom, T vmin, T vmax,		\
			     bool show_out, int wid,			\
			     const char *wname) {			\
    display_wid_fprop = (wid >= 0) ? wid :				\
      new_window((wname ? wname : "module_2_1 display"));		\
    select_window(display_wid_fprop);					\
    /*    disable_window_updates();*/					\
    gui << black_on_white(255, 0) << gui_only();			\
    									\
    if (dynamic_cast< trainable_module<T,T,T,Tstate,Tstate,Tout>* >(&m)) { \
      /* trainable_module */						\
      trainable_module_gui::						\
	name1(*this, dynamic_cast<trainable_module<T,T,T,Tstate,Tstate,Tout>& >(m), \
	      in1, in2, out, h0, w0, zoom, vmin, vmax, show_out, wid);	\
    } else {								\
      cerr << "Warning: unknown display function for module_2_1 object"; \
      cerr << "(" << typeid(m).name() << ")." << endl;			\
    }									\
    /*    enable_window_updates();*/					\
  }
  
  DISPLAY_2_1(display_fprop)
  DISPLAY_2_1(display_bprop)
  DISPLAY_2_1(display_bbprop)
  
  ////////////////////////////////////////////////////////////////
  // trainable_module_gui

#define DISPLAY_TRAINABLE(name1, name2)					\
  template<typename T, class Tin1, class Tin2, class Ten,typename	\
	   Tds1,typename Tds2>						\
  void trainable_module_gui::						\
  name1(trainable_module<T,Tds1,Tds2,Tin1,Tin1,Ten> &dse,	\
	Tin1 &i1, Tin2 &i2, Ten &energy, uint &h0, uint &w0,		\
	double zoom, T vmin, T vmax, bool show_out, int wid,		\
	const char *wname) {						\
    module_1_1_gui m;							\
    m.name2(dse.mod1, i1, dse.out1, h0, w0, zoom, vmin, vmax,		\
	    show_out, wid, wname);					\
    /* TODO add energy, answer display */				\
  }

  DISPLAY_TRAINABLE(display_fprop, display_fprop2)
  DISPLAY_TRAINABLE(display_bprop, display_bprop2)
  DISPLAY_TRAINABLE(display_bbprop, display_bbprop2)

  ////////////////////////////////////////////////////////////////
  // layers_gui

#define DISPLAY_LAYERS(name1, name2, T)					\
  template<typename T, class Tstate>					\
  void layers_gui::name1(module_1_1_gui &g,				\
			 layers<T,Tstate> &ln,				\
			 Tstate &in, Tstate &out,			\
			 unsigned int &h0, unsigned int &w0,		\
			 double zoom, T vmin, T vmax,			\
			 bool show_out) {				\
    if (ln.modules.empty())						\
      return ;								\
    /* initialize buffers */						\
    ln.hi = &in;							\
    ln.ho = &out;							\
    /* narrow input data if required by branch */			\
    Tstate narrowed;							\
    if (ln.branch && ln.branch_narrow) {				\
      narrowed = ln.hi->narrow(ln.narrow_dim, ln.narrow_size,		\
			       ln.narrow_offset);			\
      ln.hi = &narrowed;						\
    }									\
    /* loop over modules */						\
    for(uint i = 0; i < ln.modules.size(); i++) {			\
      /* if last module, output into out */				\
      if (i == ln.modules.size() - 1 && !ln.branch)			\
	ln.ho = &out;							\
      else { /* not last module, use hidden buffers */			\
	ln.ho = (Tstate*) ln.hiddens[i];				\
	/* allocate hidden buffer if necessary */			\
	if (ln.ho == NULL) {						\
	  /* create idxdim of same order but sizes 1 */			\
	  idxdim d = ln.hi->x.get_idxdim();				\
	  for (int k = 0; k < d.order(); ++k)				\
	    d.setdim(k, 1);						\
	  /* assign buffer */						\
	  ln.hiddens[i] = new Tstate(d);				\
	  ln.ho = (Tstate*) ln.hiddens[i];				\
	}								\
      }									\
      /* run module */							\
      g.name2(*ln.modules[i], *ln.hi, *ln.ho,				\
	      h0, w0, zoom, vmin, vmax, false, g.display_wid_fprop);	\
      /* keep same input if current module is a branch */		\
      /* otherwise take out as in */					\
      bool isbranch = false;						\
      if (dynamic_cast<layers<T,Tstate>*>(ln.modules[i]) &&		\
	  ((layers<T,Tstate>*)ln.modules[i])->branch)			\
	isbranch = true;						\
      if (!isbranch)							\
	ln.hi = ln.ho;							\
      if (isbranch && i + 1 == ln.modules.size())			\
	ln.ho = ln.hi; /* if last is branch, set in to be the branch out */ \
    }									\
    if (show_out) {							\
      unsigned int h = h0, w = w0, zoomf = 1;				\
      /* display outputs text */					\
      gui << gui_only() << at(h, w) << "outputs:" << out.x;		\
      w += 150;								\
      /* display outputs */						\
      idx_bloop1(m, out.T, T) {						\
	draw_matrix(m, h, w, zoom * zoomf, zoom * zoomf, vmin, vmax);	\
	w += (uint) (m.dim(1) * zoom * zoomf + 1);			\
      }									\
      h0 += (uint) (m.dim(0) * zoom * zoomf + 1);			\
    }									\
  }

  DISPLAY_LAYERS(display_fprop, display_fprop2, x)
  DISPLAY_LAYERS(display_bprop, display_bprop2, dx)
  DISPLAY_LAYERS(display_bbprop, display_bbprop2, ddx)

  template<typename T, class Tstate>					
  void layers_gui::display_internals(layers<T,Tstate> &ln,
				     unsigned int &h0,
				     unsigned int &w0, double zoom, T vmin,
				     T vmax, uint maxwidth) {
    if (ln.modules.empty())						
      return ;
    /* loop over modules */						
    for(uint i = 0; i < ln.modules.size(); i++) {			
      /* display module's internals */
      m11g.display_internals(*ln.modules[i], h0, w0, zoom, vmin, vmax,
			     -1, NULL, maxwidth);
      // recursively display layers
      if (dynamic_cast<layers<T,Tstate>*>(ln.modules[i])) {
	layers<T,Tstate> &b = (layers<T,Tstate>&) *(ln.modules[i]);
	display_internals(b, h0, w0, zoom, vmin, vmax, maxwidth);
      }
    }
  }

  template<typename T, class Tstate>					
  void layers_gui::display_internals(module_1_1<T,Tstate> &m,
				     unsigned int &h0,
				     unsigned int &w0, double zoom, T vmin,
				     T vmax, uint maxwidth) {
    if (dynamic_cast< layers<T,Tstate>*>(&m))
      display_internals((layers<T,Tstate>&) m, h0, w0, zoom, vmin, vmax,
			maxwidth);
    else
      eblerror("failed to cast module_1_1 m to layers object for display");
  }
  
  ////////////////////////////////////////////////////////////////
  // layers2_gui

#define DISPLAY_LAYERS2(name1)						\
  template<typename T, class Tstate>					\
  void layers_2_gui::name1(module_1_1_gui &g,				\
			   layers_2<T> &l2,				\
			   Tstate &in, Tstate &out,			\
			   unsigned int &h0, unsigned int &w0,		\
			   double zoom, T vmin, T vmax,			\
			   bool show_out) {				\
    g.name1(l2.layer1, in, l2.hidden,					\
	    h0, w0, zoom, vmin, vmax, false, g.display_wid_fprop);	\
    g.name1(l2.layer2, l2.hidden, out,					\
	    h0, w0, zoom, vmin, vmax, false, g.display_wid_fprop);	\
  }

  // DISPLAY_LAYERS2(display_fprop)
  // DISPLAY_LAYERS2(display_bprop)
  // DISPLAY_LAYERS2(display_bbprop)

} // end namespace ebl

#endif /* ELB_ARCH_GUI_HPP_ */
