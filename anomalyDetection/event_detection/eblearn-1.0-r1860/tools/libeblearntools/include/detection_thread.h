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

#ifndef DETECTION_THREAD_H_
#define DETECTION_THREAD_H_

#include <stdio.h>
#include <stdlib.h>

#include "defines_tools.h"
#include "thread.h"
#include "netconf.h"
#include "configuration.h"

using namespace std;

namespace ebl {

// switch between forward only buffers or also backward
#define SFUNC fs
#define SBUF fstate_idx
#define SFUNC2(T) T,T,T,SBUF<T>
#define SFUNC3(T) T,SBUF<T>,mstate<SBUF<T> >
// backward
// #define SFUNC bbs
// #define SBUF bbstate_idx
  
  ////////////////////////////////////////////////////////////////
  // A detection thread class

  template <typename Tnet>
  class detection_thread : public thread {
  public:
    //! \param om A mutex used to synchronize threads outputs/
    //!   To synchronize all threads, give the same mutex to each of them.
    //! \param sync If true, synchronize outputs between threads, using
    //!    om, otherwise use regular unsynced outputs.
    //! \param tc The channels type of the input image, e.g. brightness Y,
    //!    or color RGB.
    detection_thread(configuration &conf, mutex *om = NULL,
		     const char *name = "",
		     const char *arg2 = NULL, bool sync = true,
		     t_chans tc = CHANS_RGB);
    ~detection_thread();
    
    //! Execute the detection thread.
    virtual void execute();
    //! Return true if new data was copied to the thread, false otherwise,
    //! if we could not obtain the mutex lock.
    virtual bool set_data(idx<ubyte> &frame, string &frame_name, uint frame_id);
    //! Return true if new data was copied to the thread, false otherwise,
    //! if we could not obtain the mutex lock.
    virtual bool set_data(string &frame_fullname, string &frame_name, 
			  uint frame_id);
    //! Return true if new data was copied from the thread, false otherwise.
    //! We get the frame back even though it was set via set_data,
    //! because we do not know which frame was actually used.
    //! (could use some kind of id, and remember frames to avoid copy).
    virtual bool get_data(vector<bbox*> &bboxes, idx<ubyte> &frame,
			  uint &total_saved, string &frame_name,
			  uint &frame_id);
    //! Return true if the thread is available to process a new frame, false
    //! otherwise.
    virtual bool available();
    //! Returns true if at least 1 input has been fed for detection.
    virtual bool fed();
    //! Set the directory where to write outputs.
    virtual void set_output_directory(string &out);
    
  private:
    //! Copy passed bounding boxes into bboxes class member
    //! (allocating new 'bbox' objects).
    void copy_bboxes(vector<bbox> &bb);
    //! Copy passed bounding boxes into bboxes class member
    //! (allocating new 'bbox' objects).
    void copy_bboxes(vector<bbox*> &bb);
    //! Turn 'out_updated' flag on, so that other threads know we just outputed
    //! new data.
    void set_out_updated();

    ////////////////////////////////////////////////////////////////
    // private members
  private:
    configuration		 conf;
    const char			*arg2;
    idx<ubyte>			 uframe;
    idx<Tnet>			 frame;
    mutex		         mutex_in;	// mutex for thread input
    mutex 		         mutex_out;	// mutex for thread output
    vector<bbox*>		 bbs;
    vector<bbox*>::iterator	 ibox;
    bool			 in_updated;	// thread input updated
    bool			 out_updated;	// thread output updated
    bool                         bavailable;    // thread is available
    bool                         bfed;          //!< Thread has been fed data.
    string                       frame_name;    // name of current frame
    uint                         frame_id;      //! Unique ID for frame.
    string                       outdir;        // output directory
    uint                         total_saved;
    using thread::mout; //! synchronized cout
    using thread::merr; //! synchronized cerr
    t_chans                      color_space;

  public:
    detector<SFUNC(Tnet)>       *pdetect;
  };

} // end namespace ebl

#include "detection_thread.hpp"

#endif /* DETECTION_THREAD_H_ */

