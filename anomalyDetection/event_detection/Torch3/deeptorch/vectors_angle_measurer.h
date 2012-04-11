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
#ifndef TORCH_VECTORS_ANGLE_MEASURER_H_
#define TORCH_VECTORS_ANGLE_MEASURER_H_

#include "Measurer.h"

namespace Torch {

// Measures the angles between some vectors and their sum.
//
//
class VectorsAngleMeasurer : public Measurer
{
  public:

    int n_vectors;      // the number of vectors in the sum
    int vector_size;
    real** vectors;     // 'n_vectors' arrays of size 'vector_size'

    real* vectors_sum;  // holds the sums of the vectors in 'vectors'

    real count;         // number of observations (calls to measureExample)
    real *angle_sums;   // the sums of the angles (degrees) between each vector and its sum
    real *angle_means;  // sums / count

    //-----
    VectorsAngleMeasurer(int n_vectors_, int vector_size_, real **vectors_, XFile *file_);

    //-----
    virtual void reset();
    virtual void measureExample();
    virtual void measureIteration();

    virtual ~VectorsAngleMeasurer();
};

}
#endif  // TORCH_VECTORS_ANGLE_MEASURER_H_
