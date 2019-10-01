#include <tonc.h>

#include "row_list.h"

#define JUDGE_LO 19
#define JUDGE_HI 13

// check key press function should only operate until end of timing window
// return score for key press
// right now return true for arrow hit or false for arrow not hit
bool check_key_presses(struct note_row * rows, int * idx_ptr, int * keys)
{
    key_poll();
    int i = 0;
    int idx = *idx_ptr;
    struct note_row * curr = rows + (sizeof(struct note_row) * idx);
    int y = curr->y;
    while (curr->notes) {
        for (i = 0; i < NUM_ARROWS; i++) {
            int mask = 1 << i;
            if (key_hit(keys[i]) && head->notes & mask) {
                if (y < JUDGE_LO && y > JUDGE_HI) {
                    free_row(curr);
                    idx++;
                    idx % 32;
                    *idx_ptr = idx;
                    return true;
                }
            }
        }
        idx++;
        idx % 32;
        curr = rows + (sizeof(struct note_row) * idx);
        y = curr->y;
    }
    *idx_ptr = idx;
    return false;
}

// make simpler row iteration function that simply decrements y coord in all rows
void arrow_flight(struct note_row * rows, int idx) {
    struct note_row * curr = rows + (sizeof(struct note_row) * idx);
    int y = curr->y;
    while (curr->notes) {
        if (y <= -16 || y > 160) free_row(curr);
        else curr->y -= 2;
        idx++;
        idx % 32;
        curr = rows + (sizeof(struct note_row) * idx);
        y = curr->y;
    }
}

// row clearing function to serve as free/delete
void free_row(struct note_row * row) {
    row->notes = 0;
    row->y = 160;
    row->div = 0;
}
