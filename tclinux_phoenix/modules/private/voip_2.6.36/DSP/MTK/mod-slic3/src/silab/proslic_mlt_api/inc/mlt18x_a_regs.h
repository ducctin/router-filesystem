/*
** Copyright (c) 2013 by Silicon Laboratories
**
** $Id: mlt18x_a_regs.h 4551 2014-10-27 20:57:24Z nizajerk $
**
*/
/*! \file mlt18x_a_regs.h
**
**  \brief Register/RAM map abstracted for MLT
**
**  \author Silicon Laboratories, Inc.
**
**  \attention
**  This file contains proprietary information. 
**  No dissemination allowed without prior written permission
**  from Silicon Laboratories, Inc.
**
**  This file is auto generated. Do no modify.
**
*/
#ifndef MLT18X_A_REGS_H
#define MLT18X_A_REGS_H


/*
** MLT18X_A Unique SPI Registers
*/
enum MLT18X_A_REG {
MLT18X_A_REG_DIAG3                       = 74,
MLT18X_A_REG_PCLK_FAULT_CNTL             = 76,
};

/*
** MLT18X_A_RAM
*/
enum MLT18X_A_RAM {
MLT18X_A_RAM_INIT_GUESS                  = 15,
MLT18X_A_RAM_Y1                          = 16,
MLT18X_A_RAM_Y2                          = 17,
MLT18X_A_RAM_Y3                          = 18,
MLT18X_A_RAM_UNUSED19                    = 19,
MLT18X_A_RAM_METER_LPF_OUT               = 328,
MLT18X_A_RAM_UNUSED333                   = 333,
MLT18X_A_RAM_UNUSED334                   = 334,
MLT18X_A_RAM_I_SOURCE1                   = 385,
MLT18X_A_RAM_I_SOURCE2                   = 386,
MLT18X_A_RAM_VTR1                        = 387,
MLT18X_A_RAM_VTR2                        = 388,
MLT18X_A_RAM_STOP_TIMER1                 = 389,
MLT18X_A_RAM_STOP_TIMER2                 = 390,
MLT18X_A_RAM_UNUSED391                   = 391,
MLT18X_A_RAM_UNUSED392                   = 392,
MLT18X_A_RAM_UNUSED416                   = 416,
MLT18X_A_RAM_UNUSED440                   = 440,
MLT18X_A_RAM_UNUSED443                   = 443,
MLT18X_A_RAM_UNUSED459                   = 459,
MLT18X_A_RAM_UNUSED487                   = 487,
MLT18X_A_RAM_UNUSED488                   = 488,
MLT18X_A_RAM_UNUSED489                   = 489,
MLT18X_A_RAM_UNUSED490                   = 490,
MLT18X_A_RAM_UNUSED491                   = 491,
MLT18X_A_RAM_CAL_TEMP11                  = 501,
MLT18X_A_RAM_METER_RAMP                  = 502,
MLT18X_A_RAM_METER_RAMP_DIR              = 503,
MLT18X_A_RAM_METER_ON_T                  = 504,
MLT18X_A_RAM_METER_PK_DET                = 505,
MLT18X_A_RAM_METER_PK_DET_T              = 506,
MLT18X_A_RAM_THERM_CNT                   = 507,
MLT18X_A_RAM_VDIFF_SENSE_DELAY           = 508,
MLT18X_A_RAM_RING_INTERP_DIFF_SYNC       = 509,
MLT18X_A_RAM_CPUMP_DEB_CNT               = 510,
MLT18X_A_RAM_UNUSED757                   = 757,
MLT18X_A_RAM_UNUSED758                   = 758,
MLT18X_A_RAM_UNUSED760                   = 760,
MLT18X_A_RAM_UNUSED761                   = 761,
MLT18X_A_RAM_UNUSED762                   = 762,
MLT18X_A_RAM_UNUSED763                   = 763,
MLT18X_A_RAM_UNUSED778                   = 778,
MLT18X_A_RAM_UNUSED779                   = 779,
MLT18X_A_RAM_UNUSED792                   = 792,
MLT18X_A_RAM_UNUSED793                   = 793,
MLT18X_A_RAM_UNUSED794                   = 794,
MLT18X_A_RAM_UNUSED795                   = 795,
MLT18X_A_RAM_UNUSED796                   = 796,
MLT18X_A_RAM_UNUSED797                   = 797,
MLT18X_A_RAM_UNUSED798                   = 798,
MLT18X_A_RAM_UNUSED799                   = 799,
MLT18X_A_RAM_UNUSED800                   = 800,
MLT18X_A_RAM_UNUSED870                   = 870,
MLT18X_A_RAM_UNUSED872                   = 872,
MLT18X_A_RAM_UNUSED873                   = 873,
MLT18X_A_RAM_UNUSED876                   = 876,
MLT18X_A_RAM_UNUSED878                   = 878,
MLT18X_A_RAM_UNUSED885                   = 885,
MLT18X_A_RAM_MADC_VBAT_SCALE             = 886,
MLT18X_A_RAM_UNUSED892                   = 892,
MLT18X_A_RAM_DIAG_GAIN_DC                = 897,
MLT18X_A_RAM_UNUSED911                   = 911,
MLT18X_A_RAM_UNUSED918                   = 918,
MLT18X_A_RAM_VBAT_TRACK_MIN              = 919,
MLT18X_A_RAM_VBAT_TRACK_MIN_RNG          = 920,
MLT18X_A_RAM_UNUSED921                   = 921,
MLT18X_A_RAM_UNUSED922                   = 922,
MLT18X_A_RAM_UNUSED923                   = 923,
MLT18X_A_RAM_UNUSED924                   = 924,
MLT18X_A_RAM_UNUSED925                   = 925,
MLT18X_A_RAM_UNUSED926                   = 926,
MLT18X_A_RAM_UNUSED928                   = 928,
MLT18X_A_RAM_METER_GAIN                  = 961,
MLT18X_A_RAM_METER_GAIN_TEMP             = 968,
MLT18X_A_RAM_METER_RAMP_STEP             = 969,
MLT18X_A_RAM_THERM_DBI                   = 970,
MLT18X_A_RAM_LPR_SCALE                   = 971,
MLT18X_A_RAM_LPR_CM_OS                   = 972,
MLT18X_A_RAM_VOV_DCDC_SLOPE              = 973,
MLT18X_A_RAM_VOV_DCDC_OS                 = 974,
MLT18X_A_RAM_VOV_RING_BAT_MAX            = 975,
MLT18X_A_RAM_SLOPE_VLIM1                 = 976,
MLT18X_A_RAM_SLOPE_RFEED1                = 977,
MLT18X_A_RAM_SLOPE_ILIM1                 = 978,
MLT18X_A_RAM_V_VLIM1                     = 979,
MLT18X_A_RAM_V_RFEED1                    = 980,
MLT18X_A_RAM_V_ILIM1                     = 981,
MLT18X_A_RAM_CONST_RFEED1                = 982,
MLT18X_A_RAM_CONST_ILIM1                 = 983,
MLT18X_A_RAM_I_VLIM1                     = 984,
MLT18X_A_RAM_SLOPE_VLIM2                 = 985,
MLT18X_A_RAM_SLOPE_RFEED2                = 986,
MLT18X_A_RAM_SLOPE_ILIM2                 = 987,
MLT18X_A_RAM_V_VLIM2                     = 988,
MLT18X_A_RAM_V_RFEED2                    = 989,
MLT18X_A_RAM_V_ILIM2                     = 990,
MLT18X_A_RAM_CONST_RFEED2                = 991,
MLT18X_A_RAM_CONST_ILIM2                 = 992,
MLT18X_A_RAM_I_VLIM2                     = 993,
MLT18X_A_RAM_DIAG_V_TAR                  = 994,
MLT18X_A_RAM_DIAG_V_TAR2                 = 995,
MLT18X_A_RAM_STOP_TIMER1_VAL             = 996,
MLT18X_A_RAM_STOP_TIMER2_VAL             = 997,
MLT18X_A_RAM_DIAG_VCM1_TAR               = 998,
MLT18X_A_RAM_DIAG_VCM_STEP               = 999,
MLT18X_A_RAM_LKG_DNT_HIRES               = 1000,
MLT18X_A_RAM_LKG_DNR_HIRES               = 1001,
MLT18X_A_RAM_LINEAR_OS                   = 1002,
MLT18X_A_RAM_CPUMP_DEB                   = 1003,
MLT18X_A_RAM_DCDC_VERR                   = 1004,
MLT18X_A_RAM_DCDC_VERR_HYST              = 1005,
MLT18X_A_RAM_DCDC_OITHRESH_LO            = 1006,
MLT18X_A_RAM_DCDC_OITHRESH_HI            = 1007,
MLT18X_A_RAM_HV_BIAS_ONHK                = 1008,
MLT18X_A_RAM_HV_BIAS_OFFHK               = 1009,
MLT18X_A_RAM_UNUSED1010                  = 1010,
MLT18X_A_RAM_UNUSED1011                  = 1011,
MLT18X_A_RAM_UNUSED1012                  = 1012,
MLT18X_A_RAM_UNUSED1013                  = 1013,
MLT18X_A_RAM_ILONG_RT_THRESH             = 1014,
MLT18X_A_RAM_VOV_RING_BAT_DCDC           = 1015,
MLT18X_A_RAM_UNUSED1016                  = 1016,
MLT18X_A_RAM_LKG_LB_OFFSET               = 1017,
MLT18X_A_RAM_LKG_OFHK_OFFSET             = 1018,
MLT18X_A_RAM_SWEEP_FREQ_TH               = 1019,
MLT18X_A_RAM_AMP_MOD_G                   = 1020,
MLT18X_A_RAM_AMP_MOD_OS                  = 1021,
MLT18X_A_RAM_UNUSED_REG256               = 1280,
MLT18X_A_RAM_DAA_ADC_OUT                 = 1304,
MLT18X_A_RAM_UNUSED_REG282               = 1306,
MLT18X_A_RAM_UNUSED_REG287               = 1311,
MLT18X_A_RAM_UNUSED_REG292               = 1316,
MLT18X_A_RAM_UNUSED_REG293               = 1317,
MLT18X_A_RAM_UNUSED_REG294               = 1318,
MLT18X_A_RAM_UNUSED_REG297               = 1321,
MLT18X_A_RAM_UNUSED_REG329               = 1353,
MLT18X_A_RAM_UNUSED_REG342               = 1366,
MLT18X_A_RAM_UNUSED_REG343               = 1367,
MLT18X_A_RAM_UNUSED_REG344               = 1368,
MLT18X_A_RAM_GPI0                        = 1380,
MLT18X_A_RAM_GPO0                        = 1384,
MLT18X_A_RAM_GPO0_OE                     = 1388,
MLT18X_A_RAM_UNUSED_REG380               = 1404,
MLT18X_A_RAM_UNUSED_REG381               = 1405,
MLT18X_A_RAM_UNUSED_REG384               = 1408,
MLT18X_A_RAM_UNUSED_REG385               = 1409,
MLT18X_A_RAM_UNUSED_REG401               = 1425,
MLT18X_A_RAM_UNUSED_REG402               = 1426,
MLT18X_A_RAM_UNUSED_REG405               = 1429,
MLT18X_A_RAM_UNUSED_REG407               = 1431,
MLT18X_A_RAM_UNUSED_REG411               = 1435,
MLT18X_A_RAM_UNUSED_REG416               = 1440,
MLT18X_A_RAM_UNUSED_REG421               = 1445,
MLT18X_A_RAM_UNUSED_REG427               = 1451,
MLT18X_A_RAM_UNUSED_REG431               = 1455,
MLT18X_A_RAM_LKG_UPT_OHT                 = 1464,
MLT18X_A_RAM_LKG_UPR_OHT                 = 1465,
MLT18X_A_RAM_LKG_DNT_OHT                 = 1466,
MLT18X_A_RAM_LKG_DNR_OHT                 = 1467,
MLT18X_A_RAM_UNUSED_REG446               = 1470,
MLT18X_A_RAM_UNUSED_REG460               = 1484,
MLT18X_A_RAM_UNUSED_REG461               = 1485,
MLT18X_A_RAM_UNUSED_REG470               = 1494,
MLT18X_A_RAM_UNUSED_REG471               = 1495,
MLT18X_A_RAM_UNUSED_REG474               = 1498,
MLT18X_A_RAM_UNUSED_REG476               = 1500,
MLT18X_A_RAM_UNUSED_REG477               = 1501,
MLT18X_A_RAM_UNUSED_REG478               = 1502,
MLT18X_A_RAM_UNUSED_REG479               = 1503,
MLT18X_A_RAM_UNUSED_REG480               = 1504,
MLT18X_A_RAM_UNUSED_REG481               = 1505,
MLT18X_A_RAM_UNUSED_REG488               = 1512,
MLT18X_A_RAM_UNUSED_REG489               = 1513,
MLT18X_A_RAM_UNUSED_REG495               = 1519,
MLT18X_A_RAM_UNUSED_REG496               = 1520,
MLT18X_A_RAM_UNUSED_REG497               = 1521,
MLT18X_A_RAM_UNUSED_REG502               = 1526,
MLT18X_A_RAM_UNUSED_REG505               = 1529,
MLT18X_A_RAM_UNUSED_REG513               = 1537,
MLT18X_A_RAM_UNUSED_REG520               = 1544,
MLT18X_A_RAM_UNUSED_REG525               = 1549,
MLT18X_A_RAM_UNUSED_REG526               = 1550,
MLT18X_A_RAM_UNUSED_REG528               = 1552,
MLT18X_A_RAM_DCDC_CPUMP                  = 1555,
MLT18X_A_RAM_UNUSED_REG532               = 1556,
MLT18X_A_RAM_UNUSED_REG535               = 1559,
MLT18X_A_RAM_UNUSED_REG538               = 1562,
MLT18X_A_RAM_UNUSED_REG542               = 1566,
MLT18X_A_RAM_UNUSED_REG543               = 1567,
MLT18X_A_RAM_UNUSED_REG548               = 1572,
MLT18X_A_RAM_DAA_PROM_MISR               = 1581,
MLT18X_A_RAM_DAA_CROM_MISR               = 1582,
MLT18X_A_RAM_UNUSED_REG569               = 1593,
MLT18X_A_RAM_UNUSED_REG570               = 1594,
MLT18X_A_RAM_JMP8                        = 1597,
MLT18X_A_RAM_JMP9                        = 1598,
MLT18X_A_RAM_JMP10                       = 1599,
MLT18X_A_RAM_JMP11                       = 1600,
MLT18X_A_RAM_JMP12                       = 1601,
MLT18X_A_RAM_JMP13                       = 1602,
MLT18X_A_RAM_JMP14                       = 1603,
MLT18X_A_RAM_JMP15                       = 1604,
MLT18X_A_RAM_METER_TRIG                  = 1605,
MLT18X_A_RAM_PM_ACTIVE                   = 1606,
MLT18X_A_RAM_PM_INACTIVE                 = 1607,
MLT18X_A_RAM_HVIC_VERSION                = 1608,
MLT18X_A_RAM_THERM_OFF                   = 1609,
MLT18X_A_RAM_THERM_HI                    = 1610,
MLT18X_A_RAM_TEST_LOAD                   = 1611,
MLT18X_A_RAM_DC_HOLD_MAN                 = 1612,
MLT18X_A_RAM_DC_HOLD_DAC_MAN             = 1613,
MLT18X_A_RAM_UNUSED_REG590               = 1614,
MLT18X_A_RAM_DCDC_CPUMP_LP               = 1615,
MLT18X_A_RAM_DCDC_CPUMP_LP_MASK          = 1616,
MLT18X_A_RAM_DCDC_CPUMP_PULLDOWN         = 1617,
MLT18X_A_RAM_BOND_STATUS                 = 1618,
MLT18X_A_RAM_BOND_MAN                    = 1619,
MLT18X_A_RAM_BOND_VAL                    = 1620,
MLT18X_A_RAM_REF_DEBOUNCE_PCLK           = 1633,
MLT18X_A_RAM_REF_DEBOUNCE_FSYNC          = 1634,
MLT18X_A_RAM_DCDC_LIFT_EN                = 1635,
MLT18X_A_RAM_DCDC_CPUMP_PGOOD            = 1636,
MLT18X_A_RAM_DCDC_CPUMP_PGOOD_WKEN       = 1637,
MLT18X_A_RAM_DCDC_CPUMP_PGOOD_FRC        = 1638,
MLT18X_A_RAM_DCDC_CPUMP_LP_MASK_SH       = 1639,
MLT18X_A_RAM_DCDC_UV_MAN                 = 1640,
MLT18X_A_RAM_DCDC_UV_DEBOUNCE            = 1641,
MLT18X_A_RAM_DCDC_OV_MAN                 = 1642,
MLT18X_A_RAM_DCDC_OV_DEBOUNCE            = 1643,
MLT18X_A_RAM_ANALOG3_TEST_MUX            = 1644,
};

#endif
