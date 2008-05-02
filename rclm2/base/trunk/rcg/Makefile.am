## Process this file with automake to produce Makefile.in

bin_PROGRAMS = rclmresultprinter

rclmresultprinter_SOURCES = \
	resultprinter.cpp

rclmresultprinter_CPPFLAGS =
rclmresultprinter_CXXFLAGS = -Wall
rclmresultprinter_LDFLAGS =
rclmresultprinter_LDADD = librcg.a


noinst_LIBRARIES = librcg.a

librcg_a_SOURCES = \
	gzfstream.cpp \
	factory.cpp \
	parser_v1.cpp \
	parser_v2.cpp \
	parser_v3.cpp \
	parser_v4.cpp \
	util.cpp

noinst_HEADERS = \
	gzfstream.h \
	factory.h \
	handler.h \
	parser.h \
	parser_v1.h \
	parser_v2.h \
	parser_v3.h \
	parser_v4.h \
	types.h \
	util.h


AM_CPPFLAGS = -I$(top_srcdir)
AM_CFLAGS = -Wall
AM_CXXFLAGS = -Wall
AM_LDFLAGS =

CLEANFILES = *~

#EXTRA_DIST =
