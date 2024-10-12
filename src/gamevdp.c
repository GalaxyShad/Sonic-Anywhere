#include "gamevdp.h"

#include "include_backend/debug.h"
#include "include_backend/system.h"
#include "include_backend/vdp.h"

#include "gamemode/game.h"
#include "resources/resourcestore.h"

#include "config/config.h"

static MDColor palette__[16 * 4];
static MDColor palette_water__[16 * 4];

void game_vdp__load_palette(GameVdpPaletteID pal_id) {
    ResourceID res_id;

    switch (pal_id) {

    case GAME_VDP_PALETTE_ID__SEGA_LOGO:
        res_id = RESOURCE__PALETTE__SEGA1_BIN;
        break;
    case GAME_VDP_PALETTE_ID__TITLE:
        res_id = RESOURCE__PALETTE__TITLE_SCREEN_BIN;
        break;
    case GAME_VDP_PALETTE_ID__LEVEL_SELECT:
        res_id = RESOURCE__PALETTE__LEVEL_SELECT_BIN;
        break;
    case GAME_VDP_PALETTE_ID__SONIC:
        res_id = RESOURCE__PALETTE__SONIC_BIN;
        break;
    case GAME_VDP_PALETTE_ID__GREEN_HILL_ZONE:
        res_id = RESOURCE__PALETTE__GREEN_HILL_ZONE_BIN;
        break;
    case GAME_VDP_PALETTE_ID__LABYRINTH_ZONE:
        res_id = RESOURCE__PALETTE__LABYRINTH_ZONE_BIN;
        break;
    case GAME_VDP_PALETTE_ID__MARBLE_ZONE:
        res_id = RESOURCE__PALETTE__MARBLE_ZONE_BIN;
        break;
    case GAME_VDP_PALETTE_ID__STAR_LIGHT_ZONE:
        res_id = RESOURCE__PALETTE__STAR_LIGHT_ZONE_BIN;
        break;
    case GAME_VDP_PALETTE_ID__SPRING_YARD_ZONE:
        res_id = RESOURCE__PALETTE__SPRING_YARD_ZONE_BIN;
        break;
    case GAME_VDP_PALETTE_ID__SCRAP_BRAIN_ZONE_ACT_1:
        res_id = RESOURCE__PALETTE__SBZ_ACT_1_BIN;
        break;
    case GAME_VDP_PALETTE_ID__SPECIAL_STAGE:
        res_id = RESOURCE__PALETTE__SPECIAL_STAGE_BIN;
        break;
    case GAME_VDP_PALETTE_ID__LABYRINTH_ZONE_WATER:
        res_id = RESOURCE__PALETTE__LABYRINTH_ZONE_UNDERWATER_BIN;
        break;
    case GAME_VDP_PALETTE_ID__SCRAP_BRAIN_ZONE_ACT_3:
        res_id = RESOURCE__PALETTE__SBZ_ACT_3_BIN;
        break;
    case GAME_VDP_PALETTE_ID__SCRAP_BRAIN_ZONE_ACT_3_WATER:
        res_id = RESOURCE__PALETTE__SBZ_ACT_3_BIN;
        break;
    case GAME_VDP_PALETTE_ID__SCRAP_BRAIN_ZONE_ACT_2:
        res_id = RESOURCE__PALETTE__SBZ_ACT_2_BIN;
        break;
    case GAME_VDP_PALETTE_ID__SONIC_UNDERWATER_LABYRINTH:
        res_id = RESOURCE__PALETTE__SONIC_LZ_UNDERWATER_BIN;
        break;
    case GAME_VDP_PALETTE_ID__SONIC_UNDERWATER_SCRAP_BRAIN:
        res_id = RESOURCE__PALETTE__SONIC_SBZ3_UNDERWATER_BIN;
        break;
    case GAME_VDP_PALETTE_ID__SPECIAL_STAGE_RESULTS:
        res_id = RESOURCE__PALETTE__SPECIAL_STAGE_RESULTS_BIN;
        break;
    case GAME_VDP_PALETTE_ID__SPECIAL_STAGE_RESULTS_CONTINUE:
        res_id = RESOURCE__PALETTE__SPECIAL_STAGE_CONTINUE_BONUS_BIN;
        break;
    case GAME_VDP_PALETTE_ID__ENDING_SEQUENCE:
        res_id = RESOURCE__PALETTE__ENDING_BIN;
        break;
    }

    ReadonlyByteArray res = resource_store__get(res_id);

    for (int i = 0; i < res.size / 2; i++) {
        MDColor color = res.arr[i * 2];
        color = (color << 8) | res.arr[i * 2 + 1];
        palette__[i] = color;
    }
}

static GameVdpPaletteWaterState water_state__;


void game_vdp__set_palette_water_state(GameVdpPaletteWaterState water_state) {
    water_state__ = water_state;
}

void game_vdp__palette_foreach(GameVdpPaletteLayerID pal_id, MDColor (*func)(MDColor)) {
    MDColor* pal = (pal_id == GAME_VDP_PALETTE_LAYER__MAIN) ? palette__ : palette_water__;

    for (int i = 0; i < 16 * 4; i++) {
        pal[i] = func(pal[i]);
    }
}

void game_vdp__palette_set_color(u8 index, MDColor color) {
    ASSERT(index >= 0 && index < 16 * 4)
    palette__[index] = color;
}


//////////////////////////////////////////////////////////////////////
static u16 vblank_routine__ = 0;

static u16 vblank_count = 0;

static bool hblank_is_need_to_change_palette = 0; // flag set to change palette during HBlank (0000 = no; 0001 = change)

static u16 vdp_h_interrupt_register_buffer = 0x8A00;

void game_vdp__set_vblank_routine_counter(u8 routine) {
    vblank_routine__ = routine;
}


static void vblank_00__();
static void vblank_02__();
static void vblank_04__();
static void vblank_06__();
static void vblank_08__();
static void vblank_0A__();
static void vblank_0C__();
static void vblank_0E__();
static void vblank_10__();
static void vblank_12__();
static void vblank_14__();
static void vblank_16__();

static void vblank_stop_music__() {
    RAISE_NOT_IMPLEMENTED
}

static void vblank_wait_pal__() {
    for (int i = 0; i < 0x700; i++)
        ;
}

static void vblank_update_palette__() {
    vdp_palette__load_u16(
      (water_state__ == GAME_VDP_PALETTE_WATER_STATE__ALL_UNDERWATER) ? palette_water__ : palette__
    );
}

// sub_106E
static void vblank_update__() {
    // stopZ80
    // waitZ80

    // TODO
    // ReadJoypads

    vblank_update_palette__();

    //    writeVRAM	v_spritetablebuffer,vram_sprites
    //    writeVRAM	v_hscrolltablebuffer,vram_hscroll

    //    startZ80
}

// clang-format off
static void (*vblank_index[])() = {
    vblank_00__,vblank_02__,
    vblank_04__,vblank_06__,
    vblank_08__,vblank_0A__,
    vblank_0C__,vblank_0E__,
    vblank_10__,vblank_12__,
    vblank_14__,vblank_16__,
    vblank_0C__
};
// clang-format on

// VBlank:
void game_vdp__on_vblank_interrupt() {
    if (vblank_routine__ == 0) {
        // goto VBla_00
    }

    vdp__set_ram_address(VDP_RAM_ACCESS_MODE__CRAM_WRITE, 0, 0, 0);
    //    move.l	(v_scrposy_vdp).w,(vdp_data_port).l ; send screen y-axis pos. to VSRAM

    //    .waitPAL:
    if (system__is_pal()) {
        vblank_wait_pal__(); // wait here in a loop doing nothing for a while...
    }

    u16 d0 = vblank_routine__;

    vblank_routine__ = 0;
    hblank_is_need_to_change_palette = 1;

    d0 = (d0 & 0x3E) >> 1;

    vblank_index[d0]();

    // VBla_Music:
    vblank_stop_music__();

    // VBla_Exit:
    vblank_count++;
}

void game_vdp__wait_for_vblank() {
    // TODO enable_ints
#if CONFIG_VDP_HAS_INTERRUPTS
    while (vblank_routine__ != 0) {
        ;
    }
#else
    vdp__render();
#endif
}

// VBla_00:
static void vblank_00__() {
    i8 id_level = 0;

    i8 id_lz = 0;
    u8 zone = 0;

    GameMode gm = game__get_game_mode();

    if (0x80 + id_level != gm && id_level != gm) {
        // .islevel:
        vblank_stop_music__();
        return;
    }

    if (system__is_pal())
        vblank_wait_pal__();

    hblank_is_need_to_change_palette = 1;

    // TODO
    // stop Z80
    // wait Z80

    vblank_update_palette__();

    // 		move.w	(v_hbla_hreg).w,(a5)
    //		startZ80
}


static u16 demo_length = 0;

static void vblank_process_demo_timer__() {
    if (demo_length == 0)
        return;

    demo_length--;
}

static void sub_1642__() {
    // TODO
    RAISE_NOT_IMPLEMENTED
}

static void vblank_02__() {
    vblank_update__();
    vblank_14__();
}

static void vblank_14__() {
    vblank_process_demo_timer__();
}

static void vblank_04__() {
    vblank_update__();

    // TODO
    //  LoadTilesAsYouMove_BGOnly
    //

    vblank_process_demo_timer__();
}

static void vblank_06__() {
    sub_1642__();
}

static void load_new_sonic_gfx__() {
    RAISE_NOT_IMPLEMENTED
    // TODO
    // 		writeVRAM	v_sgfx_buffer,ArtTile_Sonic*tile_size ; load new Sonic gfx
    //		move.b	#0,(f_sonframechg).w
}

static void vblank_08__() {
    vblank_update__();

    // move.w	(v_hbla_hreg).w,(a5)
    //      IDK why need this, a5 will be reassigned inside writeVRAM

    load_new_sonic_gfx__();

    //
    //.nochg:
    //		startZ80
    //		movem.l	(v_screenposx).w,d0-d7
    //		movem.l	d0-d7,(v_screenposx_dup).w
    //		movem.l	(v_fg_scroll_flags).w,d0-d1
    //		movem.l	d0-d1,(v_fg_scroll_flags_dup).w
    //		cmpi.b	#96,(v_hbla_line).w
    //		bhs.s	Demo_Time
    //		move.b	#1,(f_doupdatesinhblank).w
    //		addq.l	#4,sp
    //		bra.w	VBla_Exit
}

static void vblank_0A__() {
    // TODO

    // 		stopZ80
    //		waitZ80
    //		bsr.w	ReadJoypads
    //		writeCRAM	v_palette,0
    //		writeVRAM	v_spritetablebuffer,vram_sprites
    //		writeVRAM	v_hscrolltablebuffer,vram_hscroll
    //		startZ80
    //		bsr.w	PalCycle_SS
    //		tst.b	(f_sonframechg).w ; has Sonic's sprite changed?
    //		beq.s	.nochg		; if not, branch
    //

    load_new_sonic_gfx__();
    vblank_process_demo_timer__();
}

static void vblank_0C__() {
    // 		stopZ80
    //		waitZ80
    //		bsr.w	ReadJoypads
    //		tst.b	(f_wtr_state).w
    //		bne.s	.waterabove
    //
    //		writeCRAM	v_palette,0
    //		bra.s	.waterbelow
    //
    //.waterabove:
    //		writeCRAM	v_palette_water,0
    //
    //.waterbelow:
    //		move.w	(v_hbla_hreg).w,(a5)
    //		writeVRAM	v_hscrolltablebuffer,vram_hscroll
    //		writeVRAM	v_spritetablebuffer,vram_sprites
    //		tst.b	(f_sonframechg).w
    //		beq.s	.nochg
    //		writeVRAM	v_sgfx_buffer,ArtTile_Sonic*tile_size
    //		move.b	#0,(f_sonframechg).w
    //
    //.nochg:
    //		startZ80
    //		movem.l	(v_screenposx).w,d0-d7
    //		movem.l	d0-d7,(v_screenposx_dup).w
    //		movem.l	(v_fg_scroll_flags).w,d0-d1
    //		movem.l	d0-d1,(v_fg_scroll_flags_dup).w
    //		bsr.w	LoadTilesAsYouMove
    //		jsr	(AnimateLevelGfx).l
    //		jsr	(HUD_Update).l
    //		bsr.w	sub_1642
    //		rts
    RAISE_NOT_IMPLEMENTED
}

static void vblank_0E__() {
    vblank_update__();
    //  addq.b	#1,(v_vbla_0e_counter).w ; Unused besides this one write...
    vblank_routine__ = 0xE;
    RAISE_NOT_IMPLEMENTED
}

static void vblank_10__() {
    u16 id_special;

    if (game__get_game_mode() == id_special) {
        vblank_0A__();
    }

    vblank_08__();

    RAISE_NOT_IMPLEMENTED
}

static void vblank_12__() {
    vblank_update__();
    // move.w	(v_hbla_hreg).w,(a5)
    sub_1642__();

    RAISE_NOT_IMPLEMENTED
}

static void vblank_16__() {
    // 		stopZ80
    //		waitZ80
    //		bsr.w	ReadJoypads
    //		writeCRAM	v_palette,0
    //		writeVRAM	v_spritetablebuffer,vram_sprites
    //		writeVRAM	v_hscrolltablebuffer,vram_hscroll
    //		startZ80
    //		tst.b	(f_sonframechg).w
    //		beq.s	.nochg
    //		writeVRAM	v_sgfx_buffer,ArtTile_Sonic*tile_size
    //		move.b	#0,(f_sonframechg).w
    //
    //.nochg:
    //		tst.w	(v_demolength).w
    //		beq.w	.end
    //		subq.w	#1,(v_demolength).w
    //
    //.end:
    //		rts

    RAISE_NOT_IMPLEMENTED
}