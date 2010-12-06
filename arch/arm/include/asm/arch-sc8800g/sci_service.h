/******************************************************************************
 ** File Name:      sci_service.h                                                 *
 ** Author:         Fancier.Fan                                               *
 ** DATE:           03/16/2006                                                *
 ** Copyright:      2006 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file includes some common message type defines for   *
 **                 client/server operation                                   * 
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 03/16/2006     Fancier.Fan     Create.                                   *
 ******************************************************************************/
#ifndef _SCI_SERVICE_H
#define _SCI_SERVICE_H

#include "sci_types.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*****************************************************************************/
//  Description:    The type is used to identify different server
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           if external user needs to use it, please define the id 
//                  after USER_SERVICE
/*****************************************************************************/
typedef enum
{
	FILE_SERVICE = 1,				// File service, Reserved
    AUDIO_SERVICE,					// Audio service
    KEYPAD_SERVICE,					// Keypad service
    USB_SERVICE,	 				// Udisk service
    UPM_SERVICE=USB_SERVICE,
	CHR_SERVICE,					// Charge service
    TIME_SERVICE,					// RTC service
    
    INTERNAL_USER_SERVICE = 7,		// For MN and such service
    INTERNAL_USER_SERVICE8,			
    INTERNAL_USER_SERVICE9,
    
    INTERNAL_USER_SERVICE10,
    INTERNAL_USER_SERVICE11,
    INTERNAL_USER_SERVICE12,
    INTERNAL_USER_SERVICE13,
    INTERNAL_USER_SERVICE14,
    INTERNAL_USER_SERVICE15,
    INTERNAL_USER_SERVICE16,
    INTERNAL_USER_SERVICE17,
    INTERNAL_USER_SERVICE18,
    INTERNAL_USER_SERVICE19, 
       
    INTERNAL_USER_SERVICE20,    
    INTERNAL_USER_SERVICE21,    
    INTERNAL_USER_SERVICE22,    
    INTERNAL_USER_SERVICE23,    
    INTERNAL_USER_SERVICE24,    
    INTERNAL_USER_SERVICE25,    
    INTERNAL_USER_SERVICE26,    
    INTERNAL_USER_SERVICE27,    
    INTERNAL_USER_SERVICE28,    
    INTERNAL_USER_SERVICE29, 
       
    INTERNAL_USER_SERVICE30,    
    INTERNAL_USER_SERVICE31,    
    INTERNAL_USER_SERVICE32,    
    INTERNAL_USER_SERVICE33,    
    INTERNAL_USER_SERVICE34,    
    INTERNAL_USER_SERVICE35,    
    INTERNAL_USER_SERVICE36,    
    INTERNAL_USER_SERVICE37,    
    INTERNAL_USER_SERVICE38,    
    INTERNAL_USER_SERVICE39,
    
    INTERNAL_USER_SERVICE40,    
    INTERNAL_USER_SERVICE41,    
    INTERNAL_USER_SERVICE42,    
    INTERNAL_USER_SERVICE43,    
    INTERNAL_USER_SERVICE44,    
    INTERNAL_USER_SERVICE45,    
    INTERNAL_USER_SERVICE46,    
    INTERNAL_USER_SERVICE47,    
    INTERNAL_USER_SERVICE48,    
    INTERNAL_USER_SERVICE49,

    INTERNAL_USER_SERVICE50,    
    INTERNAL_USER_SERVICE51,    
    INTERNAL_USER_SERVICE52,    
    INTERNAL_USER_SERVICE53,    
    INTERNAL_USER_SERVICE54,    
    INTERNAL_USER_SERVICE55,    
    INTERNAL_USER_SERVICE56,    
    INTERNAL_USER_SERVICE57,    
    INTERNAL_USER_SERVICE58,    
    INTERNAL_USER_SERVICE59,

    INTERNAL_USER_SERVICE60,    
    INTERNAL_USER_SERVICE61,    
    INTERNAL_USER_SERVICE62,    
    INTERNAL_USER_SERVICE63,    
    
    SYSTEM_SERVICE = 64,			// 64~127 for system
    SYSTEM_SERVICE65,
    SYSTEM_SERVICE66,
    SYSTEM_SERVICE67,
    SYSTEM_SERVICE68,
    SYSTEM_SERVICE69,
    
    SYSTEM_SERVICE70,
	SYSTEM_SERVICE71,
	SYSTEM_SERVICE72,
	SYSTEM_SERVICE73,
	SYSTEM_SERVICE74,
	SYSTEM_SERVICE75,
	SYSTEM_SERVICE76,
	SYSTEM_SERVICE77,						
	SYSTEM_SERVICE78,
	SYSTEM_SERVICE79,	
	    
    SYSTEM_SERVICE80,
	SYSTEM_SERVICE81,
	SYSTEM_SERVICE82,
	SYSTEM_SERVICE83,
	SYSTEM_SERVICE84,
	SYSTEM_SERVICE85,
	SYSTEM_SERVICE86,
	SYSTEM_SERVICE87,						
	SYSTEM_SERVICE88,
	SYSTEM_SERVICE89,	

    SYSTEM_SERVICE90,
	SYSTEM_SERVICE91,
	SYSTEM_SERVICE92,
	SYSTEM_SERVICE93,
	SYSTEM_SERVICE94,
	SYSTEM_SERVICE95,
	SYSTEM_SERVICE96,
	SYSTEM_SERVICE97,						
	SYSTEM_SERVICE98,
	SYSTEM_SERVICE99,	
			    
    TP_SERVICE = 100,				// For Touchpanel service 
    ISP_SERVICE,
    EXTERNAL_USER_SERVICE = 128,	// 128~255 for external user 
    EXTERNAL_USER_SERVICE129,
    
    EXTERNAL_USER_SERVICE130,
	EXTERNAL_USER_SERVICE131,
	EXTERNAL_USER_SERVICE132,
	EXTERNAL_USER_SERVICE133,
	EXTERNAL_USER_SERVICE134,
	EXTERNAL_USER_SERVICE135,
	EXTERNAL_USER_SERVICE136,
	EXTERNAL_USER_SERVICE137,						
	EXTERNAL_USER_SERVICE138,
	EXTERNAL_USER_SERVICE139,   
	
    EXTERNAL_USER_SERVICE140,
	EXTERNAL_USER_SERVICE141,
	EXTERNAL_USER_SERVICE142,
	EXTERNAL_USER_SERVICE143,
	EXTERNAL_USER_SERVICE144,
	EXTERNAL_USER_SERVICE145,
	EXTERNAL_USER_SERVICE146,
	EXTERNAL_USER_SERVICE147,						
	EXTERNAL_USER_SERVICE148,
	EXTERNAL_USER_SERVICE149,   	
	
	
    EXTERNAL_USER_SERVICE150,
	EXTERNAL_USER_SERVICE151,
	EXTERNAL_USER_SERVICE152,
	EXTERNAL_USER_SERVICE153,
	EXTERNAL_USER_SERVICE154,
	EXTERNAL_USER_SERVICE155,
	EXTERNAL_USER_SERVICE156,
	EXTERNAL_USER_SERVICE157,						
	EXTERNAL_USER_SERVICE158,
	EXTERNAL_USER_SERVICE159,   
	
    EXTERNAL_USER_SERVICE160,
	EXTERNAL_USER_SERVICE161,
	EXTERNAL_USER_SERVICE162,
	EXTERNAL_USER_SERVICE163,
	EXTERNAL_USER_SERVICE164,
	EXTERNAL_USER_SERVICE165,
	EXTERNAL_USER_SERVICE166,
	EXTERNAL_USER_SERVICE167,						
	EXTERNAL_USER_SERVICE168,
	EXTERNAL_USER_SERVICE169,   	
	
    EXTERNAL_USER_SERVICE170,
	EXTERNAL_USER_SERVICE171,
	EXTERNAL_USER_SERVICE172,
	EXTERNAL_USER_SERVICE173,
	EXTERNAL_USER_SERVICE174,
	EXTERNAL_USER_SERVICE175,
	EXTERNAL_USER_SERVICE176,
	EXTERNAL_USER_SERVICE177,						
	EXTERNAL_USER_SERVICE178,
	EXTERNAL_USER_SERVICE179,   	
	
    EXTERNAL_USER_SERVICE180,
	EXTERNAL_USER_SERVICE181,
	EXTERNAL_USER_SERVICE182,
	EXTERNAL_USER_SERVICE183,
	EXTERNAL_USER_SERVICE184,
	EXTERNAL_USER_SERVICE185,
	EXTERNAL_USER_SERVICE186,
	EXTERNAL_USER_SERVICE187,						
	EXTERNAL_USER_SERVICE188,
	EXTERNAL_USER_SERVICE189,   	
	
    EXTERNAL_USER_SERVICE190,
	EXTERNAL_USER_SERVICE191,
	EXTERNAL_USER_SERVICE192,
	EXTERNAL_USER_SERVICE193,
	EXTERNAL_USER_SERVICE194,
	EXTERNAL_USER_SERVICE195,
	EXTERNAL_USER_SERVICE196,
	EXTERNAL_USER_SERVICE197,						
	EXTERNAL_USER_SERVICE198,
	EXTERNAL_USER_SERVICE199,   
		
    EXTERNAL_USER_SERVICE200,
	EXTERNAL_USER_SERVICE201,
	EXTERNAL_USER_SERVICE202,
	EXTERNAL_USER_SERVICE203,
	EXTERNAL_USER_SERVICE204,
	EXTERNAL_USER_SERVICE205,
	EXTERNAL_USER_SERVICE206,
	EXTERNAL_USER_SERVICE207,						
	EXTERNAL_USER_SERVICE208,
	EXTERNAL_USER_SERVICE209,   		
	
		
    EXTERNAL_USER_SERVICE210,
	EXTERNAL_USER_SERVICE211,
	EXTERNAL_USER_SERVICE212,
	EXTERNAL_USER_SERVICE213,
	EXTERNAL_USER_SERVICE214,
	EXTERNAL_USER_SERVICE215,
	EXTERNAL_USER_SERVICE216,
	EXTERNAL_USER_SERVICE217,						
	EXTERNAL_USER_SERVICE218,
	EXTERNAL_USER_SERVICE219,   	

    EXTERNAL_USER_SERVICE220,
	EXTERNAL_USER_SERVICE221,
	EXTERNAL_USER_SERVICE222,
	EXTERNAL_USER_SERVICE223,
	EXTERNAL_USER_SERVICE224,
	EXTERNAL_USER_SERVICE225,
	EXTERNAL_USER_SERVICE226,
	EXTERNAL_USER_SERVICE227,						
	EXTERNAL_USER_SERVICE228,
	EXTERNAL_USER_SERVICE229,   	

    EXTERNAL_USER_SERVICE230,
	EXTERNAL_USER_SERVICE231,
	EXTERNAL_USER_SERVICE232,
	EXTERNAL_USER_SERVICE233,
	EXTERNAL_USER_SERVICE234,
	EXTERNAL_USER_SERVICE235,
	EXTERNAL_USER_SERVICE236,
	EXTERNAL_USER_SERVICE237,						
	EXTERNAL_USER_SERVICE238,
	EXTERNAL_USER_SERVICE239,   	

    EXTERNAL_USER_SERVICE240,
	EXTERNAL_USER_SERVICE241,
	EXTERNAL_USER_SERVICE242,
	EXTERNAL_USER_SERVICE243,
	EXTERNAL_USER_SERVICE244,
	EXTERNAL_USER_SERVICE245,
	EXTERNAL_USER_SERVICE246,
	EXTERNAL_USER_SERVICE247,						
	EXTERNAL_USER_SERVICE248,
	EXTERNAL_USER_SERVICE249, 
	  	
    EXTERNAL_USER_SERVICE250,
	EXTERNAL_USER_SERVICE251,
	EXTERNAL_USER_SERVICE252,
	EXTERNAL_USER_SERVICE253,
	EXTERNAL_USER_SERVICE254,
	EXTERNAL_USER_SERVICE255,
 	
	MAX_SERVICE = 256
} SRV_ID_E;

#define UDISK_SERVICE USB_SERVICE   // UPM manager service.It has the same SERVICE ID with Udisk.
                                    // UDISK_SERVICE is used in D platform.
                                    // UPM_SERVICE is used in M platform.
                                    // BUT this is forbidden and External User will defined its own service seperately
#define MN_APP_PHONE_SERVICE  (INTERNAL_USER_SERVICE)
#define MN_APP_CALL_SERVICE   (INTERNAL_USER_SERVICE+1)
#define MN_APP_GPRS_SERVICE   (INTERNAL_USER_SERVICE+2)
#define MN_APP_SMS_SERVICE    (INTERNAL_USER_SERVICE+3)
#define MN_APP_SMSCB_SERVICE  (INTERNAL_USER_SERVICE+4)
#define MN_APP_SS_SERVICE     (INTERNAL_USER_SERVICE+5)
#define MN_APP_SIMAT_SERVICE  (INTERNAL_USER_SERVICE+6)
#define MN_APP_SIM_SERVICE    (INTERNAL_USER_SERVICE+7)

#define DRM_SERVICE           (INTERNAL_USER_SERVICE+8)  // DRM Service

#define DTL_SERVICE 		  (INTERNAL_USER_SERVICE+9)  //MTV DTL
#define CSM_SERVICE           (INTERNAL_USER_SERVICE+10) //MTV channel status monitor service id  

#define STREAMING_SERVICE (INTERNAL_USER_SERVICE + 11) //streaming

#define MSENSOR_SERVICE       (INTERNAL_USER_SERVICE+12)    // M-sensor Service


/*****************************************************************************/
//  Description:    Below is the interface for server/client  
//	Global resource dependence: 
//  Author: Richard.Yang
//	Note:
/*****************************************************************************/

#define  SCI_ID_USED     0xff
#define  SCI_INVALID_ID  0x0100

/*****************************************************************************/
//  Description:    The function is used to be called when client need 
//                  indication from server
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           id:    Task id of the client which register the event
//                  The specific funtion of argc and argv, please see each
//                  server's definition, according to function
//                  PUBLIC void SCI_SendEventToClient(
//                  uint32 server_ID,     // Server ID, EXAMPLE: KEYPAD_SERVICE
//                  uint32 ui_event_type, // Message ID of the registered event
//                  void * signal_ptr     // Signal Sent to Client
//                  );
//                  Where argc equals ui_event_type, argv equals to signal_ptr
/*****************************************************************************/
typedef void (* REG_CALLBACK) (uint32 id, uint32 argc, void *argv);

/*****************************************************************************/
//  Description:    The function is create a server list 
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC  uint32 SCI_CreateClientList( // Return SCI_ID_USED if current ID is used by user
                                     // Return SCI_SUCCESS if OK.
    uint32       server_ID,       // Server ID, must be SRV_ID_E type
    uint8        total_event_num, // Total number of the event.
    REG_CALLBACK default_fun      // Default callback function. Called if 
                                  // client has no callback function.
);

/*****************************************************************************/
//  Description:    The function is delete a server list 
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC  uint32 SCI_DeleteClientList( // Return SCI_ID_USED if current ID is used by user
                                     // Return SCI_SUCCESS if OK.
    uint32  server_ID       // Server ID, must be SRV_ID_E type
);

/*****************************************************************************/
//  Description:    The function is used by server to send meesage to 
//                  registered clients
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC void SCI_SendEventToClient(
    uint32 server_ID,     // Server ID, EXAMPLE: KEYPAD_SERVICE
    uint32 ui_event_type, // Message ID of the registered event
    void * signal_ptr     // Signal Sent to Client
);

/*****************************************************************************/
//  Description:    The function is used by client to register event to server
//                  The current task is treated as the client which register
//                  the event
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC  uint32 SCI_RegisterMsg( // Return SCI_INVALID_ID if no such sever
                                // else return SCI_SUCCESS
    uint32       server_ID,   // Server ID, must be SRV_ID_E type
    uint8        start_event, // Start number of event
    uint8        end_event,   // End number of event
    REG_CALLBACK fun          // Callback function user defined
);

/*****************************************************************************/
//  Description:    The function is used by client to unregister event to server
//                  The current task is treated as the client which register
//                  the event
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC  uint32 SCI_UnregisterMsg( // Return SCI_INVALID_ID if no such sever
                                  // else return SCI_SUCCESS
    uint32       server_ID,   // Server ID, must be SRV_ID_E type
    uint8        start_event, // Start number of event
    uint8        end_event,   // End number of event
    REG_CALLBACK fun          // Callback function user defined
);

/*****************************************************************************/
//  Description:    The function is used by client to register event to server
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC  uint32 SCI_RegisterTaskMsg( // Return SCI_INVALID_ID if no such sever
                              // else return SCI_SUCCESS
    uint32       server_ID,   // Server ID, must be SRV_ID_E type
    uint8        start_event, // Start number of event
    uint8        end_event,   // End number of event
	uint32       task_ID,     // ID of the task which register the event
    REG_CALLBACK fun          // Callback function user defined
);

/*****************************************************************************/
//  Description:    The function is used by client to unregister event to server
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC  uint32 SCI_UnregisterTaskMsg( // Return SCI_INVALID_ID if no such sever
                                  // else return SCI_SUCCESS
    uint32       server_ID,   // Server ID, must be SRV_ID_E type
    uint8        start_event, // Start number of event
    uint8        end_event,   // End number of event
	uint32       task_ID,     // ID of the task which unregister the event
    REG_CALLBACK fun          // Callback function user defined
);

/*****************************************************************************/
//  Description:    Check if the client has registe to the server.
//	Global resource dependence: 
//  Author:         Lin.Liu
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN SCI_IsClientRegisted( // Return FALSE if the server is not exist
                                     // or the client did not register any event
                                     // to the server, else return TRUE 
    uint32        server_ID,  // Server ID, must be the same with task id
    uint32        client_ID   // ID of the task which register the event
);


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif // End _SCI_SERVICE_H

