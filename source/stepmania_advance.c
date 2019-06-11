#include <string.h>
#include <tonc.h>

#include "lrarrow.h"
#include "udarrow.h"
#include "bg.h"

// copy of object memory
OBJ_ATTR obj_buffer[128];

int main()
{
    // store arrow sprite & bg tile into first character base slot
    memcpy(&tile_mem[4][0], lrarrowTiles, lrarrowTilesLen);
    memcpy(&tile_mem[4][4], udarrowTiles, udarrowTilesLen);
    memcpy(&tile_mem[0][0], bgTiles, bgTilesLen);
	memcpy(&se_mem[30][0], bgMap, bgMapLen);

    // store palettes into object & bg palette mem
    memcpy(pal_obj_mem, lrarrowPal, lrarrowPalLen);
    memcpy(pal_bg_mem, bgPal, bgPalLen);

    oam_init(obj_buffer, 128);
    REG_BG0CNT= BG_CBB(0) | BG_SBB(30) | BG_4BPP | BG_REG_32x32;
    REG_DISPCNT= DCNT_OBJ | DCNT_OBJ_1D | DCNT_MODE0 | DCNT_BG0;

    // setting up arrow sprite
    int y= 144;
    int lx = 85, dx = 103, ux = 121, rx = 139;
    u32 tid= 0, pb= 0;      // tile id, pal-bank
    OBJ_ATTR * larrow = &obj_buffer[0];
    OBJ_ATTR * darrow = &obj_buffer[1];
    OBJ_ATTR * uarrow = &obj_buffer[2];
    OBJ_ATTR * rarrow = &obj_buffer[3];
    obj_set_attr(larrow, ATTR0_SQUARE, ATTR1_SIZE_16, ATTR2_PALBANK(pb) | tid);
    obj_set_attr(darrow, ATTR0_SQUARE, ATTR1_SIZE_16, ATTR2_PALBANK(pb) | 4);
    obj_set_attr(uarrow, ATTR0_SQUARE, ATTR1_SIZE_16 | ATTR1_VFLIP, ATTR2_PALBANK(pb) | 4);
    obj_set_attr(rarrow, ATTR0_SQUARE, ATTR1_SIZE_16 | ATTR1_HFLIP, ATTR2_PALBANK(pb) | 0);

    // setting up guide arrow sprites

    // position sprite
    obj_set_pos(larrow, lx, y);
    obj_set_pos(darrow, dx, y);
    obj_set_pos(uarrow, ux, y);
    obj_set_pos(rarrow, rx, y);

    while (1) {
        vid_vsync();
        y -= 2;
        if (y <= -16 || y > 160) y = 160;
        obj_set_pos(larrow, lx, y);
        obj_set_pos(darrow, dx, y);
        obj_set_pos(uarrow, ux, y);
        obj_set_pos(rarrow, rx, y);
        oam_copy(oam_mem, obj_buffer, 4);
    }

    return 0;
}
