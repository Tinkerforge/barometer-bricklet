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
	{SIMPLE_UNIT_AIR_PRESSURE, SIMPLE_TRANSFER_VALUE, SIMPLE_DIRECTION_GET}, // TYPE_GET_AIR_PRESSURE
	{SIMPLE_UNIT_ALTITUDE, SIMPLE_TRANSFER_PERIOD, SIMPLE_DIRECTION_SET}, // TYPE_SET_ALTITUDE_CALLBACK_PERIOD
	{SIMPLE_UNIT_ALTITUDE, SIMPLE_TRANSFER_VALUE, SIMPLE_DIRECTION_GET}, // TYPE_GET_ALTITUDE
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
	simple_invocation(com, data);
}

void constructor(void) {
	simple_constructor();

	ms561101b_reset();
	SLEEP_MS(5);
	ms561101b_read_calibration();
	logbli("calibration: %d %d %d %d %d %d\n\r", BC->calibration[0], BC->calibration[1], BC->calibration[2], BC->calibration[3], BC->calibration[4], BC->calibration[5]);
}

void destructor(void) {
	simple_destructor();
}

void tick(uint8_t tick_type) {
	simple_tick(tick_type);
}

int32_t get_air_pressure(int32_t value) {
	return 0;
}

int32_t get_altitude(int32_t value) {
	return 0;
}

int32_t get_temperature(int32_t value) {
	return 0;
}

uint8_t ms561101b_get_address(void) {
	if(BS->address == I2C_EEPROM_ADDRESS_HIGH) {
		return I2C_ADDRESS_HIGH;
	}

	return I2C_ADDRESS_LOW;
}

void ms561101b_reset(void) {
	if(BA->mutex_take(*BA->mutex_twi_bricklet, 10)) {
		Twi* twi = BA->twid->pTwi;

		BA->bricklet_select(BS->port - 'a');

		twi->TWI_MMR = 0;
		twi->TWI_MMR = (1 << 8)|(ms561101b_get_address() << 16);
		twi->TWI_IADR = 0;
		twi->TWI_IADR = MS561101BA_RESET;

		while(!((twi->TWI_SR & TWI_SR_TXRDY) == TWI_SR_TXRDY));
		twi->TWI_THR = MS561101BA_RESET;

		twi->TWI_CR |= TWI_CR_STOP;
	    while(!((twi->TWI_SR & TWI_SR_TXCOMP) == TWI_SR_TXCOMP));

	    BA->bricklet_deselect(BS->port - 'a');

		BA->mutex_give(*BA->mutex_twi_bricklet);
	}
}

void ms561101b_read_calibration(void) {
	if(BA->mutex_take(*BA->mutex_twi_bricklet, 10)) {
		BA->bricklet_select(BS->port - 'a');
		Twi* twi = BA->twid->pTwi;

		for(uint8_t i = 0; i < MS561101BA_PROM_LENGTH; i++) {
			twi->TWI_MMR = 0;
			twi->TWI_MMR = (I2C_INTERNAL_ADDRESS_BYTES << 8) | (ms561101b_get_address() << 16);
			twi->TWI_IADR = 0;

			while(!((twi->TWI_SR & TWI_SR_TXRDY) == TWI_SR_TXRDY));
			twi->TWI_THR = MS561101BA_PROM_ADDR + i*MS561101BA_PROM_SIZE;

			twi->TWI_CR |= TWI_CR_STOP;
		    while(!((twi->TWI_SR & TWI_SR_TXCOMP) == TWI_SR_TXCOMP));

		    twi->TWI_MMR = 0;
		    twi->TWI_MMR = TWI_MMR_MREAD | (ms561101b_get_address() << 16);
		    twi->TWI_IADR = 0;
		    twi->TWI_CR = TWI_CR_START;

			// Read first byte
			while(!((twi->TWI_SR & TWI_SR_RXRDY) == TWI_SR_RXRDY));
			BC->calibration[i] = twi->TWI_RHR << 8;

			// Read second byte
		    twi->TWI_CR = TWI_CR_STOP;
			while(!((twi->TWI_SR & TWI_SR_RXRDY) == TWI_SR_RXRDY));
			BC->calibration[i] |= twi->TWI_RHR;

			while(!((twi->TWI_SR & TWI_SR_TXCOMP) == TWI_SR_TXCOMP));
		}
	    BA->bricklet_deselect(BS->port - 'a');
		BA->mutex_give(*BA->mutex_twi_bricklet);
	}
}
