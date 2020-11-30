#include <string.h>
#include <tonc.h>

#include "game_loop.h"
#include "graphics/title.h"
#include "reset.h"

void draw_title()
{
    memcpy(&tile_mem[0][0], titleTiles, titleTilesLen);
    memcpy(&se_mem[30][0], titleMap, titleMapLen);
    memcpy(pal_bg_mem, titlePal, titlePalLen);

    REG_BG0CNT= BG_CBB(0) | BG_SBB(30) | BG_4BPP | BG_REG_32x32;
    REG_DISPCNT= DCNT_MODE0 | DCNT_BG0;
}

int main()
{
    irq_init(NULL);
    irq_add(II_VBLANK, NULL);
    irq_add(II_KEYPAD, key_reset);
    REG_KEYCNT = 0b1100001100001100;
    irq_enable(II_KEYPAD);

    key_poll();
    draw_title();

    while(1) {
        VBlankIntrWait();
        key_poll();
        if (key_hit(KEY_START)) {
            irq_disable(II_VBLANK);
            gameplay();
            draw_title();
            irq_enable(II_VBLANK);
        }
    }
}
