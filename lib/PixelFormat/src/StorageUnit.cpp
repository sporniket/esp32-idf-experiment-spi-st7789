
// header include
#include "StorageUnit.hpp"

// write code here...

StorageUnitFormat *StorageUnitFormat::getStorageUnitFormat(StorageUnitFormatEnum id) {
    switch (id) {
    case StorageUnitFormatEnum::SINGLE_BYTE:
        return new StorageUnitFormat(8, StorageEndianness::big_endian);
    case StorageUnitFormatEnum::ONE_AND_AN_HALF_BYTE_BE:
        return new StorageUnitFormat(12, StorageEndianness::big_endian);
    case StorageUnitFormatEnum::DOUBLE_BYTES_BE:
        return new StorageUnitFormat(16, StorageEndianness::big_endian);
    case StorageUnitFormatEnum::TRIPLE_BYTES_BE:
        return new StorageUnitFormat(24, StorageEndianness::big_endian);
    case StorageUnitFormatEnum::QUADRUPLE_BYTES_BE:
        return new StorageUnitFormat(32, StorageEndianness::big_endian);
    default:
        return new StorageUnitFormat(0, StorageEndianness::big_endian);
    }
}
