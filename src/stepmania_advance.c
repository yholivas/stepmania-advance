#include <tonc.h>

#include "stepmania_advance.h"
#include "setup.h"
#include "row_list.h"
#include "buffers.h"
#include "arrows.h"

// might need if static variables are stored in the cartridge ROM 
//static struct note_row rows[32];
int main()
{
    int i;

    struct guide_arrow guides[NUM_ARROWS];
    struct note_row rows[NUM_ROWS];

    // allocate mem for objs, copy sprite & bg data to VRAM
    setup_graphics();

    for (i = 0; i < NUM_ARROWS; i++) {
        //arrows[i] = &obj_buffer[i];
        guides[i].obj = &obj_buffer[i];
        guides[i].aff = &obj_aff_buf[i];
    }

    //setup_sprites(arrows, 0, 0, x, y); // tile id, pal-bank
    setup_guides(guides);

    // key to arrow mask
    int keys[NUM_ARROWS] = {KEY_L, KEY_DOWN | KEY_UP | KEY_RIGHT | KEY_LEFT,
        KEY_B | KEY_A, KEY_R};

    // clear note row memory just in case
    for (i = 0; i < 32; i++) free_row(rows + (sizeof(struct note_row) * i));

    int frame = 0;

    while (1) {
        // use animation state machine for guide arrow shrinkage
        // also need to keep track of which arrow was pressed (rather than passing a boolean)
        /*
        if (!check_key_presses(rows, &row_idx, keys))
            obj_aff_scale(guides[i].aff, 0x0180, 0x0180);
        else 
            obj_aff_scale(guides[i].aff, 0x0100, 0x0100);
        */
        if ((frame & 63) == 0) get_row(rows);

        key_poll();
        for (i = 0; i < NUM_ARROWS; i++) {
            if (key_hit(keys[i])) obj_aff_scale(guides[i].aff, 0x0180, 0x0180);
            if (key_released(keys[i])) obj_aff_scale(guides[i].aff, 0x0100, 0x0100);
        }
        arrow_flight(rows);
        vid_vsync();
        // hopefully there are no ill effects associated with rewriting the entirety of
        //  OAM memory every frame : )
        oam_copy(oam_mem, obj_buffer, 128);
        obj_aff_copy(obj_aff_mem, obj_aff_buf, 4);
        frame++;
    }

    return 0;
}
