#ifndef CLASS_NLL_MEASURER_INC
#define CLASS_NLL_MEASURER_INC

#include "Measurer.h"
#include "DataSet.h"
#include "ClassFormat.h"

namespace Torch {

/** This class measures the negative log likelihood. In fact, it supposes
    that for each input frames, frames[i] is the log-probability for class $i$.
    The given #class_format# gives the class format of the targets of the dataset.

    By default, the measurer divides the log-probabilty by the number of examples
    and the number of frames.

    @author Ronan Collobert (collober@idiap.ch)
*/
class ClassNLLMeasurer : public Measurer
{
  public:
    ClassFormat *class_format;
    bool average_examples;
    bool average_frames;
    real internal_error;
    Sequence *inputs;
    
    //-----

    ///
    ClassNLLMeasurer(Sequence *inputs_, DataSet *data_, ClassFormat *class_format_, XFile *file_);

    //-----

    virtual void reset();
    virtual void measureExample();
    virtual void measureIteration();

    virtual ~ClassNLLMeasurer();
};


}

#endif
