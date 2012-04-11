#include "Linear.h"
#include "Random.h"

namespace Torch {

Linear::Linear(int n_inputs_, int n_outputs_) : GradientMachine(n_inputs_, n_outputs_, (n_inputs_+1)*n_outputs_)
{
  addROption("weight decay", &weight_decay, 0, "weight decay");
  addROption("l1 weight decay", &l1_weight_decay, 0, "l1 weight decay");
  addROption("l1 fast weight decay", &l1_fast_weight_decay, 1.0, "l1 fast weight decay");
  addROption("bias decay", &bias_decay, 0.0, "bias decay");
  weights = params->data[0];
  bias = params->data[0]+n_inputs*n_outputs;
  der_weights = der_params->data[0];
  der_bias = der_params->data[0]+n_inputs*n_outputs;
  reset_();
}

void Linear::reset()
{
  reset_();
}

void Linear::reset_()
{
  // Note: just to be compatible with "Torch II Dev"
  real *weights_ = weights;
  real bound = 1./sqrt((real)n_inputs);

  for(int i = 0; i < n_outputs; i++)
  {
    for(int j = 0; j < n_inputs; j++)
      weights_[j] = Random::boundedUniform(-bound, bound);
    weights_ += n_inputs;
    bias[i] = Random::boundedUniform(-bound, bound);
  }
}

void Linear::frameForward(int t, real *f_inputs, real *f_outputs)
{
  real *weights_ = weights;
  for(int i = 0; i < n_outputs; i++)
  {
    real out = bias[i];

    for(int j = 0; j < n_inputs; j++)
      out += weights_[j] * f_inputs[j];
    weights_ += n_inputs;

    f_outputs[i] = out;
  }
}

void Linear::frameBackward(int t, real *f_inputs, real *beta_, real *f_outputs, real *alpha_)
{
  if(!partial_backprop)
  {
    for(int i = 0; i < n_inputs; i++)
      beta_[i] = 0;
    
    real *weights_ = weights;
    for(int i = 0; i < n_outputs; i++)
    {
      real z = alpha_[i];
      for(int j = 0; j < n_inputs; j++)
        beta_[j] += z * weights_[j];
      weights_ += n_inputs;
    }
  }

  real *der_weights_ = der_weights;
  for(int i = 0; i < n_outputs; i++)
  {
    real z = alpha_[i];
    for(int j = 0; j < n_inputs; j++)
      der_weights_[j] += z * f_inputs[j];
    der_weights_ += n_inputs;

    der_bias[i] += z;
  }

  // Decrements the bias by a constant term. Very simplistic way of encouraging
  // sparse representations, if used in a sigmoid-like hidden unit.
  if (bias_decay !=0)
    for (int i = 0; i < n_outputs; i++)
      der_bias[i] += bias_decay;

  if (weight_decay != 0)
  {
    real *src_ = params->data[0];
    real *dest_ = der_params->data[0];
    // Note: no penalty applied on the biases.
    for(int i = 0; i < n_inputs*n_outputs; i++)
      dest_[i] += weight_decay * src_[i];
  }

  // Note that this is not exactly an L1 penalty.
  // If the abs(weight) > l1_weight_decay we get an L1 update.
  // If abs(weight) < l1_weight_decay we apply a (sort of) L2 update:
  // - when 0 < l1_fast_weight_decay < 1 then it's basically an L2 update. 
  // - when l1_fast_weight_decay = 1.0, then we move the weight faster towards
  // zero and can potentially make the weight equal to zero (which is what we
  // would like, ideally, since we're performing L1).
  if(l1_weight_decay != 0)   {
    real *src_ = params->data[0];
    real *dest_ = der_params->data[0];
    // Note: no penalty applied on the biases.
    for(int i=0; i<n_inputs*n_outputs; i++) {
      if(src_[i] > l1_weight_decay)
        dest_[i] += l1_weight_decay;
       else if(src_[i] < -l1_weight_decay)
        dest_[i] -= l1_weight_decay;
       else
        dest_[i] += l1_fast_weight_decay * src_[i];
    }
  }

}

Linear::~Linear()
{
}

}
