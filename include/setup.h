#ifndef SETUP_H
#define SETUP_H

#include "game_loop.h"
#include "row_list.h"

#define NUM_ARROWS 4

void setup_graphics();

void setup_row(struct note_row * row);

void setup_guides(struct guide_arrow * guides);

#endif //SETUP_H
