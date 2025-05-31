#include "channel.hpp"

Channel::Channel() : _ChannelName(""), _Password(""), _HasPassword(false)
{
	// other defaults as needed
}

Channel::Channel(std::string name) : _ChannelName(name){
	_HasPassword = false;
}

Channel::Channel(std::string name, std::string password) : _ChannelName(name), _Password(password){
	_HasPassword = true;
}

Channel::~Channel(){
}

void Channel::SetChannelName(std::string& name){
	_ChannelName = name;
}

void Channel::SetPassword(std::string& password){
	_Password = password;
}

bool Channel::AddClient(int fd){
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (fd == _clients[i])
			return false;
	}
	_clients.push_back(fd);
	return true;
}

bool Channel::AddModerator(int fd){
	for (size_t i = 0; i < _moderator.size(); i++)
	{
		if (fd == _moderator[i])
			return false;
	}
	_moderator.push_back(fd);
	return true;
}

std::string Channel::GetChannelName(){
	return _ChannelName;
}

std::string Channel::GetPassword(){
	return _Password;
}

std::vector<int>& Channel::GetClient(){
	return _clients;
}

// std::vector<Client*>& Channel::GetModerator(){
// 	return _moderator;
// }

bool Channel::GetHasPassword(){
	return _HasPassword;
}

bool Channel::clientExist(int fd)
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (fd == _clients[i])
			return true;
	}
	return false;
}

bool Channel::clientIsOperator(int fd){
	for (size_t i = 0; i < _moderator.size(); i++)
	{
		if (fd == _moderator[i])
			return true;
	}
	return false;
}


void Channel::removeClient(int fd){
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i] == fd)
		{
			if (i < _clients.size())
			{
				_clients.erase(_clients.begin() + i);
				break;
			}
		}
	}
	for (size_t i = 0; i < _moderator.size(); i++)
	{
		if (i == _moderator.size())
		{
			if (i < _moderator.size())
			{
				_moderator.erase(_moderator.begin() + i);
				break;
			}
		}
	}
}


bool Channel::addOperator(int fd) {
    if (!isOperator(fd)) {
        _moderator.push_back(fd);
        return true;
    }
    return false;
}

bool Channel::removeOperator(int fd) {
    for (size_t i = 0; i < _moderator.size(); ++i) {
        if (_moderator[i] == fd) {
            _moderator.erase(_moderator.begin() + i);
            return true;
        }
    }
    return false;
}

bool Channel::isOperator(int fd) const {
    for (size_t i = 0; i < _moderator.size(); ++i) {
        if (_moderator[i] == fd)
            return true;
    }
    return false;
}