
// header include
#include "PixelFormat.hpp"

// write code here...

// ======== Builtin instanciation ========

// Indexed builtins formats
const PixelFormat PixelFormat::I1I_8BE(1, PixelLayout::INTERLEAVED, StorageUnitFormat::SINGLE_BYTE);
const PixelFormat PixelFormat::I1I_16BE(1, PixelLayout::INTERLEAVED, StorageUnitFormat::DOUBLE_BYTES_BE);
const PixelFormat PixelFormat::PixelFormat_i2i(2, PixelLayout::INTERLEAVED, StorageUnitFormat::DOUBLE_BYTES_BE);
const PixelFormat PixelFormat::PixelFormat_i4i(4, PixelLayout::INTERLEAVED, StorageUnitFormat::DOUBLE_BYTES_BE);
const PixelFormat PixelFormat::PixelFormat_i8i(8, PixelLayout::INTERLEAVED, StorageUnitFormat::DOUBLE_BYTES_BE);
const PixelFormat PixelFormat::PixelFormat_i8p(8, PixelLayout::PROGRESSIVE, StorageUnitFormat::SINGLE_BYTE);
const PixelFormat PixelFormat::PixelFormat_rgb332(3, 3, 2, StorageUnitFormat::SINGLE_BYTE, false);
const PixelFormat PixelFormat::PixelFormat_rgb444c(4, 4, 4, StorageUnitFormat::SINGLE_BYTE, true);
const PixelFormat PixelFormat::PixelFormat_rgb555(5, 5, 5, StorageUnitFormat::DOUBLE_BYTES_BE, false);
const PixelFormat PixelFormat::PixelFormat_rgb565(5, 6, 5, StorageUnitFormat::DOUBLE_BYTES_BE, false);
const PixelFormat PixelFormat::PixelFormat_rgb666(6, 6, 6, StorageUnitFormat::TRIPLE_BYTES_BE, false);
const PixelFormat PixelFormat::PixelFormat_rgb888(8, 8, 8, StorageUnitFormat::TRIPLE_BYTES_BE, false);

// ======== Code implementation ========
