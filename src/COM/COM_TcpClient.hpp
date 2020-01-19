/**
 * "COM_TcpClient.hpp"
 **/



#ifndef INC_COMTCPCLIENT_HPP_
#define INC_COMTCPCLIENT_HPP_



#include "COM_TcpCommon.hpp"



namespace COM
{
	/**
		@class CTcpClient
		@brief TCP communication client class
	 **/
	class CTcpClient
	{
		public:
			/**
				@brief constructor
			 **/
			CTcpClient();

			/**
				@brief constructor
				@param[in] p_serverSocketPort : TCP server port
				@param[in] p_serverSocketIpAddr : TCP server IP address
			 **/
			CTcpClient(int p_serverSocketPort, string p_serverSocketIpAddr);

			/**
				@brief destructor
			 **/
			virtual ~CTcpClient();

			/**
				@brief method to initialize the TCP client socket
				@return the TCP client socket
			 **/
			int initTcpClient();

			/**
				@brief method to start the TCP client socket
			 **/
			int startTcpClient();

		private:
		    sockaddr_in m_serverSocketAddr;						//< TCP server socket address
		    socklen_t	m_serverSocketAddrSize;					//< TCP server socket address size
		    string 		m_serverIpAddress;						//< TCP server IP address
		    string 		m_clientInputMsg;						//< TCP client input message
			int 		m_clientRequestedMsgId;					//< TCP client requested message ID
		    int 		m_clientSocket;							//< TCP client socket
		    int 		m_serverSocketPort;						//< TCP server socket port
			int 		m_clientReceivedBytesNb;				//< TCP client received bytes number from TCP server
		    char 		m_clientReceivedBuffer[BUFFER_SIZE];	//< TCP client received buffer from TCP server
	};
}



#endif /* INC_COMTCPCLIENT_HPP_ */
