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
    TEST_ASSERT_TRUE((StorageUnitFormat::SINGLE_BYTE == StorageUnitFormat::SINGLE_BYTE));
    TEST_ASSERT_TRUE((StorageUnitFormat::SINGLE_BYTE == StorageUnitFormat(8, StorageEndianness::big_endian)));
    StorageUnitFormat f(StorageUnitFormat::SINGLE_BYTE);
    TEST_ASSERT_TRUE((f == StorageUnitFormat::SINGLE_BYTE));
    class foo {
        public:
        foo(uint8_t value, StorageUnitFormat storage) : storage(storage), value(value) {}
        const StorageUnitFormat getStorage() const { return storage; }
        const uint8_t getValue() const { return value; }

        private:
        const StorageUnitFormat storage;
        const uint8_t value;
    };
    foo bar(3, StorageUnitFormat::SINGLE_BYTE);
    foo bar2(4, StorageUnitFormat::SINGLE_BYTE);
    TEST_ASSERT_TRUE((bar.getStorage() == StorageUnitFormat::SINGLE_BYTE));
    TEST_ASSERT_TRUE((bar.getStorage().getEndianness() == StorageEndianness::big_endian));
    TEST_ASSERT_TRUE((bar.getStorage().getWidth() == 8));
    TEST_ASSERT_TRUE((bar2.getStorage() == StorageUnitFormat::SINGLE_BYTE));
    TEST_ASSERT_TRUE((bar2.getStorage().getEndianness() == StorageEndianness::big_endian));
    TEST_ASSERT_TRUE((bar2.getStorage().getWidth() == 8));
}

void __DOUBLE_BYTES_BE() {
    TEST_ASSERT_TRUE((StorageUnitFormat::DOUBLE_BYTES_BE.getEndianness() == StorageEndianness::big_endian));
    TEST_ASSERT_TRUE((StorageUnitFormat::DOUBLE_BYTES_BE.getWidth() == 16));
    TEST_ASSERT_TRUE((StorageUnitFormat::DOUBLE_BYTES_BE == StorageUnitFormat::DOUBLE_BYTES_BE));
    TEST_ASSERT_TRUE((StorageUnitFormat::DOUBLE_BYTES_BE == StorageUnitFormat(16, StorageEndianness::big_endian)));
}

void __TRIPLE_BYTES_BE() {
    TEST_ASSERT_TRUE((StorageUnitFormat::TRIPLE_BYTES_BE.getEndianness() == StorageEndianness::big_endian));
    TEST_ASSERT_TRUE((StorageUnitFormat::TRIPLE_BYTES_BE.getWidth() == 24));
    TEST_ASSERT_TRUE((StorageUnitFormat::TRIPLE_BYTES_BE == StorageUnitFormat::TRIPLE_BYTES_BE));
    TEST_ASSERT_TRUE((StorageUnitFormat::TRIPLE_BYTES_BE == StorageUnitFormat(24, StorageEndianness::big_endian)));
}

void __QUADRUPLE_BYTES_BE() {
    TEST_ASSERT_TRUE((StorageUnitFormat::QUADRUPLE_BYTES_BE.getEndianness() == StorageEndianness::big_endian));
    TEST_ASSERT_TRUE((StorageUnitFormat::QUADRUPLE_BYTES_BE.getWidth() == 32));
    TEST_ASSERT_TRUE((StorageUnitFormat::QUADRUPLE_BYTES_BE == StorageUnitFormat::QUADRUPLE_BYTES_BE));
    TEST_ASSERT_TRUE((StorageUnitFormat::QUADRUPLE_BYTES_BE == StorageUnitFormat(32, StorageEndianness::big_endian)));
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(__SINGLE_BYTE);
    RUN_TEST(__DOUBLE_BYTES_BE);
    RUN_TEST(__TRIPLE_BYTES_BE);
    RUN_TEST(__QUADRUPLE_BYTES_BE);
    UNITY_END();
}
