#pragma once

#include<map>
#include<memory>

#include "opengl_chunk_model.hpp"


typedef std::map<std::pair<int, int>, std::shared_ptr<OpenglChunkModel>> OpenglMapMoodel;
