SRCDIR=../../fife_wda/
VPATH=$(SRCDIR)
GEN=wda_version.h
LIB=libwda.a 
SHLIB=                  #libwda.so
HDR=ifbeam_c.h wda.h
OBJ=ifbeam.o libwda.o   #wda.o
SRC= ifbeam.c wda.c
TST=test_ifbeam
CXXFLAGS=-fPIC -g -O3 $(DEFS) $(ARCH) -I$(SRCDIR) -I.


all: $(GEN) $(BIN) $(LIB) $(SHLIB) $(TST)

install:
	test -d ../lib || mkdir ../lib
	test -d ../include || mkdir -p ../include
	cp $(LIB) $(SHLIB) ../lib
	cp $(HDR) ../include

clean:
	rm -f *.o *.a 

$(LIB): $(OBJ) $(UTLOBJ)
	rm -f $(LIB)
	ar qv $(LIB) $(OBJ) 

#$(SHLIB): $(OBJ) $(UTLOBJ)
#	rm -f $(SHLIB)
#	$(CC) -shared -o $(SHLIB) $(ARCH) $(OBJ) -lcurl -lc

wda_version.h: FORCE
	echo '#define WDA_VERSION "'`git describe --tags --match 'wda*'`'"' > $@.new && mv $@.new $@ || true

FORCE:

%.o: %.c
	gcc -c -o $@ $(CXXFLAGS) $<

libwda.o: wda.o
	ld wda.o -L /usr/lib64/ -lcurl -llber -lldap --relocatable -o libwda.o

test_ifbeam: test_ifbeam.o $(OBJ)
	#gcc -o ifbeam_test test_ifbeam.o $(CXXFLAGS)  $(OBJ)  -lcurl -lssl
	gcc -o ifbeam_test test_ifbeam.o $(CXXFLAGS)  $(LIB) -lidn -lssl

