#include <tonc.h>

#include "row_list.h"

#define JUDGE_LO 19
#define JUDGE_HI 13

// check key press function should only operate until end of timing window
// return score for key press
// right now return true for arrow hit or false for arrow not hit
bool check_key_presses(struct note_row * head, int * keys)
{
    key_poll();
    int i = 0;
    int y = head->y;
    while (head->notes) {
        for (i = 0; i < NUM_ARROWS; i++) {
            int mask = 1 << i;
            if (key_hit(keys[i]) && head->notes & mask) {
                if (y < JUDGE_LO && y > JUDGE_HI)
                    return true;
            }
        }
        head += sizeof(struct note_row);
        y = head->y;
    }
    return false;
}

// make simpler row iteration function that simply decrements y coord in all rows
void( )

// row clearing function to serve as free/delete
void free_row(struct note_row * row) {
    row->notes = 0;
    row->y = 160;
    row->div = 0;
}
