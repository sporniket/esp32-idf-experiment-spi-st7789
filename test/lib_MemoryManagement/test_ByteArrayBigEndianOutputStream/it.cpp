
#include "ByteArrayBigEndianOutputStream.hpp"
#include <unity.h>

/**
 * @brief Before test
 */
void setUp(void) {}

/**
 * @brief After test.
 */
void tearDown(void) {}

uint8_t bufferRaw[5];
MemoryArea buffer(bufferRaw, sizeof(bufferRaw));

uint8_t one = 1;
uint16_t two = 2;
uint32_t three = 3;

void __can_stream_values_up_to_buffer_length() {
    ByteArrayBigEndianOutputStream ostream(buffer);

    ostream << one << two;
    TEST_ASSERT_TRUE((ostream.getStatus() == ByteArrayStreamStatus::E_OK));
    TEST_ASSERT_TRUE((ostream.getLength() == 3));
    TEST_ASSERT_TRUE((1 == bufferRaw[0]));
    TEST_ASSERT_TRUE((0 == bufferRaw[1]));
    TEST_ASSERT_TRUE((2 == bufferRaw[2]));

    ostream.reset();
    ostream << three << one;
    TEST_ASSERT_TRUE((ostream.getStatus() == ByteArrayStreamStatus::E_OK));
    TEST_ASSERT_TRUE((ostream.getLength() == 5));
    TEST_ASSERT_TRUE((0 == bufferRaw[0]));
    TEST_ASSERT_TRUE((0 == bufferRaw[1]));
    TEST_ASSERT_TRUE((0 == bufferRaw[2]));
    TEST_ASSERT_TRUE((3 == bufferRaw[3]));
    TEST_ASSERT_TRUE((1 == bufferRaw[4]));
}

void __is_in_error_when_pushing_more_than_it_can_store() {
    ByteArrayBigEndianOutputStream ostream(buffer);
    
    ostream << three << one << one;
    TEST_ASSERT_TRUE((ostream.getStatus() == ByteArrayStreamStatus::E_KO__OUT_OF_BOUND));
    TEST_ASSERT_TRUE((ostream.getLength() == 5));
    TEST_ASSERT_TRUE((0 == bufferRaw[0]));
    TEST_ASSERT_TRUE((0 == bufferRaw[1]));
    TEST_ASSERT_TRUE((0 == bufferRaw[2]));
    TEST_ASSERT_TRUE((3 == bufferRaw[3]));
    TEST_ASSERT_TRUE((1 == bufferRaw[4]));

    ostream.reset();
    ostream << one << two << three;
    TEST_ASSERT_TRUE((ostream.getStatus() == ByteArrayStreamStatus::E_KO__OUT_OF_BOUND));
    TEST_ASSERT_TRUE((ostream.getLength() == 3));
    TEST_ASSERT_TRUE((1 == bufferRaw[0]));
    TEST_ASSERT_TRUE((0 == bufferRaw[1]));
    TEST_ASSERT_TRUE((2 == bufferRaw[2]));
    TEST_ASSERT_TRUE((3 == bufferRaw[3]));
    TEST_ASSERT_TRUE((1 == bufferRaw[4]));
}

void reset__goes_back_to_the_start() {
    ByteArrayBigEndianOutputStream ostream(buffer);
    
    ostream << one << two;
    ostream.reset();
    TEST_ASSERT_TRUE((ostream.getStatus() == ByteArrayStreamStatus::E_OK));
    TEST_ASSERT_TRUE((ostream.getLength() == 0));
    TEST_ASSERT_TRUE((1 == bufferRaw[0]));
    TEST_ASSERT_TRUE((0 == bufferRaw[1]));
    TEST_ASSERT_TRUE((2 == bufferRaw[2]));

    ostream << two << one;
    TEST_ASSERT_TRUE((ostream.getStatus() == ByteArrayStreamStatus::E_OK));
    TEST_ASSERT_TRUE((ostream.getLength() == 3));
    TEST_ASSERT_TRUE((0 == bufferRaw[0]));
    TEST_ASSERT_TRUE((2 == bufferRaw[1]));
    TEST_ASSERT_TRUE((1 == bufferRaw[2]));

}

void reset__reinstate_ok_state() {
    ByteArrayBigEndianOutputStream ostream(buffer);
    
    ostream << one << two << three;
    ostream.reset();
    TEST_ASSERT_TRUE((ostream.getStatus() == ByteArrayStreamStatus::E_OK));
    TEST_ASSERT_TRUE((ostream.getLength() == 0));
}

int main(int argc, char **argv) {
    ByteArrayBigEndianOutputStream ostream(buffer);
    
    UNITY_BEGIN();
    RUN_TEST(__can_stream_values_up_to_buffer_length);
    RUN_TEST(__is_in_error_when_pushing_more_than_it_can_store);
    RUN_TEST(reset__goes_back_to_the_start);
    RUN_TEST(reset__reinstate_ok_state);
    UNITY_END();
}
