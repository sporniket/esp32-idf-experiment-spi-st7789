
// header include
#include "PixelFormat.hpp"

// write code here...

// ======== Builtin instanciation ========

// Indexed builtins formats
PixelFormat *PixelFormat::getPixelFormat(PixelFormatEnum id) {
    switch (id) {
    case PixelFormatEnum::I1I_8BE:
        return new PixelFormat(1, PixelLayout::INTERLEAVED, StorageUnitFormat::getStorageUnitFormat(StorageUnitFormatEnum::SINGLE_BYTE));
    case PixelFormatEnum::I1I_16BE:
        return new PixelFormat(1, PixelLayout::INTERLEAVED, StorageUnitFormat::getStorageUnitFormat(StorageUnitFormatEnum::DOUBLE_BYTES_BE));
    case PixelFormatEnum::I2I_16BE:
        return new PixelFormat(2, PixelLayout::INTERLEAVED, StorageUnitFormat::getStorageUnitFormat(StorageUnitFormatEnum::DOUBLE_BYTES_BE));
    case PixelFormatEnum::I4I_16BE:
        return new PixelFormat(4, PixelLayout::INTERLEAVED, StorageUnitFormat::getStorageUnitFormat(StorageUnitFormatEnum::DOUBLE_BYTES_BE));
    case PixelFormatEnum::I8I_16BE:
        return new PixelFormat(8, PixelLayout::INTERLEAVED, StorageUnitFormat::getStorageUnitFormat(StorageUnitFormatEnum::DOUBLE_BYTES_BE));
    case PixelFormatEnum::I8P_8BE:
        return new PixelFormat(8, PixelLayout::PROGRESSIVE, StorageUnitFormat::getStorageUnitFormat(StorageUnitFormatEnum::SINGLE_BYTE));
    case PixelFormatEnum::RGB332_8BE:
        return new PixelFormat(3, 3, 2, StorageUnitFormat::getStorageUnitFormat(StorageUnitFormatEnum::SINGLE_BYTE), false);
    case PixelFormatEnum::RGB444C_12BE:
        return new PixelFormat(4, 4, 4, StorageUnitFormat::getStorageUnitFormat(StorageUnitFormatEnum::ONE_AND_AN_HALF_BYTE_BE), true);
    case PixelFormatEnum::RGB555_16BE:
        return new PixelFormat(5, 5, 5, StorageUnitFormat::getStorageUnitFormat(StorageUnitFormatEnum::DOUBLE_BYTES_BE), false);
    case PixelFormatEnum::RGB565_16BE:
        return new PixelFormat(5, 6, 5, StorageUnitFormat::getStorageUnitFormat(StorageUnitFormatEnum::DOUBLE_BYTES_BE), false);
    case PixelFormatEnum::RGB666_24BE:
        return new PixelFormat(6, 6, 6, StorageUnitFormat::getStorageUnitFormat(StorageUnitFormatEnum::TRIPLE_BYTES_BE), false);
    case PixelFormatEnum::RGB888_24BE:
        return new PixelFormat(8, 8, 8, StorageUnitFormat::getStorageUnitFormat(StorageUnitFormatEnum::TRIPLE_BYTES_BE), false);
    default:
        return new PixelFormat(0, PixelLayout::INTERLEAVED, StorageUnitFormat::getStorageUnitFormat(StorageUnitFormatEnum::SINGLE_BYTE));
    }
}

// ======== Code implementation ========
