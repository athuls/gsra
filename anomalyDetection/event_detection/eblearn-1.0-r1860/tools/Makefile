ifndef (prj)
prj := all
endif

# detect MAC OS
OSUPPER = $(shell uname -s 2>/dev/null | tr [:lower:] [:upper:])
OSLOWER = $(shell uname -s 2>/dev/null | tr [:upper:] [:lower:])
# 'linux' is output for Linux system, 'darwin' for OS X
DARWIN = $(strip $(findstring DARWIN, $(OSUPPER)))
ifneq ($(DARWIN),)
	LIB_EXT=dylib
else
	LIB_EXT=so
endif

#installation parameters
ifeq ($(DIR),)
 INSTALL_DIR=/usr
else
  FIRST=$(shell echo $(DIR) | cut -c1)
  ifeq ("$(FIRST)","/")
   INSTALL_DIR="$(DIR)"
  else
   HERE=$(shell pwd)
   INSTALL_DIR=$(HERE)/$(DIR)
  endif
endif

INCLUDE_INSTALL=$(INSTALL_DIR)/include/eblearn/
LIB_INSTALL=$(INSTALL_DIR)/lib/
BIN_INSTALL=$(INSTALL_DIR)/bin

INCLUDES="../core/libidx/include/*" "../core/libeblearn/include/*" \
"libidxgui/include/*" "libeblearngui/include/*" "libeblearntools/include/*"	
LIBS=libidx.${LIB_EXT} libidxgui.${LIB_EXT} libeblearn.${LIB_EXT} \
		       libeblearngui.${LIB_EXT} libeblearntools.${LIB_EXT}
BINS=train detect dscompile dssplit dsmerge matshow dsdisplay \
metarun metaparse capture dsfprop imfprop
DEMOS=mnist_train mnist_reco

all: release

release:
	@echo __________________________________________________________________
	@echo
	@echo "Compiling ${prj} project(s) in release mode..."
	@echo "Use 'make release prj=idxgui' for example to compile a specific project."
	@echo __________________________________________________________________
	mkdir -p build/Release 2> /dev/null
	cd build/Release &&  cmake -DCMAKE_BUILD_TYPE=Release ../.. \
	&& make ${prj} && cd ../..

libs:
	@echo ${LIB_EXT}
	@echo ${ARCH2}
	@echo __________________________________________________________________
	@echo
	@echo "Compiling libraries in release mode..."
	@echo __________________________________________________________________
	mkdir -p build/Release 2> /dev/null
	cd build/Release &&  cmake -DCMAKE_BUILD_TYPE=Release ../.. \
	&& make idx eblearn eblearntools && cd ../..

tool:
	@echo __________________________________________________________________
	@echo
	@echo "Compiling tools in release mode..."
	@echo __________________________________________________________________
	mkdir -p build/Release 2> /dev/null
	cd build/Release &&  cmake -DCMAKE_BUILD_TYPE=Release ../.. \
	&& make ${BINS} && cd ../..

demo:
	@echo __________________________________________________________________
	@echo
	@echo "Compiling Demos in release mode..."
	@echo __________________________________________________________________
	mkdir -p build/Release 2> /dev/null
	cd build/Release &&  cmake -DCMAKE_BUILD_TYPE=Release ../.. \
	&& make ${DEMOS} && cd ../..

test:
	@echo __________________________________________________________________
	@echo
	@echo "Compiling tester in release mode and running it..."
	@echo __________________________________________________________________
	mkdir -p build/Release 2> /dev/null
	cd build/Release &&  cmake -DCMAKE_BUILD_TYPE=Release ../.. \
	&& make tester && cd ../.. && ../bin/tester

debug:
	@echo __________________________________________________________________
	@echo
	@echo "Compiling ${prj} project(s) in debug mode..."
	@echo "Use 'make debug prj=idxgui' for example to compile a \
	specific project."
	@echo __________________________________________________________________
	mkdir -p build/Debug 2> /dev/null
	cd build/Debug &&  cmake -DCMAKE_BUILD_TYPE=Debug ../.. \
	&& make ${prj} && cd ../..

.PHONY: doc	

doc:
	cd doc/documentation && ./generate.sh

clean:
	rm -rf build
	rm -rf ../core/build
	rm -rf bin/*
	rm -rf lib/*
	rm -rf ../doc/documentation/libeblearn_html
	rm -rf ../doc/documentation/libeblearngui_html
	rm -rf ../doc/documentation/libeblearntools_html
	rm -rf ../doc/documentation/libidx_html
	rm -rf ../doc/documentation/libidxgui_html

install: prepare_install $(INCLUDES) $(LIBS) $(BINS)

prepare_install:
	@echo "Installation directory: $(INSTALL_DIR)"
	mkdir -p $(INCLUDE_INSTALL) $(LIB_INSTALL) $(BIN_INSTALL)

$(LIBS):
	@if test -e ../bin/$@; \
	then echo "cp -f ../bin/$@ $(LIB_INSTALL)" \
	     && cp -f ../bin/$@ $(LIB_INSTALL); \
	else echo "warning: ../bin/$@ not found, not installed"; \
	fi

$(INCLUDES):
	@echo "cp -f $@ $(INCLUDE_INSTALL)" && cp -f "$@" $(INCLUDE_INSTALL)

$(BINS):
	@if test -e ../bin/$@; \
	then echo "cp -f ../bin/$@ $(BIN_INSTALL)" \
	     && cp -f ../bin/$@ $(BIN_INSTALL); \
	else echo "warning: ../bin/$@ not found, not installed"; \
	fi

uninstall:
	rm -Rf $(INCLUDE_INSTALL)
	cd $(LIB_INSTALL) && rm -Rf ${LIBS}
	cd $(BIN_INSTALL) && rm -Rf ${BINS}

