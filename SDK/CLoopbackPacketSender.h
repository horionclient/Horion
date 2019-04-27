#pragma once

#include "CPacket.h"

class C_LoopbackPacketSender {
public:
	virtual ~C_LoopbackPacketSender();
	virtual __int64 send(C_Packet* packet);
	virtual __int64 sendToServer(C_Packet* packet);
	virtual __int64 sendToClient(const void* networkIdentifier, const C_Packet* packet, int a4);
	virtual __int64 sendBroadcast(const C_Packet* packet);
	virtual __int64 sendBroadcast(const void* networkIdentifier, int a3, const C_Packet* packet);
	virtual __int64 flush(void* networkIdentifier, int a3);
};