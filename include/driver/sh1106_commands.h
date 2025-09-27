/*
* This file contains a list of macro definitions that makes it 
* easier to send commands to the SH1106.
*
*/

#ifndef _SH1106_COMMANDS_H_
#define _SH1106_COMMANDS_H_

#include <stdint.h>

#define SH1106_DISPLAYOFF           0xAE
#define SH1106_DISPLAYCLKDIVIDE     0xD5
#define SH1106_OSCILLATORFREQ       0x80
#define SH1106_MULTIPLEXRATIO_1     0xA8
#define SH1106_MULTIPLEXRATIO_2     0x3F
#define SH1106_DISPLAYOFFSET_1      0xD3
#define SH1106_DISPLAYOFFSET_2      0x00
#define SH1106_DISPLAYSTARTLINE     0x40
#define SH1106_CHARGEPUMP_1         0xAD
#define SH1106_CHARGEPUMP_2         0x8B
#define SH1106_SEGMENTREMAP         0xA1
#define SH1106_COMOUTSCANDIR        0xC8
#define SH1106_COMPINHWCONFIG_1    0xDA
#define SH1106_COMPINHWCONFIG_2    0x12
#define SH1106_CONTRASTCTRL_1       0x81
#define SH1106_CONTRASTCTRL_2       0xFF
#define SH1106_PRECHARGEPERIOD_1    0xD9
#define SH1106_PRECHARGEPERIOD_2    0x1F
#define SH1106_VCOMHDESELLVL_1      0xDB
#define SH1106_VCOMHDESELLVL_2      0x40
#define SH1106_VPP                  0x33
#define SH1106_NORMALINVDISPL       0xA6
#define SH1106_DISPLAYON            0xAF

#define SH1106_PAGE0                0xB0
#define SH1106_PAGE_OFFSET(x)       (SH1106_PAGE0 + x)

extern const uint8_t init_config_steps[22];

#endif // _SH1106_COMMANDS_H_
