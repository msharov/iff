// This file is part of the interchange file format library.
//
// Copyright (C) 2003-2006 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.
//
// utils.h
//
// Utility functions for Interchange File Format.
//

#ifndef UTILS_H_1A23A18571746C4F243976256D15804A
#define UTILS_H_1A23A18571746C4F243976256D15804A

#include "xept.h"

namespace iff {

void ReadChunkHeader (const char* typeName, istream& is, CChunkHeader& header, fmt_t fmt);
void ReadGroupHeader (const char* typeName, istream& is, CGroupHeader& header, fmt_t childFmt, fmt_t fmt);
void VerifyChunkSize (const char* typeName, uoff_t chStart, uoff_t chEnd, size_t chSize); 

template <typename Chunk>
void ReadChunk (istream& is, Chunk& el, fmt_t fmt = cfmt_Autodetect)
{
    CChunkHeader header;
    const uoff_t elStart = is.pos();
    ReadChunkHeader (typeid(el).name(), is, header, fmt);
    is >> el >> ios::talign<CChunkHeader>();
    VerifyChunkSize (typeid(el).name(), elStart, is.pos(), header.SizeWithHeader());
}

template <typename T>
void ReadVector (istream& is, T& v, fmt_t childFmt = cfmt_Autodetect, fmt_t fmt = cfmt_Autodetect)
{
    CGroupHeader header;
    const uoff_t elStart = is.pos();
    ReadGroupHeader (typeid(v).name(), is, header, childFmt, fmt);
    is >> v;
    VerifyChunkSize (typeid(v).name(), elStart, is.pos(), header.SizeWithHeader());
}

template <typename T>
void WriteChunk (ostream& os, const T& v, fmt_t fmt = cfmt_Generic)
{
    const size_t elSize = Align(stream_size_of(v), alignof(CChunkHeader()));
    const CChunkHeader header (elSize, fmt);
    #ifndef NDEBUG
	const uoff_t elStart = os.pos();
    #endif
    os << header << v << ios::talign<CChunkHeader>();
    #ifndef NDEBUG
	VerifyChunkSize (typeid(v).name(), elStart, os.pos(), header.SizeWithHeader());
    #endif
}

template <typename T>
void WriteVector (ostream& os, const T& v, fmt_t childFmt = cfmt_Generic, fmt_t fmt = cfmt_Vector)
{
    const CGroupHeader header (stream_size_of(v), childFmt, fmt);
    #ifndef NDEBUG
	const uoff_t elStart = os.pos();
    #endif
    os << header << v;
    #ifndef NDEBUG
	VerifyChunkSize (typeid(v).name(), elStart, os.pos(), header.SizeWithHeader());
    #endif
}

void SkipChunk (istream& is, fmt_t fmt = cfmt_Autodetect);

inline void SkipChunk (ostream& os, fmt_t fmt = cfmt_Generic)
{
    os << CChunkHeader (0, fmt);
}

template <typename T>
inline size_t chunk_size_of (const T& v)
{
    return (Align (CChunkHeader().stream_size() + stream_size_of(v), alignof (CChunkHeader())));
}

template <typename T>
inline size_t vector_size_of (const T& v)
{
    return (CGroupHeader().stream_size() + stream_size_of(v));
}

} // namespace iff

#endif

