noinst_PROGRAMS=LoginService
LoginService_CPPFLAGS = -std=c++11	\
						-I$(top_srcdir)/Tools/include	\
						-I$(top_srcdir)/HalloSql
LoginService_LDFLAGS = -lpthread -ldl -ljemalloc	\
					   -L$(top_srcdir)/Tools/lib
LoginService_LDADD = $(top_srcdir)/HalloSql/libhallosql_all.la	\
					 $(top_srcdir)/DataModel/libdatamodel.la	\
					 $(top_srcdir)/HalloMQ/libhallomq.la	\
					 $(top_srcdir)/Tools/lib/liblog4cpp.la
					 $(top_srcdir)/Tools/lib/libuuid.la
LoginService_SOURCES = main.cpp	\
					   login_user.cpp	\
					   login_service.cpp

