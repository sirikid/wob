version = 0.12
pkgs = libseccomp wlroots

CFLAGS = -std=c99 -I./include $(shell pkg-config --cflags $(pkgs))
CPPFLAGS = -DWOB_VERSION='"$(version)"'
LDFLAGS = $(shell pkg-config --libs-only-L $(pkgs))
LDLIBS = $(shell pkg-config --libs-only-l $(pkgs))

.PHONY: all clean

all: wob

clean:
	$(RM) wob *.o include/*-protocol.h

wob: buffer.o color.o log.o main.o parse.o

main.o: include/wlr-layer-shell-unstable-v1-client-protocol.h include/xdg-output-unstable-v1-client-protocol.h

wl-proto-dir = $(shell pkg-config --variable=pkgdatadir wayland-protocols)

%-protocol.c: $(wl-proto-dir)/*/*/%.xml
	wayland-scanner private-code $< $@

include/%-client-protocol.h: ./protocols/%.xml
	wayland-scanner client-header $< $@

include/%-client-protocol.h: $(wl-proto-dir)/*/*/%.xml
	wayland-scanner client-header $< $@

include/%-protocol.h: $(wl-proto-dir)/*/*/%.xml
	wayland-scanner server-header $< $@
