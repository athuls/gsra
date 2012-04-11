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
#ifndef TORCH_STACKED_AUTOENCODER_H_
#define TORCH_STACKED_AUTOENCODER_H_

#include <string>
#include "ConnectedMachine.h"
#include "coder.h"

namespace Torch {

class Identity;
//class Linear;
//class Destructive;
//class Nonlinear;
class Coder;

// A stacked autoencoder.
//
// Uses ConnectedMachine's usual variables to hold a usual neural net,
// but also has decoders for reconstruction at each layer. See the full_sae
// attribute.
//
class StackedAutoencoder : public ConnectedMachine
{
  public:

    std::string name;                // Name of the machine. Usefull for identifying
                                // individuals in a population
    bool is_noisy;              // If True, use destructive layers in autoencoders
    bool tied_weights;          // Specifies if weights tied in autoencoders.
    std::string nonlinearity;        // Specifies which nonlinearity to use: 'sigmoid',
                                // 'tanh' or 'nonlinear'

    int n_hidden_layers;
    int *n_units_per_layer;     // size is n_hidden_layers + 2

    // Machines are easy to add to the last layer and to connect to any machine
    // in the connected machine. However, only the machines on the first layer
    // are "connected" to the input. Once we've put a second layer, it's a pain
    // to add on the first layer. So we add a fake machine on the first layer
    // that just fprops the input. We use this for example if the machine is
    // noisy to "connect" a corruption machine to the "input".
    // MAY BE USED IN MULTIPLE MACHINES!
    Identity *input_handle_machine;

    // Tabs on the various parts of the machine.
    Coder **encoders;
    Coder **noisy_encoders;
    Coder **decoders;
    Coder *outputer;

    // The following machines use the coders as building blocks.

    ConnectedMachine** autoencoders;    // a combination of a (possibly noisy) encoder
                                        // and a decoder.
    ConnectedMachine** mesd_machines;   // (possibly) multiple encoders single
                                        // decoder machine. mesd_machines[i] encodes
                                        // from the input to layer i then decodes.

    ConnectedMachine* sup_machine;      // takes x as input and returns \hat{y}
                                        // just a copy of 'this'
    ConnectedMachine* unsup_machine;    // outputs the resonstructed units
                                        // \hat{x}, \hat{h1}, \hat{h2}, ...
    ConnectedMachine* sup_unsup_machine;        // outputs the supervised output and
                                                // the resonstructed units
                                                // y, \hat{x}, \hat{h1}, \hat{h2}, ...

    StackedAutoencoder(std::string name_,
                       std::string nonlinearity_,
                       bool tied_weights_,
                       int n_inputs_,
                       int n_hidden_layers_,
                       int *n_hidden_units_per_layer_,
                       int n_outputs_,
                       bool is_noisy=false);

    //
    virtual void AddCoreMachines(ConnectedMachine* mch);
    virtual void AddUnsupMachines(ConnectedMachine* mch);
    virtual void BuildCoders();
    virtual void BuildAutoencoders();
    virtual void BuildMesdMachines();
    virtual void BuildSupMachine();
    virtual void BuildUnsupMachine();
    virtual void BuildSupUnsupMachine();

    // When 2 layers share weights, only 1 should be decayed?
    virtual void setL1WeightDecay(real weight_decay);
    virtual void setL2WeightDecay(real weight_decay);
    virtual void setBiasDecay(real bias_decay);
    virtual void setDestructionOptions(real destruct_prob, real destruct_value);

    // Saves-loads the parameters. Currently the rest of the save is in
    // helpers (the topology).
    // TODO - see about changing things so this save saves all the necessary
    // stuff.
    virtual void loadXFile(XFile *file);
    virtual void saveXFile(XFile *file);

    virtual ~StackedAutoencoder();

};

}

#endif // TORCH_STACKED_AUTOENCODER_H_
