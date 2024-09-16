
// header include
#include "StorageUnit.hpp"

// write code here...

// ======== Builtin instanciation ========
const StorageUnitFormat StorageUnitFormat::SINGLE_BYTE(8, StorageEndianness::BIG_ENDIAN);
const StorageUnitFormat StorageUnitFormat::DOUBLE_BYTES_BE(16, StorageEndianness::BIG_ENDIAN);
const StorageUnitFormat StorageUnitFormat::TRIPLE_BYTES_BE(24, StorageEndianness::BIG_ENDIAN);
const StorageUnitFormat StorageUnitFormat::QUADRUPLE_BYTES_BE(32, StorageEndianness::BIG_ENDIAN);

// ======== Code implementation ========
