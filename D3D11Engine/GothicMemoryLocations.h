#pragma once

/** Memory location switch */
//#define BUILD_GOTHIC_1_08k
//#define BUILD_GOTHIC_2_6_fix

#define THISPTR_OFFSET(x) (reinterpret_cast<DWORD>(this) + (x))

template<typename TOriginal, typename T>
static void XHook( TOriginal& original, unsigned int adr, T& hookFn ) {
    original = reinterpret_cast<TOriginal>(DetourFunction( reinterpret_cast<BYTE*>(adr), reinterpret_cast<BYTE*>(hookFn) ));
}

template<typename T>
static void XHook( unsigned int adr, T& hookFn ) {
    DetourFunction( reinterpret_cast<BYTE*>(adr), reinterpret_cast<BYTE*>(hookFn) );
}

template<typename T, size_t n >
static void PatchAddr( unsigned int adr, const T( &v )[n] ) {
    DWORD dwProtect;
    size_t len = n - 1;
    if ( VirtualProtect( reinterpret_cast<void*>(adr), len, PAGE_EXECUTE_READWRITE, &dwProtect ) ) {
        memcpy( reinterpret_cast<void*>(adr), v, len );
        VirtualProtect( reinterpret_cast<void*>(adr), len, dwProtect, &dwProtect );
        FlushInstructionCache( GetCurrentProcess(), reinterpret_cast<void*>(adr), len );
    }
}

#define INST_NOP 0x90
#define REPLACE_OP(addr, op) {unsigned char* a = (unsigned char*)addr; *a = op;}
#define REPLACE_RANGE(start, end_incl, op) {for(int i=start; i<=end_incl;i++){REPLACE_OP(i, op);}}

#ifdef BUILD_GOTHIC_1_08k
#ifdef BUILD_1_12F
#include "GothicMemoryLocations1_12f.h"
#else
#include "GothicMemoryLocations1_08k.h"
#endif
#endif

#ifdef BUILD_GOTHIC_2_6_fix
#ifdef BUILD_SPACER
#include "GothicMemoryLocations2_6_fix_Spacer.h"
#else
#include "GothicMemoryLocations2_6_fix.h"
#endif
#endif
