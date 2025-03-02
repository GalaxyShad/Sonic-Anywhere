#include "context.h"
#include "include_backend/debug.h"
#include "src/bytearray.h"
#include "src/compressors.h"
#include "src/resources/resourceid.h"
#include "src/resources/resourcestore.h"
#include "object_pointers.h"
#include "pattern_load_cues.h"
#include "palette_pointers.h"

#include "macro.h"
#include "src2/constants.h"

static void NemDec(S1Context* ctx) { 
    compressors__nemesis_decompress(
        ctx->reg.a0, ctx->reg.a0int,

        md_mem__vram()->plane_window_mut->arr  + ctx->vram_copy_to,
        md_mem__vram()->plane_window_mut->size - ctx->vram_copy_to
    );
    md_vdp__set_name_table_location_for_plane(MD_VDP_PLANE__WINDOW, md_mem__vram()->plane_window_mut);
}

static void EniDec(S1Context* ctx) { 
    compressors__enigma_decompress(
        ctx->reg.a0, ctx->reg.a0int,
        ctx->reg.a1, ctx->reg.a1int,
        ctx->reg.d0
    );
}

static void LevSelControls(S1Context* ctx);
static void PaletteCycle(S1Context* ctx) { NOT_IMPLEMENTED }
static void LevSel_ChgLine(S1Context* ctx);
static void LevSel_ChgSnd(S1Context* ctx);
static void LevSelTextLoad(S1Context* ctx);
static void ExecuteObjects(S1Context* ctx) { NOT_IMPLEMENTED }
static void BuildSprites(S1Context* ctx) { NOT_IMPLEMENTED }
static void LevelSizeLoad(S1Context* ctx) { NOT_IMPLEMENTED }
static void DeformLayers(S1Context* ctx) { NOT_IMPLEMENTED }
static void KosDec(S1Context* ctx) { NOT_IMPLEMENTED }
static void LevelLayoutLoad(S1Context* ctx) { NOT_IMPLEMENTED }
static void DrawChunks(S1Context* ctx) { NOT_IMPLEMENTED }
static void PalCycle_Title(S1Context* ctx) { NOT_IMPLEMENTED }

static void WhiteIn_DecColour(S1Context* ctx);
static void WhiteIn_FromWhite(S1Context* ctx);
static void PlaySound_Special(S1Context* ctx) { 
    ON_PLAY_SOUND_SPECIAL(ctx->reg.d0);
}
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
static void GM_Title(S1Context* ctx);
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
        writeCRAM(ctx, ctx->var->v_palette, ctx->varptr->v_palette_end, 0);
    } else {
        writeCRAM(ctx, ctx->var->v_palette_water, ctx->varptr->v_palette_water_end, 0);
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
        writeCRAM(ctx, ctx->var->v_palette, ctx->varptr->v_palette_end, 0);
    } else {
        writeCRAM(ctx, ctx->var->v_palette_water, ctx->varptr->v_palette_water_end, 0);
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

    writeCRAM(ctx, ctx->var->v_palette, ctx->varptr->v_palette_end, 0);
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
        writeCRAM(ctx, ctx->var->v_palette, ctx->varptr->v_palette_end, 0);
    } else {
        writeCRAM(ctx, ctx->var->v_palette_water, ctx->varptr->v_palette_water_end, 0);
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
    writeCRAM(ctx, ctx->var->v_palette, ctx->varptr->v_palette_end, 0);
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
        writeCRAM(ctx, ctx->var->v_palette, ctx->varptr->v_palette_end, 0);
    } else {
        writeCRAM(ctx, ctx->var->v_palette_water, ctx->varptr->v_palette_water_end, 0);
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
static void NewPLC(S1Context* ctx) { NOT_IMPLEMENTED }

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

static void PalCycle_Sega(S1Context* ctx) {
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

    u16 col = *((u16*)ctx->reg.a0);

    *((u16*)(ctx->reg.a1 + ctx->reg.d0)) = *((u16*)ctx->reg.a0);
    ctx->reg.a0 += 2;

loc_203E:
    ctx->reg.d0 += 2;
    if (ctx->reg.d1 != 0) {
        ctx->reg.d1--;
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
    ctx->reg.d0 = 1;
    return;

loc_206A:
    ctx->var->v_pcyc_time -= 0x0100;
    if ((ctx->var->v_pcyc_time) >= 0) {    
        goto loc_20BC;
    }
    ctx->var->v_pcyc_time = 0x0401;
    ctx->reg.d0 = ctx->var->v_pcyc_num + 0xC;
    if (ctx->reg.d0 >= 0x30) {
        ctx->reg.d0 = 0;
        return;
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
    if (ctx->reg.d1 != 0) {
        ctx->reg.d1--;
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

    // ---------------------------------------------------------------------------
    // Palette data
    // ---------------------------------------------------------------------------

    // ReadonlyByteArray Pal_SegaBG = s_resource(RESOURCE__PALETTE__SEGA_BACKGROUND_BIN);
    // ReadonlyByteArray Pal_Title = s_resource(RESOURCE__PALETTE__TITLE_SCREEN_BIN);
    // ReadonlyByteArray Pal_LevelSel = s_resource(RESOURCE__PALETTE__LEVEL_SELECT_BIN);
    // ReadonlyByteArray Pal_Sonic = s_resource(RESOURCE__PALETTE__SONIC_BIN);
    // ReadonlyByteArray Pal_GHZ = s_resource(RESOURCE__PALETTE__GREEN_HILL_ZONE_BIN);
    // ReadonlyByteArray Pal_LZ = s_resource(RESOURCE__PALETTE__LABYRINTH_ZONE_BIN);
    // ReadonlyByteArray Pal_LZWater = s_resource(RESOURCE__PALETTE__LABYRINTH_ZONE_UNDERWATER_BIN);
    // ReadonlyByteArray Pal_MZ = s_resource(RESOURCE__PALETTE__MARBLE_ZONE_BIN);
    // ReadonlyByteArray Pal_SLZ = s_resource(RESOURCE__PALETTE__STAR_LIGHT_ZONE_BIN);
    // ReadonlyByteArray Pal_SYZ = s_resource(RESOURCE__PALETTE__SPRING_YARD_ZONE_BIN);
    // ReadonlyByteArray Pal_SBZ1 = s_resource(ResourceID res_id)
    // ReadonlyByteArray Pal_SBZ2 =
    // ReadonlyByteArray Pal_Special =
    // ReadonlyByteArray Pal_SBZ3 =
    // ReadonlyByteArray Pal_SBZ3Water =
    // ReadonlyByteArray Pal_LZSonWater =
    // ReadonlyByteArray Pal_SBZ3SonWat =
    // ReadonlyByteArray Pal_SSResult =
    // ReadonlyByteArray Pal_Continue =
    // ReadonlyByteArray Pal_Ending =

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
    ReadonlyByteArray Eni_SegaLogo = s_resource(RESOURCE__TILEMAPS__SEGA_LOGO_ENI);
    ReadonlyByteArray Nem_SegaLogo = s_resource(RESOURCE__ARTNEM__SEGA_LOGO_NEM);

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

    // FIXME
    for (u8* i = ctx->var->v_palette; i != ctx->varptr->v_palette_end; i++) {
        *i = 0xFF;
    }

    // TODO
    locVRAM(ArtTile_Sega_Tiles*tile_size, vdp_control_port);
    ctx->reg.a0 = Nem_SegaLogo.arr; ctx->reg.a0int = Nem_SegaLogo.size;
    NemDec(ctx);

    ctx->reg.a1 = ctx->var->v_256x256;  ctx->reg.a1int = ctx->varptr->v_256x256_end - ctx->var->v_256x256;
    ctx->reg.a0 = Eni_SegaLogo.arr;     ctx->reg.a0int = Eni_SegaLogo.size;
    ctx->reg.d0 = make_art_tile(ArtTile_Sega_Tiles, 0, 0);
    EniDec(ctx);

    copyTilemapXY(ctx->var->v_256x256, vram_bg, 8, 0xA, 24, 8);
    copyTilemapXY(ctx->var->v_256x256+24*8*2, vram_fg, 0, 0, 40, 28);

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

// ===========================================================================

// ---------------------------------------------------------------------------
// Title	screen
// ---------------------------------------------------------------------------

static void GM_Title(S1Context* ctx) {
    ReadonlyByteArray Nem_JapNames    = s_resource(RESOURCE__ARTNEM__HIDDEN_JAPANESE_CREDITS_NEM);
    ReadonlyByteArray Nem_CreditText  = s_resource(RESOURCE__ARTNEM__ENDING_CREDITS_NEM);
    ReadonlyByteArray Eni_JapNames    = s_resource(RESOURCE__TILEMAPS__HIDDEN_JAPANESE_CREDITS_ENI);
    
    ReadonlyByteArray Nem_TitleFg     = s_resource(RESOURCE__ARTNEM__TITLE_SCREEN_FOREGROUND_NEM);
    ReadonlyByteArray Nem_TitleSonic  = s_resource(RESOURCE__ARTNEM__TITLE_SCREEN_SONIC_NEM);
    ReadonlyByteArray Nem_TitleTM     = s_resource(RESOURCE__ARTNEM__TITLE_SCREEN_TM_NEM);

    u8* Art_Text = s_resource(RESOURCE__ARTUNC__MENUTEXT_BIN).arr;
    u8* Art_Text_End = Art_Text + s_resource(RESOURCE__ARTUNC__MENUTEXT_BIN).size + 1;

    ReadonlyByteArray Blk16_GHZ = s_resource(RESOURCE__MAP16__GHZ_ENI);
    ReadonlyByteArray Nem_GHZ_1st = s_resource(RESOURCE__ARTNEM__8X8_GHZ1_NEM);
    u8* Blk256_GHZ = s_resource(RESOURCE__MAP256__GHZ_KOS).arr;

    ReadonlyByteArray Eni_Title = s_resource(RESOURCE__TILEMAPS__TITLE_SCREEN_ENI);

    // ===========================================================================
    // ---------------------------------------------------------------------------
    // Levels used in demos
    // ---------------------------------------------------------------------------
    u8* Demo_Levels = s_resource(RESOURCE__MISC__DEMO_LEVEL_ORDER_INTRO_BIN).arr;

    ctx->reg.d0 = bgm_Stop;
    PlaySound_Special(ctx); // stop music
    ClearPLC(ctx);
    PaletteFadeOut(ctx);
    disable_ints;
    DACDriverLoad(ctx);

    VDP_CONTROL_PORT_WRITE(0x8004);                 // 8-colour mode
    VDP_CONTROL_PORT_WRITE(0x8200+(vram_fg>>10));   // set foreground nametable address
    VDP_CONTROL_PORT_WRITE(0x8400+(vram_bg>>13));   // set background nametable address
    VDP_CONTROL_PORT_WRITE(0x9001);                 // 64-cell hscroll size
    VDP_CONTROL_PORT_WRITE(0x9200);                 // window vertical position
    VDP_CONTROL_PORT_WRITE(0x8B03);
    VDP_CONTROL_PORT_WRITE(0x8720);                 // set background colour (palette line 2, entry 0)

    ctx->var->f_wtr_state = 0;
    ClearScreen(ctx);

    clearRAM(ctx->var->v_objspace, ctx->varptr->v_objspace_end);

    locVRAM(ArtTile_Title_Japanese_Text*tile_size, vdp_control_port);
    ctx->reg.a0 = Nem_JapNames.arr; ctx->reg.a0int = Nem_JapNames.size; // load Japanese credits
    NemDec(ctx);
    locVRAM(ArtTile_Sonic_Team_Font*tile_size, vdp_control_port);
    ctx->reg.a0 = Nem_CreditText.arr; ctx->reg.a0int = Nem_CreditText.size; // load alphabet
    NemDec(ctx);
    ctx->reg.a1 = ctx->var->v_256x256; ctx->reg.a1int = ctx->varptr->v_256x256_end - ctx->var->v_256x256;
    ctx->reg.a0 = Eni_JapNames.arr;    ctx->reg.a0int = Eni_JapNames.size; // load mappings for	Japanese credits
    ctx->reg.d0 = make_art_tile(ArtTile_Title_Japanese_Text, 0, 0);
    EniDec(ctx);

    copyTilemapXY(ctx->var->v_256x256, vram_fg, 0, 0, 40, 28);

    clearRAM(ctx->var->v_palette_fading, ctx->varptr->v_palette_fading_end);

    ctx->reg.d0 = palid_Sonic; // load Sonic's palette
    PalLoad_Fade(ctx);
    ctx->varptr->v_sonicteam[0] = id_CreditsText; // load "SONIC TEAM PRESENTS" object
    ExecuteObjects(ctx);
    BuildSprites(ctx);
    PaletteFadeIn(ctx);
    disable_ints;
    locVRAM(ArtTile_Title_Foreground*tile_size, vdp_control_port);
    ctx->reg.a0 = Nem_TitleFg.arr; ctx->reg.a0int = Nem_TitleFg.size; // load title	screen patterns
    NemDec(ctx);
    locVRAM(ArtTile_Title_Sonic*tile_size, vdp_control_port);
    ctx->reg.a0 = Nem_TitleSonic.arr; ctx->reg.a0int = Nem_TitleSonic.size; // load Sonic title screen	patterns
    NemDec(ctx);
    locVRAM(ArtTile_Title_Trademark*tile_size, vdp_control_port);
    ctx->reg.a0 = Nem_TitleTM.arr; ctx->reg.a0int = Nem_TitleTM.size; // load "TM" patterns
    NemDec(ctx);
    // lea	(vdp_data_port).l,a6
    locVRAM(ArtTile_Level_Select_Font*tile_size, 4 + vdp_data_port);
    ctx->reg.a5 = Art_Text;
    ctx->reg.d1 = (Art_Text_End-Art_Text)/2-1;

    // Tit_LoadText:
    do {
        VDP_DATA_PORT_WRITE( *((u16*)ctx->reg.a5) );
        ctx->reg.a5 += 2;
        ctx->reg.d1--;
    } while (ctx->reg.d1 != 0);

    ctx->var->v_lastlamp[0] = 0; // clear lamppost counter
    ctx->var->v_debuguse = 0; // disable debug item placement mode
    ctx->var->f_demo = 0; // disable debug mode
    ctx->var->v_unused2 = 0; // unused variable
    ctx->var->v_zone = id_GHZ << 8; // set level to GHZ (00)
    ctx->var->v_pcyc_time = 0; // disable palette cycling
    LevelSizeLoad(ctx);
    DeformLayers(ctx);
    ctx->reg.a1 = ctx->var->v_16x16;    ctx->reg.a1int = 0x1800;
    ctx->reg.a0 = Blk16_GHZ.arr;        ctx->reg.a0int = Blk16_GHZ.size;  // load GHZ 16x16 mappings
    ctx->reg.d0 = make_art_tile(ArtTile_Level, 0, 0);
    EniDec(ctx);
    ctx->reg.a0 = Blk256_GHZ; // load GHZ 256x256 mapping
    ctx->reg.a1 = ctx->var->v_256x256;
    KosDec(ctx);
    LevelLayoutLoad(ctx);
    PaletteFadeOut(ctx);
    disable_ints;
    ClearScreen(ctx);
    // lea	(vdp_control_port).l,a5
    // lea	(vdp_data_port).l,a6
    // lea	(v_bgscreenposx).w,a3
    // lea	(v_lvllayout+$40).w,a4
    ctx->reg.d2 = 0x6000;
    DrawChunks(ctx);
    ctx->reg.a1 = ctx->var->v_256x256; ctx->reg.a1int = ctx->varptr->v_256x256_end - ctx->var->v_256x256;
    ctx->reg.a0 = Eni_Title.arr;       ctx->reg.a0int = Eni_Title.size; // load title screen mappings
    ctx->reg.d0 = 0;
    EniDec(ctx);

    copyTilemapXY(ctx->var->v_256x256, vram_fg, 3, 4, 34, 22);

    locVRAM(ArtTile_Level*tile_size, vdp_control_port);
    ctx->reg.a0 = Nem_GHZ_1st.arr; ctx->reg.a0int = Nem_GHZ_1st.size;  // load GHZ patterns
    NemDec(ctx);
    ctx->reg.d0 = palid_Title; // load title screen palette
    PalLoad_Fade(ctx);
    ctx->reg.d0 = bgm_Title;
    PlaySound_Special(ctx); // play title screen music
    ctx->var->f_debugmode = 0; // disable debug mode
    ctx->var->v_demolength = 0x178; // run title screen for 0x178 frames

#ifdef FixBugs
    clearRAM(ctx->varptr->v_sonicteam, ctx->varptr->v_sonicteam+object_size);
#else
    // Bug: this only clears half of the "SONIC TEAM PRESENTS" slot.
    // This is responsible for why the "PRESS START BUTTON" text doesn't
    // show up, as the routine ID isn't reset.
    clearRAM(ctx->varptr->v_sonicteam, ctx->varptr->v_sonicteam+object_size / 2);
#endif

    ctx->varptr->v_titlesonic[0] = id_TitleSonic; // load big Sonic object
    ctx->varptr->v_pressstart[0] = id_PSBTM; // load "PRESS START BUTTON" object
    // ctx->varptr->v_pressstart[obRoutine] = 0; // The 'Mega Games 10' version of Sonic 1 added this line, to fix the 'PRESS START BUTTON' object not appearing

#if Revision != 0
    if (ctx->var->v_megadrive) {    // is console Japanese?
        goto GM_Title__isjap;       
    }
#endif

    ctx->var->v_megadrive = id_PSBTM; // load "TM" object
    ctx->varptr->v_titletm[obFrame] = 3;
GM_Title__isjap:
    ctx->varptr->v_ttlsonichide[obFrame] = 2;
    ExecuteObjects(ctx);
    DeformLayers(ctx);
    BuildSprites(ctx);
    ctx->reg.d0 = plcid_Main;
    NewPLC(ctx);
    ctx->var->v_title_dcount = 0;
    ctx->var->v_title_ccount = 0;
    ctx->reg.d0 = ctx->var->v_vdp_buffer1;
    ctx->reg.d0 |= 0x40;
    VDP_CONTROL_PORT_WRITE(ctx->reg.d0);
    PaletteFadeIn(ctx);

Tit_MainLoop:
    ctx->var->v_vbla_routine = 4;
    WaitForVBla(ctx);
    ExecuteObjects(ctx);
    DeformLayers(ctx);
    BuildSprites(ctx);
    PalCycle_Title(ctx);
    RunPLC(ctx);
    ctx->reg.d0 = (ctx->varptr->v_player[obX] << 8) | ctx->varptr->v_player[obX + 1]; // move Sonic to the right
    // has Sonic object passed $1C00 on x-axis?
    if (ctx->reg.d0 >= 0x1C00) {
        ctx->var->v_gamemode = id_Sega; // go to Sega Screen
        return;
    }

    // ===========================================================================

    // ---------------------------------------------------------------------------
    // Level	select codes
    // ---------------------------------------------------------------------------
    
#if Revision == 0
    const u8 LevSelCode_J[] = { btnUp, btnDn, btnL, btnR, 0, 0xFF };
#else 
    const u8 LevSelCode_J[] = { btnUp, btnDn, btnDn, btnDn, btnL, btnR, 0, 0xFF };
#endif

    const u8 LevSelCode_US[] = { btnUp, btnDn, btnL, btnR, 0, 0xFF };


    // Tit_ChkRegion:
    // check	if the machine is US or	Japanese
    if (ctx->var->v_megadrive != 0) {
        ctx->reg.a0 = LevSelCode_US; // load US code
    } else {
        ctx->reg.a0 = LevSelCode_J; // load J code
    }

    // Tit_EnterCheat
    ctx->reg.d0 = ctx->var->v_title_dcount;
    ctx->reg.a0 += ctx->reg.d0;
    ctx->reg.d0 = ctx->var->v_jpadpress1; // get button press
    ctx->reg.d0 &= btnDir; // read only UDLR buttons
    if (*ctx->reg.a0 != ctx->reg.d0) {
        goto Tit_ResetCheat;
    }
    
    ctx->var->v_title_dcount += 1; // next button press
    if (ctx->reg.d0 != 0) {
        goto Tit_CountC;
    }
    ctx->reg.a0 = &(ctx->var->f_levselcheat);
    ctx->reg.d1 = ctx->var->v_title_ccount;
    ctx->reg.d1 >>= 1;
    ctx->reg.d1 &= 3;
    if (ctx->reg.d1 == 0) {
        goto Tit_PlayRing;
    }

    if (ctx->var->v_megadrive) {
        goto Tit_PlayRing;
    }

    ctx->reg.d1 = 1;
    ctx->reg.a0[1 + ctx->reg.d1] = ctx->reg.d1; // cheat depends on how many times C is pressed

Tit_PlayRing:
    ctx->reg.a0[ctx->reg.d1] = 1; // activate cheat
    ctx->reg.d0 = sfx_Ring;
    PlaySound_Special(ctx);
    goto Tit_CountC;
// ===========================================================================

Tit_ResetCheat:
    if (ctx->reg.d0 == 0) {
        goto Tit_CountC;
    }
    if (ctx->var->v_title_dcount == 9) {
        goto Tit_CountC;
    }
    ctx->var->v_title_dcount = 0;

Tit_CountC:
    ctx->reg.d0 = ctx->var->v_jpadpress1;
    ctx->reg.d0 &= btnC;
    if (ctx->reg.d0 != 0) {
        ctx->var->v_title_ccount++;
    }

    // loc_3230
    if (ctx->var->v_demolength == 0) {
        goto GotoDemo;
    }
    ctx->var->v_jpadpress1 &= btnStart; // check if Start pressed
    if (ctx->var->v_jpadpress1 == 0) {
        goto Tit_MainLoop;
    }

Tit_ChkLevSel:
    // check if level select code is on
    if (ctx->var->f_levselcheat == 0) {
        goto PlayLevel;
    }
    // check if A is pressed
    if ((ctx->var->v_jpadhold1 & bitA) == 0) {
        goto PlayLevel;
    }

    ctx->reg.d0 = palid_LevelSel;
    PalLoad(ctx); // load level select palette
   
    clearRAM(ctx->var->v_hscrolltablebuffer, ctx->varptr->v_hscrolltablebuffer_end);

    ctx->var->v_scrposy_vdp = ctx->reg.d0;
    disable_ints;
    // move.l	d0,(v_scrposy_vdp).w
    locVRAM(vram_bg, vdp_control_port);
    ctx->reg.d1 = plane_size_64x32/4 - 1;
    
// Tit_ClrScroll2
    do {
        VDP_DATA_PORT_WRITE(ctx->reg.d0);
        ctx->reg.d1--;
    } while (ctx->reg.d1 != 0); // clear scroll data (in VRAM)

    LevSelTextLoad(ctx);

// ---------------------------------------------------------------------------
// Level	Select
// ---------------------------------------------------------------------------

LevelSelect:
    ctx->var->v_vbla_routine = 4;
    WaitForVBla(ctx);
    LevSelControls(ctx);
    RunPLC(ctx);
    if (*((u16*)ctx->var->v_plc_buffer) != 0) {
        goto LevelSelect;
    }
    ctx->var->v_jpadpress1 = btnABC+btnStart; // is A, B, C, or Start pressed?
    if (ctx->var->v_jpadpress1 != 0) {
        goto LevelSelect;
    }
    ctx->reg.d0 = ctx->var->v_levselitem;
    // have you selected item $14 (sound test)?
    if (ctx->reg.d0 != 0x14) {
        goto LevSel_Level_SS;
    }
    ctx->reg.d0 += 0x80;
    // is Japanese Credits cheat on?
    if (ctx->var->f_creditscheat == 0) {
        goto LevSel_NoCheat;
    }
    // is sound $9F being played?
    if (ctx->reg.d0 == 0x9F) {
        goto LevSel_Credits;
    }

LevSel_NoCheat:
    // This is a workaround for a bug; see PlaySoundID for more.
    // Once you've fixed the bugs there, comment these four instructions out.
    if (ctx->reg.d0 < bgm__Last+1) {
        goto LevSel_PlaySnd;
    }
    if (ctx->reg.d0 < sfx__First) {
        goto LevelSelect;
    }

LevSel_PlaySnd:
    PlaySound_Special(ctx);
    goto LevelSelect;

// ===========================================================================

LevSel_Ending:
    ctx->var->v_gamemode = id_Ending; // set screen mode to $18 (Ending)
    ctx->var->v_zone = id_EndZ << 8; 
    return;
// ===========================================================================

LevSel_Credits:
    ctx->var->v_gamemode = id_Credits; // set screen mode to $1C (Credits)
    ctx->reg.d0 = bgm_Credits;
    PlaySound_Special(ctx);
    return;
// ===========================================================================

LevSel_Level_SS:
    ctx->reg.d0 += ctx->reg.d0;
    // ctx->reg.d0 = LevSel_Ptrs; 
    if (ctx->reg.d0 < 0) {
        goto LevelSelect;
    }
    
    // check if level is 0700 (Special Stage)
    if (ctx->reg.d0 != id_SS*0x100) {
        goto LevSel_Level;
    }

    ctx->var->v_gamemode = id_Special; // set screen mode to 0x10 (Special Stage)
    ctx->var->v_zone = 0; // clear level
    ctx->var->v_lives = 3; // set lives to 3
    ctx->reg.d0 = 0;
    ctx->var->v_rings = ctx->reg.d0; // clear rings
    ctx->var->v_time = ctx->reg.d0;  // clear time
    ctx->var->v_score = ctx->reg.d0; // clear score
#if Revision != 0
    ctx->var->v_scorelife = 0x5000; // extra life is awarded at 50_000 points
#endif
    return;
// ===========================================================================

LevSel_Level:
    ctx->reg.d0 &= 0x3FFF;
    ctx->var->v_zone = ctx->reg.d0; // set level number

PlayLevel:
    ctx->var->v_gamemode = id_Level;    // set screen mode to $0C (level)
    ctx->var->v_lives = 3;              // set lives to 3
    ctx->reg.d0 = 0;    
    ctx->var->v_rings = ctx->reg.d0;    // clear rings  
    ctx->var->v_time = ctx->reg.d0;     // clear time
    ctx->var->v_score = ctx->reg.d0;    // clear score
    ctx->var->v_lastspecial = ctx->reg.d0;  // clear special stage number
    ctx->var->v_emeralds = ctx->reg.d0;     // clear emeralds
    *((u32*)ctx->var->v_emldlist) = ctx->reg.d0;    // clear emeralds
    *((u32*)(ctx->var->v_emldlist + 4)) = ctx->reg.d0;  // clear emeralds
    ctx->var->v_continues = ctx->reg.d0;  // clear continues
#if Revision != 0
    ctx->var->v_scorelife = 0x5000; // extra life is awarded at 50000 points
#endif
    ctx->reg.d0 = bgm_Fade;
    PlaySound_Special(ctx);
    return;

    const u8 LevSel_Ptrs[] = {
#if Revision == 0
        // old level order
        id_GHZ, 0,
		id_GHZ, 1,
		id_GHZ, 2,
		id_LZ, 0,
		id_LZ, 1,
		id_LZ, 2,
		id_MZ, 0,
		id_MZ, 1,
		id_MZ, 2,
		id_SLZ, 0,
		id_SLZ, 1,
		id_SLZ, 2,
		id_SYZ, 0,
		id_SYZ, 1,
		id_SYZ, 2,
		id_SBZ, 0,
		id_SBZ, 1,
		id_LZ, 3,		// Scrap Brain Zone 3
		id_SBZ, 2,		// Final Zone
#else 
        // correct level order
        id_GHZ, 0,
		id_GHZ, 1,
		id_GHZ, 2,
		id_MZ, 0,
		id_MZ, 1,
		id_MZ, 2,
		id_SYZ, 0,
		id_SYZ, 1,
		id_SYZ, 2,
		id_LZ, 0,
		id_LZ, 1,
		id_LZ, 2,
		id_SLZ, 0,
		id_SLZ, 1,
		id_SLZ, 2,
		id_SBZ, 0,
		id_SBZ, 1,
		id_LZ, 3,
		id_SBZ, 2,
#endif
        id_SS, 0,
        0x80, 0x00
    };

// ===========================================================================

// ---------------------------------------------------------------------------
// Demo mode
// ---------------------------------------------------------------------------

GotoDemo:
    ctx->var->v_demolength = 0x1E;

loc_33B6:
    ctx->var->v_vbla_routine = 4;
    WaitForVBla(ctx);
    DeformLayers(ctx);
    PaletteCycle(ctx);
    RunPLC(ctx);
    ctx->reg.d0 = ctx->varptr->v_player[obX];
    ctx->reg.d0 += 2;
    ctx->varptr->v_player[obX] = ctx->reg.d0;
    if (ctx->reg.d0 >= 0x1C00) {
        ctx->var->v_gamemode = id_Sega;
        return;
    }

// ===========================================================================

loc_33E4:
    ctx->var->v_jpadpress1 &= btnStart;
    if (ctx->var->v_jpadpress1 == 0) {
        goto Tit_ChkLevSel;
    }
    if (ctx->var->v_demolength != 0) {
        goto loc_33B6;
    }
    ctx->reg.d0 = bgm_Fade;
    PlaySound_Special(ctx); // fade out music
    ctx->reg.d0 = ctx->var->v_demonum; // load demo number
    ctx->reg.d0 &= 7;
    ctx->reg.d0 += ctx->reg.d0;
    ctx->reg.d0 = Demo_Levels[ctx->reg.d0]; // load level number for demo
    ctx->var->v_zone = ctx->reg.d0;
    ctx->var->v_demonum++; // add 1 to demo number
    // is demo number less than 4?
    if (ctx->var->v_demonum >= 4) {
        ctx->var->v_demonum = 0;
    }

    // loc_3422
    ctx->var->f_demo = 1; // turn demo mode on
    ctx->var->v_gamemode = id_Demo; // set screen mode to 08 (demo)
    // is level number 0600 (special stage)?
    if (ctx->reg.d0 == 0x600) {
        ctx->var->v_gamemode = id_Special; // set screen mode to $10 (Special Stage)
        ctx->var->v_zone = 0; // clear level number
        ctx->var->v_lastspecial = 0; // clear special stage number
    }

    // Demo_Level:
    ctx->var->v_lives = 3; // set lives to 3
    ctx->reg.d0 = 0;
    ctx->var->v_rings = 0; // clear rings
    ctx->var->v_time = 0; // clear time
    ctx->var->v_score = 0; // clear score

#if Revision != 0
    ctx->var->v_scorelife = 0x5000; // extra life is awarded at 50000 points
#endif

    return;
}

// ---------------------------------------------------------------------------
// Subroutine to	change what you're selecting in the level select
// ---------------------------------------------------------------------------

// ||||||||||||||| S U B	R O U T	I N E |||||||||||||||||||||||||||||||||||||||

static void LevSelControls(S1Context* ctx) {
    // Check if up/down is pressed and held
    u8 d1 = ctx->var->v_jpadpress1 & (btnUp | btnDn);
    if (d1 != 0) {
        goto LevSel_UpDown;
    }

    // Subtract 1 from time to next move
    ctx->var->v_levseldelay--;
    if (ctx->var->v_levseldelay >= 0) {
        goto LevSel_SndTest;
    }

LevSel_UpDown:
    // Reset time delay
    ctx->var->v_levseldelay = 0xB;

    // Check if up/down is pressed
    d1 = ctx->var->v_jpadhold1 & (btnUp | btnDn);
    if (d1 == 0) {
        goto LevSel_SndTest;
    }

    // Get current selection
    u16 d0 = ctx->var->v_levselitem;

    // Check if up is pressed
    if (d1 & btnUp) {
        d0--; // Move up 1 selection
        if (d0 < 0) {
            d0 = 0x14; // Wrap to selection $14 if below 0
        }
    }

    // Check if down is pressed
    if (d1 & btnDn) {
        d0++; // Move down 1 selection
        if (d0 >= 0x15) {
            d0 = 0; // Wrap to selection 0 if above $14
        }
    }

LevSel_Refresh:
    // Set new selection and refresh text
    ctx->var->v_levselitem = d0;
    LevSelTextLoad(ctx);
    return;

LevSel_SndTest:
    // Check if item $14 (sound test) is selected
    if (ctx->var->v_levselitem != 0x14) {
        goto LevSel_NoMove;
    }

    // Check if left/right is pressed
    d1 = ctx->var->v_jpadpress1 & (btnL | btnR);
    if (d1 == 0) {
        goto LevSel_NoMove;
    }

    // Get current sound test number
    d0 = ctx->var->v_levselsound;

    // Check if left is pressed
    if (d1 & btnL) {
        d0--; // Subtract 1 from sound test
        if (d0 < 0) {
            d0 = 0x4F; // Wrap to $4F if below 0
        }
    }

    // Check if right is pressed
    if (d1 & btnR) {
        d0++; // Add 1 to sound test
        if (d0 >= 0x50) {
            d0 = 0; // Wrap to 0 if above $4F
        }
    }

LevSel_Refresh2:
    // Set sound test number and refresh text
    ctx->var->v_levselsound = d0;
    LevSelTextLoad(ctx);

LevSel_NoMove:
    return;
}


// ---------------------------------------------------------------------------
// Subroutine to load level select text
// ---------------------------------------------------------------------------

// ||||||||||||||| S U B	R O U T	I N E |||||||||||||||||||||||||||||||||||||||

static void LevSelTextLoad(S1Context* ctx) {
    
    // ---------------------------------------------------------------------------
    // Level	select menu text
    // ---------------------------------------------------------------------------
    u8* LevelMenuText = s_resource(
#if Revision == 0 
        RESOURCE__MISC__LEVEL_SELECT_TEXT_BIN
#else 
        RESOURCE__MISC__LEVEL_SELECT_TEXT_JP1_BIN
#endif 
    ).arr;

    // Constants
    const u32 textpos = (0x40000000 | ((0xE210 & 0x3FFF) << 16) | ((0xE210 & 0xC000) >> 14));

    // Initialize registers
    ctx->reg.a1 = (u8*)LevelMenuText;       // Pointer to text data
    // ctx->reg.a6 = (u8*)ctx->var.vdp_data_port; // VDP data port
    ctx->reg.d4 = textpos;                  // Text position on screen
    ctx->reg.d3 = 0xE680;                   // VRAM setting (4th palette, $680th tile)
    ctx->reg.d1 = 0x14;                     // Number of lines of text

    // Draw all lines of text
    do {
        // *((u32*)(ctx->reg.a6 + 4)) = ctx->reg.d4; // Set VRAM address
        LevSel_ChgLine(ctx);                      // Draw line of text
        ctx->reg.d4 += 0x800000;                  // Jump to next line
        ctx->reg.d1--;
    } while (ctx->reg.d1 != 0);

    // Highlight selected line
    ctx->reg.d0 = ctx->var->v_levselitem;          // Get selected item index
    ctx->reg.d1 = ctx->reg.d0;
    ctx->reg.d4 = textpos;                        // Reset text position
    ctx->reg.d0 <<= 7;                            // Calculate offset for selected line
    ctx->reg.d0 = ((ctx->reg.d0 & 0xFFFF) << 16) | ((ctx->reg.d0 >> 16) & 0xFFFF); // Swap words
    ctx->reg.d4 += ctx->reg.d0;                   // Adjust text position for selected line
    ctx->reg.a1 = (u8*)LevelMenuText;             // Reset text pointer
    ctx->reg.d1 <<= 3;                            // Calculate offset in text data
    ctx->reg.d0 = ctx->reg.d1;
    ctx->reg.d1 += ctx->reg.d1;                   // Multiply by 3
    ctx->reg.d1 += ctx->reg.d0;
    ctx->reg.a1 += ctx->reg.d1;                   // Adjust text pointer for selected line
    ctx->reg.d3 = 0xC680;                         // VRAM setting (3rd palette, $680th tile)
    *((u32*)(ctx->reg.a6 + 4)) = ctx->reg.d4;     // Set VRAM address
    LevSel_ChgLine(ctx);                          // Recolor selected line

    // Check if selected item is the sound test option
    ctx->reg.d3 = 0xE680;                         // Reset VRAM setting
    if (ctx->var->v_levselitem != 0x14) {
        goto LevSel_DrawSnd;
    }
    ctx->reg.d3 = 0xC680;                         // Change VRAM setting for sound test

LevSel_DrawSnd:
    // Draw sound test digits
    // *((u32*)(ctx->reg.a6 + 4)) = ctx->var.vram_bg + 0xC30; // Set VRAM address for sound test
    ctx->reg.d0 = ctx->var->v_levselsound;         // Get sound test value
    ctx->reg.d0 += 0x80;                          // Adjust for display
    ctx->reg.d2 = ctx->reg.d0;                    // Save for second digit
    ctx->reg.d0 >>= 4;                            // Extract first digit
    LevSel_ChgSnd(ctx);                           // Draw first digit
    ctx->reg.d0 = ctx->reg.d2;                    // Restore for second digit
    LevSel_ChgSnd(ctx);                           // Draw second digit
}

// ||||||||||||||| S U B	R O U T	I N E |||||||||||||||||||||||||||||||||||||||

static void LevSel_ChgSnd(S1Context* ctx) {
    // Mask the lower 4 bits of d0 to get the digit
    ctx->reg.d0 &= 0xF;

    // Check if the digit is $A-$F (hexadecimal)
    if (ctx->reg.d0 >= 0xA) {
        // Adjust for alpha characters (A-F)
        ctx->reg.d0 += 7;
    }

    // Combine with the VRAM setting (d3) and write to VDP data port (a6)
    ctx->reg.d0 += ctx->reg.d3;
    // TODO *((u16*)ctx->reg.a6) = ctx->reg.d0; 
}

// ||||||||||||||| S U B	R O U T	I N E |||||||||||||||||||||||||||||||||||||||

static void LevSel_ChgLine(S1Context* ctx) {
    // Number of characters per line
    ctx->reg.d2 = 0x17;

    // Loop through each character in the line
    do {
        // Get the next character from the text data
        ctx->reg.d0 = *((u8*)ctx->reg.a1);
        ctx->reg.a1++;

        // Check if the character is valid (positive)
        if (ctx->reg.d0 >= 0) {
            // Combine the character with the VRAM setting (d3)
            ctx->reg.d0 += ctx->reg.d3;

            // Send the character to VRAM
            *((u16*)ctx->reg.a6) = ctx->reg.d0;
        } else {
            // Use a blank character (0) for invalid characters
            *((u16*)ctx->reg.a6) = 0;
        }

        // Decrement the character counter
        ctx->reg.d2--;
    } while (ctx->reg.d2 != 0);
}