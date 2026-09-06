#include <zephyr/ztest.h>
#include "sum_log.h"

ZTEST(sum_log_test_suite, test_sum_log_basic)
{
    int result = sum_log(10, 20);

    zassert_equal(result, 30,
                  "Expected 30, but got %d", result);
}

ZTEST(sum_log_test_suite, test_sum_log_negative)
{
    int result = sum_log(-10, -20);

    zassert_equal(result, -30,
                  "Expected -30, but got %d", result);
}

ZTEST(sum_log_test_suite, test_sum_log_zero)
{
    int result = sum_log(0, 0);

    zassert_equal(result, 0,
                  "Expected 0, but got %d", result);
}

ZTEST_SUITE(sum_log_test_suite, NULL, NULL, NULL, NULL, NULL);