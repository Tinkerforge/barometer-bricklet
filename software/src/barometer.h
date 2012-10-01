/* barometer-bricklet
 * Copyright (C) 2012 Olaf Lüke <olaf@tinkerforge.com>
 *
 * barometer.h: Implementation of Barometer Bricklet messages
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

#ifndef BAROMETER_H
#define BAROMETER_H

#include <stdint.h>

#define I2C_EEPROM_ADDRESS_HIGH 84

#define I2C_ADDRESS_HIGH 118 // 0b1110111
#define I2C_ADDRESS_LOW 119 // 0b1110110

#define I2C_INTERNAL_ADDRESS_BYTES 1

// MS561101BA registers
#define MS561101BA_RESET            0x1E
#define MS561101BA_D1               0x40
#define MS561101BA_D2               0x50
#define MS561101BA_ADC_READ         0x00

// Over Sampling Ratio constants
#define MS561101BA_OSR_256          0x00
#define MS561101BA_OSR_512          0x02
#define MS561101BA_OSR_1024         0x04
#define MS561101BA_OSR_2048         0x06
#define MS561101BA_OSR_4096         0x08

#define MS561101BA_OSR MS561101BA_OSR_4096

#define MS561101BA_OSR_256_COUNTER  0
#define MS561101BA_OSR_512_COUNTER  1
#define MS561101BA_OSR_1024_COUNTER 2
#define MS561101BA_OSR_2048_COUNTER 4
#define MS561101BA_OSR_4096_COUNTER 9

#define MS561101BA_OSR_COUNTER MS561101BA_OSR_4096_COUNTER

// Calibration values
#define MS561101BA_PROM_ADDR        0xA2
#define MS561101BA_PROM_COUNT       6
#define MS561101BA_PROM_SIZE        2

#define TYPE_GET_AIR_PRESSURE 1
#define TYPE_GET_ALTITUDE 2
#define TYPE_SET_AIR_PRESSURE_CALLBACK_PERIOD 3
#define TYPE_GET_AIR_PRESSURE_CALLBACK_PERIOD 4
#define TYPE_SET_ALTITUDE_CALLBACK_PERIOD 5
#define TYPE_GET_ALTITUDE_CALLBACK_PERIOD 6
#define TYPE_SET_AIR_PRESSURE_CALLBACK_THRESHOLD 7
#define TYPE_GET_AIR_PRESSURE_CALLBACK_THRESHOLD 8
#define TYPE_SET_ALTITUDE_CALLBACK_THRESHOLD 9
#define TYPE_GET_ALTITUDE_CALLBACK_THRESHOLD 12
#define TYPE_SET_DEBOUNCE_PERIOD 11
#define TYPE_GET_DEBOUNCE_PERIOD 12
#define TYPE_SET_REFERENCE_AIR_PRESSURE 13
#define TYPE_GET_CHIP_TEMPERATURE_ 14
#define TYPE_AIR_PRESSURE 15
#define TYPE_ALTITUDE 16
#define TYPE_AIR_PRESSURE_REACHED 17
#define TYPE_ALTITUDE_REACHED 18
#define TYPE_GET_REFERENCE_AIR_PRESSURE 19

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
} __attribute__((__packed__)) GetChipTemperature_;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
	int16_t temperature;
} __attribute__((__packed__)) GetChipTemperatureReturn_;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
	int32_t air_pressure;
} __attribute__((__packed__)) SetReferenceAirPressure;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
} __attribute__((__packed__)) GetReferenceAirPressure;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
	int32_t air_pressure;
} __attribute__((__packed__)) GetReferenceAirPressureReturn;

void get_chip_temperature_(uint8_t com, GetChipTemperature_ *data);
void set_reference_air_pressure(uint8_t com, SetReferenceAirPressure *data);
void get_reference_air_pressure(uint8_t com, GetReferenceAirPressure *data);

void update_avg(uint32_t dx, uint32_t *sum, uint32_t *avg, uint8_t *tick, uint8_t avg_len);

uint8_t ms561101b_get_address(void);
void ms561101b_write(uint8_t command);
void ms561101b_read_calibration(void);
uint32_t ms561101b_read_adc();

void invocation(uint8_t com, uint8_t *data);
void constructor(void);
void destructor(void);
void tick(uint8_t tick_type);

#endif
