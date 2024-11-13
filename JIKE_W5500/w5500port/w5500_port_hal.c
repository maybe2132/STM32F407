#include "w5500_port_hal.h"
#include "socket.h"
#include "main.h"
#include <stdlib.h>
#include "string.h"
#define W5500_CS_L HAL_GPIO_WritePin(W5500_CS_PORT, W5500_CS_PIN, GPIO_PIN_RESET);
#define W5500_CS_H HAL_GPIO_WritePin(W5500_CS_PORT, W5500_CS_PIN, GPIO_PIN_SET);
wiz_NetInfo gWIZNETINFO = { .mac = {0x78, 0x83, 0x68, 0x88, 0x56, 0x72},
                            .ip =  {192, 168, 1,136},
                            .sn =  {255,255,255,0},
                            .gw =  {192, 168,1,1},
                            .dns = {180,76,76,76},
                            .dhcp = NETINFO_DHCP};
 
 
uint16_t Net_Status; 									//SOCKET ����ֵ
uint8_t destip[4] = {192, 168, 1, 108};  //tcpģʽ ������ip
uint16_t destport = 5000;									//�������˿�
uint8_t buffer[2048];											//���ݻ�����

														
void SPI_WriteByte(uint8_t TxData)										//SPIдһ�ֽ�
{                
  uint8_t data;
  HAL_SPI_TransmitReceive(&hspi1,&TxData,&data,1,100);
}
 
 
uint8_t SPI_ReadByte(void)														//SPI ��һ�ֽ�
{     
    uint8_t data[2]={0xFF,0xFF};
    HAL_SPI_TransmitReceive(&hspi1,data,data+1,1,100);
    return data[1];
}
 
 
void SPI_CrisEnter(void)																// 
{
    __set_PRIMASK(1);
}
 
 
void SPI_CrisExit(void)
{
    __set_PRIMASK(0);
}
 
 
void SPI_CS_Select(void)
{
    W5500_CS_L;
}
 
 
void SPI_CS_Deselect(void)
{
    W5500_CS_H;
}
//W5500��λ
void W5500_RESET(void)
{
    HAL_GPIO_WritePin(W5500_RST_PORT, W5500_RST_PIN, GPIO_PIN_RESET);
    HAL_Delay(50);
    HAL_GPIO_WritePin(W5500_RST_PORT, W5500_RST_PIN, GPIO_PIN_SET);
    HAL_Delay(10);
}
 
 
//��ʼ��оƬ����
void ChipParametersConfiguration(void)
{
    uint8_t tmp;
    uint8_t memsize[2][8] = {{2,2,2,2,2,2,2,2},{2,2,2,2,2,2,2,2}};
    if(ctlwizchip(CW_INIT_WIZCHIP,(void*)memsize) == -1)
    {
        while(1);
    }
 
 
    do{
        if(ctlwizchip(CW_GET_PHYLINK, (void*)&tmp) == -1)
        {
          while(1);
        }
    }while(tmp == PHY_LINK_OFF);
}
 
 
//��ʼ��������� mac ip��
void NetworkParameterConfiguration(void)  //Intialize the network information to be used in WIZCHIP
{
  uint8_t tmpstr[6];
 
 
  ctlnetwork(CN_SET_NETINFO, (void*)&gWIZNETINFO);
  ctlnetwork(CN_GET_NETINFO, (void*)&gWIZNETINFO);
  ctlwizchip(CW_GET_ID,(void*)tmpstr);
}
 
 
//W5500��ʼ��
void W5500_ChipInit(void)
{
    W5500_RESET();
    reg_wizchip_cris_cbfunc(SPI_CrisEnter, SPI_CrisExit); //ע���ٽ纯��
    reg_wizchip_cs_cbfunc(SPI_CS_Select, SPI_CS_Deselect); //ע��SPIƬѡ����
    reg_wizchip_spi_cbfunc(SPI_ReadByte, SPI_WriteByte); //ע��SPI��д����
    ChipParametersConfiguration();//��ʼ��оƬ
    NetworkParameterConfiguration();//��ʼ������������� MAC IP��
}

#if 0
void do_tcpc(int sn)
{
    uint16_t Len;
		uint8_t temp[]="\r\n";
    Net_Status = getSn_SR(sn);
 
 
    switch(Net_Status)                  /*��ȡsocket��״̬*/
    {
    case SOCK_CLOSED:                   /*socket���ڹر�״̬*/
        socket(sn,Sn_MR_TCP,8123,Sn_MR_ND);
        break;
    case SOCK_INIT:                      /*socket���ڳ�ʼ��״̬*/
				connect(sn,destip,destport);/*socket���ӷ�����*/
        break;
    case SOCK_ESTABLISHED:               /*socket�������ӽ���״̬*/
        if(getSn_IR(sn) & Sn_IR_CON) 
        {
            setSn_IR(sn, Sn_IR_CON);      /*��������жϱ�־λ*/
        }
        Len=getSn_RX_RSR(sn);            /*��ȡ���յ����ݳ���*/
        if(Len>0)  //���յ�����
        {
            recv(sn,buffer,Len);     /*��������Server������*/
            send(sn,buffer,Len);    //���ظ�������
						RS_485_Send(buffer,Len);
        }
 
 
        break;
    case SOCK_CLOSE_WAIT:        /*socket���ڵȴ��ر�״̬*/
        close(0);
        break;
    }
}
#endif
void do_tcpc(int sn)
{
		uint16_t Len;
		uint8_t temp[]="\r\n";
    Net_Status = getSn_SR(sn);
	switch(getSn_SR(sn))									// ��ȡsocket0��״??
		{
			case SOCK_INIT:									// Socket���ڳ�ʼ����??(��)״??
				listen(sn);									// �����ոմ򿪵ı��ض˿ڣ��ȴ��ͻ�����??
			break;
			case SOCK_ESTABLISHED:							// Socket�������ӽ���״??
				if(getSn_IR(sn) & Sn_IR_CON)			
				{
					setSn_IR(sn, Sn_IR_CON);					// Sn_IR��CONλ��1��֪ͨW5500�����ѽ�??
				}
				// ���ݻػ����Գ������ݴ���λ���ͻ��˷���W5500��W5500���յ����ݺ��ٻظ��ͻ���
				Len=getSn_RX_RSR(sn);						// ��ȡW5500���н��ջ���Ĵ�����ֵ��Sn_RX_RSR��ʾ���ջ������ѽ��պͱ�������ݴ�С
				if(Len>0)
				{
					recv(sn,buffer,Len);						// W5500�������Կͻ��˵����ݣ���ͨ��SPI���͸�MCU
					printf("%s\r\n",buffer);				// ���ڴ�ӡ���յ�������
					send(sn,buffer,Len);						// ���յ����ݺ��ٻظ��ͻ��ˣ�������ݻ�??
				}

			break;
			case SOCK_CLOSE_WAIT:							// Socket���ڵȴ��ر�״??
				close(sn);									// �ر�Socket0
			break;
			case SOCK_CLOSED:								// Socket���ڹر�״??
				socket(sn,Sn_MR_TCP,8123,Sn_MR_ND);	// ��Socket0��������ΪTCP����ʱģʽ����һ�����ض�??
			break;
		}

}