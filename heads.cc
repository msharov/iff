// This file is part of the interchange file format library.
//
// Copyright (C) 2003-2009 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.

#include "xept.h"

namespace iff {

//----------------------------------------------------------------------

/// Verifies that the chunk is of \p fmt, or throws an exception.
void CChunkHeader::Verify (fmt_t fmt, const char* chunkName, uoff_t offset) const
{
    if (m_Format != fmt && fmt != cfmt_Autodetect)
	throw XFormatMismatch (chunkName, offset, fmt, m_Format);
}

//----------------------------------------------------------------------

/// Initializes empty header.
CGroupHeader::CGroupHeader (void)
: CChunkHeader (stream_size() - CChunkHeader::stream_size(), cfmt_Generic),
  m_ChildFormat (cfmt_Generic)
{
}

/// Initializes header with given parameters
CGroupHeader::CGroupHeader (chsize_t size, fmt_t childFormat, fmt_t fmt)
: CChunkHeader (size + (stream_size() - CChunkHeader::stream_size()), fmt),
  m_ChildFormat (childFormat)
{
}

/// Verifies that the vector is of \p fmt, or throws an exception.
void CGroupHeader::Verify (fmt_t childFmt, fmt_t fmt, const char* chunkName, uoff_t offset) const
{
    CChunkHeader::Verify (fmt, chunkName, offset);
    if (m_ChildFormat != childFmt && childFmt != cfmt_Autodetect)
	throw XFormatMismatch (chunkName, offset, childFmt, m_ChildFormat);
}

//----------------------------------------------------------------------

} // namespace iff
