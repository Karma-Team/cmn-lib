/**
 * "COM_TcpServer.cpp"
 **/



#include <COM_TcpServer.hpp>



COM::CTcpServer::CTcpServer()
{
    m_serverSocketAddrSize			= 0;
    m_clientSocketAddrSize			= 0;
    m_serverIpAddress 				= TCP_SERVER_IP_ADDRESS;
    m_serverSocketPort				= TCP_SERVER_PORT;
    m_serverReceivedBytesNb 		= 0;
	m_serverSocket					= -1;
	m_clientSocket					= -1;
    strcpy(m_serverReceivedBuffer, 	"");
    strcpy(m_clientName, 			"");
    strcpy(m_clientPort, 			"");
}



COM::CTcpServer::CTcpServer(int p_serverSocketPort, string p_serverSocketIpAddr)
{
    m_serverSocketAddrSize			= 0;
    m_clientSocketAddrSize			= 0;
    m_serverIpAddress 				= p_serverSocketIpAddr;
    m_serverSocketPort				= p_serverSocketPort;
    m_serverReceivedBytesNb 		= 0;
	m_serverSocket					= -1;
	m_clientSocket					= -1;
    strcpy(m_serverReceivedBuffer, 	"");
    strcpy(m_clientName, 			"");
    strcpy(m_clientPort, 			"");
}



COM::CTcpServer::~CTcpServer()
{
    // Close the server socket
    	close(m_serverSocket);
		cout << "> TCP server socket closed" << endl;
}



int COM::CTcpServer::initTcpServer()
{
	cout << "> Initialize the TCP server" << endl;

	// Create the TCP server socket
		m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
		if(m_serverSocket == -1)
		{
			cerr << "> Can't create the server socket! Quitting" << endl;
			return -1;
		}
		cout << "> TCP server socket created" << endl;

    // Assign an address and a port to the TCP server socket
		m_serverSocketAddr.sin_family	= AF_INET;
		m_serverSocketAddr.sin_port 	= htons(m_serverSocketPort);
		if(inet_pton(AF_INET, m_serverIpAddress.c_str(), &m_serverSocketAddr.sin_addr) != 1)
		{
			cerr << "> Can't convert the Internet address! Quitting" << endl;
			return -1;
		}

		m_serverSocketAddrSize = sizeof(m_serverSocketAddr);
		if(bind(m_serverSocket, (sockaddr*)&m_serverSocketAddr, m_serverSocketAddrSize) == -1)
		{
			cerr << "> Can't assign an address and a port to the server socket! Quitting" << endl;
			return -1;
		}
		cout << "> Address and port assigned to the TCP server socket" << endl;

    // Enable the TCP server socket to accept client connections
		if(listen(m_serverSocket, SOMAXCONN) == -1)
		{
			cerr << "> Can't enable the server socket to accept connections! Quitting" << endl;
			return -1;
		}
		cout << "> Client connections enabled to the TCP server socket" << endl;

	return m_serverSocket;
}


int COM::CTcpServer::startTcpServer()

{
	cout << "> Start the TCP server" << endl;

	int 	l_clientRequestedMsgId[1];

	if(m_serverSocket != -1)
	{
		// Wait for a client connection
			m_clientSocketAddrSize 	= sizeof(m_clientSocketAddr);
			m_clientSocket 			= accept(m_serverSocket, (sockaddr*)&m_clientSocketAddr, &m_clientSocketAddrSize);

		// Translate the client socket address to a location and a service name
			memset(m_clientName, 0, NI_MAXHOST);
			memset(m_clientPort, 0, NI_MAXSERV);
			if(getnameinfo((sockaddr*)&m_clientSocketAddr, sizeof(m_clientSocketAddr), m_clientName, NI_MAXHOST, m_clientPort, NI_MAXSERV, 0) == 0)
			{
				cout << "> " << m_clientName << " connected on port " << m_clientPort << endl;
			}
			else
			{
				inet_ntop(AF_INET, &m_clientSocketAddr.sin_addr, m_clientName, NI_MAXHOST);
				cout << "> " << m_clientName << " connected on port " << ntohs(m_clientSocketAddr.sin_port) << endl;
			}

		// While loop: accept and echo message back to client
			while(true)
			{
				// Initialize the buffer
					memset(m_serverReceivedBuffer, 0, BUFFER_SIZE);

				// Wait for the client to send data
					m_serverReceivedBytesNb = recv(m_clientSocket, l_clientRequestedMsgId, sizeof(l_clientRequestedMsgId), 0);
					if(m_serverReceivedBytesNb == -1)
					{
						cerr << "> Error in recv()! Quitting" << endl;
						break;
					}
					if(m_serverReceivedBytesNb == 0)
					{
						cout << "> Client disconnected! Quitting " << endl;
						break;
					}

				// Send requested message to client
					switch(l_clientRequestedMsgId[0])
					{
						case MSG_ID_PATH:
							cout << "> Requested message from client : MSG_ID_PATH\n";
							sendHeaderMsgToClient(l_clientRequestedMsgId[0], sizeof(SPathMsgBody));
							sendPathMsgToClient();
							break;

						case MSG_ID_PATH_CORRECTION:
							cout << "> Requested message from client : MSG_ID_PATH_CORRECTION\n";
							sendHeaderMsgToClient(l_clientRequestedMsgId[0], sizeof(SPathCorrectionMsgBody));
							sendPathCorrectionMsgToClient();
							break;

						case MSG_ID_WORKSHOP_ORDER:
							cout << "> Requested message from client : MSG_ID_WORKSHOP_ORDER\n";
							sendHeaderMsgToClient(l_clientRequestedMsgId[0], sizeof(SWorkShopOrderMsgBody));
							sendWorkShopOrderMsgToClient();
							break;

						case MSG_ID_STOP:
							cout << "> Requested message from client : MSG_ID_STOP\n";
							sendHeaderMsgToClient(l_clientRequestedMsgId[0], sizeof(SStopMsgBody));
							sendStopMsgToClient();
							break;

						case MSG_ID_WORKSHOP_REPORT:
							cout << "> Requested message from client : MSG_ID_WORKSHOP_REPORT\n";
							sendHeaderMsgToClient(l_clientRequestedMsgId[0], sizeof(SWorkShopReportMsgBody));
							sendWorkShopReportMsgToClient();
							break;

						case MSG_ID_BIT_REPORT:
							cout << "> Requested message from client : MSG_ID_BIT_REPORT\n";
							sendHeaderMsgToClient(l_clientRequestedMsgId[0], sizeof(SBitReportMsgBody));
							sendBitReportMsgToClient();
							break;

						case MSG_ID_ERROR:
							cout << "> Requested message from client : MSG_ID_ERROR\n";
							sendHeaderMsgToClient(l_clientRequestedMsgId[0], sizeof(SErrorMsgBody));
							sendErrorMsgToClient();
							break;

						default:
							sendHeaderMsgToClient(l_clientRequestedMsgId[0], 0);
							cout << "> Unknown message ID\n";
					}
			}

		// Close the client socket
			close(m_clientSocket);
	}
	else
	{
		cerr << "> TCP server is not initialized! Quitting" << endl;
		return -1;
	}

	return 1;
}



int COM::CTcpServer::sendHeaderMsgToClient(uint32_t p_msgId, uint32_t p_msgSize)
{
	m_msgHeader.id 		= p_msgId;
	m_msgHeader.size	= p_msgSize;

	if(send(m_clientSocket, &m_msgHeader, sizeof(SMsgHeader), 0) == -1)
	{
		cout << "> Can't send message header to client! Quitting " << endl;
		return -1;
	}
	cout << "> Message header sent to client" << "\n";

	return 1;
}



int COM::CTcpServer::sendPathMsgToClient()
{
	if(send(m_clientSocket, &m_pathMsgBody, sizeof(SPathMsgBody), 0) == -1)
	{
		cout << "> Can't send path message to client! Quitting " << endl;
		return -1;
	}
	cout << "> Path message sent to client" << "\n";

	return 1;
}



int COM::CTcpServer::sendPathCorrectionMsgToClient()
{
	if(send(m_clientSocket, &m_pathCorrectionMsgBody, sizeof(SPathCorrectionMsgBody), 0) == -1)
	{
		cout << "> Can't send path correction message to client! Quitting " << endl;
		return -1;
	}
	cout << "> Path correction message sent to client" << "\n";

	return 1;
}



int COM::CTcpServer::sendWorkShopOrderMsgToClient()
{
	if(send(m_clientSocket, &m_workShopOrderMsgBody, sizeof(SWorkShopOrderMsgBody), 0) == -1)
	{
		cout << "> Can't send workshop order message to client! Quitting " << endl;
		return -1;
	}
	cout << "> Workshop order message sent to client" << "\n";

	return 1;
}



int COM::CTcpServer::sendStopMsgToClient()
{
	if(send(m_clientSocket, &m_stopMsgBody, sizeof(SStopMsgBody), 0) == -1)
	{
		cout << "> Can't send stop message to client! Quitting " << endl;
		return -1;
	}
	cout << "> Stop message sent to client" << "\n";

	return 1;
}



int COM::CTcpServer::sendWorkShopReportMsgToClient()
{
	if(send(m_clientSocket, &m_workShopReportMsgBody, sizeof(SWorkShopReportMsgBody), 0) == -1)
	{
		cout << "> Can't send workshop report message to client! Quitting " << endl;
		return -1;
	}
	cout << "> Workshop report message sent to client" << "\n";

	return 1;
}



int COM::CTcpServer::sendBitReportMsgToClient()
{
	if(send(m_clientSocket, &m_bitReportMsgBody, sizeof(SBitReportMsgBody), 0) == -1)
	{
		cout << "> Can't send bit report message to client! Quitting " << endl;
		return -1;
	}
	cout << "> Bit report message sent to client" << "\n";

	return 1;
}



int COM::CTcpServer::sendErrorMsgToClient()
{
	if(send(m_clientSocket, &m_errorMsgBody, sizeof(SErrorMsgBody), 0) == -1)
	{
		cout << "> Can't send Error message to client! Quitting " << endl;
		return -1;
	}
	cout << "> Error message sent to client" << "\n";

	return 1;
}
