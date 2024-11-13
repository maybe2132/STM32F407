#include "main.h"
#include "flash.h"




//��ȡָ����ַ����(32λ����) 
//faddr:����ַ 
//����ֵ:��Ӧ����.
 uint32_t STMFLASH_ReadWord(uint32_t faddr)
{
    return *(uint32_t*)faddr; 
}

/**
 *@���ܣ����ڲ�Flash��ȡָ���ֽ�����
 *@����1��ReadAddress��������ʼ��ַ
 *@����2��*data��      ��ȡ�������ݻ����׵�ַ
 *@����3��length��     ��ȡ�ֽڸ���
 */
void ReadFlashData(uint32_t ReadAddress, uint8_t *data, uint32_t length)
{
    for(uint32_t i=0;i<length;i++)
    {
        data[i]=*(uint8_t*)(ReadAddress+i); //��ȡ����
    }
}

//��ȡĳ����ַ���ڵ�flash����
//addr:flash��ַ
//����ֵ:0~11,��addr���ڵ�����
uint8_t STMFLASH_GetFlashSector(uint32_t addr)
{
    if(addr<ADDR_FLASH_SECTOR_1)return FLASH_SECTOR_0;
    else if(addr<ADDR_FLASH_SECTOR_2)return FLASH_SECTOR_1;
    else if(addr<ADDR_FLASH_SECTOR_3)return FLASH_SECTOR_2;
    else if(addr<ADDR_FLASH_SECTOR_4)return FLASH_SECTOR_3;
    else if(addr<ADDR_FLASH_SECTOR_5)return FLASH_SECTOR_4;
    else if(addr<ADDR_FLASH_SECTOR_6)return FLASH_SECTOR_5;
    else if(addr<ADDR_FLASH_SECTOR_7)return FLASH_SECTOR_6;
    else if(addr<ADDR_FLASH_SECTOR_8)return FLASH_SECTOR_7;
    else if(addr<ADDR_FLASH_SECTOR_9)return FLASH_SECTOR_8;
    else if(addr<ADDR_FLASH_SECTOR_10)return FLASH_SECTOR_9;
    else if(addr<ADDR_FLASH_SECTOR_11)return FLASH_SECTOR_10;   
    return FLASH_SECTOR_11;	
}

/**
 *@���ܣ����ڲ�Flashд������
 *@����1��WriteAddress������Ҫд���Ŀ���ַ��ƫ�Ƶ�ַ��
 *@����2��*data�� д��������׵�ַ
 *@����3��length��д�����ݵĸ���
 */
void WriteFlashData(uint32_t WriteAddress, uint8_t *data, uint32_t length)
{
    FLASH_EraseInitTypeDef FlashEraseInit;
    HAL_StatusTypeDef FlashStatus=HAL_OK;
    uint32_t SectorError=0;
    uint32_t addrx=0;
    uint32_t endaddr=0;
    
    if( (WriteAddress < FMC_FLASH_BASE) || ( WriteAddress + length >= FMC_FLASH_END) || (length <= 0) )
    return;

    HAL_FLASH_Unlock();              //����
    addrx = WriteAddress;            //д�����ʼ��ַ
    endaddr = WriteAddress+length;   //д��Ľ�����ַ


        while(addrx<endaddr)  //ɨ��һ���ϰ�.(�Է�FFFFFFFF�ĵط�,�Ȳ���)
        {
             if(STMFLASH_ReadWord(addrx)!=0XFFFFFFFF)//�з�0XFFFFFFFF�ĵط�,Ҫ�����������
            {   
                FlashEraseInit.TypeErase=FLASH_TYPEERASE_SECTORS;       //�������ͣ��������� 
                FlashEraseInit.Sector=STMFLASH_GetFlashSector(addrx);   //Ҫ����������
                FlashEraseInit.NbSectors=1;                             //һ��ֻ����һ������
                FlashEraseInit.VoltageRange=FLASH_VOLTAGE_RANGE_3;      //��ѹ��Χ��VCC=2.7~3.6V֮��!!
                if(HAL_FLASHEx_Erase(&FlashEraseInit,&SectorError)!=HAL_OK) 
                {
                    break;//����������
                }
                }else addrx+=1;
                FLASH_WaitForLastOperation(FLASH_WAITETIME);                //�ȴ��ϴβ������
        }
    
    FlashStatus=FLASH_WaitForLastOperation(FLASH_WAITETIME);            //�ȴ��ϴβ������
    if(FlashStatus==HAL_OK)
    {
         while(WriteAddress<endaddr)//д����
         {
            if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE,WriteAddress,*data)!=HAL_OK)//д������
            { 
                break;	//д���쳣
            }
            WriteAddress+=1;
            data++;
        }  
    }
    HAL_FLASH_Lock();           //����
}

