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

#define TYPE_GET_AIR_PRESSURE 1
#define TYPE_GET_ALTITUDE 2
#define TYPE_GET_TEMPERATURE 3
#define TYPE_SET_AIR_PRESSURE_CALLBACK_PERIOD 4
#define TYPE_GET_AIR_PRESSURE_CALLBACK_PERIOD 5
#define TYPE_SET_ALTITUDE_CALLBACK_PERIOD 6
#define TYPE_GET_ALTITUDE_CALLBACK_PERIOD 7
#define TYPE_SET_TEMPERATURE_CALLBACK_PERIOD 8
#define TYPE_GET_TEMPERATURE_CALLBACK_PERIOD 9
#define TYPE_SET_AIR_PRESSURE_CALLBACK_THRESHOLD 10
#define TYPE_GET_AIR_PRESSURE_CALLBACK_THRESHOLD 11
#define TYPE_SET_ALTITUDE_CALLBACK_THRESHOLD 12
#define TYPE_GET_ALTITUDE_CALLBACK_THRESHOLD 13
#define TYPE_SET_TEMPERATURE_CALLBACK_THRESHOLD 14
#define TYPE_GET_TEMPERATURE_CALLBACK_THRESHOLD 15
#define TYPE_SET_DEBOUNCE_PERIOD 16
#define TYPE_GET_DEBOUNCE_PERIOD 17
#define TYPE_CALIBRATE_ALTITUDE 18
#define TYPE_AIR_PRESSURE 19
#define TYPE_AIR_PRESSURE_REACHED 20
#define TYPE_ALTITUDE 21
#define TYPE_ALTITUDE_REACHED 22
#define TYPE_TEMPERATURE 23
#define TYPE_TEMPERATURE_REACHED 24

int32_t get_air_pressure(int32_t value);
int32_t get_altitude(int32_t value);
int32_t get_temperature(int32_t value);

void invocation(uint8_t com, uint8_t *data);
void constructor(void);
void destructor(void);
void tick(uint8_t tick_type);

#endif