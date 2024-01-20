#include <string.h>
#include <tonc.h>

#include "game_loop.h"
#include "graphics/gnu-splash-screen.h"
#include "graphics/menu-arrow.h"
#include "graphics/no-options.h"
#include "graphics/title.h"
#include "reset.h"
#include "tonc_bios.h"
#include "tonc_memdef.h"
#include "tonc_memmap.h"

#define NO_OPTS_X   104
#define NO_OPTS_Y    93
#define NO_OPTS_CB    1
#define NO_OPTS_SE   29

#define TITLE_CB      0
#define TITLE_SE     30

#define SEL_CB        4

struct menu_selection {
    fnptr fn;
    int pos;
};

void draw_options()
{
    REG_BG1HOFS = -NO_OPTS_X;
    REG_BG1VOFS = -NO_OPTS_Y;
}

void setup_options()
{
    memcpy(&tile_mem[NO_OPTS_CB][0], no_optionsTiles, no_optionsTilesLen);
    memcpy(&se_mem[NO_OPTS_SE][0], no_optionsMap, no_optionsMapLen);
    REG_BG1HOFS = NO_OPTS_X;
    REG_BG1VOFS = NO_OPTS_Y;

    REG_BG1CNT = BG_CBB(NO_OPTS_CB) | BG_SBB(NO_OPTS_SE) | BG_8BPP | BG_REG_32x32 | BG_PRIO(0);
}

void draw_title()
{
    memcpy(&tile_mem[TITLE_CB][0], titleTiles, titleTilesLen);
    memcpy(&tile_mem[SEL_CB][0], menu_arrowTiles, menu_arrowTilesLen);
    memcpy(&se_mem[TITLE_SE][0], titleMap, titleMapLen);
    memcpy(pal_bg_mem, SharedPal, SharedPalLen);
    pal_obj_mem[1] = menu_arrowPal[1];

    REG_BG0CNT= BG_CBB(TITLE_CB) | BG_SBB(TITLE_SE) | BG_8BPP | BG_REG_32x32 | BG_PRIO(1);

    //setup_options();
    //REG_DISPCNT= DCNT_OBJ | DCNT_OBJ_1D | DCNT_MODE0 | DCNT_BG0 | DCNT_BG1;
}

void start_game()
{
    gameplay();
    draw_title();
}

void draw_splash()
{
    memcpy(&tile_mem[0][0], gnu_splash_screenTiles, gnu_splash_screenTilesLen);
    memcpy(&se_mem[30][0], gnu_splash_screenMap, gnu_splash_screenMapLen);
    memcpy(pal_bg_mem, gnu_splash_screenPal, gnu_splash_screenPalLen);

    REG_BG0CNT= BG_CBB(0) | BG_SBB(30) | BG_4BPP | BG_REG_32x32;
    REG_DISPCNT=  DCNT_MODE0 | DCNT_BG0;
}

void draw_solid_screen(/*int bg_num, const unsigned short color*/)
{
	pal_bg_mem[16] = gnu_splash_screenPal[0];
	REG_BG1CNT = BG_CBB(1) | BG_SBB(29);
}

void fade_ab(int ticks)
{
	REG_BLDCNT = BLD_BUILD(BLD_BG0, BLD_BG1 | BLD_BACKDROP, 1);
	REG_BLDALPHA = BLDA_BUILD(16, 0);
	for (int i = 0; i <= 16; ++i) {
		int frames = ticks;
		while (frames > 0) {
			VBlankIntrWait();
			frames--;
		}
		REG_BLDALPHA = BLDA_BUILD(16 - i, i);
	}
}

void fade_ba(int ticks)
{
	REG_BLDCNT = BLD_BUILD(BLD_BG0 | BLD_BACKDROP, BLD_BG1 | BLD_BACKDROP, 1);
	REG_BLDALPHA = BLDA_BUILD(0, 16);
	for (int i = 0; i <= 16; ++i) {
		int frames = ticks;
		while (frames > 0) {
			VBlankIntrWait();
			frames--;
		}
		REG_BLDALPHA = BLDA_BUILD(i, 16 - i);
	}
}

void fade_from_bw(int ticks)
{
	for (int i = 16; i >= 0; --i) {
		int frames = ticks;
		while (frames > 0) {
			VBlankIntrWait();
			frames--;
		}
		REG_BLDY = i;
	}
}

/* draw selection arrow, default on START
 * menu_selection struct stores a menu arrow position and a function pointer
 * toggle between structs by pressing up or down and when start is hit jump to the fnptr in that struct
 * TODO: make a more complicated list or array to iterate over when up/down are pressed
 */
int main()
{
    irq_init(NULL);
    irq_add(II_VBLANK, NULL);
    irq_add(II_KEYPAD, key_reset);
    REG_KEYCNT = 0b1100001100001100;
    irq_enable(II_KEYPAD);
    key_poll();

    REG_BLDCNT = BLD_WHITE | BLD_BG0 | BLD_BACKDROP;
    REG_BLDY = 0b10000;
    draw_splash();

    fade_from_bw(2);

    // right now it pauses for ~10 seconds to display the GNU splash screen
    // use blending - blend from white background to gnu screen and then blend to title screen
    int i = 590;
    while (i > 0) {
        VBlankIntrWait();
        key_poll();
        i -= (key_hit(KEY_START | KEY_A) << 12) ;
        i--;
    }

    draw_solid_screen();

    fade_ab(2);

    // this is a bit sus, probably should wait for vblank before clearing blend & switching bgs 
    // draw only title screen, fade to it from bg0
    draw_title();

    fade_ba(2);

    REG_BLDCNT = 0;
    // then draw options
    setup_options();
    REG_DISPCNT= DCNT_OBJ | DCNT_OBJ_1D | DCNT_MODE0 | DCNT_BG0 | DCNT_BG1;

    OBJ_ATTR menu_arrow;
    oam_init(oam_mem, 128);
    obj_set_attr(&menu_arrow, ATTR0_SQUARE, ATTR1_SIZE_16, 0);

    struct menu_selection start = { .fn = start_game, .pos = 109 }; 
    struct menu_selection opts = { .fn = draw_options, .pos = 131 };
    struct menu_selection * curr_selection = &start;

    // TODO: make "no options" background as BG1, toggle when it appears or not in options fn
    // have to figure out how to make it disappear when up/down are pressed
    while(1) {
        VBlankIntrWait();
        key_poll();
        if (key_hit(KEY_START)) {
            irq_disable(II_VBLANK);
            (curr_selection->fn)();
            // todo: move the draw_title call to a separate function for starting the game
            irq_enable(II_VBLANK);
        } else if (key_hit(KEY_UP | KEY_DOWN)) {
            if (curr_selection == &start) curr_selection = &opts;
            else curr_selection = &start;
        }
        obj_set_pos(&menu_arrow, 7, curr_selection->pos);
        oam_copy(oam_mem, &menu_arrow, 1);
    }
}
