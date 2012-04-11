#ifndef GRADIENT_CHECK_MEASURER_INC
#define GRADIENT_CHECK_MEASURER_INC

#include "Measurer.h"
#include "GradientMachine.h"
#include "Criterion.h"

namespace Torch {

class GradientCheckMeasurer : public Measurer
{
  public:
    real *save_params;
    real *save_der_params;
    real *fake_der_params;
    GradientMachine *machine;
    Criterion *criterion;

    //-----

    ///
    GradientCheckMeasurer(GradientMachine *machine_, Criterion *criterion_, DataSet *data_, XFile *file_);

    //-----

    virtual void measureExample();
    virtual ~GradientCheckMeasurer();
};


}

#endif
