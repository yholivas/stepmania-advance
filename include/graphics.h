#pragma once
#include <tonc_types.h>

// probably replace ctrl_reg with bpp and size (32x32 etc) or something
struct bg_scene {
	const unsigned int *tiles;
	int tiles_len;
	const unsigned short *map;
	int map_len;
	int cb;
	int se;
	u16 ctrl_reg;
};

struct palette {
	const unsigned short *val;
	int len;
};

struct scene {
	struct bg_scene *bgs;
	int num_bgs;
	struct palette *pal;
};

void setup_bg(struct bg_scene bg, int num);
void write_palette(struct palette *pal);
void draw_scene(struct scene scene);

enum bw {
	white = 2 << 6,
	black = 3 << 6
};

void fade_to_bw(enum bw bw, int ticks);
void fade_from_bw(enum bw bw, int ticks);
void fade_ab(u16 bg_b, int ticks);
void fade_ba(u16 bg_b, int ticks);
