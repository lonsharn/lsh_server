noinst_LTLIBRARIES = libsqlstmt.la
libsqlstmt_la_CXXFLAGS = -std=c++11 -Wno-deprecated-declarations	\
						 -I$(top_srcdir)/Tools/include
libsqlstmt_la_LDFLAGS= 	-ljemalloc -lsqlite3	\
						-L$(top_srcdir)/Tools/lib
libsqlstmt_la_SOURCES = sql_connect.cpp	\
						sql_stmt.cpp	\
						sqlite_connect.cpp	\
						sqlite_stmt.cpp
