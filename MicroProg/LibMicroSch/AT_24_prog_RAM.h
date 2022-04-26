#ifndef AT_24_prog_RAM_h
#define AT_24_prog_RAM_h
 
typedef enum{ 
  DD38           =  0    ,     //BASE_CALC
  DA24           =  1    ,     //BASE16
  DD109          =  2    ,     //PAROLES
  DA3            =  3    ,     //DEBUG_SYM
  DA8            =  4    ,     //NAMEFUNC16
  DD9            =  5    ,     //PORT_UART_3
  DD60           =  6    ,     //PORT_UART_3
  DD61           =  7    ,     //PORT_UART_3
  DD3            =  8    ,     //PORT_UART_3
  DD5            =  9    ,     //PORT_UART_3
  DD62           =  10   ,     //OR-2
  DD8            =  11   ,     //OR-3
  MB1            =  12   ,     //MB_0X05
  MB2            =  13   ,     //MB_0X05_VAL
  DD2            =  14   ,     //TIME_CONTROL
  U2             =  15   ,     //PERIF_V2
  U15            =  16   ,     //DIN_2U64
  DA5            =  17   ,     //AIN_SCALE_V3
  DA13           =  18   ,     //AIN_SCALE_V3
  DD53           =  19   ,     //DISKR_FUNC_V4
  DD48           =  20   ,     //CONTROL_V2
  DD27           =  21   ,     //SEL_REF_V2
  DD20           =  22   ,     //REF_CONTROL
  DA9            =  23   ,     //FREQ_FORBIDDEN_V2
  DA22           =  24   ,     //CURRENT_CONTROL
  DA11           =  25   ,     //PICKUP_V2
  DA10           =  26   ,     //MUX2
  DD59           =  27   ,     //TEMP_LSU_2
  U7             =  28   ,     //UF5_F2S
  DD4            =  29   ,     //POWER_ON_PUSK
  DD65           =  30   ,     //PUSK_READY_2
  DD12           =  31   ,     //STOP_2
  DRV_V5         =  32   ,     //DRV_V5
  DA32           =  33   ,     //MIN_MAX_LIMITER
  DA30           =  34   ,     //MIN_MAX_LIMITER
  DD1            =  35   ,     //RTC
  DD30           =  36   ,     //BIT16_TO_U16
  DD33           =  37   ,     //BIT16_TO_U16
  DD35           =  38   ,     //BIT16_TO_U16
  DD66           =  39   ,     //BIT16_TO_U16
  DD34           =  40   ,     //OR-2
  DD26           =  41   ,     //FACTORY_SET
  DA12           =  42   ,     //DRV_UD
  DD214          =  43   ,     //OR-2
  DD22           =  44   ,     //READY_STATUS
  DD29           =  45   ,     //OR-2
  DD89           =  46   ,     //BIT8_TO_U16
  DD90           =  47   ,     //S16_S16_TO_S16
  DD7            =  48   ,     //MUX24
  DA89           =  49   ,     //KX+B
  DA15           =  50   ,     //KX+B_32_2
  DA54           =  51   ,     //FREQ_SCALE
  DA55           =  52   ,     //FREQ_SCALE
  DA4            =  53   ,     //MUX8_2
  DA2            =  54   ,     //FB_COMBO
  DA6            =  55   ,     //MUX8_2
  ADD3           =  56   ,     //ADD
  DA173          =  57   ,     //MIN_MAX_2
  DD135          =  58   ,     //OR-4
  DA119          =  59   ,     //PID_IQ
  ADD5           =  60   ,     //ADD
  DA188          =  61   ,     //MIN_MAX_2
  DD179          =  62   ,     //MUX4
  DA191          =  63   ,     //PID_IQ
  DA7            =  64   ,     //PID_REF_CONVERT
  DD45           =  65   ,     //UNDERLOAD
  DA21           =  66   ,     //CURRENT_IMBALANCE
  DA16           =  67   ,     //LOAD_LOSS
  DD169          =  68   ,     //BIT16_TO_U16
  DD168          =  69   ,     //BIT16_TO_U16
  DD170          =  70   ,     //BIT16_TO_U16
  DD56           =  71   ,     //WARNINGS
  DD82           =  72   ,     //PULSE_RISE_PRM
  DD47           =  73   ,     //OR-2
  DD11           =  74   ,     //OR-2
  DD10           =  75   ,     //OR-4
  DA23           =  76   ,     //FREQLIMITCHECK
  DA25           =  77   ,     //FREQLIMITCHECK
  DD24           =  78   ,     //FAULT_WARNING
  DD25           =  79   ,     //JRN_V2
  DD28           =  80   ,     //LED_RUN
  DD49           =  81   ,     //GET_CRC_PO
  DA66           =  82   ,     //POWER_LOSS_APV
  DD6            =  83   ,     //APV1
  DD15           =  84   ,     //LOCAL_APV_V3
  DD37           =  85   ,     //LED_WIFI
  DD16           =  86   ,     //LOCAL_APV_V3
  DD13           =  87   ,     //LOCAL_APV_V3
  DD18           =  88   ,     //LOCAL_APV_V3
  DD36           =  89   ,     //LOCAL_APV_V3
  DD21           =  90   ,     //LOCAL_APV_V3
  DD17           =  91   ,     //LOCAL_APV_V3
  DD14           =  92   ,     //LOCAL_APV_V3
  DD151          =  93   ,     //BOOTLOADER
  DD39           =  94   ,     //SVP_UNIT
  U16            =  95   ,     //KOEFF_RS1
  DD40           =  96   ,     //DIN_UP_DOWN
  DA20           =  97   ,     //TPM
  DD31           =  98   ,     //LED_CONTROL
  U1             =  99   ,     //TORQUE_MAX
  DA1            =  100  ,     //MULL
  DA26           =  101  ,     //TORMOZ_V2
  DD42           =  102  ,     //LOGER
  DD43           =  103  ,     //RW_GROUP_PRM
  DD44           =  104  ,     //RW_GROUP_PRM
  DD32           =  105  ,     //BRAKE_CNTRL
  DD46           =  106  ,     //MUX_4X4_4
  DA17           =  107  ,     //MULL
  DA18           =  108  ,     //TEMP_CONTROL
  DD50           =  109  ,     //EXT_BLOCKS_PARAMS
  DD64           =  110  ,     //LIFT_CMD_V3
  DD63           =  111  ,     //ENCO_PHASING_BLOCK
  DD55           =  112  ,     //CONT_FAIL
  DD51           =  113  ,     //START_STOP_MUX
  DD52           =  114  ,     //OR-2
  DD54           =  115  ,     //EVAK_V3
  U3             =  116  ,     //RAMP_CTRL_MOD2
  DD41           =  117  ,     //FUNCTIONAL_BLOCK
  DD23           =  118  ,     //FAIL_COMMON_V2
  DA27           =  119  ,     //AIN_SCALE_V3
  DA28           =  120  ,     //AIN_SCALE_V3
  DA57           =  121  ,     //FREQ_SCALE
  DA56           =  122  ,     //FREQ_SCALE
  DD57           =  123  ,     //LOCAL_APV_V3
  DD58           =  124  ,     //LOCAL_APV_V3
  DA33           =  125  ,     //KX+B_32_2
  DA14           =  126  ,     //DERATING_CONTROL
  DA19           =  127  ,     //NO_FLOW_DR_RUN2
  DD19           =  128  ,     //END_OF_CURVE
  DA34           =  129  ,     //MIN_MAX_LIMITER
  DD67           =  130        //PUMP_DERAG
}nameFuncExe_type; 
 
  #define defB_0_BIT                         16
  #define defB_1_BIT                         17
  #define defB_ACKFAIL                       18
  #define defB_AIN_1_ALARM                   19
  #define defB_AIN_1_ALARM_FIXSPD            20
  #define defB_AIN_1_ERR                     21
  #define defB_AIN_1_FAIL                    22
  #define defB_AIN_2_ALARM                   23
  #define defB_AIN_2_ALARM_FIXSPD            24
  #define defB_AIN_2_ERR                     25
  #define defB_AIN_2_FAIL                    26
  #define defB_AIN_3_ALARM                   27
  #define defB_AIN_3_ALARM_FIXSPD            28
  #define defB_AIN_3_ERR                     29
  #define defB_AIN_3_FAIL                    30
  #define defB_AIN_4_ALARM                   31
  #define defB_AIN_4_ALARM_FIXSPD            32
  #define defB_AIN_4_ERR                     33
  #define defB_AIN_4_FAIL                    34
  #define defB_AIN_5_ERR                     35
  #define defB_ALARM_OVERLOAD                36
  #define defB_ALARM_UNDERLOAD               37
  #define defB_AUTOVENTIL_FAIL               38
  #define defB_AUTO_MODE                     39
  #define defB_AVAR_TPBR                     40
  #define defB_AV_FAIL_NO_MOTOR              41
  #define defB_AV_REVERSE_ERR                42
  #define defB_BACKUP_POWER_STATUS           43
  #define defB_BLINK_APV                     44
  #define defB_BRAKE_GENERAL                 45
  #define defB_CHANGE_MST_SLV                46
  #define defB_CHARGE_OK                     47
  #define defB_CH_PID                        48
  #define defB_CH_REF_DIN                    49
  #define defB_CH_REF_LP                     50
  #define defB_CH_REF_RP                     51
  #define defB_CMD_REVERS                    52
  #define defB_CONTACTOR_ON                  53
  #define defB_CONT_BRK_FB_ERR               54
  #define defB_CONT_NO_FB_ERR                55
  #define defB_CURR_IMBALANCE_ALARM          56
  #define defB_CURR_IMBALANCE_FAILURE        57
  #define defB_CURR_IMBALANCE_WARNING        58
  #define defB_DF_ENABLE_PUSK                59
  #define defB_DF_EXT_STOP                   60
  #define defB_DF_F_DOWN                     61
  #define defB_DF_F_UP                       62
  #define defB_DF_NO_SPEED_FAULT             63
  #define defB_DF_NO_SPEED_WARN              64
  #define defB_DF_PUSK                       65
  #define defB_DF_SEL_TIME                   66
  #define defB_DF_STOP                       67
  #define defB_DF_STOP_VIBEG                 68
  #define defB_DIN_BRAKE                     69
  #define defB_DIN_CONTACTOR                 70
  #define defB_DIN_EVAC_START                71
  #define defB_DIN_FAIL_RESET                72
  #define defB_DIN_ME_F                      73
  #define defB_DIN_SEL_CH_CONTROL            74
  #define defB_DIN_SEL_CH_REF                75
  #define defB_DIN_SEL_N_UF                  76
  #define defB_DIN_USER_FAULT_1              77
  #define defB_DIN_USER_FAULT_2              78
  #define defB_DOUBLE_DIR_ERROR              79
  #define defB_DRV_UD_START                  80
  #define defB_DRV_UD_ZAR                    81
  #define defB_DRY_ON                        82
  #define defB_DR_FAILURE                    83
  #define defB_DR_WARNING                    84
  #define defB_ENABLE_CONTR_DIN              85
  #define defB_ENCODER_PHASING_DONE          86
  #define defB_ENCODER_PHASING_WORK          87
  #define defB_ENCO_PHASING_ABORT_FAULT      88
  #define defB_ENCO_PHASING_LIFTDROP_FAULT   89
  #define defB_ENCO_PHASING_RTIMEOUT_FAULT   90
  #define defB_ENCO_PHASING_SPEED_FAULT      91
  #define defB_END_OF_CURVE_ERR              92
  #define defB_END_OF_CURVE_WARN             93
  #define defB_EN_PUSK_LC                    94
  #define defB_EN_PUSK_RC                    95
  #define defB_EN_PUSK_WIFI                  96
  #define defB_ERROR_ENCODER                 97
  #define defB_ERR_DELTA_MST_SLV             98
  #define defB_ERR_MB_CONNECT                99
  #define defB_ERR_MB_CONNECT_3              100
  #define defB_ERR_MB_CONNECT_4              101
  #define defB_ERR_TIME_APV                  102
  #define defB_ERR_WIFI_CONNECT              103
  #define defB_ERR_ZAR_TIME                  104
  #define defB_EVAC_ON                       105
  #define defB_EXT24_RS2_DUPLICATE_NETADR    106
  #define defB_EXTERNAL_BLOCK_FAIL           107
  #define defB_FAIL_CHECK_TORM_RES           108
  #define defB_FAIL_MOTOR_OVERHEATING        109
  #define defB_FAIL_STOP_NETWORK             110
  #define defB_FAIL_TEK_DRV_BIT              111
  #define defB_FAIL_TEK_DRV_BIT_NOT          112
  #define defB_FAST_STOP                     113
  #define defB_FAZIROVKA_ERR                 114
  #define defB_FORBID_FREQ_NOW               115
  #define defB_F_MAX_ALARM                   116
  #define defB_F_MAX_ER                      117
  #define defB_F_MAX_ERR                     118
  #define defB_F_MIN_ALARM                   119
  #define defB_F_MIN_ER                      120
  #define defB_F_MIN_ERR                     121
  #define defB_F_OUT_SIGN                    122
  #define defB_F_PUSK_FORB                   123
  #define defB_F_REF_ALARM                   124
  #define defB_F_VPRUP                       125
  #define defB_GT_DWN                        126
  #define defB_GT_EQ_RES_VAL                 127
  #define defB_GT_UP                         128
  #define defB_GT_ZERO                       129
  #define defB_HOLD_ZERO                     130
  #define defB_IMAX_ER                       131
  #define defB_IMAX_ERR                      132
  #define defB_LED_FAIL                      133
  #define defB_LED_READY                     134
  #define defB_LED_RUN                       135
  #define defB_LED_WIFI                      136
  #define defB_LED_WIFI_STAT                 137
  #define defB_LIFT_BRAKE_OFF                138
  #define defB_LIFT_BRK_FAIL                 139
  #define defB_LIFT_BRK_WO_PWM               140
  #define defB_LOAD_LOSS_FAIL                141
  #define defB_LOCK_TEMP_BRAKE               142
  #define defB_MAX_INPUT_FAIL                143
  #define defB_MB_COIL_10                    144
  #define defB_MB_COIL_11                    145
  #define defB_MB_COIL_12                    146
  #define defB_MB_COIL_13                    147
  #define defB_MB_COIL_14                    148
  #define defB_MB_COIL_15                    149
  #define defB_MB_COIL_8                     150
  #define defB_MB_COIL_9                     151
  #define defB_MEAS_PARAM                    152
  #define defB_MEAS_PARAM_INTERRUPT          153
  #define defB_MINUTE_IMPULS                 154
  #define defB_MKARFAIL                      155
  #define defB_MODBUS_SLAVE_ERR              156
  #define defB_MTZ_COMP                      157
  #define defB_MTZ_LEAKAGE                   158
  #define defB_NET00015                      159
  #define defB_NET00023                      160
  #define defB_NET00038                      161
  #define defB_NET00039                      162
  #define defB_NET00066                      163
  #define defB_NET00196                      164
  #define defB_NET00353                      165
  #define defB_NET00383                      166
  #define defB_NET00420                      167
  #define defB_NET00467                      168
  #define defB_NET00470                      169
  #define defB_NET00753                      170
  #define defB_NET01009                      171
  #define defB_NEXT_WARN_ASU                 172
  #define defB_NF_FAILURE                    173
  #define defB_NF_SLEEP_BLINK                174
  #define defB_NF_SLEEP_WARNING              175
  #define defB_NF_WARNING                    176
  #define defB_NO_WATER                      177
  #define defB_NO_WATER_A                    178
  #define defB_OFF_ZAR                       179
  #define defB_PHASE_12ERR                   180
  #define defB_PHASE_3ERR                    181
  #define defB_PICK_UP_ON                    182
  #define defB_PID_GT_DOWN                   183
  #define defB_PID_GT_UP                     184
  #define defB_POWER_ER                      185
  #define defB_POWER_ON_PUSK                 186
  #define defB_PROZV_ON                      187
  #define defB_PR_OUT_SIG                    188
  #define defB_PTC_EN                        189
  #define defB_PUMP_CLEAN_FLG                190
  #define defB_PUMP_CLEAN_ON_CMD             191
  #define defB_PUMP_CLEAN_POWER_ERR          192
  #define defB_PUMP_CLEAN_STOP_CMD           193
  #define defB_PUSK_ALL_IMP                  194
  #define defB_PUSK_APV                      195
  #define defB_PUSK_CMM                      196
  #define defB_PUSK_FORBIDDEN_WARN           197
  #define defB_PUSK_NETWORK                  198
  #define defB_PUSK_NETWORK_PB               199
  #define defB_PUSK_OUT_LIFT                 200
  #define defB_PUSK_PROFINET                 201
  #define defB_PUSK_SLEEP_NO_FLOW            202
  #define defB_PWM_OFF_DRV                   203
  #define defB_PWM_ON_DRV                    204
  #define defB_P_FAIL_RESET                  205
  #define defB_READY_PUSK_ALL                206
  #define defB_READY_PUSK_STATUS             207
  #define defB_RELE_COOLER                   208
  #define defB_RELE_MASTER_SLAVE             209
  #define defB_RESET_APV                     210
  #define defB_RESET_APV_NET                 211
  #define defB_RESET_CNT_JOB_NET             212
  #define defB_RESTT_CNT_JOB_DIN             213
  #define defB_RUN_DRV                       214
  #define defB_RUN_STATUS                    215
  #define defB_SIMULATION_MODE_IN_ACTION     216
  #define defB_SLAVE_BAD_PACKET              217
  #define defB_SLAVE_NOT_PRESENT             218
  #define defB_SLAVE_NOT_START               219
  #define defB_SLAVE_NO_ASWER_FAIL           220
  #define defB_START_CLEAN                   221
  #define defB_START_ON_POWER_WRN            222
  #define defB_START_OUT                     223
  #define defB_STOP_APV                      224
  #define defB_STOP_BY_DC                    225
  #define defB_STOP_CMM                      226
  #define defB_STOP_FREQ                     227
  #define defB_STOP_KEY                      228
  #define defB_STOP_MODBUS_SLAVE             229
  #define defB_STOP_NETWORK                  230
  #define defB_STOP_NETWORK_PB               231
  #define defB_STOP_OUT                      232
  #define defB_STOP_OUT_LIFT                 233
  #define defB_STOP_PROFINET                 234
  #define defB_STOP_SLEEP_NO_FLOW            235
  #define defB_T_DISKR                       236
  #define defB_UD_KZ_FAIL                    237
  #define defB_UNDRLOAD_ER                   238
  #define defB_UNDRLOAD_ERR                  239
  #define defB_VIBEG_STAT                    240
  #define defB_VPHIRUP                       241
  #define defB_WAIT_UF                       242
  #define defB_WARNING_BIT                   243
  #define defB_WARNING_CHARGE                244
  #define defB_WARNING_DERATING_ACTION       245
  #define defB_WARNING_FMAX                  246
  #define defB_WARNING_FMIN                  247
  #define defB_WARNING_NEDOGR                248
  #define defB_WARNING_NO_WATER              249
  #define defB_WARNING_OS_PID                250
  #define defB_WARNING_PEREGR                251
  #define defB_WARNING_STOP_AIN1             252
  #define defB_WARNING_STOP_AIN2             253
  #define defB_WARNING_STOP_AIN3             254
  #define defB_WARNING_STOP_AIN4             255
  #define defB_WARN_DISKR_FUNC_EXIST         256
  #define defB_WARN_DISKR_INPUT_BUSY         257
  #define defB_WIFI_NET_ERR                  258
  #define defB_WORK_FLAG_LIFT                259
  #define defB_WORK_MODE                     260
  #define defNet_Fict_bit                    261
 
 typedef struct {
               u16  WARN_DISKR_FUNC_EXIST         : 1  ;   //256
               u16  WARN_DISKR_INPUT_BUSY         : 1  ;   //257
               u16  WIFI_NET_ERR                  : 1  ;   //258
               u16  WORK_FLAG_LIFT                : 1  ;   //259
               u16  WORK_MODE                     : 1  ;   //260
               u16  Net_Fict_bit                  : 11  ;   //261
               u16  VIBEG_STAT                    : 1  ;   //240
               u16  VPHIRUP                       : 1  ;   //241
               u16  WAIT_UF                       : 1  ;   //242
               u16  WARNING_BIT                   : 1  ;   //243
               u16  WARNING_CHARGE                : 1  ;   //244
               u16  WARNING_DERATING_ACTION       : 1  ;   //245
               u16  WARNING_FMAX                  : 1  ;   //246
               u16  WARNING_FMIN                  : 1  ;   //247
               u16  WARNING_NEDOGR                : 1  ;   //248
               u16  WARNING_NO_WATER              : 1  ;   //249
               u16  WARNING_OS_PID                : 1  ;   //250
               u16  WARNING_PEREGR                : 1  ;   //251
               u16  WARNING_STOP_AIN1             : 1  ;   //252
               u16  WARNING_STOP_AIN2             : 1  ;   //253
               u16  WARNING_STOP_AIN3             : 1  ;   //254
               u16  WARNING_STOP_AIN4             : 1  ;   //255
               u16  STOP_APV                      : 1  ;   //224
               u16  STOP_BY_DC                    : 1  ;   //225
               u16  STOP_CMM                      : 1  ;   //226
               u16  STOP_FREQ                     : 1  ;   //227
               u16  STOP_KEY                      : 1  ;   //228
               u16  STOP_MODBUS_SLAVE             : 1  ;   //229
               u16  STOP_NETWORK                  : 1  ;   //230
               u16  STOP_NETWORK_PB               : 1  ;   //231
               u16  STOP_OUT                      : 1  ;   //232
               u16  STOP_OUT_LIFT                 : 1  ;   //233
               u16  STOP_PROFINET                 : 1  ;   //234
               u16  STOP_SLEEP_NO_FLOW            : 1  ;   //235
               u16  T_DISKR                       : 1  ;   //236
               u16  UD_KZ_FAIL                    : 1  ;   //237
               u16  UNDRLOAD_ER                   : 1  ;   //238
               u16  UNDRLOAD_ERR                  : 1  ;   //239
               u16  RELE_COOLER                   : 1  ;   //208
               u16  RELE_MASTER_SLAVE             : 1  ;   //209
               u16  RESET_APV                     : 1  ;   //210
               u16  RESET_APV_NET                 : 1  ;   //211
               u16  RESET_CNT_JOB_NET             : 1  ;   //212
               u16  RESTT_CNT_JOB_DIN             : 1  ;   //213
               u16  RUN_DRV                       : 1  ;   //214
               u16  RUN_STATUS                    : 1  ;   //215
               u16  SIMULATION_MODE_IN_ACTION     : 1  ;   //216
               u16  SLAVE_BAD_PACKET              : 1  ;   //217
               u16  SLAVE_NOT_PRESENT             : 1  ;   //218
               u16  SLAVE_NOT_START               : 1  ;   //219
               u16  SLAVE_NO_ASWER_FAIL           : 1  ;   //220
               u16  START_CLEAN                   : 1  ;   //221
               u16  START_ON_POWER_WRN            : 1  ;   //222
               u16  START_OUT                     : 1  ;   //223
               u16  PUMP_CLEAN_POWER_ERR          : 1  ;   //192
               u16  PUMP_CLEAN_STOP_CMD           : 1  ;   //193
               u16  PUSK_ALL_IMP                  : 1  ;   //194
               u16  PUSK_APV                      : 1  ;   //195
               u16  PUSK_CMM                      : 1  ;   //196
               u16  PUSK_FORBIDDEN_WARN           : 1  ;   //197
               u16  PUSK_NETWORK                  : 1  ;   //198
               u16  PUSK_NETWORK_PB               : 1  ;   //199
               u16  PUSK_OUT_LIFT                 : 1  ;   //200
               u16  PUSK_PROFINET                 : 1  ;   //201
               u16  PUSK_SLEEP_NO_FLOW            : 1  ;   //202
               u16  PWM_OFF_DRV                   : 1  ;   //203
               u16  PWM_ON_DRV                    : 1  ;   //204
               u16  P_FAIL_RESET                  : 1  ;   //205
               u16  READY_PUSK_ALL                : 1  ;   //206
               u16  READY_PUSK_STATUS             : 1  ;   //207
               u16  NF_WARNING                    : 1  ;   //176
               u16  NO_WATER                      : 1  ;   //177
               u16  NO_WATER_A                    : 1  ;   //178
               u16  OFF_ZAR                       : 1  ;   //179
               u16  PHASE_12ERR                   : 1  ;   //180
               u16  PHASE_3ERR                    : 1  ;   //181
               u16  PICK_UP_ON                    : 1  ;   //182
               u16  PID_GT_DOWN                   : 1  ;   //183
               u16  PID_GT_UP                     : 1  ;   //184
               u16  POWER_ER                      : 1  ;   //185
               u16  POWER_ON_PUSK                 : 1  ;   //186
               u16  PROZV_ON                      : 1  ;   //187
               u16  PR_OUT_SIG                    : 1  ;   //188
               u16  PTC_EN                        : 1  ;   //189
               u16  PUMP_CLEAN_FLG                : 1  ;   //190
               u16  PUMP_CLEAN_ON_CMD             : 1  ;   //191
               u16  NET00023                      : 1  ;   //160
               u16  NET00038                      : 1  ;   //161
               u16  NET00039                      : 1  ;   //162
               u16  NET00066                      : 1  ;   //163
               u16  NET00196                      : 1  ;   //164
               u16  NET00353                      : 1  ;   //165
               u16  NET00383                      : 1  ;   //166
               u16  NET00420                      : 1  ;   //167
               u16  NET00467                      : 1  ;   //168
               u16  NET00470                      : 1  ;   //169
               u16  NET00753                      : 1  ;   //170
               u16  NET01009                      : 1  ;   //171
               u16  NEXT_WARN_ASU                 : 1  ;   //172
               u16  NF_FAILURE                    : 1  ;   //173
               u16  NF_SLEEP_BLINK                : 1  ;   //174
               u16  NF_SLEEP_WARNING              : 1  ;   //175
               u16  MB_COIL_10                    : 1  ;   //144
               u16  MB_COIL_11                    : 1  ;   //145
               u16  MB_COIL_12                    : 1  ;   //146
               u16  MB_COIL_13                    : 1  ;   //147
               u16  MB_COIL_14                    : 1  ;   //148
               u16  MB_COIL_15                    : 1  ;   //149
               u16  MB_COIL_8                     : 1  ;   //150
               u16  MB_COIL_9                     : 1  ;   //151
               u16  MEAS_PARAM                    : 1  ;   //152
               u16  MEAS_PARAM_INTERRUPT          : 1  ;   //153
               u16  MINUTE_IMPULS                 : 1  ;   //154
               u16  MKARFAIL                      : 1  ;   //155
               u16  MODBUS_SLAVE_ERR              : 1  ;   //156
               u16  MTZ_COMP                      : 1  ;   //157
               u16  MTZ_LEAKAGE                   : 1  ;   //158
               u16  NET00015                      : 1  ;   //159
               u16  GT_UP                         : 1  ;   //128
               u16  GT_ZERO                       : 1  ;   //129
               u16  HOLD_ZERO                     : 1  ;   //130
               u16  IMAX_ER                       : 1  ;   //131
               u16  IMAX_ERR                      : 1  ;   //132
               u16  LED_FAIL                      : 1  ;   //133
               u16  LED_READY                     : 1  ;   //134
               u16  LED_RUN                       : 1  ;   //135
               u16  LED_WIFI                      : 1  ;   //136
               u16  LED_WIFI_STAT                 : 1  ;   //137
               u16  LIFT_BRAKE_OFF                : 1  ;   //138
               u16  LIFT_BRK_FAIL                 : 1  ;   //139
               u16  LIFT_BRK_WO_PWM               : 1  ;   //140
               u16  LOAD_LOSS_FAIL                : 1  ;   //141
               u16  LOCK_TEMP_BRAKE               : 1  ;   //142
               u16  MAX_INPUT_FAIL                : 1  ;   //143
               u16  FAIL_TEK_DRV_BIT_NOT          : 1  ;   //112
               u16  FAST_STOP                     : 1  ;   //113
               u16  FAZIROVKA_ERR                 : 1  ;   //114
               u16  FORBID_FREQ_NOW               : 1  ;   //115
               u16  F_MAX_ALARM                   : 1  ;   //116
               u16  F_MAX_ER                      : 1  ;   //117
               u16  F_MAX_ERR                     : 1  ;   //118
               u16  F_MIN_ALARM                   : 1  ;   //119
               u16  F_MIN_ER                      : 1  ;   //120
               u16  F_MIN_ERR                     : 1  ;   //121
               u16  F_OUT_SIGN                    : 1  ;   //122
               u16  F_PUSK_FORB                   : 1  ;   //123
               u16  F_REF_ALARM                   : 1  ;   //124
               u16  F_VPRUP                       : 1  ;   //125
               u16  GT_DWN                        : 1  ;   //126
               u16  GT_EQ_RES_VAL                 : 1  ;   //127
               u16  EN_PUSK_WIFI                  : 1  ;   //96
               u16  ERROR_ENCODER                 : 1  ;   //97
               u16  ERR_DELTA_MST_SLV             : 1  ;   //98
               u16  ERR_MB_CONNECT                : 1  ;   //99
               u16  ERR_MB_CONNECT_3              : 1  ;   //100
               u16  ERR_MB_CONNECT_4              : 1  ;   //101
               u16  ERR_TIME_APV                  : 1  ;   //102
               u16  ERR_WIFI_CONNECT              : 1  ;   //103
               u16  ERR_ZAR_TIME                  : 1  ;   //104
               u16  EVAC_ON                       : 1  ;   //105
               u16  EXT24_RS2_DUPLICATE_NETADR    : 1  ;   //106
               u16  EXTERNAL_BLOCK_FAIL           : 1  ;   //107
               u16  FAIL_CHECK_TORM_RES           : 1  ;   //108
               u16  FAIL_MOTOR_OVERHEATING        : 1  ;   //109
               u16  FAIL_STOP_NETWORK             : 1  ;   //110
               u16  FAIL_TEK_DRV_BIT              : 1  ;   //111
               u16  DRV_UD_START                  : 1  ;   //80
               u16  DRV_UD_ZAR                    : 1  ;   //81
               u16  DRY_ON                        : 1  ;   //82
               u16  DR_FAILURE                    : 1  ;   //83
               u16  DR_WARNING                    : 1  ;   //84
               u16  ENABLE_CONTR_DIN              : 1  ;   //85
               u16  ENCODER_PHASING_DONE          : 1  ;   //86
               u16  ENCODER_PHASING_WORK          : 1  ;   //87
               u16  ENCO_PHASING_ABORT_FAULT      : 1  ;   //88
               u16  ENCO_PHASING_LIFTDROP_FAULT   : 1  ;   //89
               u16  ENCO_PHASING_RTIMEOUT_FAULT   : 1  ;   //90
               u16  ENCO_PHASING_SPEED_FAULT      : 1  ;   //91
               u16  END_OF_CURVE_ERR              : 1  ;   //92
               u16  END_OF_CURVE_WARN             : 1  ;   //93
               u16  EN_PUSK_LC                    : 1  ;   //94
               u16  EN_PUSK_RC                    : 1  ;   //95
               u16  DF_NO_SPEED_WARN              : 1  ;   //64
               u16  DF_PUSK                       : 1  ;   //65
               u16  DF_SEL_TIME                   : 1  ;   //66
               u16  DF_STOP                       : 1  ;   //67
               u16  DF_STOP_VIBEG                 : 1  ;   //68
               u16  DIN_BRAKE                     : 1  ;   //69
               u16  DIN_CONTACTOR                 : 1  ;   //70
               u16  DIN_EVAC_START                : 1  ;   //71
               u16  DIN_FAIL_RESET                : 1  ;   //72
               u16  DIN_ME_F                      : 1  ;   //73
               u16  DIN_SEL_CH_CONTROL            : 1  ;   //74
               u16  DIN_SEL_CH_REF                : 1  ;   //75
               u16  DIN_SEL_N_UF                  : 1  ;   //76
               u16  DIN_USER_FAULT_1              : 1  ;   //77
               u16  DIN_USER_FAULT_2              : 1  ;   //78
               u16  DOUBLE_DIR_ERROR              : 1  ;   //79
               u16  CH_PID                        : 1  ;   //48
               u16  CH_REF_DIN                    : 1  ;   //49
               u16  CH_REF_LP                     : 1  ;   //50
               u16  CH_REF_RP                     : 1  ;   //51
               u16  CMD_REVERS                    : 1  ;   //52
               u16  CONTACTOR_ON                  : 1  ;   //53
               u16  CONT_BRK_FB_ERR               : 1  ;   //54
               u16  CONT_NO_FB_ERR                : 1  ;   //55
               u16  CURR_IMBALANCE_ALARM          : 1  ;   //56
               u16  CURR_IMBALANCE_FAILURE        : 1  ;   //57
               u16  CURR_IMBALANCE_WARNING        : 1  ;   //58
               u16  DF_ENABLE_PUSK                : 1  ;   //59
               u16  DF_EXT_STOP                   : 1  ;   //60
               u16  DF_F_DOWN                     : 1  ;   //61
               u16  DF_F_UP                       : 1  ;   //62
               u16  DF_NO_SPEED_FAULT             : 1  ;   //63
               u16  AIN_4_ALARM_FIXSPD            : 1  ;   //32
               u16  AIN_4_ERR                     : 1  ;   //33
               u16  AIN_4_FAIL                    : 1  ;   //34
               u16  AIN_5_ERR                     : 1  ;   //35
               u16  ALARM_OVERLOAD                : 1  ;   //36
               u16  ALARM_UNDERLOAD               : 1  ;   //37
               u16  AUTOVENTIL_FAIL               : 1  ;   //38
               u16  AUTO_MODE                     : 1  ;   //39
               u16  AVAR_TPBR                     : 1  ;   //40
               u16  AV_FAIL_NO_MOTOR              : 1  ;   //41
               u16  AV_REVERSE_ERR                : 1  ;   //42
               u16  BACKUP_POWER_STATUS           : 1  ;   //43
               u16  BLINK_APV                     : 1  ;   //44
               u16  BRAKE_GENERAL                 : 1  ;   //45
               u16  CHANGE_MST_SLV                : 1  ;   //46
               u16  CHARGE_OK                     : 1  ;   //47
               u16  B_0_BIT                       : 1  ;   //16
               u16  B_1_BIT                       : 1  ;   //17
               u16  ACKFAIL                       : 1  ;   //18
               u16  AIN_1_ALARM                   : 1  ;   //19
               u16  AIN_1_ALARM_FIXSPD            : 1  ;   //20
               u16  AIN_1_ERR                     : 1  ;   //21
               u16  AIN_1_FAIL                    : 1  ;   //22
               u16  AIN_2_ALARM                   : 1  ;   //23
               u16  AIN_2_ALARM_FIXSPD            : 1  ;   //24
               u16  AIN_2_ERR                     : 1  ;   //25
               u16  AIN_2_FAIL                    : 1  ;   //26
               u16  AIN_3_ALARM                   : 1  ;   //27
               u16  AIN_3_ALARM_FIXSPD            : 1  ;   //28
               u16  AIN_3_ERR                     : 1  ;   //29
               u16  AIN_3_FAIL                    : 1  ;   //30
               u16  AIN_4_ALARM                   : 1  ;   //31
/* 0x0020*/    float              AOUT_1;
/* 0x0024*/    float              AOUT_2;
/* 0x0028*/    float              AOUT_3;
/* 0x002C*/    float              AOUT_4;
/* 0x0030*/    float              NET00139;
/* 0x0034*/    float              NET00144;
/* 0x0038*/    float              NET00146;
/* 0x003C*/    float              NET00190;
/* 0x0040*/    float              NET00273;
/* 0x0044*/    float              Fict_Float;
/* 0x0048*/    s32                F_0;
/* 0x004C*/    s32                F_0_1_HZ;
/* 0x0050*/    s32                F_0_5_HZ;
/* 0x0054*/    s32                F_0_01_HZ;
/* 0x0058*/    s32                F_1_0_HZ;
/* 0x005C*/    s32                F_5_0_HZ;
/* 0x0060*/    s32                F_10_HZ;
/* 0x0064*/    s32                F_0_25;
/* 0x0068*/    s32                UD_LOW_LIM;
/* 0x006C*/    s32                F_0_5;
/* 0x0070*/    s32                F_80_HZ;
/* 0x0074*/    s32                TORMSW_LOW_LIM;
/* 0x0078*/    s32                F_2_0;
/* 0x007C*/    s32                F_3_0;
/* 0x0080*/    s32                UD_HIGH_LIM;
/* 0x0084*/    s32                F_400_HZ;
/* 0x0088*/    s32                F_10_0;
/* 0x008C*/    s32                Net_0_S32;
/* 0x0090*/    s32                ACCEL1;
/* 0x0094*/    s32                ACCEL2;
/* 0x0098*/    s32                AIN1;
/* 0x009C*/    s32                AIN2;
/* 0x00A0*/    s32                AIN3;
/* 0x00A4*/    s32                AIN4;
/* 0x00A8*/    s32                AIN_1_FORMAT;
/* 0x00AC*/    s32                AIN_1_OUT;
/* 0x00B0*/    s32                AIN_2_FORMAT;
/* 0x00B4*/    s32                AIN_2_OUT;
/* 0x00B8*/    s32                AIN_3_FORMAT;
/* 0x00BC*/    s32                AIN_3_OUT;
/* 0x00C0*/    s32                AIN_4_FORMAT;
/* 0x00C4*/    s32                AIN_4_OUT;
/* 0x00C8*/    s32                AOUT3_MAXIMUM;
/* 0x00CC*/    s32                AOUT3_MINIMUM;
/* 0x00D0*/    s32                AOUT4_MAXIMUM;
/* 0x00D4*/    s32                AOUT4_MINIMUM;
/* 0x00D8*/    s32                AUTOVENTIL_F_OUT;
/* 0x00DC*/    s32                BUS31_63;
/* 0x00E0*/    s32                COS_FI;
/* 0x00E4*/    s32                CURR_OGR;
/* 0x00E8*/    s32                CUR_REF;
/* 0x00EC*/    s32                DECEL1;
/* 0x00F0*/    s32                DECEL2;
/* 0x00F4*/    s32                DERAG_REF;
/* 0x00F8*/    s32                DIN_SRC0_31;
/* 0x00FC*/    s32                ENCODER_F_ROTOR;
/* 0x0100*/    s32                ENCODER_PHASING_SPEED;
/* 0x0104*/    s32                ENCODER_PHASING_WORK_SPD;
/* 0x0108*/    s32                ERR_PID;
/* 0x010C*/    s32                F1_ZAPRET_MAX;
/* 0x0110*/    s32                F1_ZAPRET_MIN;
/* 0x0114*/    s32                F2_ZAPRET_MAX;
/* 0x0118*/    s32                F2_ZAPRET_MIN;
/* 0x011C*/    s32                F3_ZAPRET_MAX;
/* 0x0120*/    s32                F3_ZAPRET_MIN;
/* 0x0124*/    s32                F4_ZAPRET_MAX;
/* 0x0128*/    s32                F4_ZAPRET_MIN;
/* 0x012C*/    s32                F5_ZAPRET_MAX;
/* 0x0130*/    s32                F5_ZAPRET_MIN;
/* 0x0134*/    s32                FAIL_MAX_AIN_1;
/* 0x0138*/    s32                FAIL_MAX_AIN_2;
/* 0x013C*/    s32                FAIL_MAX_AIN_3;
/* 0x0140*/    s32                FAIL_MAX_AIN_4;
/* 0x0144*/    s32                FAIL_MIN_AIN_1;
/* 0x0148*/    s32                FAIL_MIN_AIN_2;
/* 0x014C*/    s32                FAIL_MIN_AIN_3;
/* 0x0150*/    s32                FAIL_MIN_AIN_4;
/* 0x0154*/    s32                FREQ_FIX_MAX_1;
/* 0x0158*/    s32                FREQ_FIX_MAX_2;
/* 0x015C*/    s32                FREQ_FIX_MAX_3;
/* 0x0160*/    s32                FREQ_FIX_MAX_4;
/* 0x0164*/    s32                FREQ_FIX_MIN_1;
/* 0x0168*/    s32                FREQ_FIX_MIN_2;
/* 0x016C*/    s32                FREQ_FIX_MIN_3;
/* 0x0170*/    s32                FREQ_FIX_MIN_4;
/* 0x0174*/    s32                F_IN_AOUT;
/* 0x0178*/    s32                F_OUT;
/* 0x017C*/    s32                F_OUT_ABS;
/* 0x0180*/    s32                F_PICKUP;
/* 0x0184*/    s32                F_REF_AIN_1;
/* 0x0188*/    s32                F_REF_AIN_2;
/* 0x018C*/    s32                F_REF_AIN_3;
/* 0x0190*/    s32                F_REF_AIN_4;
/* 0x0194*/    s32                F_REF_AOUT;
/* 0x0198*/    s32                F_REF_ASU;
/* 0x019C*/    s32                F_REF_DIN;
/* 0x01A0*/    s32                F_REF_DIN_UP_DOWN;
/* 0x01A4*/    s32                F_REF_DRV;
/* 0x01A8*/    s32                F_REF_PULT_WIFI;
/* 0x01AC*/    s32                F_ROTOR;
/* 0x01B0*/    s32                IQ_0;
/* 0x01B4*/    s32                I_ACT;
/* 0x01B8*/    s32                I_FUL;
/* 0x01BC*/    s32                I_MAX_MOT_IQ;
/* 0x01C0*/    s32                I_REACT;
/* 0x01C4*/    s32                KP;
/* 0x01C8*/    s32                MAX_AIN_1;
/* 0x01CC*/    s32                MAX_AIN_2;
/* 0x01D0*/    s32                MAX_AIN_3;
/* 0x01D4*/    s32                MAX_AIN_4;
/* 0x01D8*/    s32                MAX_PID_ADAPTIVE;
/* 0x01DC*/    s32                ME_IQ;
/* 0x01E0*/    s32                ME_Z_IQ;
/* 0x01E4*/    s32                MIN_AIN_1;
/* 0x01E8*/    s32                MIN_AIN_2;
/* 0x01EC*/    s32                MIN_AIN_3;
/* 0x01F0*/    s32                MIN_AIN_4;
/* 0x01F4*/    s32                MIN_PID_ADAPTIVE;
/* 0x01F8*/    s32                NET00000;
/* 0x01FC*/    s32                NET00001;
/* 0x0200*/    s32                NET00002;
/* 0x0204*/    s32                NET00003;
/* 0x0208*/    s32                NET00004;
/* 0x020C*/    s32                NET00005;
/* 0x0210*/    s32                NET00006;
/* 0x0214*/    s32                NET00007;
/* 0x0218*/    s32                NET00008;
/* 0x021C*/    s32                NET00020;
/* 0x0220*/    s32                NET00022;
/* 0x0224*/    s32                NET00025;
/* 0x0228*/    s32                NET00028;
/* 0x022C*/    s32                NET00030;
/* 0x0230*/    s32                NET00037;
/* 0x0234*/    s32                NET00045;
/* 0x0238*/    s32                NET00067;
/* 0x023C*/    s32                NET00073;
/* 0x0240*/    s32                NET00077;
/* 0x0244*/    s32                NET00081;
/* 0x0248*/    s32                NET00087;
/* 0x024C*/    s32                NET00088;
/* 0x0250*/    s32                NET00089;
/* 0x0254*/    s32                NET00091;
/* 0x0258*/    s32                NET00093;
/* 0x025C*/    s32                NET00095;
/* 0x0260*/    s32                NET00096;
/* 0x0264*/    s32                NET00097;
/* 0x0268*/    s32                NET00098;
/* 0x026C*/    s32                NET00100;
/* 0x0270*/    s32                NET00101;
/* 0x0274*/    s32                NET00102;
/* 0x0278*/    s32                NET00111;
/* 0x027C*/    s32                NET00119;
/* 0x0280*/    s32                NET00125;
/* 0x0284*/    s32                NET00132;
/* 0x0288*/    s32                NET00138;
/* 0x028C*/    s32                NET00140;
/* 0x0290*/    s32                NET00161;
/* 0x0294*/    s32                NET00178;
/* 0x0298*/    s32                NET00194;
/* 0x029C*/    s32                NET00199;
/* 0x02A0*/    s32                NET00203;
/* 0x02A4*/    s32                NET00206;
/* 0x02A8*/    s32                NET00208;
/* 0x02AC*/    s32                NET00209;
/* 0x02B0*/    s32                NET00218;
/* 0x02B4*/    s32                NET00219;
/* 0x02B8*/    s32                NET00222;
/* 0x02BC*/    s32                NET00224;
/* 0x02C0*/    s32                NET00226;
/* 0x02C4*/    s32                NET00230;
/* 0x02C8*/    s32                NET00231;
/* 0x02CC*/    s32                NET00236;
/* 0x02D0*/    s32                NET00237;
/* 0x02D4*/    s32                NET00238;
/* 0x02D8*/    s32                NET00239;
/* 0x02DC*/    s32                NET00243;
/* 0x02E0*/    s32                NET00244;
/* 0x02E4*/    s32                NET00245;
/* 0x02E8*/    s32                NET00248;
/* 0x02EC*/    s32                NET00251;
/* 0x02F0*/    s32                NET00272;
/* 0x02F4*/    s32                NET00278;
/* 0x02F8*/    s32                NET00289;
/* 0x02FC*/    s32                NET00290;
/* 0x0300*/    s32                NET00291;
/* 0x0304*/    s32                NET00292;
/* 0x0308*/    s32                NET00306;
/* 0x030C*/    s32                NET00307;
/* 0x0310*/    s32                NET00311;
/* 0x0314*/    s32                NET00314;
/* 0x0318*/    s32                NET00317;
/* 0x031C*/    s32                NET00319;
/* 0x0320*/    s32                NET00322;
/* 0x0324*/    s32                NET00327;
/* 0x0328*/    s32                NET00334;
/* 0x032C*/    s32                NET00335;
/* 0x0330*/    s32                NET00336;
/* 0x0334*/    s32                NET00340;
/* 0x0338*/    s32                NET00341;
/* 0x033C*/    s32                NET00342;
/* 0x0340*/    s32                NET00344;
/* 0x0344*/    s32                NET00346;
/* 0x0348*/    s32                NET00351;
/* 0x034C*/    s32                NET00365;
/* 0x0350*/    s32                NET00369;
/* 0x0354*/    s32                NET00372;
/* 0x0358*/    s32                NET00373;
/* 0x035C*/    s32                NET00379;
/* 0x0360*/    s32                NET00380;
/* 0x0364*/    s32                NET00381;
/* 0x0368*/    s32                NET00395;
/* 0x036C*/    s32                NET00402;
/* 0x0370*/    s32                NET00443;
/* 0x0374*/    s32                NET00452;
/* 0x0378*/    s32                NET00454;
/* 0x037C*/    s32                NET00458;
/* 0x0380*/    s32                NET00459;
/* 0x0384*/    s32                NET00469;
/* 0x0388*/    s32                NET00475;
/* 0x038C*/    s32                NET00488;
/* 0x0390*/    s32                NET00498;
/* 0x0394*/    s32                NET00502;
/* 0x0398*/    s32                NET00504;
/* 0x039C*/    s32                NET00518;
/* 0x03A0*/    s32                NET00523;
/* 0x03A4*/    s32                NET00524;
/* 0x03A8*/    s32                NET00525;
/* 0x03AC*/    s32                NET00529;
/* 0x03B0*/    s32                NET00533;
/* 0x03B4*/    s32                NET00537;
/* 0x03B8*/    s32                NET00548;
/* 0x03BC*/    s32                NET00555;
/* 0x03C0*/    s32                NET00566;
/* 0x03C4*/    s32                NET00567;
/* 0x03C8*/    s32                NET00569;
/* 0x03CC*/    s32                NET00577;
/* 0x03D0*/    s32                NET00579;
/* 0x03D4*/    s32                NET00582;
/* 0x03D8*/    s32                NET00584;
/* 0x03DC*/    s32                NET00586;
/* 0x03E0*/    s32                NET00588;
/* 0x03E4*/    s32                NET00589;
/* 0x03E8*/    s32                NET00590;
/* 0x03EC*/    s32                NET00613;
/* 0x03F0*/    s32                NET00614;
/* 0x03F4*/    s32                NET00618;
/* 0x03F8*/    s32                NET00619;
/* 0x03FC*/    s32                NET00622;
/* 0x0400*/    s32                NET00639;
/* 0x0404*/    s32                NET00640;
/* 0x0408*/    s32                NET00642;
/* 0x040C*/    s32                NET00653;
/* 0x0410*/    s32                NET00657;
/* 0x0414*/    s32                NET00658;
/* 0x0418*/    s32                NET00659;
/* 0x041C*/    s32                NET00671;
/* 0x0420*/    s32                NET00684;
/* 0x0424*/    s32                NET00685;
/* 0x0428*/    s32                NET00687;
/* 0x042C*/    s32                NET00696;
/* 0x0430*/    s32                NET00709;
/* 0x0434*/    s32                NET00723;
/* 0x0438*/    s32                NET00727;
/* 0x043C*/    s32                NET00728;
/* 0x0440*/    s32                NET00729;
/* 0x0444*/    s32                NET00731;
/* 0x0448*/    s32                NET00732;
/* 0x044C*/    s32                NET00733;
/* 0x0450*/    s32                NET00770;
/* 0x0454*/    s32                NET00778;
/* 0x0458*/    s32                NET00781;
/* 0x045C*/    s32                NET00785;
/* 0x0460*/    s32                NET01013;
/* 0x0464*/    s32                NET01068;
/* 0x0468*/    s32                NET01080;
/* 0x046C*/    s32                NET01169;
/* 0x0470*/    s32                NET01170;
/* 0x0474*/    s32                NET01171;
/* 0x0478*/    s32                NET01172;
/* 0x047C*/    s32                NET01173;
/* 0x0480*/    s32                NET01175;
/* 0x0484*/    s32                NET01176;
/* 0x0488*/    s32                NET01177;
/* 0x048C*/    s32                NET01178;
/* 0x0490*/    s32                NET01179;
/* 0x0494*/    s32                NET01180;
/* 0x0498*/    s32                NET01181;
/* 0x049C*/    s32                NET01182;
/* 0x04A0*/    s32                NET01183;
/* 0x04A4*/    s32                NET01184;
/* 0x04A8*/    s32                NET01185;
/* 0x04AC*/    s32                OBRATN_SVIAZ_PID;
/* 0x04B0*/    s32                OBRATN_SVIAZ_PID_AOUT;
/* 0x04B4*/    s32                OS_PID_FUNC_FC;
/* 0x04B8*/    s32                P1;
/* 0x04BC*/    s32                P2;
/* 0x04C0*/    s32                P3;
/* 0x04C4*/    s32                P4;
/* 0x04C8*/    s32                P5;
/* 0x04CC*/    s32                PID_MAX_VAL;
/* 0x04D0*/    s32                PID_OUT_2;
/* 0x04D4*/    s32                PID_REF;
/* 0x04D8*/    s32                PID_REF_DIN_UP_DOWN;
/* 0x04DC*/    s32                PUMP_CLEAN_REF;
/* 0x04E0*/    s32                P_ACT;
/* 0x04E4*/    s32                P_DV_OGR;
/* 0x04E8*/    s32                P_FULL;
/* 0x04EC*/    s32                P_GEN_OGR;
/* 0x04F0*/    s32                P_REACT;
/* 0x04F4*/    s32                REG_IQ_OUT;
/* 0x04F8*/    s32                TORQUE_REF;
/* 0x04FC*/    s32                TORQUE_ZAD;
/* 0x0500*/    s32                T_MAX_OGR;
/* 0x0504*/    s32                T_MIN_OGR;
/* 0x0508*/    s32                UD_MAX_PU;
/* 0x050C*/    s32                UD_MIN_EVAC;
/* 0x0510*/    s32                UD_MIN_UST;
/* 0x0514*/    s32                UD_TEK;
/* 0x0518*/    s32                UD_TORM_MAX;
/* 0x051C*/    s32                UD_TORM_MIN;
/* 0x0520*/    s32                UIN_MAX;
/* 0x0524*/    s32                UIN_MIN;
/* 0x0528*/    s32                UNIX_TIME;
/* 0x052C*/    s32                UST_BIT_ZAD_1;
/* 0x0530*/    s32                UST_BIT_ZAD_2;
/* 0x0534*/    s32                UST_BIT_ZAD_3;
/* 0x0538*/    s32                UST_BIT_ZAD_4;
/* 0x053C*/    s32                UST_FIX_ZAD_1;
/* 0x0540*/    s32                UST_FIX_ZAD_2;
/* 0x0544*/    s32                UST_FIX_ZAD_3;
/* 0x0548*/    s32                UST_FIX_ZAD_4;
/* 0x054C*/    s32                UST_FMAX_REG;
/* 0x0550*/    s32                UST_FMIN_REG;
/* 0x0554*/    s32                UST_F_MAX;
/* 0x0558*/    s32                UST_F_MIN;
/* 0x055C*/    s32                UST_F_PUSK;
/* 0x0560*/    s32                UST_F_ZAD_1;
/* 0x0564*/    s32                UST_F_ZAD_10;
/* 0x0568*/    s32                UST_F_ZAD_11;
/* 0x056C*/    s32                UST_F_ZAD_12;
/* 0x0570*/    s32                UST_F_ZAD_13;
/* 0x0574*/    s32                UST_F_ZAD_14;
/* 0x0578*/    s32                UST_F_ZAD_15;
/* 0x057C*/    s32                UST_F_ZAD_2;
/* 0x0580*/    s32                UST_F_ZAD_3;
/* 0x0584*/    s32                UST_F_ZAD_4;
/* 0x0588*/    s32                UST_F_ZAD_5;
/* 0x058C*/    s32                UST_F_ZAD_6;
/* 0x0590*/    s32                UST_F_ZAD_7;
/* 0x0594*/    s32                UST_F_ZAD_8;
/* 0x0598*/    s32                UST_F_ZAD_9;
/* 0x059C*/    s32                UST_I_NOM_MOT;
/* 0x05A0*/    s32                U_OUT;
/* 0x05A4*/    s32                U_PICKUP;
/* 0x05A8*/    s32                U_REF_DRV;
/* 0x05AC*/    s32                ZADANIE_DLA_PID;
/* 0x05B0*/    s32                ZADANIE_DLA_PID_AOUT;
/* 0x05B4*/    s32                Fict_S32;
/* 0x05B8*/    s16                Net_0;
/* 0x05BA*/    s16                Net_1;
/* 0x05BC*/    s16                Net_2;
/* 0x05BE*/    s16                Net_3;
/* 0x05C0*/    s16                Net_4;
/* 0x05C2*/    s16                Net_5;
/* 0x05C4*/    s16                Net_6;
/* 0x05C6*/    s16                Net_7;
/* 0x05C8*/    s16                Net_8;
/* 0x05CA*/    s16                Net_9;
/* 0x05CC*/    s16                Net_10;
/* 0x05CE*/    s16                Net_11;
/* 0x05D0*/    s16                Net_12;
/* 0x05D2*/    s16                Net_13;
/* 0x05D4*/    s16                Net_14;
/* 0x05D6*/    s16                Net_17;
/* 0x05D8*/    s16                Net_20;
/* 0x05DA*/    s16                Net_380;
/* 0x05DC*/    s16                Net_800;
/* 0x05DE*/    s16                Net_1000;
/* 0x05E0*/    s16                UD_HIGH_ABS_CONST;
/* 0x05E2*/    s16                Net_2000;
/* 0x05E4*/    s16                ACC_TIME;
/* 0x05E6*/    s16                ACTIVE_CHANLE;
/* 0x05E8*/    s16                AIN1_ALARM_CODE;
/* 0x05EA*/    s16                AIN1_APV_ENABLE;
/* 0x05EC*/    s16                AIN1_DEL_CONTR;
/* 0x05EE*/    s16                AIN1_DEL_OFF;
/* 0x05F0*/    s16                AIN1_EVENT_STATE;
/* 0x05F2*/    s16                AIN1_FAIL_ACTION;
/* 0x05F4*/    s16                AIN1_FAIL_CODE;
/* 0x05F6*/    s16                AIN2_ALARM_CODE;
/* 0x05F8*/    s16                AIN2_APV_ENABLE;
/* 0x05FA*/    s16                AIN2_DEL_CONTR;
/* 0x05FC*/    s16                AIN2_DEL_OFF;
/* 0x05FE*/    s16                AIN2_EVENT_STATE;
/* 0x0600*/    s16                AIN2_FAIL_ACTION;
/* 0x0602*/    s16                AIN2_FAIL_CODE;
/* 0x0604*/    s16                AIN3_ADC_CODE;
/* 0x0606*/    s16                AIN3_ALARM_CODE;
/* 0x0608*/    s16                AIN3_APV_ENABLE;
/* 0x060A*/    s16                AIN3_CALIBRATION;
/* 0x060C*/    s16                AIN3_DEL_CONTR;
/* 0x060E*/    s16                AIN3_DEL_OFF;
/* 0x0610*/    s16                AIN3_ED_IZM;
/* 0x0612*/    s16                AIN3_EVENT_STATE;
/* 0x0614*/    s16                AIN3_FAIL_ACTION;
/* 0x0616*/    s16                AIN3_FAIL_CODE;
/* 0x0618*/    s16                AIN3_STEPEN;
/* 0x061A*/    s16                AIN3_TYPE_SIGNAL;
/* 0x061C*/    s16                AIN3_T_FILTER;
/* 0x061E*/    s16                AIN4_ADC_CODE;
/* 0x0620*/    s16                AIN4_ALARM_CODE;
/* 0x0622*/    s16                AIN4_APV_ENABLE;
/* 0x0624*/    s16                AIN4_CALIBRATION;
/* 0x0626*/    s16                AIN4_DEL_CONTR;
/* 0x0628*/    s16                AIN4_DEL_OFF;
/* 0x062A*/    s16                AIN4_ED_IZM;
/* 0x062C*/    s16                AIN4_EVENT_STATE;
/* 0x062E*/    s16                AIN4_FAIL_ACTION;
/* 0x0630*/    s16                AIN4_FAIL_CODE;
/* 0x0632*/    s16                AIN4_STEPEN;
/* 0x0634*/    s16                AIN4_TYPE_SIGNAL;
/* 0x0636*/    s16                AIN4_T_FILTER;
/* 0x0638*/    s16                AOUT3_CALIBR;
/* 0x063A*/    s16                AOUT3_FUNCTION;
/* 0x063C*/    s16                AOUT3_POLARITY;
/* 0x063E*/    s16                AOUT3_TEST_VAL;
/* 0x0640*/    s16                AOUT3_TYPE;
/* 0x0642*/    s16                AOUT4_CALIBR;
/* 0x0644*/    s16                AOUT4_FUNCTION;
/* 0x0646*/    s16                AOUT4_POLARITY;
/* 0x0648*/    s16                AOUT4_TEST_VAL;
/* 0x064A*/    s16                AOUT4_TYPE;
/* 0x064C*/    s16                APV_ALL_CNT;
/* 0x064E*/    s16                APV_ALL_TIME;
/* 0x0650*/    s16                APV_SETTING;
/* 0x0652*/    s16                APV_US_CNT;
/* 0x0654*/    s16                BASE_NOMER10;
/* 0x0656*/    s16                BASE_NOMER11;
/* 0x0658*/    s16                BASE_NOMER9;
/* 0x065A*/    s16                BRAKE_EN;
/* 0x065C*/    s16                BRAKING_TYPE;
/* 0x065E*/    s16                CHAN_CNT_1;
/* 0x0660*/    s16                CHAN_SEL;
/* 0x0662*/    s16                CHAN_TASK_1;
/* 0x0664*/    s16                CONST_RS;
/* 0x0666*/    s16                CONTACTOR_ERR_EN;
/* 0x0668*/    s16                CONT_MODE;
/* 0x066A*/    s16                CP24B_VISIBLE;
/* 0x066C*/    s16                CP24_VISIBLE;
/* 0x066E*/    s16                CURRENT_BASE;
/* 0x0670*/    s16                CURRENT_INPUT_VOLTAGE;
/* 0x0672*/    s16                CURRENT_WARN;
/* 0x0674*/    s16                DATE_FAIL;
/* 0x0676*/    s16                DAY;
/* 0x0678*/    s16                DEC_TIME;
/* 0x067A*/    s16                DEC_TIME_ORIGIN;
/* 0x067C*/    s16                DERAG_DIN;
/* 0x067E*/    s16                DF_NO_SPEED_STOP;
/* 0x0680*/    s16                DF_NUM_SEL;
/* 0x0682*/    s16                DF_REVERS;
/* 0x0684*/    s16                DIN10_FUNC;
/* 0x0686*/    s16                DIN10_TEK;
/* 0x0688*/    s16                DIN11_FUNC;
/* 0x068A*/    s16                DIN11_TEK;
/* 0x068C*/    s16                DIN12_FUNC;
/* 0x068E*/    s16                DIN12_TEK;
/* 0x0690*/    s16                DIN13_FUNC;
/* 0x0692*/    s16                DIN13_TEK;
/* 0x0694*/    s16                DIN1_13;
/* 0x0696*/    s16                DIN1_FUNC;
/* 0x0698*/    s16                DIN1_TEK;
/* 0x069A*/    s16                DIN2_FUNC;
/* 0x069C*/    s16                DIN2_TEK;
/* 0x069E*/    s16                DIN3_FUNC;
/* 0x06A0*/    s16                DIN3_TEK;
/* 0x06A2*/    s16                DIN4_FUNC;
/* 0x06A4*/    s16                DIN4_TEK;
/* 0x06A6*/    s16                DIN5_FUNC;
/* 0x06A8*/    s16                DIN5_TEK;
/* 0x06AA*/    s16                DIN6_FUNC;
/* 0x06AC*/    s16                DIN6_TEK;
/* 0x06AE*/    s16                DIN7_FUNC;
/* 0x06B0*/    s16                DIN7_TEK;
/* 0x06B2*/    s16                DIN8_FUNC;
/* 0x06B4*/    s16                DIN8_TEK;
/* 0x06B6*/    s16                DIN9_FUNC;
/* 0x06B8*/    s16                DIN9_TEK;
/* 0x06BA*/    s16                DIN_10_FILTR;
/* 0x06BC*/    s16                DIN_10_LOGIC;
/* 0x06BE*/    s16                DIN_11_FILTR;
/* 0x06C0*/    s16                DIN_11_LOGIC;
/* 0x06C2*/    s16                DIN_12_FILTR;
/* 0x06C4*/    s16                DIN_12_LOGIC;
/* 0x06C6*/    s16                DIN_13_FILTR;
/* 0x06C8*/    s16                DIN_13_LOGIC;
/* 0x06CA*/    s16                DIN_8_FILTR;
/* 0x06CC*/    s16                DIN_8_LOGIC;
/* 0x06CE*/    s16                DIN_9_FILTR;
/* 0x06D0*/    s16                DIN_9_LOGIC;
/* 0x06D2*/    s16                DOUT_1_12;
/* 0x06D4*/    s16                DTC_COMP_MODE;
/* 0x06D6*/    s16                EM_FREQ_STOP;
/* 0x06D8*/    s16                ENCODER_PHASE;
/* 0x06DA*/    s16                ENCODER_PHASING_ACCEL;
/* 0x06DC*/    s16                ENCODER_PHASING_CURRENT;
/* 0x06DE*/    s16                ENCODER_PHASING_DECEL;
/* 0x06E0*/    s16                ENCODER_PHASING_ENABLE;
/* 0x06E2*/    s16                ENCO_ANGLE_SHIFT;
/* 0x06E4*/    s16                ENCO_CALC_PERIOD;
/* 0x06E6*/    s16                ENCO_EMULATOR_PULSE_CNT;
/* 0x06E8*/    s16                ENCO_FAST_SPEED;
/* 0x06EA*/    s16                ENCO_FAST_SPEED_SIGN;
/* 0x06EC*/    s16                ENCO_PHASING;
/* 0x06EE*/    s16                ENCO_PROTOCOL_TYPE;
/* 0x06F0*/    s16                ENCO_RESOLUTION_BIT;
/* 0x06F2*/    s16                ENCO_RESOLUTION_MARK;
/* 0x06F4*/    s16                ENCO_RESOLUTION_TYPE;
/* 0x06F6*/    s16                ENCO_SINCOS_AMPL_MUL;
/* 0x06F8*/    s16                ENCO_SPEED_FILTER;
/* 0x06FA*/    s16                ENCO_THETA_FILTER;
/* 0x06FC*/    s16                EN_CON_STATE;
/* 0x06FE*/    s16                ERR_MICROPROG;
/* 0x0700*/    s16                ERR_MICRO_2;
/* 0x0702*/    s16                EVAC_DIR;
/* 0x0704*/    s16                EVAC_DIR_MODE;
/* 0x0706*/    s16                EVAK_FUNC_EN;
/* 0x0708*/    s16                EXT1_INFO_VISIBLE;
/* 0x070A*/    s16                EXT2_INFO_VISIBLE;
/* 0x070C*/    s16                EXT3_INFO_VISIBLE;
/* 0x070E*/    s16                EXT_AIO2_VISIBLE;
/* 0x0710*/    s16                EXT_ANY_ENCO_VISIBLE;
/* 0x0712*/    s16                EXT_DIN_VISIBLE;
/* 0x0714*/    s16                EXT_ENCO_ENDAT_SC_VISIBLE;
/* 0x0716*/    s16                EXT_ENCO_ENDAT_VISIBLE;
/* 0x0718*/    s16                EXT_ENCO_INC_VISIBLE;
/* 0x071A*/    s16                EXT_ENCO_SC_VISIBLE;
/* 0x071C*/    s16                EXT_ERROR_AIO2_VISIBLE;
/* 0x071E*/    s16                EXT_ERROR_RS2_VISIBLE;
/* 0x0720*/    s16                EXT_ETHERNET_VISIBLE;
/* 0x0722*/    s16                EXT_PROFIBUS_VISIBLE;
/* 0x0724*/    s16                EXT_PROFINET_VISIBLE;
/* 0x0726*/    s16                EXT_RS2_BDR3;
/* 0x0728*/    s16                EXT_RS2_BDR4;
/* 0x072A*/    s16                EXT_RS2_NETADR3;
/* 0x072C*/    s16                EXT_RS2_NETADR4;
/* 0x072E*/    s16                EXT_RS2_TIMCNTRL3;
/* 0x0730*/    s16                EXT_RS2_TIMCNTRL4;
/* 0x0732*/    s16                EXT_RS2_TIMEOUT3;
/* 0x0734*/    s16                EXT_RS2_TIMEOUT4;
/* 0x0736*/    s16                EXT_RS2_VISIBLE;
/* 0x0738*/    s16                FAIL_32_TO_47;
/* 0x073A*/    s16                FAIL_48_TO_63;
/* 0x073C*/    s16                FAIL_64_TO_80;
/* 0x073E*/    s16                FAIL_81_TO_96;
/* 0x0740*/    s16                FAIL_FIX_DRV;
/* 0x0742*/    s16                FAIL_MASK_0;
/* 0x0744*/    s16                FAIL_MASK_1;
/* 0x0746*/    s16                FAIL_TEK_DRV;
/* 0x0748*/    s16                FAIL_WARNING;
/* 0x074A*/    s16                FMAX_APV_ENABLE;
/* 0x074C*/    s16                FMIN_APV_ENABLE;
/* 0x074E*/    s16                FORMAT_AIN_1;
/* 0x0750*/    s16                FORMAT_AIN_2;
/* 0x0752*/    s16                FORMAT_AIN_3;
/* 0x0754*/    s16                FORMAT_AIN_4;
/* 0x0756*/    s16                FORMAT_AOUT1;
/* 0x0758*/    s16                FORMAT_AOUT2;
/* 0x075A*/    s16                FORMAT_AOUT3;
/* 0x075C*/    s16                FORMAT_AOUT4;
/* 0x075E*/    s16                FORMAT_PID;
/* 0x0760*/    s16                FORMAT_PID_ADAPTIVE;
/* 0x0762*/    s16                FOUT_FAIL;
/* 0x0764*/    s16                FUNCTIONAL;
/* 0x0766*/    s16                F_REF_S16_XXX_X;
/* 0x0768*/    s16                GT_FORMAT;
/* 0x076A*/    s16                GT_TYPE;
/* 0x076C*/    s16                HOUR;
/* 0x076E*/    s16                IACT_FAIL;
/* 0x0770*/    s16                IA_FAIL;
/* 0x0772*/    s16                IB_FAIL;
/* 0x0774*/    s16                IC_FAIL;
/* 0x0776*/    s16                IFULL_FAIL;
/* 0x0778*/    s16                IGBT_KEY_RESISTANCE;
/* 0x077A*/    s16                INDEX;
/* 0x077C*/    s16                INDEXFAIL;
/* 0x077E*/    s16                INTEGR_RS;
/* 0x0780*/    s16                I_MIN_COMPENSATE;
/* 0x0782*/    s16                I_NOM_MOT;
/* 0x0784*/    s16                I_U;
/* 0x0786*/    s16                I_USTIR;
/* 0x0788*/    s16                I_V;
/* 0x078A*/    s16                I_W;
/* 0x078C*/    s16                LIFT_LOAD_CAPACITY;
/* 0x078E*/    s16                LIFT_NOM_SPD;
/* 0x0790*/    s16                LIFT_SPD_ED_IZM;
/* 0x0792*/    s16                MANAUL_AUTO_SET;
/* 0x0794*/    s16                MAXIMUM_INPUT_VOLTAGE;
/* 0x0796*/    s16                ME;
/* 0x0798*/    s16                MEAS_PARAM_MODE;
/* 0x079A*/    s16                MINUTE;
/* 0x079C*/    s16                MONTH;
/* 0x079E*/    s16                MOT1_4_EN;
/* 0x07A0*/    s16                MOTOR_COS_FI;
/* 0x07A2*/    s16                MOTOR_KPD;
/* 0x07A4*/    s16                MOTOR_TMAX_ACTION;
/* 0x07A6*/    s16                MOTOR_TYPE;
/* 0x07A8*/    s16                MOTOR_T_MAX;
/* 0x07AA*/    s16                MOTOR_WORK_TIME;
/* 0x07AC*/    s16                NET00009;
/* 0x07AE*/    s16                NET00010;
/* 0x07B0*/    s16                NET00011;
/* 0x07B2*/    s16                NET00013;
/* 0x07B4*/    s16                NET00017;
/* 0x07B6*/    s16                NET00018;
/* 0x07B8*/    s16                NET00019;
/* 0x07BA*/    s16                NET00021;
/* 0x07BC*/    s16                NET00027;
/* 0x07BE*/    s16                NET00031;
/* 0x07C0*/    s16                NET00032;
/* 0x07C2*/    s16                NET00033;
/* 0x07C4*/    s16                NET00034;
/* 0x07C6*/    s16                NET00035;
/* 0x07C8*/    s16                NET00036;
/* 0x07CA*/    s16                NET00040;
/* 0x07CC*/    s16                NET00041;
/* 0x07CE*/    s16                NET00042;
/* 0x07D0*/    s16                NET00043;
/* 0x07D2*/    s16                NET00044;
/* 0x07D4*/    s16                NET00046;
/* 0x07D6*/    s16                NET00048;
/* 0x07D8*/    s16                NET00049;
/* 0x07DA*/    s16                NET00050;
/* 0x07DC*/    s16                NET00051;
/* 0x07DE*/    s16                NET00052;
/* 0x07E0*/    s16                NET00053;
/* 0x07E2*/    s16                NET00054;
/* 0x07E4*/    s16                NET00055;
/* 0x07E6*/    s16                NET00056;
/* 0x07E8*/    s16                NET00057;
/* 0x07EA*/    s16                NET00058;
/* 0x07EC*/    s16                NET00059;
/* 0x07EE*/    s16                NET00060;
/* 0x07F0*/    s16                NET00061;
/* 0x07F2*/    s16                NET00062;
/* 0x07F4*/    s16                NET00063;
/* 0x07F6*/    s16                NET00064;
/* 0x07F8*/    s16                NET00065;
/* 0x07FA*/    s16                NET00068;
/* 0x07FC*/    s16                NET00069;
/* 0x07FE*/    s16                NET00070;
/* 0x0800*/    s16                NET00072;
/* 0x0802*/    s16                NET00074;
/* 0x0804*/    s16                NET00075;
/* 0x0806*/    s16                NET00076;
/* 0x0808*/    s16                NET00078;
/* 0x080A*/    s16                NET00079;
/* 0x080C*/    s16                NET00080;
/* 0x080E*/    s16                NET00082;
/* 0x0810*/    s16                NET00083;
/* 0x0812*/    s16                NET00084;
/* 0x0814*/    s16                NET00085;
/* 0x0816*/    s16                NET00086;
/* 0x0818*/    s16                NET00090;
/* 0x081A*/    s16                NET00092;
/* 0x081C*/    s16                NET00094;
/* 0x081E*/    s16                NET00099;
/* 0x0820*/    s16                NET00103;
/* 0x0822*/    s16                NET00105;
/* 0x0824*/    s16                NET00106;
/* 0x0826*/    s16                NET00107;
/* 0x0828*/    s16                NET00108;
/* 0x082A*/    s16                NET00110;
/* 0x082C*/    s16                NET00112;
/* 0x082E*/    s16                NET00113;
/* 0x0830*/    s16                NET00114;
/* 0x0832*/    s16                NET00115;
/* 0x0834*/    s16                NET00116;
/* 0x0836*/    s16                NET00117;
/* 0x0838*/    s16                NET00118;
/* 0x083A*/    s16                NET00120;
/* 0x083C*/    s16                NET00122;
/* 0x083E*/    s16                NET00123;
/* 0x0840*/    s16                NET00124;
/* 0x0842*/    s16                NET00126;
/* 0x0844*/    s16                NET00127;
/* 0x0846*/    s16                NET00128;
/* 0x0848*/    s16                NET00129;
/* 0x084A*/    s16                NET00130;
/* 0x084C*/    s16                NET00131;
/* 0x084E*/    s16                NET00133;
/* 0x0850*/    s16                NET00134;
/* 0x0852*/    s16                NET00136;
/* 0x0854*/    s16                NET00137;
/* 0x0856*/    s16                NET00141;
/* 0x0858*/    s16                NET00143;
/* 0x085A*/    s16                NET00145;
/* 0x085C*/    s16                NET00148;
/* 0x085E*/    s16                NET00149;
/* 0x0860*/    s16                NET00150;
/* 0x0862*/    s16                NET00151;
/* 0x0864*/    s16                NET00152;
/* 0x0866*/    s16                NET00153;
/* 0x0868*/    s16                NET00154;
/* 0x086A*/    s16                NET00155;
/* 0x086C*/    s16                NET00156;
/* 0x086E*/    s16                NET00157;
/* 0x0870*/    s16                NET00158;
/* 0x0872*/    s16                NET00159;
/* 0x0874*/    s16                NET00160;
/* 0x0876*/    s16                NET00162;
/* 0x0878*/    s16                NET00163;
/* 0x087A*/    s16                NET00164;
/* 0x087C*/    s16                NET00165;
/* 0x087E*/    s16                NET00167;
/* 0x0880*/    s16                NET00169;
/* 0x0882*/    s16                NET00170;
/* 0x0884*/    s16                NET00171;
/* 0x0886*/    s16                NET00172;
/* 0x0888*/    s16                NET00173;
/* 0x088A*/    s16                NET00174;
/* 0x088C*/    s16                NET00175;
/* 0x088E*/    s16                NET00176;
/* 0x0890*/    s16                NET00177;
/* 0x0892*/    s16                NET00179;
/* 0x0894*/    s16                NET00180;
/* 0x0896*/    s16                NET00182;
/* 0x0898*/    s16                NET00183;
/* 0x089A*/    s16                NET00184;
/* 0x089C*/    s16                NET00185;
/* 0x089E*/    s16                NET00186;
/* 0x08A0*/    s16                NET00187;
/* 0x08A2*/    s16                NET00188;
/* 0x08A4*/    s16                NET00189;
/* 0x08A6*/    s16                NET00191;
/* 0x08A8*/    s16                NET00192;
/* 0x08AA*/    s16                NET00193;
/* 0x08AC*/    s16                NET00195;
/* 0x08AE*/    s16                NET00197;
/* 0x08B0*/    s16                NET00198;
/* 0x08B2*/    s16                NET00200;
/* 0x08B4*/    s16                NET00201;
/* 0x08B6*/    s16                NET00202;
/* 0x08B8*/    s16                NET00204;
/* 0x08BA*/    s16                NET00205;
/* 0x08BC*/    s16                NET00207;
/* 0x08BE*/    s16                NET00210;
/* 0x08C0*/    s16                NET00211;
/* 0x08C2*/    s16                NET00212;
/* 0x08C4*/    s16                NET00213;
/* 0x08C6*/    s16                NET00214;
/* 0x08C8*/    s16                NET00215;
/* 0x08CA*/    s16                NET00216;
/* 0x08CC*/    s16                NET00217;
/* 0x08CE*/    s16                NET00220;
/* 0x08D0*/    s16                NET00221;
/* 0x08D2*/    s16                NET00225;
/* 0x08D4*/    s16                NET00227;
/* 0x08D6*/    s16                NET00228;
/* 0x08D8*/    s16                NET00229;
/* 0x08DA*/    s16                NET00233;
/* 0x08DC*/    s16                NET00234;
/* 0x08DE*/    s16                NET00235;
/* 0x08E0*/    s16                NET00240;
/* 0x08E2*/    s16                NET00241;
/* 0x08E4*/    s16                NET00242;
/* 0x08E6*/    s16                NET00246;
/* 0x08E8*/    s16                NET00247;
/* 0x08EA*/    s16                NET00252;
/* 0x08EC*/    s16                NET00253;
/* 0x08EE*/    s16                NET00254;
/* 0x08F0*/    s16                NET00255;
/* 0x08F2*/    s16                NET00256;
/* 0x08F4*/    s16                NET00257;
/* 0x08F6*/    s16                NET00258;
/* 0x08F8*/    s16                NET00259;
/* 0x08FA*/    s16                NET00260;
/* 0x08FC*/    s16                NET00261;
/* 0x08FE*/    s16                NET00262;
/* 0x0900*/    s16                NET00263;
/* 0x0902*/    s16                NET00264;
/* 0x0904*/    s16                NET00265;
/* 0x0906*/    s16                NET00266;
/* 0x0908*/    s16                NET00267;
/* 0x090A*/    s16                NET00268;
/* 0x090C*/    s16                NET00269;
/* 0x090E*/    s16                NET00270;
/* 0x0910*/    s16                NET00271;
/* 0x0912*/    s16                NET00274;
/* 0x0914*/    s16                NET00275;
/* 0x0916*/    s16                NET00276;
/* 0x0918*/    s16                NET00279;
/* 0x091A*/    s16                NET00280;
/* 0x091C*/    s16                NET00281;
/* 0x091E*/    s16                NET00282;
/* 0x0920*/    s16                NET00283;
/* 0x0922*/    s16                NET00284;
/* 0x0924*/    s16                NET00285;
/* 0x0926*/    s16                NET00286;
/* 0x0928*/    s16                NET00287;
/* 0x092A*/    s16                NET00288;
/* 0x092C*/    s16                NET00294;
/* 0x092E*/    s16                NET00296;
/* 0x0930*/    s16                NET00297;
/* 0x0932*/    s16                NET00298;
/* 0x0934*/    s16                NET00299;
/* 0x0936*/    s16                NET00300;
/* 0x0938*/    s16                NET00301;
/* 0x093A*/    s16                NET00302;
/* 0x093C*/    s16                NET00303;
/* 0x093E*/    s16                NET00304;
/* 0x0940*/    s16                NET00305;
/* 0x0942*/    s16                NET00308;
/* 0x0944*/    s16                NET00309;
/* 0x0946*/    s16                NET00312;
/* 0x0948*/    s16                NET00313;
/* 0x094A*/    s16                NET00315;
/* 0x094C*/    s16                NET00316;
/* 0x094E*/    s16                NET00318;
/* 0x0950*/    s16                NET00320;
/* 0x0952*/    s16                NET00321;
/* 0x0954*/    s16                NET00323;
/* 0x0956*/    s16                NET00324;
/* 0x0958*/    s16                NET00325;
/* 0x095A*/    s16                NET00326;
/* 0x095C*/    s16                NET00328;
/* 0x095E*/    s16                NET00329;
/* 0x0960*/    s16                NET00330;
/* 0x0962*/    s16                NET00331;
/* 0x0964*/    s16                NET00332;
/* 0x0966*/    s16                NET00333;
/* 0x0968*/    s16                NET00337;
/* 0x096A*/    s16                NET00338;
/* 0x096C*/    s16                NET00339;
/* 0x096E*/    s16                NET00343;
/* 0x0970*/    s16                NET00345;
/* 0x0972*/    s16                NET00347;
/* 0x0974*/    s16                NET00348;
/* 0x0976*/    s16                NET00357;
/* 0x0978*/    s16                NET00358;
/* 0x097A*/    s16                NET00359;
/* 0x097C*/    s16                NET00360;
/* 0x097E*/    s16                NET00361;
/* 0x0980*/    s16                NET00362;
/* 0x0982*/    s16                NET00363;
/* 0x0984*/    s16                NET00364;
/* 0x0986*/    s16                NET00366;
/* 0x0988*/    s16                NET00367;
/* 0x098A*/    s16                NET00368;
/* 0x098C*/    s16                NET00370;
/* 0x098E*/    s16                NET00371;
/* 0x0990*/    s16                NET00374;
/* 0x0992*/    s16                NET00375;
/* 0x0994*/    s16                NET00376;
/* 0x0996*/    s16                NET00377;
/* 0x0998*/    s16                NET00378;
/* 0x099A*/    s16                NET00382;
/* 0x099C*/    s16                NET00384;
/* 0x099E*/    s16                NET00385;
/* 0x09A0*/    s16                NET00386;
/* 0x09A2*/    s16                NET00387;
/* 0x09A4*/    s16                NET00388;
/* 0x09A6*/    s16                NET00389;
/* 0x09A8*/    s16                NET00390;
/* 0x09AA*/    s16                NET00391;
/* 0x09AC*/    s16                NET00392;
/* 0x09AE*/    s16                NET00393;
/* 0x09B0*/    s16                NET00394;
/* 0x09B2*/    s16                NET00396;
/* 0x09B4*/    s16                NET00397;
/* 0x09B6*/    s16                NET00398;
/* 0x09B8*/    s16                NET00400;
/* 0x09BA*/    s16                NET00401;
/* 0x09BC*/    s16                NET00403;
/* 0x09BE*/    s16                NET00404;
/* 0x09C0*/    s16                NET00405;
/* 0x09C2*/    s16                NET00406;
/* 0x09C4*/    s16                NET00407;
/* 0x09C6*/    s16                NET00408;
/* 0x09C8*/    s16                NET00409;
/* 0x09CA*/    s16                NET00410;
/* 0x09CC*/    s16                NET00411;
/* 0x09CE*/    s16                NET00412;
/* 0x09D0*/    s16                NET00413;
/* 0x09D2*/    s16                NET00414;
/* 0x09D4*/    s16                NET00415;
/* 0x09D6*/    s16                NET00416;
/* 0x09D8*/    s16                NET00417;
/* 0x09DA*/    s16                NET00418;
/* 0x09DC*/    s16                NET00419;
/* 0x09DE*/    s16                NET00421;
/* 0x09E0*/    s16                NET00422;
/* 0x09E2*/    s16                NET00423;
/* 0x09E4*/    s16                NET00424;
/* 0x09E6*/    s16                NET00426;
/* 0x09E8*/    s16                NET00429;
/* 0x09EA*/    s16                NET00430;
/* 0x09EC*/    s16                NET00432;
/* 0x09EE*/    s16                NET00433;
/* 0x09F0*/    s16                NET00436;
/* 0x09F2*/    s16                NET00437;
/* 0x09F4*/    s16                NET00438;
/* 0x09F6*/    s16                NET00439;
/* 0x09F8*/    s16                NET00440;
/* 0x09FA*/    s16                NET00441;
/* 0x09FC*/    s16                NET00446;
/* 0x09FE*/    s16                NET00447;
/* 0x0A00*/    s16                NET00448;
/* 0x0A02*/    s16                NET00450;
/* 0x0A04*/    s16                NET00451;
/* 0x0A06*/    s16                NET00453;
/* 0x0A08*/    s16                NET00455;
/* 0x0A0A*/    s16                NET00457;
/* 0x0A0C*/    s16                NET00460;
/* 0x0A0E*/    s16                NET00461;
/* 0x0A10*/    s16                NET00462;
/* 0x0A12*/    s16                NET00463;
/* 0x0A14*/    s16                NET00464;
/* 0x0A16*/    s16                NET00466;
/* 0x0A18*/    s16                NET00468;
/* 0x0A1A*/    s16                NET00471;
/* 0x0A1C*/    s16                NET00472;
/* 0x0A1E*/    s16                NET00473;
/* 0x0A20*/    s16                NET00474;
/* 0x0A22*/    s16                NET00476;
/* 0x0A24*/    s16                NET00477;
/* 0x0A26*/    s16                NET00478;
/* 0x0A28*/    s16                NET00479;
/* 0x0A2A*/    s16                NET00480;
/* 0x0A2C*/    s16                NET00481;
/* 0x0A2E*/    s16                NET00482;
/* 0x0A30*/    s16                NET00483;
/* 0x0A32*/    s16                NET00484;
/* 0x0A34*/    s16                NET00485;
/* 0x0A36*/    s16                NET00486;
/* 0x0A38*/    s16                NET00487;
/* 0x0A3A*/    s16                NET00489;
/* 0x0A3C*/    s16                NET00490;
/* 0x0A3E*/    s16                NET00491;
/* 0x0A40*/    s16                NET00492;
/* 0x0A42*/    s16                NET00493;
/* 0x0A44*/    s16                NET00494;
/* 0x0A46*/    s16                NET00495;
/* 0x0A48*/    s16                NET00496;
/* 0x0A4A*/    s16                NET00497;
/* 0x0A4C*/    s16                NET00499;
/* 0x0A4E*/    s16                NET00500;
/* 0x0A50*/    s16                NET00501;
/* 0x0A52*/    s16                NET00503;
/* 0x0A54*/    s16                NET00505;
/* 0x0A56*/    s16                NET00506;
/* 0x0A58*/    s16                NET00507;
/* 0x0A5A*/    s16                NET00508;
/* 0x0A5C*/    s16                NET00509;
/* 0x0A5E*/    s16                NET00510;
/* 0x0A60*/    s16                NET00511;
/* 0x0A62*/    s16                NET00512;
/* 0x0A64*/    s16                NET00513;
/* 0x0A66*/    s16                NET00514;
/* 0x0A68*/    s16                NET00515;
/* 0x0A6A*/    s16                NET00517;
/* 0x0A6C*/    s16                NET00519;
/* 0x0A6E*/    s16                NET00520;
/* 0x0A70*/    s16                NET00522;
/* 0x0A72*/    s16                NET00526;
/* 0x0A74*/    s16                NET00527;
/* 0x0A76*/    s16                NET00528;
/* 0x0A78*/    s16                NET00530;
/* 0x0A7A*/    s16                NET00531;
/* 0x0A7C*/    s16                NET00532;
/* 0x0A7E*/    s16                NET00534;
/* 0x0A80*/    s16                NET00535;
/* 0x0A82*/    s16                NET00536;
/* 0x0A84*/    s16                NET00538;
/* 0x0A86*/    s16                NET00539;
/* 0x0A88*/    s16                NET00540;
/* 0x0A8A*/    s16                NET00541;
/* 0x0A8C*/    s16                NET00542;
/* 0x0A8E*/    s16                NET00543;
/* 0x0A90*/    s16                NET00544;
/* 0x0A92*/    s16                NET00545;
/* 0x0A94*/    s16                NET00546;
/* 0x0A96*/    s16                NET00547;
/* 0x0A98*/    s16                NET00549;
/* 0x0A9A*/    s16                NET00550;
/* 0x0A9C*/    s16                NET00551;
/* 0x0A9E*/    s16                NET00552;
/* 0x0AA0*/    s16                NET00553;
/* 0x0AA2*/    s16                NET00554;
/* 0x0AA4*/    s16                NET00556;
/* 0x0AA6*/    s16                NET00557;
/* 0x0AA8*/    s16                NET00558;
/* 0x0AAA*/    s16                NET00559;
/* 0x0AAC*/    s16                NET00560;
/* 0x0AAE*/    s16                NET00561;
/* 0x0AB0*/    s16                NET00562;
/* 0x0AB2*/    s16                NET00563;
/* 0x0AB4*/    s16                NET00565;
/* 0x0AB6*/    s16                NET00568;
/* 0x0AB8*/    s16                NET00570;
/* 0x0ABA*/    s16                NET00571;
/* 0x0ABC*/    s16                NET00572;
/* 0x0ABE*/    s16                NET00573;
/* 0x0AC0*/    s16                NET00574;
/* 0x0AC2*/    s16                NET00575;
/* 0x0AC4*/    s16                NET00576;
/* 0x0AC6*/    s16                NET00578;
/* 0x0AC8*/    s16                NET00580;
/* 0x0ACA*/    s16                NET00581;
/* 0x0ACC*/    s16                NET00583;
/* 0x0ACE*/    s16                NET00585;
/* 0x0AD0*/    s16                NET00587;
/* 0x0AD2*/    s16                NET00591;
/* 0x0AD4*/    s16                NET00593;
/* 0x0AD6*/    s16                NET00595;
/* 0x0AD8*/    s16                NET00596;
/* 0x0ADA*/    s16                NET00597;
/* 0x0ADC*/    s16                NET00598;
/* 0x0ADE*/    s16                NET00599;
/* 0x0AE0*/    s16                NET00600;
/* 0x0AE2*/    s16                NET00601;
/* 0x0AE4*/    s16                NET00602;
/* 0x0AE6*/    s16                NET00603;
/* 0x0AE8*/    s16                NET00604;
/* 0x0AEA*/    s16                NET00605;
/* 0x0AEC*/    s16                NET00606;
/* 0x0AEE*/    s16                NET00607;
/* 0x0AF0*/    s16                NET00608;
/* 0x0AF2*/    s16                NET00609;
/* 0x0AF4*/    s16                NET00610;
/* 0x0AF6*/    s16                NET00611;
/* 0x0AF8*/    s16                NET00612;
/* 0x0AFA*/    s16                NET00615;
/* 0x0AFC*/    s16                NET00616;
/* 0x0AFE*/    s16                NET00617;
/* 0x0B00*/    s16                NET00620;
/* 0x0B02*/    s16                NET00621;
/* 0x0B04*/    s16                NET00623;
/* 0x0B06*/    s16                NET00624;
/* 0x0B08*/    s16                NET00625;
/* 0x0B0A*/    s16                NET00626;
/* 0x0B0C*/    s16                NET00627;
/* 0x0B0E*/    s16                NET00628;
/* 0x0B10*/    s16                NET00629;
/* 0x0B12*/    s16                NET00630;
/* 0x0B14*/    s16                NET00631;
/* 0x0B16*/    s16                NET00632;
/* 0x0B18*/    s16                NET00633;
/* 0x0B1A*/    s16                NET00634;
/* 0x0B1C*/    s16                NET00635;
/* 0x0B1E*/    s16                NET00636;
/* 0x0B20*/    s16                NET00637;
/* 0x0B22*/    s16                NET00638;
/* 0x0B24*/    s16                NET00641;
/* 0x0B26*/    s16                NET00643;
/* 0x0B28*/    s16                NET00644;
/* 0x0B2A*/    s16                NET00645;
/* 0x0B2C*/    s16                NET00646;
/* 0x0B2E*/    s16                NET00647;
/* 0x0B30*/    s16                NET00648;
/* 0x0B32*/    s16                NET00649;
/* 0x0B34*/    s16                NET00650;
/* 0x0B36*/    s16                NET00651;
/* 0x0B38*/    s16                NET00652;
/* 0x0B3A*/    s16                NET00654;
/* 0x0B3C*/    s16                NET00655;
/* 0x0B3E*/    s16                NET00656;
/* 0x0B40*/    s16                NET00661;
/* 0x0B42*/    s16                NET00662;
/* 0x0B44*/    s16                NET00664;
/* 0x0B46*/    s16                NET00665;
/* 0x0B48*/    s16                NET00666;
/* 0x0B4A*/    s16                NET00667;
/* 0x0B4C*/    s16                NET00668;
/* 0x0B4E*/    s16                NET00669;
/* 0x0B50*/    s16                NET00670;
/* 0x0B52*/    s16                NET00673;
/* 0x0B54*/    s16                NET00675;
/* 0x0B56*/    s16                NET00676;
/* 0x0B58*/    s16                NET00677;
/* 0x0B5A*/    s16                NET00678;
/* 0x0B5C*/    s16                NET00679;
/* 0x0B5E*/    s16                NET00680;
/* 0x0B60*/    s16                NET00681;
/* 0x0B62*/    s16                NET00682;
/* 0x0B64*/    s16                NET00686;
/* 0x0B66*/    s16                NET00689;
/* 0x0B68*/    s16                NET00694;
/* 0x0B6A*/    s16                NET00697;
/* 0x0B6C*/    s16                NET00698;
/* 0x0B6E*/    s16                NET00699;
/* 0x0B70*/    s16                NET00700;
/* 0x0B72*/    s16                NET00701;
/* 0x0B74*/    s16                NET00702;
/* 0x0B76*/    s16                NET00703;
/* 0x0B78*/    s16                NET00704;
/* 0x0B7A*/    s16                NET00705;
/* 0x0B7C*/    s16                NET00706;
/* 0x0B7E*/    s16                NET00707;
/* 0x0B80*/    s16                NET00708;
/* 0x0B82*/    s16                NET00713;
/* 0x0B84*/    s16                NET00714;
/* 0x0B86*/    s16                NET00715;
/* 0x0B88*/    s16                NET00717;
/* 0x0B8A*/    s16                NET00718;
/* 0x0B8C*/    s16                NET00719;
/* 0x0B8E*/    s16                NET00720;
/* 0x0B90*/    s16                NET00722;
/* 0x0B92*/    s16                NET00724;
/* 0x0B94*/    s16                NET00725;
/* 0x0B96*/    s16                NET00730;
/* 0x0B98*/    s16                NET00734;
/* 0x0B9A*/    s16                NET00735;
/* 0x0B9C*/    s16                NET00736;
/* 0x0B9E*/    s16                NET00737;
/* 0x0BA0*/    s16                NET00738;
/* 0x0BA2*/    s16                NET00739;
/* 0x0BA4*/    s16                NET00740;
/* 0x0BA6*/    s16                NET00741;
/* 0x0BA8*/    s16                NET00742;
/* 0x0BAA*/    s16                NET00743;
/* 0x0BAC*/    s16                NET00744;
/* 0x0BAE*/    s16                NET00745;
/* 0x0BB0*/    s16                NET00746;
/* 0x0BB2*/    s16                NET00747;
/* 0x0BB4*/    s16                NET00748;
/* 0x0BB6*/    s16                NET00749;
/* 0x0BB8*/    s16                NET00750;
/* 0x0BBA*/    s16                NET00751;
/* 0x0BBC*/    s16                NET00752;
/* 0x0BBE*/    s16                NET00754;
/* 0x0BC0*/    s16                NET00755;
/* 0x0BC2*/    s16                NET00756;
/* 0x0BC4*/    s16                NET00758;
/* 0x0BC6*/    s16                NET00759;
/* 0x0BC8*/    s16                NET00760;
/* 0x0BCA*/    s16                NET00761;
/* 0x0BCC*/    s16                NET00762;
/* 0x0BCE*/    s16                NET00763;
/* 0x0BD0*/    s16                NET00764;
/* 0x0BD2*/    s16                NET00765;
/* 0x0BD4*/    s16                NET00766;
/* 0x0BD6*/    s16                NET00771;
/* 0x0BD8*/    s16                NET00774;
/* 0x0BDA*/    s16                NET00775;
/* 0x0BDC*/    s16                NET00776;
/* 0x0BDE*/    s16                NET00777;
/* 0x0BE0*/    s16                NET00779;
/* 0x0BE2*/    s16                NET00780;
/* 0x0BE4*/    s16                NET00782;
/* 0x0BE6*/    s16                NET00784;
/* 0x0BE8*/    s16                NET00787;
/* 0x0BEA*/    s16                NET00789;
/* 0x0BEC*/    s16                NET00793;
/* 0x0BEE*/    s16                NET00794;
/* 0x0BF0*/    s16                NET00795;
/* 0x0BF2*/    s16                NET00796;
/* 0x0BF4*/    s16                NET00797;
/* 0x0BF6*/    s16                NET00798;
/* 0x0BF8*/    s16                NET00799;
/* 0x0BFA*/    s16                NET00800;
/* 0x0BFC*/    s16                NET00802;
/* 0x0BFE*/    s16                NET00806;
/* 0x0C00*/    s16                NET00807;
/* 0x0C02*/    s16                NET00810;
/* 0x0C04*/    s16                NET00811;
/* 0x0C06*/    s16                NET00812;
/* 0x0C08*/    s16                NET00819;
/* 0x0C0A*/    s16                NET00824;
/* 0x0C0C*/    s16                NET00835;
/* 0x0C0E*/    s16                NET00844;
/* 0x0C10*/    s16                NET00846;
/* 0x0C12*/    s16                NET00854;
/* 0x0C14*/    s16                NET00865;
/* 0x0C16*/    s16                NET00962;
/* 0x0C18*/    s16                NET00965;
/* 0x0C1A*/    s16                NET00981;
/* 0x0C1C*/    s16                NET01001;
/* 0x0C1E*/    s16                NET01017;
/* 0x0C20*/    s16                NET01027;
/* 0x0C22*/    s16                NET01034;
/* 0x0C24*/    s16                NET01045;
/* 0x0C26*/    s16                NET01049;
/* 0x0C28*/    s16                NET01051;
/* 0x0C2A*/    s16                NET01054;
/* 0x0C2C*/    s16                NET01055;
/* 0x0C2E*/    s16                NET01056;
/* 0x0C30*/    s16                NET01057;
/* 0x0C32*/    s16                NET01058;
/* 0x0C34*/    s16                NET01059;
/* 0x0C36*/    s16                NET01060;
/* 0x0C38*/    s16                NET01061;
/* 0x0C3A*/    s16                NET01062;
/* 0x0C3C*/    s16                NET01063;
/* 0x0C3E*/    s16                NET01064;
/* 0x0C40*/    s16                NET01065;
/* 0x0C42*/    s16                NET01066;
/* 0x0C44*/    s16                NET01067;
/* 0x0C46*/    s16                NET01070;
/* 0x0C48*/    s16                NET01071;
/* 0x0C4A*/    s16                NET01073;
/* 0x0C4C*/    s16                NET01074;
/* 0x0C4E*/    s16                NET01079;
/* 0x0C50*/    s16                NET01081;
/* 0x0C52*/    s16                NET01082;
/* 0x0C54*/    s16                NET01153;
/* 0x0C56*/    s16                NET01154;
/* 0x0C58*/    s16                NOM_MOT_FRQ;
/* 0x0C5A*/    s16                NOM_MOT_SPD;
/* 0x0C5C*/    s16                NOM_TORQ;
/* 0x0C5E*/    s16                NOT_READY_PUSK;
/* 0x0C60*/    s16                NUM_FAIL;
/* 0x0C62*/    s16                OVERLOAD_APV_EN;
/* 0x0C64*/    s16                PARALEL_IGBT_COUNT;
/* 0x0C66*/    s16                PARAM_CURR;
/* 0x0C68*/    s16                PARAM_POWER;
/* 0x0C6A*/    s16                PARAM_TORQ;
/* 0x0C6C*/    s16                PICK_UP_STATE;
/* 0x0C6E*/    s16                POWER_APVTIME_SEC;
/* 0x0C70*/    s16                POWER_APV_ENABLE;
/* 0x0C72*/    s16                POWER_KOLV_APV;
/* 0x0C74*/    s16                POWER_NOM;
/* 0x0C76*/    s16                POWER_UNIT;
/* 0x0C78*/    s16                PRIOR_TASK;
/* 0x0C7A*/    s16                PROFI_ENABLE;
/* 0x0C7C*/    s16                RECT_U_OPEN;
/* 0x0C7E*/    s16                REF_MODE;
/* 0x0C80*/    s16                RELAY_1;
/* 0x0C82*/    s16                RELAY_2;
/* 0x0C84*/    s16                RELAY_3;
/* 0x0C86*/    s16                RELAY_4;
/* 0x0C88*/    s16                RELAY_5;
/* 0x0C8A*/    s16                RELAY_6;
/* 0x0C8C*/    s16                RELAY_7;
/* 0x0C8E*/    s16                RELAY_8;
/* 0x0C90*/    s16                RELE1_SKIP_TIME;
/* 0x0C92*/    s16                RELE_4_LOGIC;
/* 0x0C94*/    s16                RELE_5_LOGIC;
/* 0x0C96*/    s16                RESET_WIFI_PASS;
/* 0x0C98*/    s16                RES_MOTOR_WORK_TIME;
/* 0x0C9A*/    s16                REVERSE_MODE;
/* 0x0C9C*/    s16                RS;
/* 0x0C9E*/    s16                SECOND;
/* 0x0CA0*/    s16                SEL_OS_PID;
/* 0x0CA2*/    s16                SEL_REF_PID;
/* 0x0CA4*/    s16                SENS_FAIL_AIN1;
/* 0x0CA6*/    s16                SENS_FAIL_AIN2;
/* 0x0CA8*/    s16                SENS_FAIL_AIN3;
/* 0x0CAA*/    s16                SENS_FAIL_AIN4;
/* 0x0CAC*/    s16                SPEED_DI;
/* 0x0CAE*/    s16                START_CUR_LIM;
/* 0x0CB0*/    s16                START_CUR_LIM_VAL;
/* 0x0CB2*/    s16                STATE_GT;
/* 0x0CB4*/    s16                STATUS_DRV;
/* 0x0CB6*/    s16                STATUS_WORD1;
/* 0x0CB8*/    s16                STEP_ED_IZM_WROT;
/* 0x0CBA*/    s16                S_TYPE_MODE;
/* 0x0CBC*/    s16                TACC_SKIP_FREQ;
/* 0x0CBE*/    s16                TASK_FAN_BRD;
/* 0x0CC0*/    s16                TASK_FAN_IGBT;
/* 0x0CC2*/    s16                TD;
/* 0x0CC4*/    s16                TDEC_SKIP_FREQ;
/* 0x0CC6*/    s16                TI;
/* 0x0CC8*/    s16                TIGBT_FAIL;
/* 0x0CCA*/    s16                TIME_FAIL;
/* 0x0CCC*/    s16                TIME_NORMAL_APV;
/* 0x0CCE*/    s16                TIME_SCALE;
/* 0x0CD0*/    s16                TIME_TO_STOP;
/* 0x0CD2*/    s16                TMAX_PROFINET;
/* 0x0CD4*/    s16                TMIN_PROFINET;
/* 0x0CD6*/    s16                TOFF_IGBT_DTC;
/* 0x0CD8*/    s16                TON_IGBT_DTC;
/* 0x0CDA*/    s16                TORQUE_BASE;
/* 0x0CDC*/    s16                TSTOP_AIN1;
/* 0x0CDE*/    s16                TSTOP_AIN2;
/* 0x0CE0*/    s16                TSTOP_AIN3;
/* 0x0CE2*/    s16                TSTOP_AIN4;
/* 0x0CE4*/    s16                TSTOP_FMAX;
/* 0x0CE6*/    s16                TSTOP_FMIN;
/* 0x0CE8*/    s16                TSTOP_NO_WATER;
/* 0x0CEA*/    s16                T_BLOCK;
/* 0x0CEC*/    s16                T_BOARD;
/* 0x0CEE*/    s16                T_FPUSK;
/* 0x0CF0*/    s16                T_IGBT;
/* 0x0CF2*/    s16                T_MAX;
/* 0x0CF4*/    s16                T_MIN;
/* 0x0CF6*/    s16                T_STOP_CURR_IMBALANCE;
/* 0x0CF8*/    s16                T_STOP_NEDOGR;
/* 0x0CFA*/    s16                T_STOP_PEREGR;
/* 0x0CFC*/    s16                UD_CHARGE;
/* 0x0CFE*/    s16                UD_CHARGE_EVAC;
/* 0x0D00*/    s16                UD_FAIL;
/* 0x0D02*/    s16                UD_MAX_CHARGE;
/* 0x0D04*/    s16                UD_MAX_UST;
/* 0x0D06*/    s16                UD_MIN_ABS;
/* 0x0D08*/    s16                UD_REGULATOR;
/* 0x0D0A*/    s16                UF1_TYPE;
/* 0x0D0C*/    s16                UF2_TYPE;
/* 0x0D0E*/    s16                UIN_BRAKE_FAULT;
/* 0x0D10*/    s16                UIN_MODE;
/* 0x0D12*/    s16                UNDRLOAD_APV_EN;
/* 0x0D14*/    s16                UST_CH_CONTR_2;
/* 0x0D16*/    s16                UST_CH_REF_2;
/* 0x0D18*/    s16                UST_DATA_MAX;
/* 0x0D1A*/    s16                UST_TYPE_DRV;
/* 0x0D1C*/    s16                UST_T_PEREGRUZ;
/* 0x0D1E*/    s16                VAL_AOUT3;
/* 0x0D20*/    s16                VAL_AOUT4;
/* 0x0D22*/    s16                VFD_SIMULATION_IMOT;
/* 0x0D24*/    s16                VFD_SIMULATION_MODE;
/* 0x0D26*/    s16                VFD_SIMULATION_UD;
/* 0x0D28*/    s16                VFD_SIMULATION_UIN;
/* 0x0D2A*/    s16                VOLTAGE_U_IGBT;
/* 0x0D2C*/    s16                VOLT_BASE;
/* 0x0D2E*/    s16                VYBEG_TIME;
/* 0x0D30*/    s16                WARN_MASK_0;
/* 0x0D32*/    s16                WARN_MASK_1;
/* 0x0D34*/    s16                WARN_MASK_2;
/* 0x0D36*/    s16                WIFI_MAX_CONNECTION;
/* 0x0D38*/    s16                YEAR;
/* 0x0D3A*/    s16                ZP;
/* 0x0D3C*/    s16                Fict_S16_1;
/* 0x0D3E*/    s16                Fict_S16_2;
/* 0x0D40*/    s16                Fict_S16_3;
 
// распределение kolvState
 
/* 0x0D44*/    s16                State_DA3[2];                //DEBUG_SYM
/* 0x0D48*/    s16                State_DA5[6];                //AIN_SCALE_V3
/* 0x0D54*/    s16                State_DA9[36];               //FREQ_FORBIDDEN_V2
/* 0x0D9C*/    s16                State_DA11[18];              //PICKUP_V2
/* 0x0DC0*/    s16                State_DA12[14];              //DRV_UD
/* 0x0DDC*/    s16                State_DA13[6];               //AIN_SCALE_V3
/* 0x0DE8*/    s16                State_DA16[2];               //LOAD_LOSS
/* 0x0DEC*/    s16                State_DA18[12];              //TEMP_CONTROL
/* 0x0E04*/    s16                State_DA19[32];              //NO_FLOW_DR_RUN2
/* 0x0E44*/    s16                State_DA20[8];               //TPM
/* 0x0E54*/    s16                State_DA21[4];               //CURRENT_IMBALANCE
/* 0x0E5C*/    s16                State_DA22[10];              //CURRENT_CONTROL
/* 0x0E70*/    s16                State_DA23[4];               //FREQLIMITCHECK
/* 0x0E78*/    s16                State_DA25[4];               //FREQLIMITCHECK
/* 0x0E80*/    s16                State_DA27[6];               //AIN_SCALE_V3
/* 0x0E8C*/    s16                State_DA28[6];               //AIN_SCALE_V3
/* 0x0E98*/    s16                State_DA54[4];               //FREQ_SCALE
/* 0x0EA0*/    s16                State_DA55[4];               //FREQ_SCALE
/* 0x0EA8*/    s16                State_DA56[4];               //FREQ_SCALE
/* 0x0EB0*/    s16                State_DA57[4];               //FREQ_SCALE
/* 0x0EB8*/    s16                State_DA66[8];               //POWER_LOSS_APV
/* 0x0EC8*/    s16                State_DA119[24];             //PID_IQ
/* 0x0EF8*/    s16                State_DA191[24];             //PID_IQ
/* 0x0F28*/    s16                State_DD3[4];                //PORT_UART_3
/* 0x0F30*/    s16                State_DD4[2];                //POWER_ON_PUSK
/* 0x0F34*/    s16                State_DD5[4];                //PORT_UART_3
/* 0x0F3C*/    s16                State_DD6[2];                //APV1
/* 0x0F40*/    s16                State_DD9[4];                //PORT_UART_3
/* 0x0F48*/    s16                State_DD13[12];              //LOCAL_APV_V3
/* 0x0F60*/    s16                State_DD14[12];              //LOCAL_APV_V3
/* 0x0F78*/    s16                State_DD15[12];              //LOCAL_APV_V3
/* 0x0F90*/    s16                State_DD16[12];              //LOCAL_APV_V3
/* 0x0FA8*/    s16                State_DD17[12];              //LOCAL_APV_V3
/* 0x0FC0*/    s16                State_DD18[12];              //LOCAL_APV_V3
/* 0x0FD8*/    s16                State_DD19[2];               //END_OF_CURVE
/* 0x0FDC*/    s16                State_DD20[2];               //REF_CONTROL
/* 0x0FE0*/    s16                State_DD21[12];              //LOCAL_APV_V3
/* 0x0FF8*/    s16                State_DD23[58];              //FAIL_COMMON_V2
/* 0x106C*/    s16                State_DD25[4];               //JRN_V2
/* 0x1074*/    s16                State_DD26[10];              //FACTORY_SET
/* 0x1088*/    s16                State_DD27[2];               //SEL_REF_V2
/* 0x108C*/    s16                State_DD28[2];               //LED_RUN
/* 0x1090*/    s16                State_DD31[6];               //LED_CONTROL
/* 0x109C*/    s16                State_DD32[4];               //BRAKE_CNTRL
/* 0x10A4*/    s16                State_DD36[12];              //LOCAL_APV_V3
/* 0x10BC*/    s16                State_DD37[4];               //LED_WIFI
/* 0x10C4*/    s16                State_DD40[8];               //DIN_UP_DOWN
/* 0x10D4*/    s16                State_DD43[22];              //RW_GROUP_PRM
/* 0x1100*/    s16                State_DD44[22];              //RW_GROUP_PRM
/* 0x112C*/    s16                State_DD45[4];               //UNDERLOAD
/* 0x1134*/    s16                State_DD53[82];              //DISKR_FUNC_V4
/* 0x11D8*/    s16                State_DD54[6];               //EVAK_V3
/* 0x11E4*/    s16                State_DD56[136];             //WARNINGS
/* 0x12F4*/    s16                State_DD57[12];              //LOCAL_APV_V3
/* 0x130C*/    s16                State_DD58[12];              //LOCAL_APV_V3
/* 0x1324*/    s16                State_DD59[122];             //TEMP_LSU_2
/* 0x1418*/    s16                State_DD60[4];               //PORT_UART_3
/* 0x1420*/    s16                State_DD61[4];               //PORT_UART_3
/* 0x1428*/    s16                State_DD63[18];              //ENCO_PHASING_BLOCK
/* 0x144C*/    s16                State_DD64[16];              //LIFT_CMD_V3
/* 0x146C*/    s16                State_DD65[2];               //PUSK_READY_2
/* 0x1470*/    s16                State_DD67[18];              //PUMP_DERAG
/* 0x1494*/    s16                State_DD109[4];              //PAROLES
/* 0x149C*/    s16                State_DD151[2];              //BOOTLOADER
/* 0x14A0*/    s16                State_DRV_V5[98];            //DRV_V5
/* 0x1564*/    s16                State_MB1[4];                //MB_0X05
/* 0x156C*/    s16                State_MB2[4];                //MB_0X05_VAL
/* 0x1574*/    s16                State_U1[12];                //TORQUE_MAX
/* 0x158C*/    s16                State_U2[316];               //PERIF_V2
/* 0x1804*/    s16                State_U3[20];                //RAMP_CTRL_MOD2
/* 0x182C*/    s16                State_U7[90];                //UF5_F2S
 } AllPrm_Type;
// 6368
#endif //AT_24_prog_RAM_h
