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

decodermarshal.c: decodermarshal.h decoder_marshal.list $(GLIB_GENMARSHAL)
	$(GLIB_GENMARSHAL) $(srcdir)/decoder_marshal.list --body --prefix=_zak_gtk_decoder_marshal > $(srcdir)/$@

decodermarshal.h: decoder_marshal.list $(GLIB_GENMARSHAL)
	$(GLIB_GENMARSHAL) $(srcdir)/decoder_marshal.list --header --prefix=_zak_gtk_decoder_marshal > $(srcdir)/$@

libzakgtkdecoder_la_SOURCES = \
                              decodermarshal.c \
                              zakgtkdecoder.c

libzakgtkdecoder_la_LDFLAGS = -no-undefined

libzakgtkdecoder_include_HEADERS = \
                             libzakgtkdecoder.h \
                             zakgtkdecoder.h

libzakgtkdecoder_includedir = $(includedir)/libzakgtkdecoder

noinst_HEADERS = \
                 decodermarshal.h
