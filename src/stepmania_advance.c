#include <string.h>
#include <tonc.h>

#include "game_loop.h"
#include "graphics/menu-arrow.h"
#include "graphics/title.h"
#include "reset.h"

struct menu_selection {
    fnptr fn;
    int pos;
};

void draw_title()
{
    memcpy(&tile_mem[0][0], titleTiles, titleTilesLen);
    memcpy(&tile_mem[4][0], menu_arrowTiles, menu_arrowTilesLen);
    memcpy(&se_mem[30][0], titleMap, titleMapLen);
    memcpy(pal_bg_mem, titlePal, titlePalLen);
    pal_obj_mem[1] = menu_arrowPal[1];

    REG_BG0CNT= BG_CBB(0) | BG_SBB(30) | BG_4BPP | BG_REG_32x32;
    REG_DISPCNT= DCNT_OBJ | DCNT_OBJ_1D | DCNT_MODE0 | DCNT_BG0;
}

// idea: create a struct with a menu arrow position and a function pointer
// toggle between structs by pressing up or down and when start is hit jump to the fnptr in that struct

int main()
{
    irq_init(NULL);
    irq_add(II_VBLANK, NULL);
    irq_add(II_KEYPAD, key_reset);
    REG_KEYCNT = 0b1100001100001100;
    irq_enable(II_KEYPAD);

    key_poll();
    draw_title();

    OBJ_ATTR menu_arrow;
    oam_init(oam_mem, 128);
    obj_set_attr(&menu_arrow, ATTR0_SQUARE, ATTR1_SIZE_16, 0);

    struct menu_selection start = { .fn = gameplay, .pos = 109 }; 
    struct menu_selection opts = { .fn = NULL, .pos = 131 };
    struct menu_selection * curr_selection = &start;
    // TODO: function to draw selection arrow, default on START
    // store Y position in a variable, default is OPTIONS
    // U/D button presses flip a flag, -1 or 0, -1 subtracts y to land on START, default is -1
    while(1) {
        VBlankIntrWait();
        key_poll();
        if (key_hit(KEY_START)) {
            irq_disable(II_VBLANK);
            (curr_selection->fn)();
            oam_init(oam_mem, 128);
            draw_title();
            irq_enable(II_VBLANK);
        } else if (key_hit(KEY_UP) || key_hit(KEY_DOWN)) {
            if (curr_selection == &start) curr_selection = &opts;
            else curr_selection = &start;
        }
        obj_set_pos(&menu_arrow, 7, curr_selection->pos);
        oam_copy(oam_mem, &menu_arrow, 1);
    }
}
