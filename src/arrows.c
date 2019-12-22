#define L_ARR_POS   85
#define D_ARR_POS   103
#define U_ARR_POS   121
#define R_ARR_POS   139

#define L_GDE_POS   L_ARR_POS
#define D_GDE_POS   D_ARR_POS
#define U_GDE_POS   (U_ARR_POS - 1)
#define R_GDE_POS   R_ARR_POS

#define GDE_Y       16
#define R_GDE_Y     (GDE_Y - 1)

const int y_guide[4] = {R_GDE_Y, GDE_Y, GDE_Y, GDE_Y};
const int x_guide[4] = {R_GDE_POS, U_GDE_POS, D_GDE_POS, L_GDE_POS};
const int x[4] = {R_ARR_POS, U_ARR_POS, D_ARR_POS, L_ARR_POS};
