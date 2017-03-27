typedef VOID( __cdecl* _PVFI ) ( INT );
extern "C" const _PVFI _FPinit;
extern "C" VOID __cdecl _initp_misc_cfltcvt_tab( VOID );

VOID ActivateFloatingPointCRT( VOID )
{
    if ( _FPinit != NULL )
    {
        ( *_FPinit ) ( TRUE );
    }

    _initp_misc_cfltcvt_tab();
}