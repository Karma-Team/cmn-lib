/**
 * "COM_TcpClient.cpp"
 **/



#include <COM_TcpClient.hpp>



COM::CTcpClient::CTcpClient()
{
    m_serverIpAddress 				= TCP_SERVER_IP_ADDRESS;
	m_clientSocket					= -1;
    m_serverSocketPort				= TCP_SERVER_PORT;
    m_clientReceivedBytesNb 		= 0;
}



COM::CTcpClient::CTcpClient(int p_serverSocketPort, string p_serverSocketIpAddr)
{
	m_serverIpAddress				= p_serverSocketIpAddr;
	m_clientSocket					= -1;
	m_serverSocketPort				= p_serverSocketPort;
    m_clientReceivedBytesNb 		= 0;
}



COM::CTcpClient::~CTcpClient()
{
    // Close the client socket
    	close(m_clientSocket);
		cout << "TCP client socket closed" << endl;
}



int COM::CTcpClient::initTcpClient()
{
	cout << "Initialize the TCP client" << endl;
    socklen_t l_serverSocketAddrSize;

    // Create the client socket
		m_clientSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (m_clientSocket == -1)
		{
			cerr << "Can't create the client socket! Quitting" << endl;
			return 1;
		}

	// Connect the client socket to the server one
		m_serverSocketAddr.sin_family 	= AF_INET;
		m_serverSocketAddr.sin_port 	= htons(m_serverSocketPort);
		if(inet_pton(AF_INET, m_serverIpAddress.c_str(), &m_serverSocketAddr.sin_addr) != 1)
		{
			cerr << "Can't convert the Internet address! Quitting" << endl;
			return -1;
		}

		l_serverSocketAddrSize = sizeof(m_serverSocketAddr);
		if(connect(m_clientSocket, (sockaddr*)&m_serverSocketAddr, l_serverSocketAddrSize) == -1)
		{
			cerr << "Can't connect the client socket to the server one! Quitting" << endl;
			return -1;
		}

	return m_clientSocket;
}



int COM::CTcpClient::sendRequestedMsgIdToServer(uint32_t p_RequestedMsgId)
{
	if(send(m_clientSocket, &p_RequestedMsgId, sizeof(p_RequestedMsgId), 0) == -1)
	{
		cout << "Can't send requested message ID to server! Please try again \r\n";
		return -1;
	}
	cout << "> Requested message ID sent to server \r\n";

	return 1;
}



int COM::CTcpClient::receivePathMsgFromServer()
{
	m_clientReceivedBytesNb = recv(m_clientSocket, m_pathMsgBody, sizeof(SPathMsgBody), 0);
	if (m_clientReceivedBytesNb == -1)
	{
		cerr << "Error in recv()!" << endl;
		return -1;
	}
	else
	{
		cout << "Path message received from server" << "\r\n";
	}
	return 1;
}



int COM::CTcpClient::receivePathCorrectionMsgFromServer()
{
	m_clientReceivedBytesNb = recv(m_clientSocket, m_pathCorrectionMsgBody, sizeof(SPathCorrectionMsgBody), 0);
	if (m_clientReceivedBytesNb == -1)
	{
		cerr << "Error in recv()!" << endl;
		return -1;
	}
	else
	{
		cout << "Path correction message received from server" << "\r\n";
	}
	return 1;
}



int COM::CTcpClient::receiveWorkShopOrderMsgFromServer()
{
	m_clientReceivedBytesNb = recv(m_clientSocket, m_workShopOrderMsgBody, sizeof(SWorkShopOrderMsgBody), 0);
	if (m_clientReceivedBytesNb == -1)
	{
		cerr << "Error in recv()!" << endl;
		return -1;
	}
	else
	{
		cout << "Workshop order message received from server" << "\r\n";
	}
	return 1;
}



int COM::CTcpClient::receiveStopMsgFromServer()
{
	m_clientReceivedBytesNb = recv(m_clientSocket, m_stopMsgBody, sizeof(SStopMsgBody), 0);
	if (m_clientReceivedBytesNb == -1)
	{
		cerr << "Error in recv()!" << endl;
		return -1;
	}
	else
	{
		cout << "Stop message received from server" << "\r\n";
	}
	return 1;
}



int COM::CTcpClient::receiveWorkShopReportMsgFromServer()
{
	m_clientReceivedBytesNb = recv(m_clientSocket, m_workShopReportMsgBody, sizeof(SWorkShopReportMsgBody), 0);
	if (m_clientReceivedBytesNb == -1)
	{
		cerr << "Error in recv()!" << endl;
		return -1;
	}
	else
	{
		cout << "Workshop report message received from server" << "\r\n";
	}
	return 1;
}



int COM::CTcpClient::receiveBitReportMsgFromServer()
{
	m_clientReceivedBytesNb = recv(m_clientSocket, m_bitReportMsgBody, sizeof(SBitReportMsgBody), 0);
	if (m_clientReceivedBytesNb == -1)
	{
		cerr << "Error in recv()!" << endl;
		return -1;
	}
	else
	{
		cout << "Bit report message received from server" << "\r\n";
	}
	return 1;
}



int COM::CTcpClient::receiveErrorMsgFromServer()
{
	m_clientReceivedBytesNb = recv(m_clientSocket, m_errorMsgBody, sizeof(SErrorMsgBody), 0);
	if (m_clientReceivedBytesNb == -1)
	{
		cerr << "Error in recv()!" << endl;
		return -1;
	}
	else
	{
		cout << "Error message received from server" << "\r\n";
	}
	return 1;
}
