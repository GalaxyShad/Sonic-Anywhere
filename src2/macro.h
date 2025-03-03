#ifndef __CLIONPROJECTS_SONIC_ANYWHERE_SRC2_MACRO_H_
#define __CLIONPROJECTS_SONIC_ANYWHERE_SRC2_MACRO_H_

#include "include_backend/debug.h"
#include "include_backend/mdaudio.h"
#include "include_backend/mdvdp.h"
#include "include_backend/tinyint.h"
#include "src/bytearray.h"
#include "src/compressors.h"
#include "src2/context.h"

#define FixBugs
#define Revision 1

#define DEREF_ADR_TO_LONG(ADR)          ((ADR[0] << 24) | (ADR[1] << 16) | (ADR[2] << 8) | ADR[3])
#define DEREF_ADR_TO_WORD(ADR)          ((ADR[0] << 8) | ADR[1])
#define MOVE_WORD_TO_ADR(WORD, ADR)     ADR[0] = ((WORD) >> 8) & 0xFF; ADR[1] = ((WORD) & 0xFF)
#define MOVE_LONG_TO_ADR(LONG, ADR)     ADR[0] = ((LONG) >> 24) & 0xFF; ADR[1] = ((LONG) >> 16) & 0xFF; ADR[2] = ((LONG) >> 8) & 0xFF; ADR[3] = ((LONG) >> 0) & 0xFF;

void ON_PLAY_SOUND_SPECIAL(u8 snd) {
    if (snd == sfx_Sega) {
        md_audio__play_sound_special(SND__SEGA);
    }
}

void vdp_set_register(u8 reg, u8 data) {
    // https://segaretro.org/Sega_Mega_Drive/VDP_registers#Status_Register
    
    switch (reg) {
        // Mode Register 1
        case 0x00: {
            u8 l, ie1, m4, m3, de;

            l   = (data >> 5) & 1;  // 1 = leftmost 8 pixels are blanked to background colour.
            ie1 = (data >> 4) & 1;  // 1 = enable horizontal interrupts.
            m4  = (data >> 2) & 1;  // 1 = normal operation; 0 = masks high bits of color entries (this bit controls Mode 4 in SMS mode).
            m3  = (data >> 1) & 1;  // 1 = freeze H/V counter on level 2 interrupt; 0 = enable H/V counter.
            de  = (data >> 0) & 1;  // 1 = disable display.

            break;
        }

        // Mode Register 2
        case 0x01: {
            u8 vr, de, ie0, m1, m2, m5;

            vr = (data >> 7) & 1;  // VR: 1 = use 128kB of VRAM. Will not work correctly on standard consoles with 64kB VRAM.      
            de = (data >> 6) & 1;  // DE: 1 = enable display; 0 = fill display with background colour.     
            ie0 = (data >> 5) & 1; // IE0: 1 = enable vertical interrupts.     
            m1 = (data >> 4) & 1;  // M1: 1 = enable DMA operations; 0 = ignore DMA operations.        
            m2 = (data >> 3) & 1;  // M2: 1 = 240 pixel (30 cell) PAL mode; 0 = 224 pixel (28 cell) NTSC mode.     
            m5 = (data >> 2) & 1;  // M5: 1 = Mega Drive (mode 5) display; 0 = Master System (mode 4) display.     

            break;
        }

        // Plane A Name Table Location
        case 0x02: {
            u8 pa6, pa5_pa3;

            pa6 = (data >> 6) & 1;          // PA5-PA3: Bits 15-13 of foreground (plane A) nametable address in VRAM. Effectively the address (which must be a multiple of $2000) divided by $400.
            pa5_pa3 = (data >> 3) & 0b111;  // PA6: Used for 128kB VRAM.

            break;
        }

        // Window Name Table Location
        case 0x03: {
            u8 w6, w5_w1;

            w6 = (data >> 6) & 1;
            w5_w1 = (data >> 1) & 0b11111;

            // W5-W1: Bits 15-11 of window nametable address in VRAM. Effectively the address (which must be a multiple of $800) divided by $400.
            // W1: Ignored in 320 pixel wide mode, limiting the address to a multiple of $1000.
            // W6: Used for 128kB VRAM.

            break;
        }

        // Plane B Name Table Location
        case 0x04: {
            u8 pb3, pb2_pb0;

            pb3 = (data >> 3) & 1;
            pb2_pb0 = data & 0b111;

            // PB2-PB0: Bits 15-13 of foreground (plane B) nametable address in VRAM. Effectively the address (which must be a multiple of $2000) divided by $2000.
            // PB3: Used for 128kB VRAM.

            break;
        }

        // Sprite Table Location
        case 0x05: {
            u8 st7, st6_st0;

            st7 = (data >> 7) & 1;
            st6_st0 = (data >> 0) & (0xFF >> 1);

            // ST6-ST0: Bits 15-9 of sprite table address in VRAM. Effectively the address (which must be a multiple of $200) divided by $200.
            // ST0: Ignored in 320 pixel wide mode, limiting the address to a multiple of $400.
            // ST7: Used for 128kB VRAM.

            break;
        }

        // -
        case 0x06: {
            u8 sp5;

            sp5 = (data >> 5) & 1;
            
            // SP5: Bit 16 of sprite table address in 128kB VRAM.
            
            break;
        }

        // Background Colour
        case 0x07: {
            u8 pl5_pl4, c3_c0;

            pl5_pl4 = (data >> 4) & 0b11;
            c3_c0 = (data >> 0) & 0b1111;

            // PL5-PL4: Palette line.
            // C3-C0: Colour.

            break;
        }

        // Unused
        case 0x08: break; // Master System horizontal scroll register
        case 0x09: break; // Master System vertical scroll register	

        // Horizontal Interrupt Counter
        case 0x0A: {
            u8 h = data;

            // H: Number of scanlines between horizontal interrupts.

            break;
        }

        // Mode Register 3
        case 0x0B: {
            u8 ie2, vs, hs1_hs2;

            ie2 = (data >> 3) & 1;
            vs = (data >> 2) & 1;
            hs1_hs2 = data & 0b11;

            // IE2: 1 = enable external interrupts, caused by the TH pin being set to input mode and having the TH interrupt enable bit set. (Both of these are controlled by the Mega Drive' I/O registers).
            // VS: Vertical scrolling mode: 1 = 16 pixel columns (1 word per column in VSRAM); 0 = full screen (1 longword only in VSRAM).
            // HS1-HS2: Horizontal scrolling mode: 00 = full screen; 01 = invalid; 10 = 8 pixel rows; 11 = single pixel rows.
            
            break;
        }

        // Mode Register 4
        case 0x0C: {
            u8 rs1, vs, hs, ep, sh, ls1_ls2, rs0;

            rs1 = (data >> 7) & 1;
            vs = (data >> 6) & 1;
            hs = (data >> 5) & 1;
            ep = (data >> 4) & 1;
            sh = (data >> 3) & 1;
            ls1_ls2 = (data >> 1) & 0b11;
            rs0 = (data >> 0) & 0b1;

            // RS1/RS0: 1 = 320 pixel (40 cell) wide mode; 0 = 256 pixel (32 cell) wide mode. Both bits must be the same.
            // VS: Appears to do something relating to the horizontal sync, which seems to freeze it. (source md.railgun.works)
            // HS: Replaces the VSync signal with a pixel clock signal when set. (source md.railgun.works)
            // EP: 1 = enable external pixel bus.
            // SH: 1 = enable shadow/highlight mode.
            // LS1-LS0: Interlace mode: 00 = no interlace; 01 = interlace normal resolution; 10 = no interlace; 11 = interlace double resolution. Changes do not take effect until the next vertical blank.

            break;
        }

        // Horizontal Scroll Data Location
        case 0x0D: {
            u8 hs6, hs5_hs0;

            hs6 = (data >> 6) & 1;
            hs5_hs0 = (data >> 0) & (0xFF >> 2);

            // HS5-HS0: Bits 15-10 of horizontal scroll data address in VRAM. Effectively the address (which must be a multiple of $400) divided by $400.
            // HS6: Used for 128kB VRAM.

            break;
        }

        // -
        case 0x0E: {
            u8 pb4, pa0;

            pb4 = (data >> 4) & 1;
            pa0 = (data >> 0) & 1;

            // PB4: Bit 16 of background (plane B) nametable address in 128kB VRAM.
            // PA0: Bit 16 of foreground (plane A) nametable address in 128kB VRAM.
        
            break;
        }

        // Auto-Increment Value
        case 0x0F: {
            u8 inc7_inc0 = data;

            // INC7-INC0: Value to be added to the VDP address register after each read/write to the data port. 2 is most common.

            break;
        }

        // Plane Size
        case 0x10: {
            u8 h1_h0, w1_w0;

            h1_h0 = (data >> 4) & 0b11;
            w1_w0 = (data >> 0) & 0b11;

            // H1-H0: Height setting for background and foreground (planes A & B). 00 = 256 pixels (32 cells); 01 = 512 pixels (64 cells); 10 = invalid; 11 = 1024 pixels (128 cells).
            // W1-W0: Width setting for background and foreground (planes A & B). Same as above.
            // Height/width settings of 64x128 or 128x128 cells are invalid, due to a maximum plane size of $2000 bytes.

            break;
        }

        // Window Plane Horizontal Position
        case 0x11: {
            u8 r, hp4_hp0;

            r = (data >> 7) & 1;
            hp4_hp0 = (data >> 0) & 0b11111;

            // R: 1 = draw window from HP to right edge of screen; 0 = draw window from HP to left edge of screen.
            // HP4-HP0: Horizontal position on screen to start drawing the window plane (in units of 8 pixels).

            break;
        }

        // Window Plane Vertical Position
        case 0x12: {
            u8 d, vp4_vp0;

            d = (data >> 7) & 1;
            vp4_vp0 = (data >> 0) & 0b11111;

            // D: 1 = draw window from VP to bottom edge of screen; 0 = draw window from VP to top edge of screen.
            // VP4-VP0: Vertical position on screen to start drawing the window plane (in units of 8 pixels).   
        
            break;
        }

        // DMA Length
        case 0x13: {
            u8 l = data;

            // L7-L0: Low byte of DMA length in bytes, divided by 2.

            break;
        }
        case 0x14: {
            u8 h = data;

            // H7-H0: High byte of DMA length in bytes, divided by 2.

            break;
        }

        // DMA Source
        case 0x15: {
            u8 l = data;

            // L7-L0: Low byte of DMA source address, divided by 2.
        
            break;
        }
        case 0x16: {
            u8 m = data;

            // M7-M0: Middle byte of DMA source address, divided by 2.
        
            break;
        }
        case 0x17: {
            u8 t1, t0, h;

            t1 = (data >> 7) & 1;
            t0 = (data >> 6) & 1;
            h = (data >> 0) & (0xFF >> 2);

            // H5-H0: High byte of DMA source address, divided by 2.
            // T1/T0: DMA type. 0x = 68k to VRAM copy (T0 is used as the highest bit in source address); 10 = VRAM fill (source can be left blank); 11 = VRAM to VRAM copy.

            break;
        }
    }
}

void vdp_control_port_write_to_register(u8 b0, u8 b1) {
    u8 register_number = b0 & 0b11111;
    u8 data = b1;

    LOG(
        "VDP_CONTROL_PORT_WRITE_TO_REG\n"
        "reg:   %X\n"
        "data:  %X",
        register_number, data
    );
}

void vdp_control_port_write_to_address(u8* bytes) {
    u8 cd1 = (bytes[0] >> 7) & 0b1;
    u8 cd0 = (bytes[0] >> 6) & 0b1;
    u8 a13_a8 = (bytes[0] >> 0) & (0xFF >> 2);

    u8 a7_a0 = (bytes[1] >> 0) & 0xFF;

    u8 cd5 = (bytes[3] >> 7) & 0b1;
    u8 cd4 = (bytes[3] >> 6) & 0b1;
    u8 cd3_cd2 = (bytes[3] >> 4) & 0b11;

    u8 a15_a14 = (bytes[3] >> 0) & 0b11;

    u32 adr = (a15_a14 << 14) | (a13_a8 << 8) | a7_a0;
    u8 dst = (cd3_cd2 << 1) | cd1;

    LOG(
        "VDP_CONTROL_PORT_WRITE_TO_ADR\n"
        "adr:   %X\n"
        "mode:  %X %s\n"
        "dst:   %X %s\n"
        "VRAM to VRAM?: %X\n"
        "DMA?:          %X",
        adr,
        cd0, cd0 ? "write" : "read",
        dst, (dst == 0b000) ? "VRAM"         :
             (dst == 0b100) ? "CRAM (read)"  :
             (dst == 0b001) ? "CRAM (write)" :
             (dst == 0b010) ? "VSRAM"        :
             (dst == 0b110) ? "VSRAM (byte read)" :
                              "UNKNOWN",
        cd4,
        cd5
    );
}

void vdp_control_port_write(u32 data) {
    u8 bytes[4];

    bytes[0] = (data >> 24) & 0xFF;  
    bytes[1] = (data >> 16) & 0xFF; 
    bytes[2] = (data >> 8) & 0xFF;
    bytes[3] = (data >> 0) & 0xFF;

    if (((bytes[2] >> 5) & 0b111) == 0b100) {
        vdp_control_port_write_to_register(bytes[2], bytes[3]);
    } else {
        vdp_control_port_write_to_address(bytes);
    }
}

#define VDP_CONTROL_PORT_READ() 0
#define VDP_CONTROL_PORT_WRITE(X) vdp_control_port_write(X)
#define VDP_DATA_PORT_WRITE(X)

#define PORT_WRITE(ADR, X)
#define PORT_READ(ADR) 0

#define startZ80
#define stopZ80
#define waitZ80

#define enable_ints
#define disable_ints

static void writeCRAM(S1Context* ctx, void* SRC, void* SRC_END, size DST) {
    u32 len = SRC_END - SRC;
    md_vdp__dma_set_source(SRC, len, MD_VDP_DMA_TYPE__68K_TO_VRAM_COPY);
    md_vdp__dma_set_destination_type(DST, MD_VDP_DMA_DST__CRAM);
    
    u32 unk = 0x80+((DST & 0xC000)>>14);
    ctx->var->v_vdp_buffer2 = unk;
}

static void writeVRAM(void* SRC, void* SRC_END, size DST) {
    NOT_IMPLEMENTED
}

// from https://github.com/cvghivebrain/s1disasm
// ---------------------------------------------------------------------------
// Copy a tilemap from 68K (ROM/RAM) to the VRAM without using DMA
// input: source, destination, width [cells], height [cells]
// uses d0, d1, d2, d3, d4, a1
// --------------------------------------
static void copyTilemapXY(u8* src, size dst, size x, size y, size width, size height) {
    MdVdpPlane plane;

    if (dst >= vram_fg && dst < vram_bg) {
        plane = MD_VDP_PLANE__FOREGROUND;
    } else if (dst >= vram_bg && dst < vram_sprites) {
        plane = MD_VDP_PLANE__BACKGROUND;
    } else if (dst >= vram_sprites && dst < vram_hscroll) {
        // TODO
    } else if (dst >= vram_hscroll) {
        // TODO
    }

    md_vdp__copy_tilemap_to_plane_r_2(
        plane, 
        x, y, 
        src, 
        width, height
    );
} 

static void copyTilemap(u8* src, size dst, size width, size height) { 
    NOT_IMPLEMENTED 
}

static void fillVRAM(u8 byte, void* start, void* end) { NOT_IMPLEMENTED }

static void clearRAM(void* start, void* end) { NOT_IMPLEMENTED }

static void locVRAM_(S1Context* ctx, size loc, size controlport) { 
    ctx->vram_copy_to = loc;
 }

#define locVRAM(LOC, PORT) locVRAM_(ctx, LOC, PORT)

#define make_art_tile(ADDR, PAL, PRI) (((PRI&1)<<15)|((PAL&3)<<13)|ADDR)


#endif // __CLIONPROJECTS_SONIC_ANYWHERE_SRC2_MACRO_H_