SUBDIRS = icons

glademodulesdir = $(GLADE_MODULE_DIR)

gladecatalogsdir = $(GLADE_CATALOG_DIR)

gladecatalogs_DATA = zakgtkdecoder.xml

EXTRA_DIST = $(gladecatalogs_DATA)

if PLATFORM_WIN32
libmodulesext = -0.dll
libmodulesextglade = .dll
libmodulesdir = $(bindir)
else
libmodulesext = .so
libmodulesextglade = $(libmodulesext)
libmodulesdir = $(libdir)
endif

install-exec-hook:
	cd $(glademodulesdir) && \
	ln -s -f $(libmodulesdir)/libzakgtkdecoder$(libmodulesext) libzakgtkdecoder$(libmodulesextglade)

uninstall-hook:
	rm -f $(glademodulesdir)/libzakgtkdecoder$(libmodulesextglade)
