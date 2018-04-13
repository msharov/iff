################ Source files ##########################################

test/SRCS	:= $(wildcard test/?????.cc)
test/BINS	:= $(addprefix $O,$(test/SRCS:.cc=))
test/OBJS	:= $(addprefix $O,$(test/SRCS:.cc=.o)) $Otest/stdmain.o
test/DEPS	:= ${test/OBJS:.o=.d}
test/OUTS	:= $(addprefix $O,$(test/SRCS:.cc=.out))
test/LIBS	+= ${LIBA} ${LIBS}

################ Compilation ###########################################

.PHONY:	test/all test/run test/clean test/check

test/all:	${test/BINS}

# The correct output of a test is stored in testXX.std
# When the test runs, its output is compared to .std
#
test/run:	${test/BINS}
	@for i in ${test/BINS}; do \
	    echo "Running $$i"; \
	    $$i < /dev/null > $$i.out 2>&1; \
	    diff test/`basename $$i`.std $$i.out && rm -f $$i.out; \
	done

${test/BINS}: $Otest/%: $Otest/%.o $Otest/stdmain.o ${ALLTGTS}
	@echo "Linking $@ ..."
	@${CC} ${LDFLAGS} -o $@ $< $Otest/stdmain.o ${test/LIBS}

$Otest/.d:	$O.d
	@[ -d $Otest ] || mkdir $Otest
	@touch $@

################ Maintenance ###########################################

clean:	test/clean
test/clean:
	@if [ -d $Otest ]; then\
	    rm -f ${test/BINS} ${test/OBJS} ${test/DEPS} ${test/OUTS} $Otest/.d;\
	    rmdir $Otest;\
	fi
check:		test/run
test/check:	check

${test/OBJS}:	${ALLTGTS} $Otest/.d

-include ${test/DEPS}
