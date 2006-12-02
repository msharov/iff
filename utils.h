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
void ReadVectorHeader (const char* typeName, istream& is, CVectorHeader& header, fmt_t childFmt, fmt_t fmt);
void ReadContainerHeader (const char* typeName, istream& is, CContainerHeader& header, fmt_t childFmt, fmt_t fmt);
void VerifyChunkSize (const char* typeName, uoff_t chStart, uoff_t chEnd, size_t chSize); 

template <typename Chunk>
void ReadChunk (istream& is, Chunk& el, fmt_t fmt = cfmt_Autodetect)
{
    CChunkHeader header;
    const uoff_t elStart = is.pos();
    ReadChunkHeader (typeid(el).name(), is, header, fmt);
    is >> el >> ios::talign<CChunkHeader>();
    VerifyChunkSize (typeid(el).name(), elStart, is.pos(), header.m_Size);
}

template <class Vector>
void ReadVector (istream& is, Vector& ctr, fmt_t childFmt = cfmt_Autodetect, fmt_t fmt = cfmt_Autodetect)
{
    CVectorHeader header;
    const uoff_t elStart = is.pos();
    ReadVectorHeader (typeid(ctr).name(), is, header, childFmt, fmt);
    is >> ctr;
    VerifyChunkSize (typeid(ctr).name(), elStart, is.pos(), header.m_Size);
}

template <class Container>
void ReadContainer (istream& is, Container& ctr, fmt_t childFmt = cfmt_Autodetect, fmt_t fmt = cfmt_Autodetect)
{
    CContainerHeader header;
    const uoff_t elStart = is.pos();
    ReadContainerHeader (typeid(ctr).name(), is, header, childFmt, fmt);
    ctr.resize (header.m_nChildren);
    foreach (typename Container::iterator, i, ctr)
	ReadChunk (is, *i, childFmt);
    VerifyChunkSize (typeid(ctr).name(), elStart, is.pos(), header.m_Size);
}

template <typename Chunk>
void WriteChunk (ostream& os, const Chunk& el, fmt_t fmt = cfmt_Generic)
{
    const size_t elSize = Align(stream_size_of(el), alignof(CChunkHeader()));
    const CChunkHeader header (elSize, fmt);
    #ifndef NDEBUG
	const uoff_t elStart = os.pos();
    #endif
    os << header << el << ios::talign<CChunkHeader>();
    #ifndef NDEBUG
	VerifyChunkSize (typeid(el).name(), elStart, os.pos(), header.m_Size);
    #endif
}

template <class Vector>
void WriteVector (ostream& os, const Vector& ctr, fmt_t childFmt = cfmt_Generic, fmt_t fmt = cfmt_Vector)
{
    const CVectorHeader header (stream_size_of(ctr), childFmt, fmt);
    #ifndef NDEBUG
	const uoff_t elStart = os.pos();
    #endif
    os << header << ctr;
    #ifndef NDEBUG
	VerifyChunkSize (typeid(ctr).name(), elStart, os.pos(), header.m_Size);
    #endif
}

template <class Container>
void WriteContainer (ostream& os, const Container& ctr, fmt_t childFmt = cfmt_Generic, fmt_t fmt = cfmt_Container)
{
    const CVectorHeader header (stream_size_of(ctr), childFmt, fmt);
    #ifndef NDEBUG
	const uoff_t elStart = os.pos();
    #endif
    os << header;
    os << chsize_t (ctr.size());
    foreach (typename Container::const_iterator, i, ctr)
	WriteChunk (os, *i, childFmt);
    #ifndef NDEBUG
	VerifyChunkSize (typeid(ctr).name(), elStart, os.pos(), header.m_Size);
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
    return (Align (CChunkHeader().stream_size() + stream_size_of (v), alignof (CChunkHeader())));
}

template <typename T>
inline size_t vector_size_of (const T& v)
{
    return (CVectorHeader().stream_size() + stream_size_of (v));
}

template <typename T>
inline size_t container_size_of (const T& v)
{
    return (CContainerHeader().stream_size() + stream_size_of (v));
}

} // namespace iff

#endif

