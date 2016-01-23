noinst_LTLIBRARIES = libhallosql_my.la libhallosql_pg.la libhallosql_all.la
libhallosql_my_la_CXXFLAGS = -std=c++11 -w -g -D_DEBUG_ -D_S_NOLIBGXX -D_KC_DEBUG -DKC_MY_SQL	\
							 -I$(top_srcdir)/Tools/include/
libhallosql_my_la_LDFLAGS=	-lmysqlclient -ljemalloc	\
							-L$(top_srcdir)/Tools/lib
libhallosql_my_la_SOURCES = kc_mysql.cpp	\
							kc_sqlbase.cpp	\
							kc_string.cpp	\
							kcsql.cpp

libhallosql_pg_la_CXXFLAGS = -std=c++11 -w -g -D_DEBUG_ -D_S_NOLIBGXX -D_KC_DEBUG -DKC_PG_SQL \
							 -I$(top_srcdir)/Tools/include
libhallosql_pg_la_LDFLAGS =	-lpq -ljemalloc	\
							-L$(top_srcdir)/Tools/lib
libhallosql_pg_la_SOURCES = kc_pgsql.cpp	\
							kc_sqlbase.cpp	\
							kc_string.cpp	\
							kcsql.cpp

libhallosql_all_la_CXXFLAGS = -std=c++11 -w -g -D_DEBUG_ -D_S_NOLIBGXX -D_KC_DEBUG -DKC_PG_SQL \
							  -I$(top_srcdir)/Tools/include/	\
							  -I$(top_srcdir)/Tools/include/
libhallosql_all_la_LDFLAGS=	-lpq -lmysqlclient -ljemalloc	\
							-L$(top_srcdir)/Tools/lib
libhallosql_all_la_SOURCES = kc_pgsql.cpp	\
							 kc_mysql.cpp	\
							 kc_sqlbase.cpp	\
							 kc_string.cpp	\
							 kcsql.cpp
