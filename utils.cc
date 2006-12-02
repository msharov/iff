// This file is part of the interchange file format library.
//
// Copyright (C) 2003-2006 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.
//
// utils.cc
//

#include "utils.h"

namespace iff {

/// Reads chunk \p header from \p is and validates \p fmt.
void ReadChunkHeader (const char* typeName, istream& is, CChunkHeader& header, fmt_t fmt)
{
    is.verify_remaining ("iff::ReadChunkHeader", typeName, stream_size_of(header));
    const uoff_t elStart = is.pos();
    is >> header;
    header.Verify (fmt, typeName, elStart);
    is.verify_remaining ("iff::ReadChunkHeader", typeName, header.DataSize());
}

/// Reads vector \p header from \p is and validates \p fmt.
void ReadVectorHeader (const char* typeName, istream& is, CVectorHeader& header, fmt_t childFmt, fmt_t fmt)
{
    is.verify_remaining ("iff::ReadVectorHeader", typeName, stream_size_of(header));
    const uoff_t elStart = is.pos();
    is >> header;
    header.Verify (childFmt, fmt, typeName, elStart);
    is.verify_remaining ("iff::ReadVectorHeader", typeName, header.DataSize());
}

/// Reads container \p header from \p is and validates \p fmt.
void ReadContainerHeader (const char* typeName, istream& is, CContainerHeader& header, fmt_t childFmt, fmt_t fmt)
{
    is.verify_remaining ("iff::ReadContainerHeader", typeName, stream_size_of(header));
    const uoff_t elStart = is.pos();
    is >> header;
    header.Verify (childFmt, fmt, typeName, elStart);
    is.verify_remaining ("iff::ReadContainerHeader", typeName, header.DataSize());
}

/// Throws XChunkSizeMismatch if chStart + chSize != chEnd
void VerifyChunkSize (const char* typeName, uoff_t chStart, uoff_t chEnd, size_t chSize)
{
    const size_t realSize = chEnd - chStart;
    if (chSize != realSize)
	throw XChunkSizeMismatch (typeName, chStart, chSize, realSize);
}

/// Skips chunk in \p is (by reading the header and skipping m_Size bytes).
void SkipChunk (istream& is, fmt_t fmt)
{
    CChunkHeader header;
    ReadChunkHeader ("iff::SkipChunk", is, header, fmt);
    is.skip (header.DataSize());
}

} // namespace iff

