/* barometer-bricklet
 * Copyright (C) 2012-2013 Olaf Lüke <olaf@tinkerforge.com>
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
	{NULL, SIMPLE_SIGNEDNESS_INT, FID_AIR_PRESSURE, FID_AIR_PRESSURE_REACHED, SIMPLE_UNIT_AIR_PRESSURE}, // air pressure
	{NULL, SIMPLE_SIGNEDNESS_INT, FID_ALTITUDE,     FID_ALTITUDE_REACHED,     SIMPLE_UNIT_ALTITUDE}, // altitude
};

const uint8_t smp_length = sizeof(smp);

typedef struct {
	int32_t air_pressure; // hPa/1000
	int32_t factor; // altitude difference in mm per hPa/1000 air pressure difference
} __attribute__((__packed__)) AltitudeFactor;

const AltitudeFactor altitude_factors[] = {
	{1225005, 14},
	{1118311, 13},
	{1012049, 12},
	{ 909702, 11},
	{ 808871, 10},
	{ 710051,  9},
	{ 614001,  8},
	{ 520296,  7}
};

void invocation(const ComType com, const uint8_t *data) {
	switch(((SimpleStandardMessage*)data)->header.fid) {
		case FID_GET_CHIP_TEMPERATURE_: {
			get_chip_temperature_(com, (GetChipTemperature_*)data);
			return;
		}

		case FID_SET_REFERENCE_AIR_PRESSURE: {
			set_reference_air_pressure(com, (SetReferenceAirPressure*)data);
			return;
		}

		case FID_GET_REFERENCE_AIR_PRESSURE: {
			get_reference_air_pressure(com, (GetReferenceAirPressure*)data);
			return;
		}

		case FID_SET_AVERAGING: {
			set_averaging(com, (SetAveraging*)data);
			return;
		}

		case FID_GET_AVERAGING: {
			get_averaging(com, (GetAveraging*)data);
			return;
		}

		case FID_SET_I2C_MODE: {
			set_i2c_mode(com, (SetI2CMode*)data);
			break;
		}

		case FID_GET_I2C_MODE: {
			get_i2c_mode(com, (GetI2CMode*)data);
			break;
		}

		default: {
			simple_invocation(com, data);
			break;
		}
	}

	if(((SimpleStandardMessage*)data)->header.fid > FID_LAST) {
		BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_NOT_SUPPORTED, com);
	}
}

void constructor(void) {
	_Static_assert(sizeof(BrickContext) <= BRICKLET_CONTEXT_MAX_SIZE, "BrickContext too big");

	simple_constructor();

	BC->calibration_valid = false;
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

	BC->num_moving_average = NUM_D1_MOVING_AVERAGE_DEFAULT;
	BC->num_average_d1     = NUM_D1_AVERAGE_DEFAULT;
	BC->num_average_d2     = NUM_D2_AVERAGE_DEFAULT;

	BC->air_pressure_ref = REFERENCE_AIR_PRESSURE;

	BC->temperature = 0;

	ms561101b_write(MS561101BA_RESET);
	SLEEP_MS(5);
	ms561101b_read_calibration();
	ms561101b_write(MS561101BA_D1 | MS561101BA_OSR);

	BC->pending_d = MS561101BA_D1;
	BC->i2c_mode = 0;
}

void destructor(void) {
	simple_destructor();
}

void tick(const uint8_t tick_type) {
	simple_tick(tick_type);

	if(tick_type & TICK_TASK_TYPE_CALCULATION) {
		if(!BC->calibration_valid) {
			BC->value[SIMPLE_UNIT_AIR_PRESSURE] = MIN_AIR_PRESSURE;
			BC->value[SIMPLE_UNIT_ALTITUDE] = 0;
			BC->temperature = MIN_TEMPERATURE;
		} else if(BC->counter != 0) {
			BC->counter--;
		} else {
			uint32_t dx;

			if(BC->pending_d > 0 && !ms561101b_read_adc(&dx)) {
				return;
			}

			if(BC->pending_d == MS561101BA_D1) {
				update_avg(dx, &BC->d1_avg_sum, &BC->d1_avg, &BC->d1_avg_tick, BC->num_average_d1);

				if(BC->num_moving_average == 0) {
					BC->d1_moving_avg = BC->d1_avg;
				} else {
					if(BC->d1_moving_avg_tick == 255) {
						for(uint8_t i = 0; i < BC->num_moving_average; i++) {
							BC->d1_moving_avg_history[i] = dx;
						}

						BC->d1_moving_avg_sum = dx * BC->num_moving_average;
						BC->d1_moving_avg_tick = 0;
						BC->d1_moving_avg = dx;
					}

					if(BC->d1_avg_tick == 0) {
						BC->d1_moving_avg_sum = BC->d1_moving_avg_sum -
												BC->d1_moving_avg_history[BC->d1_moving_avg_tick] +
												BC->d1_avg;
						BC->d1_moving_avg_history[BC->d1_moving_avg_tick] = BC->d1_avg;
						BC->d1_moving_avg_tick = (BC->d1_moving_avg_tick + 1) % BC->num_moving_average;
						BC->d1_moving_avg = (BC->d1_moving_avg_sum + BC->num_moving_average / 2) / BC->num_moving_average;
					}
				}

				BC->pending_d = -MS561101BA_D2;
			} else if(BC->pending_d == MS561101BA_D2) {
				update_avg(dx, &BC->d2_avg_sum, &BC->d2_avg, &BC->d2_avg_tick, BC->num_average_d2);
				calculate();

				BC->pending_d = -MS561101BA_D1;
			}

			if(ms561101b_write((uint16_t)(-BC->pending_d) | MS561101BA_OSR)) {
				BC->pending_d = -BC->pending_d;
				BC->counter = MS561101BA_OSR_COUNTER;
			}
		}
	}
}

void get_chip_temperature_(const ComType com, const GetChipTemperature_ *data) {
	GetChipTemperatureReturn_ gctr;

	gctr.header        = data->header;
	gctr.header.length = sizeof(GetChipTemperatureReturn_);
	gctr.temperature   = BC->temperature;

	BA->send_blocking_with_timeout(&gctr, sizeof(GetChipTemperatureReturn_), com);
}

void set_reference_air_pressure(const ComType com, const SetReferenceAirPressure *data) {
	if(BC->calibration_valid) {
		if(data->air_pressure == 0) {
			BC->air_pressure_ref = BC->value[SIMPLE_UNIT_AIR_PRESSURE];
		} else {
			BC->air_pressure_ref = BETWEEN(MIN_AIR_PRESSURE, data->air_pressure, MAX_AIR_PRESSURE);
		}
	}

	BA->com_return_setter(com, data);
}

void get_reference_air_pressure(const ComType com, const GetReferenceAirPressure *data) {
	GetReferenceAirPressureReturn grapr;

	grapr.header        = data->header;
	grapr.header.length = sizeof(GetReferenceAirPressureReturn);
	grapr.air_pressure  = BC->air_pressure_ref;

	BA->send_blocking_with_timeout(&grapr, sizeof(GetReferenceAirPressureReturn), com);
}

void set_averaging(const ComType com, const SetAveraging *data) {
	if(BC->num_moving_average != data->moving_average_pressure) {
		BC->d1_moving_avg_tick = 255;
	}

	if(BC->num_average_d1 != data->average_pressure) {
		BC->d1_avg_sum = 0;
		BC->d1_avg_tick = 0;
	}

	if(BC->num_average_d2 != data->average_temperature) {
		BC->d2_avg_sum = 0;
		BC->d2_avg_tick = 0;
	}

	BC->num_moving_average = MIN(NUM_D1_MOVING_AVERAGE_MAX, data->moving_average_pressure);
	BC->num_average_d1 = MIN(NUM_D1_AVERAGE_MAX, data->average_pressure);
	BC->num_average_d2 = MIN(NUM_D2_AVERAGE_MAX, data->average_temperature);

	BA->com_return_setter(com, data);
}

void get_averaging(const ComType com, const GetAveraging *data) {
	GetAveragingReturn gar;

	gar.header                  = data->header;
	gar.header.length           = sizeof(GetAveragingReturn);
	gar.moving_average_pressure = BC->num_moving_average;
	gar.average_pressure        = BC->num_average_d1;
	gar.average_temperature     = BC->num_average_d2;

	BA->send_blocking_with_timeout(&gar, sizeof(GetAveragingReturn), com);
}

void calculate(void) {
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

	int32_t air_pressure = (((((int64_t)BC->d1_moving_avg * sens) >> 21) - off) * 10) >> 15; // hPa/1000

	air_pressure = BETWEEN(MIN_AIR_PRESSURE, air_pressure, MAX_AIR_PRESSURE);
	BC->value[SIMPLE_UNIT_AIR_PRESSURE] = air_pressure; // FIXME

	// altitude
	uint8_t size = sizeof(altitude_factors) / sizeof(altitude_factors[0]);
	uint8_t upper = size - 1;
	uint8_t lower;

	while(upper > 0 && air_pressure > altitude_factors[upper].air_pressure) {
		upper--;
	}

	int32_t altitude;
	int32_t delta = BC->air_pressure_ref - air_pressure;

	if(upper < size - 1) {
		lower = upper + 1;

		int32_t total_delta = altitude_factors[upper].air_pressure - altitude_factors[lower].air_pressure;
		int32_t upper_delta = altitude_factors[upper].air_pressure - air_pressure;
		int32_t lower_delta = air_pressure - altitude_factors[lower].air_pressure;
		int32_t factor = ((altitude_factors[upper].factor << ALTITUDE_INTERPOLATION_PRECISION) * lower_delta +
		                  (altitude_factors[lower].factor << ALTITUDE_INTERPOLATION_PRECISION) * upper_delta) / total_delta;

		altitude = ((int64_t)delta * (int64_t)factor) >> ALTITUDE_INTERPOLATION_PRECISION;
	} else {
		lower = upper;
		altitude = delta * altitude_factors[upper].factor;
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

void update_avg(const uint32_t dx, uint32_t *sum, uint32_t *avg, uint8_t *tick, const uint8_t avg_len) {
	if(avg_len == 0) {
		*tick = 0;
		*avg = dx;
		return;
	}

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

bool ms561101b_write(const uint8_t command) {
	if(BA->mutex_take(*BA->mutex_twi_bricklet, 10)) {
		if(BC->i2c_mode == I2C_MODE_SLOW) {
			// Switch to 100khz
			BA->twid->pTwi->TWI_CWGR = 0;
			BA->twid->pTwi->TWI_CWGR = (1 << 16) | (158<< 8) | 158;
		}

		BA->bricklet_select(BS->port - 'a');
		Twi* twi = BA->twid->pTwi;

		twi->TWI_MMR = (ms561101b_get_address() << 16);
		while(!((twi->TWI_SR & TWI_SR_TXRDY) == TWI_SR_TXRDY));

		twi->TWI_THR = command;
		twi->TWI_CR = TWI_CR_STOP;
		while(!((twi->TWI_SR & TWI_SR_TXCOMP) == TWI_SR_TXCOMP));

		BA->bricklet_deselect(BS->port - 'a');
		// Switch back to 400khz
		BA->twid->pTwi->TWI_CWGR = 0;
		BA->twid->pTwi->TWI_CWGR = (76 << 8) | 76;

		BA->mutex_give(*BA->mutex_twi_bricklet);

		return true;
	}

	return false;
}

void ms561101b_crc4(uint16_t *prom) {
	uint16_t reminder = 0;
	uint16_t backup = prom[7];

	prom[7] = 0xFF00 & prom[7];

	for(uint8_t i = 0; i < 16; i++) {
		if(i % 2 == 1) {
			reminder ^= prom[i >> 1] & 0x00FF;
		} else {
			reminder ^= prom[i >> 1] >> 8;
		}

		for(uint8_t k = 8; k > 0; k--) {
			if(reminder & 0x8000) {
				reminder = (reminder << 1) ^ 0x3000;
			} else {
				reminder = reminder << 1;
			}
		}
	}

	reminder = 0x000F & (reminder >> 12);
	prom[7] = backup;

	BC->calibration_valid = (reminder == (prom[7] & 0x000F));
}

void ms561101b_read_calibration(void) {
	uint16_t prom[MS561101BA_PROM_COUNT];

	if(BA->mutex_take(*BA->mutex_twi_bricklet, 10)) {
		// The calibration is read in the constructor, so
		// the user had no chance yet to slow down the I2C bus.
		// To protect against EMI, always read the calibration slow.
		// Switch to 100khz
		BA->twid->pTwi->TWI_CWGR = 0;
		BA->twid->pTwi->TWI_CWGR = (1 << 16) | (158<< 8) | 158;

		uint8_t bytes[2];

		BA->bricklet_select(BS->port - 'a');

		for(uint8_t i = 0; i < MS561101BA_PROM_COUNT; i++) {
			BA->TWID_Read(BA->twid, ms561101b_get_address(),
			              MS561101BA_PROM_ADDR + (i * MS561101BA_PROM_SIZE),
			              1, bytes, MS561101BA_PROM_SIZE, NULL);
			prom[i] = (bytes[0] << 8) | bytes[1];
		}

		BA->bricklet_deselect(BS->port - 'a');

		// Switch back to 400khz
		BA->twid->pTwi->TWI_CWGR = 0;
		BA->twid->pTwi->TWI_CWGR = (76 << 8) | 76;

		BA->mutex_give(*BA->mutex_twi_bricklet);
	}

	for(uint8_t i = 0; i < 6; i++) {
		BC->calibration[i] = prom[i + 1];
	}

	ms561101b_crc4(prom);
}

bool ms561101b_read_adc(uint32_t *value) {
	if(BA->mutex_take(*BA->mutex_twi_bricklet, 10)) {
		uint8_t bytes[3];

		if(BC->i2c_mode == I2C_MODE_SLOW) {
			// Switch to 100khz
			BA->twid->pTwi->TWI_CWGR = 0;
			BA->twid->pTwi->TWI_CWGR = (1 << 16) | (158<< 8) | 158;
		}

		BA->bricklet_select(BS->port - 'a');
		BA->TWID_Read(BA->twid, ms561101b_get_address(), MS561101BA_ADC_READ,
		              1, bytes, 3, NULL);

		*value = (bytes[0] << 16) | (bytes[1] << 8) | bytes[2];

		BA->bricklet_deselect(BS->port - 'a');

		// Switch back to 400khz
		BA->twid->pTwi->TWI_CWGR = 0;
		BA->twid->pTwi->TWI_CWGR = (76 << 8) | 76;

		BA->mutex_give(*BA->mutex_twi_bricklet);

		return true;
	}

	return false;
}


void get_i2c_mode(const ComType com, const GetI2CMode *data) {
	GetI2CModeReturn gi2cmr;

	gi2cmr.header         = data->header;
	gi2cmr.header.length  = sizeof(GetI2CModeReturn);
	gi2cmr.mode           = BC->i2c_mode;

	BA->send_blocking_with_timeout(&gi2cmr, sizeof(GetI2CModeReturn), com);
}

void set_i2c_mode(const ComType com, const SetI2CMode *data) {
	if(data->mode > I2C_MODE_SLOW) {
		BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	BC->i2c_mode = data->mode;
	BA->com_return_setter(com, data);
}
