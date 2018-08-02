SRCDIR=.
INCDIR=${LIBTCODDIR}/include
CFLAGS=$(FLAGS) -g -I$(INCDIR) -I$(SRCDIR) -Wall
CC=gcc
CPP=g++
.SUFFIXES: .o .h .c .hpp .cpp

$(TEMP)/%.o : $(SRCDIR)/%.cpp
	$(CPP) $(CFLAGS) -o $@ -c $< 
$(TEMP)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -o $@ -c $< 

CPP_OBJS=$(TEMP)/main.o $(TEMP)/actor.o $(TEMP)/map.o $(TEMP)/engine.o $(TEMP)/attacker.o $(TEMP)/ai.o $(TEMP)/destructible.o $(TEMP)/gui.o $(TEMP)/container.o $(TEMP)/pickable.o $(TEMP)/effects.o $(TEMP)/color.o $(TEMP)/camera.o $(TEMP)/savesys.o

all : main

main : $(CPP_OBJS)
	$(CPP) $(CPP_OBJS) -o $@ -L${LIBTCODDIR} -lz -ltcod -ltcodxx -Wl,-rpath=.

clean :
	\rm -f $(CPP_OBJS) main