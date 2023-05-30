TARGET_EXEC := myApp
SRCS := busctl.c g_lib_to_work.c bus_status.c bus_list.c bus_emit.c bus_introspect.c bus_property.c bus_call.c
OBJS := busctl.o g_lib_to_work.o bus_status.o bus_list.o bus_emit.o bus_introspect.o bus_property.o bus_call.o
CFLAGS := $(shell pkg-config --cflags gio-2.0 glib-2.0)
LIBS :=  $(shell pkg-config --libs gio-2.0 glib-2.0)

CC := gcc

$(TARGET_EXEC): $(OBJS)
	$(CC) -g -o $@ $(OBJS) $(LIBS)

%.o : %.c
	$(CC) $(INCLUDE) $(CFLAGS) -g -w -c $< -o $@ 

clean:
	rm -r $(TARGET_EXEC) $(OBJS)
