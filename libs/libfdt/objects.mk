#/**
# Copyright (c) 2010 Anup Patel.
# All rights reserved.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
# @file objects.mk
# @version 1.0
# @author Anup Patel (anup@brainfault.org)
# @brief list of libfdt objects to be build
# */

libs-cppflags-$(CONFIG_LIBFDT)+= -I$(libs_dir)/libfdt
libs-objs-$(CONFIG_LIBFDT)+= libfdt/libfdt.o
