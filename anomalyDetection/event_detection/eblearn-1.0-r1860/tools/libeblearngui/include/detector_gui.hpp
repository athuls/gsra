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

#ifndef DETECTOR_GUI_HPP_
#define DETECTOR_GUI_HPP_

using namespace std;

#include <deque>

namespace ebl {

  ////////////////////////////////////////////////////////////////
  // detector_gui

  template <typename T, class Tstate>
  detector_gui<T,Tstate>::
  detector_gui(uint draw_extracted_,
	       bool show_detqueue_, uint step_, uint qheight_,
	       uint qwidth_, bool show_detqueue2_, uint step2_,
	       uint qheight2_, uint qwidth2_)
    : draw_extracted(draw_extracted_),
      show_detqueue(show_detqueue_), show_detqueue2(show_detqueue2_),
      step(step_), step2(step2_), qheight(qheight_), qwidth(qwidth_), 
      qheight2(qheight2_), qwidth2(qwidth2_), detcnt(0) {
    cout << "detector_gui: " << (draw_extracted > 0 ? "" : "not ")
	 << "showing extracted windows." << endl;
  }

  template <typename T, class Tstate>
  detector_gui<T,Tstate>::~detector_gui() {
  }
  
  template <typename T, class Tstate> template <typename Tin>
  vector<bbox*>& detector_gui<T,Tstate>::
  display(detector<T,Tstate> &cl, idx<Tin> &img, double threshold,
	  const char *frame_name, unsigned int h0, unsigned int w0,
	  double dzoom,  T vmin, T vmax, int wid, const char *wname,
	  float transparency) {
    display_wid = (wid >= 0) ? wid :
      new_window((wname ? wname : "detector"));
    select_window(display_wid);

    // run network
    vector<bbox*>& vb = cl.fprop(img, threshold, frame_name);
    display_minimal(img, vb, cl.labels, h0, w0, dzoom, vmin, vmax, display_wid,
		    false, transparency);
    // draw masks class
    if (!mask_class.empty()) {
      idx<T> mask = cl.get_mask(mask_class);
      draw_mask(mask, h0, w0, dzoom, dzoom,
		255, 0, 0, 127, mask_threshold);
    }
    return vb;
  }

  template <typename T, class Tstate> template <typename Tin>
  void detector_gui<T,Tstate>::
  display_minimal(idx<Tin> &img, vector<bbox*>& vb, vector<string> &labels,
		  unsigned int h0, unsigned int w0,
		  double dzoom,  T vmin, T vmax, int wid, bool show_parts,
		  float transparency) {
    // draw image
    draw_matrix(img, h0, w0, dzoom, dzoom, (Tin)vmin, (Tin)vmax);   
    // draw bboxes (in reverse order to display best on top)
    bbox *bb = NULL;
    for (int i = vb.size() - 1; i >= 0; --i) {
      bb = vb[(uint) i];
      // draw parts
      if (show_parts && dynamic_cast<bbox_parts*>(bb))
	draw_bbox_parts(*((bbox_parts*) bb), labels, h0, w0, dzoom);
      // draw box
      draw_bbox(*bb, labels, h0, w0, dzoom, transparency);
    }
  }

  template <typename T, class Tstate> template <typename Tin> 
  vector<bbox*>& detector_gui<T,Tstate>::
  display_input(detector<T,Tstate> &cl, idx<Tin> &img, double threshold,
		const char *frame_name, 
		unsigned int h0, unsigned int w0, double dzoom, T vmin,
		T vmax, int wid, const char *wname, float transparency) {
    display_wid = (wid >= 0) ? wid :
      new_window((wname ? wname : "detector: output"));
    select_window(display_wid);
    //    disable_window_updates();


    vector<bbox*> &bb = display(cl, img, threshold, frame_name,
				h0, w0, dzoom, vmin, vmax, wid, wname,
				transparency);
    w0 += (uint) (img.dim(1) * dzoom + 5);
    // draw input
    draw_matrix(img, "input", h0, w0, dzoom, dzoom, (Tin) vmin, (Tin) vmax);
    return bb;
  }

  template <typename T, class Tstate> template <typename Tin>
  vector<bbox*>& detector_gui<T,Tstate>::
  display_inputs_outputs(detector<T,Tstate> &cl, idx<Tin> &img,
			 double threshold,
			 const char *frame_name, 
			 unsigned int h0, unsigned int w0, double dzoom,
			 T vmin, T vmax, int wid, const char *wname,
			 T in_vmin, T in_vmax, float transparency) {
    display_wid_fprop = (wid >= 0) ? wid : 
      new_window((wname ? wname : "detector: inputs, outputs & internals"));
    select_window(display_wid_fprop);

    // draw input and output
    vector<bbox*>& bb =
      display_input(cl, img, threshold, frame_name,
		    h0, w0, dzoom, in_vmin, in_vmax, display_wid_fprop, wname,
		    transparency);
    // compute min and max of all outputs, to maximize intensity display range
    bool first_time = true;
    if (vmin == vmax) {
      for (uint i = 0; i < cl.outputs.size(); ++i) {
	idx<T> outx = cl.outputs[i]->x;
	if (first_time) {
	  vmin = idx_min(outx);
	  vmax = idx_max(outx);
	  first_time = false;
	} else {
	  vmin = MIN(vmin, idx_min(outx));
	  vmax = std::max(vmax, idx_max(outx));	  
	}
      }
    }
    DEBUG("Displaying outputs in range [" << vmin << ", " << vmax
	  << "], outputs actual range is [" << idx_min(cl.outputs[0]->x) << ", "
	  << idx_max(cl.outputs[0]->x) << "]");
    int h = (int) (cl.indim.dim(1) * dzoom + 20);
    // draw extracted windows
    if (draw_extracted == 1) { // preprocessed
      vector<idx<T> >& pp = cl.get_preprocessed();
      idx<T> m;
      if (pp.size() > 0) {
	m = pp[0];
	h += 15 * 3;
	uint wpp = w0, hpp = h;
	gui << black_on_white() << at(h - 15 * 4, w0) << pp.size()
	    << " positive windows with dimensions " << m;
	ostringstream o;
	o.precision(3);
	for (uint i = 0; i < pp.size() && wpp < 5000; ++i) {
	  hpp = h;
	  m = pp[i];
	  bbox &b = *bb[i];
	  // print bbox infos
	  o.str(""); o << b.confidence; gui << at(hpp - 15 * 3, wpp) << o.str();
	  gui << at(hpp - 15 * 2, wpp) << b.scale_index;
	  gui << at(hpp - 15 * 1, wpp)
	      << ((uint) b.class_id < cl.labels.size() ?
		  cl.labels[b.class_id].c_str() : "***");
	  // draw input box
	  rect<float> r(hpp + b.i.h0 - b.i0.h0,
		      wpp + b.i.w0 - b.i0.w0, b.i.height, b.i.width);
	  draw_box(r, 0, 0, 255);
	  idx_bloop1(chan, m, T) {
	    // draw channel
	    draw_matrix(chan, hpp, wpp, dzoom, dzoom, (T)vmin, (T)vmax);
	    hpp += chan.dim(0) + 1;
	  }
	  wpp += m.dim(2) + 2;
	}
	h = hpp + 20;
      }
    }
    else if (draw_extracted == 2) { // originals
      vector<idx<T> >& pp = cl.get_originals();
      idx<T> m;
      if (pp.size() > 0) {
	m = pp[0];
	h += 15 * 3;
	uint wpp = w0, hpp = h;
	gui << black_on_white() << at(h - 15 * 4, w0) << pp.size()
	    << " positive windows with dimensions " << m;
	ostringstream o;
	o.precision(3);
	for (uint i = 0; i < pp.size() && wpp < 5000; ++i) {
	  hpp = h;
	  m = pp[i];
	  bbox &b = *bb[i];
	  // print bbox infos
	  o.str(""); o << b.confidence; gui << at(hpp - 15 * 3, wpp) << o.str();
	  gui << at(hpp - 15 * 2, wpp) << b.scale_index;
	  gui << at(hpp - 15 * 1, wpp)
	      << ((uint) b.class_id < cl.labels.size() ?
		  cl.labels[b.class_id].c_str() : "***");
	  m = m.shift_dim(0, 2);
	  draw_matrix(m, hpp, wpp, dzoom, dzoom, (T)0, (T)255);
	  hpp += m.dim(0) + 1;
	  wpp += m.dim(1) + 2;
	}
	h = hpp + 20;
      }
    }
    // draw internal inputs and outputs
    int scale = 0;
    // maximum height of outputs to display
    int ohmax = cl.outputs[0]->x.dim(1);
    // find the maximum height of inputs to display
    int ihmax = 0;
    for (uint i = 0; i < cl.ppinputs[0]->size(); ++i) {
      Tstate &t = (*(cl.ppinputs[0]))[i];
      ihmax += t.x.dim(1) + 2;
    }
    ostringstream s;
    // display all outputs
    first_time = true;
    for (uint i = 0; i < cl.ppinputs.size(); ++i) {
      Tstate &t = (*cl.ppinputs[i])[0];
      idx<T> outx = cl.outputs[i]->x;

      // draw inputs
      string ss;
      ss << "scale #" << scale;
      int htmp = h;
      for (uint j = 0; j < cl.ppinputs[i]->size(); ++j) {
	Tstate &t = (*(cl.ppinputs[i]))[j];
	ss << " " << t.x.dim(1) << "x" << t.x.dim(2);
	idx<T> tx = t.x.shift_dim(0, 2);
	draw_matrix(tx, htmp, w0, dzoom, dzoom, (T)vmin, (T)vmax);
	htmp += t.x.dim(1) + 2;
      }
      gui << black_on_white() << at(h - 15, w0) << ss;
      gui << white_on_transparent();
      // draw bboxes on scaled input
      if (!cl.bboxes_off) {
	for (vector<bbox*>::iterator i = bb.begin(); i != bb.end(); ++i) {
	  if (scale == (*i)->scale_index)
	    draw_bbox(**i, cl.labels, h, w0, dzoom, transparency, false);
	}
	// draw outputs
	int hcat = 0;
	double czoom = dzoom * 2.0;
	uint lab = 0;
	{ idx_bloop1(category, outx, T) {
	    s.str("");
	    if (first_time) {
	      if (lab < cl.labels.size())
		s << cl.labels[lab] << " ";
	      else
		s << "feature " << lab << " ";
	    }
	    s << category.dim(0) << "x" << category.dim(1);
	    gui << at((uint) (h + ihmax * dzoom + 5 + hcat), 
		      (uint) (w0 + category.dim(1) * czoom + 2));
	    gui << black_on_white() << s.str();
	    draw_matrix(category, (uint) (h + ihmax * dzoom + 5 + hcat), w0, 
			czoom, czoom, vmin, vmax);
	    hcat += (int) (ohmax * czoom + 2);
	    lab++;
	  }}
      }
      scale++;
      w0 += (uint) (t.x.dim(2) * dzoom + 5);
      first_time = false;
    }

    // display queues of detections
    if (show_detqueue || show_detqueue2) {
      uint hh0 = h0;
      vector<idx<T> > &new_detections = cl.get_originals();
      if (show_detqueue)
	update_and_display_queue(detqueue, step, qheight, qwidth,
				 new_detections, detcnt, hh0, w0, dzoom);
      if (show_detqueue2)
	update_and_display_queue(detqueue2, step2, qheight2, qwidth2,
				 new_detections, detcnt, hh0, w0, dzoom);
      detcnt += new_detections.size();
    }
    // reactive window drawing
    // enable_window_updates();
    return bb;
  }

  template <typename T, class Tstate> void detector_gui<T,Tstate>::
  update_and_display_queue(deque<idx<T> > &queue, uint step, uint qheight,
			   uint qwidth, vector<idx<T> > &new_detections,
			   uint detcnt, uint &h0, uint &w0, double dzoom) {
    // update queue
    uint queuesz = qheight * qwidth;
    // loop over all new detections and add new ones based on the step
    for (typename vector<idx<T> >::iterator i = new_detections.begin();
	 i != new_detections.end(); ++i, detcnt++) {
      if (!(detcnt % std::max((uint) 1, step))) { // add when multiple of step
	if ((queue.size() >= queuesz) && (queue.size() > 0))
	  queue.pop_front();
	queue.push_back(*i);
      }
    }
    // display queue
    uint w = 0, wn = 0, h = 0;
    intg hmax = 0, wmax = 0;
    h = h0;
    for (typename deque<idx<T> >::iterator i = queue.begin();
	 i != queue.end(); ++i) {
      draw_matrix(*i, h, w0 + w, dzoom, dzoom, (T)0, (T)255);
      w += i->dim(1) + 2;
      wn++;
      hmax = std::max(hmax, i->dim(0));
      wmax = std::max(wmax, (intg) w);
      if (wn >= qwidth) {
	wn = 0;
	w = 0;
	h += hmax + 2;
	hmax = 0;
      }
    }
    // update h0 and w0
    h0 += hmax;
    w0 += wmax;
  }

  template <typename T, class Tstate> template <typename Tin>
  vector<bbox*>& detector_gui<T,Tstate>::
  display_all(detector<T,Tstate> &cl, idx<Tin> &img, double threshold,
	      const char *frame_name,
	      unsigned int h0, unsigned int w0, double dzoom, T vmin,
	      T vmax, int wid, const char *wname) {
    display_wid_fprop = (wid >= 0) ? wid : 
      new_window((wname ? wname : "detector: inputs, outputs & internals"));
    select_window(display_wid_fprop);

    // draw input and output
    vector<bbox*>& bb =
      display_inputs_outputs(cl, img, threshold, frame_name,
			     h0, w0, dzoom, vmin, vmax, display_wid_fprop);

    // disable_window_updates();
    // draw internal states of first scale
    w0 = (cl.indim.dim(2) + 5) * 2 + 5;
    module_1_1_gui mg;
    cl.prepare(img);
    cl.prepare_scale(0);
    mg.display_fprop(*(module_1_1<T,Tstate>*) &cl.thenet,
    		     *cl.input, *cl.output, h0, w0, (double) 1.0,
		     (T) -1.0, (T) 1.0, true, display_wid_fprop);
    //    enable_window_updates();
    return bb;
  }

  template <typename T, class Tstate> template <typename Tin>
  void detector_gui<T,Tstate>::display_current(detector<T,Tstate> &cl, 
					       idx<Tin> &sample,
					       int wid, const char *wname,
					       double dzoom){
    display_wid_fprop = (wid >= 0) ? wid : 
      new_window((wname ? wname : "detector: inputs, outputs & internals"));
    select_window(display_wid_fprop);
    disable_window_updates();
    clear_window();
    // draw internal states of first scale
    module_1_1_gui mg;
    cl.prepare(sample);
    cl.prepare_scale(0);
    mg.display_fprop(*(module_1_1<T,Tstate>*) &cl.thenet,
    		     *cl.input, *cl.output, (uint) 0, (uint) 0, dzoom,
		     (T) -1.0, (T) 1.0, true, display_wid_fprop);
    enable_window_updates();
  }

  template <typename T, class Tstate>
  void detector_gui<T,Tstate>::set_mask_class(const char *name, T threshold) {
    if (name) {
      mask_class = name;
      mask_threshold = threshold;
    }
  }
  
} // end namespace ebl

#endif
