
#include "eeprom.h"
//    PH7     ------> I2C3_SCL
//    PH8     ------> I2C3_SDA
/**
  * @brief   д����ֽڵ�I2C EEPROM�У���һ��д����ֽ������ܳ���EEPROM�Ĵ�С��AT24C02ÿҳ��8���ֽ�
  * @param   
  *		@arg pBuffer:������ָ��
  *		@arg WriteAddr:д��ַ 
  * @retval  ��
  */
uint32_t I2C_EE_ByteWrite(uint8_t* pBuffer, uint8_t WriteAddr,uint8_t NumByteToWrite)
{
	HAL_StatusTypeDef status=HAL_OK;
	
	status=HAL_I2C_Mem_Write_DMA(&I2C_X,EEPROM_ADDRESS,(uint16_t)WriteAddr,I2C_MEMADD_SIZE_8BIT,pBuffer,NumByteToWrite);
	//�ȴ����ͽ���
	while (HAL_I2C_GetState(&I2C_X) != HAL_I2C_STATE_READY);
	//���EEPROM�Ƿ�׼���ý����µĲ���
	while (HAL_I2C_IsDeviceReady(&I2C_X, EEPROM_ADDRESS, 300, 300) != HAL_OK);
	while (HAL_I2C_GetState(&I2C_X) != HAL_I2C_STATE_READY);
	return status;	
}
/**
  * @brief   ���������е�����д��I2C EEPROM��,�������漰����ҳ��������ΪHAL_I2C_Mem_Write_DMA()���ֻ�ܴ�8���ֽڣ�
						 Ҫ��������ֽ���Ҫ��ҳ
  * @param   
  *		@arg pBuffer:������ָ��
  *		@arg WriteAddr:д��ַ
  *     @arg NumByteToWrite:д���ֽ���
  * @retval  ��
  */


void I2C_EE_BufferWrite(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
{
	uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;
	
	Addr = WriteAddr % 8;	//��д����ʼ��ַ��8ȡ�࣬�õ�����������ݸ���
  count = 8 - Addr;	//��ַ����ҳ�ܹ�д��ĸ���
  NumOfPage =  NumByteToWrite / 8;//�����������Ҫ��ҳ��
  NumOfSingle = NumByteToWrite % 8;//���һҳ��д�����
	//��ʼд��λ�øպ���ҳ����
	if(Addr==0)
	{
		if(NumByteToWrite<=count)//������ֽ����պ�һҳ����д��
		{
			I2C_EE_ByteWrite(pBuffer, WriteAddr, NumOfSingle);
		}
		else
		{
			while(NumOfPage--)
			{
				I2C_EE_ByteWrite(pBuffer, WriteAddr, 8);
				WriteAddr+=8;	//ÿдһҳ��д��ַ��Ҫ��8
				pBuffer+=8;//ÿдһҳ����������������ݵ�ַ��Ҫ��8
			}
			if(NumOfSingle!=0)//���ʣ��ļ�����д��EEPROM
			{
				I2C_EE_ByteWrite(pBuffer, WriteAddr, NumOfSingle);
			}
		}
	}
	else
	{
		if(NumOfPage== 0) //���Ҫ�������������һҳ
    {
      I2C_EE_ByteWrite(pBuffer, WriteAddr, NumOfSingle);
    }
		else
		{
			//��ʵд���ַ������ʱ����Ҫ�Ƚ����ڵ���һҳ��������Ȼ�������¼�������ҳ����ʣ�����
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
  * @brief   ��EEPROM�����ȡһ������ 
  * @param   ������������һ���Զ���
  *		@arg pBuffer:��Ŵ�EEPROM��ȡ�����ݵĻ�����ָ��
  *		@arg ReadAddr:��ȡ���ݵ�EEPROM�ĵ�ַ
  *     @arg NumByteToWrite:Ҫ��EEPROM��ȡ���ֽ���
  * @retval  ��
  */
uint32_t I2C_EE_BufferRead(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{
	HAL_StatusTypeDef status = HAL_OK;
	status=HAL_I2C_Mem_Read_DMA(&I2C_X,EEPROM_ADDRESS,ReadAddr, I2C_MEMADD_SIZE_8BIT, (uint8_t *)pBuffer, NumByteToRead);
 
	return status;
}

#if 0
//д���ȡ����
uint8_t I2C_Test(void)
{
	uint16_t i;
	printf("д�������:");
	for(i=0;i<DATA_Size;i++)
	{
		I2c_Buf_Write[i]=i;
		printf("0x%02X ", I2c_Buf_Write[i]);
		if(i%16 == 15)  //i=255;  
		printf("\n\r");  
	}
	//��I2c_Buf_Write��˳�����������д��EERPOM�� 
	I2C_EE_BufferWrite( I2c_Buf_Write, 0x00, DATA_Size);
	printf("����������");
	//��EEPROM��������˳�򱣳ֵ�I2c_Buf_Read��
	I2C_EE_BufferRead(I2c_Buf_Read, 0x00, DATA_Size); 
	//��I2c_Buf_Read�е�����ͨ�����ڴ�ӡ
	for (i=0; i<DATA_Size; i++)
	{	
		if(I2c_Buf_Read[i] != I2c_Buf_Write[i])
		{
			printf("0x%02X ", I2c_Buf_Read[i]);
			printf("����:I2C EEPROMд������������ݲ�һ��");
			return 0;
		}
		printf("0x%02X ", I2c_Buf_Read[i]);
		if(i%16 == 15)    
		printf("\n\r");
 
	}
	printf("I2C(AT24C02)��д���Գɹ�");
	return 1;
}
// ��ȡ����
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