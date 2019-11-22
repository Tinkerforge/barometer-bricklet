/* barometer-bricklet
 * Copyright (C) 2012-2013 Olaf Lüke <olaf@tinkerforge.com>
 *
 * config.h: Barometer Bricklet specific configuration
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include <stdbool.h>

#include "bricklib/drivers/board/sam3s/SAM3S.h"

#include "barometer.h"

#define BOARD_MCK 64000000

#define BRICKLET_FIRMWARE_VERSION_MAJOR 2
#define BRICKLET_FIRMWARE_VERSION_MINOR 0
#define BRICKLET_FIRMWARE_VERSION_REVISION 2

#define BRICKLET_HARDWARE_VERSION_MAJOR 1
#define BRICKLET_HARDWARE_VERSION_MINOR 0
#define BRICKLET_HARDWARE_VERSION_REVISION 0

#define BRICKLET_DEVICE_IDENTIFIER 221

#define LOGGING_LEVEL LOGGING_NONE
#define DEBUG_BRICKLET 0

#define BRICKLET_HAS_SIMPLE_SENSOR
#define BRICKLET_VALUE_APPLIED_OUTSIDE
#define INVOCATION_IN_BRICKLET_CODE
#define NUM_SIMPLE_VALUES 2

#define NUM_D1_AVERAGE_MAX 10
#define NUM_D2_AVERAGE_MAX 255
#define NUM_D1_MOVING_AVERAGE_MAX 25

#define NUM_D1_AVERAGE_DEFAULT 10
#define NUM_D2_AVERAGE_DEFAULT 10
#define NUM_D1_MOVING_AVERAGE_DEFAULT 25

#define ALTITUDE_INTERPOLATION_PRECISION 8

#define REFERENCE_AIR_PRESSURE 1013250
#define MIN_AIR_PRESSURE 10000
#define MAX_AIR_PRESSURE 1200000
#define MIN_TEMPERATURE (-4000)
#define MAX_TEMPERATURE 8500

typedef struct {
	int32_t  value[NUM_SIMPLE_VALUES];
	int32_t  last_value[NUM_SIMPLE_VALUES];

	uint32_t signal_period[NUM_SIMPLE_VALUES];
	uint32_t signal_period_counter[NUM_SIMPLE_VALUES];

	uint32_t threshold_debounce;
	uint32_t threshold_period_current[NUM_SIMPLE_VALUES];
	int32_t  threshold_min[NUM_SIMPLE_VALUES];
	int32_t  threshold_max[NUM_SIMPLE_VALUES];
	char     threshold_option[NUM_SIMPLE_VALUES];

	int32_t  threshold_min_save[NUM_SIMPLE_VALUES];
	int32_t  threshold_max_save[NUM_SIMPLE_VALUES];
	char     threshold_option_save[NUM_SIMPLE_VALUES];

	uint32_t tick;

	uint16_t calibration[6];
	bool calibration_valid;
	uint8_t counter;
	int8_t pending_d;

	uint32_t d1_avg_sum;
	uint8_t d1_avg_tick;
	uint32_t d1_avg;

	uint32_t d1_moving_avg_history[NUM_D1_MOVING_AVERAGE_MAX];
	uint32_t d1_moving_avg_sum;
	uint8_t d1_moving_avg_tick;
	uint32_t d1_moving_avg;

	uint32_t d2_avg_sum;
	uint8_t d2_avg_tick;
	uint32_t d2_avg;

	int32_t air_pressure_ref; // hPa/1000

	int16_t temperature; // °C/100
	uint8_t num_moving_average;
	uint8_t num_average_d1;
	uint8_t num_average_d2;
} /*__attribute__((packed))*/ BrickContext;

#endif
