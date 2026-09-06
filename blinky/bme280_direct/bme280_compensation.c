#include "bme280_direct.h"

int32_t bme280_direct_compensate_temperature(
	int32_t adc_temperature,
	const struct bme280_temp_calib *calib)
{
	int32_t var1;
	int32_t var2;
	int32_t difference;
	int32_t t_fine;

	var1 =
		((((adc_temperature >> 3) -
		   ((int32_t)calib->dig_t1 << 1))) *
		 (int32_t)calib->dig_t2) >> 11;

	difference =
		(adc_temperature >> 4) -
		(int32_t)calib->dig_t1;

	var2 =
		((((difference * difference) >> 12) *
		  (int32_t)calib->dig_t3) >> 14);

	t_fine = var1 + var2;

	/* Temperature in hundredths of a degree Celsius. */
	return (t_fine * 5 + 128) >> 8;
}
