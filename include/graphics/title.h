
//{{BLOCK(title)

//======================================================================
//
//	title, 256x256@8, 
//	+ palette 16 entries, not compressed
//	+ 301 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 32 + 19264 + 2048 = 21344
//
//	Time-stamp: 2020-01-03, 18:43:41
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.15
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_TITLE_H
#define GRIT_TITLE_H

#define titleTilesLen 19264
extern const unsigned int titleTiles[4816];

#define titleMapLen 2048
extern const unsigned short titleMap[1024];

#define titlePalLen 32
extern const unsigned short titlePal[16];

#endif // GRIT_TITLE_H

//}}BLOCK(title)
