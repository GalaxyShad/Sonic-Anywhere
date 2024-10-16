#include "gamevdp.h"

#include "gamemode/game.h"

#include "include_backend/debug.h"
#include "include_backend/system.h"
#include "include_backend/vdp.h"

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

static u16 vblank_routine__ = 0;
static u16 vblank_count__ = 0;
static bool hblank_is_need_to_change_palette__ = 0; // flag set to change palette during HBlank (0000 = no; 0001 = change)
static u16 vdp_h_interrupt_register_buffer__ = 0x8A00;
static u16 demo_length__ = 0;

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

static void vblank_stop_music__() {
    NOT_IMPLEMENTED
}

static void vblank_wait_pal__() {
    for (int i = 0; i < 0x700; i++)
        ;
}

static void vblank_update_palette__() {
    game_vdp__palette_copy_to_vdp();
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

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

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

// clang-format off
static void (*vblank_index__[])() = {
    vblank_00__,vblank_02__,
    vblank_04__,vblank_06__,
    vblank_08__,vblank_0A__,
    vblank_0C__,vblank_0E__,
    vblank_10__,vblank_12__,
    vblank_14__,vblank_16__,
    vblank_0C__
};
// clang-format on


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

    hblank_is_need_to_change_palette__ = 1;

    // TODO
    // stop Z80
    // wait Z80

    vblank_update_palette__();

    // 		move.w	(v_hbla_hreg).w,(a5)
    //		startZ80
}

static void vblank_process_demo_timer__() {
    if (demo_length__ == 0)
        return;

    demo_length__--;
}

static void sub_1642__() {
    // TODO
    NOT_IMPLEMENTED
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
    NOT_IMPLEMENTED
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
    NOT_IMPLEMENTED
}

static void vblank_0E__() {
    vblank_update__();
    //  addq.b	#1,(v_vbla_0e_counter).w ; Unused besides this one write...
    vblank_routine__ = 0xE;
    NOT_IMPLEMENTED
}

static void vblank_10__() {
    u16 id_special;

    if (game__get_game_mode() == id_special) {
        vblank_0A__();
    }

    vblank_08__();

    NOT_IMPLEMENTED
}

static void vblank_12__() {
    vblank_update__();
    // move.w	(v_hbla_hreg).w,(a5)
    sub_1642__();

    NOT_IMPLEMENTED
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

    NOT_IMPLEMENTED
}

//////////////////////////////////////////////////////////////////////////////////////////
//// PUBLIC
//////////////////////////////////////////////////////////////////////////////////////////

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
    hblank_is_need_to_change_palette__ = 1;

    d0 = (d0 & 0x3E) >> 1;

    vblank_index__[d0]();

    // VBla_Music:
    vblank_stop_music__();

    // VBla_Exit:
    vblank_count__++;
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

void game_vdp__set_vblank_routine_counter(u8 routine) {
    vblank_routine__ = routine;
}