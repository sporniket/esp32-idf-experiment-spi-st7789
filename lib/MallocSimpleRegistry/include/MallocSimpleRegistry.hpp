// Copyright 2023 David SPORN
// ---
// This file is part of 'MallocSimpleRegistry'.
// ---
// 'MallocSimpleRegistry' is free software: you can redistribute it and/or 
// modify it under the terms of the GNU General Public License as published 
// by the Free Software Foundation, either version 3 of the License, or 
// (at your option) any later version.

// 'MallocSimpleRegistry' is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General 
// Public License for more details.

// You should have received a copy of the GNU General Public License along 
// with 'MallocSimpleRegistry'. If not, see <https://www.gnu.org/licenses/>. 
#ifndef MALLOC_SIMPLE_REGISTRY_HPP
#define MALLOC_SIMPLE_REGISTRY_HPP

// standard includes
#include <cstdint>
#include <string>
#include <map>

// esp32 includes

// project includes
#include "MallocSimpleDescriptor.hpp"

using MallocSimpleRegistry = std::map<std::string, MallocSimpleDescriptor*> ;

#endif