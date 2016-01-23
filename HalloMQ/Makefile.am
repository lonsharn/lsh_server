noinst_LTLIBRARIES = libhallomq.la
libhallomq_la_CXXFLAGS = -std=c++11 -Wno-deprecated-declarations -DFRAMEWORK	\
						 -I$(top_srcdir)/Tools/include/activemq-cpp-3.8.3/	\
						 -I$(top_srcdir)/Tools/include
libhallomq_la_LDFLAGS= -lpthread -ldl -lactivemq-cpp -luuid	-ljemalloc	\
					   -L$(top_srcdir)/Tools/lib
libhallomq_la_SOURCES = hallo_mq.cpp	\
						hmq_connection.cpp	\
						hmq_message.cpp	\
						hmq_message_listener.cpp	\
						hmq_exception_listener.cpp	\
						hmq_session.cpp
