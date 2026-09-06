#include <zephyr/devicetree.h>
#include <zephyr/ztest.h>

#include "bme280_direct.h"

#define BME280_TEST_NODE DT_NODELABEL(bme280_i2c)

ZTEST(bme280_test_suite, test_devicetree_configuration)
{
	zassert_true(
		DT_NODE_EXISTS(BME280_TEST_NODE),
		"BME280 Devicetree node does not exist");

#if DT_NODE_EXISTS(BME280_TEST_NODE)
	zassert_true(
		DT_NODE_HAS_STATUS(BME280_TEST_NODE, okay),
		"BME280 node is not enabled");

	zassert_equal(
		DT_REG_ADDR(BME280_TEST_NODE),
		0x77,
		"Expected address 0x77");
#endif
}

ZTEST(bme280_test_suite, test_temperature_datasheet_example)
{
	struct bme280_temp_calib calib = {
		.dig_t1 = 27504,
		.dig_t2 = 26435,
		.dig_t3 = -1000
	};

	int32_t temperature =
		bme280_direct_compensate_temperature(
			519888, &calib);

	zassert_equal(
		temperature,
		2508,
		"Expected 25.08 C, got %d",
		temperature);
}

ZTEST(bme280_test_suite, test_temperature_sanity)
{
	struct bme280_temp_calib calib = {
		.dig_t1 = 27504,
		.dig_t2 = 26435,
		.dig_t3 = -1000
	};

	int32_t lower_temperature =
		bme280_direct_compensate_temperature(
			480000, &calib);

	int32_t higher_temperature =
		bme280_direct_compensate_temperature(
			550000, &calib);

	zassert_true(
		higher_temperature > lower_temperature,
		"Higher ADC input should produce higher temperature");

	zassert_true(
		lower_temperature > -4000 &&
		higher_temperature < 8500,
		"Temperature is outside the BME280 range");
}

ZTEST_SUITE(bme280_test_suite, NULL, NULL, NULL, NULL, NULL);