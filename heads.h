// Copyright (c) 2003-2006 by Mike Sharov <msharov@users.sourceforge.net>
//
// iffheader.h
//
// 	Chunk header formats for the Interchange File Format.
//

#ifndef IFFHEADER_H_6AE64E742217B13E3F576C6D5A4C3292
#define IFFHEADER_H_6AE64E742217B13E3F576C6D5A4C3292

#include <ustl.h>

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

#if BYTE_ORDER == LITTLE_ENDIAN
    #define IFF_FMT(a,b,c,d)	(::iff::fmt_t)(((::iff::fmt_t)(d) << 24) | \
					 ((::iff::fmt_t)(c) << 16) | \
					 ((::iff::fmt_t)(b) << 8) | \
					 (::iff::fmt_t)(a))
#else
    #define IFF_FMT(a,b,c,d)	(::iff::fmt_t)(((::iff::fmt_t)(a) << 24) | \
					 ((::iff::fmt_t)(b) << 16) | \
					 ((::iff::fmt_t)(c) << 8) | \
					 (::iff::fmt_t)(d))
#endif
#define IFF_SFMT(s)		IFF_FMT(s[0],s[1],s[2],s[3])

/// \todo These should be in a configuration file somewhere
extern const fmt_t cfmt_Unknown;
extern const fmt_t cfmt_Container;
extern const fmt_t cfmt_SimpleContainer;
extern const fmt_t cfmt_CompoundContainer;
extern const fmt_t cfmt_Bitmap;
extern const fmt_t cfmt_Properties;
extern const fmt_t cfmt_BitmapHeader;
extern const fmt_t cfmt_ColorMap;
extern const fmt_t cfmt_ColorLookupTable;
extern const fmt_t cfmt_ColorCycle1;
extern const fmt_t cfmt_ColorCycle2;
extern const fmt_t cfmt_HotSpot;
extern const fmt_t cfmt_Sprite;
extern const fmt_t cfmt_VoiceHeader;
extern const fmt_t cfmt_Name;
extern const fmt_t cfmt_Copyright;
extern const fmt_t cfmt_Author;
extern const fmt_t cfmt_Annotation;
extern const fmt_t cfmt_Attack;
extern const fmt_t cfmt_Release;
extern const fmt_t cfmt_Generic;

// Extension chunk formats
extern const fmt_t cfmt_Vector;
extern const fmt_t cfmt_StringTable;
extern const fmt_t cfmt_Autodetect;

//----------------------------------------------------------------------

/// \class CChunkHeader iffheader.h iff.h
///
/// \brief Header for non-aggregate chunks.
///
/// Contains size and format information for the chunk. Like
/// CContainerHeader, the written format is the spec too, so
/// can be written manually. The size is the size of the
/// entire chunk, including the header (to simplify size
/// computations). The format is entirely user-defined.
///
class CChunkHeader {
public:
			CChunkHeader (void);
			CChunkHeader (chsize_t size, fmt_t fmt = cfmt_Generic);
    void		read (istream& is);
    void		write (ostream& os) const;
    inline size_t	stream_size (void) const	{ return (stream_size_of(m_Format) + stream_size_of(m_Size)); }
    inline size_t	DataSize (void) const		{ return (m_Size - stream_size()); }
    void		Verify (fmt_t fmt = cfmt_Generic, const char* chunkName = "chunk", uoff_t offset = 0);
public:
    fmt_t		m_Format;	///< Format of the chunk
    chsize_t		m_Size;		///< Size of the chunk including the header.
};

//----------------------------------------------------------------------

/// \class CVectorHeader iffheader.h iff.h
///
/// \brief Header for aggregate chunks without element count.
///
/// ContainerHeader has the child count, which is redunant information
/// if the data is already stored in a vector, or some such container.
/// Hence this header will omit the child count.
///
class CVectorHeader : public CChunkHeader {
public:
			CVectorHeader (void);
			CVectorHeader (chsize_t size, fmt_t childFormat = cfmt_Generic, fmt_t fmt = cfmt_Vector);
    void		read (istream& is);
    void		write (ostream& os) const;
    inline size_t	stream_size (void) const	{ return (CChunkHeader::stream_size() + stream_size_of(m_ChildFormat)); }
    inline size_t	DataSize (void) const		{ return (m_Size - stream_size()); }
    void		Verify (fmt_t childFormat = cfmt_Generic, fmt_t fmt = cfmt_Vector, const char* chunkName = "vector", uoff_t offset = 0);
public:
    fmt_t		m_ChildFormat;	///< Format of each child chunk.
};

//----------------------------------------------------------------------

/// \class CContainerHeader iffheader.h iff.h
///
/// \brief Header for aggregate chunks.
///
/// Chunks that contain other chunks are common and are thus standardized
/// with this class. Number of child chunks and their size are specified.
///
class CContainerHeader : public CVectorHeader {
public:
			CContainerHeader (void);
			CContainerHeader (chsize_t size, ccount_t nChildren, fmt_t childFormat = cfmt_Generic, fmt_t fmt = cfmt_SimpleContainer);
    void		read (istream& is);
    void		write (ostream& os) const;
    inline size_t	stream_size (void) const	{ return (CVectorHeader::stream_size() + stream_size_of(m_nChildren)); }
    inline size_t	DataSize (void) const		{ return (m_Size - stream_size()); }
    inline void		Verify (fmt_t childFormat = cfmt_Generic, fmt_t fmt = cfmt_SimpleContainer, const char* chunkName = "container", uoff_t offset = 0)	{ CVectorHeader::Verify (childFormat, fmt, chunkName, offset); }
public:
    ccount_t		m_nChildren;	///< Number of child chunks.
};

//----------------------------------------------------------------------

} // namespace iff

STD_STREAMABLE(iff::CChunkHeader)
STD_STREAMABLE(iff::CVectorHeader)
STD_STREAMABLE(iff::CContainerHeader)

#endif

