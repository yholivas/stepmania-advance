#ifndef ROW_LIST_H
#define ROW_LIST_H

enum notediv {Fourth, Eighth, Twelfth, Sixteenth, Twentyfourth, Thirtysecond};

// notes is just a 4-bit note word
// use an index instead of a tail pointer or something
struct note_row {
    int notes;
    int y;
    enum notediv div;
};

// requires head to not be null
void check_key_presses(struct note_row * head, int * keys);

#endif //ROW_LIST_H
