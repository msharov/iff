include Config.mk

SRCS	= $(wildcard *.cc)
OBJS	= $(SRCS:.cc=.o)
INCS	= $(filter-out bsconf.%,$(wildcard *.h)) libiff.tbff

################ Library link names ####################################

TOCLEAN	+= ${LIBSO} ${LIBA} ${LIBSOBLD}

ALLINST	= install-incs
ifdef BUILD_SHARED
ALLLIBS	+= ${LIBSOBLD}
ALLINST	+= install-shared
endif
ifdef BUILD_STATIC
ALLLIBS	+= ${LIBA}
ALLINST	+= install-static
endif

################ Compilation ###########################################

.PHONY:	all install uninstall install-incs uninstall-incs
.PHONY: install-static install-shared uninstall-static uninstall-shared
.PHONY:	clean depend html dist distclean maintainer-clean

all:	${ALLLIBS}

%.o:	%.cc
	@echo "    Compiling $< ..."
	@${CXX} ${CXXFLAGS} -o $@ -c $<

%.s:	%.cc
	@echo "    Compiling $< to assembly ..."
	@${CXX} ${CXXFLAGS} -S -o $@ -c $<

${LIBA}:	${OBJS}
	@echo "Linking $@ ..."
	@${AR} r $@ $?
	@${RANLIB} $@

${LIBSOBLD}:	${OBJS}
	@echo "Linking $@ ..."
	@${LD} ${LDFLAGS} ${SHBLDFL} -o $@ $^ ${LIBS}

html:
	@${DOXYGEN} ${DOCT}

################ Installation ##########################################

install:	${ALLINST}
uninstall:	$(subst install,uninstall,${ALLINST})

install-shared: ${LIBSOBLD}
	@echo "Installing ${LIBSOBLD} to ${LIBDIR} ..."
	@${INSTALLDIR} ${LIBDIR}
	@${INSTALLLIB} ${LIBSOBLD} ${LIBDIR}
	@(cd ${LIBDIR}; \
	    rm -f ${LIBSO} ${LIBSOLNK}; \
	    ln -sf ${LIBSOBLD} ${LIBSO}; \
	    ln -sf ${LIBSOBLD} ${LIBSOLNK})

uninstall-shared:
	@echo "Removing ${LIBSOBLD} from ${LIBDIR} ..."
	@(cd ${LIBDIR}; \
	    rm -f ${LIBSO} ${LIBSOLNK} ${LIBSOBLD})

install-static: ${LIBA}
	@echo "Installing ${LIBA} to ${LIBDIR} ..."
	@${INSTALLDIR} ${LIBDIR}
	@${INSTALLLIB} ${LIBA} ${LIBDIR}

uninstall-static:
	@echo "Removing ${LIBA} from ${LIBDIR} ..."
	@rm -f ${LIBDIR}/${LIBA}

install-incs: ${INCS}
	@echo "Installing headers to ${INCDIR} ..."
	@${INSTALLDIR} ${INCDIR}/${LIBNAME}
	@for i in $(filter-out ${LIBNAME}.h,${INCS}); do	\
	    ${INSTALLDATA} $$i ${INCDIR}/${LIBNAME}/$$i;	\
	done;
	@${INSTALLDATA} ${LIBNAME}.h ${INCDIR}

uninstall-incs:
	@echo "Removing headers from ${INCDIR} ..."
	@rm -rf ${INCDIR}/${LIBNAME} ${INCDIR}/${LIBNAME}.h

################ Maintenance ###########################################

clean:
	@echo "Removing generated files ..."
	@rm -f ${OBJS} ${TOCLEAN}

depend: ${SRCS}
	@${CXX} ${CXXFLAGS} -M ${SRCS} > .depend;

TMPDIR	= /tmp
DISTDIR	= ${HOME}/stored
ifeq (${BUILD},0)
DISTVER	= ${MAJOR}.${MINOR}
else
DISTVER	= ${MAJOR}.${MINOR}.${BUILD}
endif
DISTNAM	= ${LIBNAME}-${DISTVER}
DISTLSM	= ${DISTNAM}.lsm
DISTTAR	= ${DISTNAM}.tar.bz2
DDOCTAR	= ${DISTNAM}-docs.tar.bz2

dist:
	mkdir ${TMPDIR}/${DISTNAM}
	cp -r . ${TMPDIR}/${DISTNAM}
	+${MAKE} -C ${TMPDIR}/${DISTNAM} distclean
	(cd ${TMPDIR}/${DISTNAM}; rm -rf docs/html .git)
	(cd ${TMPDIR}; tar jcf ${DISTDIR}/${DISTTAR} ${DISTNAM}; rm -rf ${DISTNAM})

distclean:	clean
	@rm -f Config.mk config.h bsconf.o bsconf .depend

maintainer-clean: distclean
	@rm -rf docs/html

-include .depend
 
