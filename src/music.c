#include <tonc.h>

#include "music.h"
#include "song.h"

void stop_music()
{
	REG_TM0CNT_H = 0;
	REG_DMA1CNT_H = 0;
}

void play_music()
{
	REG_SOUNDCNT_L = 0; // disable dmg sound
	// set all sound to 100%, [8,9] enable left and right dma sound, dma sound A use timer 0, reset A FIFO
	REG_SOUNDCNT_H = 0x0b0e; 
	REG_SOUNDCNT_X = 0x0080; // set bit 7 to enable FIFO

	REG_DMA1SAD = (unsigned long)gba_eurobeat_30k_raw; // source DMA
	REG_DMA1DAD = 0x040000a0; // dest DMA dma sound A FIFO
	// bit 9 - repeat
	// bit 10 - transfer type 32 bit
	// bit 12-13 - dma start timing on sound FIFO (only does this with DMA1/DMA2)
	// bit 15 - dma enable
	REG_DMA1CNT_H = 0xb600;

	/* about dma start timing mode:
	 * when sound controller requests DMA, 4 words (16 bytes) are transferred
	 * dest address not incremented (because it's doing a sound FIFO transfer)
	 */

	REG_TM0CNT_L = 0xfdf3; // 0xffff - this value = cpu freq / sampling freq
	REG_TM0CNT_H = 0x0080; // sets bit 7 - start the timer
}
