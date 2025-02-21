#ifndef __CLIONPROJECTS_SONIC_ANYWHERE_SRC2_VARIABLES_H_
#define __CLIONPROJECTS_SONIC_ANYWHERE_SRC2_VARIABLES_H_

#include "constants.h"
#include "include_backend/tinyint.h"

typedef struct S1Variables {
    u8 v_256x256[0x52 * 0x200]; // 256x256 tile mappings ($52 chunks)
    
    u8 v_lvllayout[0x400]; // level and background layouts

    u8 v_bgscroll_buffer[0x200]; // background scroll buffer
    u8 v_ngfx_buffer[0x200];     // Nemesis graphics decompression buffer
    u8 v_spritequeue[0x400]; // sprite display queue, in order of priority
    u8 v_16x16[0x1800];      // 16x16 tile mappings

    u8 v_sgfx_buffer[tile_size * 23]; // buffered Sonic graphics ($17 cells)

    u8 v_unused_l17[0x20];              // unused
    u8 v_tracksonic[0x100];         // position tracking data for Sonic
    u8 v_hscrolltablebuffer[0x380]; // scrolling table data
    u8 unused_l20[0x80];		// would be unused, but data from v_hscrolltablebuffer can spill into here

    u8 v_objspace[object_size * 0x80]; // object variable space ($40 bytes per object)

    u8 unused_l24[0x40];		// unused

    u8 v_gamemode; // game mode (00=Sega; 04=Title; 08=Demo; 0C=Level; 10=SS; 14=Cont; 18=End; 1C=Credit; +8C=PreLevel)
    u8 unused_l27;		// unused
    u8 v_jpadhold2;  // joypad input - held, duplicate
    u8 v_jpadpress2; // joypad input - pressed, duplicate
    u8 v_jpadhold1;  // joypad input - held
    u8 v_jpadpress1; // joypad input - pressed
    u8 unused_l32[6]; // unused
    
    u16 v_vdp_buffer1;		// VDP instruction buffer
    u8 unused_l35[6];		// unused
    u16 v_demolength;    // the length of a demo in frames
    u16 v_scrposy_vdp;   // screen position y (VDP)
    u16 v_bgscrposy_vdp; // background screen position y (VDP)
    u16 v_scrposx_vdp;   // screen position x (VDP)
    u16 v_bgscrposx_vdp; // background screen position x (VDP)
    u16 v_bg3scrposy_vdp;
    u16 v_bg3scrposx_vdp;
    u8	unused_l43[2];		// unused
    u16 v_hbla_hreg; // VDP H.interrupt register buffer (8Axx)
    
    u8 v_pfade_start; // palette fading - start position in bytes
    u8 v_pfade_size;  // palette fading - number of colours

    u8 v_vbla_0e_counter; // tracks how many times vertical interrupts routine 0E occured (pretty much unused because routine 0E is unused)
    u8 unused_l50;		// unused
    u8 v_vbla_routine; // VBlank - routine counter
    u8 unused_l52; //		; unused
    u8 v_spritecount; // number of sprites on-screen
    u8 unused_l54[5]; //		; unused
    u16 v_pcyc_num;  // palette cycling - current reference number
    u16 v_pcyc_time; // palette cycling - time until the next change
    u32 v_random;    // pseudo random number buffer
    u16 f_pause;     // flag set to pause the game
    u8	unused_l59[4]; //		; unused
    u16 v_vdp_buffer2; // VDP instruction buffer
    u8	unused_l61[2]; //		; unused
    u16 f_hbla_pal;         // flag set to change palette during HBlank (0000 = no; 0001 = change)
    u16 v_waterpos1;        // water height, actual
    u16 v_waterpos2;        // water height, ignoring sway
    u16 v_waterpos3;        // water height, next target
    u8 f_water;             // flag set for water
    u8 v_wtr_routine;       // water event - routine counter
    u8 f_wtr_state;         // water palette state when water is above/below the screen (00 = partly/all dry; 01 = all
                            // underwater)
    u8 f_doupdatesinhblank; // defers performing various tasks to the Horizontal Interrupt (H-Blank)
    u8 v_pal_buffer[0x30];  // palette data buffer (used for palette cycling)

    u8 v_plc_buffer[6 * 16]; // pattern load cues buffer (maximum $10 PLCs)
  
    u32 v_plc_ptrnemcode; // pointer for nemesis decompression code ($1502 or $150C)
    u32 v_plc_repeatcount;
    u32 v_plc_paletteindex;
    u32 v_plc_previousrow;
    u32 v_plc_dataword;
    u32 v_plc_shiftvalue;
    u16 v_plc_patternsleft;
    u16 v_plc_framepatternsleft;
    u8	unused_l83[4];		// ; unused

    // Variables that are reset between levels
    u32 v_screenposx;   // screen position x
    u32 v_screenposy;   // screen position y
    u32 v_bgscreenposx; // background screen position x
    u32 v_bgscreenposy; // background screen position y
    u32 v_bg2screenposx;
    u32 v_bg2screenposy;
    u32 v_bg3screenposx;
    u32 v_bg3screenposy;


    u16 v_limitleft1;       // left level boundary (unused)
    u16 v_limitright1;      // right level boundary (unused)
    u16 v_limittop1;        // top level boundary (unused)
    u16 v_limitbtm1;   // bottom level boundary
    u16 v_limitleft2;  // left level boundary
    u16 v_limitright2; // right level boundary
    u16 v_limittop2;   // top level boundary
    u16 v_limitbtm2;   // bottom level boundary
    u16 v_unused11;         // unused
    u16 v_limitleft3; // left level boundary, at the end of an act
    u8 unused_l106[6];                      // unused
    u16 v_scrshiftx; // x-screen shift (new - last) * $100
    u16 v_scrshifty; // y-screen shift (new - last) * $100
    u16 v_lookshift; // screen shift when Sonic looks up/down
    u8 v_unused7;           // unused
    u8 v_unused8;           // unused
    u8 v_dle_routine; // dynamic level event - routine counter
    u8 unused_l113;                      // unused
    u8 f_nobgscroll; // flag set to cancel background scrolling
    u8 unused_l115;                      // unused
    u8 v_unused9;           // unused
    u8 v_unused117;                      // unused
    u8 v_unused10;          // unused
    u8 unused_l119;                      // unused
    u8 v_fg_xblock;  // foreground x-block parity (for redraw)
    u8 v_fg_yblock;  // foreground y-block parity (for redraw)
    u8 v_bg1_xblock; // background x-block parity (for redraw)
    u8 v_bg1_yblock; // background y-block parity (for redraw)
    u8 v_bg2_xblock; // secondary background x-block parity (for redraw)
    u8 v_bg2_yblock;        // secondary background y-block parity (unused)
    u8 v_bg3_xblock; // teritary background x-block parity (for redraw)
    u8 v_bg3_yblock;        // teritary background y-block parity (unused)
    u8 unused_l128[2];                      // unused
    u16 v_fg_scroll_flags;  // screen redraw flags for foreground
    u16 v_bg1_scroll_flags; // screen redraw flags for background 1
    u16 v_bg2_scroll_flags; // screen redraw flags for background 2
    u16 v_bg3_scroll_flags; // screen redraw flags for background 3
    u8 f_bgscrollvert;      // flag for vertical background scrolling
    u8 unused_l134[3];                      // unused
    u16 v_sonspeedmax; // Sonic's maximum speed
    u16 v_sonspeedacc; // Sonic's acceleration
    u16 v_sonspeeddec; // Sonic's deceleration
    u8 v_sonframenum;  // frame to display for Sonic
    u8 f_sonframechg;  // flag set to update Sonic's sprite frame
    u8 v_anglebuffer;  // angle of collision block that Sonic or object is standing on
    u8 unused_l141;                      // unused
    u8 v_anglebuffer2; // other angle of collision block that Sonic or object is standing on
    u8 unused_l143;                      // unused
    u8 v_opl_routine; // ObjPosLoad - routine counter
    u8 unused_l145;                      // unused
    u16 v_opl_screen; // ObjPosLoad - screen variable
    u8 v_opl_data[0x10]; // ObjPosLoad - data buffer
    u16 v_ssangle;    // Special Stage angle
    u16 v_ssrotate;   // Special Stage rotation speed
    u8 unused_l150[0xC];                     // unused
    u16 v_btnpushtime1; // button push duration - in level
    u16 v_btnpushtime2; // button push duration - in demo
    u16 v_palchgspeed;  // palette fade/transition speed (0 is fastest)
    u32 v_collindex;    // ROM address for collision index of current level
    u16 v_palss_num;    // palette cycling in Special Stage - reference number
    u16 v_palss_time;   // palette cycling in Special Stage - time until next change
    u16 v_palss_index;      // palette cycling in Special Stage - index into palette cycle 2 (unused?)
    u16 v_ssbganim; // Special Stage background animation
    u8 unused_l159[2];                      // unused
    u16 v_obj31ypos; // y-position of object 31 (MZ stomper)
    u8 unused_l161;                      // unused
    u8 v_bossstatus; // status of boss and prison capsule (01 = boss defeated;// 02 = prison opened)
    u16 v_trackpos;  // position tracking reference number
    
    u8 f_lockscreen; // flag set to lock screen during bosses
    u8 unused_l166;                 // unused
    u8 v_256loop1;    // 256x256 level tile which contains a loop (GHZ/SLZ)
    u8 v_256loop2;    // 256x256 level tile which contains a loop (GHZ/SLZ)
    u8 v_256roll1;    // 256x256 level tile which contains a roll tunnel (GHZ)
    u8 v_256roll2;    // 256x256 level tile which contains a roll tunnel (GHZ)
    u8 v_lani0_frame; // level graphics animation 0 - current frame
    u8 v_lani0_time;  // level graphics animation 0 - time until next frame
    u8 v_lani1_frame; // level graphics animation 1 - current frame
    u8 v_lani1_time;  // level graphics animation 1 - time until next frame
    u8 v_lani2_frame; // level graphics animation 2 - current frame
    u8 v_lani2_time;  // level graphics animation 2 - time until next frame
    u8 v_lani3_frame; // level graphics animation 3 - current frame
    u8 v_lani3_time;  // level graphics animation 3 - time until next frame
    u8 v_lani4_frame; // level graphics animation 4 - current frame
    u8 v_lani4_time;  // level graphics animation 4 - time until next frame
    u8 v_lani5_frame; // level graphics animation 5 - current frame
    u8 v_lani5_time;  // level graphics animation 5 - time until next frame
    u8 unused_l183[2];                 // unused
    u16 v_gfxbigring;  // settings for giant ring graphics loading
    u8 f_conveyrev;    // flag set to reverse conveyor belts in LZ/SBZ
    u8 v_obj63[6];        // object 63 (LZ/SBZ platforms) variables
    u8 f_wtunnelmode;  // LZ water tunnel mode
    u8 f_playerctrl;   // Player control override flags (object ineraction, control enable)
    u8 f_wtunnelallow; // LZ water tunnels (00 = enabled;// 01 = disabled)
    u8 f_slidemode;    // LZ water slide mode
    u8 v_obj6B;        // object 6B (SBZ stomper) variable
    u8 f_lockctrl;     // flag set to lock controls during ending sequence
    u8 f_bigring;      // flag set when Sonic collects the giant ring
    u8 f_obj56;        // object 56 flag
    u8 unused_l195;                 // unused
    u16 v_itembonus;  // item bonus from broken enemies, blocks etc.
    u16 v_timebonus;  // time bonus at the end of an act
    u16 v_ringbonus;  // ring bonus at the end of an act
    u8 f_endactbonus; // time/ring bonus update flag at the end of an act
    u8 v_sonicend;    // routine counter for Sonic in the ending sequence
    u16 v_lz_deform;  // LZ deformation offset, in units of $80
    u8 unused_l202[6];                // unused
    u8 f_switch[0x10]; // flags set when Sonic stands on a switch
    u16 v_scroll_block_1_size;
    u16 v_scroll_block_2_size;		;// unused
    u16 v_scroll_block_3_size; // unused
    u16 v_scroll_block_4_size;   // unused
    u8 unused_l208[8];                           // unused
    u8 v_spritetablebuffer[0x280]; // sprite table (last $80 bytes are overwritten by v_palette_water_fading)

    u8 v_palette_water[0x80]; // main underwater palette
   
    u8 v_palette[0x80]; // main palette
   
    u8 v_palette_fading[0x80]; // duplicate palette, used for transitions
    
    u8 v_objstate[0xC0]; // object state list
    u8 stack[0x140];
    
    // RAM beyond this point is only cleared on a cold-boot
    u8 unused_l221[2];                // unused
    u16 f_restart;    // restart level flag
    u16 v_framecount; // frame counter (adds 1 every frame)

    u8 v_debugitem; // debug item currently selected (NOT the object number of the item)
    u8 unused_l226;                    // unused
    u16 v_debuguse;       // debug mode use & routine counter (when Sonic is a ring/item)
    u8 v_debugspeedtimer; // debug mode - timer before movement starts
    u8 v_debugspeed;      // debug mode - movement speed
    u32 v_vbla_count;     // vertical interrupt counter (adds 1 every VBlank)
    
    u8 v_zone;  // current zone number
    u8 v_act;   // current act number
    u8 v_lives; // number of lives
    u8 unused_l235;          // unused
    u16 v_air; // air remaining while underwater
    u8 v_lastspecial; // last special stage number
    u8 unused_l238;                // unused
    u8 v_continues; // number of continues
    u8 unused_l240;                // unused
    u8 f_timeover;   // time over flag
    u8 v_lifecount;  // lives counter value (for actual number, see "v_lives")
    u8 f_lifecount;  // lives counter update flag
    u8 f_ringcount;  // ring counter update flag
    u8 f_timecount;  // time counter update flag
    u8 f_scorecount; // score counter update flag
    u16 v_rings;     // rings
    
    u32 v_time; // time
    u32 v_score; // score
    u8 unused_l251[2];            // unused
    u8 v_shield; // shield status (00 = no;// 01 = yes)
    u8 v_invinc; // invinciblity status (00 = no;// 01 = yes)
    u8 v_shoes;  // speed shoes status (00 = no;// 01 = yes)
    u8 v_unused1; // an unused fourth player status (Goggles?)

    u8 v_lastlamp[2];    // number of the last lamppost you hit
    u16 v_lamp_xpos;  // x-axis for Sonic to respawn at lamppost
    u16 v_lamp_ypos;  // y-axis for Sonic to respawn at lamppost
    u16 v_lamp_rings; // rings stored at lamppost
    u32 v_lamp_time;  // time stored at lamppost
    u8 v_lamp_dle;    // dynamic level event routine counter at lamppost
    u8 unused_l263;                   // unused
    u16 v_lamp_limitbtm; // level bottom boundary at lamppost
    u16 v_lamp_scrx;     // x-axis screen at lamppost
    u16 v_lamp_scry;     // y-axis screen at lamppost
    u16 v_lamp_bgscrx;   // x-axis BG screen at lamppost
    u16 v_lamp_bgscry;   // y-axis BG screen at lamppost
    u16 v_lamp_bg2scrx;  // x-axis BG2 screen at lamppost
    u16 v_lamp_bg2scry;  // y-axis BG2 screen at lamppost
    u16 v_lamp_bg3scrx;  // x-axis BG3 screen at lamppost
    u16 v_lamp_bg3scry;  // y-axis BG3 screen at lamppost
    u16 v_lamp_wtrpos;   // water position at lamppost
    u8 v_lamp_wtrrout;   // water routine at lamppost
    u8 v_lamp_wtrstat;   // water state at lamppost
    u8 v_lamp_lives;     // lives counter at lamppost
    u8 unused_l277[2];                   // unused
    u8 v_emeralds;   // number of chaos emeralds
    u8 v_emldlist[6];   // which individual emeralds you have (00 = no;// 01 = yes)
    u16 v_oscillate; // oscillation bitfield 
    
    u8 v_timingvariables[0x40 + 0x20];

    u8 v_ani0_time;  // synchronised sprite animation 0 - time until next frame (used for synchronised animations)
    u8 v_ani0_frame; // synchronised sprite animation 0 - current frame
    u8 v_ani1_time;  // synchronised sprite animation 1 - time until next frame
    u8 v_ani1_frame; // synchronised sprite animation 1 - current frame
    u8 v_ani2_time;  // synchronised sprite animation 2 - time until next frame
    u8 v_ani2_frame; // synchronised sprite animation 2 - current frame
    u8 v_ani3_time;  // synchronised sprite animation 3 - time until next frame
    u8 v_ani3_frame; // synchronised sprite animation 3 - current frame
    u16 v_ani3_buf;  // synchronised sprite animation 3 - info buffer
    u8 unused_l293[0x26];              // unused
    u16 v_limittopdb; // level upper boundary, buffered for debug mode
    u16 v_limitbtmdb; // level bottom boundary, buffered for debug mode
    u8 unused_l296[0xC];               // unused
    u16 v_chunk0collision; // very subtly (and perhaps unintentionally) used by FindNearestTile when encountering chunk 0

    u8 unused_l299[0xE];                     // unused
    u32 v_screenposx_dup;   // screen position x (duplicate)
    u32 v_screenposy_dup;   // screen position y (duplicate)
    u32 v_bgscreenposx_dup; // background screen position x (duplicate)
    u32 v_bgscreenposy_dup; // background screen position y (duplicate)
    u32 v_bg2screenposx_dup;
    u32 v_bg2screenposy_dup;
    u32 v_bg3screenposx_dup;
    u32 v_bg3screenposy_dup;
    u16 v_fg_scroll_flags_dup;
    u16 v_bg1_scroll_flags_dup;
    u16 v_bg2_scroll_flags_dup;
    u16 v_bg3_scroll_flags_dup;
    u8 unused_l312[0x48]; // unused
    u16 v_levseldelay; // level select - time until change when up/down is held
    u16 v_levselitem;  // level select - item selected
    u16 v_levselsound; // level select - sound selected
    u8 unused_l316[0x3A];                              // unused
    
    // 	if Revision=0
    // u32 v_scorecopy; // score, duplicate
    // 	else
    u32 v_scorelife; // points required for an extra life (JP1 only)
    // 	endif
    
    u8 unused_l324[0x1C];                // unused
    u8 f_levselcheat;   // level select cheat flag
    u8 f_slomocheat;    // slow motion & frame advance cheat flag
    u8 f_debugcheat;    // debug mode cheat flag
    u8 f_creditscheat;  // hidden credits & press start cheat flag
    u16 v_title_dcount; // number of times the d-pad is pressed on title screen
    u16 v_title_ccount; // number of times C is pressed on title screen
    u8 unused_l331[2];                  // unused
    u16 v_unused2; // unused
    u8 v_unused3;  // unused
    u8 v_unused4;  // unused
    u8 v_unused5;  // unused
    u8 v_unused6;  // unused

    u16 f_demo;    // demo mode flag (0 = no;// 1 = yes;// $8001 = ending)
    u16 v_demonum; // demo level number (not the same as the level number)
    u16 v_creditsnum;		// credits index number
    u8 unused_l341[2];               // unused
    u8 v_megadrive; // Megadrive machine type
    u8 unused_l343;               // unused
    u16 f_debugmode; // debug mode flag
    u32 v_init;      // 'init' text string

    // ;// Error handler
    // 	phase v_objstate
    // u8 v_regbuffer[0x40]; // stores registers d0-a7 during an error event
    // u32 v_spbuffer;    // stores most recent sp address
    // u8 v_errortype;    // error type
    // 	dephase
} S1Variables;

typedef struct S1VariablePointers {
    u8* v_ram_start;

    u8* v_256x256_end;
    u8* v_lvllayout_end;
    u8* v_ngfx_buffer_end;
    u8* v_sgfx_buffer_end;
    u8* v_hscrolltablebuffer_end;
    u8* v_hscrolltablebuffer_end_padded;

    // ; Title screen objects
    u8* v_sonicteam; //	= v_objspace+object_size*2	; object variable space for the "SONIC TEAM PRESENTS" text ($40 bytes)
    u8* v_titlesonic; //	= v_objspace+object_size*1	; object variable space for Sonic in the title screen ($40 bytes)
    u8* v_pressstart; //	= v_objspace+object_size*2	; object variable space for the "PRESS START BUTTON" text ($40 bytes)
    u8* v_titletm; //	= v_objspace+object_size*3	; object variable space for the trademark symbol ($40 bytes)
    u8* v_ttlsonichide; //	= v_objspace+object_size*4	; object variable space for hiding part of Sonic ($40 bytes)

    // ; Level objects
    u8* v_player; //	= v_objspace+object_size*0	; object variable space for Sonic ($40 bytes)
    u8* v_hud; //		= v_objspace+object_size*1	; object variable space for the HUD ($40 bytes)

    u8* v_titlecard; //	= v_objspace+object_size*2	; object variable space for the title card ($100 bytes)
    u8* v_ttlcardname; //	= v_titlecard+object_size*0		; object variable space for the title card zone name text ($40 bytes) 
    u8* v_ttlcardzone;	//= v_titlecard+object_size*1	; object variable space for the title card "ZONE" text ($40 bytes) 
    u8* v_ttlcardact;	//= v_titlecard+object_size*2	; object variable space for the title card act text ($40 bytes)
    u8* v_ttlcardoval; //	= v_titlecard+object_size*3	; object variable space for the title card oval ($40 bytes)

    u8* v_gameovertext1; //	= v_objspace+object_size*2	; object variable space for the "GAME"/"TIME" in "GAME OVER"/"TIME OVER" text ($40 bytes) 
    u8* v_gameovertext2;	//= v_objspace+object_size*3	; object variable space for the "OVER" in "GAME OVER"/"TIME OVER" text ($40 bytes)

    u8* v_shieldobj; //	= v_objspace+object_size*6	; object variable space for the shield ($40 bytes)
    u8* v_starsobj1; //	= v_objspace+object_size*8	; object variable space for the invincibility stars #1 ($40 bytes)
    u8* v_starsobj2; //	= v_objspace+object_size*9	; object variable space for the invincibility stars #2 ($40 bytes)
    u8* v_starsobj3; //	= v_objspace+object_size*10	; object variable space for the invincibility stars #3 ($40 bytes)
    u8* v_starsobj4; //	= v_objspace+object_size*11	; object variable space for the invincibility stars #4 ($40 bytes)

    u8* v_splash; //	= v_objspace+object_size*12	; object variable space for the water splash ($40 bytes)
    u8* v_sonicbubbles; //	= v_objspace+object_size*13	; object variable space for the bubbles that come out of Sonic's mouth/drown countdown ($40 bytes) 
    u8* v_watersurface1;	//= v_objspace+object_size*30	; object variable space for the water surface #1 ($40 bytes) 
    u8* v_watersurface2;	//= v_objspace+object_size*31	; object variable space for the water surface #1 ($40 bytes)

    u8* v_endcard; //	= v_objspace+object_size*23	; object variable space for the level results card ($1C0 bytes)
    u8* v_endcardsonic; //	= v_endcard+object_size*0	; object variable space for the level results card "SONIC HAS" text ($40 bytes) 
    u8* v_endcardpassed;//= v_endcard+object_size*1	; object variable space for the level results card "PASSED" text ($40 bytes) 
    u8* v_endcardact;	//= v_endcard+object_size*2	; object variable space for the level results card act text ($40 bytes) 
    u8* v_endcardscore;	//= v_endcard+object_size*3	; object variable space for the level results card score totally ($40 bytes) 
    u8* v_endcardtime;	//= v_endcard+object_size*4	; object variable space for the level results card time bonus tally ($40 bytes) 
    u8* v_endcardring;  //= v_endcard+object_size*5	; object variable space for the level results card ring bonus tally ($40 bytes) 
    u8* v_endcardoval;	//= v_endcard+object_size*6	; object variable space for the level results card oval ($40 bytes)

    u8* v_lvlobjspace; //	= v_objspace+object_size*32	; level object variable space ($1800 bytes)
    u8* v_lvlobjend; //	= v_lvlobjspace+object_size*96
    u8* v_objspace_end; //	= v_lvlobjend

    // ; Special Stage objects
    u8* v_ssrescard; //	= v_objspace+object_size*23	; object variable space for the Special Stage results card ($140 bytes)
    u8* v_ssrestext; //	= v_ssrescard+object_size*0	; object variable space for the Special Stage results card text ($40 bytes) 
    u8* v_ssresscore; //	= v_ssrescard+object_size*1	; object variable space for the Special Stage results card score tally ($40 bytes) 
    u8* v_ssresring; //	= v_ssrescard+object_size*2	; object variable space for the Special Stage results card ring bonus tally ($40 bytes) 
    u8* v_ssresoval; //	= v_ssrescard+object_size*3	; object variable space for the Special Stage results card oval ($40 bytes) 
    u8* v_ssrescontinue; //	= v_ssrescard+object_size*4	; object variable space for the Special Stage results card continue icon ($40 bytes) 
    u8* v_ssresemeralds; //	= v_objspace+object_size*32 object variable space for the emeralds in the Special Stage results ($180 bytes)

    // ; Continue screen objects
    u8* v_continuetext; //	= v_objspace+object_size*1	; object variable space for the continue screen text ($40 bytes)
    u8* v_continuelight; //	= v_objspace+object_size*2	; object variable space for the continue screen light spot ($40 bytes) 
    u8* v_continueicon; //	= v_objspace+object_size*3	; object variable space for the continue screen icon ($40 bytes)

    // ; Ending objects
    u8* v_endemeralds;      //	= v_objspace+object_size*16	; object variable space for the emeralds in the ending ($180 bytes)
    u8* v_endemeralds_end;  //	= v_objspace+object_size*32
    u8* v_endlogo;          //	= v_objspace+object_size*16	; object variable space for the logo in the ending ($40 bytes)

    // ; Credits objects
    u8* v_credits; //	= v_objspace+object_size*2	; object variable space for the credits text ($40 bytes)
    u8* v_endeggman; //	= v_objspace+object_size*2	; object variable space for Eggman after the credits ($40 bytes)
    u8* v_tryagain; //	= v_objspace+object_size*3	; object variable space for the "TRY AGAIN" text ($40 bytes)
    u8* v_eggmanchaos; //	= v_objspace+object_size*32	; object variable space for the emeralds juggled by Eggman ($180 bytes)

    // u8* v_snddriver_ram =	SMPS_RAM		; sound driver state

    u8* v_hbla_line; //= v_hbla_hreg+1			// screen line where water starts and palette is changed by HBlank
   
    u8* v_misc_variables;
    u8* v_misc_variables_end;

    u8* v_plc_buffer_only_end;

    u8* v_plc_buffer_end;

    u8* v_levelvariables; //				    ; variables that are reset between levels

    u8* v_trackbyte; // = v_trackpos+1			;// low byte for position tracking

    u8* v_levelvariables_end;

    u8* v_spritetablebuffer_end;

    u8* v_palette_water_fading; // duplicate underwater palette, used for transitions ($80 bytes)

    u8* v_palette_water_end;
    u8* v_palette_end;

    u8* v_palette_fading_end;
    u8* v_objstate_end;

    u8* v_systemstack;
    u8* v_crossresetram; // RAM beyond this point is only cleared on a cold-boot

    u8* v_framebyte; // = v_framecount+1			;// low byte for frame counter

    u8* v_vbla_word;
    u8* v_vbla_byte;

    u8* v_airbyte;

    u8* v_ringbyte;


    u8* v_timemin;  // time - minutes
    u8* v_timesec;  // time - seconds
    u8* v_timecent; // time - centiseconds

    u8* v_timingandscreenvariables;

    u8* v_timingvariables_end;

    u8* v_ram_end;

    // Special stage
    u8* v_ssbuffer1;		//= v_256x256
    u8* v_ssblockbuffer;		//= v_ssbuffer1+$1020 ;// ($2000 bytes)
    u8* v_ssblockbuffer_end;	//= v_ssblockbuffer+$80*$40
    u8* v_ssbuffer2;		//= v_256x256+$4000
    u8* v_ssblocktypes;		//= v_ssbuffer2
    u8* v_ssitembuffer;		//= v_ssbuffer2+$400 ;// ($100 bytes)
    u8* v_ssitembuffer_end;	//= v_ssitembuffer+$100
    u8* v_ssbuffer3;		//= v_256x256+$8000
    u8* v_ssscroll_buffer;	//= v_ngfx_buffer+$100
} S1VariablePointers;

S1VariablePointers* s1_getVariablePointers();
S1Variables* s1_getVariables();

#endif // __CLIONPROJECTS_SONIC_ANYWHERE_SRC2_VARIABLES_H_


