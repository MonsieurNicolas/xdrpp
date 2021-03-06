## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = $(cereal_CPPFLAGS) $(autocheck_CPPFLAGS)

bin_PROGRAMS = xdrc/xdrc

xdrc_xdrc_SOURCES = xdrc/xdrc.cc xdrc/gen_hh.cc xdrc/gen_server.cc	\
	xdrc/scan.ll xdrc/parse.yy xdrc/union.h xdrc/xdrc_internal.h
if NEED_GETOPT_LONG
xdrc_xdrc_SOURCES += compat/getopt_long.c compat/getopt.h
getopt.h: $(top_srcdir)/compat/getopt.h
	cp $(top_srcdir)/compat/getopt.h getopt.h
MAYBE_GETOPT = getopt.h
else # ! NEED_GETOPT_LONG
MAYBE_GETOPT =
endif # ! NEED_GETOPT_LONG
xdrc/xdrc.$(OBJEXT): xdrpp/config.h $(MAYBE_GETOPT)
xdrc_xdrc_LDADD =

AM_YFLAGS = -d
BUILT_SOURCES = xdrc/parse.cc xdrc/parse.hh xdrc/scan.cc	\
	xdrpp/rpc_msg.hh xdrpp/rpcb_prot.hh xdrpp/config.h

lib_LIBRARIES = xdrpp/libxdrpp.a
xdrpp_libxdrpp_a_SOURCES = xdrpp/iniparse.cc xdrpp/marshal.cc	\
	xdrpp/msgsock.cc xdrpp/printer.cc xdrpp/pollset.cc	\
	xdrpp/rpcbind.cc xdrpp/rpc_msg.cc xdrpp/server.cc	\
	xdrpp/socket.cc xdrpp/socket_unix.cc xdrpp/srpc.cc xdrpp/arpc.cc

nodist_pkginclude_HEADERS = xdrpp/build_endian.h

# If we use AC_CONFIG_HEADERS([xdrpp/config.h]) in configure.ac, then
# autoconf adds -Ixdrpp, which causes errors for files like endian.h
# (system headers find the xdrpp version instead of the system
# version).  Creating ./config.h and then copying it into ./xdrpp/
# solves the problem.
xdrpp/config.h: $(builddir)/config.h
	cp $(builddir)/config.h xdrpp/config.h

pkginclude_HEADERS = xdrpp/types.h xdrpp/cereal.h xdrpp/clear.h		\
	xdrpp/exception.h xdrpp/iniparse.h xdrpp/marshal.h		\
	xdrpp/printer.h xdrpp/rpc_msg.hh xdrpp/message.h		\
	xdrpp/msgsock.h xdrpp/arpc.h xdrpp/pollset.h xdrpp/server.h	\
	xdrpp/socket.h xdrpp/srpc.h xdrpp/rpcbind.h xdrpp/autocheck.h	\
	xdrpp/endian.h xdrpp/build_endian.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = xdrpp.pc

LDADD = $(top_builddir)/xdrpp/libxdrpp.a

check_PROGRAMS = tests/test-msgsock tests/test-marshal		\
	tests/test-srpc tests/test-printer tests/test-listener	\
	tests/test-arpc tests/test-compare tests/test-types
if USE_CEREAL
check_PROGRAMS += tests/test-cereal
endif
if USE_AUTOCHECK
check_PROGRAMS += tests/test-autocheck
endif
#TESTS = $(check_PROGRAMS)
tests_test_printer_SOURCES = tests/printer.cc
tests_test_msgsock_SOURCES = tests/msgsock.cc
tests_test_marshal_SOURCES = tests/marshal.cc
tests_test_cereal_SOURCES = tests/cereal.cc
tests_test_srpc_SOURCES = tests/srpc.cc
tests_test_listener_SOURCES = tests/listener.cc
tests_test_arpc_SOURCES = tests/arpc.cc
tests_test_autocheck_SOURCES = tests/autocheck.cc
tests_test_compare_SOURCES = tests/compare.cc
tests_test_types_SOURCES = tests/types.cc
tests/arpc.$(OBJEXT): tests/xdrtest.hh
tests/marshal.$(OBJEXT): tests/xdrtest.hh
tests/printer.$(OBJEXT): tests/xdrtest.hh
tests/cereal.$(OBJEXT): tests/xdrtest.hh
tests/srpc.$(OBJEXT): tests/xdrtest.hh
tests/listener.$(OBJEXT): tests/xdrtest.hh
tests/arpc.$(OBJEXT): tests/xdrtest.hh
tests/autocheck.$(OBJEXT): tests/xdrtest.hh
tests/compare.$(OBJEXT): tests/xdrtest.hh
tests/types.$(OBJEXT): tests/xdrtest.hh

SUFFIXES = .x .hh
.x.hh:
	$(XDRC) -hh -o $@ $<
$(top_builddir)/tests/xdrtest.hh: $(XDRC)
$(top_builddir)/xdrpp/rpc_msg.hh: $(XDRC)
$(top_builddir)/xdrpp/rpcb_prot.hh: $(XDRC)

CLEANFILES = *~ */*~ */*/*~ .gitignore~ tests/xdrtest.hh	\
	xdrpp/rpc_msg.hh xdrpp/rpcb_prot.hh
DISTCLEANFILES = xdrpp/config.h getopt.h

$(srcdir)/doc/xdrc.1: $(srcdir)/doc/xdrc.1.md
	-pandoc -s -w man $(srcdir)/doc/xdrc.1.md -o $(srcdir)/doc/xdrc.1

distclean-local:
	rm -rf autom4te.cache

maintainer-clean-local:
	+@echo rm -rf `sed -ne 's!^/!!p' .gitignore` Makefile.in
	rm -rf `sed -ne 's!^/!!p' .gitignore` Makefile.in

man_MANS = doc/xdrc.1
EXTRA_DIST = .gitignore autogen.sh doc/xdrc.1 doc/xdrc.1.md		\
	xdrpp/build_endian.h.in xdrpp/rpc_msg.x xdrpp/rpcb_prot.x	\
	tests/xdrtest.x doc/rfc1833.txt doc/rfc4506.txt			\
	doc/rfc5531.txt doc/rfc5665.txt
MAINTAINERCLEANFILES = xdrc/parse.hh xdrc/parse.cc xdrc/scan.cc	\
	$(srcdir)/doc/xdrc.1

#xdrpp.pc.in xdrpp-uninstalled.pc.in 

ACLOCAL_AMFLAGS = -I m4

# Workaround for old automake generating parser.h not parser.hh
.h.hh:
	cp $< $@
