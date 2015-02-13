SRCDIR=../../fife_wda/
VPATH=$(SRCDIR)
GEN=wda_version.h
#LIB=libwda.a
SHLIB=libwda
HDR=ifbeam_c.h wda.h
OBJ=ifbeam.o wda.o
SRC=ifbeam.c wda.c
TST=test_ifbeam
###CPPFLAGS=-I/usr/local/opt/openssl/include
#
MAJOR=2
MINOR=21
RELEASE=0
VERSION=$(MAJOR).$(MINOR)

platform=$(shell uname -s)

ifeq ($(platform),Darwin)
CXXFLAGS=-fPIC -g -O3 $(DEFS) $(ARCH) -I$(SRCDIR) -I. -I/usr/local/opt/openssl/include
LDFLAGS=-L/usr/local/opt/openssl/lib
else
CXXFLAGS=-fPIC -g -O3 $(DEFS) $(ARCH) -I$(SRCDIR) -I.
LDFLAGS=
endif

all: $(GEN) $(BIN) $(LIB) $(SHLIB) $(TST)

install:
	test -d ../lib || mkdir ../lib
	test -d ../include || mkdir -p ../include
	cp -P $(SHLIB)* ../lib
	cp $(HDR) ../include

clean:
	rm -f *.o *.a $(SHLIB)*

#$(LIB): $(OBJ) $(UTLOBJ)
#	rm -f $(LIB)
#	ar qv $(LIB) $(OBJ)

ifeq ($(platform),Darwin)
$(SHLIB): $(OBJ) $(UTLOBJ)
	rm -f $(SHLIB).*
	gcc -dynamiclib \
	    -install_name $(SHLIB).$(MAJOR).dylib \
	    -compatibility_version $(MAJOR).0 \
	    -current_version $(VERSION).$(RELEASE) \
	    -undefined dynamic_lookup $(OBJ) -o $(SHLIB).$(VERSION).$(RELEASE).dylib
	ln -sf $(SHLIB).$(VERSION).$(RELEASE).dylib $(SHLIB).$(MAJOR).$(MINOR).dylib
	ln -sf $(SHLIB).$(VERSION).$(RELEASE).dylib $(SHLIB).$(MAJOR).dylib
	ln -sf $(SHLIB).$(VERSION).$(RELEASE).dylib $(SHLIB).dylib
	echo $(VERSION) > VERSION
else
$(SHLIB): $(OBJ) $(UTLOBJ)
	rm -f $(SHLIB)*
	#$(CC) -shared -o $(SHLIB) $(ARCH) $(OBJ) -lcurl -lc
	#$(CC) -shared -Wl,-soname,$(SHLIB).$(MAJOR) -o $(SHLIB).$(VERSION).$(RELEASE) $(OBJ) -lcurl -lc
	gcc -shared -Wl,-soname,$(SHLIB).so.$(MAJOR) -o $(SHLIB).so.$(VERSION).$(RELEASE) $(OBJ) -lcurl -lc $(LDFLAGS)
	ln -sf $(SHLIB).so.$(VERSION).$(RELEASE) $(SHLIB).so.$(MAJOR)
	ln -sf $(SHLIB).so.$(MAJOR) $(SHLIB).so
	echo $(VERSION) > VERSION
endif

wda_version.h: FORCE
	test -d .git && echo '#define WDA_VERSION "'`git describe --tags --match 'wda*'`'"' > $@.new && mv $@.new $@ || true

FORCE:

%.o: %.c
	gcc -c -o $@ $(CXXFLAGS) $<

test_ifbeam: test_ifbeam.o $(OBJ)
	gcc -o ifbeam_test test_ifbeam.o $(CXXFLAGS) $(OBJ) -lcurl $(LDFLAGS) -lssl -lcrypto

