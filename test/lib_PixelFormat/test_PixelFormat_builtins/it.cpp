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

void __I1I_8BE() {
    TEST_ASSERT_TRUE((PixelFormat::I1I_8BE.getType() == PixelType::INDEXED));
    const IndexedFormat& f = std::get<IndexedFormat>(PixelFormat::I1I_8BE.getFormat());
    TEST_ASSERT_TRUE((f.getIndexWidth() == 1));
    TEST_ASSERT_TRUE((PixelFormat::I1I_8BE.getLayout() == PixelLayout::INTERLEAVED));
    //TEST_ASSERT_TRUE((PixelFormat::I1I_8BE.getStorage() == StorageUnitFormat::SINGLE_BYTE));
}


int main(int argc, char **argv) {    
    UNITY_BEGIN();
    RUN_TEST(__I1I_8BE);
    UNITY_END();
}
