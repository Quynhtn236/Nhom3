
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rc522.h"

uint8_t rst_pin;

char* scanCard(){
    char* CID = (char*) malloc (8 * sizeof(char));
    uint8_t ucArray_ID[4];    /* LÆ°u trá»¯ loáº¡i IC vÃ  UID (sá»‘ hiá»‡u thá»© tá»± cá»§a IC) cá»§a tháº» IC */
    uint8_t ucStatusReturn;      /* Tráº¡ng thÃ¡i tráº£ vá» */
    if ((ucStatusReturn = PcdRequest(PICC_REQALL, ucArray_ID)) != MI_OK) { /* Náº¿u tháº¥t báº¡i, thá»­ tÃ¬m tháº» láº§n ná»¯a */

        return NULL;
    }

    if (ucStatusReturn == MI_OK) {

        if (PcdAnticoll(ucArray_ID) == MI_OK) {
            sprintf(CID, "%02X%02X%02X%02X",
                ucArray_ID[0],
                ucArray_ID[1],
                ucArray_ID[2],
                ucArray_ID[3]);
            printf("ID la : %s\r\n", CID);
           }
    }


    return CID;
}

void RC522_setup(uint8_t rst)
{
    rst_pin = rst;
    wiringPiSetup();
    pinMode(rst_pin, OUTPUT);
    digitalWrite(rst_pin, 1);
    wiringPiSPISetup(0, 100000);
}

void RC522_RST(uint8_t v)
{
    digitalWrite(rst_pin, v);
}

/**
  * @brief  Đọc thanh ghi RC522
  * @param  ucAddress: Địa chỉ thanh ghi
  * @retval Giá trị hiện tại của thanh ghi
  */
uint8_t ReadRawRC ( uint8_t ucAddress )
{
    static uint8_t _buf1[0x2];
    uint8_t ucAddr, ucReturn;

    ucAddr = ( ( ucAddress << 1 ) & 0x7E ) | 0x80;
    _buf1[0] = ucAddr;
    wiringPiSPIDataRW(0, _buf1, 2);
    ucReturn = _buf1[1];

    return ucReturn;
}

/**
  * @brief  Ghi vào thanh ghi RC522
  * @param  ucAddress: Địa chỉ thanh ghi
  * @param  ucValue: Giá trị ghi vào thanh ghi
  * @retval None
  */
void WriteRawRC ( uint8_t ucAddress, uint8_t ucValue )
{
    static uint8_t _buf1[0x2];
    uint8_t ucAddr;

    ucAddr = ( ucAddress << 1 ) & 0x7E;
    _buf1[0] = ucAddr;
    _buf1[1] = ucValue;
    wiringPiSPIDataRW(0, _buf1, 2);
}

/**
  * @brief  Đặt bit mặt nạ trong thanh ghi RC522
  * @param  ucReg: Địa chỉ thanh ghi
  * @param   ucMask: Giá trị mặt nạ
  * @retval None
  */
void SetBitMask ( uint8_t ucReg, uint8_t ucMask )
{
    uint8_t ucTemp;

    ucTemp = ReadRawRC ( ucReg );
    WriteRawRC ( ucReg, ucTemp | ucMask ); // Đặt bit mặt nạ
}

/**
  * @brief  Xóa bit mặt nạ trong thanh ghi RC522
  * @param  ucReg: Địa chỉ thanh ghi
  * @param  ucMask: Giá trị mặt nạ
  * @retval None
  */
void ClearBitMask ( uint8_t ucReg, uint8_t ucMask )
{
    uint8_t ucTemp;

    ucTemp = ReadRawRC ( ucReg );
    WriteRawRC ( ucReg, ucTemp & ( ~ ucMask) ); // Xóa bit mặt nạ
}

/**
  * @brief  Bật anten
  * @param  None
  * @retval None
  */
void PcdAntennaOn ( void )
{
    uint8_t uc;

    uc = ReadRawRC ( TxControlReg );
    if ( ! ( uc & 0x03 ) ) {
        SetBitMask(TxControlReg, 0x03);
    }
}


/**
  * @brief  Tắt anten
  * @param  Không
  * @retval Không
  */
void PcdAntennaOff(void) {
    ClearBitMask(TxControlReg, 0x03);
}

/**
  * @brief  Đặt lại RC522
  * @param  Không
  * @retval Không
  */
void PcdReset(void) {
    RC522_RST(1);
    delayMicroseconds(1);
    RC522_RST(0);
    delayMicroseconds(1);
    RC522_RST(1);
    delayMicroseconds(1);
    WriteRawRC(CommandReg, 0x0F);
    while (ReadRawRC(CommandReg) & 0x10);
    delayMicroseconds(1);
    // Định nghĩa chế độ gửi và nhận thông thường và giao tiếp với thẻ Mifare, giá trị khởi tạo CRC là 0x6363
    WriteRawRC(ModeReg, 0x3D);
    WriteRawRC(TReloadRegL, 30);      // 16 bit timer thấp
    WriteRawRC(TReloadRegH, 0);       // 16 bit timer cao
    WriteRawRC(TModeReg, 0x8D);       // Định nghĩa cài đặt bộ định thời nội bộ
    WriteRawRC(TPrescalerReg, 0x3E);  // Đặt hệ số chia bộ định thời
    WriteRawRC(TxAutoReg, 0x40);      // Điều chế tín hiệu gửi là 100% ASK
}

/**
  * @brief  Thiết lập chế độ làm việc của RC522
  * @param  ucType: Chế độ làm việc
  * @retval Không
  */
void M500PcdConfigISOType(uint8_t ucType) {
    if (ucType == 'A') {               // ISO14443_A
        ClearBitMask(Status2Reg, 0x08);
        WriteRawRC(ModeReg, 0x3D);     // 3F
        WriteRawRC(RxSelReg, 0x86);    // 84
        WriteRawRC(RFCfgReg, 0x7F);    // 4F
        WriteRawRC(TReloadRegL, 30);
        WriteRawRC(TReloadRegH, 0);
        WriteRawRC(TModeReg, 0x8D);
        WriteRawRC(TPrescalerReg, 0x3E);
        delayMicroseconds(2);
        PcdAntennaOn();                // Bật anten
    }
}

/**
  * @brief  Giao tiếp giữa RC522 và thẻ ISO14443
  * @param  ucCommand: Lệnh RC522
  * @param  pInData: Dữ liệu gửi đến thẻ qua RC522
  * @param  ucInLenByte: Độ dài dữ liệu gửi (tính bằng byte)
  * @param  pOutData: Dữ liệu trả về từ thẻ
  * @param  pOutLenBit: Độ dài dữ liệu trả về (tính bằng bit)
  * @retval Trạng thái = MI_OK nếu thành công
  */
char PcdComMF522(uint8_t ucCommand,
                 uint8_t* pInData,
                 uint8_t ucInLenByte,
                 uint8_t* pOutData,
                 uint32_t* pOutLenBit) {
    char cStatus = MI_ERR;
    uint8_t ucIrqEn = 0x00;
    uint8_t ucWaitFor = 0x00;
    uint8_t ucLastBits;
    uint8_t ucN;
    uint32_t ul;

    switch (ucCommand) {
        case PCD_AUTHENT:         // Xác thực Mifare
            ucIrqEn = 0x12;       // Cho phép yêu cầu ngắt lỗi ErrIEn, cho phép ngắt trạng thái trống IdleIEn
            ucWaitFor = 0x10;     // Đợi xác nhận thẻ trong quá trình xác thực, kiểm tra cờ trạng thái trống IdleIEn
            break;

        case PCD_TRANSCEIVE:      // Gửi/Nhận dữ liệu
            ucIrqEn = 0x77;       // Cho phép ngắt TxIEn RxIEn IdleIEn LoAlertIEn ErrIEn TimerIEn
            ucWaitFor = 0x30;     // Đợi xác nhận thẻ trong quá trình tìm kiếm, kiểm tra cờ ngắt trạng thái trống và cờ ngắt nhận
            break;

        default:
            break;
    }
    // Đảo IRqInv để cấu hình chân IRQ và giá trị IRq của thanh ghi Status1Reg
    WriteRawRC(ComIEnReg, ucIrqEn | 0x80);
    // Khi đặt bit Set1, các cờ mờ ComIrqReg sẽ được xóa
    ClearBitMask(ComIrqReg, 0x80);
    // Gửi lệnh rảnh
    WriteRawRC(CommandReg, PCD_IDLE);
    // Đặt bit FlushBuffer để xóa con trỏ đọc và ghi FIFO nội bộ cùng với cờ BufferOvfl của thanh ghi ErrReg được xóa
    SetBitMask(FIFOLevelReg, 0x80);
    for (ul = 0; ul < ucInLenByte; ul++) {
        WriteRawRC(FIFODataReg, pInData[ul]);    // Ghi dữ liệu vào FIFOData
    }
    WriteRawRC(CommandReg, ucCommand);   // Ghi lệnh
    if (ucCommand == PCD_TRANSCEIVE) {    // Đặt bit StartSend để bắt đầu gửi dữ liệu, bit này chỉ có tác dụng khi sử dụng lệnh gửi/nhận
        SetBitMask(BitFramingReg, 0x80);
    }

    ul = 1000;  // Thời gian chờ tối đa để thực hiện thẻ M1, điều chỉnh dựa trên tần số clock

    // Xác thực và thời gian chờ tìm kiếm thẻ
    do {
        ucN = ReadRawRC(ComIrqReg);  // Kiểm tra ngắt sự kiện
        ul--;
    } while ((ul != 0) && (!(ucN & 0x01)) && (!(ucN & ucWaitFor)));
    ClearBitMask(BitFramingReg, 0x80);  // Xóa bit cho phép StartSend

    if (ul != 0) {
        // Đọc thanh ghi lỗi ErrorReg gồm các cờ lỗi BufferOfI, CollErr, ParityErr, ProtocolErr
        if (!(ReadRawRC(ErrorReg) & 0x1B)) {
            cStatus = MI_OK;
            if (ucN & ucIrqEn & 0x01) {  // Kiểm tra xem có ngắt bộ đếm thời gian hay không
                cStatus = MI_NOTAGERR;
            }
            if (ucCommand == PCD_TRANSCEIVE) {
                ucN = ReadRawRC(FIFOLevelReg);  // Đọc số byte được lưu trong FIFO
                ucLastBits = ReadRawRC(ControlReg) & 0x07;  // Số bit hợp lệ cuối cùng nhận được
                if (ucLastBits) {
                    *pOutLenBit = (ucN - 1) * 8 + ucLastBits;  // Số lượng bit dữ liệu đã nhận được = (số byte - 1) * 8 + số bit hợp lệ cuối cùng
                } else {
                    *pOutLenBit = ucN * 8;  // Số lượng bit dữ liệu đã nhận được = số byte * 8
                }
                if (ucN == 0) {
                    ucN = 1;
                }
                if (ucN > MAXRLEN) {
                    ucN = MAXRLEN;
                }
                for (ul = 0; ul < ucN; ul++) {
                    pOutData[ul] = ReadRawRC(FIFODataReg);
                }
            }
        } else {
            cStatus = MI_ERR;
        }
    }
    SetBitMask(ControlReg, 0x80);  // Dừng bộ đếm thời gian
    WriteRawRC(CommandReg, PCD_IDLE);
    return cStatus;
}


/**
  * @brief Tìm kiếm thẻ
  * @param  ucReq_code, Chế độ tìm kiếm thẻ = 0x52, Tìm kiếm tất cả các thẻ tuân theo tiêu chuẩn 14443A trong khu vực cảm biến;
            Chế độ tìm kiếm = 0x26, Tìm kiếm các thẻ chưa vào trạng thái ngủ
  * @param  pTagType, Mã loại thẻ
             = 0x4400, Mifare_UltraLight
             = 0x0400, Mifare_One(S50)
             = 0x0200, Mifare_One(S70)
             = 0x0800, Mifare_Pro(X))
             = 0x4403, Mifare_DESFire
  * @retval Giá trị trạng thái = MI_OK, thành công
  */
char PcdRequest(uint8_t ucReq_code, uint8_t* pTagType)
{
    char cStatus;
    uint8_t ucComMF522Buf[MAXRLEN];
    uint32_t ulLen;

    // Xóa chỉ thị MIFARECyptol và dữ liệu giao tiếp của tất cả các thẻ được mã hóa
    ClearBitMask(Status2Reg, 0x08);
    // Đặt bit thứ 7 của byte cuối cùng được gửi
    WriteRawRC(BitFramingReg, 0x07);
    // Đưa tín hiệu năng lượng modulat 13.56 MHz đã qua xử lý đến các chân TX1 và TX2
    SetBitMask(TxControlReg, 0x03);
    ucComMF522Buf[0] = ucReq_code; // Lưu mã lệnh thẻ
    cStatus = PcdComMF522(PCD_TRANSCEIVE,
                          ucComMF522Buf,
                          1,
                          ucComMF522Buf,
                          &ulLen); // Tìm kiếm thẻ
    if ((cStatus == MI_OK) && (ulLen == 0x10)) { // Tìm kiếm thẻ thành công và trả về loại thẻ
        *pTagType = ucComMF522Buf[0];
        *(pTagType + 1) = ucComMF522Buf[1];
    }
    else {
        cStatus = MI_ERR;
    }
    return cStatus;
}

/**
  * @brief  Phòng tránh va chạm
  * @param  pSnr, Số thứ tự thẻ, 4 byte
  * @retval Giá trị trạng thái = MI_OK, thành công
  */
char PcdAnticoll(uint8_t* pSnr)
{
    char cStatus;
    uint8_t uc, ucSnr_check = 0;
    uint8_t ucComMF522Buf[MAXRLEN];
    uint32_t ulLen;

    // Xóa bit MFCryptol On, chỉ khi thành công MFAuthent thì bit này mới được đặt
    ClearBitMask(Status2Reg, 0x08);
    // Xóa thanh ghi dừng truyền/nhận
    WriteRawRC(BitFramingReg, 0x00);
    // Xóa tất cả các bit nhận sau va chạm
    ClearBitMask(CollReg, 0x80);
    ucComMF522Buf[0] = 0x93; // Lệnh phòng tránh va chạm thẻ
    ucComMF522Buf[1] = 0x20;
    cStatus = PcdComMF522(PCD_TRANSCEIVE,
                          ucComMF522Buf,
                          2,
                          ucComMF522Buf,
                          &ulLen); // Giao tiếp với thẻ
    if (cStatus == MI_OK) { // Giao tiếp thành công
        for (uc = 0; uc < 4; uc++) {
            *(pSnr + uc) = ucComMF522Buf[uc]; // Đọc UID
            ucSnr_check ^= ucComMF522Buf[uc];
        }
        if (ucSnr_check != ucComMF522Buf[uc]) {
            cStatus = MI_ERR;
        }
    }
    SetBitMask(CollReg, 0x80);
    return cStatus;
}

/**
  * @brief  Tính toán CRC16 bằng RC522
  * @param  pIndata, Mảng dữ liệu tính CRC16
  * @param  ucLen, Độ dài mảng dữ liệu tính CRC16 (theo byte)
  * @param  pOutData, Địa chỉ bộ nhớ để lưu kết quả CRC16
  * @retval Không có
  */
void CalulateCRC(uint8_t* pIndata,
                 uint8_t ucLen,
                 uint8_t* pOutData)
{
    uint8_t uc, ucN;
    ClearBitMask(DivIrqReg, 0x04);
    WriteRawRC(CommandReg, PCD_IDLE);
    SetBitMask(FIFOLevelReg, 0x80);
    for (uc = 0; uc < ucLen; uc++) {
        WriteRawRC(FIFODataReg, *(pIndata + uc));
    }
    WriteRawRC(CommandReg, PCD_CALCCRC);
    uc = 0xFF;
    do {
        ucN = ReadRawRC(DivIrqReg);
        uc--;
    } while ((uc != 0) && !(ucN & 0x04));
    pOutData[0] = ReadRawRC(CRCResultRegL);
    pOutData[1] = ReadRawRC(CRCResultRegM);
}

/**
  * @brief  Chọn thẻ
  * @param  pSnr: Số thứ tự thẻ, 4 byte
  * @retval Giá trị trạng thái = MI_OK, thành công
  */
char PcdSelect(uint8_t* pSnr)
{
    char ucN;
    uint8_t uc;
    uint8_t ucComMF522Buf[MAXRLEN];
    uint32_t ulLen;
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (uc = 0; uc < 4; uc++)
    {
        ucComMF522Buf[uc + 2] = *(pSnr + uc);
        ucComMF522Buf[6] ^= *(pSnr + uc);
    }
    CalulateCRC(ucComMF522Buf, 7, &ucComMF522Buf[7]);
    ClearBitMask(Status2Reg, 0x08);
    ucN = PcdComMF522(PCD_TRANSCEIVE,
                      ucComMF522Buf,
                      9,
                      ucComMF522Buf,
                      &ulLen);
    if ((ucN == MI_OK) && (ulLen == 0x18))
    {
        ucN = MI_OK;
    }
    else
    {
        ucN = MI_ERR;
    }
    return ucN;
}

/**
  * @brief  Xác thực mật khẩu thẻ
  * @param  ucAuth_mode: Chế độ xác thực mật khẩu = 0x60, xác thực khóa A,
            Chế độ xác thực mật khẩu = 0x61, xác thực khóa B
  * @param  ucAddr: Địa chỉ khối
  * @param  pKey: Mật khẩu
  * @param  pSnr: Số thứ tự thẻ, 4 byte
  * @retval Giá trị trạng thái = MI_OK, thành công
  */
char PcdAuthState(uint8_t ucAuth_mode,
                  uint8_t ucAddr,
                  uint8_t* pKey,
                  uint8_t* pSnr)
{
    char cStatus;
    uint8_t uc, ucComMF522Buf[MAXRLEN];
    uint32_t ulLen;

    ucComMF522Buf[0] = ucAuth_mode;
    ucComMF522Buf[1] = ucAddr;
    for (uc = 0; uc < 6; uc++)
    {
        ucComMF522Buf[uc + 2] = *(pKey + uc);
    }
    for (uc = 0; uc < 6; uc++)
    {
        ucComMF522Buf[uc + 8] = *(pSnr + uc);
    }
    cStatus = PcdComMF522(PCD_AUTHENT,
                          ucComMF522Buf,
                          12,
                          ucComMF522Buf,
                          &ulLen);
    if ((cStatus != MI_OK) || (!(ReadRawRC(Status2Reg) & 0x08)))
    {
        cStatus = MI_ERR;
    }
    return cStatus;
}

/**
  * @brief  Ghi dữ liệu vào khối thẻ M1
  * @param  ucAddr: Địa chỉ khối
  * @param  pData: Dữ liệu ghi vào, 16 byte
  * @retval Giá trị trạng thái = MI_OK, thành công
  */
char PcdWrite(uint8_t ucAddr, uint8_t* pData)
{
    char cStatus;
    uint8_t uc, ucComMF522Buf[MAXRLEN];
    uint32_t ulLen;

    ucComMF522Buf[0] = PICC_WRITE;
    ucComMF522Buf[1] = ucAddr;
    CalulateCRC(ucComMF522Buf, 2, &ucComMF522Buf[2]);
    cStatus = PcdComMF522(PCD_TRANSCEIVE,
                          ucComMF522Buf,
                          4,
                          ucComMF522Buf,
                          &ulLen);
    if ((cStatus != MI_OK) || (ulLen != 4) ||
        ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {
        cStatus = MI_ERR;
    }
    if (cStatus == MI_OK)
    {
        //memcpy(ucComMF522Buf, pData, 16);
        for (uc = 0; uc < 16; uc++)
        {
            ucComMF522Buf[uc] = *(pData + uc);
        }
        CalulateCRC(ucComMF522Buf, 16, &ucComMF522Buf[16]);
        cStatus = PcdComMF522(PCD_TRANSCEIVE,
                              ucComMF522Buf,
                              18,
                              ucComMF522Buf,
                              &ulLen);
        if ((cStatus != MI_OK) || (ulLen != 4) ||
            ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {
            cStatus = MI_ERR;
        }
    }
    return cStatus;
}

/**
  * @brief  Đọc dữ liệu khối thẻ M1
  * @param  ucAddr: Địa chỉ khối
  * @param  pData: Dữ liệu đọc ra, 16 byte
  * @retval Giá trị trạng thái = MI_OK, thành công
  */
char PcdRead(uint8_t ucAddr, uint8_t* pData)
{
    char cStatus;
    uint8_t uc, ucComMF522Buf[MAXRLEN];
    uint32_t ulLen;

    ucComMF522Buf[0] = PICC_READ;
    ucComMF522Buf[1] = ucAddr;
    CalulateCRC(ucComMF522Buf, 2, &ucComMF522Buf[2]);
    cStatus = PcdComMF522(PCD_TRANSCEIVE,
                          ucComMF522Buf,
                          4,
                          ucComMF522Buf,
                          &ulLen);
    if ((cStatus == MI_OK) && (ulLen == 0x90))
    {
        for (uc = 0; uc < 16; uc++)
        {
            *(pData + uc) = ucComMF522Buf[uc];
        }
    }
    else
    {
        cStatus = MI_ERR;
    }

    return cStatus;
}

/**
  * @brief  Đặt thẻ vào trạng thái ngủ
  * @param  Không có
  * @retval Giá trị trạng thái = MI_OK, thành công
  */
char PcdHalt(void)
{
    uint8_t ucComMF522Buf[MAXRLEN];
    uint32_t ulLen;

    ucComMF522Buf[0] = PICC_HALT;
    ucComMF522Buf[1] = 0;
    CalulateCRC(ucComMF522Buf, 2, &ucComMF522Buf[2]);
    PcdComMF522(PCD_TRANSCEIVE,
                ucComMF522Buf,
                4,
                ucComMF522Buf,
                &ulLen);
    return MI_OK;
}

void IC_CMT(uint8_t* UID,
            uint8_t* KEY,
            uint8_t RW,
            uint8_t* Dat)
{
    uint8_t ucArray_ID[4] = {0}; // Lưu trước loại thẻ IC và UID (số serial IC)
    PcdRequest(0x52, ucArray_ID); // Tìm thẻ
    PcdAnticoll(ucArray_ID);      // Ngăn chặn va chạm
    PcdSelect(UID);               // Chọn thẻ
    PcdAuthState(0x60, 0x10, KEY, UID); // Xác thực
    if (RW) // Lựa chọn đọc ghi, 1 là đọc, 0 là ghi
    {
        PcdRead(0x10, Dat);
    }
    else
    {
        PcdWrite(0x10, Dat);
    }
    PcdHalt();
}

