#include <string.h>
#include <tonc.h>

#include "game_loop.h"
#include "graphics.h"
#include "graphics/bg-arrows.h"
#include "graphics/gnu-splash-screen.h"
#include "graphics/menu-arrow.h"
#include "graphics/options.h"
#include "graphics/fg-arrows.h"
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

#define SEL_CB	    4

struct menu_selection {
	fnptr fn;
	int pos;
};

// TODO: options popup
//  one idea (which it sorta does now) is to replace the title screen
//   leave the background scrolling arrows alone
//   just copy no options tile + map to cb 1
//   have to reset it when returning to title screen

// later: probably replace tilemem[num] with bg.cb and se_mem[30-num] with bg.se
void setup_options()
{
	memcpy(&tile_mem[NO_OPTS_CB][0], optionsTiles, optionsTilesLen);
	memcpy(&se_mem[NO_OPTS_SE][0], optionsMap, optionsMapLen);

	REG_BG1CNT = BG_CBB(NO_OPTS_CB) | BG_SBB(NO_OPTS_SE) | BG_4BPP | BG_REG_32x32 | BG_PRIO(0);
}

void draw_title()
{
	memcpy(&tile_mem[2][0], bg_arrowsTiles, bg_arrowsTilesLen);
	memcpy(&se_mem[28][0], bg_arrowsMap, bg_arrowsMapLen);

	memcpy(&tile_mem[TITLE_CB][0], fg_arrowsTiles, fg_arrowsTilesLen);
	memcpy(&se_mem[TITLE_SE][0], fg_arrowsMap, fg_arrowsMapLen);

	memcpy(&tile_mem[SEL_CB][0], menu_arrowTiles, menu_arrowTilesLen);
	pal_obj_mem[1] = menu_arrowPal[1];
	pal_obj_mem[2] = menu_arrowPal[2];

	REG_BG0CNT= BG_CBB(TITLE_CB) | BG_SBB(TITLE_SE) | BG_4BPP | BG_REG_32x32 | BG_PRIO(1);
	REG_BG2CNT = BG_CBB(2) | BG_SBB(28) | BG_4BPP | BG_REG_32x32 | BG_PRIO(2);

	//setup_options();
	REG_DISPCNT= DCNT_OBJ | DCNT_OBJ_1D | DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_BG2;
}

void blank_bg(int bg)
{
	*(REG_BGCNT + 4 * bg) = BG_CBB(bg) | BG_SBB(24) | BG_PRIO(1);
}

void start_game()
{
	// TODO: fade to black then fade from black to gameplay - probably 1 tick fade
	fade_to_bw(black, 1);
	//irq_disable(II_VBLANK);
	gameplay();
	draw_title();
	//irq_enable(II_VBLANK);
	fade_from_bw(black, 1);
}
/* draw selection arrow, default on START
 * menu_selection struct stores a menu arrow position and a function pointer
 * toggle between structs by pressing up or down and when start is hit jump to the fnptr in that struct
 * TODO: make a more complicated list or array to iterate over when up/down are pressed
 */

struct bg_scene title_bg = {
	.tiles = titleTiles,
	.tiles_len = titleTilesLen,
	.map = titleMap,
	.map_len = titleMapLen,
	.cb = 1,
	.se = 29,
	.ctrl_reg = BG_4BPP | BG_REG_32x32 | BG_PRIO(0)
};

struct palette title_pal = {
	.val = SharedPal,
	.len = SharedPalLen
};

struct bg_scene splash_bg = {
	.tiles = gnu_splash_screenTiles,
	.tiles_len = gnu_splash_screenTilesLen,
	.map = gnu_splash_screenMap,
	.map_len = gnu_splash_screenMapLen,
	.cb = 1,
	.se = 27,
	.ctrl_reg = BG_4BPP | BG_REG_32x32 | BG_PRIO(0)
};

struct palette splash_pal = {
	.val = gnu_splash_screenPal,
	.len = gnu_splash_screenPalLen
};

struct scene splash_sc = {
	.bgs = &splash_bg,
	.num_bgs = 1,
	.pal = &splash_pal
};

int main()
{
	irq_init(NULL);
	irq_add(II_VBLANK, NULL);
	irq_add(II_KEYPAD, key_reset);
	REG_KEYCNT = 0b1100001100001100; // L+R+St+Sel
	irq_enable(II_KEYPAD);
	key_poll();
	REG_DISPCNT = DCNT_BLANK;

	draw_scene(splash_sc);

	fade_from_bw(white, 2);

	// right now it pauses for ~10 seconds to display the GNU splash screen
	// use blending - blend from white background to gnu screen and then blend to title screen
	int i = 590;
	while (i > 0) {
		VBlankIntrWait();
		key_poll();
		i -= (key_hit(KEY_START | KEY_A) << 12) ;
		i--;
	}

	// use screen entry 24 - should be all zeroes
	// set bg0 to use this screen entry - needs prio1
	// cb[0][0] should be a blank square anyway
	blank_bg(0);	

	fade_ab(BLD_BG0, 2);

	OBJ_ATTR menu_arrow;
	oam_init(oam_mem, 128);
	obj_set_attr(&menu_arrow, ATTR0_SQUARE | ATTR0_BLEND, ATTR1_SIZE_16, 0);
	// draw only title screen, fade to it from bg0
	setup_bg(title_bg, 1); // num is cb
	write_palette(&title_pal);

	fade_ba(BLD_BG0, 2);

	REG_BLDCNT = 0;

	draw_title();
	struct menu_selection start = { .fn = start_game, .pos = 109 }; 
	// TODO: make options its own little loop, still increments bg2ofs though
	struct menu_selection opts = { .fn = setup_options, .pos = 131 };
	struct menu_selection * curr_selection = &start;

	// make "no options" background as BG1, toggle when it appears or not in options fn
	// have to figure out how to make it disappear when up/down are pressed
	//
	// also add sounds for when user selects an option
	// causes arrow selector to go to right a bit and then back
	unsigned int x = 0;
	unsigned int x2 = 0;
	while (1) {
		VBlankIntrWait();
		key_poll();
		if (key_hit(KEY_START)) {
			(curr_selection->fn)();
			// todo: move the draw_title call to a separate function for starting the game
		} else if (key_hit(KEY_UP | KEY_DOWN)) {
			if (curr_selection == &start) curr_selection = &opts;
			else curr_selection = &start;
		}
		x2 += 2;
		// 0x3ff - set to 0 when x > 1023
		REG_BG2HOFS = (x2 >> 2) & 0x3ff;
		REG_BG2VOFS = (++x >> 2) & 0x3ff;
		REG_BG0HOFS = (x >> 1) & 0x3ff;
		REG_BG0VOFS = (x >> 1) & 0x3ff;
		obj_set_pos(&menu_arrow, 7, curr_selection->pos);
		oam_copy(oam_mem, &menu_arrow, 1);
	}
}
