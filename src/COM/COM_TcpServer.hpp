/**
 * "COM_TcpServer.hpp"
 **/



#ifndef INC_COMTCPSERVER_HPP_
#define INC_COMTCPSERVER_HPP_



#include "COM_TcpCommon.hpp"



namespace COM
{
	/**
		@class CTcpClient
		@brief TCP communication server class
	 **/
	class CTcpServer
	{
		public:
			/**
				@brief constructor
			 **/
			CTcpServer();

			/**
				@brief constructor
				@param[in] p_serverSocketPort : TCP server port
				@param[in] p_serverSocketIpAddr : TCP server IP address
			 **/
			CTcpServer(int p_serverSocketPort, string p_serverSocketIpAddr);

			/**
				@brief destructor
			 **/
			virtual ~CTcpServer();

			/**
				@brief method to initialize the TCP server socket
				@return the TCP server socket
			 **/
			int initTcpServer();

			/**
				@brief method to start the TCP server socket
			 **/
			int startTcpServer();

		private:
		    sockaddr_in m_serverSocketAddr;						//< TCP server socket address
		    sockaddr_in m_clientSocketAddr;						//< TCP client socket address
		    socklen_t 	m_serverSocketAddrSize;					//< TCP server socket address size
		    socklen_t 	m_clientSocketAddrSize;					//< TCP client socket address size
		    string 		m_serverIpAddress;						//< TCP server IP address
		    int 		m_clientRequestedMsgId; 				//< TCP client requested message ID
		    int 		m_serverSocketPort;						//< TCP server socket port
			int 		m_serverReceivedBytesNb;				//< TCP server received bytes number
			int 		m_serverSocket;							//< TCP server socket
		    int 		m_clientSocket;							//< TCP client socket
		    char 		m_serverReceivedBuffer[BUFFER_SIZE];	//< TCP server received buffer
			char 		m_clientName[NI_MAXHOST];				//< TCP client name
			char 		m_clientPort[NI_MAXSERV];				//< TCP client port
	};
}



#endif /* INC_COMTCPSERVER_HPP_ */
