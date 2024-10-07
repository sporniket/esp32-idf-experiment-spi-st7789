
// header include
#include "PixelFormat.hpp"

// write code here...

// ======== Builtin instanciation ========

// Indexed builtins formats
const PixelFormat PixelFormat::I1I_8BE(1, PixelLayout::INTERLEAVED, StorageUnitFormat::SINGLE_BYTE);
const PixelFormat PixelFormat::I1I_16BE(1, PixelLayout::INTERLEAVED, StorageUnitFormat::DOUBLE_BYTES_BE);
const PixelFormat PixelFormat::I2I_16BE(2, PixelLayout::INTERLEAVED, StorageUnitFormat::DOUBLE_BYTES_BE);
const PixelFormat PixelFormat::I4I_16BE(4, PixelLayout::INTERLEAVED, StorageUnitFormat::DOUBLE_BYTES_BE);
const PixelFormat PixelFormat::I8I_16BE(8, PixelLayout::INTERLEAVED, StorageUnitFormat::DOUBLE_BYTES_BE);
const PixelFormat PixelFormat::I8P_8BE(8, PixelLayout::PROGRESSIVE, StorageUnitFormat::SINGLE_BYTE);
const PixelFormat PixelFormat::RGB332_8BE(3, 3, 2, StorageUnitFormat::SINGLE_BYTE, false);
const PixelFormat PixelFormat::RGB444C_12BE(4, 4, 4, StorageUnitFormat::ONE_AND_AN_HALF_BYTE_BE, true);
const PixelFormat PixelFormat::RGB555_16BE(5, 5, 5, StorageUnitFormat::DOUBLE_BYTES_BE, false);
const PixelFormat PixelFormat::RGB565_16BE(5, 6, 5, StorageUnitFormat::DOUBLE_BYTES_BE, false);
const PixelFormat PixelFormat::RGB666_24BE(6, 6, 6, StorageUnitFormat::TRIPLE_BYTES_BE, false);
const PixelFormat PixelFormat::RGB888_24BE(8, 8, 8, StorageUnitFormat::TRIPLE_BYTES_BE, false);

// ======== Code implementation ========
