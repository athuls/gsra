#include "Measurer.h"
#include "DataSet.h"
#include "DiskXFile.h"

namespace Torch {

Measurer::Measurer(DataSet *data_, XFile *file_)
{
  file = file_;
  data = data_;

  addBOption("binary mode", &binary_mode, false, "binary mode");
}

void Measurer::reset()
{
}

void Measurer::measureExample()
{
}

void Measurer::measureIteration()
{
}

void Measurer::measureEnd()
{
}

Measurer::~Measurer()
{
}

}
