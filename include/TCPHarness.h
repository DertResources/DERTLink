#pragma once
#include "../include/DertlibConfig.h"
#include "../include/TripleBuffer.h"
#include "../include/DebugTracer.h"
#include "../include/DynamicBuffer.h"
#include "../include/DebugTracer.h"

#ifdef _WIN32
    //this keeps windows.h from including winsock2
    #define _WINSOCKAPI_
    #include <windows.h>
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <stdlib.h>
    #include <stdio.h>
    #pragma comment (lib, "Ws2_32.lib")
#else
    #include <iostream>
    #include <cstring>
    #include <cerrno>
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
#endif


namespace dlnk
{

class TCPHarness
{
public:
    enum class AllocationDirection
    {
        InBuffer,
        OutBuffer
    };

    void InitalizeSocket();

    void AllocateDouble(double*& ptr, AllocationDirection d);
    void AllocateFloat(float*& ptr, AllocationDirection d);
    void AllocateUInt8(uint8_t*& ptr, AllocationDirection d);
    void AllocateUInt16(uint16_t*& ptr, AllocationDirection d);
    void AllocateUInt32(uint32_t*& ptr, AllocationDirection d);
    void AllocateUInt64(uint64_t*& ptr, AllocationDirection d);
    void AllocateInt8(int8_t*& ptr, AllocationDirection d);
    void AllocateInt16(int16_t*& ptr, AllocationDirection d);
    void AllocateInt32(int32_t*& ptr, AllocationDirection d);
    void AllocateInt64(int64_t*& ptr, AllocationDirection d);
    void AllocateBool(bool*& ptr, AllocationDirection d);
    void AllocateString(uint8_t& ptr, AllocationDirection d);
    
    ~TCPHarness();

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
#else
    int server_fd, client_fd;
    struct sockaddr_in addr{};
    socklen_t addr_len = sizeof(addr);
    char buf[128];
    int BACKLOG = 10;
    int sock_fd;
#endif
};


}; // namespace: dlnk