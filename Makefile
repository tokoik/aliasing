CFLAGS	= -I/usr/X11R6/include -DX11 -Wall
LDLIBS	= -L/usr/X11R6/lib -lglut -lGLU -lGL -lm
OBJECTS	= $(subst .c,.o,$(wildcard *.c))
TARGET	= aliasing

$(TARGET): $(OBJECTS)
	$(LINK.c) $^ $(LOADLIBES) $(LDLIBS) -o $@
clean:
	-rm -f $(TARGET) *.o *~

main.o: menu.h aliasing.h
menu.o: menu.h
