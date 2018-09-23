#pragma once
#include "DataTypeDef.h"

typedef struct
{
    unsigned short int    addr            ;   /* 显示屏地址               */
    unsigned short int    yymm            ;   /* 显示屏系统时间YYMM       */
    unsigned short int    ddhh            ;   /* 显示屏系统时间DDhh       */
    unsigned short int    mmss            ;   /* 显示屏系统时间mmss       */
}__attribute__((packed)) st_idc_head;

typedef struct
{
    unsigned int          Avolt           ;   /* 主路的A相电压            */
    unsigned int          Bvolt           ;   /* 主路的B相电压            */
    unsigned int          Cvolt           ;   /* 主路的C相电压            */
    unsigned int          ABvolt          ;   /* 主路的AB线电压           */
    unsigned int          BCvolt          ;   /* 主路的BC线电压           */
    unsigned int          CAvolt          ;   /* 主路的CA线电压           */
    unsigned int          Acurrent        ;   /* 主路的A相电流            */
    unsigned int          Bcurrent        ;   /* 主路的B相电流            */
    unsigned int          Ccurrent        ;   /* 主路的C相电流            */
    unsigned int          Ncurrent        ;   /* 主路的N相电流            */
    unsigned int          Tcombine1       ;   /* 主路总有功总电能         */
    unsigned int          Tcombine2       ;   /* 主路总无功总电能         */
    unsigned int          Acombine1       ;   /* 主路的A相有功电能        */
    unsigned int          Bcombine1       ;   /* 主路的B相有功电能        */
    unsigned int          Ccombine1       ;   /* 主路的C相有功电能        */
    unsigned int          Acombine2       ;   /* 主路的A相无功电能        */
    unsigned int          Bcombine2       ;   /* 主路的B相无功电能        */
    unsigned int          Ccombine2       ;   /* 主路的C相无功电能        */
    signed int            Tactive1        ;   /* 主路总有功功率           */
    signed int            Tactive2        ;   /* 主路总无功功率           */
    signed int            Tactive3        ;   /* 主路总视在功率           */
    signed short int      factor          ;   /* 主路总功率因数           */
    signed int            Aactive1        ;   /* 主路的A相有功功率        */
    signed int            Bactive1        ;   /* 主路的B相有功功率        */
    signed int            Cactive1        ;   /* 主路的C相有功功率        */
    signed int            Aactive2        ;   /* 主路的A相无功功率        */
    signed int            Bactive2        ;   /* 主路的B相无功功率        */
    signed int            Cactive2        ;   /* 主路的C相无功功率        */
    signed int            Aactive3        ;   /* 主路的A相视在功率        */
    signed int            Bactive3        ;   /* 主路的B相视在功率        */
    signed int            Cactive3        ;   /* 主路的C相视在功率        */
    signed short int      Afactor         ;   /* 主路的A相功率因数        */
    signed short int      Bfactor         ;   /* 主路的B相功率因数        */
    signed short int      Cfactor         ;   /* 主路的C相功率因数        */
    unsigned short int    frequency       ;   /* 主路的频率               */
    unsigned short int    Switch          ;   /* 主路的开关量输入状态     */
    unsigned short int    Relays          ;   /* 主路的继电器输出状态     */
    unsigned short int    Avoltharm       ;   /* 主路的A相电压谐波百分比  */
    unsigned short int    Bvoltharm       ;   /* 主路的B相电压谐波百分比  */
    unsigned short int    Cvoltharm       ;   /* 主路的C相电压谐波百分比  */
    unsigned short int    Acurrentharm    ;   /* 主路的A相电流谐波百分比  */
    unsigned short int    Bcurrentharm    ;   /* 主路的B相电流谐波百分比  */
    unsigned short int    Ccurrentharm    ;   /* 主路的C相电流谐波百分比  */
}__attribute__((packed)) st_ac_ml;

typedef struct
{
    unsigned int          current          ;   /* 主路电流                 */
    unsigned int          combine          ;   /* 主路电能                 */
    unsigned int          volt             ;   /* 主路电压                 */
    signed int            active           ;   /* 主路功率                 */
    unsigned short int    Switch           ;   /* 主路开关量状态           */
    unsigned short int    Relays           ;   /* 主路的继电器输出状态     */
    signed short int      pir              ;   /* 主路母线正对地绝缘电阻值 */
    signed short int      nir              ;   /* 主路母线负对地绝缘电阻值 */
}__attribute__((packed)) st_dc_ml;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct
{
    st_idc_head         head;
    st_ac_ml            ml_a;                             // B.3.2.1   // B.3.2.21
    st_ac_ml            ml_b;
    unsigned int        sl_a_current[REPORT_SUB_NUM];    // B.3.2.13       /* 支路电流                */
    signed int          sl_a_active1[REPORT_SUB_NUM];    // B.3.2.14       /* 支路有功功率            */
    signed int          sl_a_active2[REPORT_SUB_NUM];    // B.3.2.15       /* 支路无功功率            */
    signed int          sl_a_active3[REPORT_SUB_NUM];    // B.3.2.16       /* 支路视在功率            */
    signed short int    sl_a_factor[REPORT_SUB_NUM];     // B.3.2.17       /* 支路功率因数            */
    unsigned int        sl_a_combine1[REPORT_SUB_NUM];   // B.3.2.18       /* 支路有功电能            */
    unsigned int        sl_a_combine2[REPORT_SUB_NUM];   // B.3.2.19       /* 支路无功电能            */
    unsigned int        sl_b_current[REPORT_SUB_NUM];
    signed int          sl_b_active1[REPORT_SUB_NUM];
    signed int          sl_b_active2[REPORT_SUB_NUM];
    signed int          sl_b_active3[REPORT_SUB_NUM];
    signed short int    sl_b_factor[REPORT_SUB_NUM];
    unsigned int        sl_b_combine1[REPORT_SUB_NUM];
    unsigned int        sl_b_combine2[REPORT_SUB_NUM];
    unsigned short int  sl_a_switch[REPORT_SUB_NUM];     // B.3.2.3        /* 支路开关量输入状态      */
    unsigned short int  sl_b_switch[REPORT_SUB_NUM];
}__attribute__((packed)) st_ac_reg;

typedef struct
{
    st_idc_head         head;
    st_dc_ml            ml_a;                             // B.3.2.1   // B.3.2.21
    st_dc_ml            ml_b;
    unsigned short int  sl_a_current[REPORT_SUB_NUM];    // B.3.2.13       /* 支路电流                 */
    unsigned int        sl_a_combine[REPORT_SUB_NUM];    // B.3.2.18       /* 支路电能                */
    signed short int    sl_a_active[REPORT_SUB_NUM];     // B.3.2.14       /* 支路功率                */
    unsigned short int  sl_b_current[REPORT_SUB_NUM];
    unsigned int        sl_b_combine[REPORT_SUB_NUM];
    signed short int    sl_b_active[REPORT_SUB_NUM];
    signed short int    sl_a_ir[REPORT_SUB_NUM];         // B.3.2.20       /* 支路绝缘电阻值         */
    signed short int    sl_b_ir[REPORT_SUB_NUM];
    unsigned short int  sl_a_switch[REPORT_SUB_NUM];     // B.3.2.3        /* 支路开关量状态         */
    unsigned short int  sl_b_switch[REPORT_SUB_NUM];
}__attribute__((packed)) st_dc_reg;

