ULONG BaseOfCodeSection ( HMODULE hModule )
{
	PIMAGE_DOS_HEADER pDosHeader = PIMAGE_DOS_HEADER ( hModule );

	if ( pDosHeader->e_magic != IMAGE_DOS_SIGNATURE )
		return 0x0;

	PIMAGE_NT_HEADERS pNTHeader = PIMAGE_NT_HEADERS ( ( ULONG ) hModule + pDosHeader->e_lfanew );

	if ( pNTHeader->Signature != IMAGE_NT_SIGNATURE )
		return 0x0;

	return pNTHeader->OptionalHeader.BaseOfCode;
}

ULONG SizeOfCodeSection ( HMODULE hModule )
{
	PIMAGE_DOS_HEADER pDosHeader = PIMAGE_DOS_HEADER ( hModule );

	if ( pDosHeader->e_magic != IMAGE_DOS_SIGNATURE )
		return 0x0;

	PIMAGE_NT_HEADERS pNTHeader = PIMAGE_NT_HEADERS ( ( ULONG ) hModule + pDosHeader->e_lfanew );

	if ( pNTHeader->Signature != IMAGE_NT_SIGNATURE )
		return 0x0;

	return pNTHeader->OptionalHeader.SizeOfCode;
}

ULONG PatternSearch ( std::string sModuleName, PBYTE pbPattern, std::string sMask )
{
	BOOL bPatternDidMatch = FALSE;
	HMODULE hModule = GetModuleHandle ( sModuleName.c_str( ) );

	if ( !hModule )
		return 0x0;

	ULONG uCodeBase = ( ULONG ) hModule + BaseOfCodeSection ( hModule );
	ULONG uSizeOfCode = SizeOfCodeSection ( hModule );
	ULONG uArraySize = sMask.length( );

	if ( !uCodeBase || !uSizeOfCode || !uArraySize )
		return 0x0;

	for ( size_t i = uCodeBase; i <= uCodeBase + uSizeOfCode; i++ )
	{
		for ( size_t t = 0; t < uArraySize; t++ )
		{
			if ( *( ( PBYTE ) i + t ) == pbPattern [ t ] || sMask.c_str( ) [ t ] == '?' )
				bPatternDidMatch = TRUE;

			else
			{
				bPatternDidMatch = FALSE;
				break;
			}

		}

		if ( bPatternDidMatch )
			return i;
	}

	return 0x0;
}
