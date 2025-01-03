#include <tonc_memmap.h>
#include <tonc_bios.h>
#include <tonc_video.h>
#include "graphics.h"

void setup_bg(struct bg_scene bg, int num)
{
	memcpy32(&tile_mem[bg.cb][0], bg.tiles, bg.tiles_len);
	memcpy16(&se_mem[bg.se][0], bg.map, bg.map_len);

	REG_BGCNT[bg.cb] = BG_CBB(bg.cb) | BG_SBB(bg.se) | bg.ctrl_reg;
}

void write_palette(struct palette *pal)
{
	memcpy16(pal_bg_mem, pal->val, pal->len);
}

void draw_scene(struct scene scene)
{
	write_palette(scene.pal);
	int dispcnt = 0;
	for (int i = 0; i < scene.num_bgs; ++i)
	{
		int num = scene.bgs[i].cb;
		setup_bg(scene.bgs[i], num);
		dispcnt |= 1 << (num + 8);
	}
	REG_DISPCNT = dispcnt;
}

void fade_to_bw(enum bw bw, int ticks)
{
	REG_BLDY = 0;
	REG_BLDCNT = bw | 0x3f;
	for (int i = 1; i <= 16; ++i) {
		int frames = ticks;
		while (frames > 0) {
			VBlankIntrWait();
			frames--;
		}
		REG_BLDY = i;
	}
}

void fade_from_bw(enum bw bw, int ticks)
{
	REG_BLDY = 0b10000;
	REG_BLDCNT = bw | 0x3f;
	for (int i = 16; i >= 0; --i) {
		int frames = ticks;
		while (frames > 0) {
			VBlankIntrWait();
			frames--;
		}
		REG_BLDY = i;
	}
	REG_BLDCNT = 0;
}

void fade_ab(u16 bg_b, int ticks)
{
	REG_BLDCNT = BLD_BUILD(BLD_BG1, bg_b | BLD_BACKDROP, 1);
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

void fade_ba(u16 bg_b, int ticks)
{
	REG_BLDCNT = BLD_BUILD(BLD_BG1, bg_b | BLD_BACKDROP, 1);
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
