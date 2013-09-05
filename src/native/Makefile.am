# JNativeHook: Global keyboard and mouse hooking for Java.
# Copyright (C) 2006-2013 Alexander Barker.  All Rights Received.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published 
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

ACLOCAL_AMFLAGS = -I m4

DARWIN_SRC =
UNIX_SRC = unix/x_input_helper.c unix/x_unicode_helper.c
WINDOWS_SRC = windows/hook_callback.c windows/win_unicode_helper.c

if BUILD_DARWIN
OS_SRC = $(DARWIN_SRC)
endif

if BUILD_UNIX
OS_SRC = $(UNIX_SRC)
endif

if BUILD_WINDOWS
OS_SRC = $(WINDOWS_SRC)
endif

pkgconfigdir = $(libdir)/pc
pkgconfig_DATA = $(top_builddir)/pc/nativehook.pc

include_HEADERS = include/nativehook.h

lib_LTLIBRARIES = libnativehook.la
libnativehook_la_CFLAGS = $(AM_CFLAGS) -Wall -Wextra -pedantic -Wno-unused-parameter -I$(top_srcdir)/common -I$(top_srcdir)/include
libnativehook_la_LDFLAGS = $(LTLDFLAGS) -no-undefined -as-needed
libnativehook_la_SOURCES = common/scancode_table.c \
	common/logger.c \
	$(backend)/hook_thread.c \
	$(backend)/input_converter.c \
	$(backend)/library_load.c \
	$(backend)/post_event.c \
	$(backend)/system_properties.c \
	$(OS_SRC)

if BUILD_DEMO
bin_PROGRAMS = hookdemo postdemo

hookdemo_SOURCES = demo/hook_demo.c
hookdemo_LDADD = $(top_builddir)/libnativehook.la
hookdemo_CFLAGS = -Wall -Wextra -pedantic -I$(top_srcdir)/include
hookdemo_LDFLAGS = -Wl,--as-needed,-O1

postdemo_SOURCES = demo/post_demo.c
postdemo_LDADD = $(top_builddir)/libnativehook.la
postdemo_CFLAGS = -Wall -Wextra -pedantic -I$(top_srcdir)/include
postdemo_LDFLAGS = -Wl,--as-needed,-O1
endif

EXTRA_DIST = license-commercial.txt license-gpl.txt readme.txt
