#include <tonc.h>

#include "stepmania_advance.h"
#include "setup.h"
#include "row_list.h"

#define L_ARR_POS 85
#define D_ARR_POS 103
#define U_ARR_POS 121
#define R_ARR_POS 139

// copy of object memory
OBJ_ATTR obj_buffer[128];
OBJ_AFFINE obj_aff_buf[4];
// might need if static variables are stored in the cartridge ROM 
//static struct note_row rows[32];
int main()
{
    int i;

    struct guide_arrow guides[NUM_ARROWS];

    // allocate mem for objs, copy sprite & bg data to VRAM
    setup_graphics();

    // sprite arrays
    OBJ_AFFINE * aff_guides[NUM_ARROWS];
    OBJ_ATTR * arrows[NUM_ARROWS], * guide_objs[NUM_ARROWS];
    for (i = 0; i < NUM_ARROWS; i++) {
        arrows[i] = &obj_buffer[i];
        guide_objs[i] = &obj_buffer[4 + i];
        guides[i].aff = &obj_aff_buf[i];
    }

    setup_sprites(arrows, 0, 0, x, y); // tile id, pal-bank
    setup_aff_sprites(guide_arrows, aff_guides, 8, 1, x_guide, y_guide);

    // key to arrow mask
    int keys[NUM_ARROWS] = {KEY_L, KEY_DOWN | KEY_UP | KEY_RIGHT | KEY_LEFT,
        KEY_B | KEY_A, KEY_R};

    struct note_row rows[32];
    int row_idx = 0;
    // clear note row memory just in case
    for (i = 0; i < 32; i++) free_row(rows + (sizeof(struct note_row) * i));

    u32 frames = 0;
    while (1) {
        // use animation state machine for guide arrow shrinkage
        if (!check_key_presses(rows, &row_idx, keys))
            obj_aff_scale(aff_guides[i], 0x0180, 0x0180);
        else 
            obj_aff_scale(aff_guides[i], 0x0100, 0x0100);
        arrow_flight(rows, row_idx);
        for (i = 0; i < NUM_ARROWS; i++) obj_set_pos(arrows[i], x[i], y[i]);
        vid_vsync();
        oam_copy(oam_mem, obj_buffer, 8);
        obj_aff_copy(obj_aff_mem, obj_aff_buf, 4);
        frames++;
    }

    return 0;
}
