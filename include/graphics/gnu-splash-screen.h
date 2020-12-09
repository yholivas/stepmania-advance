
//{{BLOCK(gnu_splash_screen)

//======================================================================
//
//	gnu_splash_screen, 256x256@4, 
//	+ palette 16 entries, not compressed
//	+ 204 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 32 + 6528 + 2048 = 8608
//
//	Time-stamp: 2020-12-04, 11:56:32
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.15
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_GNU_SPLASH_SCREEN_H
#define GRIT_GNU_SPLASH_SCREEN_H

#define gnu_splash_screenTilesLen 6528
extern const unsigned int gnu_splash_screenTiles[1632];

#define gnu_splash_screenMapLen 2048
extern const unsigned short gnu_splash_screenMap[1024];

#define gnu_splash_screenPalLen 32
extern const unsigned short gnu_splash_screenPal[16];

#endif // GRIT_GNU_SPLASH_SCREEN_H

//}}BLOCK(gnu_splash_screen)
