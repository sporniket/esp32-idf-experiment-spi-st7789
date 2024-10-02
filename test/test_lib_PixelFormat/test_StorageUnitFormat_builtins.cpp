#include "StorageUnit.hpp"
#include <unity.h>
#include <bit>

/**
 * @brief Before test
 */
void setUp(void) {}

/**
 * @brief After test.
 */
void tearDown(void) {}

void test_StorageUnitFormat__SINGLE_BYTE() {
    TEST_ASSERT_TRUE((StorageUnitFormat::SINGLE_BYTE.getEndianness() == StorageEndianness::big_endian));
    TEST_ASSERT_TRUE((StorageUnitFormat::SINGLE_BYTE.getWidth() == 8));
}

void test_StorageUnitFormat__DOUBLE_BYTES_BE() {
    TEST_ASSERT_TRUE((StorageUnitFormat::DOUBLE_BYTES_BE.getEndianness() == StorageEndianness::big_endian));
    TEST_ASSERT_TRUE((StorageUnitFormat::DOUBLE_BYTES_BE.getWidth() == 16));
}

void test_StorageUnitFormat__TRIPLE_BYTES_BE() {
    TEST_ASSERT_TRUE((StorageUnitFormat::TRIPLE_BYTES_BE.getEndianness() == StorageEndianness::big_endian));
    TEST_ASSERT_TRUE((StorageUnitFormat::TRIPLE_BYTES_BE.getWidth() == 24));
}

void test_StorageUnitFormat__QUADRUPLE_BYTES_BE() {
    TEST_ASSERT_TRUE((StorageUnitFormat::QUADRUPLE_BYTES_BE.getEndianness() == StorageEndianness::big_endian));
    TEST_ASSERT_TRUE((StorageUnitFormat::QUADRUPLE_BYTES_BE.getWidth() == 32));
}

int main(int argc, char **argv) {    
    UNITY_BEGIN();
    RUN_TEST(test_StorageUnitFormat__SINGLE_BYTE);
    RUN_TEST(test_StorageUnitFormat__DOUBLE_BYTES_BE);
    RUN_TEST(test_StorageUnitFormat__TRIPLE_BYTES_BE);
    RUN_TEST(test_StorageUnitFormat__QUADRUPLE_BYTES_BE);
    UNITY_END();
}
