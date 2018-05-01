#ifndef BUGNET_H_
#define BUGNET_H_

#include <mosquittopp.h>
#include <string>

class bugnet : public mosqpp::mosquittopp
{
public:
    bugnet(const char* id, const char* host_, int port_);
    virtual ~bugnet();

    void do_publish(std::string topic, std::string value);

    void set_message_callback(void (*callback)(std::string,std::string));
private:
    void on_connect(int rc);
    void on_publish(int mid);
    void on_message(const struct mosquitto_message *message);

    std::string host;
    int port;

    void (*on_message_callback)(std::string,std::string);
};

#endif
