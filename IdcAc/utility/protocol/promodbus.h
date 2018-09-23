#ifndef PROMODBUS_H
#define PROMODBUS_H
#include "iprotocol.h"
#include "DataTypeStruct.h"

#define MAX_REG_NUM (100)
#define FUN_REG_READ (0x03)
#define FUN_REG_WRITE_ONE (0x06)
#define FUN_REG_WRITE_MORE (0x10)

#define REG_RANGE_M_1 (0x0010)
#define REG_RANGE_M_2 (0x0011)
#define REG_RELAY_M_LIGHT (0x000A)
#define REG_RELAY_M_SOUND (0x000B)

#define REG_RANGE_BEGIN (0x0012)
#define REG_RANGE_END (0x0065+1)

#define REG_BASIC_BEGIN (0x0000)
#define REG_BASIC_END (0x0067+1)

#define REG_LINE_MAIN_BEGIN (0x1000)
#define REG_LINE_MAIN_END (0x1059+1)

#define REG_LINE_SUB_PHASE_ONE_BEGIN (0x2000)
#define REG_LINE_SUB_PHASE_ONE_END (0x22F3+1)

#define REG_LINE_SUB_PHASE_THREE_BEGIN (0x3000)
#define REG_LINE_SUB_PHASE_THREE_END (0x30DF+1)

#define REG_HARM_BEGIN (0x4000)
#define REG_HARM_END (0x41A1+1)

#define REG_EVENT_BEGIN (0x7000)
#define REG_EVENT_END (0x7006+1)

#define REG_SWITCH_BEGIN (0)
#define REG_SWITCH_END (8+1)

#define REG_ZERO_ENERGY (0x006B)

class CProModbus :
        public IProtocol
{
public:
    CProModbus();
    virtual ~CProModbus();

    virtual string frame_format();
    virtual int frame_parse();

public:
    //交流
    //基本参数数据
    int get_basic(u_dev_measure &dev_measure, const MU_INT8 dev_addr);
    //主进线电参数数据
    int get_line_main(u_dev_measure &dev_measure, const MU_INT8 dev_addr);
    //输出支路电参数数据（单相）
    int get_line_sub_phase_one(u_dev_measure &dev_measure, const MU_INT8 dev_addr);
    //输出支路电参数数据（三相）
    int get_line_sub_phase_three(u_dev_measure &dev_measure, const MU_INT8 dev_addr);
    //谐波参数数据
    int get_harm(u_dev_measure &dev_measure, const MU_INT8 dev_addr);
    //事件记录
    int get_event(u_dev_measure &dev_measure, const MU_INT8 dev_addr);

    //开关量
    int get_switch(u_dev_measure &dev_measure, const MU_INT8 dev_addr);

    int set_range_one(const MU_INT16 val, const MU_INT16 reg_addr, const MU_INT8 dev_addr);
    int set_range_all(const VEC_INT16& vec_val, const MU_INT8 dev_addr, const MU_INT16 reg_addr);
    int set_relay(const MU_INT16 relay_val, const MU_INT8 dev_addr, const MU_INT16 reg_addr);
    int set_zero_energy(const MU_INT16 val, const MU_INT8 dev_addr);
private:
    string format_read();
    string format_read_basic();
    int parse_read_basic(u_dev_measure &dev_measure);
    string format_read_line_main();
    int parse_read_line_main(u_dev_measure &dev_measure);
    string format_read_line_sub_phase_one();
    int parse_read_line_sub_phase_one(u_dev_measure &dev_measure);
    string format_read_line_sub_phase_three();
    int parse_read_line_sub_phase_three(u_dev_measure &dev_measure);
    string format_read_harm();
    int parse_read_harm(u_dev_measure &dev_measure);
    string format_read_event();
    int parse_read_event(u_dev_measure &dev_measure);

    //读开关量,0-8reg
    string fomart_read_switch();
    int parse_read_switch(u_dev_measure &dev_measure);

    //写量程
    string format_write_range();
    int parse_write_range();

    //写进线继电器输出状态
    string format_write_relay();
    int parse_write_relay();

    //写电能清零
    string format_write_zero_energy();
    int parse_write_zero_energy();

public:
    void SetDevAddr(MU_INT8 dev_addr);
    void SetFunCode(MU_INT8 func_code);
    void SetRegAddr(MU_INT16 reg_addr);
    void SetRegNum(MU_INT16 reg_num);
    VEC_INT16& GetVecRegValWrite();
    void SetVecRegValWrite(const VEC_INT16& vec_reg_val);
    MU_INT32 GetFrameLen();
    string GetFrameAsStr();
    int SetFrameByStr(string& str);
    int AddFrameByDtuData(string& str);
    void SetRegBegin(MU_INT32 begin);
    void SetRegEnd(MU_INT32 end);
    void ClearRegValRead();
private:
    void ClearFrame();
private:
    MU_INT32 regBegin;
    MU_INT32 regEnd;
private:
    //发送，接收帧
    MU_INT8* transfFrame;//发送，接收帧
    MU_INT32 frameLenMax;//帧内存长度
    MU_INT32 frameLen;//帧实际长度

    MU_INT8 devAddr;//地址码
    MU_INT8 funCode;//功能码
    MU_INT16 regAddr;//寄存器地址
    MU_INT16 crcParity;//crc校验

    //返回
    MU_INT8 replyLen;//返回数据长度

    //读取
    MU_INT16 regNum;//寄存器数量

    //写入
    VEC_INT16 vecRegValWrite;//寄存器值
    //读出
    VEC_INT8 vecRegValRead;//寄存器值
};

#endif // PROMODBUS_H

/*modbus：
地址码：
电表地址
功能码：
读寄存器（功能码0x03）
单个寄存器写(功能码0x06)，多个寄存器写(功能码0x10)。
1个寄存器对应2个字节数据

异常响应：
地址码:0x01
功能码:0x80 + 请求功能码(0x03)
异常码:01/02/03
crc校验:0xFFFF 低字节在前

读取帧示例：
发送帧：
地址码:0x01
功能码:0x03
寄存器地址:0x0000 高字节在前
寄存器数量:0x0001 高字节在前
crc校验:0xFFFF 低字节在前
响应帧：
地址码:0x00
功能码:0x03
返回数据长度：0x02
返回数据:0x0001 高字节在前
crc校验:0x7984 低字节在前

写入帧示例：
单独配置波特率值
发送帧：
地址码:0x01
功能码:0x06
寄存器地址:0x0004 高字节在前
寄存器值:0x0005 高字节在前
crc校验:0x0808 低字节在前
响应帧：同发送帧一样。

同时配置波特率，数据位，校验位，停止位值
发送帧：
地址码:0x01
功能码:0x10
寄存器地址:0x0004 高字节在前
寄存器数量N:0x0002 高字节在前
数据长度：0x04
寄存器值1:0x0003 高字节在前
寄存器值2:0x0841
crc校验:0xC5AC 低字节在前
*/
