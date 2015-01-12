#include <iostream>
#include <stdio.h>

#include <Engine.h>
#include <Core/SceneGraph.h>
#include <Rendering/RenderSystem.h>
#include <Core/Message.h>

#include <Network/Socket.h>
#include <System/SSTypes.h>

using namespace SandboxSimulator;

int main(i32 ArgC,Literal ArgV[])
{
    /*
    SSEngine* Eng = new SSEngine();
    
	Eng->RegisterSystem(new RenderSystem());
    Eng->Initialize(ArgC,ArgV);

    Entity* E = Eng->GetSceneGraph()->CreateEntity();
    Eng->GetSceneGraph()->AddComponent(E, new RenderComponent());
    RenderComponent* r = (RenderComponent*)E->GetComponentByType(CT_RENDER);

    Material* Mat = new Material();
    r->SetMaterial(Mat);
    MatVec3Node* Vec3Node = new MatVec3Node("ConstInputTest", Vec3(1,0,1));
    Mat->SetInput(MI_ALBEDO, Vec3Node->GetOutput());

    r->AddVertex(Vec3(0.5 , 0.5,0));
    r->AddVertex(Vec3(0.5 ,-0.5,0));
    r->AddVertex(Vec3(-0.5,-0.5,0));

    r->AddVertex(Vec3(-0.5,-0.5,0));
    r->AddVertex(Vec3(-0.5, 0.5,0));
    r->AddVertex(Vec3( 0.5, 0.5,0));
    */

    UdpSocket* Socket = new UdpSocket();
    Socket->Bind(sf::Socket::AnyPort);
    sf::Packet* packet = new sf::Packet();
    (*packet) << (i32) 1 << (i8)PT_CONNECT;
    Socket->Send(packet, "127.0.0.1", 3889);

    //Connections ACK, returned assigned client id
    sf::IpAddress sender;
    u16 port;
    packet = Socket->Receive(sender, port);
    i8 PacketType;
    u16 ClientID;
    (*packet) >> PacketType >> ClientID;
    //

    //Send test packet
    packet = new sf::Packet();
    (*packet) << (i32) 2 << (i8)PT_UPDATE << ClientID << (i8)4;
    Socket->Send(packet, "127.0.0.1", 3889);

    //Get ACK for that packet
    bool Connected = true;
    while(Connected)
    {
        packet = Socket->Receive(sender, port);
        i8 PacketType;
        (*packet) >> PacketType;
        switch(PacketType)
        {
            case PT_ACK:
                //a packet was acknowledged, get the ack packet ID and remove it from pending ack queue (TODO)
                i32 PacketID;
                (*packet) >> PacketID;
                printf("Packet %d acknowledged by server!\n", PacketID);
                break;
            case PT_PING:
                    printf("Server pinged us! LOL let's just ignore it, nothing bad will happen... right?\n");
                break;
            case PT_DISCONNECT:
                i8 Reason;
                (*packet) >> Reason;
                if(Reason == DR_TIMEOUT)
                    printf("Disconnected, reason: timed out.\n");
                else if (Reason == DR_QUIT)
                    printf("Disconnected, reason: quit.\n");

                Connected = false;
                break;
            default:
                //process as usual, probably the world state.
                break;
        }
    }

    //Eng->Run();
    //Eng->Shutdown();
    return 0;
}
