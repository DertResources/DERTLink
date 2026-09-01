#include "../include/TCPHarness.h"

namespace dlnk
{

void TCPHarness::AllocateDouble(double*& ptr, AllocationDirection d)
{    
    SCOPE_TRACE("TCPHarness::AllocateDouble");
    if(d == AllocationDirection::InBuffer)
        RecivingBuffer.AllocateDouble(ptr);
    else if(d == AllocationDirection::OutBuffer)
        OutgoingBuffer.AllocateDouble(ptr);
    else
        THROW_ERROR("AllocationDirection not handled");
};
void TCPHarness::AllocateFloat(float*& ptr, AllocationDirection d)
{    
    SCOPE_TRACE("TCPHarness::AllocateFloat");
    if(d == AllocationDirection::InBuffer)
        RecivingBuffer.AllocateFloat(ptr);
    else if(d == AllocationDirection::OutBuffer)
        OutgoingBuffer.AllocateFloat(ptr);
    else
        THROW_ERROR("AllocationDirection not handled");
};
void TCPHarness::AllocateUInt8(uint8_t*& ptr, AllocationDirection d)
{    
    SCOPE_TRACE("TCPHarness::AllocateUInt8");
    if(d == AllocationDirection::InBuffer)
        RecivingBuffer.AllocateUInt8(ptr);
    else if(d == AllocationDirection::OutBuffer)
        OutgoingBuffer.AllocateUInt8(ptr);
    else
        THROW_ERROR("AllocationDirection not handled");
};
void TCPHarness::AllocateUInt16(uint16_t*& ptr, AllocationDirection d)
{    
    SCOPE_TRACE("TCPHarness::AllocateUInt16");
    if(d == AllocationDirection::InBuffer)
        RecivingBuffer.AllocateUInt16(ptr);
    else if(d == AllocationDirection::OutBuffer)
        OutgoingBuffer.AllocateUInt16(ptr);
    else
        THROW_ERROR("AllocationDirection not handled");
};
void TCPHarness::AllocateUInt32(uint32_t*& ptr, AllocationDirection d)
{    
    SCOPE_TRACE("TCPHarness::AllocateUInt32");
    if(d == AllocationDirection::InBuffer)
        RecivingBuffer.AllocateUInt32(ptr);
    else if(d == AllocationDirection::OutBuffer)
        OutgoingBuffer.AllocateUInt32(ptr);
    else
        THROW_ERROR("AllocationDirection not handled");
};
void TCPHarness::AllocateUInt64(uint64_t*& ptr, AllocationDirection d)
{    
    SCOPE_TRACE("TCPHarness::AllocateUInt64");
    if(d == AllocationDirection::InBuffer)
        RecivingBuffer.AllocateUInt64(ptr);
    else if(d == AllocationDirection::OutBuffer)
        OutgoingBuffer.AllocateUInt64(ptr);
    else
        THROW_ERROR("AllocationDirection not handled");
};
void TCPHarness::AllocateInt8(int8_t*& ptr, AllocationDirection d)
{    
    SCOPE_TRACE("TCPHarness::AllocateInt8");
    if(d == AllocationDirection::InBuffer)
        RecivingBuffer.AllocateInt8(ptr);
    else if(d == AllocationDirection::OutBuffer)
        OutgoingBuffer.AllocateInt8(ptr);
    else
        THROW_ERROR("AllocationDirection not handled");
};
void TCPHarness::AllocateInt16(int16_t*& ptr, AllocationDirection d)
{    
    SCOPE_TRACE("TCPHarness::AllocateInt16");
    if(d == AllocationDirection::InBuffer)
        RecivingBuffer.AllocateInt16(ptr);
    else if(d == AllocationDirection::OutBuffer)
        OutgoingBuffer.AllocateInt16(ptr);
    else
        THROW_ERROR("AllocationDirection not handled");
};
void TCPHarness::AllocateInt32(int32_t*& ptr, AllocationDirection d)
{    
    SCOPE_TRACE("TCPHarness::AllocateInt32");
    if(d == AllocationDirection::InBuffer)
        RecivingBuffer.AllocateInt32(ptr);
    else if(d == AllocationDirection::OutBuffer)
        OutgoingBuffer.AllocateInt32(ptr);
    else
        THROW_ERROR("AllocationDirection not handled");
};
void TCPHarness::AllocateInt64(int64_t*& ptr, AllocationDirection d)
{    
    SCOPE_TRACE("TCPHarness::AllocateInt64");
    if(d == AllocationDirection::InBuffer)
        RecivingBuffer.AllocateInt64(ptr);
    else if(d == AllocationDirection::OutBuffer)
        OutgoingBuffer.AllocateInt64(ptr);
    else
        THROW_ERROR("AllocationDirection not handled");
};
void TCPHarness::AllocateBool(bool*& ptr, AllocationDirection d)
{    
    SCOPE_TRACE("TCPHarness::AllocateBool");
    if(d == AllocationDirection::InBuffer)
        RecivingBuffer.AllocateBool(ptr);
    else if(d == AllocationDirection::OutBuffer)
        OutgoingBuffer.AllocateBool(ptr);
    else
        THROW_ERROR("AllocationDirection not handled");
};
void TCPHarness::AllocateString(uint8_t& ptr, AllocationDirection d)
{    
    SCOPE_TRACE("TCPHarness::AllocateString");
    if(d == AllocationDirection::InBuffer)
        RecivingBuffer.AllocateString(ptr);
    else if(d == AllocationDirection::OutBuffer)
        OutgoingBuffer.AllocateString(ptr);
    else
        THROW_ERROR("AllocationDirection not handled");
};

TCPHarness::~TCPHarness()
{
    SCOPE_TRACE("TCPHarness::~TCPHarness");
#ifdef _WIN32
    closesocket(ClientSocket);
    WSACleanup();
#else
    close(sock_fd);
#endif
}

void TCPHarness::InitalizeSocket()
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
    iResult = getaddrinfo(NULL, STRINGIFY(DEFAULT_PORT), &hints, &result);
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
    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
        THROW_ERROR("socket() failed");

    // Allow quick restart on the same port
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        THROW_ERROR("setsockopt(SO_REUSEADDR) failed");

    // Bind
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(DEFAULT_PORT);

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
        THROW_ERROR("bind() failed");

    // Listen
    if (listen(server_fd, BACKLOG) < 0)
        THROW_ERROR("listen() failed");

    DISPLAY_DEBUG("Listening on port " + std::to_string(DEFAULT_PORT) + "...");

    // Accept
    client_fd = accept(server_fd, (struct sockaddr*)&addr, &addr_len);
    if (client_fd < 0) 
        THROW_ERROR("accept() failed");
    std::string strTmp = inet_ntoa(addr.sin_addr);
    DISPLAY_DEBUG("Client connected: " + strTmp + ":" + std::to_string(ntohs(addr.sin_port)));
#endif
}


}; // namespace: dlnk