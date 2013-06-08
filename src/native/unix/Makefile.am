# JNativeHook: Global keyboard and mouse hooking for Java.
# Copyright (C) 2006-2013 Alexander Barker.  All Rights Received.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

ACLOCAL_AMFLAGS = -I m4

AM_CFLAGS = -Wall -Wextra -pedantic -O2 -fno-strict-aliasing -std=c99
AM_CFLAGS += -I$(top_srcdir)/include
AM_CFLAGS += -D_GNU_SOURCE -D_XOPEN_SOURCE
AM_LDFLAGS = -Wl,--as-needed,-O1

#include_HEADERS = include/nativehook.h

lib_LTLIBRARIES = libnativehook.la
libnativehook_la_CFLAGS = -Wall -Wextra -pedantic -fno-strict-aliasing -I$(top_srcdir)/include
libnativehook_la_LDFLAGS = $(LTLDFLAGS)
libnativehook_la_SOURCES = \
	convert_to_native.c \
	convert_to_virtual.c \
	hook_thread.c \
	library_load.c \
	post_events.c \
	system_properties.c \
	x_input_helpers.c

CLEANFILES = 

DISTCLEANFILES = \
	Makefile
