/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2012 Alexander Barker.  All Rights Received.
 * http://code.google.com/p/jnativehook/
 *
 * JNativeHook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * JNativeHook is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _included_system_properties
#define _included_system_properties

// Retrieves the keyboard auto repeat rate.
extern int_fast32_t get_auto_repeat_rate();

// Retrieves the keyboard auto repeat delay.
extern int_fast32_t get_auto_repeat_delay();

// Retrieves the mouse acceleration multiplier.
extern int_fast32_t get_pointer_acceleration_multiplier();

// Retrieves the mouse acceleration threshold.
extern int_fast32_t get_pointer_acceleration_threshold();

// Retrieves the mouse sensitivity.
extern int_fast32_t get_pointer_sensitivity();

// Retrieves the double/triple click interval.
extern int_fast32_t get_multi_click_time();

#endif
