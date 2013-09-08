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


#ifndef __FUNCTION__
#define __FUNCTION__ __func__
#endif

#ifdef USE_QUIET
#define COPYWRITE() (void) 0;
#else
#include <stdio.h>
#define COPYWRITE()	fprintf(stdout, \
		"JNativeHook: Global keyboard and mouse hooking for Java.\n" \
		"Copyright (C) 2006-2013 Alexander Barker.  All Rights Received.\n" \
		"http://code.google.com/p/jnativehook/\n" \
		"\n" \
		"JNativeHook is free software: you can redistribute it and/or modify\n" \
		"it under the terms of the GNU Lesser General Public License as published\n" \
		"by the Free Software Foundation, either version 3 of the License, or\n" \
		"(at your option) any later version.\n" \
		"\n" \
		"JNativeHook is distributed in the hope that it will be useful,\n" \
		"but WITHOUT ANY WARRANTY; without even the implied warranty of\n" \
		"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n" \
		"GNU General Public License for more details.\n" \
		"\n" \
		"You should have received a copy of the GNU Lesser General Public License\n" \
		"along with this program.  If not, see <http://www.gnu.org/licenses/>.\n\n");
#endif

// logger(level, message)
extern logger_t logger;
