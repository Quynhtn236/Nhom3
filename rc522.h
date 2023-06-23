#ifndef __RC522_CONFIG_H
#define __RC522_CONFIG_H

/////////////////////////////////////////////////////////////////////
// Lệnh cho MF522
/////////////////////////////////////////////////////////////////////
#define PCD_IDLE              0x00               // Hủy lệnh hiện tại
#define PCD_AUTHENT           0x0E               // Xác thực khóa
#define PCD_RECEIVE           0x08               // Nhận dữ liệu
#define PCD_TRANSMIT          0x04               // Gửi dữ liệu
#define PCD_TRANSCEIVE        0x0C               // Gửi và nhận dữ liệu
#define PCD_RESETPHASE        0x0F               // Đặt lại
#define PCD_CALCCRC           0x03               // Tính toán CRC

/////////////////////////////////////////////////////////////////////
// Lệnh cho thẻ Mifare_One
/////////////////////////////////////////////////////////////////////
#define PICC_REQIDL           0x26               // Tìm trong vùng không vào trạng thái ngủ
#define PICC_REQALL           0x52               // Tìm trong vùng tất cả các thẻ
#define PICC_ANTICOLL1        0x93               // Chống va chạm
#define PICC_ANTICOLL2        0x95               // Chống va chạm
#define PICC_AUTHENT1A        0x60               // Xác thực khóa A
#define PICC_AUTHENT1B        0x61               // Xác thực khóa B
#define PICC_READ             0x30               // Đọc khối
#define PICC_WRITE            0xA0               // Ghi khối
#define PICC_DECREMENT        0xC0               // Giảm giá trị
#define PICC_INCREMENT        0xC1               // Tăng giá trị
#define PICC_RESTORE          0xC2               // Khôi phục dữ liệu khối vào bộ đệm
#define PICC_TRANSFER         0xB0               // Lưu dữ liệu từ bộ đệm
#define PICC_HALT             0x50               // Chế độ ngủ

/////////////////////////////////////////////////////////////////////
// Định nghĩa độ dài FIFO cho MF522
/////////////////////////////////////////////////////////////////////
#define DEF_FIFO_LENGTH       64                 // Kích thước FIFO = 64 byte
#define MAXRLEN  18

/////////////////////////////////////////////////////////////////////
// Định nghĩa thanh ghi MF522
/////////////////////////////////////////////////////////////////////
// TRANG 0
#define     RFU00                 0x00
#define     CommandReg            0x01
#define     ComIEnReg             0x02
#define     DivlEnReg             0x03
#define     ComIrqReg             0x04
#define     DivIrqReg             0x05
#define     ErrorReg              0x06
#define     Status1Reg            0x07
#define     Status2Reg            0x08
#define     FIFODataReg           0x09
#define     FIFOLevelReg          0x0A
#define     WaterLevelReg         0x0B
#define     ControlReg            0x0C
#define     BitFramingReg         0x0D
#define     CollReg               0x0E
#define     RFU0F                 0x0F
// TRANG 1
#define     RFU10                 0x10
#define     ModeReg               0x11
#define     TxModeReg             0x12
#define     RxModeReg             0x13
#define     TxControlReg          0x14
#define     TxAutoReg             0x15
#define     TxSelReg              0x16
#define     RxSelReg              0x17
#define     RxThresholdReg        0x18
#define     DemodReg              0x19
#define     RFU1A                 0x1A
#define     RFU1B                 0x1B
#define     MifareReg             0x1C
#define     RFU1D                 0x1D
#define     RFU1E                 0x1E
#define     SerialSpeedReg        0x1F
// TRANG 2
#define     RFU20                 0x20
#define     CRCResultRegM         0x21
#define     CRCResultRegL         0x22
#define     RFU23                 0x23
#define     ModWidthReg           0x24
#define     RFU25                 0x25
#define     RFCfgReg              0x26
#define     GsNReg                0x27
#define     CWGsCfgReg            0x28
#define     ModGsCfgReg           0x29
#define     TModeReg              0x2A
#define     TPrescalerReg         0x2B
#define     TReloadRegH           0x2C
#define     TReloadRegL           0x2D
#define     TCounterValueRegH     0x2E
#define     TCounterValueRegL     0x2F
// TRANG 3
#define     RFU30                 0x30
#define     TestSel1Reg           0x31
#define     TestSel2Reg           0x32
#define     TestPinEnReg          0x33
#define     TestPinValueReg       0x34
#define     TestBusReg            0x35
#define     AutoTestReg           0x36
#define     VersionReg            0x37
#define     AnalogTestReg         0x38
#define     TestDAC1Reg           0x39
#define     TestDAC2Reg           0x3A
#define     TestADCReg            0x3B
#define     RFU3C                 0x3C
#define     RFU3D                 0x3D
#define     RFU3E                 0x3E
#define     RFU3F		  		  0x3F

/////////////////////////////////////////////////////////////////////
// Các mã lỗi trả về khi giao tiếp với MF522
/////////////////////////////////////////////////////////////////////
#define 	MI_OK                 0x26
#define 	MI_NOTAGERR           0xcc
#define 	MI_ERR                0xbb

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

void RC522_setup(uint8_t rst);

void PcdReset(void);
void M500PcdConfigISOType(uint8_t type);
char PcdRequest(uint8_t req_code, uint8_t* pTagType);
char PcdAnticoll(uint8_t* pSnr);

#endif

