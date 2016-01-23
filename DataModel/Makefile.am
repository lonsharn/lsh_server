noinst_LTLIBRARIES = libdatamodel.la
libdatamodel_la_CXXFLAGS = -std=c++11 -I$(top_srcdir)/Tools/include
libdatamodel_la_LIBADD = -ljsoncpp -ljemalloc	\
						 $(top_srcdir)/HalloSql/libhallosql_all.la	\
						 -L$(top_srcdir)/Tools/lib
libdatamodel_la_SOURCES = attribute_node.cpp	\
						  attribute.cpp	\
						  json_serializer.cpp	\
						  sql_serializer.cpp	\
						  property_node.cpp
