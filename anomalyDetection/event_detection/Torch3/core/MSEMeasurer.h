#ifndef MSE_MEASURER_INC
#define MSE_MEASURER_INC

#include "Measurer.h"

namespace Torch {

/** Mean Squared Error measurer.
    Compute the MSE between its inputs,
    and the targets of its associated #DataSet#.

  addBOption("average examples", &average_examples, true, "divided by the number of examples");
  addBOption("average frame size", &average_frame_size, true, "divided by the frame size");
  addBOption("average frames", &average_frames, true, "divided by the number of frames");

    Options:
    \begin{tabular}{lcll}
      "average examples"    &  bool  &  divided by the number of examples  &  [true]\\
      "average frame size"  &  bool  &  divided by the frame size          &  [true]\\
      "average frames"      &  bool  &  divided by the number of frames    &  [true]
    \end{tabular}

    @author Ronan Collobert (collober@idiap.ch)
*/
class MSEMeasurer : public Measurer
{
  public:
    bool average_examples;
    bool average_frame_size;
    bool average_frames;
    real internal_error;
    Sequence *inputs;

    //-----

    ///
    MSEMeasurer(Sequence *inputs_, DataSet *data_, XFile *file_);

    //-----

    virtual void reset();
    virtual void measureExample();
    virtual void measureIteration();

    virtual ~MSEMeasurer();
};

}

#endif
