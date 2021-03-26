// This file is part of the interchange file format library.
//
// Copyright (c) 2003 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.

#pragma once
#include "heads.h"

namespace iff {

//----------------------------------------------------------------------
// Exception definitions
//----------------------------------------------------------------------

/// \class XFormatMismatch xept.h iff.h
/// \brief Thrown when the format of the block being read does not match the format requested.
class XFormatMismatch : public exception {
public:
		XFormatMismatch (const char* typeName, uoff_t offset, fmt_t expected, fmt_t actual) noexcept;
    const char*	what (void) const noexcept override { return ("IFF format mismatch"); }
    void	info (string& msgbuf, const char* fmt) const noexcept override;
private:
    const char*	_typename;
    uoff_t	_offset;
    fmt_t	_expected;
    fmt_t	_actual;
};

/// \class XChunkSizeMismatch xept.h iff.h
/// \brief Thrown when the calculated size of the block being read differs from the given size.
class XChunkSizeMismatch : public exception {
public:
		XChunkSizeMismatch (const char* typeName, uoff_t offset, size_t expected, size_t actual) noexcept;
    const char*	what (void) const noexcept override { return ("IFF chunk size mismatch"); }
    void	info (string& msgbuf, const char* fmt) const noexcept override;
protected:
    const char*	_typename;
    uoff_t	_offset;
    size_t	_expected;
    size_t	_actual;
};

} // namespace iff
