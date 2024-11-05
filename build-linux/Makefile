##..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
##
##  Copyright 2024 Douglas John Moore and Scott Meyer Peimann.
##
##  Licensed under the Apache License, Version 2.0 (the "License");
##  you may not use this file except in compliance with the License.
##  You may obtain a copy of the License at
## 
##      http://www.apache.org/licenses/LICENSE-2.0
##
##  Unless required by applicable law or agreed to in writing, software
##  distributed under the License is distributed on an "AS IS" BASIS,
##  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
##  See the License for the specific language governing permissions and
##  limitations under the License.
##
##..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
##
##  Makefile
##  - SystemC and UVM/SystemC example builds.
##  - Uses static library linking for SystemC and UVM/SystemC.
##  - Uses nearly all C++ compiler warnings, flagging them as errors,
##    to prevent latent bugs and enforce good coding practices.
##
##  NOTES:
##  1. The use of makedepend is work-in-progress, and is not of
##     production quality, yet.
##
##  HISTORY:
##  2024-09-24, Moore, Peimann - created
##
##..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../

##--basic tools' configuration
SHELL = bash

##--Project setup
PROJECT_SOURCE = ../source
PROJECT_INCLUDE += -I ${PROJECT_SOURCE}
PROJECT_INCLUDE += -I ${PROJECT_SOURCE}/include

##--SystemC setup
##--NB: disabled errors for various items as the SystemC reference
##--    implementation fails, otherwise.
SYSTEMC_BASE = ../systemc-2.3.4

SYSTEMC_INCLUDE += -I ${SYSTEMC_BASE}/include

##STEMC_LIB_DIR += -L ${SYSTEMC_BASE}/lib-linux64
SYSTEMC_LIBS += ${SYSTEMC_BASE}/lib-linux64/libsystemc.a

##--UVM/SystemC setup
UVM_SYSTEMC_BASE += ../uvm-systemc-1.0-beta5

UVM_SYSTEMC_INCLUDE += -I ${UVM_SYSTEMC_BASE}/include

##M_SYSTEMC_LIB_DIR += -L ${UVM_SYSTEMC_BASE}/lib-linux64
UVM_SYSTEMC_LIBS += ${UVM_SYSTEMC_BASE}/lib-linux64/libuvm-systemc.a

##--C++ general setup
CXXFLAGS += -std=c++20
CXXFLAGS += ${PROJECT_INCLUDE}
CXXFLAGS += ${SYSTEMC_INCLUDE}
CXXFLAGS += ${UVM_SYSTEMC_INCLUDE}

##--C++ warnings for everything C++
CXXFLAGS += -fmax-errors=4
CXXFLAGS += -pedantic-errors
CXXFLAGS += -Werror
CXXFLAGS += -Wpedantic -Wall -Wextra
CXXFLAGS += -Wno-comma-subscript
##XFLAGS += -Wabi                       ## not applicable
CXXFLAGS += -Walloca
##XFLAGS += -Walloc-size                ## g++-13 and later
CXXFLAGS += -Walloc-size-larger-than=65535
CXXFLAGS += -Walloc-zero
CXXFLAGS += -Warith-conversion
CXXFLAGS += -Warray-bounds=2            ## g++-13 use 3
CXXFLAGS += -Wattribute-alias=2
CXXFLAGS += -Wbidi-chars=any
CXXFLAGS += -Wcast-align=strict
CXXFLAGS += -Wcast-qual
CXXFLAGS += -Wconversion
CXXFLAGS += -Wcoverage-mismatch
CXXFLAGS += -Wdate-time
CXXFLAGS += -Wdeprecated
CXXFLAGS += -Wdisabled-optimization
CXXFLAGS += -Wdouble-promotion
CXXFLAGS += -Wduplicated-branches
CXXFLAGS += -Wduplicated-cond
CXXFLAGS += -Weffc++
CXXFLAGS += -Wenum-conversion
##XFLAGS += -Wflex-array-member-not-at-end  ## g++-13 and later
CXXFLAGS += -Wfloat-equal
CXXFLAGS += -Wformat=2
CXXFLAGS += -Wformat-overflow=2
CXXFLAGS += -Wformat-signedness
CXXFLAGS += -Wformat-truncation=2
CXXFLAGS += -Wframe-larger-than=65535
CXXFLAGS += -Wimplicit-fallthrough=5
CXXFLAGS += -Winline
CXXFLAGS += -Winterference-size
CXXFLAGS += -Winvalid-pch
##XFLAGS += -Winvalid-utf8              ## g++-13 and later
CXXFLAGS += -Wlarger-than=65535
CXXFLAGS += -Wlogical-op
CXXFLAGS += -Wmissing-attributes
CXXFLAGS += -Wmissing-declarations
CXXFLAGS += -Wmissing-include-dirs
CXXFLAGS += -Wmissing-noreturn
CXXFLAGS += -Wnull-dereference
CXXFLAGS += -Wpacked
##XFLAGS += -Wpedantic-ms-format        ## g++-13 and later
CXXFLAGS += -Wredundant-decls
CXXFLAGS += -Wreturn-local-addr
CXXFLAGS += -Wshadow=global
CXXFLAGS += -Wshift-negative-value
CXXFLAGS += -Wshift-overflow=2
CXXFLAGS += -Wsign-conversion
CXXFLAGS += -Wstack-usage=65535
CXXFLAGS += -Wstrict-aliasing=3
##XFLAGS += -Wstrict-flex-arrays=3      ## g++-13 and later
CXXFLAGS += -Wstrict-overflow=5
CXXFLAGS += -Wstringop-overflow=4
CXXFLAGS += -Wstringop-truncation
CXXFLAGS += -Wsuggest-attribute=cold
CXXFLAGS += -Wsuggest-attribute=const
CXXFLAGS += -Wsuggest-attribute=format
CXXFLAGS += -Wsuggest-attribute=malloc
CXXFLAGS += -Wsuggest-attribute=noreturn
CXXFLAGS += -Wsuggest-attribute=pure
##XFLAGS += -Wsuggest-attribute=returns_nonnull  ## g++-13 and later
CXXFLAGS += -Wswitch-default
CXXFLAGS += -Wswitch-enum
CXXFLAGS += -Wtrampolines
CXXFLAGS += -Wtrivial-auto-var-init
CXXFLAGS += -Wundef
CXXFLAGS += -Wunknown-pragmas
CXXFLAGS += -Wunsafe-loop-optimizations
CXXFLAGS += -Wunused-but-set-parameter
CXXFLAGS += -Wunused-const-variable=1   ## NB: =2 gives superfluous warnings
CXXFLAGS += -Wunused-macros
CXXFLAGS += -Wunused-parameter
CXXFLAGS += -Wuse-after-free=3
CXXFLAGS += -Wuseless-cast
CXXFLAGS += -Wvector-operation-performance
CXXFLAGS += -Wvla
##XFLAGS += -Wnrvo                      ## g++-13 only

##--linker general setup
##--NB: UVM must appear before SystemC
LDFLAGS += ${UVM_SYSTEMC_LIB_DIR}
LDFLAGS += ${SYSTEMC_LIB_DIR}

LDLIBS += ${UVM_SYSTEMC_LIBS}
LDLIBS += ${SYSTEMC_LIBS}

##--VPATH is your friend?
vpath %.cpp ${PROJECT_SOURCE}

##--makedepend support
MAKEFILE_DEPEND = Makefile.depend

##--phony targets
.phony: all
.phony: clean
.phony: depend
.phony: tests
.phony: tests_systemc
.phony: tests_uvm

##..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../

#all: tests depend
all: tests

clean:
	$(RM)  test-EXAMPLE-single_cycle test-EXAMPLE-single_cycle.o
	$(RM)  uvm-EXAMPLE-single_cycle uvm-EXAMPLE-single_cycle.o
	$(RM)  "${MAKEFILE_DEPEND}" "${MAKEFILE_DEPEND}.bak"

##..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../

tests: tests_systemc tests_uvm

tests_systemc: test-EXAMPLE-single_cycle

tests_uvm: uvm-EXAMPLE-single_cycle

##..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../

ifeq '${shell [ -e "${MAKEFILE_DEPEND}" ]; echo $$?}' '0'
include ${MAKEFILE_DEPEND}
endif

##..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../

depend:
	## bootstrap, if required
	[[ -e "${MAKEFILE_DEPEND}" ]] || touch "${MAKEFILE_DEPEND}"
	## NB: to get compiler include dir list: gcc -xc++ -E -v /dev/null
	## this causes makedepend to barf:  -- $${CFLAGS} $${CXXFLAGS}
	makedepend \
	  -Y \
	  ${PROJECT_INCLUDE} \
	  ${SYSTEMC_INCLUDE} \
	  ${UVM_SYSTEMC_INCLUDE} \
	  -f "${MAKEFILE_DEPEND}" \
	  -- ${PROJECT_SOURCE}/uvm-EXAMPLE-single_cycle.cpp

##..1..../....2..../....3..../....4..../....5..../....6..../....7..../....8..../
##
##--end of file
##
