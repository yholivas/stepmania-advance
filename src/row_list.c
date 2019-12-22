#include <tonc.h>

#include "arrows.h"
#include "buffers.h"
#include "chart.h"
#include "row_list.h"
#include "setup.h"

#define JUDGE_LO 19
#define JUDGE_HI 13

static int base_idx = 0;
static int next_row = 0;
static int row_idx = 0;
static int obj_idx = 0;

// parameters:
//  struct note_row rows[] - array of note row structs to look in
// returns:
//  note row pointer with objects filled out
struct note_row * get_row(struct note_row * rows)
{
    if (next_row == ROW_COUNT) return NULL;
    struct note_row * row = &rows[row_idx];
    row_idx = (row_idx + 1) & (MAX_ROWS - 1);
    int packed_row = test_chart[next_row];
    next_row++;
    int notes = packed_row & 0b1111;
    row->notes = notes;
    row->y = 160;
    row->div = packed_row >> 4;
    for (int i = 0; i < NUM_ARROWS; i++) {
        if (notes & (1 << i)) {
            row->sprites[i] = &obj_buffer[4 + obj_idx];
            obj_idx++;
            if (obj_idx > 124) obj_idx = 0;
        }
    }
    setup_row(row);
    return row;
}

// check key press function should only operate until end of timing window
// return score for key press
// right now return true for arrow hit or false for arrow not hit
bool check_key_presses(struct note_row * rows, int * keys)
{
    // TODO: check arrows individually or else jumps will be almost impossible to do
    key_poll();
    int mask = 0;
    for (int i = 0; i < NUM_ARROWS; i++) {
        if(key_hit(keys[i])) mask |= 1 << i;
    }
    if (mask == 0) return false;

    for (int i = base_idx; i != row_idx; i = (i + 1) & (MAX_ROWS - 1)) {
        int y = rows[i].y;
        int notes = rows[i].notes;
        if (notes == 0) continue;
        if (y < JUDGE_LO && y > JUDGE_HI) {
            if (notes & mask) {
                free_row(rows, i);
                return true;
            }
        }
    }
    return false;
}

// use 'notes' field of note_row struct to figure out what notes to change position
// use 'y' field of note_row struct to determine position of all notes in row
void arrow_flight(struct note_row * rows) {
    for (int i = 0; i < MAX_ROWS; i++) {
        int notes = rows[i].notes;
        if (notes == 0) continue;
        int y = rows[i].y;
        if (y <= -16 || y > 160) { free_row(rows, i); continue; }
        else y -= 2;
        for (int j = 0; j < NUM_ARROWS; j++)
            obj_set_pos(rows[i].sprites[j], x[j], rows[i].y);
        if (notes & 1) { obj_set_pos(rows[i].sprites[0], x[0], y); }
        if (notes & 2) { obj_set_pos(rows[i].sprites[1], x[1], y); }
        if (notes & 4) { obj_set_pos(rows[i].sprites[2], x[2], y); }
        if (notes & 8) { obj_set_pos(rows[i].sprites[3], x[3], y); }
        rows[i].y = y;
    }
}

// row clearing function to serve as free/delete
void free_row(struct note_row * rows, int idx) {
    struct note_row * row = &rows[idx];
    row->notes = 0;
    row->y = 160;
    row->div = 0;
    for (int i = 0; i < NUM_ARROWS; i++)
        obj_set_attr(row->sprites[i], ATTR0_HIDE, 0, 0);
    if (idx == base_idx) base_idx = (base_idx + 1) & (MAX_ROWS - 1);
}
