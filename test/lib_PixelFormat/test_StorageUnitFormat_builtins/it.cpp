#include "StorageUnit.hpp"
#include <unity.h>

/**
 * @brief Before test
 */
void setUp(void) {}

/**
 * @brief After test.
 */
void tearDown(void) {}

void __SINGLE_BYTE() {
    TEST_ASSERT_TRUE((StorageUnitFormat::SINGLE_BYTE.getEndianness() == StorageEndianness::big_endian));
    TEST_ASSERT_TRUE((StorageUnitFormat::SINGLE_BYTE.getWidth() == 8));
}

void __DOUBLE_BYTES_BE() {
    TEST_ASSERT_TRUE((StorageUnitFormat::DOUBLE_BYTES_BE.getEndianness() == StorageEndianness::big_endian));
    TEST_ASSERT_TRUE((StorageUnitFormat::DOUBLE_BYTES_BE.getWidth() == 16));
}

void __TRIPLE_BYTES_BE() {
    TEST_ASSERT_TRUE((StorageUnitFormat::TRIPLE_BYTES_BE.getEndianness() == StorageEndianness::big_endian));
    TEST_ASSERT_TRUE((StorageUnitFormat::TRIPLE_BYTES_BE.getWidth() == 24));
}

void __QUADRUPLE_BYTES_BE() {
    TEST_ASSERT_TRUE((StorageUnitFormat::QUADRUPLE_BYTES_BE.getEndianness() == StorageEndianness::big_endian));
    TEST_ASSERT_TRUE((StorageUnitFormat::QUADRUPLE_BYTES_BE.getWidth() == 32));
}

int main(int argc, char **argv) {    
    UNITY_BEGIN();
    RUN_TEST(__SINGLE_BYTE);
    RUN_TEST(__DOUBLE_BYTES_BE);
    RUN_TEST(__TRIPLE_BYTES_BE);
    RUN_TEST(__QUADRUPLE_BYTES_BE);
    UNITY_END();
}
