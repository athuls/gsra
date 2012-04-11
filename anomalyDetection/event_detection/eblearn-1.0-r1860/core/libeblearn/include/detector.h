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

#ifndef DETECTOR_H_
#define DETECTOR_H_

#include "libidx.h"
#include "ebl_states.h"
#include "ebl_arch.h"
#include "ebl_preprocessing.h"
#include "bbox.h"

using namespace std;

namespace ebl {

  ////////////////////////////////////////////////////////////////
  // detector

  //! different types of resolutions
  //! MANUAL: resolutions are specified manually by height and width
  //! SCALES: a series of scaling factors, 1 being the network's size
  //! NSCALES: n scales are computed by evenely interpolating between network's
  //!          size and maximum resolution.
  //! SCALES_STEP: scales range from 1 to maximum resolution, with a step size
  //! ORIGINAL: only use image's original resolution.
  //! NETWORK: resize all inputs to the minimal network's input size.
  //! SCALES_STEP_UP: scale step from min up to max scale.
  enum t_scaling { MANUAL = 0, SCALES = 1, NSCALES = 2, SCALES_STEP = 3,
		   ORIGINAL = 4, NETWORK = 5, SCALES_STEP_UP = 6 };
  
  enum t_pruning { pruning_none = 0, pruning_overlap = 1 };
  
  template <typename T, class Tstate = fstate_idx<T> >
    class detector {
  public:    
  
    ////////////////////////////////////////////////////////////////
    // constructors
    
    //! Constructor. Default resolutions are 1, 2 and 4 times the network's 
    //! size. Resolutions can be set using set_resolutions().
    //! Background class name default "bg" will be searched in the list
    //! of class names. To specify another background class, pass a non NULL
    //! background parameter.
    //! \param labels A vector of label strings.
    //! \param pp An optional resizing (and preprocessing) module,
    //!   e.g. resizepp_module. If null, use resize_module by default.
    //! \param background The name of the background class. Default is "bg".
    //!          If given, positive answers for this class are ignored.
    //! \param adapt_scales If true, adapt each scale so that they are valid
    //!          input sizes for 'thenet' network. Otherwise, the network
    //!          must crop inputs itself (see 'crop' attribute of modules).
    detector(module_1_1<T,Tstate> &thenet, vector<string> &labels,
	     answer_module<T,T,T,Tstate> &answer,
	     resizepp_module<T,Tstate> *resize = NULL,
	     const char *background = NULL,
	     std::ostream &out = std::cout, std::ostream &err = std::cerr,
	     bool adapt_scales = false);

    //! Destructor.
    virtual ~detector();

    ////////////////////////////////////////////////////////////////
    // configuration

    //! Set the multi-scale to 1 scale only: the image's original scale.
    void set_scaling_original();

    //! Set the scaling type.
    void set_scaling_type(t_scaling type);

    //! Set all scales manually.
    void set_resolutions(const vector<idxdim> &scales);

    //! Set scales a factors of the input sizes.
    void set_resolutions(const vector<double> &factors);

    //! Use nresolutions resolutions between the maximum resolution and the
    //! minimum resolution.
    //! \param nresolutions The number of resolutions to use.
    void set_resolutions(int resolutions);

    //! Specify resolutions by hand in an nx2 idx (heightxwidth),
    //! e.g. 240x320, 120x160.
    //! \param resolutions A uint idx containing resolutions (of size nx2)
    void set_resolutions(idx<uint> &resolutions);

    //! Add zero padding of (hzpad * the network's minimum input height)
    //! on each vertical sides and (wzpad * min width) on horizontal sides.
    void set_zpads(float hzpad, float wzpad);
  
    //! Specify resolutions by the factor step, starting from factor 1
    //! (network's size), adding factor_steps until reaching the original
    //! resolution.
    //! \param max_scale The maximum scale factor of the original resolution,
    //!    1.0 by default, i.e. the original resolution.
    //! \param min_scale The minimum scale factor of the smallest network size
    //!    1.0 by default, meaning the minimum network input size.
    void set_resolutions(double scales_steps, double max_scale = 1.0,
			 double min_scale = 1.0);

    //! Return the id of the class 'name' or -1 if not found.
    int get_class_id(const string &name);

    //! set background class (which will be ignored).
    void set_bgclass(const char *bg = NULL);

    //! Set the mask class, which is ignored by bounding box detection.
    //! Instead, call get_mask() to retrieve a mask map of values above
    //! a given threshold. This is useful for continuous classes rather
    //! than discrete classes.
    bool set_mask_class(const char *mask);

    //! set detector to silent: do not print results on std output
    void set_silent();

    //! Enable saving of each (preprocessed) window inducing a positive
    //! detection into directory. All detections except for the background
    //! class are dumped into a directory corresponding to the class' name.
    //! This returns the directory string used.
    string& set_save(const string directory);

    //! Set the minimum size of each side of an input to use as input
    //! to the network. I.e. a input to the network will be at least
    //! min_size * min_size big.
    //! \param min_size The minimum width or height in pixels.
    void set_min_resolution(uint min_size);

    //! Set the maximum size of each side of an input to use as input
    //! to the network. I.e. a input to the network will be at most
    //! max_size * max_size big.
    //! \param max_size The maximum width or height in pixels.
    void set_max_resolution(uint max_size);

    //! Enable pruning of type 'type'. Refer to t_pruning declaration for
    //! different types. Default type is 1, regular pruning.
    void set_pruning(t_pruning type = pruning_overlap,
		     float min_hcenter_dist = 0.0, float min_wcenter_dist = 0.0,
		     float max_overlap = 1.0,
		     bool share_parts = false, T threshold2 = 0.0,
		     float max_center_dist = 0.0,
		     float max_center_dist2 = 0.0,
		     float max_wcenter_dist = 0.0,
		     float max_wcenter_dist2 = 0.0,
		     float min_wcenter_dist2 = 0.0,
		     float max_overlap2 = 0.0, bool mean_bb = false,
		     float same_scale_mhd = 0.0,
		     float same_scale_mwd = 0.0,
		     float min_scale_pred = 0.0, float max_scale_pred = 0.0);

    //! Enable or disable clustering nms.
    void set_cluster_nms(bool set);
    //! Enable or disable scaler mode, i.e. using scale prediction for boxes.
    void set_scaler_mode(bool set);
    //! Set output smoothing type. 0: none, 1: 3x3 kernel.
    void set_smoothing(uint type);

    //! Set factors to be applied on the height and width of output bounding
    //! boxes.
    void set_bbox_factors(float hfactor, float wfactor, float woverh,
			  float hfactor2, float wfactor2);

    //! Enable memory optimization by using only 2 buffers (in and out)
    //! for entire flow. Those same buffers must have been passed to the
    //! network's constructor.
    //! \param keep_inputs If false, re-use input buffers for optimization.
    //!   If true, some operations may be unavailable, such as saving
    //!   the pre-processed detected windows.
    void set_mem_optimization(Tstate &in, Tstate &out, 
			      bool keep_inputs = false);

    //! Limit the size of the image to be processed based on the maximum
    //! image_height / object_height. This ratio can be seen as the
    //! maximum number of objects that can fit next to each other
    //! in one image along the height axis.
    void set_max_object_hratio(double hratio);

    //! Set by hand the minimum network input.
    void set_min_input(intg h, intg w);

    //! Enables dumping of all outputs using the base name 'name', to which
    //! is appending the idx's size and '.mat'. Each resolution
    //! will be dump as a separate matrix file.
    //! Dumping will be called at the end of each fprop call for each
    //! resolution.
    void set_outputs_dumping(const char *name);

    //! Turn off the extraction of bounding boxes.
    void set_bboxes_off();

    //! Returns the vector of label strings.
    vector<string>& get_labels();
  
    ////////////////////////////////////////////////////////////////
    // execution
    
    //! fprop input image throught network.
    //! if image's and network's type differ, cast image into network's type
    //! through an idx_copy (avoid for better performance).
    //! @param frame_name Optional name for the frame being processed, used
    //!          in the output files to be saved.
    template <class Tin>
      vector<bbox*>& fprop(idx<Tin> &img, T threshold,
			   const char *frame_name = NULL);

    //! Non-maximum suppression, fills pruned given raw and excludes bbox
    //! lower than threshold.
    void nms(vector<bbox*> &raw, vector<bbox*> &pruned, float threshold);

    //! Return a reference to a vector of windows in the original image that
    //! yielded a detection.
    vector<idx<T> >& get_originals();

    //! Return the preprocessed input corresponding to bounding box 'b'
    //! or throws an exception if out of bounds.
    idx<T> get_preprocessed(const bbox &b);

    //! Return a reference to a vector of windows in the preprocessed/scaled
    //! image that yielded a detection.
    vector<idx<T> >& get_preprocessed();

    //! Return a mask of output maps with the same size as the input.
    //! The mask is a max of all output resolution.
    idx<T> get_mask(string &classname);

    //! Returns the number of bboxes saved so far.
    uint get_total_saved();

    //! Limit the number of regions saved per frame.
    void set_save_max_per_frame(uint max);

    //! Print bounding boxes on standard output.
    void pretty_bboxes(vector<bbox*> &bboxes, std::ostream &out = std::cout);

    //! Print short description of bounding boxes on standard output,
    //! with an optional prefix string in front of each line.
    void pretty_bboxes_short(vector<bbox*> &bboxes,
 			     std::ostream &out = std::cout);

  protected:
  
    //! initialize dimensions and multi-resolution buffers.
    void init(idxdim &dinput);

    ////////////////////////////////////////////////////////////////////////////
    // scales methods
  
    //! Compute all scales based on minimum, maximum and input dimensions,
    //! and scaling type.
    //! \param netdim The network's minimal input size. 
    //! \param mindim The minimum scale size.
    //! \param maxdim The maximum scale size.
    //! \param indim The original input dimensions.
    void compute_scales(vector<idxdim> &scales, idxdim &netdim, idxdim &mindim,
			idxdim &maxdim, idxdim &indim, t_scaling type,
			uint nscales, double scales_step);
  
    //! Compute 'nscales' scales between 'mindim' and 'maxdim' resolutions
    //! and push them into 'scales' vector.
    void compute_resolutions(vector<idxdim> &scales,
			     idxdim &mindim, idxdim &maxdim, uint nscales);
 
    //! Compute each scale as a factor of 'indim' for each element of
    //! 'scale_factors' and put them into 'scales' vector.
    void compute_resolutions(vector<idxdim> &scales,
			     idxdim &indim, vector<double> &scale_factors);

    //! Compute each scale with a step of 'scales_step' starting from 'maxdim'
    //! down to 'mindim'.
    //! \param mindim The minimum scale size.
    //! \param maxdim The maximum scale size.
    void compute_resolutions(vector<idxdim> &scales, idxdim &mindim,
			     idxdim &maxdim, double scales_step);

    //! Compute each scale with a step of 'scales_step' starting from 'mindim'
    //! up to 'maxdim'.
    //! \param mindim The minimum scale size.
    //! \param maxdim The maximum scale size.
    void compute_resolutions_up(vector<idxdim> &scales, idxdim &indim,
				idxdim &mindim, idxdim &maxdim,
				double scales_step);

    //! checks that resolutions match the network size, if not adjust them.
    //! this method assumes nresolutions and resolutions members have already
    //! been initialized.
    void validate_resolutions();

    ////////////////////////////////////////////////////////////////
    // pruning
  
    //! Prune bounding boxes between scales into prune_bboxes.
    //! The pruning criterion is the overlap between two bounding boxes.
    //! If 2 bb's overlap ratio is more than 'max_overlap' (see
    //! set_bbox_overlap()), keep only the one with highest confidence.
    //! \param same_class_only If true, only classes from the same class
    //!   can prune each other, otherwise any bb can cancel any other bb.
    void prune_overlap(vector<bbox*> &raw_bboxes, vector<bbox*> &prune_bboxes,
		       float max_match,
		       bool same_class_only = false,
		       float min_hcenter_dist = 0.0,
		       float min_wcenter_dist = 0.0,
		       float threshold = 0.0,
		       float same_scale_mhd = 0.0, float same_scale_mwd = 0.0);

    void prune_vote(vector<bbox*> &raw_bboxes, vector<bbox*> &prune_bboxes,
		    float max_match,
		    bool same_class_only = false,
		    float min_hcenter_dist = 0.0,
		    float min_wcenter_dist = 0.0, int classid = 0);

    ////////////////////////////////////////////////////////////////////////////
    // bboxes operations
  
    //! Smooth outputs.
    void smooth_outputs();

    //! Extract bounding boxes with higher confidence than 'threshold'
    //! from internal 'outputs' into 'bboxes'.
    void extract_bboxes(T threshold, vector<bbox*> &bboxes);

    void dfs(vector<bbox*> &bboxes, vector<bool> &explored,
	     uint i, float match, float max_center_dist, vector<bbox*> &comp);
  void cluster_bboxes(float match, float max_center_dist, float max_wcenter_dist, vector<bbox*> &bboxes, float threshold = 0);

    //! save all bounding boxes of original (in original resolution) and
    //! preprocessed (resized and filtered) input into directory dir.
    void save_bboxes(const vector<bbox*> &bboxes, const string &dir,
		     const char *frame_name = NULL);

    //! Sort bboxes by confidence (most confident first);
    void sort_bboxes(vector<bbox*> &bboxes);

    //! Normalize width of boxes w.r.t. to their height with ratio 'woverh'.
    void normalize_bboxes(vector<bbox*> &bboxes, float woverh);

    //! Add a name to the vector of class names.
    //! This can be useful when generating
    //! intermediate classes from existing classes.
    void add_class(const char *name);

    ////////////////////////////////////////////////////////////////////////////
    // processing methods
  
    //! Prepare image and resolutions. This should be called before
    //! preprocess_resolution().
    //! This mostly involves casting image into network's type and computing
    //! each scale's dimensions (no resizing) based on image's size.
    template <class Tin>
      void prepare(idx<Tin> &img);
    
    //! Do preprocessing (resizing and channel/edge processing) for a particular
    //! resolution. This will set 'input' and 'output' buffers, that can then
    //! be used to fprop the network. This uses the 'image' member prepared
    //! by prepare() and should therefore be called after prepare().
    //! \param res The resolution to be preprocessed.
    void prepare_scale(uint i);
    
    //! do a fprop on thenet with multiple rescaled inputs
    void multi_res_fprop();       

    ////////////////////////////////////////////////////////////////
    // members
  protected:
    module_1_1<T,Tstate>	&thenet; //!< The network.
    resizepp_module<T,Tstate>   *resizepp; //!< Resize module for multi-scaling.
    bool                 resizepp_delete; //!< We are responsible for deleting.
    idx<T>		 image;
    double		 contrast;
    double		 brightness;
    idx<float>		 sizes;
    fstate_idx<T>        finput; //! A forward buffer containing input image.
    Tstate              *input;        //!< input buffer
    Tstate              *output;       //!< output buffer
    Tstate              *tmp;           //!< tmp.
    Tstate              *minput; //!< input buffer, used with mem optim.
    vector<mstate<Tstate>*> ppinputs; //!< Preprocessed inputs of all scales.
    vector<Tstate*>	 outputs; //!< Output buffers of all scales.
    vector<string>   	 labels; //!< String label of each class.
  protected:
    // dimensions //////////////////////////////////////////////////////////////
    idxdim               indim; //!< Input dimensions.
    idxdim		 netdim; //!< network's input dimensions
    // bboxes //////////////////////////////////////////////////////////////////
    vector<rect<int> >	 original_bboxes; //!< Bboxes in image after resizing.


  
    int                  bgclass;
    int                  mask_class;
    idx<T>               mask;
    // scales //////////////////////////////////////////////////////////////
    vector<idxdim>       scales; //!< Multi-scale (ideal) scales.
    vector<idxdim>       actual_scales; //!< Actually used scales.
    vector<idxdim>       manual_scales; //!< Scales set manually.
    vector<double>       scale_factors; //!< A list of scale factors.
    uint		 nscales; //!< Number of scales if set by hand.
    double               scales_step;
    double               min_scale;//!< Minimum scale as factor of original res.
    double               max_scale;//!< Maximum scale as factor of original res.
    t_scaling            restype; //!< resolution type
    // saving //////////////////////////////////////////////////////////////
  
    bool                 silent; //!< print results on std output if not silent
    bool                 save_mode; //!< save detected windows or not
    string               save_dir; //!< directory where to save detections
    vector<uint>         save_counts; //!< file counter for each class
    vector<bbox*>        raw_bboxes; //!< raw bboxes extracted from outputs
    vector<bbox*>        pruned_bboxes; //!< scale-pruned bboxes
    uint                 min_size; //!< minimum input size to network
    uint                 max_size; //!< maximum input size to network
    vector<idx<T> >      odetections; //!< original windows yielding detection
    vector<idx<T> >      ppdetections; //!< preprocessed wins yielding detection
    bool                 bodetections; //!< odetections is up-to-date or not
    bool                 bppdetections; //!< ppdetections is up-to-date or not
    uint                 save_max_per_frame; //!< max number of region saved
    t_pruning            pruning; //!< Type of pruning.
    bool                 share_parts; //!< Allow parts sharing or not.
    T                    threshold2; //!< A threshold used for pruning.
    float                bbhfactor; //!< height bbox factor
    float                bbwfactor; //!< width bbox factor
    float                bb_woverh; //!< width / height factor
    float                bbhfactor2; //!< height bbox factor
    float                bbwfactor2; //!< width bbox factor
    bool                 mem_optimization; //!< optimize memory or not.
    bool                 optimization_swap; //!< swap buffers or not.
    bool                 keep_inputs; //! optimize input buffers or not.
    uint                 hzpad; //! Zero-pad on height (each side).
    uint                 wzpad; //! Zero-pad on width (each side).
    std::ostream         &mout; //! output stream.
    std::ostream         &merr; //! error output stream.
    float                min_hcenter_dist;
    float                min_wcenter_dist;
    float                max_center_dist; //!< Max center distance for pruning.
    float                max_center_dist2; //!< Max center distance for pruning.
    float                max_wcenter_dist; //!< Max center distance for pruning.
    float                max_wcenter_dist2; //!< Max center distance for pruning.
    float                min_wcenter_dist2;
    float                same_scale_mhd;
    float                same_scale_mwd;
    float                min_scale_pred;
    float                max_scale_pred;

    float                max_overlap; //!< Maximum ratio of overlap authorized.
    float                max_overlap2;
    bool                 mean_bb;
    double               max_object_hratio; //! max image_height/object_height
    intg                 min_input_height;
    intg                 min_input_width;
    // smoothing //////////////////////////////////////////////////////////////
    uint                 smoothing_type;
    idx<T>               smoothing_kernel;
    bool                 initialized;
    string               outputs_dump; //!< Outputs dumping name.
    bool                 bboxes_off; //!< Do not extract bboxes if true.
    bool                 adapt_scales; //!< Adapt scales to network structure.
    bool scaler_mode;
    answer_module<T,T,T,Tstate> &answer;
    bool cluster_nms;
    
    ////////////////////////////////////////////////////////////////
    // friends
    template <typename T2, class Tstate2> friend class detector_gui;
  };

} // end namespace ebl

#include "detector.hpp"

#endif /* DETECTOR_H_ */
