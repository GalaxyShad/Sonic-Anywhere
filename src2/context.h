#ifndef __CLIONPROJECTS_SONIC_ANYWHERE_SRC2_CONTEXT_H_
#define __CLIONPROJECTS_SONIC_ANYWHERE_SRC2_CONTEXT_H_

#include "variables.h"

typedef struct S1Context {
    S1Variables*         var;
    S1VariablePointers*  varptr;

    struct {
        u32 d0, d1, d2, d3, d4, d5, d6, d7;
        u8 *a0, *a1, *a2, *a3, *a4, *a5, *a6, *a7;
    } reg;

    u8* vramAddr;
    
    u8* vram_fg_;
    u8* vram_bg_;
    u8* vram_sprites_;
    u8* vram_hscroll_;
} S1Context;

#endif // __CLIONPROJECTS_SONIC_ANYWHERE_SRC2_CONTEXT_H_