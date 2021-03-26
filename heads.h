// This file is part of the interchange file format library.
//
// Copyright (c) 2003 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.

#pragma once
#include "config.h"

/// Contains functions for dealing with IFF formatted files.
namespace iff {
using namespace ustl;

//----------------------------------------------------------------------
// Type definitions
//----------------------------------------------------------------------
using fmt_t	= uint32_t;	///< Type for format fields in the headers
using chsize_t	= uint32_t;	///< Type for the chunk size field
using ccount_t	= uint32_t;	///< Type for the child count field

//----------------------------------------------------------------------
// IFF standard chunk formats
//----------------------------------------------------------------------

#if IFF_BYTE_ORDER == USTL_LITTLE_ENDIAN
    inline static constexpr fmt_t IFF_FMT (fmt_t a, fmt_t b, fmt_t c, fmt_t d)
	{ return (d<<24)|(c<<16)|(b<<8)|a; }
    inline static void boci2n (uint32_t& v)	{ v = le_to_native(v); }
    inline static uint32_t bon2i(uint32_t v)	{ return (native_to_le(v)); }
#else
    inline static constexpr fmt_t IFF_FMT (fmt_t a, fmt_t b, fmt_t c, fmt_t d)
	{ return (a<<24)|(b<<16)|(c<<8)|d; }
    inline static void boci2n (uint32_t& v)	{ v = be_to_native(v); }
    inline static uint32_t bon2i(uint32_t v)	{ return (native_to_be(v)); }
#endif
inline constexpr fmt_t IFF_SFMT (const char* s)
    { return IFF_FMT(s[0],s[1],s[2],s[3]); }

enum : fmt_t {
    cfmt_Unknown		= 0,
    cfmt_FORM			= IFF_SFMT("FORM"),
    cfmt_LIST			= IFF_SFMT("LIST"),
    cfmt_CAT			= IFF_SFMT("CAT "),
    cfmt_Filler			= IFF_SFMT("    "),
    cfmt_Bitmap			= IFF_SFMT("ILBM"),
    cfmt_Properties		= IFF_SFMT("PROP"),
    cfmt_BitmapHeader		= IFF_SFMT("BMHD"),
    cfmt_ColorMap		= IFF_SFMT("CMAP"),
    cfmt_ColorLookupTable	= IFF_SFMT("CLUT"),
    cfmt_ColorCycle1		= IFF_SFMT("CRNG"),
    cfmt_ColorCycle2		= IFF_SFMT("CCRT"),
    cfmt_HotSpot		= IFF_SFMT("GRAB"),
    cfmt_Sprite			= IFF_SFMT("SPRT"),
    cfmt_VoiceHeader		= IFF_SFMT("VHDR"),
    cfmt_Name			= IFF_SFMT("NAME"),
    cfmt_Copyright		= IFF_SFMT("(c) "),
    cfmt_Author			= IFF_SFMT("AUTH"),
    cfmt_Annotation		= IFF_SFMT("ANNO"),
    cfmt_Attack			= IFF_SFMT("ATAK"),
    cfmt_Release		= IFF_SFMT("RLSE"),
    cfmt_Generic		= IFF_SFMT("BODY"),
    cfmt_Vector			= IFF_SFMT("VECT"),
    cfmt_CountedContainer	= IFF_SFMT("CNTR"),
    cfmt_StringTable		= IFF_SFMT("STRT"),
    cfmt_Autodetect		= IFF_SFMT("AUTO")
};

//----------------------------------------------------------------------

/// \class CChunkHeader heads.h iff.h
///
/// \brief Header for non-aggregate chunks.
///
/// Contains size and format information for the chunk. Like
/// CContainerHeader, the written format is the spec too, so
/// can be written manually. The size is the size of the
/// chunk data, not including the header
///
class CChunkHeader {
public:
    inline constexpr		CChunkHeader (void)		: _format (cfmt_Generic), _size (0) {}
    inline constexpr explicit	CChunkHeader (chsize_t size, fmt_t fmt = cfmt_Generic)	: _format (fmt), _size (size) {}
    inline void			read (istream& is)		{ is >> _format >> _size; boci2n(_format); boci2n(_size); }
    inline void			write (ostream& os) const	{ os << bon2i(_format) << bon2i(_size); }
    inline size_t		stream_size (void) const	{ return (stream_size_of(_format) + stream_size_of(_size)); }
    inline constexpr size_t	Size (void) const		{ return _size; }
    inline size_t		SizeWithHeader (void) const	{ return Size() + stream_size(); }
    inline constexpr fmt_t	Format (void) const		{ return _format; }
    void			Verify (fmt_t fmt = cfmt_Generic, const char* chunkName = "chunk", uoff_t offset = 0) const;
private:
    fmt_t	_format;	///< Format of the chunk
    chsize_t	_size;		///< Size of the chunk including the header.
};

//----------------------------------------------------------------------

/// \class CGroupHeader heads.h iff.h
///
/// \brief Header for aggregate chunks like FORM, LIST, and CAT
/// This header can be written standalone, or with WriteVector.
/// Each IFF file must start with a group header with file size
/// and type of contents.
///
class CGroupHeader : public CChunkHeader {
public:
			CGroupHeader (void);
			CGroupHeader (chsize_t size, fmt_t childFormat = cfmt_Generic, fmt_t fmt = cfmt_FORM);
    inline void		read (istream& is)		{ CChunkHeader::read (is); is >> _childFormat; boci2n (_childFormat); }
    inline void		write (ostream& os) const	{ CChunkHeader::write (os); os << bon2i(_childFormat); }
    inline size_t	stream_size (void) const	{ return CChunkHeader::stream_size() + stream_size_of(_childFormat); }
    inline fmt_t	ChildFormat (void) const	{ return _childFormat; }
    inline size_t	Size (void) const		{ return CChunkHeader::Size() - (stream_size() - CChunkHeader::stream_size()); }
    void		Verify (fmt_t childFormat = cfmt_Generic, fmt_t fmt = cfmt_FORM, const char* chunkName = "FORM", uoff_t offset = 0) const;
private:
    fmt_t		_childFormat;	///< Format of each child chunk.
};

//----------------------------------------------------------------------

/// \brief PROP section values
struct SProp {
    fmt_t	m_Name;		///< 4 character prop name (use IFF_FMT to define)
    uint32_t	m_Value;	///< Property value.
};

//----------------------------------------------------------------------

} // namespace iff

ALIGNOF (iff::CChunkHeader, IFF_GRAIN)
