// SpriteMoth.h

#ifndef SPRITE_MOTH_H
#define SPRITE_MOTH_H

struct SprMothInfo {
    // This can be up to 8 bytes
    INT8 vy; // velocity Y
    UINT8 vcount;
    INT8 die;
    UINT8 spawn_count;
    UINT8 vtravelamount;
};

#endif