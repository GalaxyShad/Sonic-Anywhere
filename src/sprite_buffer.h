#ifndef SONIC_ANYWHERE_SPRITE_BUFFER_H
#define SONIC_ANYWHERE_SPRITE_BUFFER_H

#include "mappings/sprite_piece.h"

void s_sprite_buffer__reset();
void s_sprite_buffer__add(const GameSpritePiece* spr, i16 x, i16 y);

void s_sprite_buffer__push_to_vram();

#endif // SONIC_ANYWHERE_SPRITE_BUFFER_H
