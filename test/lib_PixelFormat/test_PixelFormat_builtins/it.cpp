#include <unity.h>
#include "PixelFormat.hpp"

/**
 * @brief Before test
 */
void setUp(void) {}

/**
 * @brief After test.
 */
void tearDown(void) {}

void instance__should_return_expected_properties() {
    const PixelFormat bar(1, PixelLayout::INTERLEAVED, StorageUnitFormat::SINGLE_BYTE);
    TEST_ASSERT_TRUE((bar.getStorage() == StorageUnitFormat::SINGLE_BYTE));
    TEST_ASSERT_TRUE((bar.getLayout() == PixelLayout::INTERLEAVED));
    TEST_ASSERT_TRUE((bar.getType() == PixelType::INDEXED));
    const IndexedFormat &f = std::get<IndexedFormat>(bar.getFormat());
    TEST_ASSERT_TRUE((f.getIndexWidth() == 1));
    TEST_ASSERT_TRUE((bar == PixelFormat::I1I_8BE));
}

void rgb_instance__should_find_equality() {
    const PixelFormat bar(8,8,8,StorageUnitFormat::TRIPLE_BYTES_BE,false) ;
    const PixelFormat &foo = bar;
    TEST_ASSERT_TRUE((bar.getStorage() == StorageUnitFormat::TRIPLE_BYTES_BE));
    TEST_ASSERT_TRUE((bar.getLayout() == PixelLayout::PROGRESSIVE));
    TEST_ASSERT_TRUE((bar.getType() == PixelType::RGB));
    const RGBFormat &f = std::get<RGBFormat>(bar.getFormat());
    TEST_ASSERT_TRUE((f.getRedWidth() == 8));
    TEST_ASSERT_TRUE((f.getGreenWidth() == 8));
    TEST_ASSERT_TRUE((f.getBlueWidth() == 8));
    TEST_ASSERT_TRUE((foo == PixelFormat::RGB888_24BE));
}

void __I1I_8BE() {
    TEST_ASSERT_TRUE((PixelFormat::I1I_8BE.getLayout() == PixelLayout::INTERLEAVED));
    TEST_ASSERT_TRUE((PixelFormat::I1I_8BE.getStorage() == StorageUnitFormat::SINGLE_BYTE));
    TEST_ASSERT_TRUE((PixelFormat::I1I_8BE.getType() == PixelType::INDEXED));
    const IndexedFormat &f = std::get<IndexedFormat>(PixelFormat::I1I_8BE.getFormat());
    TEST_ASSERT_TRUE((f.getIndexWidth() == 1));
}

void __I1I_16BE() {
    TEST_ASSERT_TRUE((PixelFormat::I1I_16BE.getLayout() == PixelLayout::INTERLEAVED));
    TEST_ASSERT_TRUE((PixelFormat::I1I_16BE.getStorage() == StorageUnitFormat::DOUBLE_BYTES_BE));
    TEST_ASSERT_TRUE((PixelFormat::I1I_16BE.getType() == PixelType::INDEXED));
    const IndexedFormat &f = std::get<IndexedFormat>(PixelFormat::I1I_16BE.getFormat());
    TEST_ASSERT_TRUE((f.getIndexWidth() == 1));
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(instance__should_return_expected_properties);
    RUN_TEST(rgb_instance__should_find_equality);
    RUN_TEST(__I1I_8BE);
    RUN_TEST(__I1I_16BE);
    UNITY_END();
}
