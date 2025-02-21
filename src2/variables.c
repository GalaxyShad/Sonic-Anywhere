#include "variables.h"

static S1VariablePointers ptrs_;
static S1Variables vars_;

static void fillPointers__(S1Variables* v) {
    ptrs_.v_ram_start = (u8*)v;

    ptrs_.v_256x256_end = v->v_256x256 + (0x52*0x200);
    ptrs_.v_lvllayout_end = v->v_lvllayout + 0x400;
    ptrs_.v_ngfx_buffer_end = v->v_ngfx_buffer + 0x200;
    ptrs_.v_sgfx_buffer_end = v->v_sgfx_buffer + tile_size*23;
    ptrs_.v_hscrolltablebuffer_end = v->v_hscrolltablebuffer + 0x380;
    ptrs_.v_hscrolltablebuffer_end_padded = v->v_hscrolltablebuffer + 0x380 + 0x80;

    // Title screen objects
    ptrs_.v_sonicteam	= v->v_objspace+object_size*2;	// object variable space for the "SONIC TEAM PRESENTS" text ($40 bytes)
    ptrs_.v_titlesonic	= v->v_objspace+object_size*1;	// object variable space for Sonic in the title screen ($40 bytes)
    ptrs_.v_pressstart	= v->v_objspace+object_size*2;	// object variable space for the "PRESS START BUTTON" text ($40 bytes)
    ptrs_.v_titletm	= v->v_objspace+object_size*3;	// object variable space for the trademark symbol ($40 bytes)
    ptrs_.v_ttlsonichide	= v->v_objspace+object_size*4;	// object variable space for hiding part of Sonic ($40 bytes)

    // Level objects
    ptrs_.v_player	= v->v_objspace+object_size*0;	// object variable space for Sonic ($40 bytes)
    ptrs_.v_hud		= v->v_objspace+object_size*1;	// object variable space for the HUD ($40 bytes)

    ptrs_.v_titlecard	= v->v_objspace+object_size*2;	// object variable space for the title card ($100 bytes)
    ptrs_.v_ttlcardname	= ptrs_.v_titlecard+object_size*0;		// object variable space for the title card zone name text ($40 bytes)
    ptrs_.v_ttlcardzone	= ptrs_.v_titlecard+object_size*1;	// object variable space for the title card "ZONE" text ($40 bytes)
    ptrs_.v_ttlcardact	= ptrs_.v_titlecard+object_size*2;	// object variable space for the title card act text ($40 bytes)
    ptrs_.v_ttlcardoval	= ptrs_.v_titlecard+object_size*3;	// object variable space for the title card oval ($40 bytes)

    ptrs_.v_gameovertext1	= v->v_objspace+object_size*2;	// object variable space for the "GAME"/"TIME" in "GAME OVER"/"TIME OVER" text ($40 bytes)
    ptrs_.v_gameovertext2	= v->v_objspace+object_size*3;	// object variable space for the "OVER" in "GAME OVER"/"TIME OVER" text ($40 bytes)

    ptrs_.v_shieldobj	= v->v_objspace+object_size*6;	// object variable space for the shield ($40 bytes)
    ptrs_.v_starsobj1	= v->v_objspace+object_size*8;	// object variable space for the invincibility stars #1 ($40 bytes)
    ptrs_.v_starsobj2	= v->v_objspace+object_size*9;	// object variable space for the invincibility stars #2 ($40 bytes)
    ptrs_.v_starsobj3	= v->v_objspace+object_size*10;	// object variable space for the invincibility stars #3 ($40 bytes)
    ptrs_.v_starsobj4	= v->v_objspace+object_size*11;	// object variable space for the invincibility stars #4 ($40 bytes)

    ptrs_.v_splash	= v->v_objspace+object_size*12;	// object variable space for the water splash ($40 bytes)
    ptrs_.v_sonicbubbles	= v->v_objspace+object_size*13;	// object variable space for the bubbles that come out of Sonic's mouth/drown countdown ($40 bytes)
    ptrs_.v_watersurface1	= v->v_objspace+object_size*30;	// object variable space for the water surface #1 ($40 bytes)
    ptrs_.v_watersurface2	= v->v_objspace+object_size*31;	// object variable space for the water surface #1 ($40 bytes)

    ptrs_.v_endcard	= v->v_objspace+object_size*23;	// object variable space for the level results card ($1C0 bytes)
    ptrs_.v_endcardsonic	= ptrs_.v_endcard+object_size*0;	// object variable space for the level results card "SONIC HAS" text ($40 bytes)
    ptrs_.v_endcardpassed	= ptrs_.v_endcard+object_size*1;	// object variable space for the level results card "PASSED" text ($40 bytes)
    ptrs_.v_endcardact	= ptrs_.v_endcard+object_size*2;	// object variable space for the level results card act text ($40 bytes)
    ptrs_.v_endcardscore	= ptrs_.v_endcard+object_size*3;	// object variable space for the level results card score tally ($40 bytes)
    ptrs_.v_endcardtime	= ptrs_.v_endcard+object_size*4;	// object variable space for the level results card time bonus tally ($40 bytes)
    ptrs_.v_endcardring	= ptrs_.v_endcard+object_size*5;	// object variable space for the level results card ring bonus tally ($40 bytes)
    ptrs_.v_endcardoval	= ptrs_.v_endcard+object_size*6;	// object variable space for the level results card oval ($40 bytes)

    ptrs_.v_lvlobjspace	= v->v_objspace+object_size*32;	// level object variable space ($1800 bytes)
    ptrs_.v_lvlobjend	= ptrs_.v_lvlobjspace+object_size*96;
    ptrs_.v_objspace_end	= ptrs_.v_lvlobjend;

    // Special Stage objects
    ptrs_.v_ssrescard	= v->v_objspace+object_size*23;	// object variable space for the Special Stage results card ($140 bytes)
    ptrs_.v_ssrestext	= ptrs_.v_ssrescard+object_size*0;	// object variable space for the Special Stage results card text ($40 bytes)
    ptrs_.v_ssresscore	= ptrs_.v_ssrescard+object_size*1;	// object variable space for the Special Stage results card score tally ($40 bytes)
    ptrs_.v_ssresring	= ptrs_.v_ssrescard+object_size*2;	// object variable space for the Special Stage results card ring bonus tally ($40 bytes)
    ptrs_.v_ssresoval	= ptrs_.v_ssrescard+object_size*3;	// object variable space for the Special Stage results card oval ($40 bytes)
    ptrs_.v_ssrescontinue	= ptrs_.v_ssrescard+object_size*4;	// object variable space for the Special Stage results card continue icon ($40 bytes)
    ptrs_.v_ssresemeralds	= v->v_objspace+object_size*32;	// object variable space for the emeralds in the Special Stage results ($180 bytes)

    // Continue screen objects
    ptrs_.v_continuetext	= v->v_objspace+object_size*1;	// object variable space for the continue screen text ($40 bytes)
    ptrs_.v_continuelight	= v->v_objspace+object_size*2;	// object variable space for the continue screen light spot ($40 bytes)
    ptrs_.v_continueicon	= v->v_objspace+object_size*3;	// object variable space for the continue screen icon ($40 bytes)

    // Ending objects
    ptrs_.v_endemeralds	= v->v_objspace+object_size*16;	// object variable space for the emeralds in the ending ($180 bytes)
    ptrs_.v_endemeralds_end	= v->v_objspace+object_size*32;
    ptrs_.v_endlogo	= v->v_objspace+object_size*16;	// object variable space for the logo in the ending ($40 bytes)

    // Credits objects
    ptrs_.v_credits	= v->v_objspace+object_size*2;	// object variable space for the credits text ($40 bytes)
    ptrs_.v_endeggman	= v->v_objspace+object_size*2;	// object variable space for Eggman after the credits ($40 bytes)
    ptrs_.v_tryagain	= v->v_objspace+object_size*3;	// object variable space for the "TRY AGAIN" text ($40 bytes)
    ptrs_.v_eggmanchaos	= v->v_objspace+object_size*32;	// object variable space for the emeralds juggled by Eggman ($180 bytes)

    ptrs_.v_hbla_line = (u8*)(&v->v_hbla_hreg)+1;

    ptrs_.v_misc_variables = &v->v_vbla_0e_counter;
    ptrs_.v_misc_variables_end = v->v_pal_buffer + 0x30 + 1;

    ptrs_.v_plc_buffer_only_end = v->v_plc_buffer + 6*16;

    ptrs_.v_plc_buffer_end = (u8*)(&v->v_plc_framepatternsleft + 1) + 4;

    ptrs_.v_levelvariables = (u8*)&v->v_screenposx;

    ptrs_.v_trackbyte = (u8*)&v->v_trackpos + 1;

    ptrs_.v_levelvariables_end = v->v_spritetablebuffer;
    ptrs_.v_spritetablebuffer_end = v->v_spritetablebuffer + 0x280;

    ptrs_.v_palette_water_fading = ptrs_.v_spritetablebuffer_end - 0x80;

    ptrs_.v_palette_water_end = v->v_palette_water + 0x80;
    ptrs_.v_palette_end = v->v_palette + 0x80;

    ptrs_.v_palette_fading_end = v->v_palette_fading + 0x80;

    ptrs_.v_objstate_end = v->v_objstate + 0xC0;

    ptrs_.v_systemstack = (u8*)&v->f_restart - 2;
    ptrs_.v_crossresetram = (u8*)ptrs_.v_systemstack;

    ptrs_.v_framebyte = (u8*)&v->v_framecount + 1; // TODO Endian

    ptrs_.v_vbla_word = (u8*)&v->v_vbla_count + 2;
    ptrs_.v_vbla_byte = (u8*)ptrs_.v_vbla_word+1;

    ptrs_.v_airbyte = (u8*)&v->v_air + 1;

    ptrs_.v_ringbyte = (u8*)&v->v_rings + 1;

    ptrs_.v_timemin = (u8*)&v->v_time+1;
    ptrs_.v_timesec = (u8*)&v->v_time+2;
    ptrs_.v_timecent = (u8*)&v->v_time+3;

    ptrs_.v_timingvariables_end = (u8*)v->v_timingvariables;

    ptrs_.v_ram_end = (u8*)((u32*)&v->v_init + 1);

    // Special stage
    ptrs_.v_ssbuffer1 = v->v_256x256;
    ptrs_.v_ssblockbuffer = ptrs_.v_ssbuffer1 + 0x1020;
    ptrs_.v_ssblockbuffer_end = ptrs_.v_ssblockbuffer + 0x80 + 0x40;
    ptrs_.v_ssbuffer2 = v->v_256x256 + 0x4000;
    ptrs_.v_ssblocktypes = ptrs_.v_ssbuffer2;
    ptrs_.v_ssitembuffer = ptrs_.v_ssbuffer2 + 0x400;
    ptrs_.v_ssitembuffer_end = ptrs_.v_ssitembuffer+0x100;
    ptrs_.v_ssbuffer3 = v->v_256x256 + 0x8000;
    ptrs_.v_ssscroll_buffer = v->v_ngfx_buffer + 0x100;
}


S1VariablePointers* s1_getVariablePointers() {
    fillPointers__(&vars_);
    return &ptrs_;
}

S1Variables* s1_getVariables() {
    return &vars_;
}

