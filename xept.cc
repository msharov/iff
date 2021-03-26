// This file is part of the interchange file format library.
//
// Copyright (c) 2003 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.

#include "xept.h"

namespace iff {

//----------------------------------------------------------------------

/// Wrong format error for \p typeName at \p offset; \p expected format does not match \p actual format.
XFormatMismatch::XFormatMismatch (const char* typeName, uoff_t offset, fmt_t expected, fmt_t actual) noexcept
: exception()
,_typename (typeName)
,_offset (offset)
,_expected (expected)
,_actual (actual)
{
}

/// Writes error description to \p msgbuf. Default fmt is "iff chunk %s at 0x%zX has format %08X (%c%c%c%c), expected %08X (%c%c%c%c)"
void XFormatMismatch::info (string& msgbuf, const char* fmt) const noexcept
{
    if (!fmt) fmt = "iff chunk %s at 0x%zX has format %08X (%c%c%c%c), expected %08X (%c%c%c%c)";
    char expstr[sizeof(fmt_t)], actstr[sizeof(fmt_t)];
    for (size_t i = 0; i < sizeof(fmt_t); ++i) {
	char ec = _expected >> (i*8), ac = _actual >> (i*8);
	expstr[i] = isprint(ec) ? ec : '.';
	actstr[i] = isprint(ac) ? ac : '.';
    }
    char dmname [256];
    strncpy (dmname, _typename, VectorSize(dmname));
    dmname[VectorSize(dmname)-1] = 0;
    demangle_type_name (VectorBlock (dmname));
    try {
	msgbuf.format (fmt, dmname, _offset,
		_actual, actstr[0], actstr[1], actstr[2], actstr[3],
		_expected, expstr[0], expstr[1], expstr[2], expstr[3]);
    } catch (...) {}
}

//----------------------------------------------------------------------

/// Chunk of \p typeName at \p offset is not of the \p expected size.
XChunkSizeMismatch::XChunkSizeMismatch (const char* typeName, uoff_t offset, size_t expected, size_t actual) noexcept
: exception()
,_typename (typeName)
,_offset (offset)
,_expected (expected)
,_actual (actual)
{
}

/// Writes error description to \p msgbuf. Default \p fmt is "iff chunk %s at 0x%zX is %zu bytes, expected %zu"
void XChunkSizeMismatch::info (string& msgbuf, const char* fmt) const noexcept
{
    if (!fmt) fmt = "iff chunk %s at 0x%zX is %zu bytes, expected %zu";
    char dmname [256];
    strncpy (dmname, _typename, VectorSize(dmname));
    dmname[VectorSize(dmname)-1] = 0;
    demangle_type_name (VectorBlock (dmname));
    try { msgbuf.format (fmt, dmname, _offset, _actual, _expected); } catch (...) {}
}

//----------------------------------------------------------------------

} // namespace iff
