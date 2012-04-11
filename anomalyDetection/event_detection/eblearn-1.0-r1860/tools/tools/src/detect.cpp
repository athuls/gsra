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

#include <map>
#include <string>
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <sstream>
#include <iomanip>
#include <time.h>
#include "libeblearn.h"
#include "libeblearntools.h"

#ifndef __WINDOWS__
#include <fenv.h>
#endif

#ifdef __GUI__
#include "libeblearngui.h"
#endif

using namespace std;
using namespace ebl; // all eblearn objects are under the ebl namespace

#ifdef __DEBUGMEM__
  INIT_DEBUGMEM()
#endif

typedef float t_net; // network precision

#ifdef __GUI__
MAIN_QTHREAD(int, argc, char **, argv) { // macro to enable multithreaded gui
#else
  int main(int argc, char **argv) { // regular main without gui
#endif
    try {
      // check input parameters
      if ((argc != 2) && (argc != 3) ) {
	cerr << "warning: wrong number of parameters." << endl;
	cerr << "usage: detect <config file> [directory or file]" << endl;
	//	return -1;
      }
#ifdef __LINUX__
      feenableexcept(FE_DIVBYZERO | FE_INVALID); // enable float exceptions
#endif
      // load configuration
      configuration	conf(argv[1], true, true, false);
      if (!conf.exists("root2")) {
	string dir;
	dir << dirname(argv[1]) << "/";
	cout << "Looking for trained files in: " << dir << endl;
	conf.set("root2", dir.c_str());
	conf.set("current_dir", dir.c_str());
      }
      conf.resolve(); // manual call to resolving variable
      if (conf.exists_true("show_conf")) conf.pretty();
      // output synchronization
      bool sync = conf.exists_true("sync_outputs");
      mutex out_mutex;
      mutex_ostream mutout(std::cout, &out_mutex, "Thread M");
      mutex_ostream muterr(std::cerr, &out_mutex, "Thread M");
      ostream &mout = sync ? mutout : cout;
      ostream &merr = sync ? muterr : cerr;
      // output dir
      string outdir = "detections"; 
      if (conf.exists_true("nms"))
	outdir << "_" << tstamp();
      outdir << "/";
      mkdir_full(outdir);
      mout << "Saving outputs to " << outdir << endl;
      // save conf to output dir
      string cname = outdir;
      cname << filename(argv[1]);
      if (conf.write(cname.c_str()))
	mout << "Wrote configuration to " << cname << endl;
      // load classes of network
      idx<ubyte> classes(1,1);
      vector<string> sclasses;
      try { // try loading classes names but do not stop upon failure
	load_matrix<ubyte>(classes, conf.get_cstring("classes"));
      } catch(string &err) {
	merr << "warning: " << err;
	merr << endl;
      }
      sclasses = ubyteidx_to_stringvector(classes);
      t_bbox_saving bbsaving = bbox_none;
      if (conf.exists("bbox_saving"))
	bbsaving = (t_bbox_saving) conf.get_int("bbox_saving");
      bboxes boxes(bbsaving, &outdir, mout, merr);
      
      bool              silent        = conf.exists_true("silent");
      uint              ipp_cores     = 1;
      if (conf.exists("ipp_cores")) ipp_cores = conf.get_uint("ipp_cores");
      ipp_init(ipp_cores); // limit IPP (if available) to 1 core
      bool		save_video    = conf.exists_true("save_video");      
      bool              save_detections = conf.exists_true("save_detections");
      int		height        = -1;
      int		width         = -1;
      if (conf.exists("input_height")) height = conf.get_int("input_height");
      if (conf.exists("input_width")) width = conf.get_int("input_width");
      bool              input_random  = conf.exists_true("input_random");
      uint              npasses       = 1;
      char              next_on_key   = 0;
      if (conf.exists("next_on_key")) {
	next_on_key = conf.get_char("next_on_key");
	mout << "Press " << next_on_key << " to process next frame." << endl;
      }
      uint              skip_frames   = conf.exists("skip_frames") ? 
	conf.get_uint("skip_frames") : 0;
      if (conf.exists("input_npasses"))
	npasses = conf.get_uint("input_npasses");
      string viddir;
      if (save_video) {
	viddir << outdir << "video/";
	mkdir_full(viddir);
      }
      bool precomputed_boxes = conf.exists("bbox_file");
      uint 	save_bbox_period 	    = 1;
      if (conf.exists("save_bbox_period")) 
	save_bbox_period = std::max((uint)1, conf.get_uint("save_bbox_period"));
    
      string		cam_type;
#ifdef __LINUX__ // default camera for linux if not defined
      cam_type = "v4l2";
#endif
      if (conf.exists("camera"))
	cam_type = conf.get_string("camera");
      
      // allocate threads
      uint nthreads = 1;
      bool updated = false;
      idx<ubyte> detframe; // frame returned by detection thread
      if (conf.exists("nthreads"))
	nthreads = (std::max)((uint) 1, conf.get_uint("nthreads"));
      list<detection_thread<t_net>*>  threads;
      list<detection_thread<t_net>*>::iterator ithreads;
      idx<uint> total_saved(nthreads);
      idx_clear(total_saved);
      mout << "Initializing " << nthreads << " detection threads." << endl;
      for (uint i = 0; i < nthreads; ++i) {
	ostringstream tname;
	tname << "Thread " << i;
	detection_thread<t_net> *dt =
	  new detection_thread<t_net>(conf, &out_mutex, tname.str().c_str(),
				      NULL, sync);
	threads.push_back(dt);
	dt->start();
	dt->set_output_directory(outdir);
      }
      // image search can be configured with a search pattern
      const char *fpattern = IMAGE_PATTERN_MAT;
      if (conf.exists("file_pattern"))
	fpattern = conf.get_cstring("file_pattern");

      // initialize camera (opencv, directory, shmem or video)
      idx<ubyte> frame(1,1,1);
      camera<ubyte> *cam = NULL, *cam2 = NULL;
      if (!strcmp(cam_type.c_str(), "directory")) {
	string dir;
	if (argc >= 3) // read input dir from command line
	  dir = argv[2];
	else if (conf.exists("input_dir"))
	  dir = conf.get_string("input_dir");
	// given list
	list<string> files;
	if (conf.exists("input_list")) {
	  files = string_to_stringlist(conf.get_string("input_list"));	 
	  cam = new camera_directory<ubyte>(dir.c_str(), height, width,
					    input_random, npasses, mout, merr,
					    fpattern, &files);
	} else // given directory only
	  cam = new camera_directory<ubyte>(dir.c_str(), height, width,
					    input_random, npasses, mout, merr,
					    fpattern, &files);
      } else if (!strcmp(cam_type.c_str(), "opencv"))
	cam = new camera_opencv<ubyte>(-1, height, width);
#ifdef __LINUX__
      else if (!strcmp(cam_type.c_str(), "v4l2"))
	cam = new camera_v4l2<ubyte>(conf.get_cstring("device"),
				     height, width,
				     conf.exists_true("camera_grayscale"));
#endif
#ifdef __KINECT__
      else if (!strcmp(cam_type.c_str(), "kinect"))
	cam = new camera_kinect<ubyte>(height, width);
#endif
      else if (!strcmp(cam_type.c_str(), "shmem"))
	cam = new camera_shmem<ubyte>("shared-mem", height, width);
      else if (!strcmp(cam_type.c_str(), "video")) {
	if (argc >= 3)
	  cam = new camera_video<ubyte>
	    (argv[2], height, width, conf.get_uint("input_video_sstep"),
	     conf.get_uint("input_video_max_duration"));
	else eblerror("expected 2nd argument");
      } else eblerror("unknown camera type, set \"camera\" in your .conf");
      // a camera directory may be used first, then switching to regular cam
      if (conf.exists_true("precamera"))
	cam2 = new camera_directory<ubyte>(conf.get_cstring("precamdir"),
					   height, width, input_random,
					   npasses, mout, merr, fpattern);
      if (conf.exists_true("camera_grayscale"))
	cam->set_grayscale();
	
      // answer variables & initializations
      vector<bbox*> bb;
    
      // gui
#ifdef __GUI__
      bool display	     = conf.exists_bool("display");
      bool show_parts        = conf.exists_true("show_parts");
      // mindisplay     = conf.exists_bool("minimal_display");
      uint display_sleep  = 0;
      if (conf.exists("display_sleep"))
	display_sleep = conf.get_uint("display_sleep");
      // display_states = conf.exists_bool("display_states");
      uint qstep1 = 0, qheight1 = 0, qwidth1 = 0,
	qheight2 = 0, qwidth2 = 0, qstep2 = 0;
      if (conf.exists_bool("queue1")) {
	qstep1 = conf.get_uint("qstep1");
	qheight1 = conf.get_uint("qheight1");
	qwidth1 = conf.get_uint("qwidth1"); }
      if (conf.exists_bool("queue2")) {
	qstep2 = conf.get_uint("qstep2");
	qheight2 = conf.get_uint("qheight2");
	qwidth2 = conf.get_uint("qwidth2"); }
      // wid_states  = display_states ? new_window("network states"):0;
      // night_mode();
      uint wid  = display ? new_window("eblearn object recognition") : 0;
      night_mode();
      float display_transp = 0.0;
      if (conf.exists("display_bb_transparency"))
	display_transp = conf.get_float("display_bb_transparency");
      detector_gui<t_net> dgui(conf.exists_true("show_extracted"));
#endif  
      // timing variables
      timer tpass, toverall, tstop;
      uint cnt = 0;
      if (!silent)
	mout << "i=" << cnt << endl;
      bool stop = false, finished = false;
  
      // loop
      toverall.start();
      while (!finished) {
	// check for results and send new image for each thread
	uint i = 0;
	finished = true;
	for (ithreads = threads.begin(); 
	     ithreads != threads.end(); ++ithreads, ++i) {
	  // do nothing if thread is finished already
	  if ((*ithreads)->finished())
	    continue ;
	  finished = false; // a thread is not finished
	  string processed_fname;
	  uint processed_id = 0;
	  // retrieve new data if present
	  updated = (*ithreads)->get_data(bb, detframe, 
					  *(total_saved.idx_ptr() + i),
					  processed_fname, processed_id);
	  // save bounding boxes
	  if (updated) {
	    updated = false;
	    idxdim d(detframe);
	    if (bbsaving != bbox_none) {
	      mout << "Adding " << bb.size() << " boxes into new group: "
		   << processed_fname << " with id " << processed_id << endl;
	      boxes.new_group(d, &processed_fname, processed_id);
	      boxes.add(bb, d, &processed_fname, processed_id);
	      if (cnt % save_bbox_period == 0)
		boxes.save();
	    }
	    cnt++;
	    // display processed frame
#ifdef __GUI__
	    if (display) {
	      select_window(wid);
	      disable_window_updates();
	      clear_resize_window();
	      set_window_title(processed_fname.c_str());
	      dgui.display_minimal(detframe, bb, 
				   ((*ithreads)->pdetect ?
				    (*ithreads)->pdetect->get_labels()
				    : sclasses),
				   0, 0, 1, 0, 255, wid, show_parts,
				   display_transp);
	      enable_window_updates();
	      if (save_video && display) {
		string fname;
		fname << viddir << processed_fname;
		save_window(fname.c_str());
		if (!silent) mout << "saved " << fname << endl;
	      }
	    }
	    // sleep display
	    if (display_sleep > 0) {
	      mout << "sleeping for " << display_sleep << "ms." << endl;
	      millisleep(display_sleep);
	    }
#endif
	    // output info
	    uint k = cnt, tot = cam->size() - cnt; // progress variables
	    if (!silent) {
	      if (save_detections) {
		mout << "total_saved=" << idx_sum(total_saved);
		if (conf.exists("save_max"))
		  mout << " / " << tot;
		mout << endl;
	      }
	      mout << "remaining=" << (cam->size() - cnt)
		   << " elapsed=" << toverall.elapsed();
	      if (cam->size() > 0)
		mout << " ETA=" << toverall.eta(cnt, cam->size());
	      if (conf.exists("save_max") && save_detections) {
		k = idx_sum(total_saved);
		tot = conf.get_uint("save_max");
		mout << " save_max_ETA="
		     << toverall.eta(k, tot);
	      }
	      mout << endl;
	      mout << "i=" << cnt << " processing: " << tpass.elapsed_ms()
		   << " fps: " << cam->fps() << endl;
	    }
	    // save progress
	    job::write_progress(k, tot);
	  }
	  // check if ready
	  if ((*ithreads)->available()) {
	    if (stop)
	      (*ithreads)->ask_stop(); // stop but let thread finish
	    else {
	      // grab a new frame if available
	      if (cam->empty()) {
		stop = true;
		tstop.start(); // start countdown timer
		(*ithreads)->ask_stop(); // ask this thread to stop
		millisleep(50);
	      } else {
#ifdef __GUI__
		int key = gui.pop_key_pressed();
		// if thread has already received data, wait for next key
		if ((*ithreads)->fed() && next_on_key) {
		  if ((int) next_on_key != key && (int) next_on_key != key + 32)
		    continue ; // pause until key is pressed
		  else
		    mout << "Key pressed (" << key 
			 << ") allowing next frame to process." << endl;
		}
#endif
		// if the pre-camera is defined use it until empty
		if (cam2 && !cam2->empty())
		  frame = cam2->grab();
		else { // empty pre-camera, use regular camera
		  if (skip_frames > 0)
		    cam->skip(skip_frames); // skip frames if skip_frames > 0
		  if (cam->empty()) continue ;
		  if (precomputed_boxes && !save_video)
		    cam->next(); // move to next frame but without grabbing
		  else // actually grab the frame
		    frame = cam->grab();
		}
		// send new frame to this thread
		string frame_name = cam->frame_name();
		while (!(*ithreads)->set_data(frame, frame_name, 
					      cam->frame_id()))
		  millisleep(5);
		// we just sent a new frame
		tpass.restart();
	      }
	    }
	  }
	}
	if (conf.exists("save_max") && !stop &&
	    idx_sum(total_saved) > conf.get_uint("save_max")) {
	  mout << "Reached max number of detections, exiting." << endl;
	  stop = true; // limit number of detection saves
	  tstop.start(); // start countdown timer
	}
	// sleep a bit between each iteration
	millisleep(5);
	// check if stop countdown reached 0
	if (stop && tstop.elapsed_minutes() >= 20) {
	  cerr << "threads did not all return 20 min after request, stopping"
	       << endl;
	  break ; // program too long to stop, force exit
	}
      }
      // saving boxes
      if (bbsaving != bbox_none)
	boxes.save();
      mout << "Execution time: " << toverall.elapsed() << endl;
      if (save_video)
	cam->stop_recording(conf.exists_bool("use_original_fps") ?
			    cam->fps() : conf.get_uint("save_video_fps"),
			    outdir.c_str());
      // free variables
      if (cam) delete cam;
      for (ithreads = threads.begin(); ithreads != threads.end(); ++ithreads) {
	if (!(*ithreads)->finished())
	  (*ithreads)->stop(); // stop thread without waiting
	delete *ithreads;
      }
#ifdef __GUI__
      if (!conf.exists_true("no_gui_quit")) {
	mout << "Closing windows..." << endl;
	quit_gui(); // close all windows
	mout << "Windows closed." << endl;
      }
#endif
      job::write_finished(); // declare job finished
      mout << "Detection finished." << endl;
      // evaluation of bbox
      if (conf.exists_true("evaluate") && conf.exists("evaluate_cmd")) {
	string cmd;
	cmd << "cd " << outdir << " && " << conf.get_string("evaluate_cmd");
	int res = std::system(cmd.c_str());
	if (res != 0)
	  cerr << "bbox evaluation failed with command " << cmd << endl;
      }
    } eblcatcherror();
  return 0;
}
