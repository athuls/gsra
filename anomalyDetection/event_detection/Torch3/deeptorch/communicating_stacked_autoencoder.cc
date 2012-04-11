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
#include "communicating_stacked_autoencoder.h"

//#include "Linear.h"
//#include "Tanh.h"
//#include "destructive.h"
//#include "nonlinear.h"
//#include "machines/identity.h"
//#include "transposed_tied_linear.h"
//#include <iostream>
namespace Torch {

CommunicatingStackedAutoencoder::CommunicatingStackedAutoencoder(std::string name_,
                                                                 std::string nonlinearity_,
                                                                 bool tied_weights_,
                                                                 int n_inputs_,
                                                                 int n_hidden_layers_,
                                                                 int *n_hidden_units_per_layer_,
                                                                 int n_outputs_,
                                                                 bool is_noisy_,
                                                                 int *n_speech_units_,
                                                                 int communication_type_,
                                                                 int n_communication_layers_)
    : StackedAutoencoder( name_, nonlinearity_, tied_weights_, n_inputs_,
                          n_hidden_layers_, n_hidden_units_per_layer_, n_outputs_,
                          is_noisy_)
{
  communication_type = communication_type_;
  n_communication_layers = n_communication_layers_;

  n_speech_units = (int*) allocator->alloc(sizeof(int)*(n_hidden_layers));
  for(int i=0; i<n_hidden_layers; i++)  {
    n_speech_units[i] = n_speech_units_[i];
  }

  // We're building what's needed for all 3 modes of communication, though
  // that is not necessary.

  BuildCommunicationCoders();
  BuildCommunicationAutoencoders();

  hidden_handles = (Identity**) allocator->alloc(sizeof(Identity*)*n_hidden_layers);
  speaker_handles = (Identity**) allocator->alloc(sizeof(Identity*)*n_hidden_layers);
  for(int i=0; i<n_hidden_layers; i++)  {
    hidden_handles[i] = new(allocator) Identity(encoders[i]->n_outputs);
  }

  if (communication_type > 0)
    for (int i=0; i<n_communication_layers; i++)
      speaker_handles[i] = new(allocator) Identity(speakers[i]->n_outputs);

  // The machine constructs
  sup_unsup_comA_machine = NULL;
  sup_unsup_comB_machine = NULL;
  sup_unsup_comC_machine = NULL;
  mentor = NULL;
  mentor_communicator = NULL;

  if (communication_type==0)
    BuildSupUnsupComA();
  else if (communication_type==1)
    BuildSupUnsupComB();
  else if (communication_type==2)
    BuildSupUnsupComC();
  else
    error("CommunicatingStackedAutoencoder::CommunicatingStackedAutoencoder: invalid communication_type");

  //BuildSupUnsupCsupCunsupMachine();
  //BuildMentor();

}

void CommunicatingStackedAutoencoder::BuildCommunicationCoders()
{
  // speakers
  speakers = (Coder**) allocator->alloc(sizeof(Coder*)*(n_communication_layers));
  for(int i=0; i<n_communication_layers; i++)    {
    if (communication_type == 2)
      speakers[i] = new(allocator)Coder(encoders[i]->n_outputs, n_speech_units[i],
                                        false, NULL, false, nonlinearity);
    else if (communication_type == 1)
      // This only works for two layers with the same number of hidden units.
      speakers[i] = new(allocator)Coder(encoders[i]->n_outputs, encoders[i]->n_outputs,
                                        false, NULL, false, nonlinearity);
    else
      speakers[i] = NULL;

  }

  // noisy speaker
  if(is_noisy)  {
    noisy_speakers = (Coder**) allocator->alloc(sizeof(Coder*)*(n_communication_layers));
    for(int i=0; i<n_communication_layers; i++)    {
      if (communication_type == 2)
        noisy_speakers[i] = new(allocator)Coder(encoders[i]->n_outputs, n_speech_units[i],
                                                true, speakers[i], false, nonlinearity);
      else if (communication_type == 1) // Using noise in this case is... weird
        // This only works for two layers with the same number of hidden units.
        noisy_speakers[i] = new(allocator)Coder(encoders[i]->n_outputs, encoders[i]->n_outputs,
                                                true, speakers[i], false, nonlinearity);
      else
        noisy_speakers[i] = NULL;

    }
  }     else    {
    noisy_speakers = NULL;
  }

  // listener
  if (communication_type == 2) {
    listeners = (Coder**) allocator->alloc(sizeof(Coder*)*(n_communication_layers));
    for(int i=0; i<n_communication_layers; i++)    {
      if(tied_weights)      {
        listeners[i] = new(allocator)Coder(n_speech_units[i], encoders[i]->n_outputs,
                                           true, speakers[i], true, nonlinearity);
      }   else    {
        listeners[i] = new(allocator)Coder(n_speech_units[i], encoders[i]->n_outputs,
                                           false, NULL, false, nonlinearity);
      }
    }
  }
  else
    listeners = NULL;
}

void CommunicatingStackedAutoencoder::BuildCommunicationAutoencoders()
{
  if (communication_type==2) {
    speakerlisteners = (ConnectedMachine**) allocator->alloc(sizeof(ConnectedMachine*)*(n_communication_layers));

    for(int i=0; i<n_communication_layers; i++) {
      speakerlisteners[i] = new(allocator)ConnectedMachine();

      if(is_noisy)
        speakerlisteners[i]->addFCL(noisy_speakers[i]);
      else
        speakerlisteners[i]->addFCL(speakers[i]);

      speakerlisteners[i]->addFCL(listeners[i]);
      speakerlisteners[i]->build();
    }
  }
  else
    speakerlisteners = NULL;
}

// TODO there may be something lighter when not noisy if we don't always use
// the autoencoder but instead plug an identity machine and the listener in
// the speaker
void CommunicatingStackedAutoencoder::AddComCMachines(ConnectedMachine *mch)
{
  if(!is_noisy) {
    AddMachines(mch,
                (GradientMachine**) speakers, (GradientMachine**) encoders);
    mch->addLayer();

    mch->addMachine(outputer);
    mch->connectOn(encoders[n_hidden_layers-1]);

    AddUnsupMachines(mch);

    AddMachines(mch,
                (GradientMachine**) speaker_handles, (GradientMachine**) speakers);

    AddMachines(mch,
                (GradientMachine**) listeners, (GradientMachine**) speakers);
  }
  // Since we can't plug the listeners on the speakers, we'll not waste time
  // with identity handles, and not add a layer. Speakers directly on last
  // layer.
  else  {
    mch->addMachine(outputer);
    mch->connectOn(encoders[n_hidden_layers-1]);

    AddUnsupMachines(mch);

    AddMachines(mch,
                (GradientMachine**) speakers, (GradientMachine**) encoders);

    AddMachines(mch,
                (GradientMachine**) speakerlisteners, (GradientMachine**) encoders);
  }
}

void CommunicatingStackedAutoencoder::AddMachines(ConnectedMachine *mch,
                                                  GradientMachine **addees,
                                                  GradientMachine **connectees)
{
  for(int i=0; i<n_communication_layers; i++) {
  
    mch->addMachine(addees[i]);
    mch->connectOn(connectees[i]);
  }
}

void CommunicatingStackedAutoencoder::BuildSupUnsupComA()
{
  sup_unsup_comA_machine = new(allocator) ConnectedMachine();
  AddCoreMachines(sup_unsup_comA_machine);

  sup_unsup_comA_machine->addMachine(outputer);
  sup_unsup_comA_machine->connectOn(encoders[n_hidden_layers-1]);

  AddUnsupMachines(sup_unsup_comA_machine);

  AddMachines(sup_unsup_comA_machine,
              (GradientMachine**) hidden_handles, (GradientMachine**) encoders);

  sup_unsup_comA_machine->build();
}

void CommunicatingStackedAutoencoder::BuildSupUnsupComB()
{
  sup_unsup_comB_machine = new(allocator) ConnectedMachine();

  AddCoreMachines(sup_unsup_comB_machine);

  sup_unsup_comB_machine->addMachine(outputer);
  sup_unsup_comB_machine->connectOn(encoders[n_hidden_layers-1]);

  AddUnsupMachines(sup_unsup_comB_machine);

  AddMachines(sup_unsup_comB_machine,
              (GradientMachine**) speakers, (GradientMachine**) encoders);

  sup_unsup_comB_machine->build();
}

void CommunicatingStackedAutoencoder::BuildSupUnsupComC()
{
  sup_unsup_comC_machine = new(allocator) ConnectedMachine();

  AddCoreMachines(sup_unsup_comC_machine);
  
  AddComCMachines(sup_unsup_comC_machine);

  sup_unsup_comC_machine->build();
}

/*
void CommunicatingStackedAutoencoder::BuildSupUnsupCsupCunsupMachine()
{
  sup_unsup_csup_cunsup_machine = new(allocator) ConnectedMachine();

  AddCoreMachines(sup_unsup_csup_cunsup_machine);

  // Add the output - Can't do FCL because if only 1 layer, then
  // there might be an identity layer.
  sup_unsup_csup_cunsup_machine->addMachine(outputer);
  sup_unsup_csup_cunsup_machine->connectOn(encoders[n_hidden_layers-1]);

  // add the hidden reconstruction and communicating components
  AddUnsupMachines(sup_unsup_csup_cunsup_machine);
  AddCommunicationMachines(sup_unsup_csup_cunsup_machine);

  sup_unsup_csup_cunsup_machine->build();
}
*/

void CommunicatingStackedAutoencoder::BuildMentor()
{
  if(communication_type!=2)
    return;

  // Mentor
  mentor = new(allocator) ConnectedMachine();

  // Construct the core machine
  // we could use AddCoreMachines, but we don't need the
  // identity machine that would be put on the first layer
  for(int i=0; i<n_communication_layers; i++)    {
    mentor->addMachine(encoders[i]);
    // connect it if not the first layer
    if(i>0)     {
      mentor->connectOn(encoders[i-1]);
    }
    mentor->addLayer();
  }

  // These are the sole outputs
  if(!is_noisy) {
    AddMachines(mentor,
                (GradientMachine**) speakers, (GradientMachine**) encoders);
    mentor->addLayer();

    AddMachines(mentor,
                (GradientMachine**) speaker_handles, (GradientMachine**) speakers);

    AddMachines(mentor,
                (GradientMachine**) listeners, (GradientMachine**) speakers);
  }
  // Since we can't plug the listeners on the speakers, we'll not waste time
  // with identity handles, and not add a layer. Speakers directly on last
  // layer.
  else  {
    AddMachines(mentor,
                (GradientMachine**) speakers, (GradientMachine**) encoders);

    AddMachines(mentor,
                (GradientMachine**) speakerlisteners, (GradientMachine**) encoders);
  }

  mentor->build();

  // Mentor communicator
  mentor_communicator  = new(allocator) ConnectedMachine();
  for(int i=0; i<n_communication_layers; i++)    {
    mentor_communicator->addMachine(speakers[i]);
    mentor_communicator->addMachine(speakerlisteners[i]);
  }
  mentor_communicator->build();

}

void CommunicatingStackedAutoencoder::setL1WeightDecay(real weight_decay)
{
  StackedAutoencoder::setL1WeightDecay(weight_decay);
  warning("CommunicatingStackedAutoencoder::setL1WeightDecay - fixme");
}

void CommunicatingStackedAutoencoder::setL2WeightDecay(real weight_decay)
{
  StackedAutoencoder::setL2WeightDecay(weight_decay);
  warning("CommunicatingStackedAutoencoder::setL2WeightDecay - fixme");
}

void CommunicatingStackedAutoencoder::setDestructionOptions(real destruct_prob, real destruct_value)
{
  StackedAutoencoder::setDestructionOptions(destruct_prob, destruct_value);
  warning("CommunicatingStackedAutoencoder::setDestructionOptions - fixme");
}

void CommunicatingStackedAutoencoder::loadXFile(XFile *file)
{
  sup_unsup_comC_machine->loadXFile(file);
}

void CommunicatingStackedAutoencoder::saveXFile(XFile *file)
{
  sup_unsup_comC_machine->saveXFile(file);
}

CommunicatingStackedAutoencoder::~CommunicatingStackedAutoencoder()
{
}

}
