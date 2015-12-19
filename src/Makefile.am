if PLATFORM_WIN32
libmodulesext = -0.dll
libmodulesdir = $(bindir)
else
libmodulesext = .so
libmodulesdir = $(libdir)
endif

LIBS = $(ZAKGTKDECODER_LIBS)

AM_CPPFLAGS = $(ZAKGTKDECODER_CFLAGS) \
              -DLOCALEDIR=\"$(localedir)\" \
              -DG_LOG_DOMAIN=\"ZakGtkDecoder\"

lib_LTLIBRARIES = libzakgtkdecoder.la

libzakgtkdecoder_la_SOURCES =

libzakgtkdecoder_la_LDFLAGS = -no-undefined

libzakgtkdecoder_include_HEADERS = \
                             libzakgtkdecoder.h

libzakgtkdecoder_includedir = $(includedir)/libzakgtkdecoder
