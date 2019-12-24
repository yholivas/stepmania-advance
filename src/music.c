#include <tonc.h>

const unsigned int sound_wave[] =
{
    0x10325476,0x98badcfe,
};

void setup_audio(void)
{
    // enable sound
    REG_SNDSTAT = SSTAT_ENABLE;
    // enable channel 3 at full volume for both channels
    REG_SNDDMGCNT = SDMG_BUILD_LR(SDMG_WAVE, 7);
    // set volume ratio to 100%
    REG_SNDDSCNT = SDS_DMG100;

    REG_SND3SEL = 0b1000000; // write to bank 0
    // copying over sound bank to wave ram registers
    REG_WAVE_RAM0 = sound_wave[0];
    REG_WAVE_RAM1 = sound_wave[1];
    REG_WAVE_RAM2 = sound_wave[0];
    REG_WAVE_RAM3 = sound_wave[1];
    REG_SND3SEL = 0b10000000; // play bank 0
}
