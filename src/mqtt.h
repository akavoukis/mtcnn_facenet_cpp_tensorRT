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


/**
 * A callback class for use with the main MQTT client.
 */
class callback : public virtual mqtt::callback
{
    public:
        void connection_lost(const string& cause) ;
        void delivery_complete(mqtt::delivery_token_ptr tok);
};

/**
 * A base action listener.
 */
class action_listener : public virtual mqtt::iaction_listener
{
protected:
	void on_failure(const mqtt::token& tok);
	void on_success(const mqtt::token& tok);
};

/**
 * A derived action listener for publish events.
 */
class delivery_action_listener : public action_listener
{
	atomic<bool> done_;
	void on_failure(const mqtt::token& tok);
	void on_success(const mqtt::token& tok);
public:
	delivery_action_listener() : done_(false) {}
	bool is_done() const { return done_; }
};



class mqtt_class {
    public:
        mqtt_class();
        int mqtt_init();
        int mqtt_send_data(vector<string> detections);
    private:
        const int QOS = 1;
        const std::chrono::duration<unsigned int> TIMEOUT = std::chrono::seconds(10);
        //mqtt::async_client client;
};


#endif // __MQTT_H__