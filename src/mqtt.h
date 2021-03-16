#ifndef __MQTT_H__
#define __MQTT_H__

#include <iostream>
#include <cstdlib>
#include <string>
#include <thread>
#include <atomic>
#include <chrono>
#include <cstring>
#include "mqtt/async_client.h"

using namespace std;

class callback  : public virtual mqtt::callback
{
	void connection_lost(const string& cause) override;
	void delivery_complete(mqtt::delivery_token_ptr tok) override;
};

class mqtt_class {
    public:
        mqtt_class(const string address, const string clientID);
        int mqtt_init();
        int mqtt_send_data(vector<string> detections);
    private:
        const int QOS = 1;
        string connection_address;
        string mqtt_client_id;
        const string persist_dir {"./persist"};
        const std::chrono::duration<unsigned int> TIMEOUT = std::chrono::seconds(10);
        mqtt::async_client *mqtt_client;
        callback cb;
};


#endif // __MQTT_H__