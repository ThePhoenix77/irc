#ifndef CHANNEL_HPP
#define CHANNEL_HPP


#include <string>
#include <vector>
#include "Server.hpp"
#include <set>



class Channel
{
private:
	std::string _ChannelName;
	std::string _Password; //if it was set
	std::vector<int> _clients;
	std::vector<int> _moderator;
	// std::set<int> 		_invited;

	bool _HasPassword;

	std::string _topic;

	bool _inviteOnly;
	bool _topicOpOnly;
	int _userLimit;

public:
	void setInviteOnly(bool inviteOnly) { _inviteOnly = inviteOnly;}
	bool isInviteOnly() const { return _inviteOnly;}
	void setTopicOpOnly(bool topicOpOnly) { _topicOpOnly = topicOpOnly; }
	bool isTopicOpOnly() const { return _topicOpOnly; }
	void setPassword(const std::string &password) { _Password = password; _HasPassword = true; }
	std::string getPassword() const { return _Password; }
	void setUserLimit(int limit) { _userLimit = limit; }
	int getUserLimit() const { return _userLimit; }

	bool addOperator(int fd);
	bool removeOperator(int fd);
	bool isOperator(int fd) const;


	void setTopic(const std::string &topic) { _topic = topic; }
	std::string getTopic() const { return _topic; }
	Channel();
	Channel(std::string name);
	Channel(std::string name, std::string password);
	~Channel();



	void SetChannelName(std::string& name);
	void SetPassword(std::string& password);
	bool AddClient(int fd);
	bool AddModerator(int fd);
	bool clientExist(int fd);
	bool clientIsOperator(int fd);
	std::string GetChannelName();
	std::string GetPassword();

	std::vector<int> &GetClient();
	// std::vector<Client> &GetModerator();
	bool GetHasPassword();


	void removeClient(int fd);
};







#endif
