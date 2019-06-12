#ifndef SETUP_H
#define SETUP_H

#define NUM_ARROWS 4

void setup_graphics();

void setup_sprites(OBJ_ATTR ** sprites, u32 tid, u32 pb, int * x, int * y);

void setup_aff_sprites(OBJ_ATTR ** sprites, OBJ_AFFINE ** aff_attr,
        u32 tid, u32 pb, int * x, int * y);

#endif //SETUP_H
