#ifndef LINEAR_INC
#define LINEAR_INC

#include "GradientMachine.h"

namespace Torch {

/** Linear layer for #GradientMachine#.
    Formally speaking, $ouputs[i] = \sum_j w_{ij} inputs[i] + b_i$.\\
    $w_{ij}$ and $b_j$ are in #params#, with the following structure:\\
    $w_00... w_0n, b_0, w_10... w_1n, b_1, ...$\\

    If you want, you can add a weight decay which looks like
    $\sum_i,j w_{ij}^2 + sum_i b_i^2$.

    Alternatively, you can add an L1 penalty, which looks like
    $\sum_i,j |w_{ij}|$ (but see comments for details).

    You can also add a non-zero ``bias decay'' term, which pushes the \\
    biases towards $-\infty$. This results in (simplistically) sparser \\
    hidden representations.

    Options:
    \begin{tabular}{lcll}
      "weight decay"  &  real  &  the weight decay & [0]
      "l1 weight decay"  &  real  &  the l1 weight decay & [1]
      "l1 fast weight decay"  &  real  & speed of l1 decay near 0 & [2]
      "bias decay" & real & "simple way of achieving sparse representations" [3]
    \end{tabular}

    @author Ronan Collobert (collober@idiap.ch)
*/
class Linear : public GradientMachine
{
  public:
    real weight_decay;
    real l1_weight_decay;
    real l1_fast_weight_decay;
    real bias_decay;
    real *weights;
    real *bias;
    real *der_weights;
    real *der_bias;
    void reset_();

    //-----

    ///
    Linear(int n_inputs_, int n_outputs_);

    //-----

    virtual void frameForward(int t, real *f_inputs, real *f_outputs);
    virtual void frameBackward(int t, real *f_inputs, real *beta_, real *f_outputs, real *alpha_);

    /// Computes new random values for weights.
    virtual void reset();

    virtual ~Linear();
};


}

#endif
