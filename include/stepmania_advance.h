/* global constants that extend to multiple parts of program go here
 * (mostly things forgotten in tonc library)
 */
#ifndef STEPMANIA_ADVANCE_H
#define STEPMANIA_ADVANCE_H

struct guide_arrow {
    int palbank;
    OBJ_AFFINE * aff;
};

extern OBJ_ATTR obj_buffer[128];
extern OBJ_AFFINE obj_aff_buf[4];

#endif //STEPMANIA_ADVANCE_H
