/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2013 Alexander Barker.  All Rights Received.
 * http://code.google.com/p/jnativehook/
 *
 * JNativeHook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * JNativeHook is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <nativehook.h>
#include <stdbool.h>
#include <stdio.h>

#include "logger.h"

static bool (*current_info_proc)(const char *) = &info;
static bool (*current_warn_proc)(const char *) = &warn;
static bool (*current_error_proc)(const char *) = &error;

#ifdef USE_DEBUG
static bool (*current_debug_proc)(const char *) = &debug;

bool debug(const char * message) {
	return fprintf(stdout, "%s\n", message) >= 0;
}
#endif

bool info(const char * message) {
	// Discard info level messages.
	return false;
}

bool warn(const char * message) {
	return fprintf(stdout, "%s\n", message) >= 0;
}

bool error(const char * message) {
	return fprintf(stderr, "%s\n", message) >= 0;
}

NATIVEHOOK_API bool hook_set_logger_proc(log_level level, bool (*logger_proc)(const char *)) {
	bool status = true;
	
	switch (level) {
		#ifdef USE_DEBUG
		case LOG_LEVEL_DEBUG:
			if (logger_proc == NULL) {
				current_debug_proc = &debug;
			}
			else {
				current_debug_proc = logger_proc;
			}
			break;
		#endif
		case LOG_LEVEL_INFO:
			if (logger_proc == NULL) {
				current_info_proc = &info;
			}
			else {
				current_info_proc = logger_proc;
			}
			break;
			
		case LOG_LEVEL_WARN:
			if (logger_proc == NULL) {
				current_warn_proc = &warn;
			}
			else {
				current_warn_proc = logger_proc;
			}
			break;
		case LOG_LEVEL_ERROR:
			if (logger_proc == NULL) {
				current_error_proc = &error;
			}
			else {
				current_error_proc = logger_proc;
			}
			break;
			
		default:
			status = false;
			break;
	}
	
	return status;
}
