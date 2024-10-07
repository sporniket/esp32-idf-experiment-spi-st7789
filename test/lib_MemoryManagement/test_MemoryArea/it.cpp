#include "MemoryArea.hpp"
#include <unity.h>

/**
 * @brief Before test
 */
void setUp(void) {}

/**
 * @brief After test.
 */
void tearDown(void) {}

void __hasExpectedProperties() {
    uint8_t buffer[10];
    MemoryArea memoryArea(buffer, sizeof(buffer));
    TEST_ASSERT_TRUE((memoryArea.getStart() == buffer));
    TEST_ASSERT_TRUE((memoryArea.getLength() == sizeof(buffer)));
    TEST_ASSERT_TRUE((memoryArea.getEnd() == (buffer + sizeof(buffer))));
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(__hasExpectedProperties);
    UNITY_END();
}
