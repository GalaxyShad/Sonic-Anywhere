#include "sprite_buffer.h"

#include "include_backend/debug.h"
#include "include_backend/mdvdp.h"

typedef struct SpriteNode {
    u16 xpos, ypos;
    u8 width, height, tile, xflip, yflip, pal, pri;
    u8 next;
} SpriteNode;

static int sprite_count__ = 0;
static SpriteNode sprite_list__[0x50] = {0};

void s_sprite_buffer__reset() {
    sprite_count__ = 0;
}

void s_sprite_buffer__add(const GameSpritePiece* spr, i16 x, i16 y, u16 tile_shift) {
    ASSERT(sprite_count__ < 0x50)

    SpriteNode* dst = sprite_list__ + sprite_count__;

    dst->xpos = x + spr->xpos;
    dst->ypos = y + spr->ypos;

    dst->xflip = spr->xflip;
    dst->yflip = spr->yflip;

    dst->width = spr->width;
    dst->height = spr->height;

    dst->pal = spr->pal;
    dst->pri = spr->pri;

    dst->tile = spr->tile + tile_shift;

    sprite_count__++;

    dst->next = sprite_count__;
}

static void sprite_to_vram__(const SpriteNode* spr, u8* dst) {
    dst[0] = (spr->ypos >> 8) & 0b11;
    dst[1] = (spr->ypos >> 0) & 0xFF;

    dst[2] = ((spr->width & 0b11) << 2) | ((spr->height & 0b11) << 0);
    dst[3] = spr->next & 0b01111111;

    dst[4] = (spr->pri & 0b1) << 7;
    dst[4] |= (spr->pal & 0b11) << 5;
    dst[4] |= (spr->yflip & 0b1) << 4;
    dst[4] |= (spr->xflip & 0b1) << 3;
    dst[4] |= (spr->tile & 0b111);
    dst[5] = (spr->tile & 0xFF);

    dst[6] = (spr->xpos >> 8) & 0b1;
    dst[7] = (spr->xpos >> 0) & 0xFF;
}

void s_sprite_buffer__push_to_vram() {
    const MdMemoryVram* vram = md_vdp__dma_begin();

    for (int i = 0; i < 0x50; i++) {
        sprite_to_vram__(&sprite_list__[i], vram->plane_sprite_mut->arr + 8 * i);
    }

    md_vdp__dma_end();
}
