#include <windows.h>
#include <winioctl.h>
#include <stdio.h>


BOOL bGetHddSerial( std::string &sHddSerial )
{
	STORAGE_DEVICE_DESCRIPTOR* pStorageDeviceDescriptor = NULL;
	STORAGE_PROPERTY_QUERY qry;

	DWORD dwBytesReturnValue = 0;
	HANDLE hDevice = INVALID_HANDLE_VALUE;
	BOOL bResult = FALSE;

	char szBuffer[ 10000 ];


	hDevice = CreateFile( 
		"\\\\.\\PhysicalDrive0",  // drive to open
		0,                        // no access to the drive
        FILE_SHARE_READ |         // share mode
        FILE_SHARE_WRITE, 
        NULL,                     // default security attributes
        OPEN_EXISTING,            // disposition
        0,                        // file attributes
        NULL );                   // do not copy file attributes

	
	if ( hDevice == INVALID_HANDLE_VALUE ) // cannot open the drive
		return FALSE;


	qry.PropertyId = StorageDeviceProperty;
	qry.QueryType = PropertyStandardQuery;

	if ( bResult = DeviceIoControl( 
		hDevice, 
		IOCTL_STORAGE_QUERY_PROPERTY,
		&qry,
        sizeof( qry ),
		&szBuffer,
		sizeof( szBuffer ),
		&dwBytesReturnValue, 
		NULL ) )
	{
		pStorageDeviceDescriptor = ( PSTORAGE_DEVICE_DESCRIPTOR ) &szBuffer;
		sHddSerial = ( PCHAR ) pStorageDeviceDescriptor + pStorageDeviceDescriptor->SerialNumberOffset;

		sHddSerial.replace( sHddSerial.find_first_of( " " ), 
		sHddSerial.find_first_not_of( " " ), "" );
	}

	
	if ( hDevice != INVALID_HANDLE_VALUE )
		CloseHandle( hDevice );

	return bResult;
}


int main( )
{
	std::string sHddSerial;
	BOOL bResult;
	
	bResult = bGetHddSerial( sHddSerial );
	
	if ( bResult ) 
		printf( sHddSerial.c_str( ) );
	
	else 
		printf ( "Couldn't get the physical hdd serial! Error: %d.\n", GetLastError( ) );
	
	_getch( );

	return 1;
}
