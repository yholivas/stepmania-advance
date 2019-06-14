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
    int y_guide[NUM_ARROWS] = {16, 16, 16, 15};
    int x_guide[NUM_ARROWS] = {L_ARR_POS, D_ARR_POS, U_ARR_POS - 1, R_ARR_POS};
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

    // mapping keys to arrows
    int keys[NUM_ARROWS] = {KEY_L, KEY_DOWN | KEY_UP | KEY_RIGHT | KEY_LEFT,
        KEY_B | KEY_A, KEY_R};

    u32 frames = 0;
    while (1) {
        key_poll();
        for (i = 0; i < NUM_ARROWS; i++) {
            if (key_hit(keys[i])) {
                if (y[i] < 19 && y[i] > 13)
                    y[i] = 160;
                else
                    obj_aff_scale(aff_guides[i], 0x0180, 0x0180);
            }
            if (key_released(keys[i])) obj_aff_scale(aff_guides[i], 0x0100, 0x0100);
            if (y[i] == 160) {
                if ((frames & 63) == (i * 16)) y[i] -= 2;
            } else if (y[i] <= -16 || y[i] > 160) {
                y[i] = 160;
            } else {
                y[i] -= 2;
            }
        }
        for (i = 0; i < NUM_ARROWS; i++) obj_set_pos(arrows[i], x[i], y[i]);
        vid_vsync();
        oam_copy(oam_mem, obj_buffer, 8);
        obj_aff_copy(obj_aff_mem, obj_aff_buf, 4);
        frames++;
    }

    return 0;
}
