#include "StorageUnit.hpp"
#include <unity.h>
#include <memory>

/**
 * @brief Before test
 */
void setUp(void) {}

/**
 * @brief After test.
 */
void tearDown(void) {}

void __SINGLE_BYTE() {
    std::unique_ptr<StorageUnitFormat> f {StorageUnitFormat::getStorageUnitFormat(StorageUnitFormatEnum::SINGLE_BYTE)} ;
    TEST_ASSERT_TRUE((f->getEndianness() == StorageEndianness::big_endian));
    TEST_ASSERT_TRUE((f->getWidth() == 8));
    TEST_ASSERT_TRUE((*f == StorageUnitFormat(8, StorageEndianness::big_endian)));
}

void __ONE_AND_AN_HALF_BYTE_BE() {
    std::unique_ptr<StorageUnitFormat> f {StorageUnitFormat::getStorageUnitFormat(StorageUnitFormatEnum::ONE_AND_AN_HALF_BYTE_BE)} ;
    TEST_ASSERT_TRUE((f->getEndianness() == StorageEndianness::big_endian));
    TEST_ASSERT_TRUE((f->getWidth() == 12));
    TEST_ASSERT_TRUE((*f == StorageUnitFormat(12, StorageEndianness::big_endian)));
}

void __DOUBLE_BYTES_BE() {
    std::unique_ptr<StorageUnitFormat> f {StorageUnitFormat::getStorageUnitFormat(StorageUnitFormatEnum::DOUBLE_BYTES_BE)} ;
    TEST_ASSERT_TRUE((f->getEndianness() == StorageEndianness::big_endian));
    TEST_ASSERT_TRUE((f->getWidth() == 16));
    TEST_ASSERT_TRUE((*f == StorageUnitFormat(16, StorageEndianness::big_endian)));
}

void __TRIPLE_BYTES_BE() {
    std::unique_ptr<StorageUnitFormat> f {StorageUnitFormat::getStorageUnitFormat(StorageUnitFormatEnum::TRIPLE_BYTES_BE)} ;
    TEST_ASSERT_TRUE((f->getEndianness() == StorageEndianness::big_endian));
    TEST_ASSERT_TRUE((f->getWidth() == 24));
    TEST_ASSERT_TRUE((*f == StorageUnitFormat(24, StorageEndianness::big_endian)));
}

void __QUADRUPLE_BYTES_BE() {
    std::unique_ptr<StorageUnitFormat> f {StorageUnitFormat::getStorageUnitFormat(StorageUnitFormatEnum::QUADRUPLE_BYTES_BE)} ;
    TEST_ASSERT_TRUE((f->getEndianness() == StorageEndianness::big_endian));
    TEST_ASSERT_TRUE((f->getWidth() == 32));
    TEST_ASSERT_TRUE((*f == StorageUnitFormat(32, StorageEndianness::big_endian)));
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(__SINGLE_BYTE);
    RUN_TEST(__ONE_AND_AN_HALF_BYTE_BE);
    RUN_TEST(__DOUBLE_BYTES_BE);
    RUN_TEST(__TRIPLE_BYTES_BE);
    RUN_TEST(__QUADRUPLE_BYTES_BE);
    UNITY_END();
}
