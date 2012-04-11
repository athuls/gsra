#ifndef CLASS_MEASURER_INC
#define CLASS_MEASURER_INC

#include "Measurer.h"
#include "ClassFormat.h"

namespace Torch {

/** Compute the classification error (in %)
    of the #inputs# with respect to the #targets# of #data#.

    The format of the class is given with a #ClassFormat#.
    It can print the confusion matrix if specified.

    @author Ronan Collobert (collober@idiap.ch)
*/
class ClassMeasurer : public Measurer
{
  public:
    real internal_error;
    int **confusion;
    Sequence *inputs;
    ClassFormat *class_format;
    bool calc_confusion_at_each_iter;
    int n_classes;
    int n_examples;

    //-----

    ///
    ClassMeasurer(Sequence *inputs_, DataSet *data_, ClassFormat *class_format_, XFile *file_,
                  bool calc_confusion_at_each_iter_=false);

    //-----

    void printConfusionMatrix();
    virtual void reset();
    virtual void measureExample();
    virtual void measureIteration();
    void reset_();

    virtual ~ClassMeasurer();
};

}

#endif
