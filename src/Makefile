OBJS=		main.o cpu6502.o memory.o vic.o cia2.o peripheral.o hostio.o cia1.o info6502.o \
		iecbus.o cia.o iec1541.o disk.o
CPPFLAGS+=	-Wall -Werror -g
LDFLAGS+=	`sdl-config --libs`

c64emu:		$(OBJS)
		$(CXX) -o c64emu $(OBJS) $(LDFLAGS)

clean:
		rm -f c64emu $(OBJS)
