#include <tonc.h>

#include "game_loop.h"
#include "song.h"
#include "setup.h"
#include "row_list.h"
#include "music.h"
#include "buffers.h"
#include "arrows.h"

void gameplay()
{
    int i;

    struct guide_arrow guides[NUM_ARROWS];
    struct note_row rows[MAX_ROWS];

    // allocate mem for objs, copy sprite & bg data to VRAM
    setup_graphics();
    setup_audio();
    setup_guides(guides);

    tte_init_se(0, BG_CBB(0) | BG_SBB(31), 0, CLR_CREAM, 0, NULL, NULL);
    tte_init_con();

    // key to arrow mask
    //int keys[NUM_ARROWS] = {KEY_R, KEY_A | KEY_B, KEY_UP | KEY_DOWN | KEY_RIGHT | KEY_LEFT, KEY_L};
    // for emulator usage:
    int keys[NUM_ARROWS] = {KEY_RIGHT, KEY_UP, KEY_DOWN, KEY_LEFT};

    free_all(rows);

    irq_enable(II_VBLANK);

    int row_frame = 0;
    // TODO: make this a #define or something or make it dependent on some speed setting
    int song_frame = -70;
    int score = 0;
    int song_idx = 0;
    tte_printf("#{es;P}Score: %d", score);

    while (song_idx < SONG_LENGTH) {
        VBlankIntrWait();

        int score_inc = check_key_presses(rows, keys);
        if (score_inc) {
            score += score_inc;
            tte_printf("#{es;P}Score: %d", score);
        }

        // fetch another row
        if (row_frame == FRAME_TEMPO) {
            get_row(rows);
            row_frame = 0;
        }

        if (song_frame == FRAME_TEMPO && song_idx < SONG_LENGTH) {
            REG_SNDDMGCNT &= 0xBBFF;
            REG_SND3CNT = NOTE_DURATION | 1 << 13;
            if (song[song_idx]) REG_SND3FREQ = SFREQ_BUILD(song[song_idx], 1, 1);
            REG_SNDDMGCNT |= 0x4400;
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
    oam_init(oam_mem, 128);

    irq_disable(II_VBLANK);
    free_all(rows);
}
