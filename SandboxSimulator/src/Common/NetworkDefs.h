#ifndef SandboxSimulator_Network_h
#define SandboxSimulator_Network_h

#include <SFML/Network.hpp>

namespace SandboxSimulator {
#define TIMEOUT_LIMIT 10000
#define CLIENT_LIMIT 256

enum PACKET_TYPE
{
    PT_CONNECT,
    PT_DISCONNECT,
    PT_COMMAND,
    PT_ESSENTIAL_COMMAND,
    PT_ACK,
    PT_PING,
    PT_COUNT
};

enum DISCONNECT_REASON
{
    DR_TIMEOUT,
    DR_QUIT,
    DR_COUNT
};
}

#endif