/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

///////////6.1 Add sum_log and sum_printk header files//////////
#if defined(CONFIG_SUM_PRINT)
#include "sum_printk.h"
#elif defined(CONFIG_SUM_LOG)
#include "sum_log.h"
#else
#error "A sum implementation must be selected"
#endif

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   2000

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led5180)

//////////5.2 Poll button to toggle LED2////////////////////////
//#define BUTTON_NODE DT_ALIAS(sw0)
#define POLL_TIME_MS 20

/////////5.3 Add custom button5180 devicetree alias////////////
#define BUTTON_NODE DT_ALIAS(button5180)



/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec button =GPIO_DT_SPEC_GET(BUTTON_NODE, gpios);

int main(void)
{
	/////////6.1 Add sum_log and sum_printk calls/////////////////
	#if defined(CONFIG_SUM_PRINT)
		int sum_result = sum_printk(10, 20);
	#elif defined(CONFIG_SUM_LOG)
		int sum_result = sum_log(10, 20);
	#endif
		(void)sum_result;
		
	int ret;
	bool led_state = true;

	bool previous_button_state = false;

	// if (!gpio_is_ready_dt(&led)) {
	// 	return 0;
	// }

	if (!gpio_is_ready_dt(&led) ||
	    !gpio_is_ready_dt(&button)) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
	if (ret < 0) {
		return 0;
	}

	while (1) {
		//ret = gpio_pin_toggle_dt(&led);
		// if (ret < 0) {
		// 	return 0;
		// }
		// led_state = !led_state;
		// printf("LED state: %s\n", led_state ? "ON" : "OFF");
		// k_msleep(SLEEP_TIME_MS);

		///////////////5.2 Poll button to toggle LED2////////////////////////
		int button_state = gpio_pin_get_dt(&button);
		if (button_state < 0) {
			return 0;
		}

		/* Only toggle once when the button changes from released to pressed. */
		if (button_state && !previous_button_state) {
			led_state = !led_state;

			ret = gpio_pin_set_dt(&led, led_state);
			if (ret < 0) {
				return 0;
			}

			printf("LED state: %s\n",
			       led_state ? "ON" : "OFF");
		}

		previous_button_state = button_state;
		k_msleep(POLL_TIME_MS);
		// ///////////////////////////////////////////////////////////////////
	}
	return 0;
}
