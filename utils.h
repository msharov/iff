// This file is part of the interchange file format library.
//
// Copyright (C) 2003-2009 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.

#ifndef UTILS_H_1A23A18571746C4F243976256D15804A
#define UTILS_H_1A23A18571746C4F243976256D15804A

#include "xept.h"

namespace iff {

//----------------------------------------------------------------------

void ReadChunkHeader (const char* typeName, istream& is, CChunkHeader& header, fmt_t fmt);
void ReadGroupHeader (const char* typeName, istream& is, CGroupHeader& header, fmt_t childFmt, fmt_t fmt);
void VerifyChunkSize (const char* typeName, uoff_t chStart, uoff_t chEnd, size_t chSize); 

//----------------------------------------------------------------------

/// Reads \p v from \p is as a chunk of given \p fmt.
template <typename Chunk>
void ReadChunk (istream& is, Chunk& v, fmt_t fmt)
{
    CChunkHeader header;
    const uoff_t vStart = is.pos();
    ReadChunkHeader (typeid(v).name(), is, header, fmt);
    is >> v >> ios::talign<CChunkHeader>();
    VerifyChunkSize (typeid(v).name(), vStart, is.pos(), header.SizeWithHeader());
}

/// Reads \p v from \p is as a group chunk of given \p fmt with elements of type \p childFmt.
template <typename T>
void ReadGroup (istream& is, T& v, fmt_t childFmt, fmt_t fmt)
{
    CGroupHeader header;
    const uoff_t vStart = is.pos();
    ReadGroupHeader (typeid(v).name(), is, header, childFmt, fmt);
    is >> v;
    VerifyChunkSize (typeid(v).name(), vStart, is.pos(), header.SizeWithHeader());
}

#define IFF_GROUP_READER(gtype)	\
template <typename T>		\
inline void Read##gtype (istream& is, T& v, fmt_t childFmt)	\
    { ReadGroup (is, v, childFmt, cfmt_##gtype); }
IFF_GROUP_READER(Vector)
IFF_GROUP_READER(FORM)
IFF_GROUP_READER(LIST)
IFF_GROUP_READER(CAT)
#undef IFF_GROUP_READER

//----------------------------------------------------------------------

/// Writes \p v to \p os as a chunk of given \p fmt.
template <typename T>
void WriteChunk (ostream& os, const T& v, fmt_t fmt)
{
    const size_t vSize = Align(stream_size_of(v), alignof(CChunkHeader()));
    const CChunkHeader header (vSize, fmt);
    #ifndef NDEBUG
	const uoff_t vStart = os.pos();
    #endif
    os << header << v << ios::talign<CChunkHeader>();
    #ifndef NDEBUG
	VerifyChunkSize (typeid(v).name(), vStart, os.pos(), header.SizeWithHeader());
    #endif
}

/// Writes \p v to \p os as a group chunk of given \p fmt with elements of type \p childFmt.
template <typename T>
void WriteGroup (ostream& os, const T& v, fmt_t childFmt, fmt_t fmt)
{
    const CGroupHeader header (stream_size_of(v), childFmt, fmt);
    #ifndef NDEBUG
	const uoff_t vStart = os.pos();
    #endif
    os << header << v;
    #ifndef NDEBUG
	VerifyChunkSize (typeid(v).name(), vStart, os.pos(), header.SizeWithHeader());
    #endif
}

#define IFF_GROUP_WRITER(gtype)	\
template <typename T>		\
inline void Write##gtype (ostream& os, const T& v, fmt_t childFmt)	\
    { WriteGroup (os, v, childFmt, cfmt_##gtype); }
IFF_GROUP_WRITER(Vector)
IFF_GROUP_WRITER(FORM)
IFF_GROUP_WRITER(LIST)
IFF_GROUP_WRITER(CAT)
#undef IFF_GROUP_WRITER

//----------------------------------------------------------------------

/// Inspects a group chunk header at current position in \p is and returns its child chunk format without changing \p is.
inline fmt_t PeekGroupFormat (const istream& is)
{
    assert (is.aligned (alignof (CChunkHeader())) && "Current stream offset is not at a chunk header");
    return (*(const fmt_t*)(is.ipos() + stream_size_of(CChunkHeader())));
}

/// Inspects a chunk header at current position in \p is and returns its format without changing \p is.
inline fmt_t PeekChunkFormat (const istream& is)
{
    assert (is.aligned (alignof (CChunkHeader())) && "Current stream offset is not at a chunk header");
    return (*(const fmt_t*)(is.ipos()));
}

/// \brief Calls #PeekChunkFormat and returns #PeekGroupFormat for group chunks.
///
/// This is useful as a generalized format query when you already know
/// which chunk formats are group chunks and which aren't.
///
inline fmt_t PeekChunkOrGroupFormat (const istream& is)
{
    fmt_t f = PeekChunkFormat (is);
    if ((f == cfmt_FORM) | (f == cfmt_LIST) | (f == cfmt_CAT) | (f == cfmt_Vector))
	f = PeekGroupFormat (is);
    return (f);
}

void SkipChunk (istream& is, fmt_t fmt = cfmt_Autodetect);

/// Writes a null chunk of \p fmt to \p os.
inline void SkipChunk (ostream& os, fmt_t fmt = cfmt_Generic)
{
    os << CChunkHeader (0, fmt);
}

/// Returns the stream size required to #WriteChunk \p v.
template <typename T>
inline size_t chunk_size_of (const T& v)
{
    return (Align (CChunkHeader().stream_size() + stream_size_of(v), alignof (CChunkHeader())));
}

#define IFF_GROUP_SIZE_OF(gtype)		\
template <typename T>				\
inline size_t gtype##_size_of (const T& v)	\
    { return (CGroupHeader().stream_size() + stream_size_of(v)); }
IFF_GROUP_SIZE_OF(vector)
IFF_GROUP_SIZE_OF(form)
IFF_GROUP_SIZE_OF(list)
IFF_GROUP_SIZE_OF(cat)
#undef IFF_GROUP_SIZE_OF

//----------------------------------------------------------------------

} // namespace iff

#endif
