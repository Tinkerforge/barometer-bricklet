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
#define SIMPLE_UNIT_TEMPERATURE 2

const SimpleMessageProperty smp[] = {
	{SIMPLE_UNIT_AIR_PRESSURE, SIMPLE_TRANSFER_VALUE, SIMPLE_DIRECTION_GET}, // TYPE_GET_AIR_PRESSURE
	{SIMPLE_UNIT_ALTITUDE, SIMPLE_TRANSFER_VALUE, SIMPLE_DIRECTION_GET}, // TYPE_GET_ALTITUDE
	{SIMPLE_UNIT_TEMPERATURE, SIMPLE_TRANSFER_VALUE, SIMPLE_DIRECTION_GET}, // TYPE_GET_TEMPERATURE
	{SIMPLE_UNIT_AIR_PRESSURE, SIMPLE_TRANSFER_PERIOD, SIMPLE_DIRECTION_SET}, // TYPE_SET_AIR_PRESSURE_CALLBACK_PERIOD
	{SIMPLE_UNIT_AIR_PRESSURE, SIMPLE_TRANSFER_PERIOD, SIMPLE_DIRECTION_GET}, // TYPE_GET_AIR_PRESSURE_CALLBACK_PERIOD
	{SIMPLE_UNIT_ALTITUDE, SIMPLE_TRANSFER_PERIOD, SIMPLE_DIRECTION_SET}, // TYPE_SET_ALTITUDE_CALLBACK_PERIOD
	{SIMPLE_UNIT_ALTITUDE, SIMPLE_TRANSFER_PERIOD, SIMPLE_DIRECTION_GET}, // TYPE_GET_ALTITUDE_CALLBACK_PERIOD
	{SIMPLE_UNIT_TEMPERATURE, SIMPLE_TRANSFER_PERIOD, SIMPLE_DIRECTION_SET}, // TYPE_SET_TEMPERATURE_CALLBACK_PERIOD
	{SIMPLE_UNIT_TEMPERATURE, SIMPLE_TRANSFER_PERIOD, SIMPLE_DIRECTION_GET}, // TYPE_GET_TEMPERATURE_CALLBACK_PERIOD
	{SIMPLE_UNIT_AIR_PRESSURE, SIMPLE_TRANSFER_THRESHOLD, SIMPLE_DIRECTION_SET}, // TYPE_SET_AIR_PRESSURE_CALLBACK_THRESHOLD
	{SIMPLE_UNIT_AIR_PRESSURE, SIMPLE_TRANSFER_THRESHOLD, SIMPLE_DIRECTION_GET}, // TYPE_GET_AIR_PRESSURE_CALLBACK_THRESHOLD
	{SIMPLE_UNIT_ALTITUDE, SIMPLE_TRANSFER_THRESHOLD, SIMPLE_DIRECTION_SET}, // TYPE_SET_ALTITUDE_CALLBACK_THRESHOLD
	{SIMPLE_UNIT_ALTITUDE, SIMPLE_TRANSFER_THRESHOLD, SIMPLE_DIRECTION_GET}, // TYPE_GET_ALTITUDE_CALLBACK_THRESHOLD
	{SIMPLE_UNIT_TEMPERATURE, SIMPLE_TRANSFER_THRESHOLD, SIMPLE_DIRECTION_SET}, // TYPE_SET_TEMPERATURE_CALLBACK_THRESHOLD
	{SIMPLE_UNIT_TEMPERATURE, SIMPLE_TRANSFER_THRESHOLD, SIMPLE_DIRECTION_GET}, // TYPE_GET_TEMPERATURE_CALLBACK_THRESHOLD
	{0, SIMPLE_TRANSFER_DEBOUNCE, SIMPLE_DIRECTION_SET}, // TYPE_SET_DEBOUNCE_PERIOD
	{0, SIMPLE_TRANSFER_DEBOUNCE, SIMPLE_DIRECTION_GET}, // TYPE_GET_DEBOUNCE_PERIOD
};

const SimpleUnitProperty sup[] = {
	{get_air_pressure, SIMPLE_SIGNEDNESS_INT, TYPE_AIR_PRESSURE, TYPE_AIR_PRESSURE_REACHED, SIMPLE_UNIT_AIR_PRESSURE}, // air pressure
	{get_altitude, SIMPLE_SIGNEDNESS_INT, TYPE_ALTITUDE, TYPE_ALTITUDE_REACHED, SIMPLE_UNIT_ALTITUDE}, // altitude
	{get_temperature, SIMPLE_SIGNEDNESS_INT, TYPE_TEMPERATURE, TYPE_TEMPERATURE_REACHED, SIMPLE_UNIT_TEMPERATURE} // temperature
};

void invocation(uint8_t com, uint8_t *data) {
	switch(((SimpleStandardMessage*)data)->type) {
		case TYPE_CALIBRATE_ALTITUDE:
			calibrate_altitude(com, (CalibrateAltitude*)data);
			return;
	}

	simple_invocation(com, data);
}

void constructor(void) {
	_Static_assert(sizeof(BrickContext) <= BRICKLET_CONTEXT_MAX_SIZE, "BrickContext too big");

	simple_constructor();

	BC->counter = 0;
	BC->pending_d = 0;
	BC->d[0] = 0;
	BC->d[1] = 0;
	BC->air_pressure = 0;
	BC->reference_air_pressure = 0;
	BC->altitude = 0;
	BC->temperature = 0;

	ms561101b_write(MS561101BA_RESET);
	SLEEP_MS(5);
	ms561101b_read_calibration();
	logbli("calibration: %d %d %d %d %d %d\n\r",
	       BC->calibration[0],
	       BC->calibration[1],
	       BC->calibration[2],
	       BC->calibration[3],
	       BC->calibration[4],
	       BC->calibration[5]);
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
			BC->counter = MS561101BA_OSR_4096_COUNTER;
			BC->d[BC->pending_d] = ms561101b_read_adc();
			BC->pending_d = 1 - BC->pending_d;

			if(BC->pending_d == 0) {
				ms561101b_write(MS561101BA_D1 | MS561101BA_OSR_4096);
			} else {
				ms561101b_write(MS561101BA_D2 | MS561101BA_OSR_4096);
			}

			int32_t dt = (int32_t)BC->d[1] - ((int32_t)BC->calibration[4] << 8);
			int32_t temp = 2000 + (((int64_t)dt * (int64_t)BC->calibration[5]) >> 23);
			int64_t off = ((int64_t)BC->calibration[1] << 16) + (((int64_t)BC->calibration[3] * dt) >> 7);
			int64_t sens = ((int64_t)BC->calibration[0] << 15) + (((int64_t)BC->calibration[2] * dt) >> 8);

			// air_pressure
			if(temp < 2000) {
				int32_t k = 5 * (temp - 2000) * (temp - 2000);

				off -= k >> 1;
				sens -= k >> 2;

				if (temp < -1500) {
					int32_t n = (temp + 1500) * (temp + 1500);

					off -= 7 * n;
					sens -= (11 * n) >> 1;
				}
			}

			BC->air_pressure = ((((int64_t)BC->d[0] * sens) >> 21) - off) >> 15;

			// altitude, assume 0.001 mbar difference per cm
			BC->altitude = (BC->reference_air_pressure - BC->air_pressure) * 10;

			// temperature
			if(temp < 2000) {
				BC->temperature = temp - (((int64_t)dt * (int64_t)dt) >> 31);
			} else {
				BC->temperature = temp;
			}

			BA->printf("p: %d, t: %d, a: %d\n\r", get_air_pressure(0), get_temperature(0), get_altitude(0));
		}
	}
}

int32_t get_air_pressure(int32_t value) {
	return BC->air_pressure;
}

int32_t get_altitude(int32_t value) {
	return BC->altitude;
}

int32_t get_temperature(int32_t value) {
	return BC->temperature;
}

void calibrate_altitude(uint8_t com, CalibrateAltitude *data) {
	BC->reference_air_pressure = BC->air_pressure;
}

uint8_t ms561101b_get_address(void) {
	if(BS->address == I2C_EEPROM_ADDRESS_HIGH) {
		return I2C_ADDRESS_HIGH;
	}

	return I2C_ADDRESS_LOW;
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
			BA->TWID_Read(BA->twid, ms561101b_get_address(), MS561101BA_PROM_ADDR + (i * MS561101BA_PROM_SIZE), 1, bytes, 3, NULL);
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
		BA->TWID_Read(BA->twid, ms561101b_get_address(), MS561101BA_ADC_READ, 1, bytes, 3, NULL);

		value = (bytes[0] << 16) | (bytes[1] << 8) | bytes[2];

		BA->bricklet_deselect(BS->port - 'a');
		BA->mutex_give(*BA->mutex_twi_bricklet);
	}

	return value;
}
