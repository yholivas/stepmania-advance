
//{{BLOCK(title)

//======================================================================
//
//	title, 256x256@4, 
//	+ palette 16 entries, not compressed
//	+ 125 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 32 + 4000 + 2048 = 6080
//
//	Time-stamp: 2020-11-29, 22:08:40
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.15
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_TITLE_H
#define GRIT_TITLE_H

#define titleTilesLen 4000
extern const unsigned int titleTiles[1000];

#define titleMapLen 2048
extern const unsigned short titleMap[1024];

#define titlePalLen 32
extern const unsigned short titlePal[16];

#endif // GRIT_TITLE_H

//}}BLOCK(title)
