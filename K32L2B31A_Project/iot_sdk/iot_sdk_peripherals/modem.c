/*! @file : modem.c
 * @author  JAVIER ELIAS TOBON AYUBB
 * @version 1.0.0
 * @date    12/10/2021
 * @brief   Driver para 
 * @details
 *
*/
/*******************************************************************************
 * Includes
 ******************************************************************************/

#include <sensorHume.h>
#include <sensorMeta.h>
#include <sensorTemp.h>
#include "modem.h"
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "K32L2B31A.h"
#include "fsl_debug_console.h"
#include "alarma.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define TRUE 1
#define FALSE 0
#define CNTL_Z	0x1A  // fin de Tx envio SMS y PUB MQTT
#define APN_TIGO TRUE

#ifdef APN_CLARO
const char APN_APP[]="internet.comcel.com.co";
#endif

#ifdef APN_MOVISTAR
const char APN_APP[]="internet.movistar.com.co";
#endif

#ifdef APN_TIGO
const char APN_APP[]="web.colombiamovil.com.co";
#endif

#define SIZE_BUFFER_COMANDO	50

/*******************************************************************************
 * Private Prototypes
 ******************************************************************************/
void Alarma_Init(void);
uint32_t Alarma_Set(uint32_t time2Wait);
char Alarma_Elapsed(uint32_t time2Test);

char Modem_Rta_Cmd(uint32_t time2Wait,char *rtaValidaStr,char estadoOk,char estadoErr);
char Recibido_URC(void);
void Modem_Check_URC_Run(void);
void Modem_Rta_Run(void);
int32_t numerodebytedisponiblesenbuffer(void);
uint8_t sacarByteBuffCir(void);

/*******************************************************************************
 * External vars
 ******************************************************************************/

/*******************************************************************************
 * Local vars
 ******************************************************************************/
char mqttSendSt;
char buffer_comando_recibido[SIZE_BUFFER_COMANDO];
char rtaModemStr[50];
char modemEstadoOK,modemEstadoExcepcion;
static char procesandoComando=0;
static uint32_t timeOutRta;
char 		buffer_comando_enviar[SIZE_BUFFER_COMANDO];
uint32_t 	index_buffer_nuevo_comandio_recibido=0;
char modemSt;
static char recibiMsgQtt;

/*******************************************************************************
 * Private Source Code
 ******************************************************************************/
enum{
	ST_MOD_IDLE,
	ST_MOD_CFG,	 // Configuracion inicial
	ST_MOD_ECO,  // quita ECO
	ST_MOD_CFG_URC,
	ST_MOD_CFG_ALL_URC,
	ST_MOD_PWR_OFF,
	ST_MOD_APN,
	ST_MOD_PWR_ON,
	ST_MOD_SIM,
	ST_MOD_SIGNAL,
	ST_MOD_SEARCHING,
	ST_MOD_ACT_CTX,
	ST_MOD_OPEN_MQTT,
	ST_MOD_CONN_TOPIC,
	ST_MOD_CONN_PUB,
	ST_MOD_PUBLIC_DAT,
	ST_MOD_KEEP_ALIVE,  // manda un AT y espera un OK
	ST_MOD_CHK_URC,
	ST_MOD_SUB_TOPIC,
};

/*******************************************************************************
 * Public Source Code
 ******************************************************************************/
void Send_MQTT_Data(char * topicPtr,char *datosPtr){
	//
}

void limpiarBufferComando(){
	for(int i=0;i<SIZE_BUFFER_COMANDO;i++){
		buffer_comando_recibido[i]=0x00;
	}
	index_buffer_nuevo_comandio_recibido=0;
}

char Send_MQTT_Error(void){
	return mqttSendSt;
}

void Modem_Init(void){
	modemSt = ST_MOD_CFG;
}

void Modem_Task_Run(void){

	Modem_Rta_Run();
	switch(modemSt){
	case ST_MOD_IDLE: // IDLE
	break;
	case ST_MOD_CFG:
		Modem_Send_Cmd("ATE0\r\n"); 								// ATE0 Quitar ECO
      	Modem_Rta_Cmd(TIME_WAIT_RTA_CMD,"OK",ST_MOD_PWR_OFF,ST_MOD_CFG); //rx OK
	break;
	case ST_MOD_CFG_URC:
		Modem_Send_Cmd("AT+QURCCFG=\"urcport\",\"uart1\"\r\n");
		Modem_Rta_Cmd(TIME_WAIT_RTA_CMD,"OK",ST_MOD_CFG_ALL_URC,ST_MOD_CFG_URC);
	break;
	case ST_MOD_CFG_ALL_URC:
		Modem_Send_Cmd("AT+QINDCFG=\"ALL\",1,1\r\n");
		Modem_Rta_Cmd(TIME_WAIT_RTA_CMD,"OK",ST_MOD_PWR_OFF,ST_MOD_CFG_URC);
	break;
	case ST_MOD_PWR_OFF:
		Modem_Send_Cmd("AT+CFUN=0\r\n"); // Modo Avion				//AT+CFUN=0
		Modem_Rta_Cmd(TIME_WAIT_RTA_CMD,"OK",ST_MOD_APN,ST_MOD_PWR_OFF); //rx OK
 	break;
	case ST_MOD_APN:
		buffer_comando_enviar[0] = 0;
		strcat(buffer_comando_enviar,"AT+CGDCONT=1,\"IP\",\"");
		strcat(buffer_comando_enviar,APN_APP);
		strcat(buffer_comando_enviar,"\"\r\n");
		Modem_Send_Cmd(buffer_comando_enviar); //Modem_Send_Cmd("AT+CGDCONT=1,\"IP\",\"internet.comcel.com.co\"\r\n"); //tx "AT+CGDCONT=1,"IP",APN_APP
		Modem_Rta_Cmd(TIME_WAIT_RTA_CMD,"OK",ST_MOD_PWR_ON,ST_MOD_APN); 	// rx "OK"
	break;
	case ST_MOD_PWR_ON:
		Modem_Send_Cmd("AT+CFUN=1\r\n");								//tx "AT+CFUN=1"
		Modem_Rta_Cmd(TIME_WAIT_RTA_CMD,"OK",ST_MOD_SIM,ST_MOD_PWR_ON); 	//rx "OK"
    break;
 	case ST_MOD_SIM:
 		Modem_Send_Cmd("AT+CPIN?\r\n");									//tx "AT+CPIN?"
 		Modem_Rta_Cmd(TIME_WAIT_RTA_CMD,"READY",ST_MOD_SIGNAL,ST_MOD_SIM); // rx "READY"
	break;
	case ST_MOD_SIGNAL:
		Modem_Send_Cmd("AT+CSQ\r\n");									//tx "AT+CSQ"
		Modem_Rta_Cmd(TIME_WAIT_RTA_CMD,"+CSQ",ST_MOD_SEARCHING,ST_MOD_SIGNAL);
	break;
	case ST_MOD_SEARCHING:
		Modem_Send_Cmd("AT+CREG?\r\n");											//tx "AT+CREG?"
		Modem_Rta_Cmd(TIME_WAIT_RTA_CMD,"0,1",ST_MOD_ACT_CTX,ST_MOD_SEARCHING); 	//rx  "0,1"
	break;
	case ST_MOD_ACT_CTX:
		Modem_Send_Cmd("AT+QIACT=1\r\n"); 									//tx "AT+QIACT=1"
		Modem_Rta_Cmd(TIME_WAIT_RTA_CMD,"OK",ST_MOD_OPEN_MQTT,ST_MOD_ACT_CTX); 	//rx "OK"
	break;
	case ST_MOD_OPEN_MQTT:
		Modem_Send_Cmd("AT+QMTOPEN=0,\"20.121.197.141\",1883\r\n"); //tx "AT+QMTOPEN=0,"142.93.88.99",1883"
		Modem_Rta_Cmd(TIME_WAIT_RTA_CMD,"+QMTOPEN: 0,0",ST_MOD_CONN_TOPIC,ST_MOD_OPEN_MQTT);
	break;
	case ST_MOD_CONN_TOPIC:
		Modem_Send_Cmd("AT+QMTCONN=0,\"topic/mensajes\"\r\n");	//tx "AT+QMTCONN=0,"TOPICO_APP""
		Modem_Rta_Cmd(TIME_WAIT_RTA_CMD,"+QMTCONN: 0,0,0",ST_MOD_SUB_TOPIC,ST_MOD_OPEN_MQTT);
	break;
	case ST_MOD_SUB_TOPIC:
		Modem_Send_Cmd("AT+QMTSUB=0 ,1,\"topic/mensajes\",1\r\n");	//tx "AT+QMTCONN=0,"TOPICO_APP""
		Modem_Rta_Cmd(TIME_WAIT_RTA_CMD,"+QMTSUB: 0,1,0,1",ST_MOD_CONN_PUB,ST_MOD_CONN_TOPIC);
	break;
	case ST_MOD_CONN_PUB:
		Modem_Send_Cmd("AT+QMTPUB=0,0,0,0,\"topic/mediciones\"\r\n");
		Modem_Rta_Cmd(TIME_WAIT_RTA_CMD,">",ST_MOD_PUBLIC_DAT,ST_MOD_OPEN_MQTT);
	break;
	case ST_MOD_PUBLIC_DAT:
		printf("%f,%f,%f \r\n",SenTempObtenerDatoCenti(),SenHumeObtenerDatoRH(),SenMetaObtenerDatoppm());
		putchar(CNTL_Z);
		Modem_Rta_Cmd(TIME_WAIT_RTA_CMD,"OK",ST_MOD_KEEP_ALIVE,ST_MOD_CONN_PUB);
	break;
	case ST_MOD_KEEP_ALIVE:
		Modem_Send_Cmd("AT\r\n");
		Modem_Rta_Cmd(3, "OK", ST_MOD_CONN_PUB, ST_MOD_KEEP_ALIVE);
	break;
	case ST_MOD_CHK_URC:
		if(Recibido_URC()){
			Modem_Check_URC_Run();
			if(recibiMsgQtt){
				recibiMsgQtt = 0;
				modemSt = ST_MOD_KEEP_ALIVE;
			}
		}
	break;
	}
 }

char *RxMqtt;

char Test_Rta_Modem(char *rta2TestStr){
	char *puntero_ok;
	puntero_ok=(char*)(strstr((char*)(&buffer_comando_recibido[0]),rta2TestStr));
	RxMqtt=(char*)(strstr((char*)(&buffer_comando_recibido[0]),rta2TestStr));
	if(puntero_ok) return 1;
	else return 0;
}

// Retorna TRUE si la respuesta es la Esperada
char Respuesta_Modem(char *rtaEsperada){
unsigned char nroBytesRecibidos,k;
char *puntero_ok;

	nroBytesRecibidos = numerodebytedisponiblesenbuffer();
	if(nroBytesRecibidos){
		for(k=0;k<nroBytesRecibidos;k++){
			buffer_comando_recibido[k]=sacarByteBuffCir();
    	}
		puntero_ok=(char*)(strstr((char*)(&buffer_comando_recibido[0]),rtaEsperada));
		if(puntero_ok == 0) return 0;
		else return 1;
 	}else return 0;
	return 0;
}

char Modem_Rta_Cmd(uint32_t time2Wait,char *rtaValidaStr,char estadoOk,char estadoErr){
	if(!procesandoComando){
		procesandoComando = 1;
   		limpiarBufferComando();
		timeOutRta = Alarma_Set(time2Wait);
		strcpy(rtaModemStr,rtaValidaStr);
		modemEstadoOK = estadoOk;
		modemEstadoExcepcion = estadoErr;
		modemSt = ST_MOD_IDLE;
		return 1;
	} return 0;
}

// Funcion de llamado continuo
void Modem_Rta_Run(void){
	if(procesandoComando){
		if(Alarma_Elapsed(timeOutRta)){
			if(Respuesta_Modem(rtaModemStr)){
				modemSt = modemEstadoOK;
			}else{
				modemSt = modemEstadoExcepcion;
			}
			procesandoComando = 0;
		}
	}
}

void Modem_Check_URC_Run(void){
	//!!! Recepcion de URCs
	if(Respuesta_Modem("+QMTRECV")){ // mensaje MQTT
		recibiMsgQtt = 1;
		if(Test_Rta_Modem("activar")){
			//led_on_green();
			activarAlarma();

		}
	}else {
		if(Test_Rta_Modem("desactivar")){	// algun otro URC
			apagarAlarma();
		}
	}

}

char Recibido_URC(void){
	static uint32_t time2waitEnd_URC;
	if(numerodebytedisponiblesenbuffer()){
		if(Alarma_Elapsed(time2waitEnd_URC)){
			return 1;
		}
		return 0;
	}else{
		time2waitEnd_URC = Alarma_Set(5); //tiempo que espera para fin de Rx de URC
		return 0;
	}
}



