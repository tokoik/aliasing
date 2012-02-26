CFLAGS	= -I/usr/X11R6/include -DX11 -Wall
LDLIBS	= -L/usr/X11R6/lib -lglut -lGLU -lGL -lm
OBJECTS	= $(subst .c,.o,$(wildcard *.c))
TARGET	= aliasing

$(TARGET): $(OBJECTS)
	$(LINK.c) $^ $(LOADLIBES) $(LDLIBS) -o $@

clean:
	-rm -f $(TARGET) *.o *~

depend:
	makedepend -- $(CFLAGS) -- $(wildcard *.c)

# DO NOT DELETE

main.o: /usr/include/math.h /usr/include/architecture/i386/math.h
main.o: /usr/include/sys/cdefs.h /usr/include/sys/_symbol_aliasing.h
main.o: /usr/include/sys/_posix_availability.h aliasing.h
main.o: /usr/include/stdlib.h /usr/include/Availability.h
main.o: /usr/include/AvailabilityInternal.h /usr/include/_types.h
main.o: /usr/include/sys/_types.h /usr/include/machine/_types.h
main.o: /usr/include/i386/_types.h /usr/include/sys/wait.h
main.o: /usr/include/sys/signal.h /usr/include/sys/appleapiopts.h
main.o: /usr/include/machine/signal.h /usr/include/i386/signal.h
main.o: /usr/include/i386/_structs.h /usr/include/sys/_structs.h
main.o: /usr/include/machine/_structs.h /usr/include/sys/resource.h
main.o: /usr/include/machine/endian.h /usr/include/i386/endian.h
main.o: /usr/include/sys/_endian.h /usr/include/libkern/_OSByteOrder.h
main.o: /usr/include/libkern/i386/_OSByteOrder.h /usr/include/alloca.h
main.o: /usr/include/machine/types.h /usr/include/i386/types.h
main.o: /usr/X11R6/include/GL/glut.h /usr/X11R6/include/GL/gl.h
main.o: /usr/X11R6/include/GL/glext.h /usr/include/stddef.h
main.o: /usr/include/inttypes.h /usr/include/stdint.h
main.o: /usr/X11R6/include/GL/glu.h menu.h
menu.o: aliasing.h /usr/include/stdlib.h /usr/include/Availability.h
menu.o: /usr/include/AvailabilityInternal.h /usr/include/_types.h
menu.o: /usr/include/sys/_types.h /usr/include/sys/cdefs.h
menu.o: /usr/include/sys/_symbol_aliasing.h
menu.o: /usr/include/sys/_posix_availability.h /usr/include/machine/_types.h
menu.o: /usr/include/i386/_types.h /usr/include/sys/wait.h
menu.o: /usr/include/sys/signal.h /usr/include/sys/appleapiopts.h
menu.o: /usr/include/machine/signal.h /usr/include/i386/signal.h
menu.o: /usr/include/i386/_structs.h /usr/include/sys/_structs.h
menu.o: /usr/include/machine/_structs.h /usr/include/sys/resource.h
menu.o: /usr/include/machine/endian.h /usr/include/i386/endian.h
menu.o: /usr/include/sys/_endian.h /usr/include/libkern/_OSByteOrder.h
menu.o: /usr/include/libkern/i386/_OSByteOrder.h /usr/include/alloca.h
menu.o: /usr/include/machine/types.h /usr/include/i386/types.h
menu.o: /usr/X11R6/include/GL/glut.h /usr/X11R6/include/GL/gl.h
menu.o: /usr/X11R6/include/GL/glext.h /usr/include/stddef.h
menu.o: /usr/include/inttypes.h /usr/include/stdint.h
menu.o: /usr/X11R6/include/GL/glu.h menu.h
