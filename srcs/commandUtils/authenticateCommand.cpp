#include "../../includes/Command.hpp"
#include "../../includes/utils/Containers.hpp"
#include "../../includes/Lists.hpp"
#include "../../includes/Buffer.hpp"
#include "../../includes/utils/reply.hpp"
#include "../../includes/Message.hpp"
#include "../../includes/Client.hpp"

// 닉네임이 중복되었는지 확인하는 함수
static bool duplicate_nick(std::string const& nick) {
    ClientMap& clientList = Lists::getClientList(); // 전체 클라이언트 목록을 가져옵니다.

    // 모든 클라이언트를 순회하며 닉네임이 중복되는지 확인합니다.
    for (ClientMap::iterator it = clientList.begin(); it != clientList.end(); it++) {
        if (it->second->getNickname() == nick)
            return true; // 중복된 닉네임이 있으면 true를 반환합니다.
    }
    return false; // 중복된 닉네임이 없으면 false를 반환합니다.
}

// 닉네임 변경 성공 시 모든 클라이언트에게 알리는 함수
static void successNickChange(Client& client, std::string changeNick) {
    ClientMap& clientList = Lists::getClientList(); // 전체 클라이언트 목록을 가져옵니다.

    // 모든 클라이언트에게 닉네임 변경 사실을 알립니다.
    for (ClientMap::iterator it = clientList.begin(); it != clientList.end(); it++) {
        Buffer::saveMessageToBuffer(it->second->getClientFd(), reply::RPL_SUCCESSNICK(client.getNickname(), client.getUsername(), client.getHost(), changeNick));
    }
}

// 서버의 메시지 오브 더 데이(MOTD) 명령어 처리 함수
void Command::motd(Client& client, std::string const& serverHost) {
    // MOTD 시작, 본문, 종료 메시지를 클라이언트에게 전송합니다.
    Buffer::saveMessageToBuffer(client.getClientFd(), reply::RPL_MOTDSTART(serverHost, client.getNickname()));
    Buffer::saveMessageToBuffer(client.getClientFd(), reply::RPL_MOTD(serverHost, client.getNickname(), "Hello! This is FT_IRC!"));
    Buffer::saveMessageToBuffer(client.getClientFd(), reply::RPL_ENDOFMOTD(serverHost, client.getNickname()));
}

// PASS 명령어 처리 함수
void Command::pass(Client& client, std::string const& password, std::string const& serverHost) {
    messageVector const& message = Message::getMessage();

    // PASS 명령어에 대한 유효성 검사 및 처리를 수행합니다.
    if (message.size() != 2) {
        Buffer::saveMessageToBuffer(client.getClientFd(), Error::ERR_NEEDMOREPARAMS(serverHost, "PASS"));
    } else if (client.getPassConnect() & IS_PASS) {
        Buffer::saveMessageToBuffer(client.getClientFd(), Error::ERR_ALREADYREGISTERED(serverHost));
    } else if (message[1] != password) {
        Buffer::saveMessageToBuffer(client.getClientFd(), Error::ERR_PASSWDMISMATCH(serverHost));
    } else {
        client.setPassConnect(IS_PASS); // 클라이언트의 상태를 업데이트합니다.
    }
}

// NICK 명령어 처리 함수
void Command::nick(Client& client, std::string const& serverHost) {
    messageVector const& message = Message::getMessage();

    // NICK 명령어에 대한 유효성 검사 및 처리를 수행합니다.
    if (message.size() != 2)
        Buffer::saveMessageToBuffer(client.getClientFd(), Error::ERR_NONICKNAMEGIVEN(serverHost));
    else if (duplicate_nick(message[1]))
        Buffer::saveMessageToBuffer(client.getClientFd(), Error::ERR_NICKNAMEINUSE(serverHost, message[1]));
    else if (chkForbiddenChar(message[1], "#:") || std::isdigit(message[1][0]) || message[1] == "" || message[1].size() > 9)
        Buffer::saveMessageToBuffer(client.getClientFd(), Error::ERR_ERRONEUSNICKNAME(serverHost, message[1]));
    else {
        client.setPassConnect(IS_NICK);
        if (client.getNickname() != "") {
            successNickChange(client, message[1]); // 다른 클라이언트에게 닉네임 변경을 알립니다.
        }
        client.setNickname(message[1]); // 클라이언트의 닉네임을 업데이트합니다.
    }
}

// USER 명령어 처리 함수
void Command::user(Client& client, std::string const& serverHost, std::string const& serverIp, time_t const& serverStartTime) {
    messageVector const& message = Message::getMessage();

    // USER 명령어에 대한 유효성 검사 및 처리를 수행합니다.
    if (message.size() != 5)
        Buffer::saveMessageToBuffer(client.getClientFd(), Error::ERR_NEEDMOREPARAMS(serverHost, "USER"));
    else if (client.getPassConnect() & IS_USER)
        Buffer::saveMessageToBuffer(client.getClientFd(), Error::ERR_ALREADYREGISTERED(serverHost));
    else if (!(client.getPassConnect() & (IS_PASS | IS_NICK)))
        Buffer::saveMessageToBuffer(client.getClientFd(), Error::ERR_NOTREGISTERED(serverHost, "You input pass, before It enroll User"));
    else {
        client.setPassConnect(IS_USER);
        client.setUsername(message[1]);
        client.setHost(inet_ntoa(client.getAddr()));
        client.setServer(serverIp);
        if (message[4][0] == ':')
            client.setRealname(message[4].substr(1, message[4].size()));
        else
            client.setRealname(message[4]);
        if ((client.getPassConnect() & IS_LOGIN) == IS_LOGIN) {
            // 클라이언트가 로그인 상태인 경우, 서버 정보와 환영 메시지를 전송합니다.
            Buffer::saveMessageToBuffer(client.getClientFd(), reply::RPL_WELCOME(serverHost, client.getNickname(), client.getUsername(), client.getHost()));
            Buffer::saveMessageToBuffer(client.getClientFd(), reply::RPL_YOURHOST(serverHost, client.getNickname(), "1.0"));
            Buffer::saveMessageToBuffer(client.getClientFd(), reply::RPL_CREATED(serverHost, client.getNickname(), getStringTime(serverStartTime)));
            Buffer::saveMessageToBuffer(client.getClientFd(), reply::RPL_MYINFO(serverHost, client.getNickname(), "ircserv 1.0", "x", "itkol"));
            Buffer::saveMessageToBuffer(client.getClientFd(), reply::RPL_ISUPPORT(serverHost, client.getNickname()));
            Command::motd(client, serverHost); // MOTD 메시지를 전송합니다.
        }
    }
}
