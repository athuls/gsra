// Copyright 2008 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#include "stacked_autoencoder.h"
#include "Linear.h"
//#include "Tanh.h"
//#include "Sigmoid.h"
//#include "LogSoftMax.h"
#include "identity.h"
//#include "transposed_tied_linear.h"
#include "destructive.h"
//#include "nonlinear.h"
//#include "coder.h"

namespace Torch {

StackedAutoencoder::StackedAutoencoder(std::string name_,
                                       std::string nonlinearity_,
                                       bool tied_weights_,
                                       int n_inputs_,
                                       int n_hidden_layers_,
                                       int *n_units_per_hidden_layer_,
                                       int n_outputs_,
                                       bool is_noisy_)
{
  name = name_;
  is_noisy = is_noisy_;
  tied_weights = tied_weights_;
  nonlinearity = nonlinearity_;

  // the topology
  n_hidden_layers = n_hidden_layers_;

  n_units_per_layer = (int*) allocator->alloc(sizeof(int)*(n_hidden_layers+2));
  if(!n_units_per_layer)        {
    error("StackedAutoencoder::StackedAutoencoder(...) - Memory issue.");
  }

  n_units_per_layer[0] = n_inputs_;
  for(int i=0; i<n_hidden_layers; i++)  {
    n_units_per_layer[i+1] = n_units_per_hidden_layer_[i];
  }
  n_units_per_layer[n_hidden_layers+1] = n_outputs_;

  //
  input_handle_machine = new(allocator)Identity(n_units_per_layer[0]);
  BuildCoders();

  //
  BuildAutoencoders();

  BuildMesdMachines();

  BuildSupMachine();
  BuildUnsupMachine();
  BuildSupUnsupMachine();
}

void StackedAutoencoder::BuildCoders()
{
  // encoders
  encoders = (Coder**)allocator->alloc(sizeof(Coder*)*n_hidden_layers);
  for(int i=0; i<n_hidden_layers; i++) {
    encoders[i] = new(allocator) Coder(n_units_per_layer[i], n_units_per_layer[i+1],
                                       false, NULL, false, nonlinearity);
  }

  // noisy encoder
  if(is_noisy)  {
    noisy_encoders = (Coder**)allocator->alloc(sizeof(Coder*)*n_hidden_layers);

    for(int i=0; i<n_hidden_layers; i++) {
      noisy_encoders[i] = new(allocator)Coder(encoders[i]->n_inputs,
                                              encoders[i]->n_outputs,
                                              true, encoders[i], false,
                                              nonlinearity);
    }
  }
  else
    noisy_encoders = NULL;

  // decoders
  decoders = (Coder**)allocator->alloc(sizeof(Coder*)*n_hidden_layers);
  for(int i=0; i<n_hidden_layers; i++) {
    // decoder
    if(tied_weights)  {
      decoders[i] = new(allocator) Coder(encoders[i]->n_outputs, encoders[i]->n_inputs,
                                         false, encoders[i], true, nonlinearity);
    } else    {
      decoders[i] = new(allocator) Coder(encoders[i]->n_outputs, encoders[i]->n_inputs,
                                         false, NULL, false, nonlinearity);
    }
  }

  // Outputer
  outputer = new(allocator) Coder(n_units_per_layer[n_hidden_layers],
                                  n_units_per_layer[n_hidden_layers+1],
                                  false, NULL, false, "logsoftmax");


}

void StackedAutoencoder::BuildAutoencoders()
{
  autoencoders = (ConnectedMachine**) allocator->alloc(sizeof(ConnectedMachine*)*n_hidden_layers);

  for(int i=0; i<n_hidden_layers; i++) {
    autoencoders[i] = new(allocator)ConnectedMachine();

    if(is_noisy)
      autoencoders[i]->addFCL(noisy_encoders[i]);
    else
      autoencoders[i]->addFCL(encoders[i]);

    autoencoders[i]->addFCL(decoders[i]);
    autoencoders[i]->build();
  }
}

void StackedAutoencoder::BuildMesdMachines()
{
  mesd_machines = (ConnectedMachine**) allocator->alloc(sizeof(ConnectedMachine*)*n_hidden_layers);

  for(int i=0; i<n_hidden_layers; i++) {
    mesd_machines[i] = new(allocator)ConnectedMachine();

    for(int j=0; j<i+1; j++)
      mesd_machines[i]->addFCL(encoders[j]);

    mesd_machines[i]->addFCL(decoders[i]);
    mesd_machines[i]->build();
  }
}

void StackedAutoencoder::BuildSupMachine()
{
  for(int i=0; i<n_hidden_layers; i++) {
    this->addFCL(encoders[i]);
  }
  this->addFCL(outputer);
  this->build();

  sup_machine = this;
}

void StackedAutoencoder::AddCoreMachines(ConnectedMachine* mch)
{
  for(int i=0; i<n_hidden_layers; i++) {
    mch->addMachine(encoders[i]);

    // connect it, unless it's on the first layer
    if(i>0)     {
      mch->connectOn(encoders[i-1]);
    }

    // See motivation for input_handle_machine in the header...
    if(i==0 && is_noisy)  {
      mch->addMachine(input_handle_machine);
    }

    mch->addLayer();
  }
}

void StackedAutoencoder::AddUnsupMachines(ConnectedMachine* mch)
{
  for(int i=0; i<n_hidden_layers; i++) {
    // Just plug the decoder into the single encoder
    if(!is_noisy)  {
     mch->addMachine(decoders[i]);
     mch->connectOn(encoders[i]);
    // Use the autoencoder (it's noisy)
    }     else    {
      // Connect
      if(i>0)        {
       mch->addMachine(autoencoders[i]);
       mch->connectOn(encoders[i-1]);
      }   else    {
        // The first layer requires a special procedure, actually a big hack. The
        // reason is it can't be connected on the input. It must be added on the
        // first layer.
        mch->addMachine(autoencoders[i]);
        mch->connectOn(input_handle_machine);
      }
    }
  }

}

void StackedAutoencoder::BuildUnsupMachine()
{
  unsup_machine = new(allocator) ConnectedMachine();

  AddCoreMachines(unsup_machine);
  AddUnsupMachines(unsup_machine);

  unsup_machine->build();
}

void StackedAutoencoder::BuildSupUnsupMachine()
{
  sup_unsup_machine = new(allocator) ConnectedMachine();

  AddCoreMachines(sup_unsup_machine);

  // Build the final layer of sup_unsup_machine.
  // We can't call FCL because if only 1 layer, then there might be an identity
  // layer on the previous layer. We wouldn't want to connect to it.
  sup_unsup_machine->addMachine(outputer);
  sup_unsup_machine->connectOn(encoders[n_hidden_layers-1]);

  // Add the reconstruction of the input and hidden layers (except last)
  AddUnsupMachines(sup_unsup_machine);

  sup_unsup_machine->build();
}

// If tied weights, we only put weight decay on 1 of the machines that uses
// these weights.
void StackedAutoencoder::setL1WeightDecay(real weight_decay)
{
  for(int i=0; i<n_hidden_layers; i++) {
    encoders[i]->linear_layer->setROption("l1 weight decay", weight_decay);
  }

  outputer->linear_layer->setROption("l1 weight decay", weight_decay);

  if(!tied_weights)     {
    for(int i=0; i<n_hidden_layers; i++) {
      decoders[i]->linear_layer->setROption("l1 weight decay", weight_decay);
    }
  }
}

// If tied weights, we only put weight decay on 1 of the machines that uses
// these weights.
void StackedAutoencoder::setL2WeightDecay(real weight_decay)
{
  for(int i=0; i<n_hidden_layers; i++) {
    encoders[i]->linear_layer->setROption("weight decay", weight_decay);
  }

  outputer->linear_layer->setROption("weight decay", weight_decay);

  if(!tied_weights)     {
    for(int i=0; i<n_hidden_layers; i++) {
      decoders[i]->linear_layer->setROption("weight decay", weight_decay);
    }
  }
}

// Bias decay only makes sense for encoders
void StackedAutoencoder::setBiasDecay(real bias_decay)
{
  for(int i=0; i<n_hidden_layers; i++) {
    encoders[i]->linear_layer->setROption("bias decay", bias_decay);
  }

}

void StackedAutoencoder::setDestructionOptions(real destruct_prob, real destruct_value)
{
  if(is_noisy)  {
    for(int i=0; i<n_hidden_layers; i++) {
      noisy_encoders[i]->destructive_layer->setROption("Destruction probability", destruct_prob);
      noisy_encoders[i]->destructive_layer->setROption("Destruction value", destruct_value);
    }
  }
}

void StackedAutoencoder::loadXFile(XFile *file)
{
  sup_unsup_machine->loadXFile(file);
}

void StackedAutoencoder::saveXFile(XFile *file)
{
  sup_unsup_machine->saveXFile(file);
}

StackedAutoencoder::~StackedAutoencoder()
{
}

}
