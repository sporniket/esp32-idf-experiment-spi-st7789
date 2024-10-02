
// header include
#include "StorageUnit.hpp"

// write code here...

// ======== Builtin instanciation ========
const StorageUnitFormat StorageUnitFormat::SINGLE_BYTE(8, StorageEndianness::big_endian);
const StorageUnitFormat StorageUnitFormat::DOUBLE_BYTES_BE(16, StorageEndianness::big_endian);
const StorageUnitFormat StorageUnitFormat::TRIPLE_BYTES_BE(24, StorageEndianness::big_endian);
const StorageUnitFormat StorageUnitFormat::QUADRUPLE_BYTES_BE(32, StorageEndianness::big_endian);

// ======== Code implementation ========
