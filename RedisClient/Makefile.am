noinst_LTLIBRARIES = libredisclient.la
libredisclient_la_CXXFLAGS = -std=c++11 -I$(top_srcdir)/Tools/include
libredisclient_la_LDFLAGS = -shared
libredisclient_la_LIBADD=  -levent -ljemalloc -lhiredis	\
							-L$(top_srcdir)/Tools/lib
libredisclient_la_SOURCES = redis_client.cpp	\
							redis_command_callback.cpp	\
							redis_command_formatter.cpp
