/* global constants that extend to multiple parts of program go here
 * (mostly things forgotten in tonc library)
 */
#ifndef STEPMANIA_ADVANCE_H
#define STEPMANIA_ADVANCE_H

// guide arrow should contain:
//   4 OAM objects
//   4 affine objects
//   palbank to control color for animation
struct guide_arrow {
    int palbank;
    OBJ_AFFINE * aff;
    OBJ_ATTR * obj;
};

#endif //STEPMANIA_ADVANCE_H
