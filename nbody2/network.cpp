#include "network.h"
#include <iostream>

using std::cerr;
using std::endl;

sf::UdpSocket Network::socket;
std::string Network::id;

void Network::init()
{
	id = std::to_string(rand());

	if (socket.bind(PORT) != sf::Socket::Done) {
		cerr << "ERROR_BINDING_SOCKET_TO_PORT " << PORT << endl;
	}
}

void Network::send(const Game& game)
{
	sf::Packet packet = createPlayerPacket(game.player);
	//createBulletPacket(game.bullets);

	socket.send(packet, serverIp, PORT);
}


sf::Packet Network::createPlayerPacket(const Player& player)
{
	sf::Packet packet;
	packet << id;

	packet << player.x << player.y << player.dir;

	return packet;
}

void Network::createBulletPacket(const std::vector<Bullet>& bullets)
{
	;
}
