#include <tonc.h>

#include "stepmania_advance.h"
#include "setup.h"

#define L_ARR_POS 85
#define D_ARR_POS 103
#define U_ARR_POS 121
#define R_ARR_POS 139

// copy of object memory
OBJ_ATTR obj_buffer[128];
OBJ_AFFINE obj_aff_buf[4];
int main()
{
    int i;

    setup_graphics();

    // starting sprite positions
    int y_guide[NUM_ARROWS] = {16, 16, 16, 14};
    int x_guide[NUM_ARROWS] = {L_ARR_POS, D_ARR_POS, U_ARR_POS - 2, R_ARR_POS};
    int y[NUM_ARROWS] = {160, 160, 160, 160};
    int x[NUM_ARROWS] = {L_ARR_POS, D_ARR_POS, U_ARR_POS, R_ARR_POS};

    // sprite arrays
    OBJ_AFFINE * aff_guides[NUM_ARROWS];
    OBJ_ATTR * arrows[NUM_ARROWS], * guide_arrows[NUM_ARROWS];
    for (i = 0; i < NUM_ARROWS; i++) {
        arrows[i] = &obj_buffer[i];
        guide_arrows[i] = &obj_buffer[4 + i];
        aff_guides[i] = &obj_aff_buf[i];
    }

    setup_sprites(arrows, 0, 0, x, y); // tile id, pal-bank
    //setup_sprites(guide_arrows, 8, 1, x, y_guide); // tile id, pal-bank
    setup_aff_sprites(guide_arrows, aff_guides, 8, 1, x_guide, y_guide);

    while (1) {
        vid_vsync();
        for (i = 0; i < NUM_ARROWS; i++) {
            y[i] -= 2;
            if (y[i] <= -16 || y[i] > 160) y[i] = 160;
        }
        for (i = 0; i < NUM_ARROWS; i++) obj_set_pos(arrows[i], x[i], y[i]);
        oam_copy(oam_mem, obj_buffer, 8);
        obj_aff_copy(obj_aff_mem, obj_aff_buf, 4);
    }

    return 0;
}
