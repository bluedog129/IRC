#include "../../includes/Command.hpp"
#include "../../includes/Message.hpp"
#include "../../includes/Buffer.hpp"
#include "../../includes/utils/reply.hpp"
#include "../../includes/utils/Error.hpp"
#include "../../includes/Lists.hpp"
#include "../../includes/utils/Containers.hpp"
#include "../../includes/Client.hpp"

// PING 명령어의 처리를 담당하는 함수
void Command::ping(Client& client, std::string const& serverHost) {
    // 메시지 파라미터를 가져옴
    messageVector const& message = Message::getMessage();

    // Command::pong(client, serverHost, message[1]);
    // // 메시지 파라미터의 크기가 1이 아니면 오류 메시지 전송
    // std::cout << "WE ARE IN PING" << std::endl;


    // PING 만 들어온 경우
    if (message.size() == 1) {
        // std::cout << "WE ARE IN PING ERROR 1" << std::endl;
        // std::cout << "message.size() : " << message.size() << std::endl;
        // for (size_t i = 0; i < message.size(); i++) {
            // std::cout << "message[" << i << "] : " << message[i] << std::endl;
        // }
        Error::ERR_NOORIGIN(serverHost, client.getNickname());
    }
    
    // // 메시지 파라미터의 크기가 2가 아니면 추가적인 파라미터 필요 오류 메시지 전송
    else if (message.size() != 2) {
    //     std::cout << "WE ARE IN PING ERROR 2" << std::endl;
        Error::ERR_NEEDMOREPARAMS(serverHost, message[0]);
    }


    else {
    //     // PONG 명령어 호출
    //     std::cout << "WE ARE GOING TO PONG" << std::endl;
        Command::pong(client, serverHost, message[1]);
    }
}

// PONG 명령어의 처리를 담당하는 함수
void Command::pong(Client& client, std::string const& serverHost, std::string const& token) {
    // PONG 메시지를 클라이언트에게 전송
    // std::cout << "WE ARE SENDING PONG" << std::endl;
    Buffer::sendMessage(client.getClientFd(), ":" + serverHost + " PONG " + serverHost + " :" + token + "\r\n");
}
