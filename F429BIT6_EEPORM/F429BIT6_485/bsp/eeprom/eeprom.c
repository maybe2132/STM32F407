
#include "eeprom.h"
//    PH7     ------> I2C3_SCL
//    PH8     ------> I2C3_SDA
/**
  * @brief   写多个字节到I2C EEPROM中，但一次写入的字节数不能超过EEPROM的大小，AT24C02每页有8个字节
  * @param   
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址 
  * @retval  无
  */
uint32_t I2C_EE_ByteWrite(uint8_t* pBuffer, uint8_t WriteAddr,uint8_t NumByteToWrite)
{
	HAL_StatusTypeDef status=HAL_OK;
	
	status=HAL_I2C_Mem_Write_DMA(&I2C_X,EEPROM_ADDRESS,(uint16_t)WriteAddr,I2C_MEMADD_SIZE_8BIT,pBuffer,NumByteToWrite);
	//等待发送结束
	while (HAL_I2C_GetState(&I2C_X) != HAL_I2C_STATE_READY);
	//检查EEPROM是否准备好进行新的操作
	while (HAL_I2C_IsDeviceReady(&I2C_X, EEPROM_ADDRESS, 300, 300) != HAL_OK);
	while (HAL_I2C_GetState(&I2C_X) != HAL_I2C_STATE_READY);
	return status;	
}
/**
  * @brief   将缓冲区中的数据写到I2C EEPROM中,这其中涉及到换页操作，因为HAL_I2C_Mem_Write_DMA()最多只能传8个字节，
						 要传更多的字节需要换页
  * @param   
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址
  *     @arg NumByteToWrite:写的字节数
  * @retval  无
  */


void I2C_EE_BufferWrite(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
{
	uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;
	
	Addr = WriteAddr % 8;	//将写入起始地址与8取余，得到不对齐的数据个数
  count = 8 - Addr;	//地址所在页能够写入的个数
  NumOfPage =  NumByteToWrite / 8;//传入的数据需要的页数
  NumOfSingle = NumByteToWrite % 8;//最后一页的写入个数
	//起始写入位置刚好与页对齐
	if(Addr==0)
	{
		if(NumByteToWrite<=count)//传入的字节数刚好一页可以写完
		{
			I2C_EE_ByteWrite(pBuffer, WriteAddr, NumOfSingle);
		}
		else
		{
			while(NumOfPage--)
			{
				I2C_EE_ByteWrite(pBuffer, WriteAddr, 8);
				WriteAddr+=8;	//每写一页，写地址需要加8
				pBuffer+=8;//每写一页，缓冲区里面的数据地址需要加8
			}
			if(NumOfSingle!=0)//最后将剩余的几个数写入EEPROM
			{
				I2C_EE_ByteWrite(pBuffer, WriteAddr, NumOfSingle);
			}
		}
	}
	else
	{
		if(NumOfPage== 0) //如果要传入的数据少于一页
    {
      I2C_EE_ByteWrite(pBuffer, WriteAddr, NumOfSingle);
    }
		else
		{
			//其实写入地址不对齐时，需要先将所在的那一页数据填满然后再重新计算所需页数和剩余个数
			NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / 8;
      NumOfSingle = NumByteToWrite % 8;	
			if(count != 0)
      {  
        I2C_EE_ByteWrite(pBuffer, WriteAddr, count);
        WriteAddr += count;
        pBuffer += count;
      }
			 while(NumOfPage--)
      {
        I2C_EE_ByteWrite(pBuffer, WriteAddr, 8);
        WriteAddr +=  8;
        pBuffer += 8;  
      }
      if(NumOfSingle != 0)
      {
        I2C_EE_ByteWrite(pBuffer, WriteAddr, NumOfSingle); 
      }
		}
	}
	
}
uint8_t I2c_Buf_Write[50];
uint8_t I2c_Buf_Read[50];
int DATA_Size=50;

/**
  * @brief   从EEPROM里面读取一块数据 
  * @param   读操作，可以一次性读出
  *		@arg pBuffer:存放从EEPROM读取的数据的缓冲区指针
  *		@arg ReadAddr:读取数据的EEPROM的地址
  *     @arg NumByteToWrite:要从EEPROM读取的字节数
  * @retval  无
  */
uint32_t I2C_EE_BufferRead(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{
	HAL_StatusTypeDef status = HAL_OK;
	status=HAL_I2C_Mem_Read_DMA(&I2C_X,EEPROM_ADDRESS,ReadAddr, I2C_MEMADD_SIZE_8BIT, (uint8_t *)pBuffer, NumByteToRead);
 
	return status;
}

#if 0
//写入读取测试
uint8_t I2C_Test(void)
{
	uint16_t i;
	printf("写入的数据:");
	for(i=0;i<DATA_Size;i++)
	{
		I2c_Buf_Write[i]=i;
		printf("0x%02X ", I2c_Buf_Write[i]);
		if(i%16 == 15)  //i=255;  
		printf("\n\r");  
	}
	//将I2c_Buf_Write中顺序递增的数据写入EERPOM中 
	I2C_EE_BufferWrite( I2c_Buf_Write, 0x00, DATA_Size);
	printf("读出的数据");
	//将EEPROM读出数据顺序保持到I2c_Buf_Read中
	I2C_EE_BufferRead(I2c_Buf_Read, 0x00, DATA_Size); 
	//将I2c_Buf_Read中的数据通过串口打印
	for (i=0; i<DATA_Size; i++)
	{	
		if(I2c_Buf_Read[i] != I2c_Buf_Write[i])
		{
			printf("0x%02X ", I2c_Buf_Read[i]);
			printf("错误:I2C EEPROM写入与读出的数据不一致");
			return 0;
		}
		printf("0x%02X ", I2c_Buf_Read[i]);
		if(i%16 == 15)    
		printf("\n\r");
 
	}
	printf("I2C(AT24C02)读写测试成功");
	return 1;
}
// 读取测试
uint8_t I2C_ReadData(void)
{
	I2C_EE_BufferRead(I2c_Buf_Read, 0x00, DATA_Size); 
	//
	for (int i=0; i<DATA_Size; i++)
	{	
		printf("0x%02X ", I2c_Buf_Read[i]);
		if(i%16 == 15)    
		printf("\n\r");
 
	}
}
#endif