#ifndef MEASURER_INC
#define MEASURER_INC

#include "Object.h"
#include "Machine.h"
#include "DataSet.h"
#include "XFile.h"

namespace Torch {


/** Used to measure what you want during training/testing.
    Usually, trainers call measurers.
    The #DataSet# associated to the measurer allow us
    to know when the measurer should be called.
    (if the #DataSet# is the train dataset, the
    measurer will be called during the train phase...)

    Options:
    \begin{tabular}{lcll}
      "binary mode"  &  bool  &  binary mode for output & [false]
    \end{tabular}

    @author Ronan Collobert (collober@idiap.ch)
*/
class Measurer : public Object
{
  public:
    /// The measurer save measures in this file.
    XFile *file;

    /// The associated #DataSet#.
    DataSet *data;

    /// Is the measurer in binary mode ?
    bool binary_mode;

    //-----

    /** Measurer with the associated #DataSet# #data_#,
        and put results in the file #file_#.
    */
    Measurer(DataSet *data_, XFile *file_);

    /** Measure something for the current example.
        (This example has been selected in #data#
         by the trainer)
    */
    virtual void measureExample();

    /** Measure something after the current iteration.
        (After the call of #measureExample()# for each
        example of #data#)
    */
    virtual void measureIteration();

    /// Measure something at the end of the training/testing phase.
    virtual void measureEnd();

    /// Holds the error after the last iteration, if it's a real
    real current_error;

    /// Reset the measurer. (By default, do nothing).
    virtual void reset();

    //-----

    virtual ~Measurer();

};
}

#endif
