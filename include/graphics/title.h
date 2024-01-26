
//{{BLOCK(title)

//======================================================================
//
//	title, 256x256@4, 
//	+ 138 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 4416 + 2048 = 6464
//
//	Time-stamp: 2024-01-26, 07:51:06
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_TITLE_H
#define GRIT_TITLE_H

#define titleTilesLen 4416
extern const unsigned int titleTiles[1104];

#define titleMapLen 2048
extern const unsigned short titleMap[1024];

#endif // GRIT_TITLE_H

//}}BLOCK(title)

//{{BLOCK(Shared)

//======================================================================
//
//	Shared, 16x16@4, 
//	+ palette 16 entries, not compressed
//	Total size: 32 = 32
//
//	Time-stamp: 2024-01-26, 07:51:06
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_SHARED_H
#define GRIT_SHARED_H

#define SharedPalLen 32
extern const unsigned short SharedPal[16];

#endif // GRIT_SHARED_H

//}}BLOCK(Shared)
