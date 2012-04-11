/***************************************************************************
 *   Copyright (C) 2010 by Pierre Sermanet *
 *   pierre.sermanet@gmail.com *
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

#ifndef DETECTION_THREAD_HPP_
#define DETECTION_THREAD_HPP_

#include <map>
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <stdlib.h>
#include <sstream>
#include <iomanip>
#include <time.h>

#include "libeblearn.h"
#include "libeblearntools.h"

#ifdef __GUI__
#include "libeblearngui.h"
#endif

using namespace std;

namespace ebl {

  ////////////////////////////////////////////////////////////////
  // detection thread

  template <typename Tnet>
  detection_thread<Tnet>::
  detection_thread(configuration &conf_, mutex *om, const char *name_,
		   const char *arg2_, bool sync, t_chans tc)
    : thread(om, name_, sync), conf(conf_), arg2(arg2_), frame(120, 160, 1),
      mutex_in(), mutex_out(),
      in_updated(false), out_updated(false), bavailable(false), bfed(false),
      frame_name(""), frame_id(0), outdir(""), total_saved(0), color_space(tc),
      pdetect(NULL) {
  }

  template <typename Tnet>
  detection_thread<Tnet>::~detection_thread() {
  }

  template <typename Tnet>
  void detection_thread<Tnet>::copy_bboxes(vector<bbox*> &bb) {
    // lock data
    mutex_out.lock();
    // clear bboxes
    bbs.clear();
    // copy bboxes
    for (ibox = bb.begin(); ibox != bb.end(); ++ibox) {
      bbs.push_back(new bbox(**ibox));
    }
    // unlock data
    mutex_out.unlock();
  }

  template <typename Tnet>
  void detection_thread<Tnet>::set_out_updated() {
    // lock data
    mutex_out.lock();
    // set flag
    out_updated = true;
    // unlock data
    mutex_out.unlock();
  }
 
  template <typename Tnet>
  bool detection_thread<Tnet>::get_data(vector<bbox*> &bboxes2,
					idx<ubyte> &frame2,
					uint &total_saved_,
					string &frame_name_,
					uint &id) {
    // lock data
    mutex_out.lock();
    // only read data if it has been updated
    if (!out_updated) {
      // unlock data
      mutex_out.unlock();
      return false;
    }
    // clear bboxes
    for (ibox = bboxes2.begin(); ibox != bboxes2.end(); ++ibox) {
      if (*ibox)
	delete *ibox;
    }
    bboxes2.clear();
    // copy bboxes pointers (now responsible for deleting them).
    for (ibox = bbs.begin(); ibox != bbs.end(); ++ibox) {
      bboxes2.push_back(*ibox);
    }
    bbs.clear(); // no use for local bounding boxes anymore, clear them
    // check frame is correctly allocated, if not, allocate.
    if (frame2.order() != uframe.order()) 
      frame2 = idx<ubyte>(uframe.get_idxdim());
    else if (frame2.get_idxdim() != uframe.get_idxdim())
      frame2.resize(uframe.get_idxdim());
    // copy frame
    idx_copy(uframe, frame2);    
    // set total of boxes saved
    total_saved_ = total_saved;
    // set frame name
    frame_name_ = frame_name;
    // set frame id
    id = frame_id;
    // reset updated flag
    out_updated = false;
    // declare thread as available
    bavailable = true;
    // unlock data
    mutex_out.unlock();
    // confirm that we copied data.
    return true;
  }

  template <typename Tnet>
  bool detection_thread<Tnet>::set_data(idx<ubyte> &frame2, string &name,
					uint id) {
    // lock data (non blocking)
    if (!mutex_in.trylock())
      return false;
    // check frame is correctly allocated, if not, allocate.
    if (frame2.order() != uframe.order())
      uframe = idx<ubyte>(frame2.get_idxdim());
    else if (frame2.get_idxdim() != uframe.get_idxdim())
      uframe.resize(frame2.get_idxdim());
    idx_copy(frame2, uframe);	// copy frame
    frame_name = name;		// copy name
    frame_id   = id;		// copy frame_id
    in_updated = true;		// reset updated flag
    mutex_in.unlock();		// unlock data
    bavailable = false;		// declare thread as not available
    bfed       = true;		// data has been fed at least once
    return true;		// confirm that we copied data.
  }

  template <typename Tnet>
  bool detection_thread<Tnet>::set_data(string &fullname, string &name,
					uint id) {
    // lock data (non blocking)
    if (!mutex_in.trylock())
      return false;
    // load image
    uframe = load_image<ubyte>(fullname);
    mout << "loaded image " << fullname << endl;
    // copy name
    frame_name = name;
    // copy frame_id
    frame_id = id;
    // reset updated flag
    in_updated = true;
    // unlock data
    mutex_in.unlock();
    // declare thread as not available
    bavailable = false;
    // confirm that we copied data.
    return true;
  }

  template <typename Tnet>
  bool detection_thread<Tnet>::available() {
    return bavailable;
  }
  
  template <typename Tnet>
  bool detection_thread<Tnet>::fed() {
    return bfed;
  }
  
  template <typename Tnet>
  void detection_thread<Tnet>::set_output_directory(string &out) {
    outdir = out;
  }

  template <typename Tnet>
  void detection_thread<Tnet>::execute() { 
   try {
     // configuration
     bool       silent         = conf.exists_true("silent");
     //bool	color	       = conf.exists_true("color");
     uint	norm_size      = conf.get_uint("normalization_size");
     idxdim     dnorm          = idxdim(norm_size, norm_size);
     Tnet	threshold      = (Tnet) conf.get_double("threshold");
     bool	display        = false;
#ifdef __GUI__
     display = conf.exists_true("display_threads");
#endif
     bool	mindisplay     = conf.exists_true("minimal_display");
     bool       save_video     = conf.exists_true("save_video");
     bool	display_states = conf.exists_true("display_states");
     uint	display_sleep  = conf.get_uint("display_sleep");
     uint       wid	       = 0;	// window id
     uint       wid_states     = 0;	// window id
//      if (!display && save_video) {
//        // we still want to output images but not show them
//        display = true;
// #ifdef __GUI__
//        set_gui_silent();
// #endif
//      }
     // load network and weights in a forward-only parameter
     parameter<SFUNC(Tnet)> theparam;
     idx<ubyte> classes(1,1);
     //try { // try loading classes names but do not stop upon failure
     load_matrix<ubyte>(classes, conf.get_cstring("classes"));
     // } catch(string &err) {
     //   merr << "warning: " << err;
     //   merr << endl;
     // }
     vector<string> sclasses = ubyteidx_to_stringvector(classes);
     answer_module<SFUNC2(Tnet)> *ans =
       create_answer<SFUNC2(Tnet)>(conf, classes.dim(0));
     uint noutputs = ans->get_nfeatures();
     module_1_1<SFUNC(Tnet)> *net =
       create_network<SFUNC(Tnet)>(theparam, conf, noutputs);
     // loading weights
     if (!conf.exists("weights")) { // manual weights
       merr << "warning: \"weights\" variable not defined, loading manually "
	    << "if manual_load defined" << endl;
       if (conf.exists_true("manual_load"))
	   manually_load_network(*((layers<SFUNC(Tnet)>*)net), conf);
     } else { // multiple-file weights
       // concatenate weights if multiple ones
       vector<string> w =
	 string_to_stringvector(conf.get_string("weights"));
       mout << "Loading weights from: " << w << endl;
       theparam.load_x(w);
     }

#ifdef __DEBUGMEM__
     pretty_memory();
#endif
     
     // detector
     detector<SFUNC(Tnet)> detect(*net, sclasses, *ans, NULL, NULL, mout, merr);
     pdetect = &detect;

     // multi-scaling parameters
     double maxs = conf.exists("max_scale")?conf.get_double("max_scale") : 1.0;
     double mins = conf.exists("min_scale")?conf.get_double("min_scale") : 1.0;
     t_scaling scaling_type = SCALES_STEP;
     vector<idxdim> scales;
     if (conf.exists("scaling_type"))
       scaling_type = (t_scaling) conf.get_uint("scaling_type");
     switch (scaling_type) {
     case MANUAL:
       if (!conf.exists("scales"))
	 eblerror("expected \"scales\" variable to be defined in manual mode");
       scales = string_to_idxdimvector(conf.get_cstring("scales"));
       detect.set_resolutions(scales);
       break ;
     case ORIGINAL: detect.set_scaling_original(); break ;
     case SCALES_STEP:
       detect.set_resolutions(conf.get_double("scaling"), maxs, mins);
       break ;
     case SCALES_STEP_UP:
       detect.set_resolutions(conf.get_double("scaling"), maxs, mins);
       detect.set_scaling_type(scaling_type);
       break ;
     default:
       detect.set_scaling_type(scaling_type);
     }

     // optimize memory usage by using only 2 buffers for entire flow
     SBUF<Tnet> input(1, 1, 1), output(1, 1, 1);
     if (!conf.exists_false("mem_optimization"))
       detect.set_mem_optimization(input, output, true);
     // TODO: always keep inputs, otherwise detection doesnt work. fix this.
// 				   conf.exists_true("save_detections") || 
// 				   (display && !mindisplay));
     // zero padding
     float hzpad = 0, wzpad = 0;
     if (conf.exists("hzpad")) hzpad = conf.get_float("hzpad");
     if (conf.exists("wzpad")) wzpad = conf.get_float("wzpad");
     detect.set_zpads(hzpad, wzpad);
     bool bmask_class = false;
     if (conf.exists("mask_class"))
       bmask_class = detect.set_mask_class(conf.get_cstring("mask_class"));
     if (conf.exists("input_min")) // limit inputs size
       detect.set_min_resolution(conf.get_uint("input_min"));
     if (conf.exists("input_max")) // limit inputs size
       detect.set_max_resolution(conf.get_uint("input_max"));
     detect.set_silent();
     if (conf.exists_bool("save_detections")) {
       string detdir = outdir;
       detdir += "detections";
       detdir = detect.set_save(detdir);
       if (conf.exists("save_max_per_frame"))
	 detect.set_save_max_per_frame(conf.get_uint("save_max_per_frame"));
     }
     detect.set_cluster_nms(conf.exists_true("cluster_nms"));
     detect.set_scaler_mode(conf.exists_true("scaler_mode"));
     if (conf.exists("nms"))
       detect.set_pruning((t_pruning)conf.get_uint("nms"), 
			  conf.exists("min_hcenter_dist") ? 
			  conf.get_float("min_hcenter_dist") : 0.0,
			  conf.exists("min_wcenter_dist") ? 
			  conf.get_float("min_wcenter_dist") : 0.0,
			  conf.exists("bbox_max_overlap") ? 
			  conf.get_float("bbox_max_overlap") : 1.0,
			  conf.exists_true("share_parts"),
			  conf.exists("threshold2") ? 
			  (Tnet) conf.get_float("threshold2") : 0.0,
			  conf.exists("bbox_max_center_dist") ? 
			  conf.get_float("bbox_max_center_dist") : 0.0,
			  conf.exists("bbox_max_center_dist2") ? 
			  conf.get_float("bbox_max_center_dist2") : 0.0,
			  conf.exists("bbox_max_wcenter_dist") ? 
			  conf.get_float("bbox_max_wcenter_dist") : 0.0,
			  conf.exists("bbox_max_wcenter_dist2") ? 
			  conf.get_float("bbox_max_wcenter_dist2") : 0.0,
			  conf.exists("min_wcenter_dist2") ? 
			  conf.get_float("min_wcenter_dist2") : 0.0,
			  conf.exists("bbox_max_overlap2") ? 
			  conf.get_float("bbox_max_overlap2") : 0.0,
			  conf.exists_true("mean_bb"),
			  conf.exists("same_scale_mhd") ? 
			  conf.get_float("same_scale_mhd") : 0.0,
			  conf.exists("same_scale_mwd") ? 
			  conf.get_float("same_scale_mwd") : 0.0,
			  conf.exists("min_scale_pred") ? 
			  conf.get_float("min_scale_pred") : 0.0,
			  conf.exists("max_scale_pred") ? 
			  conf.get_float("max_scale_pred") : 0.0
			  );
     if (conf.exists("bbox_hfactor") && conf.exists("bbox_wfactor"))
       detect.set_bbox_factors(conf.get_float("bbox_hfactor"),
			       conf.get_float("bbox_wfactor"),
			       conf.exists("bbox_woverh") ?
			       conf.get_float("bbox_woverh") : 1.0,
			       conf.exists("bbox_hfactor2") ?
			       conf.get_float("bbox_hfactor2") : 1.0,
			       conf.exists("bbox_wfactor2") ?
			       conf.get_float("bbox_wfactor2") : 1.0);
     if (conf.exists("max_object_hratio"))
       detect.set_max_object_hratio(conf.get_double("max_object_hratio"));
     if (conf.exists("net_min_height") && conf.exists("net_min_width"))
       detect.set_min_input(conf.get_int("net_min_height"),
			    conf.get_int("net_min_width"));
     if (conf.exists("smoothing"))
       detect.set_smoothing(conf.get_uint("smoothing"));
     if (conf.exists("background_name"))
       detect.set_bgclass(conf.get_cstring("background_name"));

     // when a bbox file is given, ignore the processing, load the pre-computed
     // bboxes and feed them to the nms (non-maximum suppression).
     bboxes boxes(bbox_all, NULL, mout, merr);
     bool precomputed_boxes = false;
     if (conf.exists("bbox_file")) {
       precomputed_boxes = true;
       string bbfile = conf.get_string("bbox_file");
       boxes.load_eblearn(bbfile);
     }

     string viddir = outdir;
     viddir += "video/";
     mkdir_full(viddir);
     // gui
#ifdef __GUI__
     Tnet display_min = -1.7, display_max = 1.7, display_in_min = 0,
       display_in_max = 255;
     if (conf.exists("display_min"))
       display_min = (Tnet) conf.get_double("display_min");
     if (conf.exists("display_max"))
       display_max = (Tnet) conf.get_double("display_max");
     if (conf.exists("display_in_max"))
       display_in_max = (Tnet) conf.get_double("display_in_max");
     if (conf.exists("display_in_min"))
       display_in_min = (Tnet) conf.get_double("display_in_min");
     float display_transp = 0.0;
     if (conf.exists("display_bb_transparency"))
       display_transp = conf.get_float("display_bb_transparency");
     uint qstep1 = 0, qheight1 = 0, qwidth1 = 0,
       qheight2 = 0, qwidth2 = 0, qstep2 = 0;
     if (conf.exists_bool("queue1")) {
       qstep1 = conf.get_uint("qstep1");
       qheight1 = conf.get_uint("qheight1");
       qwidth1 = conf.get_uint("qwidth1");
     }
     if (conf.exists_bool("queue2")) {
       qstep2 = conf.get_uint("qstep2");
       qheight2 = conf.get_uint("qheight2");
       qwidth2 = conf.get_uint("qwidth2");
     }
     module_1_1_gui	netgui;
     wid_states  = display_states ? new_window("network states"):0;
     night_mode();
     string title = "eblearn object recognition: ";
     title += _name;
     if (display) {
       wid = new_window(title.c_str());
       mout << "displaying in window " << wid << endl;
       night_mode();
     }
     float		zoom = 1;
     if (conf.exists("display_zoom")) zoom = conf.get_float("display_zoom");
     detector_gui<SFUNC(Tnet)>
       dgui((conf.exists("show_extracted") ? conf.get_uint("show_extracted"):0),
	    conf.exists_bool("queue1"), qstep1, qheight1,
	    qwidth1, conf.exists_bool("queue2"), qstep2, qheight2, qwidth2);
     if (bmask_class)
       dgui.set_mask_class(conf.get_cstring("mask_class"),
			   (Tnet) conf.get_double("mask_threshold"));
#endif  
     // timing variables
     timer tpass, toverall;
     long ms;
     // loop
     toverall.start();
     // we're ready
     bavailable = true;
     while(!this->_stop) {
       tpass.restart();
       // wait until a new image is made available
       while (!in_updated && !_stop) {
	 millisleep(1);
       }
       if (_stop) break ;
       // we got a new frame, reset new frame flag
       in_updated = false; // no need to lock mutex
       if (!silent) mout << "processing " << frame_name << endl;
       // check frame is correctly allocated, if not, allocate.
       if (frame.order() != uframe.order()) 
	 frame = idx<Tnet>(uframe.get_idxdim());
       else if (frame.get_idxdim() != uframe.get_idxdim())
	 frame.resize(uframe.get_idxdim());
       // copy frame
       idx_copy(uframe, frame);
       // run detector
       if (!display) { // fprop without display
	 if (precomputed_boxes) {
	   try {
	     vector<bbox*> *bb = boxes.get_group(frame_name);
	     idxdim d = boxes.get_group_dims(frame_name);
	     vector<bbox*> pruned;
	     detect.nms(*bb, pruned, threshold);
	     copy_bboxes(pruned); // make a copy of bounding boxes
	     // resize frame so that caller knows the size of the frame
	     idxdim framedim = frame.get_idxdim();
	     if (d.dim(0) == -1 || d.dim(1) == -1)
	       eblerror("pre-computed boxes must contain full image size, "
			<< "but found: " << d);
	     framedim.setdim(0, d.dim(0));
	     framedim.setdim(1, d.dim(1));
	     frame.resize(framedim);
	     // delete bb and its bboxes
	     for (uint i = 0; i < bb->size(); ++i) {
	       bbox *b = (*bb)[i];
	       if (b) delete b;
	     }
	     delete bb;
	   } catch(eblexception &e) {
	     merr << e << endl;
	   }
	 } else {
	   vector<bbox*> &bb = detect.fprop(frame, threshold,
					    frame_name.c_str());
	   copy_bboxes(bb); // make a copy of bounding boxes
	 }
       }
#ifdef __GUI__
       else { // fprop and display
	 disable_window_updates();
	 select_window(wid);
	 clear_window();
	 string title = _name;
	 title << ": " << frame_name;
	 set_window_title(title.c_str());
	 //	 clear_resize_window();
	 if (mindisplay) {
	   vector<bbox*> &bb =
	     dgui.display(detect, frame, threshold, frame_name.c_str(),
	 		  0, 0, zoom, display_min, display_max,
			  wid, _name.c_str(), display_transp);
	   copy_bboxes(bb); // make a copy of bounding boxes
	 } else {
	   vector<bbox*> &bb =
	     dgui.display_inputs_outputs(detect, frame, threshold,
					 frame_name.c_str(), 0, 0, zoom,
					 display_min, display_max, wid,
					 _name.c_str(),
					 display_in_min, display_in_max,
					 display_transp);
	   copy_bboxes(bb); // make a copy of bounding boxes
	 }
	 enable_window_updates();
       }
       if (display_states) {
	 dgui.display_current(detect, frame, wid_states, NULL, zoom);
	 select_window(wid);
       }
       if (save_video && display) {
	 string fname = viddir;
	 fname += frame_name;
	 save_window(fname.c_str());
	 if (!silent) mout << "saved " << fname << endl;
       }
#endif
       total_saved = detect.get_total_saved();
       ms = tpass.elapsed_milliseconds();
       if (!silent) {
	 detect.pretty_bboxes_short(bbs, mout);
	 mout << "processing=" << ms << " ms ("
	      << tpass.elapsed() << ")" << endl;
       }
#ifdef __DEBUGMEM__
       pretty_memory();
#endif
       // switch 'updated' flag on to warn we just added new data
       set_out_updated();
       // display sleep
       if (display_sleep > 0) {
	 mout << "sleeping for " << display_sleep << "ms." << endl;
	 millisleep(display_sleep);
       }
       if (conf.exists("save_max") && 
	   detect.get_total_saved() > conf.get_uint("save_max"))
	 break ; // limit number of detection saves
     }
     mout << "detection finished. Execution time: " << toverall.elapsed()
	  << endl;
     // free variables
     if (net) delete net;
   } catch(string &err) { eblerror(err.c_str()); }
  }

} // end namespace ebl

#endif /* DETECTION_THREAD_HPP_ */
