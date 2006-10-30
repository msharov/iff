include ../../Config.mk

LIBNAME	= iff
MAJOR	= 1
MINOR	= 0
BUILD	= 0
SRCS	= $(wildcard *.cc)
OBJS	= $(SRCS:.cc=.o)
INCS	= $(wildcard *.h) libiff.tbff
LIBS	= ${LANGLIBS}

include ../../mk/Comrulib.mk

