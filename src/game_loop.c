#include <tonc_video.h>
#include <tonc_irq.h>
#include <tonc_bios.h>
#include <tonc_input.h>
#include <tonc_oam.h>

#include "game_loop.h"
#include "graphics.h"
#include "setup.h"
#include "row_list.h"
#include "music.h"
#include "buffers.h"
#include "arrows.h"

#define FRAME_TEMPO 16
#define SONG_LENGTH 50

void gameplay()
{
    int i;

    struct guide_arrow guides[NUM_ARROWS];
    struct note_row rows[MAX_ROWS];

    // allocate mem for objs, copy sprite & bg data to VRAM
    setup_graphics();
    setup_guides(guides);

    // key to arrow mask
    //int keys[NUM_ARROWS] = {KEY_R, KEY_A | KEY_B, KEY_UP | KEY_DOWN | KEY_RIGHT | KEY_LEFT, KEY_L};
    // for emulator usage:
    int keys[NUM_ARROWS] = {KEY_RIGHT, KEY_UP, KEY_DOWN, KEY_LEFT};

    free_all(rows);

    //irq_enable(II_VBLANK);

    fade_from_bw(black, 1);

    int row_frame = 0;
    int song_frame = 0;
    // TODO: make this a #define or something or make it dependent on some speed setting
    int score = 0;
    int song_idx = 0;
    play_music();

    while (song_idx < SONG_LENGTH) {
        VBlankIntrWait();

        int score_inc = check_key_presses(rows, keys);
        if (score_inc) {
            score += score_inc;
        }

        // fetch another row
        if (row_frame == FRAME_TEMPO) {
            get_row(rows);
            row_frame = 0;
        }

	if (song_frame == FRAME_TEMPO) {
		song_idx++;
		song_frame = 0;
	}

        // TODO: use animation state machine for guide arrow shrinkage
        for (i = 0; i < NUM_ARROWS; i++) {
            if (key_hit(keys[i])) obj_aff_scale(guides[i].aff, 0x0180, 0x0180);
            if (key_released(keys[i]) || score_inc) obj_aff_scale(guides[i].aff, 0x0100, 0x0100);
        }
        arrow_flight(rows);

        // hopefully there are no ill effects associated with rewriting the entirety of
        //  OAM memory every frame : )
        oam_copy(oam_mem, obj_buffer, 128);
        obj_aff_copy(obj_aff_mem, obj_aff_buf, 4);
        row_frame++;
	song_frame++;
    }
    stop_music();
    fade_to_bw(black, 1);
    oam_init(oam_mem, 128);

    //irq_disable(II_VBLANK);
    free_all(rows);
}
