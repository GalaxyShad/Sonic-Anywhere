#ifndef __CLIONPROJECTS_SONIC_ANYWHERE_SRC2_CONSTANTS_H_
#define __CLIONPROJECTS_SONIC_ANYWHERE_SRC2_CONSTANTS_H_

// ---------------------------------------------------------------------------
// Constants
// ---------------------------------------------------------------------------

#define Size_of_SegaPCM             0x6978
#define Size_of_DAC_driver_guess    0x1760

// Clocks
#define Master_Clock        53693175
#define M68000_Clock        (Master_Clock/7)
#define Z80_Clock           (Master_Clock/15)
#define FM_Sample_Rate      (M68000_Clock/(6*6*4))
#define PSG_Sample_Rate     (Z80_Clock/16)

// VDP addressses
#define vdp_data_port       0xC00000
#define vdp_control_port	0xC00004
#define vdp_counter		    0xC00008

#define psg_input		    0xC00011

// Z80 addresses
#define z80_ram 		        0xA00000	// start of Z80 RAM
#define z80_ram_end 		    0xA02000	// end of non-reserved Z80 RAM
#define z80_version 		    0xA10001
#define z80_port_1_data 	    0xA10002
#define z80_port_1_control  	0xA10008
#define z80_port_2_control  	0xA1000A
#define z80_expansion_control   0xA1000C
#define z80_bus_rest 	        0xA11100
#define z80_reset   		    0xA11200
#define ym2612_a0   		    0xA04000
#define ym2612_d0   		    0xA04001
#define ym2612_a1   		    0xA04002
#define ym2612_d1   		    0xA04003

#define sram_port		        0xA130F1

#define security_addr		    0xA14000

// VRAM data
#define vram_fg             	0xC000	// foreground namespace
#define vram_bg             	0xE000	// background namespace
#define vram_sprites            0xF800	// sprite table
#define vram_hscroll            0xFC00	// horizontal scroll table

#define tile_size               (8*8/2)
#define plane_size_64x32        (64*32*2)

// Game modes
#define id_Sega 	            0x00
#define id_Title    	        0x04
#define id_Demo 	            0x08
#define id_Level    	        0x0C
#define id_Special  	        0x10
#define id_Continue 	        0x14
#define id_Ending   	        0x18
#define id_Credits  	        0x1C

// Levels
enum {
    id_GHZ  = 0,
    id_LZ   = 1,
    id_MZ   = 2,
    id_SLZ  = 3,
    id_SYZ  = 4,
    id_SBZ  = 5,
    id_EndZ = 6,
    id_SS   = 7
};


// Colours
#define  cBlack		0x000		    // colour black
#define  cWhite		0xEEE		    // colour white
#define  cBlue		0xE00		    // colour blue
#define  cGreen		0x0E0		    // colour green
#define  cRed		0x00E		    // colour red
#define  cYellow	(cGreen+cRed)   // colour yellow
#define  cAqua		(cGreen+cBlue)	// colour aqua
#define  cMagenta	(cBlue+cRed)	// colour magenta

// Joypad input
#define btnStart	0b10000000 // Start button	(0x80)
#define btnA		0b01000000 // A		(0x40)
#define btnC		0b00100000 // C		(0x20)
#define btnB		0b00010000 // B		(0x10)
#define btnR		0b00001000 // Right		(0x08)
#define btnL		0b00000100 // Left		(0x04)
#define btnDn		0b00000010 // Down		(0x02)
#define btnUp		0b00000001 // Up		(0x01)
#define btnDir		0b00001111 // Any direction	(0x0F)
#define btnABC		0b01110000 // A, B or C	(0x70)
#define bitStart	7
#define bitA		6
#define bitC		5
#define bitB		4
#define bitR		3
#define bitL		2
#define bitDn		1
#define bitUp		0

// Object variables
#define obID		0	        // object ID number
#define obRender	1	        // bitfield for x/y flip, display mode
#define obGfx		2	        // palette line & VRAM setting (2 bytes)
#define obMap		4	        // mappings address (4 bytes)
#define obX		    8	        // x-axis position (2-4 bytes)
#define obScreenY	0xA	        // y-axis position for screen-fixed items (2 bytes)
#define obY		    0xC	        // y-axis position (2-4 bytes)
#define obVelX		0x10	    // x-axis velocity (2 bytes)
#define obVelY		0x12	    // y-axis velocity (2 bytes)
#define obInertia	0x14	    // potential speed (2 bytes)
#define obHeight	0x16	    // height/2
#define obWidth	    0x17	    // width/2
#define obPriority	0x18	    // sprite stack priority -- 0 is front
#define obActWid	0x19	    // action width
#define obFrame	    0x1A	    // current frame displayed
#define obAniFrame	0x1B	    // current frame in animation script
#define obAnim		0x1C	    // current animation
#define obPrevAni	0x1D	    // previous animation
#define obTimeFrame	0x1E	    // time to next frame
#define obDelayAni	0x1F	    // time to delay animation
#define obColType	0x20	    // collision response type
#define obColProp	0x21	    // collision extra property
#define obStatus	0x22	    // orientation or mode
#define obRespawnNo	0x23	    // respawn list index number
#define obRoutine	0x24	    // routine number
#define ob2ndRout	0x25	    // secondary routine number
#define obAngle	    0x26	    // angle
#define obSubtype	0x28	    // object subtype
#define obSolid	    ob2ndRout   // solid status flag

// Object variables used by Sonic
#define flashtime   	 0x30	// time between flashes after getting hit
#define invtime 	     0x32	// time left for invincibility
#define shoetime    	 0x34	// time left for speed shoes
#define stick_to_convex  objoff_38
#define standonobject    0x3D	// object Sonic stands on

// Miscellaneous object scratch-RAM
#define objoff_25	0x25
#define objoff_26	0x26
#define objoff_29	0x29
#define objoff_2A	0x2A
#define objoff_2B	0x2B
#define objoff_2C	0x2C
#define objoff_2E	0x2E
#define objoff_2F	0x2F
#define objoff_30	0x30
#define objoff_32	0x32
#define objoff_33	0x33
#define objoff_34	0x34
#define objoff_35	0x35
#define objoff_36	0x36
#define objoff_37	0x37
#define objoff_38	0x38
#define objoff_39	0x39
#define objoff_3A	0x3A
#define objoff_3B	0x3B
#define objoff_3C	0x3C
#define objoff_3D	0x3D
#define objoff_3E	0x3E
#define objoff_3F	0x3F

#define object_size_bits	6
#define object_size	        (1 << object_size_bits)

// Animation flags
#define afEnd   		0xFF	// return to beginning of animation
#define afBack  		0xFE	// go back (specified number) bytes
#define afChange    	0xFD	// run specified animation
#define afRoutine   	0xFC	// increment routine counter
#define afReset 	    0xFB	// reset animation and 2nd object routine counter
#define af2ndRoutine    0xFA	// increment 2nd routine counter

// Background music
#define bgm__First	     0x81
#define bgm_GHZ	         (((ptr_mus81-MusicIndex)/4)+bgm__First)
#define bgm_LZ		     (((ptr_mus82-MusicIndex)/4)+bgm__First)
#define bgm_MZ		     (((ptr_mus83-MusicIndex)/4)+bgm__First)
#define bgm_SLZ	         (((ptr_mus84-MusicIndex)/4)+bgm__First)
#define bgm_SYZ	         (((ptr_mus85-MusicIndex)/4)+bgm__First)
#define bgm_SBZ	         (((ptr_mus86-MusicIndex)/4)+bgm__First)
#define bgm_Invincible	 (((ptr_mus87-MusicIndex)/4)+bgm__First)
#define bgm_ExtraLife	 (((ptr_mus88-MusicIndex)/4)+bgm__First)
#define bgm_SS		     (((ptr_mus89-MusicIndex)/4)+bgm__First)
#define bgm_Title	     (((ptr_mus8A-MusicIndex)/4)+bgm__First)
#define bgm_Ending	     (((ptr_mus8B-MusicIndex)/4)+bgm__First)
#define bgm_Boss	     (((ptr_mus8C-MusicIndex)/4)+bgm__First)
#define bgm_FZ		     (((ptr_mus8D-MusicIndex)/4)+bgm__First)
#define bgm_GotThrough	 (((ptr_mus8E-MusicIndex)/4)+bgm__First)
#define bgm_GameOver	 (((ptr_mus8F-MusicIndex)/4)+bgm__First)
#define bgm_Continue	 (((ptr_mus90-MusicIndex)/4)+bgm__First)
#define bgm_Credits	     (((ptr_mus91-MusicIndex)/4)+bgm__First)
#define bgm_Drowning	 (((ptr_mus92-MusicIndex)/4)+bgm__First)
#define bgm_Emerald	     (((ptr_mus93-MusicIndex)/4)+bgm__First)
#define bgm__Last	     (((ptr_musend-MusicIndex-4)/4)+bgm__First)

// Sound effects
#define sfx__First	     0xA0
#define sfx_Jump	     ((ptr_sndA0-SoundIndex)/4)+sfx__First
#define sfx_Lamppost	 ((ptr_sndA1-SoundIndex)/4)+sfx__First
#define sfx_A2		     ((ptr_sndA2-SoundIndex)/4)+sfx__First
#define sfx_Death	     ((ptr_sndA3-SoundIndex)/4)+sfx__First
#define sfx_Skid	     ((ptr_sndA4-SoundIndex)/4)+sfx__First
#define sfx_A5		     ((ptr_sndA5-SoundIndex)/4)+sfx__First
#define sfx_HitSpikes	 ((ptr_sndA6-SoundIndex)/4)+sfx__First
#define sfx_Push	     ((ptr_sndA7-SoundIndex)/4)+sfx__First
#define sfx_SSGoal	     ((ptr_sndA8-SoundIndex)/4)+sfx__First
#define sfx_SSItem	     ((ptr_sndA9-SoundIndex)/4)+sfx__First
#define sfx_Splash	     ((ptr_sndAA-SoundIndex)/4)+sfx__First
#define sfx_AB		     ((ptr_sndAB-SoundIndex)/4)+sfx__First
#define sfx_HitBoss	     ((ptr_sndAC-SoundIndex)/4)+sfx__First
#define sfx_Bubble	     ((ptr_sndAD-SoundIndex)/4)+sfx__First
#define sfx_Fireball	 ((ptr_sndAE-SoundIndex)/4)+sfx__First
#define sfx_Shield	     ((ptr_sndAF-SoundIndex)/4)+sfx__First
#define sfx_Saw	         ((ptr_sndB0-SoundIndex)/4)+sfx__First
#define sfx_Electric	 ((ptr_sndB1-SoundIndex)/4)+sfx__First
#define sfx_Drown	     ((ptr_sndB2-SoundIndex)/4)+sfx__First
#define sfx_Flamethrower ((ptr_sndB3-SoundIndex)/4)+sfx__First
#define sfx_Bumper	     ((ptr_sndB4-SoundIndex)/4)+sfx__First
#define sfx_Ring	     ((ptr_sndB5-SoundIndex)/4)+sfx__First
#define sfx_SpikesMove	 ((ptr_sndB6-SoundIndex)/4)+sfx__First
#define sfx_Rumbling	 ((ptr_sndB7-SoundIndex)/4)+sfx__First
#define sfx_B8		     ((ptr_sndB8-SoundIndex)/4)+sfx__First
#define sfx_Collapse	 ((ptr_sndB9-SoundIndex)/4)+sfx__First
#define sfx_SSGlass	     ((ptr_sndBA-SoundIndex)/4)+sfx__First
#define sfx_Door	     ((ptr_sndBB-SoundIndex)/4)+sfx__First
#define sfx_Teleport	 ((ptr_sndBC-SoundIndex)/4)+sfx__First
#define sfx_ChainStomp	 ((ptr_sndBD-SoundIndex)/4)+sfx__First
#define sfx_Roll	     ((ptr_sndBE-SoundIndex)/4)+sfx__First
#define sfx_Continue	 ((ptr_sndBF-SoundIndex)/4)+sfx__First
#define sfx_Basaran	     ((ptr_sndC0-SoundIndex)/4)+sfx__First
#define sfx_BreakItem	 ((ptr_sndC1-SoundIndex)/4)+sfx__First
#define sfx_Warning	     ((ptr_sndC2-SoundIndex)/4)+sfx__First
#define sfx_GiantRing	 ((ptr_sndC3-SoundIndex)/4)+sfx__First
#define sfx_Bomb	     ((ptr_sndC4-SoundIndex)/4)+sfx__First
#define sfx_Cash	     ((ptr_sndC5-SoundIndex)/4)+sfx__First
#define sfx_RingLoss	 ((ptr_sndC6-SoundIndex)/4)+sfx__First
#define sfx_ChainRise	 ((ptr_sndC7-SoundIndex)/4)+sfx__First
#define sfx_Burning	     ((ptr_sndC8-SoundIndex)/4)+sfx__First
#define sfx_Bonus	     ((ptr_sndC9-SoundIndex)/4)+sfx__First
#define sfx_EnterSS	     ((ptr_sndCA-SoundIndex)/4)+sfx__First
#define sfx_WallSmash	 ((ptr_sndCB-SoundIndex)/4)+sfx__First
#define sfx_Spring	     ((ptr_sndCC-SoundIndex)/4)+sfx__First
#define sfx_Switch	     ((ptr_sndCD-SoundIndex)/4)+sfx__First
#define sfx_RingLeft	 ((ptr_sndCE-SoundIndex)/4)+sfx__First
#define sfx_Signpost	 ((ptr_sndCF-SoundIndex)/4)+sfx__First
#define sfx__Last	     ((ptr_sndend-SoundIndex-4)/4)+sfx__First

// Special sound effects
#define spec__First	     0xD0
#define sfx_Waterfall	 ((ptr_sndD0-SpecSoundIndex)/4)+spec__First
#define spec__Last	     ((ptr_specend-SpecSoundIndex-4)/4)+spec__First

#define flg__First	 0xE0
#define bgm_Fade	 0xE0//((ptr_flgE0-Sound_ExIndex)/4)+flg__First
#define sfx_Sega	 0xE1//((ptr_flgE1-Sound_ExIndex)/4)+flg__First
#define bgm_Speedup	 0xE2//((ptr_flgE2-Sound_ExIndex)/4)+flg__First
#define bgm_Slowdown 0xE3//((ptr_flgE3-Sound_ExIndex)/4)+flg__First
#define bgm_Stop	 0xE4//((ptr_flgE4-Sound_ExIndex)/4)+flg__First
#define flg__Last	 //((ptr_flgend-Sound_ExIndex-4)/4)+flg__First

// Sonic frame IDs
#define fr_Null	     0x0
#define fr_Stand	 0x1
#define fr_Wait1	 0x2
#define fr_Wait2	 0x3
#define fr_Wait3	 0x4
#define fr_LookUp	 0x5
#define fr_Walk11	 0x6
#define fr_Walk12	 0x7
#define fr_Walk13	 0x8
#define fr_Walk14	 0x9
#define fr_Walk15	 0xA
#define fr_Walk16	 0xB
#define fr_Walk21	 0xC
#define fr_Walk22	 0xD
#define fr_Walk23	 0xE
#define fr_Walk24	 0xF
#define fr_Walk25	 0x10
#define fr_Walk26	 0x11
#define fr_Walk31	 0x12
#define fr_Walk32	 0x13
#define fr_Walk33	 0x14
#define fr_Walk34	 0x15
#define fr_Walk35	 0x16
#define fr_Walk36	 0x17
#define fr_Walk41	 0x18
#define fr_Walk42	 0x19
#define fr_Walk43	 0x1A
#define fr_Walk44	 0x1B
#define fr_Walk45	 0x1C
#define fr_Walk46	 0x1D
#define fr_Run11	 0x1E
#define fr_Run12	 0x1F
#define fr_Run13	 0x20
#define fr_Run14	 0x21
#define fr_Run21	 0x22
#define fr_Run22	 0x23
#define fr_Run23	 0x24
#define fr_Run24	 0x25
#define fr_Run31	 0x26
#define fr_Run32	 0x27
#define fr_Run33	 0x28
#define fr_Run34	 0x29
#define fr_Run41	 0x2A
#define fr_Run42	 0x2B
#define fr_Run43	 0x2C
#define fr_Run44	 0x2D
#define fr_Roll1	 0x2E
#define fr_Roll2	 0x2F
#define fr_Roll3	 0x30
#define fr_Roll4	 0x31
#define fr_Roll5	 0x32
#define fr_Warp1	 0x33
#define fr_Warp2	 0x34
#define fr_Warp3	 0x35
#define fr_Warp4	 0x36
#define fr_Stop1	 0x37
#define fr_Stop2	 0x38
#define fr_Duck	     0x39
#define fr_Balance1	 0x3A
#define fr_Balance2	 0x3B
#define fr_Float1	 0x3C
#define fr_Float2	 0x3D
#define fr_Float3	 0x3E
#define fr_Float4	 0x3F
#define fr_Spring	 0x40
#define fr_Hang1	 0x41
#define fr_Hang2	 0x42
#define fr_Leap1	 0x43
#define fr_Leap2	 0x44
#define fr_Push1	 0x45
#define fr_Push2	 0x46
#define fr_Push3	 0x47
#define fr_Push4	 0x48
#define fr_Surf	     0x49
#define fr_BubStand	 0x4A
#define fr_Burnt	 0x4B
#define fr_Drown	 0x4C
#define fr_Death	 0x4D
#define fr_Shrink1	 0x4E
#define fr_Shrink2	 0x4F
#define fr_Shrink3	 0x50
#define fr_Shrink4	 0x51
#define fr_Shrink5	 0x52
#define fr_Float5	 0x53
#define fr_Float6	 0x54
#define fr_Injury	 0x55
#define fr_GetAir	 0x56
#define fr_WaterSlide 0x57

// Boss locations
// The main values are based on where the camera boundaries mainly lie
// The end values are where the camera scrolls towards after defeat
#define boss_ghz_x	 0x2960		// Green Hill Zone
#define boss_ghz_y	 0x300
#define boss_ghz_end	 boss_ghz_x+0x160

#define boss_lz_x	 0x1DE0		// Labyrinth Zone
#define boss_lz_y	 0xC0
#define boss_lz_end	 boss_lz_x+0x250

#define boss_mz_x	 0x1800		// Marble Zone
#define boss_mz_y	 0x210
#define boss_mz_end	 boss_mz_x+0x160

#define boss_slz_x	 0x2000		// Star Light Zone
#define boss_slz_y	 0x210
#define boss_slz_end	 boss_slz_x+0x160

#define boss_syz_x	 0x2C00		// Spring Yard Zone
#define boss_syz_y	 0x4CC
#define boss_syz_end	 boss_syz_x+0x140

#define boss_sbz2_x	 0x2050		// Scrap Brain Zone Act 2 Cutscene
#define boss_sbz2_y	 0x510

#define boss_fz_x	 0x2450		// Final Zone
#define boss_fz_y	 0x510
#define boss_fz_end	 boss_fz_x+0x2B0

// Tile VRAM Locations

// Shared
#define ArtTile_GHZ_MZ_Swing		     0x380
#define ArtTile_MZ_SYZ_Caterkiller	     0x4FF
#define ArtTile_GHZ_SLZ_Smashable_Wall	 0x50F

// Green Hill Zone
#define ArtTile_GHZ_Flower_4		     ArtTile_Level+0x340
#define ArtTile_GHZ_Edge_Wall		     0x34C
#define ArtTile_GHZ_Flower_Stalk	     ArtTile_Level+0x358
#define ArtTile_GHZ_Big_Flower_1	     ArtTile_Level+0x35C
#define ArtTile_GHZ_Small_Flower	     ArtTile_Level+0x36C
#define ArtTile_GHZ_Waterfall		     ArtTile_Level+0x378
#define ArtTile_GHZ_Flower_3		     ArtTile_Level+0x380
#define ArtTile_GHZ_Bridge		         0x38E
#define ArtTile_GHZ_Big_Flower_2	     ArtTile_Level+0x390
#define ArtTile_GHZ_Spike_Pole		     0x398
#define ArtTile_GHZ_Giant_Ball		     0x3AA
#define ArtTile_GHZ_Purple_Rock	     0x3D0

// Marble Zone
#define ArtTile_MZ_Block		 0x2B8
#define ArtTile_MZ_Animated_Magma	 ArtTile_Level+0x2D2
#define ArtTile_MZ_Animated_Lava	 ArtTile_Level+0x2E2
#define ArtTile_MZ_Torch		 ArtTile_Level+0x2F2
#define ArtTile_MZ_Spike_Stomper	 0x300
#define ArtTile_MZ_Fireball		 0x345
#define ArtTile_MZ_Glass_Pillar	 0x38E
#define ArtTile_MZ_Lava		 0x3A8

// Spring Yard Zone
#define ArtTile_SYZ_Bumper		 0x380
#define ArtTile_SYZ_Big_Spikeball	 0x396
#define ArtTile_SYZ_Spikeball_Chain	 0x3BA

// Labyrinth Zone
#define ArtTile_LZ_Block_1		 0x1E0
#define ArtTile_LZ_Block_2		 0x1F0
#define ArtTile_LZ_Splash		 0x259
#define ArtTile_LZ_Gargoyle		 0x2E9
#define ArtTile_LZ_Water_Surface	 0x300
#define ArtTile_LZ_Spikeball_Chain	 0x310
#define ArtTile_LZ_Flapping_Door	 0x328
#define ArtTile_LZ_Bubbles		 0x348
#define ArtTile_LZ_Moving_Block	 0x3BC
#define ArtTile_LZ_Door		 0x3C4
#define ArtTile_LZ_Harpoon		 0x3CC
#define ArtTile_LZ_Pole		 0x3DE
#define ArtTile_LZ_Push_Block		 0x3DE
#define ArtTile_LZ_Blocks		 0x3E6
#define ArtTile_LZ_Conveyor_Belt	 0x3F6
#define ArtTile_LZ_Sonic_Drowning	 0x440
#define ArtTile_LZ_Rising_Platform	 ArtTile_LZ_Blocks+0x69
#define ArtTile_LZ_Orbinaut		 0x467
#define ArtTile_LZ_Cork		 ArtTile_LZ_Blocks+0x11A

// Star Light Zone
#define ArtTile_SLZ_Seesaw		 0x374
#define ArtTile_SLZ_Fan		 0x3A0
#define ArtTile_SLZ_Pylon		 0x3CC
#define ArtTile_SLZ_Swing		 0x3DC
#define ArtTile_SLZ_Orbinaut		 0x429
#define ArtTile_SLZ_Fireball		 0x480
#define ArtTile_SLZ_Fireball_Launcher	 0x4D8
#define ArtTile_SLZ_Collapsing_Floor	 0x4E0
#define ArtTile_SLZ_Spikeball		 0x4F0

// Scrap Brain Zone
#define ArtTile_SBZ_Caterkiller	 0x2B0
#define ArtTile_SBZ_Moving_Block_Short	 0x2C0
#define ArtTile_SBZ_Door		 0x2E8
#define ArtTile_SBZ_Girder		 0x2F0
#define ArtTile_SBZ_Disc		 0x344
#define ArtTile_SBZ_Junction		 0x348
#define ArtTile_SBZ_Swing		 0x391
#define ArtTile_SBZ_Saw		 0x3B5
#define ArtTile_SBZ_Flamethrower	 0x3D9
#define ArtTile_SBZ_Collapsing_Floor	 0x3F5
#define ArtTile_SBZ_Orbinaut		 0x429
#define ArtTile_SBZ_Smoke_Puff_1	 ArtTile_Level+0x448
#define ArtTile_SBZ_Smoke_Puff_2	 ArtTile_Level+0x454
#define ArtTile_SBZ_Moving_Block_Long	 0x460
#define ArtTile_SBZ_Horizontal_Door	 0x46F
#define ArtTile_SBZ_Electric_Orb	 0x47E
#define ArtTile_SBZ_Trap_Door		 0x492
#define ArtTile_SBZ_Vanishing_Block	 0x4C3
#define ArtTile_SBZ_Spinning_Platform	 0x4DF

// Final Zone
#define ArtTile_FZ_Boss		 0x300
#define ArtTile_FZ_Eggman_Fleeing	 0x3A0
#define ArtTile_FZ_Eggman_No_Vehicle	 0x470

// General Level Art
#define ArtTile_Level			 0x000
#define ArtTile_Ball_Hog		 0x302
#define ArtTile_Bomb			 0x400
#define ArtTile_Crabmeat		 0x400
#define ArtTile_Missile_Disolve	 0x41C // Unused
#define ArtTile_Buzz_Bomber		 0x444
#define ArtTile_Chopper		        0x47B
#define ArtTile_Yadrin			        0x47B
#define ArtTile_Jaws			        0x486
#define ArtTile_Newtron		            0x49B
#define ArtTile_Burrobot		        0x4A6
#define ArtTile_Basaran		            0x4B8
#define ArtTile_Roller			        0x4B8
#define ArtTile_Moto_Bug		        0x4F0
#define ArtTile_Button			        0x50F
#define ArtTile_Spikes			        0x51B
#define ArtTile_Spring_Horizontal	    0x523
#define ArtTile_Spring_Vertical	        0x533
#define ArtTile_Shield			        0x541
#define ArtTile_Invincibility		    0x55C
#define ArtTile_Game_Over		        0x55E
#define ArtTile_Title_Card		        0x580
#define ArtTile_Animal_1		        0x580
#define ArtTile_Animal_2		        0x592
#define ArtTile_Explosion		        0x5A0
#define ArtTile_Monitor		            0x680
#define ArtTile_HUD			            0x6CA
#define ArtTile_Sonic			        0x780
#define ArtTile_Points			        0x797
#define ArtTile_Lamppost		        0x7A0
#define ArtTile_Ring			        0x7B2
#define ArtTile_Lives_Counter		    0x7D4

// Eggman
#define ArtTile_Eggman			        0x400
#define ArtTile_Eggman_Weapons		    0x46C
#define ArtTile_Eggman_Button		    0x4A4
#define ArtTile_Eggman_Spikeball	    0x518
#define ArtTile_Eggman_Trap_Floor	    0x518
#define ArtTile_Eggman_Exhaust		    ArtTile_Eggman+0x12A

// End of Level
#define ArtTile_Giant_Ring		        0x400
#define ArtTile_Giant_Ring_Flash	    0x462
#define ArtTile_Prison_Capsule		    0x49D
#define ArtTile_Hidden_Points		    0x4B6
#define ArtTile_Warp			        0x541
#define ArtTile_Mini_Sonic		        0x551
#define ArtTile_Bonuses		            0x570
#define ArtTile_Signpost		        0x680

// Sega Screen
#define ArtTile_Sega_Tiles		        0x000

// Title Screen
#define ArtTile_Title_Japanese_Text	    0x000
#define ArtTile_Title_Foreground	    0x200
#define ArtTile_Title_Sonic		        0x300
#define ArtTile_Title_Trademark	        0x510
#define ArtTile_Level_Select_Font	    0x680

// Continue Screen
#define ArtTile_Continue_Sonic		    0x500
#define ArtTile_Continue_Number	        0x6FC

// Ending
#define ArtTile_Ending_Flowers		    0x3A0
#define ArtTile_Ending_Emeralds	        0x3C5
#define ArtTile_Ending_Sonic		    0x3E1
#define ArtTile_Ending_Eggman		    0x524
#define ArtTile_Ending_Rabbit		    0x553
#define ArtTile_Ending_Chicken		    0x565
#define ArtTile_Ending_Penguin		    0x573
#define ArtTile_Ending_Seal		        0x585
#define ArtTile_Ending_Pig		        0x593
#define ArtTile_Ending_Flicky		    0x5A5
#define ArtTile_Ending_Squirrel	        0x5B3
#define ArtTile_Ending_STH		        0x5C5

// Try Again Screen
#define ArtTile_Try_Again_Emeralds	    0x3C5
#define ArtTile_Try_Again_Eggman	    0x3E1

// Special Stage
#define ArtTile_SS_Background_Clouds	0x000
#define ArtTile_SS_Background_Fish	    0x051
#define ArtTile_SS_Wall		            0x142
#define ArtTile_SS_Plane_1		        0x200
#define ArtTile_SS_Bumper		        0x23B
#define ArtTile_SS_Goal		            0x251
#define ArtTile_SS_Up_Down		        0x263
#define ArtTile_SS_R_Block		        0x2F0
#define ArtTile_SS_Plane_2		        0x300
#define ArtTile_SS_Extra_Life		    0x370
#define ArtTile_SS_Emerald_Sparkle	    0x3F0
#define ArtTile_SS_Plane_3		        0x400
#define ArtTile_SS_Red_White_Block	    0x470
#define ArtTile_SS_Ghost_Block		    0x4F0
#define ArtTile_SS_Plane_4		        0x500
#define ArtTile_SS_W_Block		        0x570
#define ArtTile_SS_Glass		        0x5F0
#define ArtTile_SS_Plane_5		        0x600
#define ArtTile_SS_Plane_6		        0x700
#define ArtTile_SS_Emerald		        0x770
#define ArtTile_SS_Zone_1		        0x797
#define ArtTile_SS_Zone_2		        0x7A0
#define ArtTile_SS_Zone_3		        0x7A9
#define ArtTile_SS_Zone_4		        0x797
#define ArtTile_SS_Zone_5		        0x7A0
#define ArtTile_SS_Zone_6		        0x7A9

// Special Stage Results
#define ArtTile_SS_Results_Emeralds	    0x541

// Font
#define ArtTile_Sonic_Team_Font	        0x0A6
#define ArtTile_Credits_Font		    0x5A0

// Error Handler
#define ArtTile_Error_Handler_Font	    0x7C0

enum {
    palid_SegaBG,
    palid_Title,
    palid_LevelSel,
    palid_Sonic,
    palid_GHZ,
    palid_LZ,
    palid_MZ,
    palid_SLZ,
    palid_SYZ,
    palid_SBZ1,
    palid_Special,
    palid_LZWater,
    palid_SBZ3,
    palid_SBZ3Water,
    palid_SBZ2,
    palid_LZSonWater,
    palid_SBZ3SonWat,
    palid_SSResult,
    palid_Continue,
    palid_Ending,
};

#endif // __CLIONPROJECTS_SONIC_ANYWHERE_SRC2_CONSTANTS_H_