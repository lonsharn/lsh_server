noinst_PROGRAMS=TestClientSession TestServerSession TestExclusiveSession TestTimeoutSession 	\
				TestAckSession TestPerformanceClient TestPerformanceServer TestDataModel TestRedisClient

#-------------------------------------------------------------------------------------------
TestClientSession_CPPFLAGS = -std=c++11	\
							 -I$(top_srcdir)/Tools/include	\
							 -I$(top_srcdir)/HalloSql
TestClientSession_LDFLAGS = -lpthread -ldl -ljsoncpp -ljemalloc	\
							 -L$(top_srcdir)/Tools/lib
TestClientSession_LDADD = $(top_srcdir)/HalloMQ/libhallomq.la	\
						  $(top_srcdir)/Tools/lib/liblog4cpp.la	\
						  $(top_srcdir)/Tools/lib/libuuid.la
TestClientSession_SOURCES = TestClientSession.cpp

#-------------------------------------------------------------------------------------------
TestServerSession_CPPFLAGS = -std=c++11	\
							 -I$(top_srcdir)/Tools/include	\
							 -I$(top_srcdir)/HalloSql
TestServerSession_LDFLAGS = -lpthread -ldl -ljsoncpp -ljemalloc	-llog4cpp -luuid\
							 -L$(top_srcdir)/Tools/lib
TestServerSession_LDADD =  $(top_srcdir)/HalloMQ/libhallomq.la
TestServerSession_SOURCES = TestServerSession.cpp

#-------------------------------------------------------------------------------------------
TestExclusiveSession_CPPFLAGS = -std=c++11	\
								-I$(top_srcdir)/Tools/include	\
								-I$(top_srcdir)/HalloSql
TestExclusiveSession_LDFLAGS = -lpthread -ldl -ljsoncpp -ljemalloc	\
							 -L$(top_srcdir)/Tools/lib
TestExclusiveSession_LDADD = $(top_srcdir)/HalloMQ/libhallomq.la	\
							 $(top_srcdir)/Tools/lib/liblog4cpp.la	\
							 $(top_srcdir)/Tools/lib/libuuid.la
TestExclusiveSession_SOURCES = TestExclusiveSession.cpp

#-------------------------------------------------------------------------------------------
TestTimeoutSession_CPPFLAGS = -std=c++11	\
							  -I$(top_srcdir)/Tools/include	\
							  -I$(top_srcdir)/HalloSql
TestTimeoutSession_LDFLAGS = -lpthread -ldl -ljsoncpp -ljemalloc\
							 -L$(top_srcdir)/Tools/lib
TestTimeoutSession_LDADD = $(top_srcdir)/HalloMQ/libhallomq.la	\
						   $(top_srcdir)/Tools/lib/liblog4cpp.la	\
						   $(top_srcdir)/Tools/lib/libuuid.la
TestTimeoutSession_SOURCES = TestTimeoutSession.cpp

#-------------------------------------------------------------------------------------------
TestPerformanceClient_CPPFLAGS = -std=c++11	\
								 -I$(top_srcdir)/Tools/include	\
								 -I$(top_srcdir)/HalloSql
TestPerformanceClient_LDFLAGS = -lpthread -ldl -ljsoncpp -ljemalloc	\
								-L$(top_srcdir)/Tools/lib
TestPerformanceClient_LDADD = $(top_srcdir)/HalloMQ/libhallomq.la	\
							  $(top_srcdir)/Tools/lib/liblog4cpp.la	\
							  $(top_srcdir)/Tools/lib/libuuid.la
TestPerformanceClient_SOURCES = TestPerformanceClient.cpp

#-------------------------------------------------------------------------------------------
TestPerformanceServer_CPPFLAGS = -std=c++11	\
								 -I$(top_srcdir)/Tools/include	\
								 -I$(top_srcdir)/HalloSql
TestPerformanceServer_LDFLAGS = -lpthread -ldl -ljsoncpp -ljemalloc	\
								-L$(top_srcdir)/Tools/lib
TestPerformanceServer_LDADD = $(top_srcdir)/HalloMQ/libhallomq.la	\
							  $(top_srcdir)/Tools/lib/liblog4cpp.la	\
							  $(top_srcdir)/Tools/lib/libuuid.la
TestPerformanceServer_SOURCES = TestPerformanceServer.cpp
#-------------------------------------------------------------------------------------------
TestAckSession_CPPFLAGS = -std=c++11	\
						  -I$(top_srcdir)/Tools/include	\
						  -I$(top_srcdir)/HalloSql
TestAckSession_LDFLAGS = -lpthread -ldl -ljsoncpp -ljemalloc	\
						 -L$(top_srcdir)/Tools/lib
TestAckSession_LDADD = $(top_srcdir)/HalloMQ/libhallomq.la	\
					   $(top_srcdir)/Tools/lib/liblog4cpp.la	\
					   $(top_srcdir)/Tools/lib/libuuid.la
TestAckSession_SOURCES = TestAckSession.cpp
#-------------------------------------------------------------------------------------------
TestDataModel_CPPFLAGS = -std=c++11	\
						 -I$(top_srcdir)/Tools/include	\
						 -I$(top_srcdir)/DataModel
TestDataModel_LDFLAGS = -lpthread -ldl -ljemalloc -luuid -ljsoncpp -llog4cpp\
						-L$(top_srcdir)/Tools/lib
TestDataModel_LDADD = $(top_srcdir)/DataModel/libdatamodel.la
TestDataModel_SOURCES = TestDataModel.cpp
#-------------------------------------------------------------------------------------------
TestRedisClient_CPPFLAGS = -std=c++11	\
						 -I$(top_srcdir)/Tools/include	\
						 -I$(top_srcdir)/RedisClient
TestRedisClient_LDFLAGS = -llog4cpp \
						  -L$(top_srcdir)/Tools/lib
TestRedisClient_LDADD = $(top_srcdir)/RedisClient/libredisclient.la
TestRedisClient_SOURCES = TestRedisClient.cpp
