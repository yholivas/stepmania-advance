#ifndef SETUP_H
#define SETUP_H

#include "stepmania_advance.h"

#define NUM_ARROWS 4

void setup_graphics();

void setup_row(OBJ_ATTR ** sprites, u32 tid, u32 pb, int * y);

void setup_guides(struct guide_arrow * guides);

#endif //SETUP_H
