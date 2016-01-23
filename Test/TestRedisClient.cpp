/**
 * @file TestDataModel.cpp
 * @brief DataModel test file
 * @author lonsharn
 * @version 1.0
 * @date 2015-11-04
 */

#include<unistd.h>
#include<string.h>
#include<iostream>
#include<map>
#include<list>

#include <uuid/uuid.h>

#include <event.h>
#include "Utility/log4cpp_wrapper.h"
#include "redis_client.h"

using namespace std;

static void
signal_cb(evutil_socket_t fd, short event, void *arg)
{
	struct event *signal = (struct event*)arg;

	printf("%s: got signal %d\n", __func__, EVENT_SIGNAL(signal));
    exit(-1);
}

int main(int argc, char** argv)
{
	try
	{
		log4cpp::PropertyConfigurator::configure("./log4cpp.conf");
	}
	catch (log4cpp::ConfigureFailure& f)
	{
		std::cout << "Configure Problem " << f.what() << std::endl;
		return -1;
	}

    RedisClient* client = RedisClient::Connect("127.0.0.1", 6379);
    if(!client)
    {
        return -1;
    }

    event_base* eb = event_base_new();

    struct event signal_int;
	event_assign(&signal_int, eb, SIGINT, EV_SIGNAL|EV_PERSIST, signal_cb,
	    &signal_int);

    client->StartAsync(eb);
    client->Command("hset userinfo uid 11111",[](any& value, bool ret)
    {
        if(!ret)
        {
        cout<<any_cast<string>(value);
            return;
        }
        list<any>& lst = any_cast<list<any>&>(value);
        for(auto it: lst)
        {
            cout<<any_cast<string>(it);
        }
        return;
    });

	event_add(&signal_int, NULL);

    event_base_dispatch(eb);
    event_base_free(eb);
    delete client;

	return 0;
}
