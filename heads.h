// This file is part of the interchange file format library.
//
// Copyright (C) 2003-2009 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.

#ifndef HEADS_H_6AE64E742217B13E3F576C6D5A4C3292
#define HEADS_H_6AE64E742217B13E3F576C6D5A4C3292

#include "config.h"

/// Contains functions for dealing with IFF formatted files.
namespace iff {
using namespace ustl;

//----------------------------------------------------------------------
// Type definitions
//----------------------------------------------------------------------
typedef uint32_t	fmt_t;		///< Type for format fields in the headers
typedef uint32_t	chsize_t;	///< Type for the chunk size field
typedef uint32_t	ccount_t;	///< Type for the child count field

//----------------------------------------------------------------------
// IFF standard chunk formats
//----------------------------------------------------------------------

#if IFF_BYTE_ORDER == USTL_LITTLE_ENDIAN
    #define IFF_FMT(a,b,c,d)	((((((::iff::fmt_t(d)<<8)|(c))<<8)|(b))<<8)|(a))
    inline void boci2n (uint32_t& v)	{ v = le_to_native(v); }
    inline uint32_t bon2i(uint32_t v)	{ return (native_to_le(v)); }
#else
    #define IFF_FMT(a,b,c,d)	((((((::iff::fmt_t(a)<<8)|(b))<<8)|(c))<<8)|(d))
    inline void boci2n (uint32_t& v)	{ v = be_to_native(v); }
    inline uint32_t bon2i(uint32_t v)	{ return (native_to_be(v)); }
#endif
#define IFF_SFMT(s)		IFF_FMT(s[0],s[1],s[2],s[3])

enum {
    cfmt_Unknown		= 0,
    cfmt_FORM			= IFF_FMT('F','O','R','M'),
    cfmt_LIST			= IFF_FMT('L','I','S','T'),
    cfmt_CAT			= IFF_FMT('C','A','T',' '),
    cfmt_Filler			= IFF_FMT(' ',' ',' ',' '),
    cfmt_Bitmap			= IFF_FMT('I','L','B','M'),
    cfmt_Properties		= IFF_FMT('P','R','O','P'),
    cfmt_BitmapHeader		= IFF_FMT('B','M','H','D'),
    cfmt_ColorMap		= IFF_FMT('C','M','A','P'),
    cfmt_ColorLookupTable	= IFF_FMT('C','L','U','T'),
    cfmt_ColorCycle1		= IFF_FMT('C','R','N','G'),
    cfmt_ColorCycle2		= IFF_FMT('C','C','R','T'),
    cfmt_HotSpot		= IFF_FMT('G','R','A','B'),
    cfmt_Sprite			= IFF_FMT('S','P','R','T'),
    cfmt_VoiceHeader		= IFF_FMT('V','H','D','R'),
    cfmt_Name			= IFF_FMT('N','A','M','E'),
    cfmt_Copyright		= IFF_FMT('(','c',')',' '),
    cfmt_Author			= IFF_FMT('A','U','T','H'),
    cfmt_Annotation		= IFF_FMT('A','N','N','O'),
    cfmt_Attack			= IFF_FMT('A','T','A','K'),
    cfmt_Release		= IFF_FMT('R','L','S','E'),
    cfmt_Generic		= IFF_FMT('B','O','D','Y'),
    cfmt_Vector			= IFF_FMT('V','E','C','T'),
    cfmt_CountedContainer	= IFF_FMT('C','N','T','R'),
    cfmt_StringTable		= IFF_FMT('S','T','R','T'),
    cfmt_Autodetect		= IFF_FMT('A','U','T','O')
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
    inline		CChunkHeader (void)		: m_Format (cfmt_Generic), m_Size (0) { }
    inline		CChunkHeader (chsize_t size, fmt_t fmt = cfmt_Generic)	: m_Format (fmt), m_Size (size) { }
    inline void		read (istream& is)		{ is >> m_Format >> m_Size; boci2n(m_Format); boci2n(m_Size); }
    inline void		write (ostream& os) const	{ os << bon2i(m_Format) << bon2i(m_Size); }
    inline size_t	stream_size (void) const	{ return (stream_size_of(m_Format) + stream_size_of(m_Size)); }
    inline size_t	Size (void) const		{ return (m_Size); }
    inline size_t	SizeWithHeader (void) const	{ return (Size() + stream_size()); }
    inline fmt_t	Format (void) const		{ return (m_Format); }
    void		Verify (fmt_t fmt = cfmt_Generic, const char* chunkName = "chunk", uoff_t offset = 0) const;
private:
    fmt_t		m_Format;	///< Format of the chunk
    chsize_t		m_Size;		///< Size of the chunk including the header.
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
    inline void		read (istream& is)		{ CChunkHeader::read (is); is >> m_ChildFormat; boci2n (m_ChildFormat); }
    inline void		write (ostream& os) const	{ CChunkHeader::write (os); os << bon2i(m_ChildFormat); }
    inline size_t	stream_size (void) const	{ return (CChunkHeader::stream_size() + stream_size_of(m_ChildFormat)); }
    inline fmt_t	ChildFormat (void) const	{ return (m_ChildFormat); }
    inline size_t	Size (void) const		{ return (CChunkHeader::Size() - (stream_size() - CChunkHeader::stream_size())); }
    void		Verify (fmt_t childFormat = cfmt_Generic, fmt_t fmt = cfmt_FORM, const char* chunkName = "FORM", uoff_t offset = 0) const;
private:
    fmt_t		m_ChildFormat;	///< Format of each child chunk.
};

//----------------------------------------------------------------------

/// \brief PROP section values
struct SProp {
    fmt_t	m_Name;		///< 4 character prop name (use IFF_FMT to define)
    uint32_t	m_Value;	///< Property value.
};

//----------------------------------------------------------------------

} // namespace iff

ALIGNOF(iff::CChunkHeader, IFF_GRAIN)
STD_STREAMABLE(iff::CChunkHeader)
STD_STREAMABLE(iff::CGroupHeader)

#endif
