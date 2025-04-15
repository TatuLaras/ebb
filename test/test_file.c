#include "unity.h"

void setUp() {}
void tearDown() {}

void test_case() {
    TEST_ASSERT(0);
}

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_case);

    return UNITY_END();
}
