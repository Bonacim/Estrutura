CXX=g++
RM=rm -f
CPPFLAGS=-g $(shell root-config --cflags)
LDFLAGS=-g $(shell root-config --ldflags)
LDLIBS=#$(shell root-config --libs)

SRCS=estrutura.cpp estrutura_imp.cpp estrutura_sdl.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: estrutura

estrutura: $(OBJS)
	$(CXX) $(LDFLAGS) -o estrutura $(OBJS) $(LDLIBS) 

depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) *~ .depend

include .depend
