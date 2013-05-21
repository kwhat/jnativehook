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

MAINTAINERCLEANFILES = Makefile.in aclocal.m4

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = license-commercial.txt license-gpl.txt readme.txt

#AM_CFLAGS = -Wall -Wextra -pedantic -fno-strict-aliasing
#AM_CFLAGS += -I$(top_srcdir)/include
#AM_CFLAGS = -D_GNU_SOURCE -D_XOPEN_SOURCE
#AM_LDFLAGS = -Wl,--as-needed,-O1

# Define bin and lib targets.
#bin_PROGRAMS = nativehookdemo
lib_LTLIBRARIES = libnativehook.la
include_HEADERS = include/libnativehook.h

if OS_UNIX
OS_SRC = \
	unix/convert_to_native.c \
	unix/convert_to_virtual.c \
	unix/hook_thread.c \
	unix/library_load.c \
	unix/post_events.c \
	unix/system_properties.c \
	unix/x_input_helpers.c

BACKEND_CFLAGS = -I$(top_srcdir)/unix
endif

if OS_DARWIN
OS_SRC = \
    darwin/OSXInputHelpers.c
endif

if OS_WINDOWS
OS_SRC = \
    windows/WinUnicodeHelper.c
endif

libnativehook_la_CFLAGS = -Wall -Wextra -pedantic -fno-strict-aliasing
libnativehook_la_LDFLAGS = $(LTLDFLAGS)
libnativehook_la_SOURCES = \
	common/error.c \
	$(OS_SRC)
