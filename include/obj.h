#pragma once

#include <memory>
#include "model.h"
std::unique_ptr<Model> load_obj(const char * filename);
