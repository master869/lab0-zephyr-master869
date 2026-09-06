#ifndef BME280_DIRECT_H_
#define BME280_DIRECT_H_

#include <stdint.h>

struct bme280_temp_calib {
	uint16_t dig_t1;
	int16_t dig_t2;
	int16_t dig_t3;
};

int bme280_direct_init(void);

int bme280_direct_read_temperature(int32_t *temperature_x100);

/* Public so it can be tested independently in Section 8.2. */
int32_t bme280_direct_compensate_temperature(
	int32_t adc_temperature,
	const struct bme280_temp_calib *calib);

#endif /* BME280_DIRECT_H_ */