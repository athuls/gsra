OS := $(shell uname -s)
TORCHDIR := $(shell pwd)
include Makefile_options_$(OS)

SUBDIRS := core
SUBDIRS += $(PACKAGES)

all:
	@echo ">>> Try to compile Torch <<<"
	@for subdir in ${SUBDIRS} ; do ( cd $$subdir ; ${MAKE} $@) || exit 10 ; done
	@echo ">> !!! Ok !!! <<<"

clean:
	@echo ">>> Atomise all <<<"
	@for subdir in ${SUBDIRS} ; do ( cd $$subdir ; ${MAKE} $@ ) || exit 10 ; done
	@\rm -Rf $(LIBTORCHXX)
	@\rm -Rf $(OBJS_DIR)
	@\rm -Rf $(LIBS_DIR)
	@echo ">> !!! Ok !!! <<<"

distclean:
	@echo ">>> Thermonuclearise all <<<"
	@for subdir in ${SUBDIRS} ; do ( cd $$subdir ; ${MAKE} $@ ) || exit 10 ; done
	@\rm -Rf lib
	@\rm -Rf objs
	@echo ">> !!! Ok !!! <<<"

depend:
	@echo ">>> Dependencies <<<"
	@for subdir in ${SUBDIRS} ; do ( cd $$subdir ; ${MAKE} $@ ) || exit 10 ; done
	@\mkdir -p $(OBJS_DIR)
	@\mkdir -p $(LIBS_DIR)
	@echo ">> !!! Ok !!! <<<"

Makefile_options_$(OS):
	@echo "**** PLEASE CHECK YOUR MAKEFILE_OPTIONS for >>" $(OS) "<< ****"
