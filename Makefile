-include Config.mk

################ Source files ##########################################

SRCS	:= $(wildcard *.cc)
INCS	:= $(filter-out ${NAME}.h,$(sort $(wildcard *.h) config.h))
OBJS	:= $(addprefix $O,$(SRCS:.cc=.o))
DEPS	:= ${OBJS:.o=.d}
CONFS	:= Config.mk config.h ${NAME}.pc
ONAME   := $(notdir $(abspath $O))
DOCS	:= $(notdir $(wildcard doc/*))
LIBA_R	:= $Olib${NAME}.a
LIBA_D	:= $Olib${NAME}_d.a
ifdef DEBUG
LIBA	:= ${LIBA_D}
else
LIBA	:= ${LIBA_R}
endif

################ Compilation ###########################################

.PHONY: all clean distclean maintainer-clean

all:	${LIBA}

${LIBA}:	${OBJS}
	@echo "Linking $@ ..."
	@rm -f $@
	@${AR} qc $@ $^
	@${RANLIB} $@

$O%.o:	%.cc
	@echo "    Compiling $< ..."
	@${CXX} ${CXXFLAGS} -MMD -MT "$(<:.cc=.s) $@" -o $@ -c $<

%.s:	%.cc
	@echo "    Compiling $< to assembly ..."
	@${CXX} ${CXXFLAGS} -S -o $@ -c $<

################ Installation ##########################################

.PHONY:	install uninstall

ifdef INCDIR
INCSI		:= $(addprefix ${INCDIR}/${NAME}/,${INCS})
INCR		:= ${INCDIR}/${NAME}.h
install:	${INCSI} ${INCR}
${INCSI}: ${INCDIR}/${NAME}/%.h: %.h
	@echo "Installing $@ ..."
	@${INSTALLDATA} $< $@
${INCR}:	${NAME}.h
	@echo "Installing $@ ..."
	@${INSTALLDATA} $< $@
uninstall:	uninstall-incs
uninstall-incs:
	@if [ -d ${INCDIR}/${NAME} ]; then\
	    echo "Removing headers ...";\
	    rm -f ${INCSI} ${INCR};\
	    ${RMPATH} ${INCDIR}/${NAME};\
	fi
endif
ifdef LIBDIR
LIBAI		:= ${LIBDIR}/$(notdir ${LIBA})
LIBAI_R		:= ${LIBDIR}/$(notdir ${LIBA_R})
LIBAI_D		:= ${LIBDIR}/$(notdir ${LIBA_D})
install:        ${LIBAI}
${LIBAI}:       ${LIBA}
	@echo "Installing $@ ..."
	@${INSTALLLIB} $< $@
uninstall:	uninstall-lib
uninstall-lib:
	@if [ -f ${LIBAI_R} -o -f ${LIBAI_D} ]; then\
	    echo "Removing ${LIBAI} ...";\
	    rm -f ${LIBAI_R} ${LIBAI_D};\
	fi
endif
ifdef DOCDIR
PKGDOCDIR	:= ${DOCDIR}/${NAME}
DOCSI		:= $(addprefix ${PKGDOCDIR}/,${DOCS})
install:	${DOCSI}
${DOCSI}: ${PKGDOCDIR}/%: doc/%
	@echo "Installing $@ ..."
	@${INSTALLDATA} $< $@
uninstall:	uninstall-docs
uninstall-docs:
	@if [ -d ${PKGDOCDIR} ]; then\
	    echo "Removing documentation ...";\
	    rm -f ${DOCSI};\
	    ${RMPATH} ${PKGDOCDIR};\
	fi
endif
ifdef PKGCONFIGDIR
PCI	:= ${PKGCONFIGDIR}/${NAME}.pc
install:	${PCI}
${PCI}:	${NAME}.pc
	@echo "Installing $@ ..."
	@${INSTALLDATA} $< $@

uninstall:	uninstall-pc
uninstall-pc:
	@if [ -f ${PCI} ]; then echo "Removing ${PCI} ..."; rm -f ${PCI}; fi
endif

################ Maintenance ###########################################

include test/Module.mk

clean:
	@if [ -h ${ONAME} ]; then\
	    rm -f ${LIBA_R} ${LIBA_D} ${OBJS} ${DEPS} $O.d ${ONAME};\
	    ${RMPATH} ${BUILDDIR};\
	fi

distclean:	clean
	@rm -f ${CONFS} config.status

maintainer-clean: distclean

$O.d:	${BUILDDIR}/.d
	@[ -h ${ONAME} ] || ln -sf ${BUILDDIR} ${ONAME}
$O%/.d:	$O.d
	@[ -d $(dir $@) ] || mkdir $(dir $@)
	@touch $@
${BUILDDIR}/.d:	Makefile
	@[ -d $(dir $@) ] || mkdir -p $(dir $@)
	@touch $@

Config.mk:	Config.mk.in
config.h:	config.h.in
${NAME}.pc:	${NAME}.pc.in
${OBJS}:	Makefile ${CONFS} $O.d config.h
${CONFS}:	configure
	@if [ -x config.status ]; then echo "Reconfiguring ...";\
	    ./config.status;\
	else echo "Running configure ...";\
	    ./configure;\
	fi

-include ${DEPS}
