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
const fmt_t cfmt_Container		= IFF_SFMT("FORM");
const fmt_t cfmt_SimpleContainer	= IFF_SFMT("CAT ");
const fmt_t cfmt_CompoundContainer	= IFF_SFMT("LIST");
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

/// Initializes empty header. m_Size == stream_size().
CChunkHeader::CChunkHeader (void)
: m_Format (cfmt_Generic),
  m_Size (stream_size())
{
}

/// Initializes header of \p size, in \p fmt.
CChunkHeader::CChunkHeader (chsize_t size, fmt_t fmt)
: m_Format (fmt),
  m_Size (size + stream_size())
{
}

/// Reads header from stream \p is
void CChunkHeader::read (istream& is)
{
    is >> m_Format >> m_Size;
}

/// Writes header into stream \p os
void CChunkHeader::write (ostream& os) const
{
    os << m_Format << m_Size;
}

/// Verifies that the chunk is of \p fmt, or throws an exception.
void CChunkHeader::Verify (fmt_t fmt, const char* chunkName, uoff_t offset)
{
    if (m_Format != fmt && fmt != cfmt_Autodetect)
	throw XFormatMismatch (chunkName, offset, fmt, m_Format);
}

//----------------------------------------------------------------------

/// Initializes empty header. m_Size == stream_size().
CVectorHeader::CVectorHeader (void)
: CChunkHeader (),
  m_ChildFormat (cfmt_Generic)
{
    m_Size += (stream_size() - CChunkHeader::stream_size());
}

/// Initializes header with given parameters
CVectorHeader::CVectorHeader (chsize_t size, fmt_t childFormat, fmt_t fmt)
: CChunkHeader (size, fmt),
  m_ChildFormat (childFormat)
{
    m_Size += (stream_size() - CChunkHeader::stream_size());
}

/// Reads header from stream \p is
void CVectorHeader::read (istream& is)
{
    CChunkHeader::read (is);
    is >> m_ChildFormat;
}

/// Writes header into stream \p os
void CVectorHeader::write (ostream& os) const
{
    CChunkHeader::write (os);
    os << m_ChildFormat;
}

/// Verifies that the vector is of \p fmt, or throws an exception.
void CVectorHeader::Verify (fmt_t childFmt, fmt_t fmt, const char* chunkName, uoff_t offset)
{
    CChunkHeader::Verify (fmt, chunkName, offset);
    if (m_ChildFormat != childFmt && childFmt != cfmt_Autodetect)
	throw XFormatMismatch (chunkName, offset, childFmt, m_ChildFormat);
}

//----------------------------------------------------------------------

/// Initializes empty header. m_Size == stream_size().
CContainerHeader::CContainerHeader (void)
: CVectorHeader (),
  m_nChildren (0)
{
    m_Size += (stream_size() - CVectorHeader::stream_size());
}

/// Initializes header with given parameters
CContainerHeader::CContainerHeader (chsize_t size, ccount_t nChildren, fmt_t childFormat, fmt_t fmt)
: CVectorHeader (size, childFormat, fmt),
  m_nChildren (nChildren)
{
    m_Size += (stream_size() - CVectorHeader::stream_size());
}

/// Reads header from stream \p is
void CContainerHeader::read (istream& is)
{
    CVectorHeader::read (is);
    is >> m_nChildren;
}

/// Writes header into stream \p os
void CContainerHeader::write (ostream& os) const
{
    CVectorHeader::write (os);
    os << m_nChildren;
}

//----------------------------------------------------------------------

} // namespace iff

