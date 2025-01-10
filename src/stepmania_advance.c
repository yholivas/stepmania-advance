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

struct menu {
	struct menu_selection* selections;
	int sels_len;
	struct bg_scene *bg;
	int x;
};

struct menu_selection {
	struct menu* (*fn)();
	int pos;
};

struct bg_scene title_bg = {
	.tiles = titleTiles,
	.tiles_len = titleTilesLen,
	.map = titleMap,
	.map_len = titleMapLen,
	.cb = 1,
	.se = 29,
	.ctrl_reg = BG_4BPP | BG_REG_32x32 | BG_PRIO(0)
};

struct bg_scene options_bg = {
	.tiles = optionsTiles,
	.tiles_len = optionsTilesLen,
	.map = optionsMap,
	.map_len = optionsMapLen,
	.cb = NO_OPTS_CB,
	.se = NO_OPTS_SE,
	.ctrl_reg = BG_4BPP | BG_REG_32x32 | BG_PRIO(0)
};

struct menu options_menu;

struct menu* display_options()
{
	memcpy(&tile_mem[1][0], optionsTiles, optionsTilesLen);
	memcpy(&se_mem[29][0], optionsMap, optionsMapLen);

	return &options_menu;
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

struct menu main_menu;

struct menu* display_title()
{
	memcpy(&tile_mem[1][0], titleTiles, titleTilesLen);
	memcpy(&se_mem[29][0], titleMap, titleMapLen);

	return &main_menu;
}

void blank_bg(int bg)
{
	*(REG_BGCNT + 4 * bg) = BG_CBB(bg) | BG_SBB(24) | BG_PRIO(1);
}

struct palette title_pal = {
	.val = SharedPal,
	.len = SharedPalLen
};

struct menu* start_game()
{
	// TODO: fade to black then fade from black to gameplay - probably 1 tick fade
	fade_to_bw(black, 1);
	//irq_disable(II_VBLANK);
	gameplay();
	draw_title();
	write_palette(&title_pal);
	//irq_enable(II_VBLANK);
	fade_from_bw(black, 1);

	return &main_menu;
}

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

struct menu_selection main_sels[2] = {
	{.fn = start_game, .pos = 109},
	{.fn = display_options, .pos = 131}
};

// .x for options menu is 96, 127

struct menu main_menu = {
	.x = 7,
	.sels_len = 2,
	.selections = main_sels,
	.bg = &title_bg
};

struct menu_selection options_sel = {
	.fn = display_title,
	.pos = 127
};

struct menu options_menu = {
	.x = 96,
	.sels_len = 1,
	.selections = &options_sel
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
	REG_BG2HOFS = REG_BG2VOFS = REG_BG0VOFS = REG_BG0HOFS = 0;

	draw_scene(splash_sc);

	fade_from_bw(white, 2);

	// right now it pauses for ~10 seconds to display the GNU splash screen
	// use blending - blend from white background to gnu screen and then blend to
	// title screen
	int i = 590;
	while (i > 0) {
		VBlankIntrWait();
		key_poll();
		i -= (key_hit(KEY_START | KEY_A) << 12);
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
	obj_set_pos(&menu_arrow, 7, 109);
	// draw only title screen, fade to it from bg0
	setup_bg(title_bg);
	write_palette(&title_pal);

	fade_ba(BLD_BG0, 2);

	REG_BLDCNT = 0;

	draw_title();
	/* menu struct
	 * has a collection of menu_selection structs
	 * also needs a background to display
	 * each menu needs an x for the selection arrow thing
	 * probably also a pointer to 
	 */
	int sel_index = 0;
	struct menu *curr_menu = &main_menu;

	// make "no options" background as BG1, toggle when it appears or not in
	// options fn have to figure out how to make it disappear when up/down are
	// pressed
	//
	// also add sounds for when user selects an option
	// causes arrow selector to go to right a bit and then back
	unsigned int x = 0;
	unsigned int x2 = 0;
	while (1) {
		VBlankIntrWait();
		key_poll();
		if (key_hit(KEY_START)) {
			curr_menu = (curr_menu->selections[sel_index].fn)();
			// on switch - have to change curr_menu to new menu
			// and load selections[0].pos
		//} else if (key_hit(KEY_UP)) {
		//	if (--sel_index < 0)
		//		sel_index = curr_menu->sels_len - 1;
		//	obj_set_pos(&menu_arrow, curr_menu->x, curr_menu->selections[sel_index].pos);
		//} else if (key_hit(KEY_DOWN)) {
		//	if (++sel_index >= curr_menu->sels_len)
		//		sel_index = 0;
		//	obj_set_pos(&menu_arrow, curr_menu->x, curr_menu->selections[sel_index].pos);
		}
		sel_index = CLAMP(sel_index + key_tri_vert(), 0, curr_menu->sels_len);
		obj_set_pos(&menu_arrow, curr_menu->x, curr_menu->selections[sel_index].pos);
		x2 += 2;
		// 0x3ff - set to 0 when x > 1023
		REG_BG2HOFS = (x2 >> 2) & 0x3ff;
		REG_BG2VOFS = (++x >> 2) & 0x3ff;
		REG_BG0HOFS = (x >> 1) & 0x3ff;
		REG_BG0VOFS = (x >> 1) & 0x3ff;
		oam_copy(oam_mem, &menu_arrow, 1);
	}
}
