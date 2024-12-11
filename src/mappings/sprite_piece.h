#ifndef SONIC_ANYWHERE_SPRITE_PIECE_H
#define SONIC_ANYWHERE_SPRITE_PIECE_H

#include "include_backend/tinyint.h"
#include "sprite_mapping_type.h"

typedef struct GameSpritePiece {
    i8 xpos, ypos;
    u8 width, height, tile, xflip, yflip, pal, pri;
} GameSpritePiece;

typedef struct GameSprite {
    size pieces_len;
    GameSpritePiece* pieces;
} GameSprite;

typedef struct GameSpriteMapping {
    size sprites_len;
    GameSprite* sprites;
} GameSpriteMapping;

GameSpriteMapping* s1_sprite_mapping(S1SpriteMappingType id);
GameSprite* s1_sprite(GameSpriteMapping* mapping, int sprite_id);
GameSpritePiece* s1_sprite_piece(GameSprite* spr, int piece_id);

GameSpriteMapping* s1_map_cred_internal();

#endif // SONIC_ANYWHERE_SPRITE_PIECE_H
