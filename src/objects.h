#ifndef SONIC_ANYWHERE_OBJECTS_H
#define SONIC_ANYWHERE_OBJECTS_H

#include "include_backend/tinyint.h"

typedef void* SObjectMappings;

typedef struct {
    u8 x_flip : 1;
    u8 y_flip : 1;
    u8 use_relative : 1; // relative screen position - coordinates are based on the level
    u8 use_abs : 1; // absolute screen position - coordinates are based on the screen (e.g. the HUD)
    u8 align_to_background : 1;
    u8 use_height : 1; // use object height to decide if object is on screen, otherwise height is assumed to be $20 (used for large objects)
    u8 use_raw_map : 1; // sprites use raw mappings - i.e. object consists of a single sprite instead of multipart sprite mappings (e.g. broken block fragments)
    u8 is_sonic_behind_a_loop : 1; // object is behind a loop (Sonic only)
    u8 is_on_screen : 1; // object is on screen
} SObjectRender;

typedef struct {
    u16 x_flip : 1;
    u16 y_flip : 1;
    u16 pal_1 : 2;
    u16 pal_2 : 2;
    u16 pal_3 : 2;
    u16 pal_4 : 2;
    u16 priority : 1;
} SObjectGfx;

typedef union {
    struct {
        u16 px;
        u16 sub_px;
    };
    u32 raw;
} SObjectAxis;

typedef struct {
    u8 current_frame;
    u8 current;
    bool is_restart;
    union {
        struct {
            u8 time_to_next_frame;
            u8 master_copy_of_time_to_next_frame;
        };
        u16 general_timer;
    };
} SObjectAnimation;

typedef struct {
    u8 is_broken : 1; // object has been broken (enemies/bosses)
    u8 is_sonic_underwater : 1;
    u8 is_pushing : 1;        // Sonic is pushing this (objects) / Sonic is pushing an object (Sonic)
    u8 is_sonic_rolljump : 1; // Sonic is jumping after rolling (Sonic only)
    u8 is_platform : 1;       // Sonic is standing on this (objects) / Sonic is standing on object (Sonic)
    u8 is_sonic_jumping_or_rolling : 1;
    u8 is_y_flipped_or_sonic_in_the_air : 1;
    u8 is_x_flipped : 1;
} SObjectStatus;

typedef struct {
    //  Object variables
    u8 id;
    SObjectRender render;
    SObjectGfx gfx;
    SObjectMappings mappings;
    u16 screen_y; // original ROM sometimes using least 2 bytes of x axis (sub_px)
    SObjectAxis x;
    SObjectAxis y;
    i16 vel_x;
    i16 vel_y;
    i16 inertia;
    u8 height_radius;         // height / 2
    u8 width_radius;          // width  / 2
    u8 sprite_stack_priority; // sprite stack priority - 0 is highest, 7 is lowest
    u8 display_width_radius;  // display width/2
    u8 frame;
    SObjectAnimation animation;
    u8 collision_type; // collision response type - 0 = none; 1-$3F = enemy; $41-$7F = items; $81-BF = hurts; $C1-$FF =
                           // custom
    u8 collision_property; // collision extra property
    SObjectStatus status;
    u8 respawn_number;
    u8 routine_number;
} SObjectProps;

typedef struct {
    void (*routine)(SObjectProps*);
    u8 props_space_id;
} SObject;

// ExecuteObjects:
void s_object_pool__all_execute();

void s_object_pool__all_clear_props();

#endif // SONIC_ANYWHERE_OBJECTS_H
