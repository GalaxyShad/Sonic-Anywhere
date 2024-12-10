#include "game.h"

#include "include_backend/mdaudio.h"
#include "include_backend/interrupt.h"
#include "include_backend/mdvdp.h"
#include "include_backend/mdmem.h"

#include "src/plc.h"
#include "src/resources/resourcestore.h"
#include "src/compressors.h"
#include "src/gamevdp.h"
#include "src/objects.h"
#include "src/object_map.h"

static u16 countdown__ = 0;

static void setup_vdp__() {
    md_vdp__set_color_mode(MD_VDP_COLOR_MODE_8_COLOR);
    md_vdp__set_name_table_location_for_plane(MD_VDP_PLANE__FOREGROUND, md_mem__vram()->plane_foreground_mut);
    md_vdp__set_name_table_location_for_plane(MD_VDP_PLANE__BACKGROUND, md_mem__vram()->plane_background_mut);
    md_vdp__set_plane_size(MD_VDP_PLANE_SIZE__64_CELLS, MD_VDP_PLANE_SIZE__32_CELLS);
    md_vdp__set_window_vertical_position(MD_VDP_WINDOW_POSITION__FROM_BACK_TO_FRONT, 0);
    md_vdp__set_scrolling_mode(MD_VDP_VSCROLL_MODE__FULL_SCROLL, MD_VDP_HSCROLL_MODE__EACH_1_LINE, 0);
    md_vdp__set_background_color(2, 0);
}

static void load_nemesis_art_to_vram__(ResourceID res_id) {
    ReadonlyByteArray res = s_resource(res_id);
    compressors__nemesis_decompress_byte_arr(&res, md_mem__vram()->plane_window_mut);
    md_vdp__set_name_table_location_for_plane(MD_VDP_PLANE__WINDOW, md_mem__vram()->plane_window_mut); // FIXME temp solution to update VDP, cuz there is no DMA
}

static void load_level_select_font__() {
    ReadonlyByteArray font_art = s_resource(RESOURCE__MISC__LEVEL_SELECT_TEXT_BIN);

//     	lea	(vdp_data_port).l,a6
    //		locVRAM	vram_text,4(a6)
    //		lea	(Art_Text).l,a5				; load level select font
    //		move.w	#(sizeof_Art_Text/2)-1,d1
    //
    //	.load_text:
    //		move.w	(a5)+,(a6)
    //		dbf	d1,.load_text				; load level select font
}

void game_mode_title() {
    md_audio__stop_sounds();
    plc__clear();
    // PaletteFadeOut
    DISABLE_INTERRUPTS();
    // bsr.w	DACDriverLoad

    setup_vdp__();

    // clr.b	(f_water_pal_full).w
    // bsr.w	ClearScreen

    s_object_pool__all_clear_props();

    // Load Japanese credits
    //load_nemesis_art_to_vram__(RESOURCE__ARTNEM__HIDDEN_JAPANESE_CREDITS_NEM);

    // Load alphabet
    load_nemesis_art_to_vram__(RESOURCE__ARTNEM__ENDING_CREDITS_NEM);

    // Load mappings for Japanese credits
    ReadonlyByteArray jp_credits_mappings = s_resource(RESOURCE__TILEMAPS__HIDDEN_JAPANESE_CREDITS_ENI);
    compressors__enigma_decompress_byte_arr(&jp_credits_mappings, md_mem()->chunks_mut, 0);

    md_vdp__copy_tilemap_to_plane_r(MD_VDP_PLANE__FOREGROUND, 0, 0, (ReadonlyByteArray*)md_mem()->chunks_mut, 0x28, 0x1C);

    // clearRAM v_palette_fading

    game_vdp__load_palette(GAME_VDP_PALETTE_ID__SONIC);

    // bsr.w	PalLoad_Fade
    s_object_pool__load(S_OBJECT_ID__CREDITS_TEXT, 2); // load "SONIC TEAM PRESENTS" object
    s_object_pool__all_execute();

    //		jsr	(BuildSprites).l
    //		bsr.w	PaletteFadeIn

    while (1) {
        game_vdp__set_vblank_routine_counter(4);
        game_vdp__wait_for_vblank();
    }

    DISABLE_INTERRUPTS()

    load_nemesis_art_to_vram__(RESOURCE__ARTNEM__TITLE_SCREEN_FOREGROUND_NEM);
    load_nemesis_art_to_vram__(RESOURCE__ARTNEM__TITLE_SCREEN_SONIC_NEM);
    load_nemesis_art_to_vram__(RESOURCE__ARTNEM__TITLE_SCREEN_TM_NEM);

    load_level_select_font__();

    // move.b	#0,(v_last_lamppost).w			; clear lamppost counter
    //		move.w	#0,(v_debug_active).w			; disable debug item placement mode
    //		move.w	#0,(v_demo_mode).w			; disable debug mode
    //		move.w	#0,(v_title_unused).w			; unused variable
    //		move.w	#id_GHZ_act1,(v_zone).w			; set level to GHZ act 1 (0000)
    //		move.w	#0,(v_palcycle_time).w			; disable palette cycling

    // Load GHZ 16x16 mappings
    ReadonlyByteArray blk_16_ghz = s_resource(RESOURCE__MAP16__GHZ_ENI);
    compressors__enigma_decompress_byte_arr(&blk_16_ghz, md_mem()->tiles_mut, 0);

    // Load GHZ 256x256 mappings
    ReadonlyByteArray blk_256_ghz = s_resource(RESOURCE__MAP256__GHZ_KOS);
    compressors__kosinski_decompress_byte_arr(&blk_256_ghz, md_mem()->chunks_mut);

    // bsr.w	LevelLayoutLoad				; load GHZ1 level layout including background
    // bsr.w	PaletteFadeOut				; fade out "SONIC TEAM PRESENTS" screen to black

    DISABLE_INTERRUPTS()

    // bsr.w	ClearScreen
    //		lea	(vdp_control_port).l,a5
    //		lea	(vdp_data_port).l,a6
    //		lea	(v_bg1_x_pos).w,a3
    //		lea	(v_level_layout+level_max_width).w,a4	; background layout start address ($FFFFA440)
    //		move.w	#draw_bg,d2
    //		bsr.w	DrawChunks				; draw background

    // Load title screen mappings
    ReadonlyByteArray title_mappings = s_resource(RESOURCE__TILEMAPS__TITLE_SCREEN_ENI);
    compressors__enigma_decompress_byte_arr(&title_mappings, md_mem()->chunks_mut, 0);

    md_vdp__copy_tilemap_to_plane_r(MD_VDP_PLANE__FOREGROUND, 3, 4, (ReadonlyByteArray*)md_mem()->chunks_mut, 0x22, 0x16);

    // locVRAM	0

    // load GHZ patterns
    load_nemesis_art_to_vram__(RESOURCE__ARTNEM__8X8_GHZ1_NEM);

    game_vdp__load_palette(GAME_VDP_PALETTE_ID__TITLE);
    // PalLoad_Next

    // play.b	1, bsr.w, mus_TitleScreen		; play title screen music
    //		move.b	#0,(f_debug_enable).w			; disable debug mode

    countdown__ = 0x178;

    // 	if FixBugs
    //		clearRAM v_sonicteam,v_sonicteam+object_size
    //	else
    //		; Bug: this only clears half of the "SONIC TEAM PRESENTS" slot.
    //		; This is responsible for why the "PRESS START BUTTON" text doesn't
    //		; show up, as the routine ID isn't reset.
    //		clearRAM v_sonicteam,v_sonicteam+object_size/2
    //	endif

    // move.b	#id_TitleSonic,(v_titlesonic).w ; load big Sonic object
    //		move.b	#id_PSBTM,(v_pressstart).w ; load "PRESS START BUTTON" object
    //		;clr.b	(v_pressstart+obRoutine).w ; The 'Mega Games 10' version of Sonic 1 added this line, to fix the 'PRESS START BUTTON' object not appearing
    //
    //		if Revision<>0
    //			tst.b   (v_megadrive).w	; is console Japanese?
    //			bpl.s   .isjap		; if yes, branch
    //		endif
    //
    //		move.b	#id_PSBTM,(v_titletm).w ; load "TM" object
    //		move.b	#3,(v_titletm+obFrame).w
    //.isjap:
    //		move.b	#id_PSBTM,(v_ttlsonichide).w ; load object which hides part of Sonic
    //		move.b	#2,(v_ttlsonichide+obFrame).w
    //		jsr	(ExecuteObjects).l
    //		bsr.w	DeformLayers
    //		jsr	(BuildSprites).l
    //		moveq	#plcid_Main,d0
    //		bsr.w	NewPLC
    //		move.w	#0,(v_title_dcount).w
    //		move.w	#0,(v_title_ccount).w
    //		move.w	(v_vdp_buffer1).w,d0
    //		ori.b	#$40,d0
    //		move.w	d0,(vdp_control_port).l
    //		bsr.w	PaletteFadeIn

    // Tit_MainLoop:
    game_vdp__set_vblank_routine_counter(4);
    game_vdp__wait_for_vblank();

    // jsr	(ExecuteObjects).l
    //		bsr.w	DeformLayers
    //		jsr	(BuildSprites).l
    //		bsr.w	PalCycle_Title
    //		bsr.w	RunPLC
    //		move.w	(v_player+obX).w,d0
    //		addq.w	#2,d0
    //		move.w	d0,(v_player+obX).w ; move Sonic to the right
    //		cmpi.w	#$1C00,d0	; has Sonic object passed $1C00 on x-axis?
    //		blo.s	Tit_ChkRegion	; if not, branch
    //
    //		move.b	#id_Sega,(v_gamemode).w ; go to Sega screen
}