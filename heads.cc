// This file is part of the interchange file format library.
//
// Copyright (C) 2003-2006 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.
//
// heads.cc
//

#include "xept.h"

namespace iff {

//----------------------------------------------------------------------

const fmt_t cfmt_Unknown		= 0;
const fmt_t cfmt_FORM			= IFF_SFMT("FORM");
const fmt_t cfmt_LIST			= IFF_SFMT("LIST");
const fmt_t cfmt_CAT			= IFF_SFMT("CAT ");
const fmt_t cfmt_Filler			= IFF_SFMT("    ");
const fmt_t cfmt_Bitmap			= IFF_SFMT("ILBM");
const fmt_t cfmt_Properties		= IFF_SFMT("PROP");
const fmt_t cfmt_BitmapHeader		= IFF_SFMT("BMHD");
const fmt_t cfmt_ColorMap		= IFF_SFMT("CMAP");
const fmt_t cfmt_ColorLookupTable	= IFF_SFMT("CLUT");
const fmt_t cfmt_ColorCycle1		= IFF_SFMT("CRNG");
const fmt_t cfmt_ColorCycle2		= IFF_SFMT("CCRT");
const fmt_t cfmt_HotSpot		= IFF_SFMT("GRAB");
const fmt_t cfmt_Sprite			= IFF_SFMT("SPRT");
const fmt_t cfmt_VoiceHeader		= IFF_SFMT("VHDR");
const fmt_t cfmt_Name			= IFF_SFMT("NAME");
const fmt_t cfmt_Copyright		= IFF_SFMT("(c) ");
const fmt_t cfmt_Author			= IFF_SFMT("AUTH");
const fmt_t cfmt_Annotation		= IFF_SFMT("ANNO");
const fmt_t cfmt_Attack			= IFF_SFMT("ATAK");
const fmt_t cfmt_Release		= IFF_SFMT("RLSE");
const fmt_t cfmt_Generic		= IFF_SFMT("BODY");

// Extension chunk formats
const fmt_t cfmt_Vector			= IFF_SFMT("VECT");
const fmt_t cfmt_CountedContainer	= IFF_SFMT("CNTR");
const fmt_t cfmt_StringTable		= IFF_SFMT("STRT");
const fmt_t cfmt_Autodetect		= IFF_SFMT("AUTO");

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

