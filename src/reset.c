#include "reset.h"

void key_reset() { swi_call(0x00); }
