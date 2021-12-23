// Asynchronous Analog Read
// Copyright (C) 2021  Joshua Booth

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef ANALOG_READ_ASYNC_H_
#define ANALOG_READ_ASYNC_H_

#include <stdint.h>

typedef void (*analogReadCompleteCallback_t)(uint16_t value, void *data);

void analogReadAsync(uint8_t pin, analogReadCompleteCallback_t cb, const void *data = nullptr);

#endif // ANALOG_READ_ASYNC_H_