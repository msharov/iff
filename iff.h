// This file is part of the interchange file format library.
//
// Copyright (C) 2003-2009 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.

#pragma once
#include "iff/utils.h"

/// \mainpage
///
/// \section intro Definitions for IFF encoding.
///
/// The IFF encoding is intended as a flexible binary file format that
/// can easily be read in full or in part by any reader. Original format
/// was introduced by Electronic Arts for storage of graphics and sound
/// files. This format was widely used on the Amiga platform and can be
/// read by some PC programs as well. The file consists of a list of chunks.
/// Chunks may contain other chunks. Byte order of the chunks is machine-
/// -native. Use the format field to determine if you need to swap bytes.
/// All blocks must be at least 2-byte aligned to be read. This implementation
/// will write 4-byte aligned blocks (which are fine too by original standard)
/// All header information is written in machine-native format. Non-native
/// files should be converted by an external utility. This this is a
/// deviation from the standard, and it is so because there is no easy way
/// to implement byte-order determination into generic read-write code.
/// Any such context is necessarily ugly and inefficient on little-endian
/// machines, which, let's face it, constitute 99% of all computers used.
