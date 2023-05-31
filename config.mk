VERSION = 6.4
XRENDERER = -lXrender
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man
X11INC = /usr/X11R6/include
X11LIB = /usr/X11R6/lib
XINERAMALIBS  = -lXinerama
XINERAMAFLAGS = -DXINERAMA
FREETYPELIBS = -lfontconfig -lXft
FREETYPEINC = /usr/include/freetype2
INCS = -I${X11INC} -I${FREETYPEINC}
LIBS = -L${X11LIB} -lX11 ${XINERAMALIBS} ${FREETYPELIBS} -lX11-xcb -lxcb -lxcb-res ${XRENDERER}
CPPFLAGS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_XOPEN_SOURCE=700L -DVERSION=\"${VERSION}\" ${XINERAMAFLAGS}
CFLAGS   = -std=c99 -pedantic -Wall -Wno-deprecated-declarations -Os ${INCS} ${CPPFLAGS}
LDFLAGS  = ${LIBS}
CC = cc
