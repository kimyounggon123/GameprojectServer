#ifndef _GAMEROOMBROADCAST_H
#define _GAMEROOMBROADCAST_H


#include "PacketForBroadcast.h"
class GameroomBroadcast
	: public PacketForBroadcast
{

	static GameroomBroadcast* instance;
	GameroomBroadcast():PacketForBroadcast(){}


public:
	static GameroomBroadcast& getInstance() {
		if (instance == nullptr) instance = new GameroomBroadcast;
		return *instance;
	}
};
#endif