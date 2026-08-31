#pragma once
#include "../include/DertlibConfig.h"
#include "../include/TripleBuffer.h"
#include "DebugTracer.h"
#include "DynamicBuffer.h"

#ifdef _WIN32
    #include <windows.h>
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <stdlib.h>
    #include <stdio.h>
    #pragma comment (lib, "Ws2_32.lib")
#endif

namespace dlnk
{

class TCPHarness
{
public:
    void InitalizeSocket()
    {
        SCOPE_TRACE("TCPHarness::InitalizeSockets");
#ifdef _WIN32
        // Initialize Winsock
        iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
        if (iResult != 0)
            THROW_ERROR("WSAStartup failed with error: " + std::to_string(iResult) + "\n");

        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_PASSIVE;

        // Resolve the server address and port
        iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
        if ( iResult != 0 ) {
            WSACleanup();
            THROW_ERROR("getaddrinfo failed with error: " + std::to_string(iResult) + "\n");
        }

        // Create a SOCKET for the server to listen for client connections.
        ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (ListenSocket == INVALID_SOCKET) {
            freeaddrinfo(result);
            WSACleanup();
            THROW_ERROR("socket failed with error: " + std::to_string(WSAGetLastError()) + "\n");
        }

        // Setup the TCP listening socket
        iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            freeaddrinfo(result);
            closesocket(ListenSocket);
            WSACleanup();
            THROW_ERROR("bind failed with error: " + std::to_string(WSAGetLastError()) + "\n");
        }

        freeaddrinfo(result);

        iResult = listen(ListenSocket, SOMAXCONN);
        if (iResult == SOCKET_ERROR) {
            closesocket(ListenSocket);
            WSACleanup();
            THROW_ERROR("listen failed with error: " + std::to_string(WSAGetLastError()) + "\n");
        }

        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            closesocket(ListenSocket);
            WSACleanup();
            THROW_ERROR("accept failed with error: " + std::to_string(WSAGetLastError()) + "\n");
        }

        closesocket(ListenSocket);
#else
        THROW_ERROR("Non-Windows TCP not implemented");
#endif
    }

#ifdef _WIN32
    ~TCPHarness()
    {
        closesocket(ClientSocket);
        WSACleanup();
    }
#endif

private:
    TripleBuffer RecivingBuffer;
    DynamicBuffer<1> OutgoingBuffer;
#ifdef _WIN32
    WSADATA wsaData;
    int iResult;
    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL;
    struct addrinfo hints;
    // int iSendResult;
    // char recvbuf[DEFAULT_BUFLEN];
    // int recvbuflen = DEFAULT_BUFLEN;
#endif
};


}; // namespace: dlnk