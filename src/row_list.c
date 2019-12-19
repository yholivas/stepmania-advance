#include <tonc.h>

#include "arrows.h"
#include "row_list.h"
#include "buffers.h"
#include "setup.h"

#define JUDGE_LO 19
#define JUDGE_HI 13

static int row_idx = 0;
static int obj_idx = 4;

// parameters:
//  TODO: figure out if you should store note rows in a static buffer or the stack
//  struct note_row rows[] - array of note row structs to look in
//  int notes - what notes to allocate objects for
//  enum notediv - what timing the notes follow
// returns:
//  note row pointer with objects filled out
struct note_row * row_alloc(struct note_row * rows, int notes, enum notediv timing)
{
    // TODO: implement searching logic
    //  right now it just returns the first row
    struct note_row * row = &rows[row_idx];
    row_idx = (row_idx + 1) & (NUM_ROWS - 1);
    row->notes = notes;
    row->y = 160;
    row->div = timing;
    // TODO: implement searching for empty objects in obj_buffer
    //  right now it just returns the first four
    // TODO: skip first four objs somehow since they contain the guide sprites
    if (notes & 8) { row->sprites[0] = &obj_buffer[obj_idx]; obj_idx++; }
    if (notes & 4) { row->sprites[1] = &obj_buffer[obj_idx]; obj_idx++; }
    if (notes & 2) { row->sprites[2] = &obj_buffer[obj_idx]; obj_idx++; }
    if (notes & 1) { row->sprites[3] = &obj_buffer[obj_idx]; obj_idx++; }
    setup_row(row);
    return row;
}

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
            if (key_hit(keys[i]) && curr->notes & mask) {
                if (y < JUDGE_LO && y > JUDGE_HI) {
                    free_row(curr);
                    idx++;
                    idx %= NUM_ROWS;
                    *idx_ptr = idx;
                    return true;
                }
            }
        }
        idx++;
        idx %= NUM_ROWS;
        curr = rows + (sizeof(struct note_row) * idx);
        y = curr->y;
    }
    *idx_ptr = idx;
    return false;
}

// make simpler row iteration function that simply decrements y coord in all rows
void arrow_flight(struct note_row * rows) {
    for (int i = 0; i < NUM_ROWS; i++) {
        // TODO: undo comment later if (y <= -16 || y > 160) free_row(curr);
        if (rows[i].notes == 0) continue;
        int y = rows[i].y;
        if (y <= -16 || y > 160) rows[i].y = 160;
        else rows[i].y -= 2;
        for (int j = 0; j  < NUM_ARROWS; j++)
            obj_set_pos(rows[i].sprites[j], x[j], rows[i].y);
    }
}

// row clearing function to serve as free/delete
void free_row(struct note_row * row) {
    row->notes = 0;
    row->y = 160;
    row->div = 0;
}
