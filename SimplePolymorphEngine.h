#ifndef __SPE__H__
#define __SPE__H__

#include "main.h"

VOID __declspec ( naked ) DummyFunc ( VOID )
{
	// 120 x 0x90
	// 1 x 0xC3
	// 91 BYTES
	__asm
	{
		// 30 BYTES
		nop; 
		nop; 
		nop;
		nop; 
		nop; 
		nop;
		nop; 
		nop; 
		nop;
		nop; 
		nop; 
		nop;
		nop; 
		nop; 
		nop;
		nop; 
		nop; 
		nop;
		nop; 
		nop; 
		nop;
		nop; 
		nop; 
		nop;
		nop; 
		nop; 
		nop;
		nop; 
		nop; 
		nop;

		// 30 BYTES
		nop; 
		nop; 
		nop;
		nop; 
		nop; 
		nop;
		nop; 
		nop; 
		nop;
		nop; 
		nop; 
		nop;
		nop; 
		nop; 
		nop;
		nop; 
		nop; 
		nop;
		nop; 
		nop; 
		nop;
		nop; 
		nop; 
		nop;
		nop; 
		nop; 
		nop;
		nop; 
		nop; 
		nop;

		// 30 BYTES
		nop; 
		nop; 
		nop;
		nop; 
		nop; 
		nop;
		nop; 
		nop; 
		nop;
		nop; 
		nop; 
		nop;
		nop; 
		nop; 
		nop;
		nop; 
		nop; 
		nop;
		nop; 
		nop; 
		nop;
		nop; 
		nop; 
		nop;
		nop; 
		nop; 
		nop;
		nop; 
		nop; 
		nop;

		// 30 BYTES
		nop; 
		nop; 
		nop;
		nop; 
		nop; 
		nop;
		nop; 
		nop; 
		nop;
		nop; 
		nop; 
		nop;
		nop; 
		nop; 
		nop;
		nop; 
		nop; 
		nop;
		nop; 
		nop; 
		nop;
		nop; 
		nop; 
		nop;
		nop; 
		nop; 
		nop;
		nop; 
		nop; 
		nop;

		// 1 BYTE
		RETN;
	}
}

/*
INT ASM_GetRandomNumber ( VOID )
{
	__asm
	{
		RDTSC;
		AND EAX, 0xFF;
	}

}
*/

VOID SPE_FillDummySection ( PBYTE pbDummyFunc )
{
    // MAKE SURE TO ONLY CALL IT ONCE!
	srand ( ( UINT ) GetTickCount( ) + ( UINT ) GetCurrentProcessId( ) );

	/* DYNAMICALLY OBTAIN THE FUNCTIONSIZE ( IN BYTES ) OF THE DUMMYSECTION */
	size_t DummySectionSize = 0;

	for ( PBYTE pbCurrentOpcode = pbDummyFunc; *pbCurrentOpcode != 0xC3; pbCurrentOpcode++ )
		DummySectionSize++;

    CONST BYTE junkBytes[5][4] = 
    { 
        0x40, 0x48, 0x43, 0x4B,
        0x8B, 0xC0, 0x8B, 0xDB,
        0x50, 0x58, 0x53, 0x5B,
        0x09, 0xC9, 0x09, 0xD2,
        0xEB, 0x00, 0x8A, 0xC0 
    };

	DWORD dwOldMask = 0;
/*
	debugLog.AppendLine ( true, "[ SIMPLE POLYMORPH ENGINE ]" );
	debugLog.AppendLine ( true, "Target = 'DUMMYFUNC' 0x%X", ( DWORD ) pbDummyFunc );
	debugLog.AppendLine ( true, "Size = %i bytes", DummySectionSize );
*/
	if ( VirtualProtect ( pbDummyFunc, DummySectionSize, PAGE_EXECUTE_READWRITE, &dwOldMask ) )
	{
		for ( size_t i = 0; i < DummySectionSize; i++ )
		{
            srand( ( DWORD ) GetTickCount( ) );
            for ( INT i = 0; i < ( int ) dllMainSize; i += 4 )
            {
                INT iRandomNumber = rand( ) % 5; // 0 - 4
                for ( int ii = 0; ii < 4; ii++ )
                {
                    pbDummyFunc[ i + ii ] = junkBytes[ iRandomNumber ][ ii ];
                }
            }
		}

		VirtualProtect ( pbDummyFunc, DummySectionSize, dwOldMask, &dwOldMask );
	}

}

/*  POLYMORPH DUMP
	6E34D5A0   40               INC EAX
	6E34D5A1   53               PUSH EBX
	6E34D5A2   8AD2             MOV DL,DL
	6E34D5A4   8B40 C0          MOV EAX,DWORD PTR DS:[EAX-40]
	6E34D5A7   43               INC EBX
	6E34D5A8   48               DEC EAX
	6E34D5A9   8B40 09          MOV EAX,DWORD PTR DS:[EAX+9]
	6E34D5AC   8BD2             MOV EDX,EDX
	6E34D5AE   40               INC EAX
	6E34D5AF   58               POP EAX
	6E34D5B0   098B 5BC05309    OR DWORD PTR DS:[EBX+953C05B],ECX
	6E34D5B6   0058 EB          ADD BYTE PTR DS:[EAX-15],BL
	6E34D5B9   00C0             ADD AL,AL
	6E34D5BB   0943 58          OR DWORD PTR DS:[EBX+58],EAX
	6E34D5BE   C058 58 09       RCR BYTE PTR DS:[EAX+58],9
	6E34D5C2   43               INC EBX
	6E34D5C3   C0DB 50          RCR BL,50                                ; Shift constant out of range 1..31
	6E34D5C6   D243 D2          ROL BYTE PTR DS:[EBX-2E],CL
	6E34D5C9   40               INC EAX
	6E34D5CA   098A 8A4B4B00    OR DWORD PTR DS:[EDX+4B4B8A],ECX
	6E34D5D0   48               DEC EAX
	6E34D5D1   40               INC EAX
	6E34D5D2   0948 48          OR DWORD PTR DS:[EAX+48],ECX
	6E34D5D5   0950 EB          OR DWORD PTR DS:[EAX-15],EDX
	6E34D5D8   DBDB             FCMOVNU ST,ST(3)
	6E34D5DA   58               POP EAX
	6E34D5DB  ^EB C3            JMP SHORT interweb.6E34D5A0
*/

#endif