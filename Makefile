BDIR =bin
SDIR =src
IDIR =include

CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj

LIBS=-lboost_system -lboost_thread -lboost_program_options

_DEPS = server.hpp network_v4.hpp statistic.hpp
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o server.o network_v4.o statistic.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	g++ -c -o $@ $< $(CFLAGS)

funnel: $(OBJ)
	g++ -o $(BDIR)/$@ $^ $(CFLAGS) $(LIBS)

#.PHONY: clean

#clean:
#	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
