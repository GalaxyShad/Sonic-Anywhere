#include "src/object_map.h"

#include "src/gamemode/game.h"

#include "include_backend/debug.h"

#define ROUTINE_MAIN 0
#define ROUTINE_DISPLAY 2

#define SCREEN_LEFT 128
#define SCREEN_TOP 128

static u16 credits_num__ = 0;

static void display__(SObjectProps* p);

static void main__(SObjectProps* p) {
    p->routine_number = ROUTINE_DISPLAY; // goto Cred_Display next

    p->x.px = SCREEN_LEFT + 160;
    p->screen_y = SCREEN_TOP + 112;

    // TODO p->gfx / tile
    // move.l	#Map_Cred,ost_mappings(a0)

    p->frame = credits_num__;
    p->render.use_abs = 1;
    p->sprite_stack_priority = 0;

    if (game__get_game_mode() != GM_TITLE)
        return display__(p);

    // move.w	#vram_title_credits/sizeof_cell,ost_tile(a0)

    p->frame = 0xA; // display "SONIC TEAM PRESENTS"

    return display__(p);

    // TODO

    // tst.b	(f_credits_cheat).w			; is hidden credits cheat on?
    //		beq.s	Cred_Display				; if not, branch
    //		cmpi.b	#btnABC+btnDn,(v_joypad_hold_actual).w	; is A+B+C+Down being pressed? ($72)
    //		bne.s	Cred_Display				; if not, branch
    //		move.w	#cWhite,(v_pal_dry_next+$40).w		; 3rd palette, 1st entry = white
    //		move.w	#$880,(v_pal_dry_next+$42).w		; 3rd palette, 2nd entry = cyan
    //		jmp	(DeleteObject).l
}

static void display__(SObjectProps* p) {
    NOT_IMPLEMENTED
}

void s_obj_routine__credits_text(SObjectProps* p) {
    switch (p->routine_number) {
    case ROUTINE_MAIN:
        return main__(p);
    case ROUTINE_DISPLAY:
        return display__(p);
    }
}