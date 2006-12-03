/* This file is part of bsconf - a configure replacement.
 *
 * This is the configuration file used by bsconf.c to specify information
 * specific to your project that it needs to substitute into files listed
 * in g_Files. Being a configuration file, this file can be used or
 * modified entirely without restriction. You should change all values
 * appropriately to the name of your project and its requirements. The
 * LGPL does not apply to this file. It can and should be treated as a
 * template for the creation of your own configuration file.
 *
 * All substituted variable names are given without enclosing @@. For
 * example: "CC" will match "@CC@" in config.h.in and replace it with
 * "gcc" in config.h.
*/

#define BSCONF_VERSION		0x03

#define PACKAGE_NAME		"iff"
#define LIB_MAJOR		"0"
#define LIB_MINOR		"1"
#define LIB_BUILD		"0"

#define PACKAGE_VERSION		LIB_MAJOR "." LIB_MINOR
#define PACKAGE_TARNAME		PACKAGE_NAME
#define PACKAGE_STRING		PACKAGE_NAME " " PACKAGE_VERSION
#define PACKAGE_BUGREPORT	"Mike Sharov <msharov@users.sourceforge.net>"

static cpchar_t g_Files [] = {
    "Config.mk",
    "config.h"
};

/* Values substitute @VARNAME@ */
static cpchar_t g_EnvVars [] = {
    "CXX",
    "LD",
    "CXXFLAGS",
    "LDFLAGS"
};

/*  VARIABLE	PROGRAM		HOW TO CALL	IF NOT FOUND */
static cpchar_t g_ProgVars [] = {
    "CXX",	"g++",		"g++",		"@CXX@",
    "CXX",	"c++",		"c++",		"g++",
    "LD",	"ld",		"ld",		"ld",
    "AR",	"ar",		"ar",		"echo",
    "RANLIB",	"ranlib",	"ranlib",	"touch",
    "DOXYGEN",	"doxygen",	"doxygen",	"echo",
    "INSTALL",	"install",	"install -c",	"cp"
};

/*   NAME	IF NOT FOUND		IF FOUND */
static cpchar_t	g_Headers [] = {
    "ustl.h",	"#undef HAVE_USTL_H",	"#define HAVE_USTL_H 1"
};

/*   NAME	IF NOT FOUND	IF FOUND */
static cpchar_t g_Libs [] = {
    "supc++",	"",		"-lsupc++",
    "ustl",	"",		"-lustl"
};

/*   NAME	IF NOT FOUND	IF FOUND */
static cpchar_t g_Functions [] = {
};

/*   NAME		WITHOUT TEXT		WITH TEXT */
static cpchar_t g_Components [] = {
    "shared",		"#BUILD_SHARED\t= 1",	"BUILD_SHARED\t= 1 ",
    "static",		"#BUILD_STATIC\t= 1",	"BUILD_STATIC\t= 1 ",
    "debug",		"#DEBUG\t\t= 1",	"DEBUG\t\t= 1 ",
    "bigendian",	"USTL_LITTLE_ENDIAN",	"USTL_BIG_ENDIAN",
    "2grain",		"#define IFF_GRAIN 4",	"#define IFF_GRAIN 2",
    "align",		"#define IFF_GRAIN 1",	"#define IFF_GRAIN 4"
};

/* Parallel to g_Components */
static SComponentInfo g_ComponentInfos [VectorSize(g_Components) / 3] = {
    { 1, "Builds the shared library (if supported by the OS)" },
    { 0, "Builds the static library" },
    { 0, "Compiles the library with debugging information" },
    { 0, "Store header fields in big-endian format, as in EA85" },
    { 0, "Align on 2-grain instead of 4-grain, as in EA85" },
    { 1, "Turn off chunk alignment" }
};

/* Substitutes names like @PACKAGE_NAME@ with the second field */
static cpchar_t g_CustomVars [] = {
    "GCC4_SIZEOPTS",
#if __GNUC__ >= 4
				"-fno-threadsafe-statics -fno-enforce-eh-specs",
#else
				"",
#endif
    "PACKAGE_NAME",		PACKAGE_NAME,
    "PACKAGE_VERSION",		PACKAGE_VERSION,
    "PACKAGE_TARNAME",		PACKAGE_TARNAME,
    "PACKAGE_STRING",		PACKAGE_STRING,
    "PACKAGE_BUGREPORT",	PACKAGE_BUGREPORT,
    "LIBNAME",			PACKAGE_NAME,
    "LIB_MAJOR",		LIB_MAJOR,
    "LIB_MINOR",		LIB_MINOR,
    "LIB_BUILD",		LIB_BUILD
};

