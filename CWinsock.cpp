#pragma comment ( lib, "ws2_32.lib" )

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <vector>
#include "CWinsock.h"

CWinsock g_Winsock;

CWinsock::CWinsock ( VOID )
{
	m_ConnectedSocket = INVALID_SOCKET;
}

CWinsock::~CWinsock ( VOID )
{

}

VOID CWinsock::InitWinsock ( VOID )
{
	WSADATA wsaData;
	
	if ( WSAStartup ( MAKEWORD ( 2, 2 ), &wsaData ) != 0 ) 
	{
		::MessageBox ( NULL, "Winsock konnte nicht initialisiert werden!", "Error", MB_ICONINFORMATION );
		ExitProcess ( 0 );
	}

}

VOID CWinsock::Connect ( const char* pszServerUrl )
{
	m_pszConnectedUrl = pszServerUrl;

	addrinfo hints, *pAddrInfo;

	SecureZeroMemory ( &hints, sizeof ( hints ) );
	
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if ( getaddrinfo ( pszServerUrl, "80", &hints, &pAddrInfo ) != 0 ) 
	{
		::MessageBox ( NULL, "getaddrinfo ist fehlgeschlagen!", "Error", MB_ICONINFORMATION );
		CleanUp( );
		ExitProcess ( 0 );
	}

	if ( ( m_ConnectedSocket = socket ( pAddrInfo->ai_family, pAddrInfo->ai_socktype, pAddrInfo->ai_protocol ) ) == INVALID_SOCKET ) 
	{
		::MessageBox ( NULL, "Socket konnte nicht erstellt werden!", "Error", MB_ICONINFORMATION );
    	freeaddrinfo ( pAddrInfo );
    	CleanUp( );
		ExitProcess ( 0 );
	}

	if ( connect ( m_ConnectedSocket, pAddrInfo->ai_addr, ( INT ) pAddrInfo->ai_addrlen ) != 0 ) 
	{
		::MessageBox ( NULL, "Es konnte keine Verbindung zum Server hergestellt werden!", "Error", MB_ICONINFORMATION );
		freeaddrinfo ( pAddrInfo );
		Disconnect( );
		CleanUp( );
		ExitProcess ( 0 );
	}

	freeaddrinfo ( pAddrInfo );
}

VOID CWinsock::SendHttpGet ( const char* pszFileUrl, std::string &sDestBuffer )
{
	std::string sMsg = "GET /";
	sMsg += pszFileUrl;
	sMsg += " HTTP/1.0\r\nHost: "; // 1.0 statt 1.1 wegen Chunked_transfer_encoding
	sMsg += m_pszConnectedUrl;
	sMsg += "\r\n\r\n";

	CHAR szRecvBuffer [ 512 ];

	if ( send ( m_ConnectedSocket, sMsg.c_str( ), sMsg.length( ), 0 ) == SOCKET_ERROR ) 
	{
		::MessageBox ( NULL, "HttpGet konnte nicht gesendet werden!", "Error", MB_ICONINFORMATION );
    	Disconnect( );
    	WSACleanup( );
		ExitProcess ( 0 );
	}
	
	// Receive data until the server closes the connection
	INT iBytesReceived = 0;
	do 
	{
		iBytesReceived = recv ( m_ConnectedSocket, szRecvBuffer, sizeof ( szRecvBuffer ), 0 );
		
		sDestBuffer.append ( szRecvBuffer, iBytesReceived );

	} while ( iBytesReceived > 0 );

	//MessageBox ( 0, sDestBuffer.c_str( ), "k", 0 );
	// httpheader entfernen

	for ( size_t i = 0; i < sDestBuffer.length( ); i++ )
	{
		if ( sDestBuffer [ i ] == '\r' && sDestBuffer [ i + 1 ] == '\n' && sDestBuffer [ i + 2 ] == '\r' && sDestBuffer [ i + 3 ] == '\n' )
			sDestBuffer.erase ( 0, i + 4 );
	}

}

VOID CWinsock::Disconnect ( VOID )
{
	if ( m_ConnectedSocket != INVALID_SOCKET )
	{
		closesocket ( m_ConnectedSocket );
		m_ConnectedSocket = INVALID_SOCKET;
	}

}

VOID CWinsock::CleanUp ( VOID )
{
	WSACleanup( );
}