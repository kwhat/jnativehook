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

bin_PROGRAMS = nativehookdemo
nativehookdemo_SOURCES = hook_demo.c
nativehookdemo_LDADD = $(top_builddir)/$(backend)/libnativehook.la
nativehookdemo_CFLAGS = -Wall -Wextra -pedantic -std=c99 -D_POSIX_C_SOURCE=199309L -I$(top_srcdir)/include
nativehookdemo_LDFLAGS = -Wl,--as-needed,-O1
