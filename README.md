SandboxSimulator2014
====================

ASPLOSIONS
omg wtf

Packet headers:

From Client to Server:

(4)ClientPacketSequence << (1)Packet_Type << (2)ClientID << (?)Data

From Server to Client:

(4)ServerPacketSequence << (1)Packet_Type << (?)Data

On client use ConnectionSystem->CreatePacket(PacketType) then << Data

On server use Client()->CreatePacket(PacketType) then << Data