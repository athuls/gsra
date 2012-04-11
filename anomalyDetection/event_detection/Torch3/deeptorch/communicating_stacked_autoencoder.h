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
#ifndef TORCH_COMMUNICATING_STACKED_AUTOENCODER_H_
#define TORCH_COMMUNICATING_STACKED_AUTOENCODER_H_

#include "ConnectedMachine.h"
#include "stacked_autoencoder.h"
#include "coder.h"
#include "identity.h"

namespace Torch {

//
// * communication 0: make the student and mentor's units be close.
// * communication 1: the student uses a coder to try and rebuild the mentor's
// hidden units.
// * communication 2: the student and the mentor both have a coder to produce a
// language. Make them agree on what they say about examples and have what they
// say be useful for reconstructing the hidden units.
//
class CommunicatingStackedAutoencoder : public StackedAutoencoder
{
  public:
    int communication_type;
    int n_communication_layers;
    int *n_speech_units;

    Identity** hidden_handles;  // these serve as handles to the hidden layers.
                                // This way we can "put the hidden layers on the
                                // output".
    Identity** speaker_handles;

    Coder **speakers;
    Coder **noisy_speakers;
    Coder **listeners;           // reconstruct your hidden from your speech
    ConnectedMachine **speakerlisteners;    // autoencoders that rebuild the
                                            // hidden from the speech.
                                            // (possible destruction)

    // normal machines
    ConnectedMachine *sup_unsup_comA_machine;
    ConnectedMachine *sup_unsup_comB_machine;
    ConnectedMachine *sup_unsup_comC_machine;

    // mentoring machines
    ConnectedMachine *mentor;   // machine with on its last layers the
                                // reconstruction of his hidden units
                                // from his speech and his speech
    ConnectedMachine *mentor_communicator;      // Ideally, we only want to
                                                // bprop to this part of the mentor
                                                // For that, we would need to
                                                // know this machine's inputs.
                                                // Currently, it just gives us
                                                // a handle on that params to
                                                // update so we limit the
                                                // number of copies

    CommunicatingStackedAutoencoder(std::string name_,
                                    std::string nonlinearity_,
                                    bool tied_weights_,
                                    int n_inputs_,
                                    int n_hidden_layers_,
                                    int *n_hidden_units_per_layer_,
                                    int n_outputs_,
                                    bool is_noisy_,
                                    int *n_speech_units_,
                                    int communication_type,
                                    int n_communication_layers);

    // Adds (and connects) a communication machine to machine. Layer determines
    // the layer at which the communication takes place.
    virtual void AddComCMachines(ConnectedMachine *mch);
    virtual void AddMachines(ConnectedMachine *mch, GradientMachine **addees,
                             GradientMachine **connectees);

    virtual void BuildCommunicationCoders();
    virtual void BuildCommunicationAutoencoders();

    virtual void BuildSupUnsupComA();
    virtual void BuildSupUnsupComB();
    virtual void BuildSupUnsupComC();

    //virtual void BuildSupUnsupCsupCunsupMachine();

    virtual void BuildMentor();

    virtual void setL1WeightDecay(real weight_decay);
    virtual void setL2WeightDecay(real weight_decay);
    virtual void setDestructionOptions(real destruct_prob, real destruct_value);

    virtual void loadXFile(XFile *file);
    virtual void saveXFile(XFile *file);

    virtual ~CommunicatingStackedAutoencoder();
};

}

#endif // TORCH_COMMUNICATING_STACKED_AUTOENCODER_H_
