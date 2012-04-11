/***************************************************************************
 *   Copyright (C) 2011 by Pierre Sermanet *
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

#ifndef FPROP_THREAD_HPP_
#define FPROP_THREAD_HPP_

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
  fprop_thread<Tnet>::fprop_thread(configuration &conf_,
				   mutex *om,
				   const char *name_,
				   const char *arg2_, bool sync,
				   t_chans tc)
    : thread(om, name_, sync), conf(conf_), arg2(arg2_), frame(120, 160, 1),
      mutex_in(), mutex_out(),
      in_updated(false), out_updated(false), bavailable(false),
      frame_name(""), frame_id(0), outdir(""), color_space(tc),
      bload_image(false), pdetect(NULL) {
  }

  template <typename Tnet>
  fprop_thread<Tnet>::~fprop_thread() {
  }

  template <typename Tnet>
  void fprop_thread<Tnet>::set_out_updated() {
    // lock data
    mutex_out.lock();
    // set flag
    out_updated = true;
    // unlock data
    mutex_out.unlock();
  }
 
  template <typename Tnet>
  bool fprop_thread<Tnet>::get_data(idx<ubyte> &frame2,
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
    // check frame is correctly allocated, if not, allocate.
    if (frame2.order() != uframe.order()) 
      frame2 = idx<ubyte>(uframe.get_idxdim());
    else if (frame2.get_idxdim() != uframe.get_idxdim())
      frame2.resize(uframe.get_idxdim());
    // copy frame
    idx_copy(uframe, frame2);    
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
  bool fprop_thread<Tnet>::set_data(idx<ubyte> &frame2, string &name,
				    uint id) {
    // lock data (non blocking)
    if (!mutex_in.trylock())
      return false;
    // check frame is correctly allocated, if not, allocate.
    if (frame2.order() != uframe.order())
      uframe = idx<ubyte>(frame2.get_idxdim());
    else if (frame2.get_idxdim() != uframe.get_idxdim())
      uframe.resize(frame2.get_idxdim());
    // do not load image, use passed frame instead
    bload_image = false;
    // copy frame
    idx_copy(frame2, uframe);
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
  bool fprop_thread<Tnet>::set_dump(string &frame_fname_,
				    string &dump_fname) {
    // lock data (non blocking)
    if (!mutex_in.trylock())
      return false;
    // set the dump name in detector
    if (!pdetect)
      eblerror("pdetect shoud be allocated");
    pdetect->set_outputs_dumping(dump_fname.c_str());
    // image file to load and process
    frame_name = frame_fname_;
    bload_image = true;
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
  bool fprop_thread<Tnet>::dumped() {
    // lock data
    mutex_out.lock();
    // only read data if it has been updated
    if (!out_updated) {
      // unlock data
      mutex_out.unlock();
      return false;
    }
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
  bool fprop_thread<Tnet>::available() {
    return bavailable;
  }
  
  template <typename Tnet>
  void fprop_thread<Tnet>::set_output_directory(string &out) {
    outdir = out;
  }

  template <typename Tnet>
  void fprop_thread<Tnet>::execute() { 
   try {
     // configuration
     bool       silent         = conf.exists_true("silent");
     bool	color	       = conf.exists_true("color");
     uint	norm_size      = conf.get_uint("normalization_size");
     Tnet	threshold      = (Tnet) conf.get_double("threshold");
     bool	display	       = false;
     bool	mindisplay     = conf.exists_true("minimal_display");
     bool       save_video     = conf.exists_true("save_video");
     bool	display_states = conf.exists_true("display_states");
     uint	display_sleep  = conf.get_uint("display_sleep");
     uint       wid	       = 0;	// window id
     uint       wid_states     = 0;	// window id
     display = conf.exists_true("display_threads");
//      if (!display && save_video) {
//        // we still want to output images but not show them
//        display = true;
// #ifdef __GUI__
//        set_gui_silent();
// #endif
//      }
     // load network and weights in a forward-only parameter
     parameter<FPROP_SFUNC(Tnet)> theparam;
     idx<ubyte> classes = load_matrix<ubyte>(conf.get_cstring("classes"));
     vector<string> sclasses = ubyteidx_to_stringvector(classes);
     answer_module<SFUNC2(Tnet)> *ans =
       create_answer<SFUNC2(Tnet)>(conf, classes.dim(0));
     uint noutputs = ans->get_nfeatures();
     module_1_1<FPROP_SFUNC(Tnet)> *net =
       create_network<FPROP_SFUNC(Tnet)>(theparam, conf, noutputs);
     // loading weights
     if (!conf.exists("weights")) { // manual weights
       merr << "warning: \"weights\" variable not defined, loading manually "
	    << "if manual_load defined" << endl;
       if (conf.exists_true("manual_load"))
	 manually_load_network(*((layers<FPROP_SFUNC(Tnet)>*)net), conf);
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
     detector<FPROP_SFUNC(Tnet)> detect(*net, sclasses, *ans, NULL, NULL,
					mout, merr);
     pdetect = &detect;

     // multi-scaling parameters
     double maxs = conf.exists("max_scale")?conf.get_double("max_scale") : 1.0;
     double mins = conf.exists("min_scale")?conf.get_double("min_scale") : 1.0;
     t_scaling scaling_type = SCALES_STEP;
     if (conf.exists("scaling_type"))
       scaling_type = (t_scaling) conf.get_uint("scaling_type");
     switch (scaling_type) {
     case ORIGINAL: detect.set_scaling_original(); break ;
     case SCALES_STEP:
       detect.set_resolutions(conf.get_double("scaling"), maxs, mins);
       break ;
     default:
       detect.set_scaling_type(scaling_type);
     }
     detect.set_bboxes_off(); // do not attempt to extract bounding boxes

     // optimize memory usage by using only 2 buffers for entire flow
     SBUF<Tnet> input(1, 1, 1), output(1, 1, 1);
     if (!conf.exists_false("mem_optimization"))
       detect.set_mem_optimization(input, output, 
				   (display && 
				    !conf.exists_true("minimal_display")));
     // zero padding
     float hzpad = 0, wzpad = 0;
     if (conf.exists("hzpad")) hzpad = conf.get_float("hzpad");
     if (conf.exists("wzpad")) wzpad = conf.get_float("wzpad");
     detect.set_zpads(hzpad, wzpad);
     if (conf.exists("input_min")) // limit inputs size
       detect.set_min_resolution(conf.get_uint("input_min"));
     if (conf.exists("input_max")) // limit inputs size
       detect.set_max_resolution(conf.get_uint("input_max"));
     detect.set_silent();
     if (conf.exists("net_min_height") && conf.exists("net_min_width"))
       detect.set_min_input(conf.get_int("net_min_height"),
			    conf.get_int("net_min_width"));
     if (conf.exists("smoothing"))
       detect.set_smoothing(conf.get_uint("smoothing"));

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
     module_1_1_gui	netgui;
     wid_states  = display_states ? new_window("network states"):0;
     night_mode();
     string title = "eblearn fprop: ";
     title += _name;
     if (display) {
       wid = new_window(title.c_str());
       mout << "displaying in window " << wid << endl;
       night_mode();
     }
     float		zoom = 1;
     if (conf.exists("display_zoom")) zoom = conf.get_float("display_zoom");
     detector_gui<FPROP_SFUNC(Tnet)> dgui;
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
       if (!silent) mout << "processing " << frame_name;
       // prepare image
       if (bload_image) { // load image directly
	 frame = load_image<Tnet>(frame_name);
       } else { // used passed image
	 // check frame is correctly allocated, if not, allocate.
	 if (frame.order() != uframe.order())
	   frame = idx<Tnet>(uframe.get_idxdim());
	 else if (frame.get_idxdim() != uframe.get_idxdim())
	   frame.resize(uframe.get_idxdim());
	 // copy frame
	 idx_copy(uframe, frame);
       }
       if (!silent) mout << " (min: " << idx_min(frame)
			 << ", max: " << idx_max(frame) << ")" << endl;

       // run detector
       if (!display) { // fprop without display
	 detect.fprop(frame, threshold, frame_name.c_str());
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
	   dgui.display(detect, frame, threshold, frame_name.c_str(),
			0, 0, zoom, display_min, display_max,
			wid, _name.c_str());
	 } else {
	   dgui.display_inputs_outputs(detect, frame, threshold,
				       frame_name.c_str(), 0, 0, zoom,
				       display_min, display_max, wid,
				       _name.c_str(),
				       display_in_min, display_in_max);
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
       ms = tpass.elapsed_milliseconds();
       if (!silent) {
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
     }
     mout << "fprop finished. Execution time: " << toverall.elapsed()
	  << endl;
     // free variables
     if (net) delete net;
   } catch(string &err) { eblerror(err.c_str()); }
  }

} // end namespace ebl

#endif /* FPROP_THREAD_HPP_ */
