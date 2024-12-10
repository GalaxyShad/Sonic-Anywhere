#include "include_backend/debug.h"
#include "sprite_piece.h"

GameSpriteMapping* s1_sprite_mapping(S1SpriteMappingType id) {
    if (id == S1_SPRITE_MAPPING_TYPE__CREDITS)
        return s1_map_cred_internal();

    return 0;
}

GameSprite* s1_sprite(GameSpriteMapping* mapping, int sprite_id) {
    ASSERT(mapping)
    ASSERT(sprite_id >= 0 && sprite_id < mapping->sprites_len)

    return &mapping->sprites[sprite_id];
}


GameSpritePiece* s1_sprite_piece(GameSprite* spr, int piece_id) {
    ASSERT(spr)
    ASSERT(piece_id >= 0 && piece_id < spr->pieces_len)

    return &spr->pieces[piece_id];
}
