#ifndef _CLIENTMANAGER_H
#define _CLIENTMANAGER_H

#include "stdafx.h"
#include "Client.h"
class ClientManager
{
	std::vector<Client*>client_list;

	/*
	// unsigned int num_of_clients() const { return static_cast<unsigned int>(client_list.size()); }
	void print_num_of_clients() {
		//int clients = num_of_clients();
		//_tprintf(_T("Num of Clients: %d\n"), clients);

		for (auto* user : client_list)
			std::cout << "[ClientManager]clients in server: " << user->getName() << " \n";
	}
	*/
public:
	ClientManager() {}
	~ClientManager() {
		for (Client* client : client_list)
			delete client;
		client_list.clear();
	}
	std::vector<Client*> get_all_clients() const { return client_list; }
	Client* find_client(const char* name) {
		Client* found = nullptr;
		for (Client* client : client_list) {
			if (strcmp(client->getName(), name) == 0) {
				found = client;
				break;
			}

		}
		return found;
	}
	Client* find_client(const Client* to_find) {
		Client* found = nullptr;
		for (Client* client : client_list) {
			if (client == to_find) {
				found = client;
				break;
			}
		}
		return found;
	}
	Client* find_client(const Client& to_find) {
		Client* found = nullptr;
		for (Client* client : client_list) {
			if (client == &to_find) {
				found = client;
				break;
			}
		}
		return found;
	}
	Client* find_client(const SOCKADDR_IN& to_find) {
		Client* found = nullptr;
		for (Client* client : client_list) {
			if (client->get_addr().sin_addr.s_addr == to_find.sin_addr.s_addr) {
				found = client;
				break;
			}
		}
		return found;
	}

	bool add_client(Client* client) {
		if (client == nullptr) return false; // check parameter gets nullptr
		Client* already_here = find_client(client);
		if (already_here != nullptr) return false; // if client is already in here, return false
		client_list.push_back(client);

		// print_num_of_clients(); // print how many clients in this server.
		return true;
	}

	bool del_client(Client* client) {
		Client* isInHere = find_client(client);
		if (isInHere == nullptr) return false;
		for (auto it = client_list.begin(); it != client_list.cend(); it++) {
			if (*it == isInHere) {
				delete* it;
				client_list.erase(it);
				break;
			}
		}
		// print_num_of_clients(); // print how many clients in this server.
		return true;
	}
	bool del_client(const SOCKADDR_IN& leaved) {
		Client* isInHere = find_client(leaved);
		if (isInHere == nullptr) return false;
		for (auto it = client_list.begin(); it != client_list.cend(); it++) {
			if (*it == isInHere) {
				delete* it;
				client_list.erase(it);
				break;
			}
		}
		// print_num_of_clients(); // print how many clients in this server.
		return true;
	}
	
};
#endif
