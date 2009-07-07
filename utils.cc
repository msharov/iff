// This file is part of the interchange file format library.
//
// Copyright (C) 2003-2009 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.

#include "utils.h"

namespace iff {

/// Reads chunk \p header from \p is and validates \p fmt.
void ReadChunkHeader (const char* typeName, istream& is, CChunkHeader& header, fmt_t fmt)
{
    is.verify_remaining ("iff::ReadChunkHeader", typeName, stream_size_of(header));
    const uoff_t elStart = is.pos();
    is >> header;
    header.Verify (fmt, typeName, elStart);
    is.verify_remaining ("iff::ReadChunkHeader", typeName, header.Size());
}

/// Reads vector \p header from \p is and validates \p fmt.
void ReadGroupHeader (const char* typeName, istream& is, CGroupHeader& header, fmt_t childFmt, fmt_t fmt)
{
    is.verify_remaining ("iff::ReadGroupHeader", typeName, stream_size_of(header));
    const uoff_t elStart = is.pos();
    is >> header;
    header.Verify (childFmt, fmt, typeName, elStart);
    is.verify_remaining ("iff::ReadGroupHeader", typeName, header.Size());
}

/// Throws XChunkSizeMismatch if chStart + chSize != chEnd
void VerifyChunkSize (const char* typeName, uoff_t chStart, uoff_t chEnd, size_t chSize)
{
    const size_t realSize = chEnd - chStart;
    if (chSize != realSize)
	throw XChunkSizeMismatch (typeName, chStart, chSize, realSize);
}

/// Skips chunk in \p is (by reading the header and skipping Size() bytes).
void SkipChunk (istream& is, fmt_t fmt)
{
    CChunkHeader header;
    ReadChunkHeader ("iff::SkipChunk", is, header, fmt);
    is.skip (header.Size());
}

} // namespace iff
