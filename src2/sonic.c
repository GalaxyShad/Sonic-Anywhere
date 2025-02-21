#include "context.h"
#include "include_backend/debug.h"
#include "src/resources/resourceid.h"
#include "src/resources/resourcestore.h"

static void writeCRAM(void* SRC, void* SRC_END, size DST) {
    NOT_IMPLEMENTED
}
static void writeVRAM(void* SRC, void* SRC_END, size DST) {
    NOT_IMPLEMENTED
}

static void copyTilemap(void* src, size dst, size width, size height) { NOT_IMPLEMENTED }

static void fillVRAM(u8 byte, void* start, void* end) { NOT_IMPLEMENTED }

static void clearRAM(void* start, void* end) { NOT_IMPLEMENTED }

static void locVRAM(size loc, size controlport) { NOT_IMPLEMENTED }

#define make_art_tile(ADDR, PAL, PRI) (((PRI&1)<<15)|((PAL&3)<<13)|ADDR)

#define VDP_CONTROL_PORT_READ() 0
#define VDP_CONTROL_PORT_WRITE(X)
#define VDP_DATA_PORT_WRITE(X)

#define PORT_WRITE(ADR, X)
#define PORT_READ(ADR) 0

#define startZ80
#define stopZ80
#define waitZ80

#define enable_ints
#define disable_ints

static void NemDec(S1Context* ctx) { NOT_IMPLEMENTED }
static void EniDec(S1Context* ctx) { NOT_IMPLEMENTED }

static void WhiteIn_DecColour(S1Context* ctx);
static void WhiteIn_FromWhite(S1Context* ctx);
static void PlaySound_Special(S1Context* ctx) { NOT_IMPLEMENTED }
static void WhiteOut_ToWhite(S1Context* ctx);
static void FadeOut_DecColour(S1Context* ctx);
static void FadeOut_ToBlack(S1Context* ctx);
static void FadeIn_AddColour(S1Context* ctx);
static void FadeIn_FromBlack(S1Context* ctx);
static void WaitForVBla(S1Context* ctx);
static void Demo_Time(S1Context* ctx);
static void ReadJoypads(S1Context* ctx);
static void VDPSetupGame(S1Context* ctx);
static void DACDriverLoad(S1Context* ctx);
static void JoypadInit(S1Context* ctx);

static void sub_106E(S1Context* ctx);
static void sub_1642(S1Context* ctx);

static void LoadTilesAsYouMove_BGOnly(S1Context* ctx) {
    NOT_IMPLEMENTED
}

static void GM_Sega(S1Context* ctx);
static void GM_Title(S1Context* ctx) { NOT_IMPLEMENTED }
static void GM_Level(S1Context* ctx) { NOT_IMPLEMENTED }
static void GM_Special(S1Context* ctx) { NOT_IMPLEMENTED }
static void GM_Cont(S1Context* ctx) { NOT_IMPLEMENTED }
static void GM_Ending(S1Context* ctx) { NOT_IMPLEMENTED }
static void GM_Credits(S1Context* ctx) { NOT_IMPLEMENTED }

typedef void (*RoutinePtr)(S1Context*);

static RoutinePtr GameModeArray[] = {
  GM_Sega,    // Sega Screen ($00)
  GM_Title,   // Title	Screen ($04)
  GM_Level,   // Demo Mode ($08)
  GM_Level,   // Normal Level ($0C)
  GM_Special, // Special Stage	($10)
  GM_Cont,    // Continue Screen ($14)
  GM_Ending,  // End of game sequence ($18)
  GM_Credits, // Credits ($1C)
};

void GameInit(S1Context* ctx) {
    u8* a6 = ctx->varptr->v_ram_start;
    u16 d7 = 0;

    // .clearRAM:
    for (u8* i = ctx->varptr->v_ram_start; i != ctx->varptr->v_ram_end; i++) {
        *i = 0;
    }

    VDPSetupGame(ctx);
    DACDriverLoad(ctx);
    JoypadInit(ctx);

    ctx->var->v_gamemode = id_Sega;

    // MainGameLoop
    while (1) {                      // loop indefinitely
        u8 d0 = ctx->var->v_gamemode; // load Game Mode
        d0 &= 0x1C; // limit Game Mode value to $1C max (change to a maximum of 7C to add more game modes)
        GameModeArray[d0 / 4](ctx); // jump to apt location in ROM
    }
}

// ===========================================================================
// ---------------------------------------------------------------------------
// Vertical interrupt
// ---------------------------------------------------------------------------

static void UpdateMusic(S1Context* ctx) {
    NOT_IMPLEMENTED
}

static void VBla_Music(S1Context* ctx);
static void VBla_Exit(S1Context* ctx);

typedef enum VBlaRoutineReturnType { VBLA_RET_TO_MUSIC, VBLA_RET_TO_EXIT } VBlaRoutineReturnType;

static void VBla_00(S1Context* ctx);
static void VBla_02(S1Context* ctx);
static void VBla_04(S1Context* ctx);
static void VBla_06(S1Context* ctx);
static void VBla_08(S1Context* ctx);
static void VBla_0A(S1Context* ctx);
static void VBla_0C(S1Context* ctx);
static void VBla_0E(S1Context* ctx);
static void VBla_10(S1Context* ctx);
static void VBla_12(S1Context* ctx);
static void VBla_14(S1Context* ctx);
static void VBla_16(S1Context* ctx);

typedef VBlaRoutineReturnType (*VblaRoutinePtr)(S1Context*);

static RoutinePtr VBla_Index[] = {
  VBla_00, VBla_02, VBla_04, VBla_06, VBla_08, VBla_0A, VBla_0C, VBla_0E, VBla_10, VBla_12, VBla_14, VBla_16, VBla_0C
};

void VBlank(S1Context* ctx) {
    if (ctx->var->v_vbla_routine == 0) {
        VBla_00(ctx);
        VBla_Music(ctx);
        VBla_Exit(ctx);
        return;
    }

    u16 d0 = VDP_CONTROL_PORT_READ();
    VDP_CONTROL_PORT_WRITE(0x40000010);
    VDP_DATA_PORT_WRITE(ctx->var->v_scrposy_vdp); // send screen y-axis pos. to VSRAM

    if (ctx->var->v_megadrive == 6) { // is Megadrive PAL?
        // .waitPAL:
        for (d0 = 0x700; d0 != 0; d0--)
            ; // wait here in a loop doing nothing for a while...
    }

    // .notPAL:
    d0 = ctx->var->v_vbla_routine;

    ctx->var->v_vbla_routine = 0;
    ctx->var->f_hbla_pal = 1;

    d0 &= 0x3E;

    VBla_Index[d0 / 2](ctx);

    if (d0 != 0x08) VBla_Music(ctx);
    VBla_Exit(ctx);
}

static void VBla_Music(S1Context* ctx) {
    UpdateMusic(ctx);
}

static void VBla_Exit(S1Context* ctx) {
    ctx->var->v_vbla_count++;
}

static void VBla_00(S1Context* ctx) {
    if (!(0x80 + id_Level == ctx->var->v_gamemode)) {
        // is game on a level?
        if (id_Level != ctx->var->v_gamemode) {
            return;
        }
    }

    // islevel
    if (ctx->var->v_zone != id_LZ) { // is level LZ ?
        return;   // if not, branch
    }

    u16 d0 = VDP_CONTROL_PORT_READ();
    if (ctx->var->v_megadrive != 6) {
        // .waitPAL:
        for (d0 = 0x700; d0 != 0; d0--)
            ; // wait here in a loop doing nothing for a while...
    }

    // notPAL:
    ctx->var->f_hbla_pal = 1; // set HBlank flag

    stopZ80;
    waitZ80;

    // is water above top of screen?
    if (ctx->var->f_wtr_state) {
        writeCRAM(ctx->var->v_palette, ctx->varptr->v_palette_end, 0);
    } else {
        writeCRAM(ctx->var->v_palette_water, ctx->varptr->v_palette_water_end, 0);
    }

    // TODO move.w	(v_hbla_hreg).w,(a5)

    startZ80;
}

// ===========================================================================

static void VBla_02(S1Context* ctx) {
    sub_106E(ctx);

    VBla_14(ctx);
}

static void VBla_14(S1Context* ctx) {
    if (ctx->var->v_demolength == 0) {
        return;
    }

    ctx->var->v_demolength--;

    return;
}

// ===========================================================================

static void VBla_04(S1Context* ctx) {
    sub_106E(ctx);
    LoadTilesAsYouMove_BGOnly(ctx);
    sub_1642(ctx);

    if (ctx->var->v_demolength != 0) {
        ctx->var->v_demolength--;
    }
}

// ===========================================================================

static void VBla_06(S1Context* ctx) {
    sub_106E(ctx);
}

// ===========================================================================

static void VBla_10(S1Context* ctx) {
    if (id_Special == ctx->var->v_gamemode) {
        return VBla_0A(ctx);
    }

    return VBla_08(ctx);
}

static void VBla_08(S1Context* ctx) {
    stopZ80;
    waitZ80;

    ReadJoypads(ctx);
    if (ctx->var->f_wtr_state == 0) {
        writeCRAM(ctx->var->v_palette, ctx->varptr->v_palette_end, 0);
    } else {
        writeCRAM(ctx->var->v_palette_water, ctx->varptr->v_palette_water_end, 0);
    }

    // TODO move.w	(v_hbla_hreg).w,(a5)

    writeVRAM(ctx->var->v_hscrolltablebuffer, ctx->varptr->v_hscrolltablebuffer_end, vram_hscroll);
    writeVRAM(ctx->var->v_spritetablebuffer, ctx->varptr->v_spritetablebuffer_end, vram_sprites);

    if (ctx->var->f_sonframechg) {
        writeVRAM(ctx->var->v_sgfx_buffer, ctx->varptr->v_sgfx_buffer_end, ArtTile_Sonic * tile_size);
        ctx->var->f_sonframechg = 0;
    }

    startZ80;

    ctx->var->v_screenposx_dup = ctx->var->v_screenposx;
    ctx->var->v_screenposy_dup = ctx->var->v_screenposy;
    ctx->var->v_bgscreenposx_dup = ctx->var->v_bgscreenposx;
    ctx->var->v_bgscreenposy_dup = ctx->var->v_bgscreenposy;
    ctx->var->v_bg2screenposx_dup = ctx->var->v_bg2screenposx;
    ctx->var->v_bg2screenposy_dup = ctx->var->v_bg2screenposy;
    ctx->var->v_bg3screenposx_dup = ctx->var->v_bg3screenposx;
    ctx->var->v_bg3screenposy_dup = ctx->var->v_bg3screenposy;

    ctx->var->v_fg_scroll_flags_dup = ctx->var->v_fg_scroll_flags;
    ctx->var->v_bg1_scroll_flags_dup = ctx->var->v_bg1_scroll_flags;

    if (*ctx->varptr->v_hbla_line >= 96) {
        Demo_Time(ctx);
        return;
    }

    ctx->var->f_doupdatesinhblank = 1;
}

// ---------------------------------------------------------------------------
// Subroutine to	run a demo for an amount of time
// ---------------------------------------------------------------------------

// ||||||||||||||| S U B	R O U T	I N E |||||||||||||||||||||||||||||||||||||||

static void LoadTilesAsYouMove(S1Context* ctx) {
    NOT_IMPLEMENTED
}
static void AnimateLevelGfx(S1Context* ctx) {
    NOT_IMPLEMENTED
}
static void HUD_Update(S1Context* ctx) {
    NOT_IMPLEMENTED
}
static void ProcessDPLC2(S1Context* ctx);

static void Demo_Time(S1Context* ctx) {
    LoadTilesAsYouMove(ctx);
    AnimateLevelGfx(ctx);
    HUD_Update(ctx);
    ProcessDPLC2(ctx);

    // is there time left on the demo?
    if (ctx->var->v_demolength != 0) {
        ctx->var->v_demolength--;
    }
}

static void PalCycle_SS(S1Context* ctx) {
    NOT_IMPLEMENTED
}

static void VBla_0A(S1Context* ctx) {
    stopZ80;
    waitZ80;

    ReadJoypads(ctx);

    writeCRAM(ctx->var->v_palette, ctx->varptr->v_palette_end, 0);
    writeVRAM(ctx->var->v_spritetablebuffer, ctx->varptr->v_spritetablebuffer_end, vram_sprites);
    writeVRAM(ctx->var->v_hscrolltablebuffer, ctx->varptr->v_hscrolltablebuffer_end, vram_hscroll);

    startZ80;

    PalCycle_SS(ctx);
    if (ctx->var->f_sonframechg) {
        writeVRAM(ctx->var->v_sgfx_buffer, ctx->varptr->v_sgfx_buffer_end, ArtTile_Sonic * tile_size);
        ctx->var->f_sonframechg = 0;
    }

    if (ctx->var->v_demolength != 0) {
        ctx->var->v_demolength--;
    }
}

// ===========================================================================

static void VBla_0C(S1Context* ctx) {
    stopZ80;
    waitZ80;

    ReadJoypads(ctx);
    if (ctx->var->f_wtr_state == 0) {
        writeCRAM(ctx->var->v_palette, ctx->varptr->v_palette_end, 0);
    } else {
        writeCRAM(ctx->var->v_palette_water, ctx->varptr->v_palette_water_end, 0);
    }

    // TODO move.w	(v_hbla_hreg).w,(a5)

    writeVRAM(ctx->var->v_hscrolltablebuffer, ctx->varptr->v_hscrolltablebuffer_end, vram_hscroll);
    writeVRAM(ctx->var->v_spritetablebuffer, ctx->varptr->v_spritetablebuffer_end, vram_sprites);

    if (ctx->var->f_sonframechg) {
        writeVRAM(ctx->var->v_sgfx_buffer, ctx->varptr->v_sgfx_buffer_end, ArtTile_Sonic * tile_size);
        ctx->var->f_sonframechg = 0;
    }

    startZ80;

    ctx->var->v_screenposx_dup = ctx->var->v_screenposx;
    ctx->var->v_screenposy_dup = ctx->var->v_screenposy;
    ctx->var->v_bgscreenposx_dup = ctx->var->v_bgscreenposx;
    ctx->var->v_bgscreenposy_dup = ctx->var->v_bgscreenposy;
    ctx->var->v_bg2screenposx_dup = ctx->var->v_bg2screenposx;
    ctx->var->v_bg2screenposy_dup = ctx->var->v_bg2screenposy;
    ctx->var->v_bg3screenposx_dup = ctx->var->v_bg3screenposx;
    ctx->var->v_bg3screenposy_dup = ctx->var->v_bg3screenposy;

    ctx->var->v_fg_scroll_flags_dup = ctx->var->v_fg_scroll_flags;
    ctx->var->v_bg1_scroll_flags_dup = ctx->var->v_bg1_scroll_flags;

    LoadTilesAsYouMove(ctx);
    AnimateLevelGfx(ctx);
    HUD_Update(ctx);

    sub_1642(ctx);
}

// ===========================================================================

static void VBla_0E(S1Context* ctx) {
    sub_106E(ctx);
    ctx->var->v_vbla_0e_counter++;
    ctx->var->v_vbla_routine = 0xE;
}

// ===========================================================================

static void VBla_12(S1Context* ctx) {
    sub_106E(ctx);

    // TODO move.w	(v_hbla_hreg).w,(a5)

    sub_1642(ctx);
}

// ===========================================================================

static void VBla_16(S1Context* ctx) {
    stopZ80;
    waitZ80;

    ReadJoypads(ctx);
    writeCRAM(ctx->var->v_palette, ctx->varptr->v_palette_end, 0);
    writeVRAM(ctx->var->v_spritetablebuffer, ctx->varptr->v_spritetablebuffer_end, vram_sprites);
    writeVRAM(ctx->var->v_hscrolltablebuffer, ctx->varptr->v_hscrolltablebuffer_end, vram_hscroll);

    startZ80;
    if (ctx->var->f_sonframechg) {
        writeVRAM(ctx->var->v_sgfx_buffer, ctx->varptr->v_sgfx_buffer_end, ArtTile_Sonic * tile_size);
        ctx->var->f_sonframechg = 0;
    }

    if (ctx->var->v_demolength != 0) {
        ctx->var->v_demolength--;
    }
}

// ||||||||||||||| S U B	R O U T	I N E |||||||||||||||||||||||||||||||||||||||

static void sub_106E(S1Context* ctx) {
    stopZ80;
    waitZ80;

    ReadJoypads(ctx);

    if (ctx->var->f_wtr_state == 0) {
        writeCRAM(ctx->var->v_palette, ctx->varptr->v_palette_end, 0);
    } else {
        writeCRAM(ctx->var->v_palette_water, ctx->varptr->v_palette_water_end, 0);
    }

    startZ80;
}

// ---------------------------------------------------------------------------
// Horizontal interrupt
// ---------------------------------------------------------------------------

static void HBlank(S1Context* ctx) {
    NOT_IMPLEMENTED
}

// ===========================================================================

static void loc_119E(S1Context* ctx) {
    NOT_IMPLEMENTED
}

// ---------------------------------------------------------------------------
// Subroutine to	initialise joypads
// ---------------------------------------------------------------------------

static void JoypadInit(S1Context* ctx) {
    NOT_IMPLEMENTED
}

// ---------------------------------------------------------------------------
// Subroutine to	read joypad input, and send it to the RAM
// ---------------------------------------------------------------------------
// ||||||||||||||| S U B	R O U T	I N E |||||||||||||||||||||||||||||||||||||||

static void ReadJoypads(S1Context* ctx) {
    NOT_IMPLEMENTED
}

// ||||||||||||||| S U B	R O U T	I N E |||||||||||||||||||||||||||||||||||||||


static void VDPSetupGame(S1Context* ctx) {
    static u16 VDPSetupArray[] = {
        0x8004,                        // 8-colour mode
        0x8134,                        // enable V.interrupts, enable DMA
        0x8200 + (vram_fg >> 10),      // set foreground nametable address
        0x8300 + (0xA000 >> 10),       // set window nametable address
        0x8400 + (vram_bg >> 13),      // set background nametable address
        0x8500 + (vram_sprites >> 9),  // set sprite table address
        0x8600,                        // unused
        0x8700,                        // set background colour (palette entry 0)
        0x8800,                        // unused
        0x8900,                        // unused
        0x8A00,                        // default H.interrupt register
        0x8B00,                        // full-screen vertical scrolling
        0x8C81,                        // 40-cell display mode
        0x8D00 + (vram_hscroll >> 10), // set background hscroll address
        0x8E00,                        // unused
        0x8F02,                        // set VDP increment size
        0x9001,                        // 64-cell hscroll size
        0x9100,                        // window horizontal position
        0x9200,                        // window vertical position
    };

    // set the VDP registers
    for (int i = 0; i < sizeof(VDPSetupArray) / sizeof(VDPSetupArray[0]); i++) {
        VDP_CONTROL_PORT_WRITE(VDPSetupArray[i]);
    }

    ctx->var->v_vdp_buffer1 = VDPSetupArray[1];
    ctx->var->v_hbla_hreg = 0x8A00+223;
    
    // clear	the CRAM
    VDP_CONTROL_PORT_WRITE(0xC0000000);
    for (int i = 0x3F; i != 0; i--) {
        VDP_DATA_PORT_WRITE(0);
    }

    ctx->var->v_scrposy_vdp = 0;
    ctx->var->v_scrposx_vdp = 0;

    fillVRAM(0, ctx->vramAddr, ctx->vramAddr + 0x10000);
}

// ---------------------------------------------------------------------------
// Subroutine to	clear the screen
// ---------------------------------------------------------------------------

// ||||||||||||||| S U B	R O U T	I N E |||||||||||||||||||||||||||||||||||||||

static void ClearScreen(S1Context* ctx) {
    fillVRAM(0, ctx->vram_fg_, ctx->vram_fg_ + plane_size_64x32);
    fillVRAM(0, ctx->vram_bg_, ctx->vram_bg_ + plane_size_64x32);

    ctx->var->v_screenposy = 0;
    ctx->var->v_screenposx = 0;

    clearRAM(ctx->var->v_spritetablebuffer, ctx->varptr->v_spritetablebuffer_end);
    clearRAM(ctx->var->v_hscrolltablebuffer, ctx->varptr->v_hscrolltablebuffer_end);
}

// ---------------------------------------------------------------------------
// Subroutine to load the DAC driver
// ---------------------------------------------------------------------------

// ||||||||||||||| S U B	R O U T	I N E |||||||||||||||||||||||||||||||||||||||

// SoundDriverLoad:
static void DACDriverLoad(S1Context* ctx) {
    NOT_IMPLEMENTED
}


// include	"_incObj/sub PlaySound.asm"
// include	"_inc/PauseGame.asm"

// ---------------------------------------------------------------------------
// Subroutine to	copy a tile map from RAM to VRAM namespace

// input:
//	a1 = tile map address
//	d0 = VRAM address
//	d1 = width (cells)
//	d2 = height (cells)
// ---------------------------------------------------------------------------

// ||||||||||||||| S U B	R O U T	I N E |||||||||||||||||||||||||||||||||||||||

static void TilemapToVRAM(S1Context* ctx, u8* a1, u32 d0, u32 d1, u32 d2) {
    u32 a6 = vdp_data_port;
    u32 d4 = 0x800000;

    // Tilemap_Line:
    do {
        PORT_WRITE(a6 + 4, d0);
        u32 d3 = d1;

        // Tilemap_Cell:
        do {
            (*a1) = PORT_READ(a6);
            a1++;
            d3--;
        } while(d3 != 0);

        d0 += d4;
        d2--;
    } while(d2 != 0);
}

// ---------------------------------------------------------------------------
// Subroutine to load pattern load cues (aka to queue pattern load requests)
// ---------------------------------------------------------------------------

// ARGUMENTS
// d0 = index of PLC list
// ---------------------------------------------------------------------------

// ||||||||||||||| S U B	R O U T	I N E |||||||||||||||||||||||||||||||||||||||

// LoadPLC:
static void AddPLC(S1Context* ctx, u32 d0) { NOT_IMPLEMENTED }

// ||||||||||||||| S U B	R O U T	I N E |||||||||||||||||||||||||||||||||||||||
// Queue pattern load requests, but clear the PLQ first

// ARGUMENTS
// d0 = index of PLC list (see ArtLoadCues)

// NOTICE: This subroutine does not check for buffer overruns. The programmer
//	  (or hacker) is responsible for making sure that no more than
//	  16 load requests are copied into the buffer.
//	  _________DO NOT PUT MORE THAN 16 LOAD REQUESTS IN A LIST!__________
//         (or if you change the size of Plc_Buffer, the limit becomes (Plc_Buffer_Only_End-Plc_Buffer)/6)

// LoadPLC2:
static void NewPLC(S1Context* ctx, u32 d0) { NOT_IMPLEMENTED }

// ||||||||||||||| S U B	R O U T	I N E |||||||||||||||||||||||||||||||||||||||

// ---------------------------------------------------------------------------
// Subroutine to	clear the pattern load cues
// ---------------------------------------------------------------------------

// Clear the pattern load queue ($FFF680 - $FFF700)
static void ClearPLC(S1Context* ctx) { NOT_IMPLEMENTED }

// ---------------------------------------------------------------------------
// Subroutine to	use graphics listed in a pattern load cue
// ---------------------------------------------------------------------------

// ||||||||||||||| S U B	R O U T	I N E |||||||||||||||||||||||||||||||||||||||
static void RunPLC(S1Context* ctx) { NOT_IMPLEMENTED }

// ||||||||||||||| S U B	R O U T	I N E |||||||||||||||||||||||||||||||||||||||


static void sub_1642(S1Context* ctx) { NOT_IMPLEMENTED }
// End of function sub_1642


// ||||||||||||||| S U B	R O U T	I N E |||||||||||||||||||||||||||||||||||||||

static void ProcessDPLC2(S1Context* ctx) { NOT_IMPLEMENTED }

// ---------------------------------------------------------------------------
// Subroutine to	execute	the pattern load cue
// ---------------------------------------------------------------------------

// ||||||||||||||| S U B	R O U T	I N E |||||||||||||||||||||||||||||||||||||||

static void QuickPLC(S1Context* ctx) { NOT_IMPLEMENTED }

// ---------------------------------------------------------------------------
// Subroutine to	fade in from black
// ---------------------------------------------------------------------------

// ||||||||||||||| S U B	R O U T	I N E |||||||||||||||||||||||||||||||||||||||

static void PaletteFadeIn(S1Context* ctx) {
    ctx->var->v_pfade_start = 0x003F;

    u32 d0 = 0;
    u8* a0 = ctx->var->v_palette;
    d0 = ctx->var->v_pfade_start;
    a0 += d0;
    u16 d1 = cBlack;
    ctx->var->v_pfade_size = d0;

    // fill
    do {
        *((u16*)a0) = d1;
        a0++;
        d0--;
    } while (d0 != 0);

    u32 d4 = 0x15;

    // .mainloop:
    do {
        ctx->var->v_vbla_routine = 0x12;
        
        WaitForVBla(ctx);
        FadeIn_FromBlack(ctx);
        RunPLC(ctx);

        d4--;
    } while (d4 != 0);
}

// ||||||||||||||| S U B	R O U T	I N E |||||||||||||||||||||||||||||||||||||||
static void FadeIn_FromBlack(S1Context* ctx) {
    ctx->reg.d0 = 0;
    ctx->reg.a0 = ctx->var->v_palette;
    ctx->reg.a1 = ctx->var->v_palette_fading;
    ctx->reg.d0 = ctx->var->v_pfade_start;
    ctx->reg.a0 += ctx->reg.d0;
    ctx->reg.a1 += ctx->reg.d0;

    ctx->reg.d0 = ctx->var->v_pfade_size;

    // .addcolour:
    do {
        FadeIn_AddColour(ctx); // increase colour
        ctx->reg.d0--; 
    } while (ctx->reg.d0 != 0); // repeat for size of palette

    // is level Labyrinth?
    if (id_LZ == ctx->var->v_zone) {
        ctx->reg.d0 = 0;

        ctx->reg.a0 = ctx->var->v_palette_water;
        ctx->reg.a1 = ctx->varptr->v_palette_water_fading;

        ctx->reg.d0 = ctx->var->v_pfade_start;
        ctx->reg.a0 += ctx->reg.d0;
        ctx->reg.a1 += ctx->reg.d0;
        ctx->reg.d0 = ctx->var->v_pfade_start;

        do {
            FadeIn_AddColour(ctx); // increase colour again
            ctx->reg.d0--;
        } while (ctx->reg.d0 != 0); 
    }
}

static void FadeIn_AddColour(S1Context* ctx) {
    // addblue
    ctx->reg.d2 = *(u16*)ctx->reg.a1; ctx->reg.a1 += 2;
    ctx->reg.d3 = *(u16*)ctx->reg.a0;

    // is colour already at threshold level?
    if (ctx->reg.d2 != ctx->reg.d3) {
        ctx->reg.d1 = ctx->reg.d3;
        ctx->reg.d1 += 0x200; // increase blue	value

        // has blue reached threshold level?
        if (ctx->reg.d2 < ctx->reg.d1) {
            *(u16*)ctx->reg.a0 = ctx->reg.d1; // update palette
            ctx->reg.a0 += 2;
            return;
        }

        // addgreen:
        ctx->reg.d1 = ctx->reg.d3;
        ctx->reg.d1 += 0x20; // increase green value

        if (ctx->reg.d2 < ctx->reg.d1) {
            *(u16*)ctx->reg.a0 = ctx->reg.d1; // update palette
            ctx->reg.a0 += 2;
            return;
        }

        // .addred:
        *(u16*)ctx->reg.a0 += 1;
        ctx->reg.a0 += 2;

        return;
    }

    ctx->reg.a0 += 2;
}


// ---------------------------------------------------------------------------
// Subroutine to fade out to black
// ---------------------------------------------------------------------------


// ||||||||||||||| S U B	R O U T	I N E |||||||||||||||||||||||||||||||||||||||
static void PaletteFadeOut(S1Context* ctx) {
    ctx->var->v_pfade_start = 0x003F;
    ctx->reg.d4 = 0x15;

    // mainloop
    do {
        ctx->var->v_vbla_routine = 0x12;
        WaitForVBla(ctx);
        FadeOut_ToBlack(ctx);
        RunPLC(ctx);
        ctx->reg.d4--;
    } while(ctx->reg.d4 != 0);
}

// ||||||||||||||| S U B	R O U T	I N E |||||||||||||||||||||||||||||||||||||||

static void FadeOut_ToBlack(S1Context* ctx) {
    ctx->reg.d0 = 0;
    ctx->reg.a0 = ctx->var->v_palette;
    ctx->reg.d0 = ctx->var->v_pfade_start;
    ctx->reg.a0 += ctx->reg.d0;
    ctx->reg.d0 = ctx->var->v_pfade_size;

    // decolour
    do {
        FadeOut_DecColour(ctx);
        if (ctx->reg.d0 != 0) ctx->reg.d0--;
    } while (ctx->reg.d0 != 0);

    ctx->reg.d0 = 0;
    ctx->reg.a0 = ctx->var->v_palette_water;
    ctx->reg.d0 = ctx->var->v_pfade_start;
    ctx->reg.a0 += ctx->reg.d0;
    ctx->reg.d0 = ctx->var->v_pfade_size;

    // decollour2
    do {
        FadeOut_DecColour(ctx);
        if (ctx->reg.d0 != 0) ctx->reg.d0--;
    } while (ctx->reg.d0 != 0);
}

// ||||||||||||||| S U B	R O U T	I N E |||||||||||||||||||||||||||||||||||||||

static void FadeOut_DecColour(S1Context* ctx) {
    // dered
    ctx->reg.d2 = *(u16*)ctx->reg.a0;
    if (ctx->reg.d2 != 0) {
        ctx->reg.d1 = ctx->reg.d2;
        ctx->reg.d1 &= 0xE;
        if (ctx->reg.d1 != 0) {
            *(u16*)ctx->reg.a0 -= 2;    // decrease red value
            ctx->reg.a0 += 2;
            return;
        }

        // ===========================================================================

        //degreen
        ctx->reg.d1 = ctx->reg.d2;
        ctx->reg.d1 &= 0xE0;
        if (ctx->reg.d1 != 0) {
             *(u16*)ctx->reg.a0 -= 0x20;    // decrease green value
            ctx->reg.a0 += 2;
        }

        // ===========================================================================

        //deblue
        ctx->reg.d1 = ctx->reg.d2;
        ctx->reg.d1 &= 0xE00;
        if (ctx->reg.d1 != 0) {
            *(u16*)ctx->reg.a0 -= 0x200;
            ctx->reg.a0 += 2;
        }

        // ===========================================================================
    }

    ctx->reg.a0 += 2;
}

// ---------------------------------------------------------------------------
// Subroutine to	fade in from white (Special Stage)
// ---------------------------------------------------------------------------

// ||||||||||||||| S U B	R O U T	I N E |||||||||||||||||||||||||||||||||||||||

static void PaletteWhiteIn(S1Context* ctx) {
    ctx->var->v_pfade_start = 0x003F;
    ctx->reg.d0 = 0;
    ctx->reg.a0 = ctx->var->v_palette;
    ctx->reg.d0 = ctx->var->v_pfade_start;
    ctx->reg.a0 += ctx->reg.d0;
    ctx->reg.d1 = cWhite;
    ctx->reg.d0 = ctx->var->v_pfade_size;

    //fill
    do {
        *(u16*)ctx->reg.a0 = ctx->reg.d1;
        ctx->reg.a0 += 2;
        ctx->reg.d0--;
    } while (ctx->reg.d0 != 0);

    ctx->reg.d4 = 0x15;

    // mainloop
    do  {
        ctx->var->v_vbla_routine = 0x12;
        WaitForVBla(ctx);
        WhiteIn_FromWhite(ctx);
        RunPLC(ctx);
    } while (ctx->reg.d4 != 0);
}

// ||||||||||||||| S U B	R O U T	I N E |||||||||||||||||||||||||||||||||||||||

static void WhiteIn_FromWhite(S1Context* ctx) {
    ctx->reg.d0 = 0;
    ctx->reg.a0 = ctx->var->v_palette;
    ctx->reg.a1 = ctx->var->v_palette_fading;
    ctx->reg.d0 = ctx->var->v_pfade_start;
    ctx->reg.a0 += ctx->reg.d0;
    ctx->reg.a1 += ctx->reg.d0;
    ctx->reg.d0 = ctx->var->v_pfade_size;

    // decolour
    do {
        WhiteIn_DecColour(ctx);
        ctx->reg.d0--;
    } while (ctx->reg.d0);

    if (ctx->var->v_zone == id_LZ) {
        ctx->reg.d0 = 0;
        ctx->reg.a0 = ctx->var->v_palette_water;
        ctx->reg.a1 = ctx->varptr->v_palette_water_fading;
        ctx->reg.d0 = ctx->var->v_pfade_start;
        ctx->reg.a0 += ctx->reg.d0;
        ctx->reg.a1 += ctx->reg.d0;
        ctx->reg.d0 = ctx->var->v_pfade_size;

        // decolour2
        do {
            WhiteIn_DecColour(ctx);
            ctx->reg.d0--;
        } while (ctx->reg.d0);
    }
}

static void WhiteIn_DecColour(S1Context* ctx) {
    // deblue
    ctx->reg.d2 = *ctx->reg.a1; ctx->reg.a1++;
    ctx->reg.d3 = ctx->reg.d2;
    if (ctx->reg.d3 != 0) {
        ctx->reg.d1 = ctx->reg.d3;
        ctx->reg.d1 -= 0x200;
        if (ctx->reg.d1 > 0 && ctx->reg.d1 > ctx->reg.d2) {
            *(u16*)ctx->reg.a0 = ctx->reg.d1;
            ctx->reg.a0 += 2;
            return;
        }

        // ===========================================================================
        
        // degreen
        ctx->reg.d1 = ctx->reg.d3;
        ctx->reg.d1 -= 0x20;
        if (ctx->reg.d1 > 0 && ctx->reg.d1 > ctx->reg.d2) {
            *(u16*)ctx->reg.a0 = ctx->reg.d1;
            ctx->reg.a0 += 2;
            return;
        }

        // ===========================================================================

        // dered
        *(u16*)ctx->reg.a0 -= 2;
        ctx->reg.a0 += 2;

        return;
    }

    ctx->reg.a0 += 2;
}

// ---------------------------------------------------------------------------
// Subroutine to fade to white (Special Stage)
// ---------------------------------------------------------------------------

// ||||||||||||||| S U B	R O U T	I N E |||||||||||||||||||||||||||||||||||||||


static void PaletteWhiteOut(S1Context* ctx) {
    ctx->var->v_pfade_start = 0x003F;
    ctx->reg.d4 = 0x15;

    // mainloop
    do {
        ctx->var->v_vbla_routine = 0x12;
        WaitForVBla(ctx);
        WhiteOut_ToWhite(ctx);
        RunPLC(ctx);
        ctx->reg.d4--;
    } while (ctx->reg.d4 != 0);
}

// ||||||||||||||| S U B	R O U T	I N E |||||||||||||||||||||||||||||||||||||||

static void WhiteOut_ToWhite(S1Context* ctx) {
    NOT_IMPLEMENTED
}

// ||||||||||||||| S U B	R O U T	I N E |||||||||||||||||||||||||||||||||||||||

static void WhiteOut_AddColour(S1Context* ctx) {
    NOT_IMPLEMENTED
}

// ---------------------------------------------------------------------------
// Palette cycling routine - Sega logo
// ---------------------------------------------------------------------------

// ||||||||||||||| S U B	R O U T	I N E |||||||||||||||||||||||||||||||||||||||

static int PalCycle_Sega(S1Context* ctx) {
    const u8* Pal_Sega1 = s_resource(RESOURCE__PALETTE__SEGA1_BIN).arr;
    const u8* Pal_Sega2 = s_resource(RESOURCE__PALETTE__SEGA2_BIN).arr;
    
    if ((ctx->var->v_pcyc_time & 0xFF) != 0) {
        goto loc_206A;
    }

    ctx->reg.a1 = ctx->var->v_palette + 0x20;
    ctx->reg.a0 = (u8*)Pal_Sega1;
    ctx->reg.d1 = 5;
    ctx->reg.d0 = ctx->var->v_pcyc_num;

loc_2020:
    if (ctx->reg.d0 >= 0) {
        goto loc_202A;
    }
    ctx->reg.a0 += 2;
    ctx->reg.d1--;
    ctx->reg.d0 += 2;
    goto loc_2020;

loc_202A:
    ctx->reg.d2 = ctx->reg.d0 & 0x1E;
    if (ctx->reg.d2 != 0) {
        goto loc_2034;
    }
    ctx->reg.d0 += 2;

loc_2034:
    if (ctx->reg.d0 >= 0x60) {
        goto loc_203E;
    }
    *((u16*)(ctx->reg.a1 + ctx->reg.d0)) = *((u16*)ctx->reg.a0);
    ctx->reg.a0 += 2;

loc_203E:
    ctx->reg.d0 += 2;
    ctx->reg.d1--;
    if (ctx->reg.d1 != 0) {
        goto loc_202A;
    }

    ctx->reg.d0 = ctx->var->v_pcyc_num + 2;
    ctx->reg.d2 = ctx->reg.d0 & 0x1E;
    if (ctx->reg.d2 != 0) {
        goto loc_2054;
    }
    ctx->reg.d0 += 2;

loc_2054:
    if (ctx->reg.d0 < 0x64) {
        goto loc_2062;
    }
    ctx->var->v_pcyc_time = 0x401;
    ctx->reg.d0 = -0xC;

loc_2062:
    ctx->var->v_pcyc_num = ctx->reg.d0;
    return 1;

loc_206A:
    ctx->var->v_pcyc_time--;
    if (ctx->var->v_pcyc_time >= 0) {
        goto loc_20BC;
    }
    ctx->var->v_pcyc_time = 4;
    ctx->reg.d0 = ctx->var->v_pcyc_num + 0xC;
    if (ctx->reg.d0 >= 0x30) {
        return 0;
    }

    ctx->var->v_pcyc_num = ctx->reg.d0;
    ctx->reg.a0 = (u8*)Pal_Sega2 + ctx->reg.d0;
    ctx->reg.a1 = ctx->var->v_palette + 0x04;
    *((u32*)ctx->reg.a1) = *((u32*)ctx->reg.a0);
    ctx->reg.a1 += 4;
    ctx->reg.a0 += 4;
    *((u32*)ctx->reg.a1) = *((u32*)ctx->reg.a0);
    ctx->reg.a1 += 4;
    ctx->reg.a0 += 4;
    *((u16*)ctx->reg.a1) = *((u16*)ctx->reg.a0);
    ctx->reg.a1 = ctx->var->v_palette + 0x20;
    ctx->reg.d0 = 0;
    ctx->reg.d1 = 0x2C;

loc_20A8:
    ctx->reg.d2 = ctx->reg.d0 & 0x1E;
    if (ctx->reg.d2 != 0) {
        goto loc_20B2;
    }
    ctx->reg.d0 += 2;

loc_20B2:
    *((u16*)(ctx->reg.a1 + ctx->reg.d0)) = *((u16*)ctx->reg.a0);
    ctx->reg.d0 += 2;
    ctx->reg.d1--;
    if (ctx->reg.d1 != 0) {
        goto loc_20A8;
    }

loc_20BC:
    ctx->reg.d0 = 1;
}

// ---------------------------------------------------------------------------
// Subroutines to load palettes

// input:
//	d0 = index number for palette
// ---------------------------------------------------------------------------

// ||||||||||||||| S U B	R O U T	I N E |||||||||||||||||||||||||||||||||||||||

static void PalLoad_Fade(S1Context* ctx) {
    NOT_IMPLEMENTED
    return;

    // Load palette pointers base address into a1
    // ctx->reg.a1 = (u8*)PalPointers;

    // Calculate offset in the palette pointers table (d0 * 8)
    ctx->reg.d0 <<= 3;
    ctx->reg.a1 += ctx->reg.d0;

    // Get palette data address into a2
    ctx->reg.a2 = (u8*)(*((u32*)ctx->reg.a1));
    ctx->reg.a1 += 4;

    // Get target RAM address into a3
    ctx->reg.a3 = (u8*)(*((u16*)ctx->reg.a1));
    ctx->reg.a1 += 2;

    // Adjust a3 to point to the fading palette area
    ctx->reg.a3 += (ctx->var->v_palette_fading - ctx->var->v_palette);

    // Get length of palette data into d7
    ctx->reg.d7 = *((u16*)ctx->reg.a1);

    // Copy palette data from a2 to a3
    do {
        *((u32*)ctx->reg.a3) = *((u32*)ctx->reg.a2);
        ctx->reg.a2 += 4;
        ctx->reg.a3 += 4;
        ctx->reg.d7--;
    } while (ctx->reg.d7 != 0);
}

// ||||||||||||||| S U B	R O U T	I N E |||||||||||||||||||||||||||||||||||||||

static void PalLoad(S1Context* ctx) {
    NOT_IMPLEMENTED
}

// ---------------------------------------------------------------------------
// Underwater palette loading subroutine
// ---------------------------------------------------------------------------

// ||||||||||||||| S U B	R O U T	I N E |||||||||||||||||||||||||||||||||||||||

static void PalLoad_Fade_Water(S1Context* ctx) { NOT_IMPLEMENTED }

// ||||||||||||||| S U B	R O U T	I N E |||||||||||||||||||||||||||||||||||||||

static void PalLoad_Water(S1Context* ctx) { NOT_IMPLEMENTED }

// ---------------------------------------------------------------------------
// Subroutine to	wait for VBlank routines to complete
// ---------------------------------------------------------------------------

// ||||||||||||||| S U B	R O U T	I N E |||||||||||||||||||||||||||||||||||||||

static void WaitForVBla(S1Context* ctx) {
    enable_ints;

    // has VBlank routine finished?
    while (ctx->var->v_vbla_routine != 0);
}

// ---------------------------------------------------------------------------
// Sega screen
// ---------------------------------------------------------------------------

static void GM_Sega(S1Context* ctx) {
    ctx->reg.d0 = bgm_Stop;
    PlaySound_Special(ctx); // stop music

    ClearPLC(ctx);
    PaletteFadeOut(ctx);

    VDP_CONTROL_PORT_WRITE(0x8004); // use 8-colour mode
    VDP_CONTROL_PORT_WRITE(0x8200+(vram_fg>>10)); // set foreground nametable address
    VDP_CONTROL_PORT_WRITE(0x8400+(vram_bg>>13)); // set background nametable address
    VDP_CONTROL_PORT_WRITE(0x8700); // set background colour (palette entry 0)
    VDP_CONTROL_PORT_WRITE(0x8B00); // full-screen vertical scrolling

    ctx->var->f_wtr_state = 0;

    disable_ints;
    ctx->reg.d0 = ctx->var->v_vdp_buffer1 & 0xBF;
    VDP_CONTROL_PORT_WRITE(ctx->reg.d0);

    ClearScreen(ctx);

    locVRAM(ArtTile_Sega_Tiles*tile_size, vdp_control_port);
    ctx->reg.a0 = s_resource(RESOURCE__ARTNEM__SEGA_LOGO_NEM).arr;
    NemDec(ctx);

    ctx->reg.a1 = ctx->var->v_256x256;
    ctx->reg.a0 = s_resource(RESOURCE__TILEMAPS__SEGA_LOGO_ENI).arr;
    ctx->reg.d0 = make_art_tile(ArtTile_Sega_Tiles, 0, 0);
    EniDec(ctx);

    copyTilemap(ctx->var->v_256x256, vram_bg+0x510, 24, 8);
    copyTilemap(ctx->var->v_256x256+24*8*2, vram_fg, 40, 28);

    // loadPal
    ctx->reg.d0 = palid_SegaBG;
    PalLoad(ctx);
    ctx->var->v_pcyc_num = -0xA;
    ctx->var->v_pcyc_time = 0;
    ctx->var->v_pal_buffer[0x12] = 0;
    ctx->var->v_pal_buffer[0x10] = 0;
    ctx->reg.d0 = ctx->var->v_vdp_buffer1;
    ctx->reg.d0 |= 0x40;
    VDP_CONTROL_PORT_WRITE(ctx->reg.d0);

    // Sega_WaitPal
    do {
        ctx->var->v_vbla_routine = 2;
        WaitForVBla(ctx);
        PalCycle_Sega(ctx);
    } while (ctx->reg.d0 != 0);

    ctx->reg.d0 = sfx_Sega;
    PlaySound_Special(ctx);
    ctx->var->v_vbla_routine = 0x14;
    WaitForVBla(ctx);
    ctx->var->v_demolength = 0x1E;

    // Sega_WaitEnd
    do {
        ctx->var->v_vbla_routine = 2;
        WaitForVBla(ctx);
        if (ctx->var->v_demolength == 0) {
            break;
        }
        
    } while ((ctx->var->v_jpadpress1 & btnStart) == 0);

    // Sega_GotoTitle
    ctx->var->v_gamemode = id_Title;
}