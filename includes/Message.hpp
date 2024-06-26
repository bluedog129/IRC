#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "./utils/Headers.hpp"
#include "./utils/Containers.hpp"

class Message {
private:
    static messageVector _commandElements;

public:
    static bool parseMessage(std::string& message);
    static void parseMessageAndExecute(int fd, std::string buffer, std::string host);
    static messageVector const& getMessage();
};

#endif