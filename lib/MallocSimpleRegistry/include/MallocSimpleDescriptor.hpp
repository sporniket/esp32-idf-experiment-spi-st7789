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
#ifndef MALLOC_SIMPLE_DESCRIPTOR_HPP
#define MALLOC_SIMPLE_DESCRIPTOR_HPP

// standard includes
#include <cstdint>

// esp32 includes

// project includes

/** @brief A descriptor for a memory allocation that as been allocated.
 */
class MallocSimpleDescriptor {
    private:
    uint32_t size;
    bool dmaCapable;
    void *start;

    public:
    virtual ~MallocSimpleDescriptor();
    MallocSimpleDescriptor(uint32_t sizeInBytes, bool isDmaCapable, void *actualMalloc)
        : size(sizeInBytes), dmaCapable(isDmaCapable), start(actualMalloc) {}
    bool isActuallyAllocated() { return nullptr == start; }
    bool isDmaCapable() { return dmaCapable; }
    bool getSize() { return size; }
    bool getStart() { return start; }
};

#endif