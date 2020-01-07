#ifndef ROW_LIST_H
#define ROW_LIST_H

#define MAX_ROWS 32

enum notediv {Fourth, Eighth, Twelfth, Sixteenth, Twentyfourth, Thirtysecond};

// notes is just a 4-bit note word
// use an index instead of a tail pointer or something
struct note_row {
    int notes;
    int y;
    enum notediv div;
    OBJ_ATTR * sprites[4];
};

struct note_row * get_row(struct note_row * rows);

// requires head to not be null
int check_key_presses(struct note_row * rows, int * keys);

void arrow_flight(struct note_row * rows);

void free_row(struct note_row * rows, int idx);

void free_all(struct note_row * rows);

#endif //ROW_LIST_H
