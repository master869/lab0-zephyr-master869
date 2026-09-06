#include "bme280_direct.h"

#include <errno.h>
#include <stdbool.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(bme280_direct, LOG_LEVEL_INF);

#define BME280_NODE              DT_NODELABEL(bme280_i2c)

#define BME280_REG_CALIB_T1      0x88
#define BME280_REG_CHIP_ID       0xD0
#define BME280_REG_CTRL_MEAS     0xF4
#define BME280_REG_TEMP_MSB      0xFA

#define BME280_EXPECTED_CHIP_ID  0x60

/* Temperature oversampling x1, pressure skipped, normal mode. */
#define BME280_CTRL_MEAS_VALUE   0x23

static const struct i2c_dt_spec bme280 = I2C_DT_SPEC_GET(BME280_NODE);

static struct bme280_temp_calib calibration;
static bool initialized;

static int read_calibration(void)
{
	uint8_t data[6];
	int ret;

	ret = i2c_burst_read_dt(&bme280, BME280_REG_CALIB_T1,
				data, sizeof(data));
	if (ret < 0) {
		return ret;
	}

	calibration.dig_t1 =
		(uint16_t)data[0] |
		((uint16_t)data[1] << 8);

	calibration.dig_t2 =
		(int16_t)((uint16_t)data[2] |
		((uint16_t)data[3] << 8));

	calibration.dig_t3 =
		(int16_t)((uint16_t)data[4] |
		((uint16_t)data[5] << 8));

	return 0;
}


int bme280_direct_init(void)
{
	uint8_t chip_id;
	uint8_t chip_id_register = BME280_REG_CHIP_ID;
	uint8_t ctrl_meas_command[2] = {
		BME280_REG_CTRL_MEAS,
		BME280_CTRL_MEAS_VALUE
	};
	int ret;

	if (!i2c_is_ready_dt(&bme280)) {
		LOG_ERR("I2C bus is not ready");
		return -ENODEV;
	}

	ret = i2c_write_read_dt(&bme280,
				&chip_id_register,
				sizeof(chip_id_register),
				&chip_id,
				sizeof(chip_id));
	if (ret < 0) {
		LOG_ERR("Failed to read chip ID: %d", ret);
		return ret;
	}

	LOG_INF("BME280 chip ID: 0x%02x", chip_id);

	if (chip_id != BME280_EXPECTED_CHIP_ID) {
		LOG_ERR("Unexpected chip ID");
		return -ENODEV;
	}

	ret = read_calibration();
	if (ret < 0) {
		LOG_ERR("Failed to read calibration: %d", ret);
		return ret;
	}

	LOG_INF("Calibration: T1=%u, T2=%d, T3=%d",
		calibration.dig_t1,
		calibration.dig_t2,
		calibration.dig_t3);

	ret = i2c_write_dt(&bme280,
			   ctrl_meas_command,
			   sizeof(ctrl_meas_command));
	if (ret < 0) {
		LOG_ERR("Failed to start measurement: %d", ret);
		return ret;
	}

	k_msleep(10);
	initialized = true;

	return 0;
}

int bme280_direct_read_temperature(int32_t *temperature_x100)
{
	uint8_t data[3];
	int32_t adc_temperature;
	int ret;

	if (!initialized || temperature_x100 == NULL) {
		return -EINVAL;
	}

	ret = i2c_burst_read_dt(&bme280,
				BME280_REG_TEMP_MSB,
				data, sizeof(data));
	if (ret < 0) {
		return ret;
	}

	adc_temperature =
		((int32_t)data[0] << 12) |
		((int32_t)data[1] << 4) |
		((int32_t)data[2] >> 4);

	if (adc_temperature == 0x80000) {
		return -EIO;
	}

	*temperature_x100 =
		bme280_direct_compensate_temperature(
			adc_temperature, &calibration);

	return 0;
}