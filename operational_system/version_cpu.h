#ifdef _MIPS_
// specific overrides for MIPS...
#define _AFX_PACKING    8       // default MIPS alignment (required)
#endif //_MIPS_


#ifdef _ALPHA_
// specific overrides for ALPHA...
#define _AFX_PACKING    8       // default AXP alignment (required)
#ifdef _AFX_NO_DEBUG_CRT
extern "C" void _BPT();
#pragma intrinsic(_BPT)
#define __debug_break() _BPT()
#else
#define __debug_break() _CrtDbgBreak()
#endif
#endif  //_ALPHA_


#ifdef _PPC_
// specific overrides for PPC...
#define _AFX_PACKING    8       // default PPC alignment (required)
#endif //_PPC_


#ifdef _IA64_
// specific overrides for IA64...
#define _AFX_PACKING    8
#define _SHADOW_DOUBLES 8
#endif //_IA64_


#ifdef _AMD64_
// specific overrides for AMD64...
#define _AFX_PACKING    8
#endif //_AMD64_
