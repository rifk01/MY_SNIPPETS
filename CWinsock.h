#ifndef __CWINSOCK__H__
#define __CWINSOCK__H__

class CWinsock
{
	public:
		CWinsock ( VOID );
		~CWinsock ( VOID );

		VOID InitWinsock ( VOID );
		VOID Connect ( const char* pszServerUrl );
		VOID SendHttpGet ( const char* pszFileUrl, std::string &sDestBuffer );

		VOID Disconnect ( VOID );
		VOID CleanUp ( VOID );
	private:
		SOCKET m_ConnectedSocket;
		const char* m_pszConnectedUrl;

}; extern CWinsock g_Winsock;

#endif