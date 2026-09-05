#include "../include/TCPHarness.h"

namespace dlnk
{

void TCPHarness::AllocateDouble(double*& ptr, AllocationDirection d)
{
    SCOPE_TRACE("TCPHarness::AllocateDouble");
    if(d == AllocationDirection::InBuffer)
        ReceivingBuffer.AllocateDouble(ptr);
    else if(d == AllocationDirection::OutBuffer)
        OutgoingBuffer.AllocateDouble(ptr);
    else
        THROW_ERROR("AllocationDirection not handled");
};
void TCPHarness::AllocateFloat(float*& ptr, AllocationDirection d)
{    
    SCOPE_TRACE("TCPHarness::AllocateFloat");
    if(d == AllocationDirection::InBuffer)
        ReceivingBuffer.AllocateFloat(ptr);
    else if(d == AllocationDirection::OutBuffer)
        OutgoingBuffer.AllocateFloat(ptr);
    else
        THROW_ERROR("AllocationDirection not handled");
};
void TCPHarness::AllocateUInt8(uint8_t*& ptr, AllocationDirection d)
{    
    SCOPE_TRACE("TCPHarness::AllocateUInt8");
    if(d == AllocationDirection::InBuffer)
        ReceivingBuffer.AllocateUInt8(ptr);
    else if(d == AllocationDirection::OutBuffer)
        OutgoingBuffer.AllocateUInt8(ptr);
    else
        THROW_ERROR("AllocationDirection not handled");
};
void TCPHarness::AllocateUInt16(uint16_t*& ptr, AllocationDirection d)
{    
    SCOPE_TRACE("TCPHarness::AllocateUInt16");
    if(d == AllocationDirection::InBuffer)
        ReceivingBuffer.AllocateUInt16(ptr);
    else if(d == AllocationDirection::OutBuffer)
        OutgoingBuffer.AllocateUInt16(ptr);
    else
        THROW_ERROR("AllocationDirection not handled");
};
void TCPHarness::AllocateUInt32(uint32_t*& ptr, AllocationDirection d)
{    
    SCOPE_TRACE("TCPHarness::AllocateUInt32");
    if(d == AllocationDirection::InBuffer)
        ReceivingBuffer.AllocateUInt32(ptr);
    else if(d == AllocationDirection::OutBuffer)
        OutgoingBuffer.AllocateUInt32(ptr);
    else
        THROW_ERROR("AllocationDirection not handled");
};
void TCPHarness::AllocateUInt64(uint64_t*& ptr, AllocationDirection d)
{    
    SCOPE_TRACE("TCPHarness::AllocateUInt64");
    if(d == AllocationDirection::InBuffer)
        ReceivingBuffer.AllocateUInt64(ptr);
    else if(d == AllocationDirection::OutBuffer)
        OutgoingBuffer.AllocateUInt64(ptr);
    else
        THROW_ERROR("AllocationDirection not handled");
};
void TCPHarness::AllocateInt8(int8_t*& ptr, AllocationDirection d)
{    
    SCOPE_TRACE("TCPHarness::AllocateInt8");
    if(d == AllocationDirection::InBuffer)
        ReceivingBuffer.AllocateInt8(ptr);
    else if(d == AllocationDirection::OutBuffer)
        OutgoingBuffer.AllocateInt8(ptr);
    else
        THROW_ERROR("AllocationDirection not handled");
};
void TCPHarness::AllocateInt16(int16_t*& ptr, AllocationDirection d)
{    
    SCOPE_TRACE("TCPHarness::AllocateInt16");
    if(d == AllocationDirection::InBuffer)
        ReceivingBuffer.AllocateInt16(ptr);
    else if(d == AllocationDirection::OutBuffer)
        OutgoingBuffer.AllocateInt16(ptr);
    else
        THROW_ERROR("AllocationDirection not handled");
};
void TCPHarness::AllocateInt32(int32_t*& ptr, AllocationDirection d)
{    
    SCOPE_TRACE("TCPHarness::AllocateInt32");
    if(d == AllocationDirection::InBuffer)
        ReceivingBuffer.AllocateInt32(ptr);
    else if(d == AllocationDirection::OutBuffer)
        OutgoingBuffer.AllocateInt32(ptr);
    else
        THROW_ERROR("AllocationDirection not handled");
};
void TCPHarness::AllocateInt64(int64_t*& ptr, AllocationDirection d)
{    
    SCOPE_TRACE("TCPHarness::AllocateInt64");
    if(d == AllocationDirection::InBuffer)
        ReceivingBuffer.AllocateInt64(ptr);
    else if(d == AllocationDirection::OutBuffer)
        OutgoingBuffer.AllocateInt64(ptr);
    else
        THROW_ERROR("AllocationDirection not handled");
};
void TCPHarness::AllocateBool(bool*& ptr, AllocationDirection d)
{    
    SCOPE_TRACE("TCPHarness::AllocateBool");
    if(d == AllocationDirection::InBuffer)
        ReceivingBuffer.AllocateBool(ptr);
    else if(d == AllocationDirection::OutBuffer)
        OutgoingBuffer.AllocateBool(ptr);
    else
        THROW_ERROR("AllocationDirection not handled");
};
void TCPHarness::AllocateString(uint8_t& ptr, AllocationDirection d)
{    
    SCOPE_TRACE("TCPHarness::AllocateString");
    if(d == AllocationDirection::InBuffer)
        ReceivingBuffer.AllocateString(ptr);
    else if(d == AllocationDirection::OutBuffer)
        OutgoingBuffer.AllocateString(ptr);
    else
        THROW_ERROR("AllocationDirection not handled");
};

void TCPHarness::InitalizeSocket()
{
#ifdef _WIN32
    SCOPE_TRACE("TCPHarness::InitalizeSocket (WIN32 Server)");
    if (ct == ConnectionType::Server)
    {
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
    }
    else
    {
        SCOPE_TRACE("TCPHarness::InitalizeSocket (WIN32 Client)");

        // Initialize Winsock
        iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
        if (iResult != 0)
           THROW_ERROR("WSAStartup failed with error: " + std::to_string(iResult) + "\n");

        ZeroMemory( &hints, sizeof(hints) );
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        // Resolve the server address and port
        iResult = getaddrinfo(ConnectionIP, ConnectionPort, &hints, &result);
        if ( iResult != 0 )
        {
            WSACleanup();
            THROW_ERROR("getaddrinfo failed with error: " + std::to_string(iResult));
        }
    }
#else
    if (ct == ConnectionType::Server)
    {
        SCOPE_TRACE("TCPHarness::InitalizeSocket (POSIX Server)");
        // Create socket
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd < 0)
            THROW_ERROR("socket() failed");

        // Allow quick restart on the same port
        int opt = 1;
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        {
            close(server_fd);
            THROW_ERROR("setsockopt(SO_REUSEADDR) failed");
        }

        // Bind
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(DEFAULT_PORT);

        if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
        {
            close(server_fd);        
            THROW_ERROR("bind() failed");
        }
    }
    else
    {
        SCOPE_TRACE("TCPHarness::InitalizeSocket (POSIX Client)");
        THROW_ERROR("Posix TCP Client Not Implemented Yet");
    }
#endif
}

void TCPHarness::PollForConnection()
{
    SCOPE_TRACE("TCPHarness::PollForClient");
#if _WIN32
    if(ct == ConnectionType::Server)
    {
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

        // Disable Nagle's on the communication socket
        BOOL nodelay = TRUE;
        iResult = setsockopt(ClientSocket, IPPROTO_TCP, TCP_NODELAY, (const char*)&nodelay, sizeof(nodelay));
        if(iResult == SOCKET_ERROR)
        {
            closesocket(ListenSocket);
            WSACleanup();
            THROW_ERROR("setsockopt failed with error: " + std::to_string(WSAGetLastError()) + "\n");
        }

        // close the listen as we now just have the client
        closesocket(ListenSocket);
    }
    else
    {
        // Attempt to connect to an address until one succeeds
        for(addrinfo* ptr=result; ptr != NULL ;ptr=ptr->ai_next)
        {
            // Create a SOCKET for connecting to server
            ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
                ptr->ai_protocol);
            if (ConnectSocket == INVALID_SOCKET)
            {
                WSACleanup();
                THROW_ERROR("socket failed with error: " + std::to_string(WSAGetLastError()));
            }

            // Connect to server.
            iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
            if (iResult == SOCKET_ERROR)
            {
                closesocket(ConnectSocket);
                ConnectSocket = INVALID_SOCKET;
                continue;
            }
            break;
        }

        freeaddrinfo(result);

        if (ConnectSocket == INVALID_SOCKET)
        {
            WSACleanup();
            THROW_ERROR("Unable to connect to server!");
        }
    }
#else
    if(ct == ConnectionType::Server)
    {
        // Listen
        if (listen(server_fd, BACKLOG) < 0)
        {
            close(server_fd);
            THROW_ERROR("listen() failed");
        }

        DISPLAY_DEBUG("Listening on port " + std::to_string(DEFAULT_PORT) + "...");

        // Accept
        client_fd = accept(server_fd, (struct sockaddr*)&addr, &addr_len);
        if (client_fd < 0) 
        {
            close(server_fd);
            THROW_ERROR("accept() failed");
        }

        // Disable Nagle's on the communication socket
        int nodelay = true;
        if(setsockopt(client_fd, IPPROTO_TCP, TCP_NODELAY, (int*)&nodelay, sizeof(int)) < 0)
        {
            close(client_fd);
            close(server_fd);
            THROW_ERROR("setsockopt() failed");
        }

        std::string strTmp = inet_ntoa(addr.sin_addr);
        DISPLAY_DEBUG("Client connected: " + strTmp + ":" + std::to_string(ntohs(addr.sin_port)));
        
        close(server_fd);
    }
    else
    {
        THROW_ERROR("Posix TCP Client Not Implemented Yet");
    }
#endif
}

TCPHarness::~TCPHarness()
{
    SCOPE_TRACE("TCPHarness::~TCPHarness");
#ifdef _WIN32
    closesocket(ClientSocket);
    WSACleanup();
#else
    close(client_fd);
#endif
}


}; // namespace: dlnk