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

#ifndef CAMERA_V4L2_H_
#define CAMERA_V4L2_H_

#include "camera.h"

using namespace std;

namespace ebl {

  //! The camera_v4l2 class interfaces with the v4l2 camera and images.
  //! It allows to grab images from v4l2 in the idx format, and also
  //! to save gui outputs into video files.
  template <typename Tdata> class camera_v4l2 : public camera<Tdata> {
  public:

    ////////////////////////////////////////////////////////////////
    // constructors/allocation

    //! Initialize v4l2 camera using device to choose which camera_v4l2 to use.
    //! height and width are optional parameters that resize the input image
    //! to those dimensions if given (different than -1). One may want to
    //! decrease the input resolution first to speed up operations, for example
    //! when computing multiple resolutions.
    //! \param device The device string, e.g. "/dev/video".
    //! \param height Resize input frame to this height if different than -1.
    //! \param width Resize input frame to this width if different than -1.
    //! \param grayscale If true, output grayscale directly.
    camera_v4l2(const char *device, int height = -1, int width = -1,
		bool grayscale = false);

    //! Destructor.
    virtual ~camera_v4l2();

    ////////////////////////////////////////////////////////////////
    // frame grabbing

    //! Return a new frame.
    virtual idx<Tdata> grab();

  private:

#ifdef __LINUX__
    void print_controls();
    int get_control(int id);
    void set_integer_control(int id, int val);
    void set_boolean_control(int id, bool val);
    void start();
#endif    

    // members ////////////////////////////////////////////////////////
  protected:
    using camera<Tdata>::frame;		//!< frame buffer 
    using camera<Tdata>::frame_id_;	//!< frame counter
    using camera<Tdata>::grabbed;	//!< frame buffer grabbed yet or not
    using camera<Tdata>::height;	//!< height
    using camera<Tdata>::width;	        //!< width
    using camera<Tdata>::bresize;       //!< resize or not during postproc.
    using camera<Tdata>::grayscale;     //!< Output grayscale or not.
    bool started;
    int nbuffers;
    void* *buffers;
    int *sizes;
    int fd;
  };

} // end namespace ebl

#include "camera_v4l2.hpp"


#endif /* CAMERA_V4L2_H_ */
