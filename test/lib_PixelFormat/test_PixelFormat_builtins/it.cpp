#include "PixelFormat.hpp"
#include <unity.h>

/**
 * @brief Before test
 */
void setUp(void) {}

/**
 * @brief After test.
 */
void tearDown(void) {}

void indexed_instance__should_have_expected_properties_and_fulfill_equality() {
    const PixelFormat bar(1, PixelLayout::INTERLEAVED,
                          StorageUnitFormat::getStorageUnitFormat(StorageUnitFormatEnum::SINGLE_BYTE));
    TEST_ASSERT_TRUE((bar.getStorage()->getEndianness() == StorageEndianness::big_endian));
    TEST_ASSERT_TRUE((bar.getStorage()->getWidth() == 8));
    TEST_ASSERT_TRUE((bar.getLayout() == PixelLayout::INTERLEAVED));
    TEST_ASSERT_TRUE((bar.getType() == PixelType::INDEXED));
    const IndexedFormat &f = std::get<IndexedFormat>(bar.getFormat());
    TEST_ASSERT_TRUE((f.getIndexWidth() == 1));
    const PixelFormat foo(1, PixelLayout::INTERLEAVED,
                          StorageUnitFormat::getStorageUnitFormat(StorageUnitFormatEnum::SINGLE_BYTE));
    TEST_ASSERT_TRUE((bar == foo));
}

void rgb_instance__should_have_expected_properties_and_fulfill_equality() {
    const PixelFormat bar(8, 8, 8, StorageUnitFormat::getStorageUnitFormat(StorageUnitFormatEnum::TRIPLE_BYTES_BE),
                          false);
    const PixelFormat foo(8, 8, 8, StorageUnitFormat::getStorageUnitFormat(StorageUnitFormatEnum::TRIPLE_BYTES_BE),
                          false);
    TEST_ASSERT_TRUE((bar.getStorage()->getEndianness() == StorageEndianness::big_endian));
    TEST_ASSERT_TRUE((bar.getStorage()->getWidth() == 24));
    TEST_ASSERT_TRUE((bar.getLayout() == PixelLayout::PROGRESSIVE));
    TEST_ASSERT_TRUE((bar.getType() == PixelType::RGB));
    const RGBFormat &f = std::get<RGBFormat>(bar.getFormat());
    TEST_ASSERT_TRUE((f.getRedWidth() == 8));
    TEST_ASSERT_TRUE((f.getGreenWidth() == 8));
    TEST_ASSERT_TRUE((f.getBlueWidth() == 8));
    TEST_ASSERT_TRUE((bar == foo));
}

void getPixelFormat__I1I_8BE() {
    const std::unique_ptr<PixelFormat> bar{PixelFormat::getPixelFormat(PixelFormatEnum::I1I_8BE)};
    const PixelFormat foo(1, PixelLayout::INTERLEAVED,
                          StorageUnitFormat::getStorageUnitFormat(StorageUnitFormatEnum::SINGLE_BYTE));
    TEST_ASSERT_TRUE((*bar == foo));
}

void getPixelFormat__I1I_16BE() {
    const std::unique_ptr<PixelFormat> bar{PixelFormat::getPixelFormat(PixelFormatEnum::I1I_16BE)};
    const PixelFormat foo(1, PixelLayout::INTERLEAVED,
                          StorageUnitFormat::getStorageUnitFormat(StorageUnitFormatEnum::DOUBLE_BYTES_BE));
    TEST_ASSERT_TRUE((*bar == foo));
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(indexed_instance__should_have_expected_properties_and_fulfill_equality);
    RUN_TEST(rgb_instance__should_have_expected_properties_and_fulfill_equality);
    RUN_TEST(getPixelFormat__I1I_8BE);
    RUN_TEST(getPixelFormat__I1I_16BE);
    UNITY_END();
}
