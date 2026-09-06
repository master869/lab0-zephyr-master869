#include <zephyr/logging/log.h>
#include "sum_log.h"

LOG_MODULE_REGISTER(sum_log, CONFIG_LOG_DEFAULT_LEVEL);

int sum_log(int a, int b)
{
    int result = a + b;
    int inputs[2] = {a, b};

    LOG_INF("Logger sum: %d + %d = %d", a, b, result);
    LOG_WRN("Example warning-level message");
    LOG_ERR("Example error-level message (demonstration only)");
    LOG_HEXDUMP_INF(inputs, sizeof(inputs), "Sum inputs");

    return result;
}