
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

void test_ByteArrayBigEndianOutputStream__can_stream_values() {
    uint8_t bufferRaw[5] MemoryArea buffer(buffer, sizeof(buffer))

            ByteArrayBigEndianOutputStream ostream(buffer);
    uint8_t one = 1;
    uint16_t two = 2;
    uint32_t three = 3;

    ostream << one << two;
    TEST_ASSERT_EQUAL_INT(ostream.status(), ByteArrayStreamStatus::E_OK);
    TEST_ASSERT_EQUAL_INT(ostream.length(), 3);
    TEST_ASSERT_EQUAL_INT(1, bufferRaw[0]);
    TEST_ASSERT_EQUAL_INT(0, bufferRaw[1]);
    TEST_ASSERT_EQUAL_INT(2, bufferRaw[2]);

    ostream.reset();
    ostream << three;
    TEST_ASSERT_EQUAL_INT(ostream.status(), ByteArrayStreamStatus::E_OK);
    TEST_ASSERT_EQUAL_INT(ostream.length(), 4);
    TEST_ASSERT_EQUAL_INT(0, bufferRaw[0]);
    TEST_ASSERT_EQUAL_INT(0, bufferRaw[1]);
    TEST_ASSERT_EQUAL_INT(0, bufferRaw[2]);
    TEST_ASSERT_EQUAL_INT(3, bufferRaw[2]);
}

void test_ByteArrayBigEndianOutputStream__is_in_error_when_pushing_more_than_it_can_store() {
    uint8_t bufferRaw[5] MemoryArea buffer(buffer, sizeof(buffer))

            ByteArrayBigEndianOutputStream ostream(buffer);
    uint8_t one = 1;
    uint16_t two = 2;
    uint32_t three = 3;

    ostream << one << two << three;
    TEST_ASSERT_EQUAL_INT(ostream.status(), ByteArrayStreamStatus::E_KO__OUT_OF_BOUND);
    TEST_ASSERT_EQUAL_INT(ostream.length(), 3);
}

void test_ByteArrayBigEndianOutputStream_reset__goes_back_to_the_start() {
    uint8_t bufferRaw[5] MemoryArea buffer(buffer, sizeof(buffer))

            ByteArrayBigEndianOutputStream ostream(buffer);
    uint8_t one = 1;
    uint16_t two = 2;

    ostream << one << two;
    ostream.reset();
    TEST_ASSERT_EQUAL_INT(ostream.status(), ByteArrayStreamStatus::E_OK);
    TEST_ASSERT_EQUAL_INT(ostream.length(), 0);
}

void test_ByteArrayBigEndianOutputStream_reset__reinstate_ok_state() {
    uint8_t bufferRaw[5] MemoryArea buffer(buffer, sizeof(buffer))

            ByteArrayBigEndianOutputStream ostream(buffer);
    uint8_t one = 1;
    uint16_t two = 2;
    uint32_t three = 3;

    ostream << one << two << three;
    ostream.reset();
    TEST_ASSERT_EQUAL_INT(ostream.status(), ByteArrayStreamStatus::E_OK);
    TEST_ASSERT_EQUAL_INT(ostream.length(), 0);
}
