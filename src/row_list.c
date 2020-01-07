#include <tonc.h>

#include "arrows.h"
#include "buffers.h"
#include "chart.h"
#include "row_list.h"
#include "setup.h"

#define GR_LO     23
#define EX_LO     21
#define FAN_LO    19
#define FAN_HI    13
#define EX_HI     11
#define GR_HI     9

#define GR_SCORE  1
#define EX_SCORE  3
#define FAN_SCORE 5

static int base_idx;
static int next_row;
static int row_idx;
static int obj_idx;

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
int check_key_presses(struct note_row * rows, int * keys)
{
    key_poll();
    int mask = 0;
    int score = 0;
    int i;
    for (i = 0; i < NUM_ARROWS; i++) {
        if(key_hit(keys[i])) mask |= 1 << i;
    }
    if (mask == 0) return 0;

    // TODO: figure out how to print "FANTASTIC" etc and combo
    for (i = base_idx; i != row_idx; i = (i + 1) & (MAX_ROWS - 1)) {
        int y = rows[i].y;
        int notes = rows[i].notes;
        if (notes == 0) continue;
        if ((notes & mask) == notes) {
            if ((y < GR_LO && y > EX_LO) || (y < EX_HI && y > GR_HI)) {
                score = GR_SCORE;
                goto note_hit;
            }
            else if ((y < EX_LO && y > FAN_LO) || (y < FAN_HI && y > EX_HI)) {
                score = EX_SCORE;
                goto note_hit;
            }
            else if (y < FAN_LO && y > FAN_HI) {
                score = FAN_SCORE;
                goto note_hit;
            }
        }
    }
    return 0;

note_hit:
    free_row(rows, i);
    return score;
}

// use 'notes' field of note_row struct to figure out what notes to change position
// use 'y' field of note_row struct to determine position of all notes in row
void arrow_flight(struct note_row * rows)
{
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
void free_row(struct note_row * rows, int idx)
{
    struct note_row * row = &rows[idx];
    row->notes = 0;
    row->y = 160;
    row->div = 0;
    for (int i = 0; i < NUM_ARROWS; i++) {
        obj_set_attr(row->sprites[i], ATTR0_HIDE, 0, 0);
        row->sprites[i] = NULL;
    }
    if (idx == base_idx) base_idx = (base_idx + 1) & (MAX_ROWS - 1);
}

void free_all(struct note_row * rows)
{
    base_idx = 0;
    next_row = 0;
    row_idx = 0;
    obj_idx = 0;
    for (int i = 0; i < 32; i++) free_row(rows, i);
}
