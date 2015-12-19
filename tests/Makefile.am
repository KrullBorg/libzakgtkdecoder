AM_CPPFLAGS = $(WARN_CFLAGS) \
              $(DISABLE_DEPRECATED_CFLAGS) \
              $(ZAKGTKDECODER_CFLAGS) \
              -I$(top_srcdir)/src \
              -DGUIDIR="\"@abs_builddir@\""

LIBS = $(ZAKGTKDECODER_LIBS) \
       -export-dynamic

LDADD = $(top_builddir)/src/libzakgtkdecoder.la

noinst_PROGRAMS =

EXTRA_DIST =
