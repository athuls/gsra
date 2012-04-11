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

#ifndef DATASET_H_
#define DATASET_H_

#define MKDIR_RIGHTS 0755

#define DATASET_SAVE "dataset"
#define DYNSET_SAVE "dynset"

#include "libidx.h"
#include "libeblearn.h"

typedef int t_label;

// jitter
typedef float t_jitter;
#define JITTERS 4 // number of jitter variables

namespace ebl {

  //! A class describing all attributes of a groundtruth object.
  class EXPORT object : public rect<int> {
  public:
    // constructors/destructors ////////////////////////////////////////////////
    
    //! Initialize this object by its bounding box.
    object(uint id);
    //! Destructor.
    virtual ~object();

    // accessors ///////////////////////////////////////////////////////////////

    //! Set rect attributes given these coordinates.
    virtual void set_rect(int xmin, int ymin, int xmax, int ymax);
    //! Set visible rect attributes given these coordinates.
    virtual void set_visible(int xmin, int ymin, int xmax, int ymax);
    //! Set centroid coordinates.
    virtual void set_centroid(int x, int y);

    // members /////////////////////////////////////////////////////////////////
    uint id; //!< Object id.
    rect<int> *visible; //!< The visible part of the object.
    pair<int,int> *centroid; //!< Centroid of object.
    string name; //!< The name of this object.
    bool difficult; //!< Difficulty flag.
    bool truncated; //!< Truncated flag.
    bool occluded; //!< Occlusion flag.
    string pose; //!< Pose description string.
    std::vector<object*> parts; //!< Parts of this object.
    bool ignored; //!< The object exists but not used as sample.
  };

  //! A class that describes jitter attributes.
  class EXPORT jitter {
  public:
    //! \param spatial_norm Normalize spatial components by dividing by this,
    //!   applied to the vector return by get_jitter_vector() only.
    jitter(float h_, float w_, float s_, float r_, int spatial_norm = 1);
    //! Create a jitter object from 'jit' with respect to a specific
    //! 'context' box.
    //! \param spatial_norm Normalize spatial components by dividing by this,
    //!   applied to the vector return by get_jitter_vector() only.
    jitter(rect<float> &context, rect<float> &jit, int spatial_norm = 1);
    //! Empty jitter.
    jitter();
    //! Destructor.
    virtual ~jitter();
    //! Returns the jittered version of r, except for rotation.
    //! \param ratio Scale spatial jitter by this ratio.
    template <typename T> 
      rect<T> get_rect(const rect<T> &r, float ratio = 1.0);
    //! Return a reference to a vector containing all jittering variables.
    const idx<t_jitter>& get_jitter_vector() const;
    //! Set jitter variables given a jitter vector.
    void set(const idx<t_jitter> &j);
    
    // members /////////////////////////////////////////////////////////////////
  public:
    float h; //!< Height offset
    float w; //!< Width offset.
    float s; //!< Scale factor.
    float r; //!< Rotation.
  private:
    idx<t_jitter> jitts; //!< A vector containing all jitter values.
  };
  
  //! The dataset class allows to extract a dataset from sample files and
  //! compile all samples into one dataset matrix, formatted for learning.
  template <class Tdata> class dataset {
  public:

    ////////////////////////////////////////////////////////////////
    // constructors/allocation

    //! Initialize the dataset's name and other internal variables, but does
    //! not allocate data matrices, user must call alloc for that effect.
    //! outdims are the target output dimensions of each sample.
    //! inroot is the root directory where to extract data.
    dataset(const char *name, const char *inroot = NULL);

    //! Destructor.
    virtual ~dataset();

    //! Count how many samples are present in files to be compiled,
    //! and allocate buffers to the number of samples.
    //! If max is given (default is 0), then the dataset is limited
    //! to max samples (or counted samples if less).
    bool alloc(intg max = 0);

    ////////////////////////////////////////////////////////////////
    // data manipulation

    //! Extract data from files into dataset.
    virtual bool extract();

    //! Split dataset into datasets ds1 and ds2, by limiting ds1 to max
    //! samples per class, putting anything left into ds2.
    //! Then save each dataset in outroot directory.
    bool split_max_and_save(const char *name1, const char *name2,
			    intg max, const string &outroot);
  
    //! Split dataset into datasets ds1 and ds2, by limiting ds1 to max
    //! samples per class, putting anything left into ds2.
    void split_max(dataset<Tdata> &ds1, dataset<Tdata> &ds2, intg max);

    //! merge datasets with names name1 and name2 found in outroot into
    //! this dataset.
    void merge(const char *name1, const char *name2, const string &outroot);

    //! Shuffle order of samples.
    void shuffle();

    ////////////////////////////////////////////////////////////////
    // data preprocessing

    //! Set type of image conversion with string conv_type.
    //! con_type can accept a number of strings,
    //! including RGB, Y, YUV, HSV, etc.
    void set_pp_conversion(const char *conv_type, idxdim &ppkernel_size);

    ////////////////////////////////////////////////////////////////
    // accessors    

    //! Get sample dimensions
    const idxdim &get_sample_outdim();

    //! Return the number of samples in the dataset.
    intg size();

    //! Return unsigned int label corresponding to class name
    t_label get_label_from_class(const string &class_name);
    
    //! Turn display on or off.
    void set_display(bool display);

    //! Make the display sleep delay milliseconds between frames
    void set_sleepdisplay(uint delay);

    //! Select the method for resizing. default is set to gaussian pyramids.
    //! options are: "gaussian" or "bilinear" for bilinear interpolation.
    void set_resize(const string &resize_mode);

    //! Specify the dimensions of 1 output sample.
    //! The default is: 96x96x3
    void set_outdims(const idxdim &d);

    //! Specify output directory.
    void set_outdir(const string &s);

    //! Specify the minimum dimensions of input samples.
    //! The default is: 1x1
    void set_mindims(const idxdim &d);

    //! Specify the maximum dimensions of input samples.
    void set_maxdims(const idxdim &d);

    //! Setting scale mode and scales: preprocess and save each image
    //! in each scale in outdir directory.
    void set_scales(const vector<double> &sc, const string &od);
    
    //! Setting fovea scales: the feature layer is duplicated for
    //! each scale factor provided here.
    void set_fovea(const vector<double> &scales);
    
    //! Set all max per class to max.
    void set_max_per_class(intg max);

    //! Set maximum number of samples.
    void set_max_data(intg max);

    //! Set the pattern used to find image.
    void set_image_pattern(const string &p);

    //! Set the list of classes to exclude (including everything else).
    void set_exclude(const vector<string> &ex);

    //! Set the list of classes to include (excluding everything else).
    void set_include(const vector<string> &inc);

    //! Set saving mode, default is "dataset", saving everything in single
    //! lush matrix format. other values can be "ppm", "jpg", etc.
    void set_save(const string &save);

    //! Set name of dataset. This name will be used by load and save methods.
    //! It is usually set in the constructor, but one can also use this method
    //! instead.
    void set_name(const string &name);

    //! If called, this method will force the dataset to assign this one
    //! label to all collected images.
    void set_label(const string &label);

    //! Multiply bounding boxes by factor. This is useful to move object's
    //! boundaries away from borders when bounding boxes are too tight around
    //! objects.
    void set_bboxfact(float factor);

    //! Multiply bounding boxes height by factor.
    //! This is useful to move object's
    //! boundaries away from borders when bounding boxes are too tight around
    //! objects.
    void set_bboxhfact(float factor);

    //! Multiply bounding boxes width by factor. This is useful to move object's
    //! boundaries away from borders when bounding boxes are too tight around
    //! objects.
    void set_bboxwfact(float factor);

    //! Force bounding box width to be h * factor.
    //! This is useful to normalize bounding boxes with varying width to a fixed
    //! height/width ratio.
    void set_bbox_woverh(float factor);

    //! If true, ignore samples with padded areas, i.e. too small for target 
    //! size.
    void set_nopadded(bool nopadded);

    //! Add n samples randomly jittered over in a (minradius,maxradius)
    //! spatial neighborhood around original location.
    void set_jitter(uint tjitter_step, uint tjitter_hmin, uint tjitter_hmax,
		    uint tjitter_wmin, uint tjitter_wmax,
		    uint scale_steps, float scale_min, float scale_max,
		    uint rotation_steps, float rotation_range,
		    uint njitter);

    //! Set minimum visibility ratio (between 0.0 and 1.0) of a sample.
    //! This may be used to ignore
    //! sample rects that are cropped too much while jittering.
    //! It may also be used when visible regions are defined, in which case
    //! the ratio is computed using the overlap of the visible bounding box
    //! with the original bounding box.
    virtual void set_minvisibility(float minvis);

    //! Add sample mirrored with vertical-axis symmetry.
    void set_wmirror();

    //! Saves all displayed frames to 'dir'.
    //! If h and w are different than 0, fix the saved frames to hxw.
    void save_display(const string &dir, uint h = 0, uint w = 0);
    
    //! use pose information to separate classes. e.g. if for class "person"
    //! we have "front" and "side" pose, create 2 classes "person_front"
    //! and "person_side" instead of 1 class "person".
    void use_pose();

    //! also extract parts of objects if available.
    //! e.g. in pascal if for class "person", parts "face", "hand" and "foot"
    //! are available.
    void use_parts();

    //! only extract parts of objects if available.
    //! e.g. in pascal if for class "person", parts "face", "hand" and "foot"
    //! are available.
    void use_parts_only();

    //! Dataset has reached maximum sample capacity (this can be controlled
    //! by setting max_data variable).
    //! A label can be optionally passed to return if the dataset is full for
    //! a particular class (this is relevant only if set_max_per_class has been
    //! previously set.
    bool full(t_label label = -1);
    
    ////////////////////////////////////////////////////////////////
    // I/O

    //! Load dataset found in root. root should be including the name of the
    //! dataset, e.g.: "/data/dataset1" is going to load dataset1_*.mat
    bool load(const string &root);

    //! Save dataset in root
    bool save(const string &root);

    ////////////////////////////////////////////////////////////////
    // print methods

    //! Prints number of classes and list on standard output
    void print_classes();
    //! Prints number of samples for each class on std output
    void print_stats();
    
    ////////////////////////////////////////////////////////////////
    // Helper functions

    //! Returns a matrix of class names based on the classes vector
    static idx<ubyte> build_classes_idx(vector<string> &classes);

  protected:
    
    ////////////////////////////////////////////////////////////////
    // allocation
    
    //! Allocate dataset with n samples, each with dimensions d.
    //! This method is not called by the constructor and must be called
    //! before adding data samples.
    //! The buffers can be allocated only once, this is not meant to
    //! be used as a buffer resize method.
    bool allocate(intg n, idxdim &d);

    //! Allocate preprocessing and resizing modules.
    void init_preprocessing();

    ////////////////////////////////////////////////////////////////
    // data manipulation

    //! add sample d to the data with label class_name
    //! (and converting from Toriginal to Tdata type).
    //! r is an optional region of interest rectangle in the image d.
    //! \param visr An optional bounding box for the visible area of the object
    //! \param cropr An optional bounding box of ROI of the image to crop.
    //!    Image should already be cropped, use only for display.
    //! \param objs An optional vector of other objects in current image,
    //!    for display purposes.
    //! \param objs An optional vector of other objects in current image,
    //!    for display purposes.
    //! \param jittforce If not null, ignore other jittering and use only this
    //!   one.
    virtual bool add_data(idx<Tdata> &d, const t_label label,
			  const string *class_name,
			  const char *filename = NULL,
			  const rect<int> *r = NULL,
			  pair<int,int> *center = NULL,
			  const rect<int> *visr = NULL,
			  const rect<int> *cropr = NULL,
			  const vector<object*> *objs = NULL,
			  const jitter *jittforce = NULL);

    //! add/save sample, called at the end of add_data.
    void add_data2(idx<Tdata> &sample, t_label label, const string *class_name,
		   const char *filename, const jitter *jitt,
		   idx<t_jitter> *js);
    
    //! add a class name
    bool add_class(const string &class_name);

    //! set all classes directly using an idx of classes strings
    void set_classes(idx<ubyte> &classidx);

    //! count how many samples are present in dataset files to be compiled.
    virtual bool count_samples();

    //! split current dataset into ds1 and ds2, using their max_per_class
    //! array (first filling ds1 until full, then ds2, therefore assuming
    //! that maximums are lower for ds1 than ds2).
    void split(dataset<Tdata> &ds1, dataset<Tdata> &ds2);

    template <class Toriginal>
      bool save_scales(idx<Toriginal> &d, const string &filename);

    //! return true if class_name is authorized (based on excluded and included
    //! variables).
    virtual bool included(const string &class_name);

    ////////////////////////////////////////////////////////////////
    // data preprocessing

    //! Preprocess data d of type Toriginal into an idx of type Tdata
    //! with output dimensions outdims and return the result.
    //! The type of preprocessing can be selected using set_pp_conversion().
    //! @param outr If not null, copy the rect of the input region in the
    //!        output image.
    //! \param visr An optional bounding box for the visible area of the object
    //! \param cropr An optional bounding box of ROI of the image to crop.
    //!    Image should already be cropped, use only for display.
    //! \param scale Scale the input box by this factor.
    //! \param inr_out If not null, this rectangle will be filled with the
    //!          actual rectangle used for extraction.
    idx<Tdata> preprocess_data(idx<Tdata> &d, const string *class_name,
			       const char *filename = NULL,
			       const rect<int> *r = NULL, double scale = 0,
			       rect<int> *outr = NULL,
			       pair<int,int> *center = NULL,
			       jitter *jitt = NULL,
			       const rect<int> *visr = NULL,
			       const rect<int> *cropr = NULL,
			       rect<int> *inr_out = NULL);

    //! Display the added sample and the original image.
    //! \param visr An optional bounding box for the visible area of the object
    //! \param cropr An optional bounding box of ROI of the image to crop.
    //!    Image should already be cropped, use only for display.
    //! \param scale Scale the input box by this factor.
    //! \param inr The actual rectangle used for extraction.
    //! \param origr The original object bounding box.
    void display_added(idx<Tdata> &added, idx<Tdata> &original, 
		       const string *class_name, 
		       const char *filename = NULL,
 		       const rect<int> *inr = NULL,  
		       const rect<int> *origr = NULL,  
		       bool active_sleepd = true,
		       pair<int,int> *center = NULL,
		       const rect<int> *visr = NULL,
		       const rect<int> *cropr = NULL,
		       const vector<object*> *objs = NULL,
		       const jitter *jitt = NULL,
		       idx<t_jitter> *js = NULL);

    ////////////////////////////////////////////////////////////////
    // Helper functions

    //! Return the class name associated with label id.
    string& get_class_string(t_label id);

    //! Return the label id associated with class name
    t_label get_class_id(const string &name);
    
    //! Compute statistics about the dataset
    void compute_stats();

    //! Count (recursively) how many files matching pattern are inside dir.
    uint count_matches(const string &dir, const string &pattern);

    //! Recursively add all files matching the pattern ext to the dataset
    //! with class class_name.
    void process_dir(const string &dir, const string &ext,
		     const string &class_name);
    
    //! Method to load an image.
    virtual void load_data(const string &fname);

    //! Fills internal 'random_jitter' vector with all possible jitters
    //! in random order.
    virtual void compute_random_jitter();
    
  protected:
    // data ////////////////////////////////////////////////////////
    idx<Tdata>		data;        	//!< data matrix
    idx<t_label>	labels;	        //!< labels matrix
    idxs<t_jitter>	jitters;        //!< jitter info matrix
    vector<string>	classes;	//!< list of classes strings
    idx<t_label>        classpairs;	//!< sample pairs class-wise
    idx<t_label>        deformpairs;	//!< sample pairs deformation-wise
    // data helpers ////////////////////////////////////////////////
    uint                height;         //!< height of output
    uint                width;          //!< width of output
    bool		allocated;	//!< data matrices allocated or not
    bool                no_outdims;     //!< no outdims were specified.
    idxdim		outdims;	//!< dims of sample out dimensions
    idxdim		mindims;	//!< min dims of input samples
    idxdim		maxdims;	//!< max dims of input samples
    bool		maxdims_set;	//!< max dims of input samples is set?
    idxdim		datadims;	//!< dimensions of data out dimensions
    intg		data_cnt;	//!< number of samples added so far
    intg		processed_cnt;	//!< #processed (not necessarly added)
    intg		max_data;	//!< user can limit samples# with this
    bool		max_data_set;	//!< max_data been set by user or not
    intg                total_samples;	//!< number of samples of dataset
    idx<intg>           max_per_class;	//!< max # samples per class
    intg                mpc;            //!< value to put in max_per_class
    bool                max_per_class_set;	//!< mpc has been set or not
    idx<Tdata>          load_img;       //!< temporary image loader
    bool                scale_mode;     //!< scales saving mode
    vector<double>      scales;         //!< integer scales
    bool                interleaved_input; //!< indicate input is interleaved
    vector<string>      exclude;        //!< list of excluded classes
    vector<string>      include;        //!< list of included classes
    bool                usepose;        //!< use pose or not
    bool                useparts;       //!< use parts or not
    bool                usepartsonly;   //!< use parts only or not
    string              save_mode;      //!< saving mode (dataset, ppm, png..)
    list<string>        images_list;    //!< List of saved image files.
    bool                wmirror;        //!< add vertical-axis symmetry
    // bbox transformations /////////////////////////////////////////////////
    float               bboxhfact;      //!< bounding boxes height factor
    float               bboxwfact;      //!< bounding boxes width factor
    float               bbox_woverh;    //!< bounding boxes width over h factor
    string              force_label;    //!< force all labels to this one
    bool                nopadded;       //!< ignore too small samples
    float               minvisibility;
    // jitter ///////////////////////////////////////////////////////
    int                 tjitter_step;   //!< Translation step, in pixels.
    int                 tjitter_hmin;   //!< Translation height jitter min.
    int                 tjitter_hmax;   //!< Translation height jitter max.
    int                 tjitter_wmin;   //!< Translation width jitter min.
    int                 tjitter_wmax;   //!< Translation width jitter max.
    int                 sjitter_steps;  //!< number of possible scales
    float               sjitter_min;    //!< min range of scales
    float               sjitter_max;    //!< max range of scales
    int                 rjitter_steps;  //!< number of possible rotations
    float               rjitter;        //!< range of rotations
    uint                njitter;        //!< number of random jitters
    bool                bjitter;        //!< use jitter or not
    vector<jitter>      random_jitter;  //!< pre-computed list of random jitter
    // names ///////////////////////////////////////////////////////
    string		name;	        //!< dataset name
    string		data_fname;	//!< data filename
    string		labels_fname;	//!< labels filename
    string		jitters_fname;	//!< jitters filename
    string		classes_fname;	//!< classes filename
    string		classpairs_fname;	//!< classpairs filename
    string		deformpairs_fname;	//!< deformpairs filename
    // directories /////////////////////////////////////////////////
    string		inroot;         //!< root directory of input files
    string		outdir;         //!< root directory of output files
    string              extension;	//!< extension of files to extract
    // display /////////////////////////////////////////////////////
    bool		display_extraction;	//!< display during extraction
    bool		display_result;	//!< display extracted dataset
    Tdata               minval;         //!< minimum value to display
    Tdata               maxval;         //!< minimum value to display
    bool                sleep_display;	//!< enable sleeping when displaying
    uint                sleep_delay;	//!< display sleep delay in ms
    bool                bsave_display;
    string              save_display_dir;
    // stats ///////////////////////////////////////////////////////
    uint                nclasses;       //!< Number of classes.
    idx<intg>           class_tally;	//!< counter for class tally
    idx<intg>		add_tally;	//!< counter for additions tally
    uint                add_errors;     //!< Number of adding failures.
    timer               xtimer;         //!< Extraction timer.
    // preprocessing ///////////////////////////////////////////////
    string		ppconv_type;	//!< name of image conversion
    idxdim		ppkernel_size;	//!< size of kernel for pp
    bool		ppconv_set;	//!< ppconv_type has been set or not
    bool		do_preprocessing;	//!< activate or deactivate pp
    string              resize_mode;    //!< type of resizing (bilin, gaussian)
    module_1_1<fs(Tdata)>  *ppmodule;       //!< pp module 
    resizepp_module<fs(Tdata)> *resizepp;   //!< pp resizing module
    rect<int>           original_bbox;  //!< bbox of image in resized image
    vector<double>      fovea; //!< fovea context
  };
  
  ////////////////////////////////////////////////////////////////
  // Helper functions
  
  //! Builds dataset filenames based on root, name, etc.
  EXPORT void build_fname(string &ds_name, const char *fname, string &fullname);

  //! Recursively goes through dir, looking for files matching extension ext,
  //! and returns the number of matches.
  EXPORT uint count_matches(const string &dir, const string &pattern);

  ////////////////////////////////////////////////////////////////
  // loading errors
  
  //! required datasets, throw error if bool is false, otherwise print success.
  //! return success.
  template <typename T>
    bool loading_error(idx<T> &mat, string &fname);
  //! optional datasets, issue warning if bool is false, otherwise print
  //! success. return succcess.
  template <typename T>
    bool loading_warning(idx<T> &mat, string &fname);
  //! Optionally load matrices from fname into 'mat', issue warning if bool is
  //! false, otherwise print success. return succcess.
  template <typename T>
    bool loading_warning(idxs<T> &mat, string &fname);
  //! optional datasets, no warning if bool is false, otherwise print
  //! success. return succcess.
  template <typename T>
    bool loading_nowarning(idx<T> &mat, string &fname);

} // end namespace ebl

#include "dataset.hpp"

#endif /* DATASET_H_ */
