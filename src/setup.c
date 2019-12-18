#include <string.h>
#include <tonc.h>

#include "arrows.h"
#include "buffers.h"
#include "setup.h"
#include "stepmania_advance.h"

#include "lrarrow.h"
#include "udarrow.h"
#include "lguide.h"
#include "dguide.h"
#include "uguide.h"
#include "rguide.h"
#include "bg.h"

#define GDE_TID 8
#define GDE_PB  1

void setup_graphics()
{
    // store arrow sprite & bg tile into first character base slot
    // arrows take up 2x2 = 4 tiles as they are 16x16 sprites
    memcpy(&tile_mem[4][0], lrarrowTiles, lrarrowTilesLen);
    memcpy(&tile_mem[4][4], udarrowTiles, udarrowTilesLen);
    memcpy(&tile_mem[4][8], lguideTiles, lguideTilesLen);
    memcpy(&tile_mem[4][12], dguideTiles, dguideTilesLen);
    memcpy(&tile_mem[4][16], uguideTiles, uguideTilesLen);
    memcpy(&tile_mem[4][20], rguideTiles, rguideTilesLen);

    // don't need until bg is more complicated than a single color
    //memcpy(&tile_mem[0][0], bgTiles, bgTilesLen);
	//memcpy(&se_mem[30][0], bgMap, bgMapLen);

    // store palettes into object & bg palette mem
    // each palette entry is 16 bits, get 16 palette entries per sprite
    memcpy(pal_obj_mem, lrarrowPal, lrarrowPalLen);
    memcpy(&pal_obj_mem[16], lguidePal, lguidePalLen);
    *pal_bg_mem = bgPal[0];
    //memcpy(pal_bg_mem, bgPal, bgPalLen);

    oam_init(obj_buffer, 128);
    REG_BG0CNT= BG_CBB(0) | BG_SBB(30) | BG_4BPP | BG_REG_32x32;
    REG_DISPCNT= DCNT_OBJ | DCNT_OBJ_1D | DCNT_MODE0 | DCNT_BG0;
}

void setup_row(struct note_row * row)
{
    // TODO: replace fixed pb and tid with ones based on timing/notediv
    u32 tid = 0;
    u32 pb = 0;

    obj_set_attr(row->sprites[0], ATTR0_SQUARE, ATTR1_SIZE_16,
            ATTR2_PALBANK(pb) | tid);
    obj_set_attr(row->sprites[1], ATTR0_SQUARE, ATTR1_SIZE_16,
            ATTR2_PALBANK(pb) | (tid+4));
    obj_set_attr(row->sprites[2], ATTR0_SQUARE, ATTR1_SIZE_16 | ATTR1_VFLIP,
            ATTR2_PALBANK(pb) | (tid+4));
    obj_set_attr(row->sprites[3], ATTR0_SQUARE, ATTR1_SIZE_16 | ATTR1_HFLIP,
            ATTR2_PALBANK(pb) | tid);

    // position sprites
    int i;
    for (i = 0; i < NUM_ARROWS; i++) obj_set_pos(row->sprites[i], x[i], row->y);
}

void setup_guides(struct guide_arrow * guides)
{
    int i;
    for (i = 0; i < NUM_ARROWS; i++) {
        obj_set_attr(guides[i].obj, ATTR0_SQUARE | ATTR0_AFF, ATTR1_SIZE_16 | ATTR1_AFF_ID(i),
                ATTR2_PALBANK(GDE_PB) | (GDE_TID + (4 * i)) | ATTR2_PRIO(3));
        obj_set_pos(guides[i].obj, x_guide[i], y_guide[i]);
        obj_aff_identity(guides[i].aff);
    }
}
