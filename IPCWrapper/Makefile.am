noinst_LTLIBRARIES = libipcwrapper.la
libipcwrapper_la_CXXFLAGS = -std=c++11 -I$(top_srcdir)/Tools/include
libipcwrapper_la_LIBADD = -ljemalloc	\
						 -L$(top_srcdir)/Tools/lib
libipcwrapper_la_SOURCES = ipc_shm.cpp
