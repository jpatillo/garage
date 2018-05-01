#include "bugnet.h"

#include <iostream>

bugnet::bugnet(const char* id, const char* host_, int port_) :
    mosquittopp(id), host(host_), port(port_)
{
    mosqpp::lib_init();

    int keepalive = 10;
    if (username_pw_set("sampleuser", "samplepass") != MOSQ_ERR_SUCCESS) {
        std::cout << "setting passwd failed" << std::endl;
    }
    connect_async(host.c_str(), port, keepalive);
    if (loop_start() != MOSQ_ERR_SUCCESS) {
        std::cout << "loop_start failed" << std::endl;
    }
}

bugnet::~bugnet()
{
    std::cout << "1" << std::endl;
    disconnect();
    if (loop_stop() != MOSQ_ERR_SUCCESS) {
        std::cout << "loop_stop failed" << std::endl;
    }
    std::cout << "2" << std::endl;
    mosqpp::lib_cleanup();
    std::cout << "3" << std::endl;
}

void bugnet::on_connect(int rc)
{
    std::cout << "Connected with code " << rc << "." << std::endl;
}

void bugnet::do_publish(std::string topic, std::string value) {
    int ret = publish(NULL, topic.c_str(), value.size(), value.c_str(), 1, false);
    if (ret != MOSQ_ERR_SUCCESS) {
        std::cout << "Sending failed." << std::endl;
    }
}

void bugnet::on_publish(int mid) {
    std::cout << "Published message with id: " << mid << std::endl;
}

void bugnet::on_message(const struct mosquitto_message *message) {
  // TODO validate the message

  // Call the callback
  (*on_message_callback)(message->topic,(char*)message->payload);
}

void bugnet::set_message_callback(void (*callback)(std::string,std::string)){
  on_message_callback = callback;
}
