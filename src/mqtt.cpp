#include "mqtt.h"

bool done_ = false;

void callback::connection_lost(const string& cause) {
    cout << "\nConnection lost" << endl;
    if (!cause.empty())
        cout << "\tcause: " << cause << endl;
}

void callback::delivery_complete(mqtt::delivery_token_ptr tok) {
    cout << "\tDelivery complete for token: "
        << (tok ? tok->get_message_id() : -1) << endl;
}

void action_listener::on_failure(const mqtt::token& tok) {
    cout << "\tListener failure for token: "
        << tok.get_message_id() << endl;
}

void action_listener::on_success(const mqtt::token& tok) {
    cout << "\tListener success for token: "
        << tok.get_message_id() << endl;
}

void on_failure(const mqtt::token& tok) {
    on_failure(tok);
    done_ = true;
}

void on_success(const mqtt::token& tok) {
    on_success(tok);
    done_ = true;
}

int mqtt_class::mqtt_send_data(vector<string> detections)
{
    cout << "\nSending message..." << endl;
    mqtt::message_ptr pubmsg = mqtt::make_message("TOPIC", "test");
    pubmsg->set_qos(QOS);
    //client.publish(pubmsg)->wait_for(TIMEOUT);
    //cout << "  ...OK" << endl;
}

mqtt_class::mqtt_class()
{

}



int mqtt_class::mqtt_init()
{
    const string DFLT_SERVER_ADDRESS	{ "tcp://localhost:1883" };
    const string CLIENT_ID				{ "paho_cpp_async_publish" };
    const string PERSIST_DIR			{ "./persist" };
    const string TOPIC { "hello" };

    const char* PAYLOAD1 = "Hello World!";
    const char* PAYLOAD2 = "Hi there!";
    const char* PAYLOAD3 = "Is anyone listening?";
    const char* PAYLOAD4 = "Someone is always listening.";
    const char* LWT_PAYLOAD = "Last will and testament.";

	// A client that just publishes normally doesn't need a persistent
	// session or Client ID unless it's using persistence, then the local
	// library requires an ID to identify the persistence files.

	string	address  = DFLT_SERVER_ADDRESS,
			clientID = CLIENT_ID;

	cout << "Initializing for server '" << address << "'..." << endl;
    mqtt::async_client client(address, clientID, PERSIST_DIR);


	callback cb;
	client.set_callback(cb);

	auto connOpts = mqtt::connect_options_builder()
		.clean_session()
		.will(mqtt::message(TOPIC, LWT_PAYLOAD, QOS))
		.finalize();

	cout << "  ...OK" << endl;

	try {
		cout << "\nConnecting..." << endl;
		mqtt::token_ptr conntok = client.connect(connOpts);
		cout << "Waiting for the connection..." << endl;
		conntok->wait();
		cout << "  ...OK" << endl;
	}
	catch (const mqtt::exception& exc) {
		cerr << exc.what() << endl;
		return 1;
	}

 	return 0;
}