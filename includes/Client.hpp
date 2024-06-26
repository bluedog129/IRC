#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "./utils/Headers.hpp"
#include "./utils/Containers.hpp"
#include "./Lists.hpp"

class Channel;

// IRC 서버의 클라이언트. 여러 정보를 저장할 예정
class Client {
private :
    int _passConnect; // PASS, NICK, USER 전부를 거쳤는 지 검증. 비트마스킹.
    bool _operator;
    time_t _finalTime;
    int _clientFd;
    in_addr _info;
    std::string _host;
    std::string _nickname;
    std::string _realname;
    std::string _username;
    std::string _server;
    ChannelMap _joinList;

    Server* _serverPtr;

public :
    Client(int clientFd, in_addr info, Server* server);
    ~Client();

	void setPassConnect(int flag);
    void setNickname(std::string nickname);
    void setRealname(std::string realname);
    void setHost(std::string host);
    void setUsername(std::string username);
    void setServer(std::string server);
    void setTime();

	void addJoinList(Channel* channel);
	void deleteJoinList(Channel* channel);
	int joinChannel(Channel* channel, std::string const& key);

	int getPassConnect() const;
	int getClientFd() const;
	bool getOperator() const;
	std::string const& getHost() const;
	std::string const& getNickname() const;
	std::string const& getRealname() const;
	std::string const& getUsername() const;
	std::string const& getServer() const;
	time_t const& getTime() const;
	in_addr const& getAddr() const;

    void setServerPtr(Server* server);
    Server* getServerPtr() const;
};

#endif
