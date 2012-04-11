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

#include "vectors_angle_measurer.h"
#include "Vec.h"

namespace Torch {

VectorsAngleMeasurer::VectorsAngleMeasurer(int n_vectors_, int vector_size_, real **vectors_, XFile *file_)
    : Measurer(NULL, file_)
{
  n_vectors = n_vectors_;
  vector_size = vector_size_;
  vectors = vectors_;

  vectors_sum = (real*)allocator->alloc(sizeof(real)*vector_size);

  angle_sums = (real*)allocator->alloc(sizeof(real)*n_vectors);
  angle_means = (real*)allocator->alloc(sizeof(real)*n_vectors);

  reset();
}

void VectorsAngleMeasurer::measureExample()
{
  count += 1.;

  // compute the sum vector
  Vec vec_vectors_sum(vectors_sum, vector_size);
  vec_vectors_sum.zero();

  real* ptr_vec;
  real* ptr_sum;
  for(int i=0; i<n_vectors; i++)        {
    ptr_vec = vectors[i];
    ptr_sum = vectors_sum;
    for(int j=0; j<vector_size; j++)        {
      *ptr_sum++ += *ptr_vec++;
    }
  }


  // compute the angles
  Vec vec_a_vector(NULL, vector_size);
  real angle;
  for(int i=0; i<n_vectors; i++)        {
    vec_a_vector.ptr = vectors[i];

    angle = vec_a_vector.iP(&vec_vectors_sum);
    angle = angle / (vec_a_vector.norm2() * vec_vectors_sum.norm2());

    // It seems I was having numerical errors.
    if(angle>1.0)
      angle = 1.0;
    if(angle<-1.0)
      angle = -1.0;

    angle = acos(angle) * 180.0 / 3.1415926535;

    angle_sums[i] += angle;
  }

}

void VectorsAngleMeasurer::measureIteration()
{
  for(int i=0; i<n_vectors; i++)        {
    angle_means[i] = angle_sums[i] / count;
  }

  if(binary_mode)       {
    file->write(angle_means, sizeof(real), n_vectors);
  }
  else  {
    for(int i=0; i<n_vectors; i++)        {
      file->printf("%g ", angle_means[i]);
    }
    file->printf("\n");
  }
  file->flush();
  reset();
}

void VectorsAngleMeasurer::reset()
{
  count = 0.;
  for(int i=0; i<n_vectors; i++)        {
    angle_sums[i] = 0.;
  }
}

VectorsAngleMeasurer::~VectorsAngleMeasurer()
{
}

}
