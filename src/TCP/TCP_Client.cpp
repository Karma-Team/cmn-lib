/**
 * "TCP_Client.cpp"
 **/



#include "TCP_Client.hpp"



TCP::CTcpClient::CTcpClient()
{
    m_serverIpAddress 				= TCP_SERVER_IP_ADDRESS;
	m_clientSocket					= -1;
    m_serverSocketPort				= TCP_SERVER_PORT;
    m_clientReceivedBytesNb 		= 0;
}



TCP::CTcpClient::CTcpClient(int p_serverSocketPort, string p_serverSocketIpAddr)
{
	m_serverIpAddress				= p_serverSocketIpAddr;
	m_clientSocket					= -1;
	m_serverSocketPort				= p_serverSocketPort;
    m_clientReceivedBytesNb 		= 0;
}



TCP::CTcpClient::~CTcpClient()
{
    // Close the client socket
    	close(m_clientSocket);
		cout << "> TCP client socket closed" << endl;
}



int TCP::CTcpClient::initTcpClient()
{
	cout << "> Initialize the TCP client" << endl;

	socklen_t 	l_serverSocketAddrSize;

    // Create the client socket
		m_clientSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (m_clientSocket == -1)
		{
			cerr << "> Can't create the client socket! Quitting" << endl;
			return 1;
		}

	// Connect the client socket to the server one
		m_serverSocketAddr.sin_family 	= AF_INET;
		m_serverSocketAddr.sin_port 	= htons(m_serverSocketPort);
		if(inet_pton(AF_INET, m_serverIpAddress.c_str(), &m_serverSocketAddr.sin_addr) != 1)
		{
			cerr << "> Can't convert the Internet address! Quitting" << endl;
			return -1;
		}

		l_serverSocketAddrSize = sizeof(m_serverSocketAddr);
		if(connect(m_clientSocket, (sockaddr*)&m_serverSocketAddr, l_serverSocketAddrSize) == -1)
		{
			cerr << "> Can't connect the client socket to the server one! Quitting" << endl;
			return -1;
		}

	// Reception thread : wait for response from server
		m_receptionThread = thread(&TCP::CTcpClient::receptionThread, this);

	return m_clientSocket;
}



void TCP::CTcpClient::receptionThread()
{
	cout << "> Reception thread launched\n";

	while(1)
	{
		int l_id = this->receiveMsgHeaderFromServer();
		switch(l_id)
		{
			case MSG_ID_PATH:
				cout << "> Wait for response from server : MSG_ID_PATH\n";
				this->receivePathMsgFromServer();
				break;

			case MSG_ID_PATH_CORRECTION:
				cout << "> Wait for response from server : MSG_ID_PATH_CORRECTION\n";
				this->receivePathCorrectionMsgFromServer();
				break;

			case MSG_ID_WORKSHOP_ORDER:
				cout << "> Wait for response from server : MSG_ID_WORKSHOP_ORDER\n";
				this->receiveWorkShopOrderMsgFromServer();
				break;

			case MSG_ID_STOP:
				cout << "> Wait for response from server : MSG_ID_STOP\n";
				this->receiveStopMsgFromServer();
				break;

			case MSG_ID_WORKSHOP_REPORT:
				cout << "> Wait for response from server : MSG_ID_WORKSHOP_REPORT\n";
				this->receiveWorkShopReportMsgFromServer();
				break;

			case MSG_ID_BIT_REPORT:
				cout << "> Wait for response from server : MSG_ID_BIT_REPORT\n";
				this->receiveBitReportMsgFromServer();
				break;

			case MSG_ID_ERROR:
				cout << "> Wait for response from server : MSG_ID_ERROR\n";
				this->receiveErrorMsgFromServer();
				break;

			default:
				cout << "> Unknown message ID\n";
		}
	}
}



int TCP::CTcpClient::sendRequestedMsgIdToServer(uint32_t p_RequestedMsgId)
{
	if(send(m_clientSocket, &p_RequestedMsgId, sizeof(p_RequestedMsgId), 0) == -1)
	{
		cout << "> Can't send requested message ID to server! Please try again\n";
		return -1;
	}
	cout << "> Requested message ID sent to server\n";

	return 1;
}



int TCP::CTcpClient::receiveMsgHeaderFromServer()
{
	m_clientReceivedBytesNb = recv(m_clientSocket, &m_msgHeader, sizeof(SMsgHeader), 0);
	if (m_clientReceivedBytesNb == -1)
	{
		cerr << "> Error in recv()!" << endl;
		return -1;
	}
	else
	{
		cout << "> Message header received from server" << "\n";
	}
	return m_msgHeader.id;
}



int TCP::CTcpClient::receivePathMsgFromServer()
{
	m_clientReceivedBytesNb = recv(m_clientSocket, &m_pathMsgBody, sizeof(SPathMsgBody), 0);
	if (m_clientReceivedBytesNb == -1)
	{
		cerr << "> Error in recv()!" << endl;
		return -1;
	}
	else
	{
		cout << "> Path message received from server" << "\n";
	}
	return 1;
}



int TCP::CTcpClient::receivePathCorrectionMsgFromServer()
{
	m_clientReceivedBytesNb = recv(m_clientSocket, &m_pathCorrectionMsgBody, sizeof(SPathCorrectionMsgBody), 0);
	if (m_clientReceivedBytesNb == -1)
	{
		cerr << "> Error in recv()!" << endl;
		return -1;
	}
	else
	{
		cout << "> Path correction message received from server" << "\n";
	}
	return 1;
}



int TCP::CTcpClient::receiveWorkShopOrderMsgFromServer()
{
	m_clientReceivedBytesNb = recv(m_clientSocket, &m_workShopOrderMsgBody, sizeof(SWorkShopOrderMsgBody), 0);
	if (m_clientReceivedBytesNb == -1)
	{
		cerr << "> Error in recv()!" << endl;
		return -1;
	}
	else
	{
		cout << "> Workshop order message received from server" << "\n";
	}
	return 1;
}



int TCP::CTcpClient::receiveStopMsgFromServer()
{
	m_clientReceivedBytesNb = recv(m_clientSocket, &m_stopMsgBody, sizeof(SStopMsgBody), 0);
	if (m_clientReceivedBytesNb == -1)
	{
		cerr << "> Error in recv()!" << endl;
		return -1;
	}
	else
	{
		cout << "> Stop message received from server" << "\n";
	}
	return 1;
}



int TCP::CTcpClient::receiveWorkShopReportMsgFromServer()
{
	m_clientReceivedBytesNb = recv(m_clientSocket, &m_workShopReportMsgBody, sizeof(SWorkShopReportMsgBody), 0);
	if (m_clientReceivedBytesNb == -1)
	{
		cerr << "> Error in recv()!" << endl;
		return -1;
	}
	else
	{
		cout << "> Workshop report message received from server" << "\n";
	}
	return 1;
}



int TCP::CTcpClient::receiveBitReportMsgFromServer()
{
	m_clientReceivedBytesNb = recv(m_clientSocket, &m_bitReportMsgBody, sizeof(SBitReportMsgBody), 0);
	if (m_clientReceivedBytesNb == -1)
	{
		cerr << "> Error in recv()!" << endl;
		return -1;
	}
	else
	{
		cout << "> Bit report message received from server" << "\n";
	}
	return 1;
}



int TCP::CTcpClient::receiveErrorMsgFromServer()
{
	m_clientReceivedBytesNb = recv(m_clientSocket, &m_errorMsgBody, sizeof(SErrorMsgBody), 0);
	if (m_clientReceivedBytesNb == -1)
	{
		cerr << "> Error in recv()!" << endl;
		return -1;
	}
	else
	{
		cout << "> Error message received from server" << "\n";
	}
	return 1;
}
