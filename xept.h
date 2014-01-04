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
			XFormatMismatch (const char* typeName, uoff_t offset, fmt_t expected, fmt_t actual) throw();
    virtual const char*	what (void) const throw() { return ("IFF format mismatch"); }
    virtual void	info (string& msgbuf, const char* fmt) const throw();
private:
    const char*		m_TypeName;
    uoff_t		m_Offset;
    fmt_t		m_Expected;
    fmt_t		m_Actual;
};

/// \class XChunkSizeMismatch xept.h iff.h
/// \brief Thrown when the calculated size of the block being read differs from the given size.
class XChunkSizeMismatch : public exception {
public:
			XChunkSizeMismatch (const char* typeName, uoff_t offset, size_t expected, size_t actual) throw();
    virtual const char*	what (void) const throw() { return ("IFF chunk size mismatch"); }
    virtual void	info (string& msgbuf, const char* fmt) const throw();
protected:
    const char*		m_TypeName;
    uoff_t		m_Offset;
    size_t		m_Expected;
    size_t		m_Actual;
};

} // namespace iff
