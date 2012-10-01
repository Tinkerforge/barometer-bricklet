/* barometer-bricklet
 * Copyright (C) 2012 Olaf Lüke <olaf@tinkerforge.com>
 *
 * barometer.c: Implementation of Barometer Bricklet messages
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

#include "barometer.h"

#include "brickletlib/bricklet_entry.h"
#include "bricklib/bricklet/bricklet_communication.h"
#include "bricklib/utility/util_definitions.h"
#include "config.h"

#define SIMPLE_UNIT_AIR_PRESSURE 0
#define SIMPLE_UNIT_ALTITUDE 1

const SimpleMessageProperty smp[] = {
	{SIMPLE_UNIT_AIR_PRESSURE, SIMPLE_TRANSFER_VALUE,     SIMPLE_DIRECTION_GET}, // TYPE_GET_AIR_PRESSURE
	{SIMPLE_UNIT_ALTITUDE,     SIMPLE_TRANSFER_VALUE,     SIMPLE_DIRECTION_GET}, // TYPE_GET_ALTITUDE
	{SIMPLE_UNIT_AIR_PRESSURE, SIMPLE_TRANSFER_PERIOD,    SIMPLE_DIRECTION_SET}, // TYPE_SET_AIR_PRESSURE_CALLBACK_PERIOD
	{SIMPLE_UNIT_AIR_PRESSURE, SIMPLE_TRANSFER_PERIOD,    SIMPLE_DIRECTION_GET}, // TYPE_GET_AIR_PRESSURE_CALLBACK_PERIOD
	{SIMPLE_UNIT_ALTITUDE,     SIMPLE_TRANSFER_PERIOD,    SIMPLE_DIRECTION_SET}, // TYPE_SET_ALTITUDE_CALLBACK_PERIOD
	{SIMPLE_UNIT_ALTITUDE,     SIMPLE_TRANSFER_PERIOD,    SIMPLE_DIRECTION_GET}, // TYPE_GET_ALTITUDE_CALLBACK_PERIOD
	{SIMPLE_UNIT_AIR_PRESSURE, SIMPLE_TRANSFER_THRESHOLD, SIMPLE_DIRECTION_SET}, // TYPE_SET_AIR_PRESSURE_CALLBACK_THRESHOLD
	{SIMPLE_UNIT_AIR_PRESSURE, SIMPLE_TRANSFER_THRESHOLD, SIMPLE_DIRECTION_GET}, // TYPE_GET_AIR_PRESSURE_CALLBACK_THRESHOLD
	{SIMPLE_UNIT_ALTITUDE,     SIMPLE_TRANSFER_THRESHOLD, SIMPLE_DIRECTION_SET}, // TYPE_SET_ALTITUDE_CALLBACK_THRESHOLD
	{SIMPLE_UNIT_ALTITUDE,     SIMPLE_TRANSFER_THRESHOLD, SIMPLE_DIRECTION_GET}, // TYPE_GET_ALTITUDE_CALLBACK_THRESHOLD
	{0, SIMPLE_TRANSFER_DEBOUNCE, SIMPLE_DIRECTION_SET}, // TYPE_SET_DEBOUNCE_PERIOD
	{0, SIMPLE_TRANSFER_DEBOUNCE, SIMPLE_DIRECTION_GET}, // TYPE_GET_DEBOUNCE_PERIOD
};

const SimpleUnitProperty sup[] = {
	{NULL, SIMPLE_SIGNEDNESS_INT, TYPE_AIR_PRESSURE, TYPE_AIR_PRESSURE_REACHED, SIMPLE_UNIT_AIR_PRESSURE}, // air pressure
	{NULL, SIMPLE_SIGNEDNESS_INT, TYPE_ALTITUDE,     TYPE_ALTITUDE_REACHED,     SIMPLE_UNIT_ALTITUDE}, // altitude
};

typedef struct {
	int32_t air_pressure; // mbar/1000
	int32_t factor; // altitude difference in mm per mbar/1000 air pressure difference
} __attribute__((__packed__)) AltitudeFactor;

const AltitudeFactor altitude_factors[] = {
	{1225005, 14},
	{1118311, 13},
	{1012049, 12},
	{ 919654, 11},
	{ 808871, 10},
	{ 710051,  9},
	{ 614001,  8},
	{ 520296,  7}
};

void invocation(uint8_t com, uint8_t *data) {
	switch(((SimpleStandardMessage*)data)->type) {
		case TYPE_GET_CHIP_TEMPERATURE_:
			get_chip_temperature_(com, (GetChipTemperature_*)data);
			return;

		case TYPE_SET_REFERENCE_AIR_PRESSURE:
			set_reference_air_pressure(com, (SetReferenceAirPressure*)data);
			return;

		case TYPE_GET_REFERENCE_AIR_PRESSURE:
			get_reference_air_pressure(com, (GetReferenceAirPressure*)data);
			return;
	}

	simple_invocation(com, data);
}

void constructor(void) {
	_Static_assert(sizeof(BrickContext) <= BRICKLET_CONTEXT_MAX_SIZE, "BrickContext too big");

	simple_constructor();

	BC->counter = 0;

	BC->d1_avg_sum = 0;
	BC->d1_avg_tick = 0;
	BC->d1_avg = 0;

	BC->d1_moving_avg_sum = 0;
	BC->d1_moving_avg_tick = 255;
	BC->d1_moving_avg = 0;

	BC->d2_avg_sum = 0;
	BC->d2_avg_tick = 0;
	BC->d2_avg = 0;

	BC->air_pressure_extra = 0;
	BC->air_pressure_extra_ref = REFERENCE_AIR_PRESSURE << EXTRA_PRECISION;

	BC->temperature = 0;

	ms561101b_write(MS561101BA_RESET);
	SLEEP_MS(5);
	ms561101b_read_calibration();
	ms561101b_write(MS561101BA_D1 | MS561101BA_OSR);

	BC->pending_d = 1;
}

void destructor(void) {
	simple_destructor();
}

void tick(uint8_t tick_type) {
	simple_tick(tick_type);

	if(tick_type & TICK_TASK_TYPE_CALCULATION) {
		if(BC->counter != 0) {
			BC->counter--;
		} else {
			BC->counter = MS561101BA_OSR_COUNTER;

			uint32_t dx = ms561101b_read_adc();

			if(BC->pending_d == 1) {
				update_avg(dx, &BC->d1_avg_sum, &BC->d1_avg, &BC->d1_avg_tick, NUM_D1_AVERAGE);

				if(BC->d1_moving_avg_tick == 255) {
					for(uint8_t i = 0; i < NUM_D1_MOVING_AVERAGE; i++) {
						BC->d1_moving_avg_history[i] = dx;
					}

					BC->d1_moving_avg_sum = dx * NUM_D1_MOVING_AVERAGE;
					BC->d1_moving_avg_tick = 0;
					BC->d1_moving_avg = dx;
				}

				if(BC->d1_avg_tick == 0) {
					BC->d1_moving_avg_sum = BC->d1_moving_avg_sum -
					                        BC->d1_moving_avg_history[BC->d1_moving_avg_tick] +
					                        BC->d1_avg;
					BC->d1_moving_avg_history[BC->d1_moving_avg_tick] = BC->d1_avg;
					BC->d1_moving_avg_tick = (BC->d1_moving_avg_tick + 1) % NUM_D1_MOVING_AVERAGE;
					BC->d1_moving_avg = (BC->d1_moving_avg_sum + NUM_D1_MOVING_AVERAGE / 2) / NUM_D1_MOVING_AVERAGE;
				}

				ms561101b_write(MS561101BA_D2 | MS561101BA_OSR);
				BC->pending_d = 2;
			} else {
				update_avg(dx, &BC->d2_avg_sum, &BC->d2_avg, &BC->d2_avg_tick, NUM_D2_AVERAGE);

				ms561101b_write(MS561101BA_D1 | MS561101BA_OSR);
				BC->pending_d = 1;
			}

			int32_t dt = (int32_t)BC->d2_avg - ((int32_t)BC->calibration[4] << 8);
			int32_t temp = 2000 + (((int64_t)dt * (int64_t)BC->calibration[5]) >> 23);
			int64_t off = ((int64_t)BC->calibration[1] << 16) + (((int64_t)BC->calibration[3] * (int64_t)dt) >> 7);
			int64_t sens = ((int64_t)BC->calibration[0] << 15) + (((int64_t)BC->calibration[2] * (int64_t)dt) >> 8);

			// air_pressure
			if(temp < 2000) {
				// second order compensation
				int32_t k = 5 * (temp - 2000) * (temp - 2000);

				off -= k >> 1;
				sens -= k >> 2;

				if(temp < -1500) {
					int32_t n = (temp + 1500) * (temp + 1500);

					off -= 7 * n;
					sens -= (11 * n) >> 1;
				}
			}

			BC->air_pressure_extra = ((((int64_t)BC->d1_moving_avg * sens) >> 21) - off) >> (15 - EXTRA_PRECISION);

			int32_t air_pressure = (BC->air_pressure_extra * 10) >> EXTRA_PRECISION; // mbar/100 -> mbar/1000

			BC->value[SIMPLE_UNIT_AIR_PRESSURE] = BETWEEN(MIN_AIR_PRESSURE, air_pressure, MAX_AIR_PRESSURE);

			// altitude
			uint8_t size = sizeof(altitude_factors) / sizeof(altitude_factors[0]);
			uint8_t upper = size - 1;
			uint8_t lower;

			while(upper > 0 && air_pressure > altitude_factors[upper].air_pressure) {
				upper--;
			}

			int32_t altitude;
			int32_t delta = (BC->air_pressure_extra_ref - BC->air_pressure_extra) * 10; // mbar/100 -> mbar/1000 with extra precision

			if(upper < size - 1) {
				lower = upper + 1;

				int32_t total_delta = altitude_factors[upper].air_pressure - altitude_factors[lower].air_pressure;
				int32_t upper_delta = altitude_factors[upper].air_pressure - air_pressure;
				int32_t lower_delta = air_pressure - altitude_factors[lower].air_pressure;
				int32_t factor = ((altitude_factors[upper].factor << ALTITUDE_INTERPOLATION_PRECISION) * lower_delta +
				                  (altitude_factors[lower].factor << ALTITUDE_INTERPOLATION_PRECISION) * upper_delta) / total_delta;

				altitude = (delta * factor) >> (ALTITUDE_INTERPOLATION_PRECISION + EXTRA_PRECISION);
			} else {
				lower = upper;
				altitude = (delta * altitude_factors[upper].factor) >> EXTRA_PRECISION;
			}

			BC->value[SIMPLE_UNIT_ALTITUDE] = altitude / 10; // mm -> cm

			// temperature
			int32_t temperature = temp;

			if(temp < 2000) {
				// second order compensation
				temperature -= ((int64_t)dt * (int64_t)dt) >> 31;
			}

			BC->temperature = BETWEEN(MIN_TEMPERATURE, temperature, MAX_TEMPERATURE);
		}
	}
}

void get_chip_temperature_(uint8_t com, GetChipTemperature_ *data) {
	GetChipTemperatureReturn_ gctr;

	gctr.stack_id      = data->stack_id;
	gctr.type          = data->type;
	gctr.length        = sizeof(GetChipTemperatureReturn_);
	gctr.temperature   = BC->temperature;

	BA->send_blocking_with_timeout(&gctr, sizeof(GetChipTemperatureReturn_), com);
}

void set_reference_air_pressure(uint8_t com, SetReferenceAirPressure *data) {
	if(data->air_pressure == 0) {
		BC->air_pressure_extra_ref = BC->air_pressure_extra;
	} else {
		BC->air_pressure_extra_ref = (data->air_pressure << EXTRA_PRECISION) / 10; // add extra precision, mbar/1000 -> mbar/100
	}
}

void get_reference_air_pressure(uint8_t com, GetReferenceAirPressure *data) {
	GetReferenceAirPressureReturn grapr;

	grapr.stack_id      = data->stack_id;
	grapr.type          = data->type;
	grapr.length        = sizeof(GetReferenceAirPressureReturn);
	grapr.air_pressure  = (BC->air_pressure_extra_ref * 10) >> EXTRA_PRECISION; // mbar/100 -> mbar/1000, remove extra precision

	BA->send_blocking_with_timeout(&grapr, sizeof(GetReferenceAirPressureReturn), com);
}

void update_avg(uint32_t dx, uint32_t *sum, uint32_t *avg, uint8_t *tick, uint8_t avg_len) {
	*sum += dx;
	*tick = (*tick + 1) % avg_len;

	if(*tick == 0) {
		*avg = (*sum + avg_len / 2) / avg_len;
		*sum = 0;
	}
}

uint8_t ms561101b_get_address(void) {
	if(BS->address == I2C_EEPROM_ADDRESS_HIGH) {
		return I2C_ADDRESS_HIGH;
	} else {
		return I2C_ADDRESS_LOW;
	}
}

void ms561101b_write(uint8_t command) {
	if(BA->mutex_take(*BA->mutex_twi_bricklet, 10)) {
		BA->bricklet_select(BS->port - 'a');
		Twi* twi = BA->twid->pTwi;

		twi->TWI_MMR = (ms561101b_get_address() << 16);
		while(!((twi->TWI_SR & TWI_SR_TXRDY) == TWI_SR_TXRDY));

		twi->TWI_THR = command;
		twi->TWI_CR = TWI_CR_STOP;
		while(!((twi->TWI_SR & TWI_SR_TXCOMP) == TWI_SR_TXCOMP));

		BA->bricklet_deselect(BS->port - 'a');
		BA->mutex_give(*BA->mutex_twi_bricklet);
	}
}

void ms561101b_read_calibration(void) {
	if(BA->mutex_take(*BA->mutex_twi_bricklet, 10)) {
		uint8_t bytes[2];

		BA->bricklet_select(BS->port - 'a');

		for(uint8_t i = 0; i < MS561101BA_PROM_COUNT; i++) {
			BA->TWID_Read(BA->twid, ms561101b_get_address(),
			              MS561101BA_PROM_ADDR + (i * MS561101BA_PROM_SIZE),
			              1, bytes, 3, NULL);
			BC->calibration[i] = (bytes[0] << 8) | bytes[1];
		}

		BA->bricklet_deselect(BS->port - 'a');
		BA->mutex_give(*BA->mutex_twi_bricklet);
	}
}

uint32_t ms561101b_read_adc() {
	uint32_t value = 0;

	if(BA->mutex_take(*BA->mutex_twi_bricklet, 10)) {
		uint8_t bytes[3];

		BA->bricklet_select(BS->port - 'a');
		BA->TWID_Read(BA->twid, ms561101b_get_address(), MS561101BA_ADC_READ,
		              1, bytes, 3, NULL);

		value = (bytes[0] << 16) | (bytes[1] << 8) | bytes[2];

		BA->bricklet_deselect(BS->port - 'a');
		BA->mutex_give(*BA->mutex_twi_bricklet);
	}

	return value;
}
