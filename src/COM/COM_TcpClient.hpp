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
				@brief method to send a request message ID to TCP server
				@param[in] p_RequestedMsgId : client requested message ID
				@return -1 if failed
			 **/
			int sendRequestedMsgIdToServer(uint32_t p_RequestedMsgId);

			/**
				@brief methods to receive a message from TCP server
				@return -1 if failed
			 **/
			void 	receptionThread();
			int 	receiveMsgHeaderFromServer();
			int 	receivePathMsgFromServer();
			int 	receivePathCorrectionMsgFromServer();
			int 	receiveWorkShopOrderMsgFromServer();
			int 	receiveStopMsgFromServer();
			int 	receiveWorkShopReportMsgFromServer();
			int 	receiveBitReportMsgFromServer();
			int 	receiveErrorMsgFromServer();

		private:
			thread					m_receptionThread;						//< TCP client reception thread for message from server
			SMsgHeader 				m_msgHeader;							//< TCP client reception buffer for message header from server
			SPathMsgBody 			m_pathMsgBody; 							//< TCP client reception buffer for path message from server
			SPathCorrectionMsgBody	m_pathCorrectionMsgBody;				//< TCP client reception buffer for path correction message from server
			SWorkShopOrderMsgBody 	m_workShopOrderMsgBody;					//< TCP client reception buffer for workshop order message from server
			SStopMsgBody		 	m_stopMsgBody;							//< TCP client reception buffer for stop message from server
			SWorkShopReportMsgBody	m_workShopReportMsgBody;				//< TCP client reception buffer for workshop order report message
			SBitReportMsgBody 		m_bitReportMsgBody;						//< TCP client reception buffer for bit report message from server from server
			SErrorMsgBody 			m_errorMsgBody;							//< TCP client reception buffer for error message from server
		    sockaddr_in 			m_serverSocketAddr;						//< TCP server socket address
		    string 					m_serverIpAddress;						//< TCP server IP address
		    int 					m_clientSocket;							//< TCP client socket
		    int 					m_serverSocketPort;						//< TCP server socket port
			int 					m_clientReceivedBytesNb;				//< TCP client received bytes number from TCP server
	};
}



#endif /* INC_COMTCPCLIENT_HPP_ */
