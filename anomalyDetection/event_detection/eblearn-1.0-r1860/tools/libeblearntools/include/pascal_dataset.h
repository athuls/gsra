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

#ifndef PASCAL_DATASET_H_
#define PASCAL_DATASET_H_

#include "dataset.h"
#include "xml_utils.h"

#define XML_PATTERN ".*[.]xml"

namespace ebl {
  
  //! The pascal_dataset class allows to extract a dataset of type PASCAL
  //! from sample files and
  //! compile all samples into one dataset matrix, formatted for learning.
  //! It derives from the dataset class, reimplementing only PASCAL specifics,
  //! such as reading xml files, etc.
  template <class Tdata> class pascal_dataset : public dataset<Tdata> {
  public:

    ////////////////////////////////////////////////////////////////
    // constructors

    //! Initialize the dataset's name and other internal variables, but does
    //! not allocate data matrices, user must call alloc for that effect.
    //! outdims are the target output dimensions of each sample.
    //! inroot is the root directory from which we extract data.
    //! ignore_diff ignores difficult objects if true.
    //! \param ignore_path The root of boxes to be ignored. Filenames must
    //!   correspond to original boxes to be used.
    pascal_dataset(const char *name, const char *inroot = NULL,
		   bool ignore_diff = true, bool ignore_trunc = false,
		   bool ignore_occluded = false,
		   const char *annotations = NULL,
		   const char *ignore_path = NULL);

    //! Destructor.
    virtual ~pascal_dataset();

    ////////////////////////////////////////////////////////////////
    // data

    //! Extract data from files into dataset.
    virtual bool extract();

    //! Set the minimum aspect ratio (width/height and between 0.0 and 1.0) 
    //! for a sample to be extracted.
    virtual void set_min_aspect_ratio(float minar);

    //! Set the maximum aspect ratio (width/height and between 0.0 and 1.0) 
    //! for a sample to be extracted.
    virtual void set_max_aspect_ratio(float maxar);

    //! Set the minimum distances to image borders below which bounding
    //! boxes will be rejected.
    virtual void set_minborders(idxdim &d);

    //! Set the maximum match between a jittered rect and another object
    //! in the same image, beyond which the jitter is ignored.
    virtual void set_max_jitter_match(float match);

  protected:

#ifdef __BOOST__ // disable some derived methods if BOOST not available
#ifdef __XML__ // disable some derived methods if XML not available
    
    //! return true if class_name is authorized (based on excluded and included
    //! variables, and difficult, truncated, occluded flags).
    virtual bool included(const string &class_name, uint difficult,
			  uint truncated, uint occluded);
    
    ////////////////////////////////////////////////////////////////
    // data
    
    //! count how many samples are present in dataset files to be compiled.
    virtual bool count_samples();

    //! count sample or not given an xml node. This will update the total_sample
    //! and total_difficult counters.
    virtual void count_sample(Node::NodeList &olist);
    
    ////////////////////////////////////////////////////////////////
    // internal methods

    //! process an xml file.
    virtual bool process_xml(const string &xmlfile);

    //! Gets all properties from an xml file.
    //! \param cropr If image is cropped to cropr, adjust coordinates
    //!    of objects in the image, do nothing if NULL.
    //! \param ignore If true, object is not used as sample.
    virtual bool get_properties(const string &xmlfile, string &image_filename,
				string &image_fullname, string &folder,
				int &height, int &width, int &depth,
				vector<object*> &objs, rect<int> **cropr,
				bool ignored = false);

    //! Gets all objects found in node list l.
    //! \param cropr If image is cropped to cropr, adjust coordinates
    //!    of objects in the image, do nothing if NULL.
    //! \param ignore If true, object is not used as sample.
    virtual void get_objects(Node::NodeList &l, vector<object*> &objs,
			     rect<int> *cropr = NULL, bool ignore = false);

    //! process one object from an xml file.
    virtual bool process_objects(const vector<object*> &objs,
				 int height, int width,
				 const string &image_fullname,
				 const string &image_filename,
				 const rect<int> *cropr = NULL);

    //! process image for one object.
    //! \param visr An optional bounding box for the visible area of the object
    //! \param cropr An optional bounding box of ROI of the image to crop.
    //!    Image should already be cropped, use only for display.
    virtual void process_image(idx<Tdata> &img, const rect<int> &r,
			       string &obj_class, uint difficult,
			       const string &image_filename,
			       pair<int,int> *center,
			       const rect<int> *visr = NULL,
			       const rect<int> *cropr = NULL);
    
#endif /* __XML__ */
#endif /* __BOOST__ */

    //! Fill internal 'random_jitter' std::vector with all possible jitters in
    //! random order.
    virtual void compute_random_jitter();

    //! Remove all jitters in internal vector 'random_jitter' for which
    //! the jitter box at index 'ibox' in 'boxes' match all other 'boxes'
    //! by more than 'max_match'.
    virtual void remove_jitter_matches(const std::vector<object*> &objs,
				       uint iobj, float max_match);
    
  protected:
    // "difficult" samples /////////////////////////////////////////
    std::vector<object*> objects; //!< A vector of all current objects.
    uint iobj; //!< The 'boxes' index of current box in current image.
    bool	ignore_difficult;	//!< ignore difficult or not
    intg	total_difficult;	//!< total number of difficult samples
    bool	ignore_truncated;	//!< ignore truncated or not
    intg	total_truncated;	//!< total number of truncated samples
    bool	ignore_occluded;	//!< ignore occluded or not
    intg	total_occluded;	        //!< total number of occluded samples
    intg        total_ignored;          //!< total ignore any catergory
    float       min_aspect_ratio;
    float       max_aspect_ratio;
    idxdim      minborders;
    idxdim      indims;
    // directories /////////////////////////////////////////////////
    string	annroot;	//!< directory of annotation xml files
    string	imgroot;	//!< directory of images
    string	ignore_root;	//!< directory of ignored annotatiosn
    // base class members to be used ///////////////////////////////
    using dataset<Tdata>::usepose;
    using dataset<Tdata>::useparts;
    using dataset<Tdata>::usepartsonly;
    using dataset<Tdata>::allocated;
    using dataset<Tdata>::total_samples;
    using dataset<Tdata>::inroot;
    using dataset<Tdata>::display_extraction;
    using dataset<Tdata>::display_result;
    using dataset<Tdata>::outdims;
    using dataset<Tdata>::full;
    using dataset<Tdata>::add_data;
    using dataset<Tdata>::print_stats;
    using dataset<Tdata>::data_cnt;
    using dataset<Tdata>::extension;
    using dataset<Tdata>::ppconv_type;
    using dataset<Tdata>::scale_mode;
    using dataset<Tdata>::exclude;
    using dataset<Tdata>::include;
    using dataset<Tdata>::force_label;
    using dataset<Tdata>::mindims;
    using dataset<Tdata>::xtimer;
    using dataset<Tdata>::processed_cnt;
    using dataset<Tdata>::minvisibility;
    using dataset<Tdata>::save_mode;
    // jitter //////////////////////////////////////////////////////
    using dataset<Tdata>::random_jitter; //!< Vector of possible jitters.
    float max_jitter_match; //!< Maximum match of jitter rect with neighbors.
  };

} // end namespace ebl

#include "pascal_dataset.hpp"

#endif /* PASCAL_DATASET_H_ */
