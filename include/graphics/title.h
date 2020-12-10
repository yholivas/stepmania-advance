
//{{BLOCK(title)

//======================================================================
//
//	title, 256x256@8, 
//	+ 128 tiles (t|f reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 8192 + 2048 = 10240
//
//	Time-stamp: 2020-12-09, 22:58:18
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.15
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_TITLE_H
#define GRIT_TITLE_H

#define titleTilesLen 8192
extern const unsigned int titleTiles[2048];

#define titleMapLen 2048
extern const unsigned short titleMap[1024];

#endif // GRIT_TITLE_H

//}}BLOCK(title)

//{{BLOCK(Shared)

//======================================================================
//
//	Shared, 16x16@8, 
//	+ palette 212 entries, not compressed
//	Total size: 424 = 424
//
//	Time-stamp: 2020-12-09, 22:58:18
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.15
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_SHARED_H
#define GRIT_SHARED_H

#define SharedPalLen 424
extern const unsigned short SharedPal[212];

#endif // GRIT_SHARED_H

//}}BLOCK(Shared)
