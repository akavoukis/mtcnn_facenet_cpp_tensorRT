#include "mqtt.h"

/**
 * A callback class for use with the main MQTT client.
 */

void callback::connection_lost(const string& cause) {
	cout << "\nConnection lost" << endl;
	if (!cause.empty())
		cout << "\tcause: " << cause << endl;
}

void callback::delivery_complete(mqtt::delivery_token_ptr tok) {
	cout << "\tDelivery complete for token: "
		<< (tok ? tok->get_message_id() : -1) << endl;
}

int mqtt_class::mqtt_send_data(vector<string> detections)
{
    cout << "\nSending message..." << endl;
    const char* PAYLOAD2 = "Hi there!";
    const string TOPIC { "hello" };
	for (vector<string>::iterator t=detections.begin(); t!=detections.end(); ++t) 
    {
		mqtt::message_ptr pubmsg = mqtt::make_message("facenet/person", *t);
		pubmsg->set_qos(QOS);
		mqtt_client->publish(pubmsg)->wait_for(TIMEOUT);
		cout << *t <<"  ...OK" << endl;
    }
}

mqtt_class::mqtt_class(const string address, const string clientID )
{
    //const string DFLT_SERVER_ADDRESS	{ "tcp://localhost:1883" };
    //const string CLIENT_ID			{ "paho_cpp_async_publish" };
    connection_address = address;
    mqtt_client_id = clientID;
	// A client that just publishes normally doesn't need a persistent
	// session or Client ID unless it's using persistence, then the local
	// library requires an ID to identify the persistence files.

	//string	address  = DFLT_SERVER_ADDRESS,
	//		clientID = CLIENT_ID;
    mqtt_client = new mqtt::async_client(connection_address, mqtt_client_id, persist_dir);
}

int mqtt_class::mqtt_init()
{
    //const string PERSIST_DIR			{ "./persist" };
    const string TOPIC { "facenet/person" };

    const char* LWT_PAYLOAD = "offline";

    cout << "Initializing for server '" << connection_address << "'..." << endl;
	mqtt_client->set_callback(cb);	

	auto connOpts = mqtt::connect_options_builder()
		.clean_session()
		.will(mqtt::message(TOPIC, LWT_PAYLOAD))
		.finalize();
	cout << "  ...OK" << endl;

	try {
		cout << "\nConnecting..." << endl;
		mqtt::token_ptr conntok = mqtt_client->connect(connOpts);
		cout << "Waiting for the connection..." << endl;
		conntok->wait();
		cout << "  ...OK" << endl;

        // cout << "\nSending message1..." << endl;
        // const char* PAYLOAD2 = "Hi there1!";
        // const string TOPIC { "hello11" };
        // mqtt::message_ptr pubmsg = mqtt::make_message(TOPIC, PAYLOAD2);
        // pubmsg->set_qos(QOS);
        // mqtt_client->publish(pubmsg)->wait_for(TIMEOUT);
        // cout << "  ...OK" << endl;
	}
	catch (const mqtt::exception& exc) {
		cerr << exc.what() << endl;
		return 1;
	}

 	return 0;
}