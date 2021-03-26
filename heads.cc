// This file is part of the interchange file format library.
//
// Copyright (c) 2003 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.

#include "xept.h"

namespace iff {

//----------------------------------------------------------------------

/// Verifies that the chunk is of \p fmt, or throws an exception.
void CChunkHeader::Verify (fmt_t fmt, const char* chunkName, uoff_t offset) const
{
    if (_format != fmt && fmt != cfmt_Autodetect)
	throw XFormatMismatch (chunkName, offset, fmt, _format);
}

//----------------------------------------------------------------------

/// Initializes empty header.
CGroupHeader::CGroupHeader (void)
: CChunkHeader (stream_size() - CChunkHeader::stream_size(), cfmt_Generic)
,_childFormat (cfmt_Generic)
{
}

/// Initializes header with given parameters
CGroupHeader::CGroupHeader (chsize_t size, fmt_t childFormat, fmt_t fmt)
: CChunkHeader (size + (stream_size() - CChunkHeader::stream_size()), fmt)
,_childFormat (childFormat)
{
}

/// Verifies that the vector is of \p fmt, or throws an exception.
void CGroupHeader::Verify (fmt_t childFmt, fmt_t fmt, const char* chunkName, uoff_t offset) const
{
    CChunkHeader::Verify (fmt, chunkName, offset);
    if (_childFormat != childFmt && childFmt != cfmt_Autodetect)
	throw XFormatMismatch (chunkName, offset, childFmt, _childFormat);
}

//----------------------------------------------------------------------

} // namespace iff
