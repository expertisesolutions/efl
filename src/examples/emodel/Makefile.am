MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = \
-I$(top_builddir)/src/lib/efl \
-I$(top_srcdir)/src/lib/eina \
-I$(top_srcdir)/src/lib/eio \
-I$(top_srcdir)/src/lib/eo \
-I$(top_srcdir)/src/lib/emodel \
-I$(top_builddir)/src/lib/eio \
-I$(top_builddir)/src/lib/eo \
-I$(top_builddir)/src/lib/emodel \
@EMOTION_CFLAGS@

LDADD = \
$(top_builddir)/src/lib/eina/libeina.la \
$(top_builddir)/src/lib/eo/libeo.la \
$(top_builddir)/src/lib/eo/libeio.la \
$(top_builddir)/src/lib/emotion/libemodel.la \
@EMOTION_LDFLAGS@

EXTRA_PROGRAMS = \
emodel_file_example \

SRCS = \
emodel_file_example.c \

DATA_FILES = Makefile.examples

EXTRA_DIST = $(DATA_FILES)

examples: $(EXTRA_PROGRAMS)

clean-local:
	rm -f $(EXTRA_PROGRAMS)

install-examples:
	mkdir -p $(datadir)/emodel/examples
	$(install_sh_DATA) -c $(SRCS) $(DATA_FILES) $(datadir)/emodel/examples

uninstall-local:
	for f in $(SRCS) $(DATA_FILES); do \
	  rm -f $(datadir)/emodel/examples/$$f ; \
	done

if ALWAYS_BUILD_EXAMPLES
noinst_PROGRAMS = $(EXTRA_PROGRAMS)
endif
