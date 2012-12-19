/******************************************************************************
 ** File Name:      emc_reg_v250a.h                                           *
 ** Author:         Johnny.Wang                                               *
 ** DATE:           2012-12-04                                                *
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    resiger address definition for emc controller for sc8825  *
 ******************************************************************************/
#include "sci_types.h"
#include "sc_reg.h"


/******************************************************************************
                          junior Macro define
******************************************************************************/
#define UL_ONEBITS 0xffffffff
#define UL_LEN 32
#define ONE 0x00000001


#define UMCTL_REG_BASE 0x60200000
#define PUBL_REG_BASE  0x60201000
#define EMC_MEM_BASE_ADDR 0x80000000
//#define UMCTL_REG_BASE 0x78000000
//#define PUBL_REG_BASE  0x78001000
//#define EMC_MEM_BASE_ADDR 0x70000000


//umctl/upctl registers declaration//{{{   	                                          			default value     
/////uMCTL Registers
#define UMCTL_CFG_PCFG_0 		(UMCTL_REG_BASE+0x400) // R/W Port 0 Configuration Register 		0x00000000
#define UMCTL_CFG_PCFG_1 		(UMCTL_REG_BASE+0x404) // R/W Port 1 Configuration Register 		0x00000000
#define UMCTL_CFG_PCFG_2 		(UMCTL_REG_BASE+0x408) // R/W Port 2 Configuration Register 		0x00000000
#define UMCTL_CFG_PCFG_3 		(UMCTL_REG_BASE+0x40C) // R/W Port 3 Configuration Register 		0x00000000
#define UMCTL_CFG_PCFG_4 		(UMCTL_REG_BASE+0x410) // R/W Port 4 Configuration Register 		0x00000000
#define UMCTL_CFG_PCFG_5 		(UMCTL_REG_BASE+0x414) // R/W Port 5 Configuration Register 		0x00000000
#define UMCTL_CFG_PCFG_6 		(UMCTL_REG_BASE+0x418) // R/W Port 6 Configuration Register 		0x00000000
#define UMCTL_CFG_PCFG_7 		(UMCTL_REG_BASE+0x41C) // R/W Port 7 Configuration Register 		0x00000000
#define UMCTL_CFG_CCFG			(UMCTL_REG_BASE+0x480) // R/W Controller Configuration Register 	0xf0000018   
#define UMCTL_CFG_DCFG_CS0  	(UMCTL_REG_BASE+0x484) // R/W DRAM Configuration Register 0			0x00000011         
#define UMCTL_CFG_CCSTAT		(UMCTL_REG_BASE+0x488) // R Controller Status Register 				0x00000000           
#define UMCTL_CFG_CCFG1		(UMCTL_REG_BASE+0x48C) // R/W Controller Configuration Register 1 	0x0ff320c8
#define UMCTL_CFG_DCFG_CS1  	(UMCTL_REG_BASE+0x494) // R/W DRAM Configuration Register 1			0x00000011         
/////uPCTL 
//Regiseters-Operational State, Control, and Status Registers                            	                                          
#define UMCTL_CFG_SCFG   		(UMCTL_REG_BASE+0x000) // R/W State Configuration Register 			0x00000f00
#define UMCTL_CFG_SCTL   		(UMCTL_REG_BASE+0x004) // R/W State Control Register 				0x00000000      
#define UMCTL_CFG_STAT   		(UMCTL_REG_BASE+0x008) // R State Status Register 					0x00000000         
#define UMCTL_CFG_INTRSTAT 	(UMCTL_REG_BASE+0x00C) // R Interrupt Status Register 				0x00000000 
//Initialization Control and Status Registers
#define UMCTL_CFG_MCMD          (UMCTL_REG_BASE+0x040) // R/W Memory Command Register 				0x00100000                                           
#define UMCTL_CFG_POWCTL        (UMCTL_REG_BASE+0x044) // R/WSC Power Up Control Register 			0x00000000              
#define UMCTL_CFG_POWSTAT       (UMCTL_REG_BASE+0x048) // R Power Up Status Register 				0x00000000                  
#define UMCTL_CFG_CMDTSTAT      (UMCTL_REG_BASE+0x04C) // R Command Timing Status Register 			0x00000000
#define UMCTL_CFG_CMDTSTATEN    (UMCTL_REG_BASE+0x050) // R/W Command Timing Status Enable Register 0x00000000                                                      
#define UMCTL_REG_MRRCFG0       (UMCTL_REG_BASE+0x060) // R/W MRR Configuration 0 Register 			0x00000000
#define UMCTL_REG_MRRSTAT0      (UMCTL_REG_BASE+0x064) // R/W MRR Status 0 Register 				0x00000000      
#define UMCTL_REG_MRRSTAT1      (UMCTL_REG_BASE+0x068) // R/W MRR Status 1Register 					0x00000000       
//Memory Control and Status Registers                                                      
#define UMCTL_CFG_MCFG1         (UMCTL_REG_BASE+0x07C) // R/W Memory Configuration 1 Register 		0x00000000       
#define UMCTL_CFG_MCFG          (UMCTL_REG_BASE+0x080) // R/W Memory Configuration Register 		0x00040000          
#define UMCTL_CFG_PPCFG         (UMCTL_REG_BASE+0x084) // R/W Partially Populated Memories Configuration Register 0x00000000                                                                        
#define UMCTL_CFG_MSTAT         (UMCTL_REG_BASE+0x088) // R Memory Status Register 					0x00000000                                                                                   
#define UMCTL_CFG_LPDDR2ZQCFG   (UMCTL_REG_BASE+0x08C) // R/W LPDDR2 ZQ Configuration Register 		0xAB0A560A
//DTU Control and Status Registers                                                                                                            
#define UMCTL_CFG_DTUPDES 		(UMCTL_REG_BASE+0x094) // R DTU Status 0x00000000                          
#define UMCTL_CFG_DTUNA   		(UMCTL_REG_BASE+0x098) // R DTU Number of Random Addresses Created 0x00000000
#define UMCTL_CFG_DTUNE   		(UMCTL_REG_BASE+0x09C) // R DTU Number of Errors 0x00000000                  
#define UMCTL_CFG_DTUPRD0 		(UMCTL_REG_BASE+0X0A0) // R DTU Parallel Read 0 0x00000000                 
#define UMCTL_CFG_DTUPRD1 		(UMCTL_REG_BASE+0x0A4) // R DTU Parallel Read 1 0x00000000                 
#define UMCTL_CFG_DTUPRD2 		(UMCTL_REG_BASE+0x0A8) // R DTU Parallel Read 2 0x00000000                 
#define UMCTL_CFG_DTUPRD3 		(UMCTL_REG_BASE+0x0AC) // R DTU Parallel Read 3 0x00000000                 
#define UMCTL_CFG_DTUAWDT 		(UMCTL_REG_BASE+0x0B0) // R/W DTU Address Width Configuration   
#define UMCTL_CFG_DTUPDES 		(UMCTL_REG_BASE+0x094) // R DTU Status 0x00000000                          
#define UMCTL_CFG_DTUNA   		(UMCTL_REG_BASE+0x098) // R DTU Number of Random Addresses Created 0x00000000
#define UMCTL_CFG_DTUNE   		(UMCTL_REG_BASE+0x09C) // R DTU Number of Errors 0x00000000                  
#define UMCTL_CFG_DTUPRD0 		(UMCTL_REG_BASE+0X0A0) // R DTU Parallel Read 0 0x00000000                 
#define UMCTL_CFG_DTUPRD1 		(UMCTL_REG_BASE+0x0A4) // R DTU Parallel Read 1 0x00000000                 
#define UMCTL_CFG_DTUPRD2 		(UMCTL_REG_BASE+0x0A8) // R DTU Parallel Read 2 0x00000000                 
#define UMCTL_CFG_DTUPRD3 		(UMCTL_REG_BASE+0x0AC) // R DTU Parallel Read 3 0x00000000                 
#define UMCTL_CFG_DTUAWDT 		(UMCTL_REG_BASE+0x0B0) // R/W DTU Address Width Configuration Dependent                                                              
         
//Memory Timing Registers
#define UMCTL_CFG_TOGCNT1U     	(UMCTL_REG_BASE+0x0c0) // R/W Toggle Counter 1U Register 0x00000064    
#define UMCTL_CFG_TINIT        	(UMCTL_REG_BASE+0x0c4) // R/W t_init Timing Register 0x000000C8           
#define UMCTL_CFG_TRSTH        	(UMCTL_REG_BASE+0x0c8) // R/W Reset High Time Register 0x00000000         
#define UMCTL_CFG_TOGCNT100N   	(UMCTL_REG_BASE+0x0cc) // R/W Toggle Counter 100N Register 0x00000001
#define UMCTL_CFG_TREFI        	(UMCTL_REG_BASE+0x0d0) // R/W t_refi Timing Register 0x00000001           
#define UMCTL_CFG_TMRD         	(UMCTL_REG_BASE+0x0d4) // R/W t_mrd Timing Register 0x00000001             
#define UMCTL_CFG_TRFC         	(UMCTL_REG_BASE+0x0d8) // R/W t_rfc Timing Register 0x00000001             
#define UMCTL_CFG_TRP          	(UMCTL_REG_BASE+0x0dc) // R/W t_rp Timing Register 0x00000006               
#define UMCTL_CFG_TRTW         	(UMCTL_REG_BASE+0x0e0) // R/W t_rtw 0x00000002                             
#define UMCTL_CFG_TAL          	(UMCTL_REG_BASE+0x0e4) // R/W AL Latency Register 0x00000000                
#define UMCTL_CFG_TCL          	(UMCTL_REG_BASE+0x0e8) // R/W CL Timing Register 0x00000004                 
#define UMCTL_CFG_TCWL         	(UMCTL_REG_BASE+0x0ec) // R/W CWL Register 0x00000003                      
#define UMCTL_CFG_TRAS         	(UMCTL_REG_BASE+0x0f0) // R/W t_ras Timing Register 0x00000010             
#define UMCTL_CFG_TRC          	(UMCTL_REG_BASE+0x0f4) // R/W t_rc Timing Register 0x00000016       
#define UMCTL_CFG_TRCD         	(UMCTL_REG_BASE+0x0f8) // R/W t_rcd Timing Register 0x00000006     
#define UMCTL_CFG_TRRD         	(UMCTL_REG_BASE+0x0fc) // R/W t_rrd Timing Register 0x00000004     
#define UMCTL_CFG_TRTP         	(UMCTL_REG_BASE+0x100) // R/W t_rtp Timing Register 0x00000003     
#define UMCTL_CFG_TWR          	(UMCTL_REG_BASE+0x104) // R/W t_wr Timing Register 0x00000006       
#define UMCTL_CFG_TWTR         	(UMCTL_REG_BASE+0x108) // R/W t_wtr Timing Register 0x00000004     
#define UMCTL_CFG_TEXSR        	(UMCTL_REG_BASE+0x10c) // R/W t_exsr Timing Register 0x00000001   
#define UMCTL_CFG_TXP          	(UMCTL_REG_BASE+0x110) // R/W t_xp Timing Register 0x00000001       
#define UMCTL_CFG_TXPDLL       	(UMCTL_REG_BASE+0x114) // R/W t_xpdll Timing Register 0x00000000 
#define UMCTL_CFG_TZQCS        	(UMCTL_REG_BASE+0x118) // R/W t_zqcs Timing Register 0x00000000   
#define UMCTL_CFG_TZQCSI       	(UMCTL_REG_BASE+0x11C) // R/W t_zqcsi Timing Register 0x00000000 
#define UMCTL_CFG_TDQS         	(UMCTL_REG_BASE+0x120) // R/W t_dqs Timing Register 0x00000001     
#define UMCTL_CFG_TCKSRE       	(UMCTL_REG_BASE+0x124) // R/W t_cksre Timing Register 0x00000000 
#define UMCTL_CFG_TCKSRX       	(UMCTL_REG_BASE+0x128) // R/W t_cksrx Timing Register 0x00000000 
#define UMCTL_CFG_TCKE         	(UMCTL_REG_BASE+0x12c) // R/W t_cke Timing Register 0x00000003     
#define UMCTL_CFG_TMOD         	(UMCTL_REG_BASE+0x130) // R/W t_mod Timing Register 0x00000000     
#define UMCTL_CFG_TRSTL        	(UMCTL_REG_BASE+0x134) // R/W Reset Low Timing Register 0x00000000
#define UMCTL_CFG_TZQCL        	(UMCTL_REG_BASE+0x138) // R/W t_zqcl Timing Register 0x00000000   
#define UMCTL_CFG_TMRR         	(UMCTL_REG_BASE+0x13C) // R/W t_mrr Timing Register 0x00000000     
#define UMCTL_CFG_TCKESR       	(UMCTL_REG_BASE+0x140) // R/W t_ckesr Timing Register 0x00000004 
#define UMCTL_CFG_TDPD         	(UMCTL_REG_BASE+0x144) // R/W t_dpd Timing Register 0x00000000     
//ECC Configuration, Control, and Status Registers
#define UMCTL_CFG_ECCCFG 		(UMCTL_REG_BASE+0x180) // R/W ECC Configuration Register 0x00000000
#define UMCTL_CFG_ECCTST 		(UMCTL_REG_BASE+0x184) // R/W ECC Test Register 0x00000000         
#define UMCTL_CFG_ECCCLR 		(UMCTL_REG_BASE+0x188) // R/WSC ECC Clear Register 0x00000000      
#define UMCTL_CFG_ECCLOG 		(UMCTL_REG_BASE+0x18C) // R ECC Log Register 0x00000000            

//DFI Control Registers                                                               
#define UMCTL_CFG_DFITCTRLDELAY (UMCTL_REG_BASE+0x240) // R/W DFI tctrl_delay Register 0x00000002                 
#define UMCTL_CFG_DFIODTCFG 	(UMCTL_REG_BASE+0x244) // R/W DFI ODT Configuration Register 0x00000000               
#define UMCTL_CFG_DFIODTCFG1 	(UMCTL_REG_BASE+0x248) // R/W DFI ODT Timing Configuration 1 Register 0x06060000     
#define UMCTL_CFG_DFIODTRANKMAP (UMCTL_REG_BASE+0x24C) // R/W DFI ODT Rank Mapping Register Configurationdependent
//DFI Write Data Registers                                                            
#define UMCTL_CFG_DFITPHYWRDATA (UMCTL_REG_BASE+0x250) // R/W DFI tphy_wrdata Register 0x00000001                 
#define UMCTL_CFG_DFITPHYWRLAT  (UMCTL_REG_BASE+0x254) // R/W DFI tphy_wrlat Register 0x00000001                   
//DFI Read Data Registers                                                             
#define UMCTL_CFG_DFITRDDATAEN  (UMCTL_REG_BASE+0x260) // R/W DFI trddata_en Register 0x00000001                   
#define UMCTL_CFG_DFITPHYRDLAT  (UMCTL_REG_BASE+0x264) // R/W DFI tphy_rddata Register 0x0000000F                  
//DFI Update Registers
#define UMCTL_CFG_DFITPHYUPDTYPE0 	(UMCTL_REG_BASE+0x270) // R/W DFI tphyupd_type0 Register 0x00000010      
#define UMCTL_CFG_DFITPHYUPDTYPE1 	(UMCTL_REG_BASE+0x274) // R/W DFI tphyupd_type1 Register 0x00000010      
#define UMCTL_CFG_DFITPHYUPDTYPE2 	(UMCTL_REG_BASE+0x278) // R/W DFI tphyupd_type2 Register 0x00000010      
#define UMCTL_CFG_DFITPHYUPDTYPE3 	(UMCTL_REG_BASE+0x27C) // R/W DFI tphyupd_type3 Register 0x00000010      
#define UMCTL_CFG_DFITCTRLUPDMIN  	(UMCTL_REG_BASE+0x280) // R/W DFI tctrlupd_min Register 0x00000010        
#define UMCTL_CFG_DFITCTRLUPDMAX  	(UMCTL_REG_BASE+0x284) // R/W DFI tctrlupd_max Register 0x00000040        
#define UMCTL_CFG_DFITCTRLUPDDLY  	(UMCTL_REG_BASE+0x288) // R/W DFI tctrlupd_dly Register 0x00000008        
#define UMCTL_CFG_DFIUPDCFG 	  	(UMCTL_REG_BASE+0x290) // R/W DFI Update Configuration Register 0x00000003     
#define UMCTL_CFG_DFITREFMSKI     	(UMCTL_REG_BASE+0x294) // R/W DFI Masked Refresh Interval Register 0x00000000
#define UMCTL_CFG_DFITCTRLUPDI    	(UMCTL_REG_BASE+0x298) // R/W DFI tctrlupd_interval Register 0x00000000     
//DFI Training Registers
#define UMCTL_CFG_DFITRCFG0 		(UMCTL_REG_BASE+0x2AC) // R/W DFI Training Configuration 0 Register 0x00000000         
#define UMCTL_CFG_DFITRSTAT0 		(UMCTL_REG_BASE+0x2B0) // R DFI Training Status 0 Register 0x00000000                 
#define UMCTL_CFG_DFITRWRLVLEN 		(UMCTL_REG_BASE+0x2B4) // R/W DFI Training dfi_wrlvl_en Register 0x00000000         
#define UMCTL_CFG_DFITRRDLVLEN 		(UMCTL_REG_BASE+0x2B8) // R/W DFI Training dfi_rdlvl_en Register 0x00000000         
#define UMCTL_CFG_DFITRRDLVLGATEEN 	(UMCTL_REG_BASE+0x2BC) // R/W DFI Training dfi_rdlvl_gate_en Register 0x00000000
//DFI Status Registers
#define UMCTL_CFG_DFISTSTAT0 		(UMCTL_REG_BASE+0x2C0) // R DFI Status Status 0 Register 0x00000000        
#define UMCTL_CFG_DFISTCFG0 		(UMCTL_REG_BASE+0x2C4) // R/W DFI Status Configuration 0 Register 0x00000000
#define UMCTL_CFG_DFISTCFG1 		(UMCTL_REG_BASE+0x2C8) // R/W DFI Status Configuration 1Register 0x00000000 
#define UMCTL_CFG_DFITDRAMCLKEN 	(UMCTL_REG_BASE+0x2D0) // R/W DFI tdram_clk_enable Register 0x00000002   
#define UMCTL_CFG_DFITDRAMCLKDIS 	(UMCTL_REG_BASE+0x2D4) // R/W DFI tdram_clk_disable Register 0x00000002 
#define UMCTL_CFG_DFISTCFG2 		(UMCTL_REG_BASE+0x2D8) // R/W DFI Status Configuration 2 Register 0x00000000 
#define UMCTL_CFG_DFISTPARCLR 		(UMCTL_REG_BASE+0x2DC) // R/WSC DFI Status Parity Clear Register 0x00000000
#define UMCTL_CFG_DFISTPARLOG 		(UMCTL_REG_BASE+0x2E0) // R DFI Status Parity Log Register 0x00000000      
//DFI Low Power Registers
#define UMCTL_CFG_DFILPCFG0 		(UMCTL_REG_BASE+0x2F0) // R/W DFI Low Power Configuration 0 Register 0x00070000
//DFI Training 2 Registers
#define UMCTL_CFG_DFITRWRLVLRESP0 	(UMCTL_REG_BASE+0x300) // R DFI Training dfi_wrlvl_resp Status 0 0x00000000          
#define UMCTL_CFG_DFITRWRLVLRESP1 	(UMCTL_REG_BASE+0x304) // R DFI Training dfi_wrlvl_resp Status 1 0x00000000          
#define UMCTL_CFG_DFITRWRLVLRESP2 	(UMCTL_REG_BASE+0x308) // R DFI Training dfi_wrlvl_resp Status 2 0x00000000          
#define UMCTL_CFG_DFITRRDLVLRESP0 	(UMCTL_REG_BASE+0x30C) // R DFI Training dfi_rdlvl_resp Status 0 0x00000000          
#define UMCTL_CFG_DFITRRDLVLRESP1 	(UMCTL_REG_BASE+0x310) // R DFI Training dfi_rdlvl_resp Status 1 0x00000000          
#define UMCTL_CFG_DFITRRDLVLRESP2 	(UMCTL_REG_BASE+0x314) // R DFI Training dfi_rdlvl_resp Status 2 0x00000000          
#define UMCTL_CFG_DFITRWRLVLDELAY0 	(UMCTL_REG_BASE+0x318) // R/W DFI Training dfi_wrlvl_delay Configuration0 0x00000000
#define UMCTL_CFG_DFITRWRLVLDELAY1 	(UMCTL_REG_BASE+0x31C) // R/W DFI Training dfi_wrlvl_delay Configuration1 0x00000000
#define UMCTL_CFG_DFITRWRLVLDELAY2 	(UMCTL_REG_BASE+0x320) // R/W DFI Training dfi_wrlvl_delay Configuration2 0x00000000
#define UMCTL_CFG_DFITRRDLVLDELAY0 	(UMCTL_REG_BASE+0x324) // R/W DFI Training dfi_rdlvl_delay Configuration0 0x00000000
#define UMCTL_CFG_DFITRRDLVLDELAY1 	(UMCTL_REG_BASE+0x328) // R/W DFI Training dfi_rdlvl_delay Configuration1 0x00000000
#define UMCTL_CFG_DFITRRDLVLDELAY2 	(UMCTL_REG_BASE+0x32C) // R/W DFI Training dfi_rdlvl_delay Configuration2 0x00000000
#define UMCTL_CFG_DFITRRDLVLGATEDELAY0 (UMCTL_REG_BASE+0x330) // R/W DFI Training dfi_rdlvl_gate_delay Configuration 0 0x00000000                                                                               
#define UMCTL_CFG_DFITRRDLVLGATEDELAY1 (UMCTL_REG_BASE+0x334) // R/W DFI Training dfi_rdlvl_gate_delay Configuration 1 0x00000000                                                                               
#define UMCTL_CFG_DFITRRDLVLGATEDELAY2 (UMCTL_REG_BASE+0x338) // R/W DFI Training dfi_rdlvl_gate_delay Configuration 2 0x00000000                                                                               
#define UMCTL_CFG_DFITRCMD 			(UMCTL_REG_BASE+0x33C) // R/W DFI Training Command Register 0x00000000                                                              
//IP Status Registers
#define UMCTL_CFG_IPVR 			(UMCTL_REG_BASE+0x3F8) // R IP Version Register Refer to the DWC DDR uPCTL Release Notes 
#define UMCTL_CFG_IPTR 			(UMCTL_REG_BASE+0x3FC) // R IP Type Register 0x44574300
/////////////PUBL CFG
#define PUBL_CFG_RIDR           (PUBL_REG_BASE+0x00*4) // R   - Revision Identification Register
#define PUBL_CFG_PIR            (PUBL_REG_BASE+0x01*4) // R/W - PHY Initialization Register
#define PUBL_CFG_PGCR           (PUBL_REG_BASE+0x02*4) // R/W - PHY General Configuration Register
#define PUBL_CFG_PGSR           (PUBL_REG_BASE+0x03*4) // R   - PHY General Status Register                                     
#define PUBL_CFG_DLLGCR         (PUBL_REG_BASE+0x04*4) // R/W - DLL General Control Register
#define PUBL_CFG_ACDLLCR        (PUBL_REG_BASE+0x05*4) // R/W - AC DLL Control Register
#define PUBL_CFG_PTR0           (PUBL_REG_BASE+0x06*4) // R/W - PHY Timing Register 0
#define PUBL_CFG_PTR1           (PUBL_REG_BASE+0x07*4) // R/W - PHY Timing Register 1
#define PUBL_CFG_PTR2           (PUBL_REG_BASE+0x08*4) // R/W - PHY Timing Register 2
#define PUBL_CFG_ACIOCR         (PUBL_REG_BASE+0x09*4) // R/W - AC I/O Configuration Register                                
#define PUBL_CFG_DXCCR          (PUBL_REG_BASE+0x0A*4) // R/W - DATX8 I/O Configuration Register
#define PUBL_CFG_DSGCR          (PUBL_REG_BASE+0x0B*4) // R/W - DFI Configuration Register
#define PUBL_CFG_DCR            (PUBL_REG_BASE+0x0C*4) // R/W - DRAM Configuration Register
#define PUBL_CFG_DTPR0          (PUBL_REG_BASE+0x0D*4) // R/W - SDRAM Timing Parameters Register 0
#define PUBL_CFG_DTPR1          (PUBL_REG_BASE+0x0E*4) // R/W - SDRAM Timing Parameters Register 1
#define PUBL_CFG_DTPR2          (PUBL_REG_BASE+0x0F*4) // R/W - SDRAM Timing Parameters Register 2
#define PUBL_CFG_MR0            (PUBL_REG_BASE+0x10*4) // R/W - Mode Register
#define PUBL_CFG_MR1            (PUBL_REG_BASE+0x11*4) // R/W - Ext}ed Mode Register
#define PUBL_CFG_MR2            (PUBL_REG_BASE+0x12*4) // R/W - Ext}ed Mode Register 2
#define PUBL_CFG_MR3            (PUBL_REG_BASE+0x13*4) // R/W - Ext}ed Mode Register 3
#define PUBL_CFG_ODTCR          (PUBL_REG_BASE+0x14*4) // R/W - ODT Configuration Register
#define PUBL_CFG_DTAR           (PUBL_REG_BASE+0x15*4) // R/W - Data Training Address Register
#define PUBL_CFG_DTDR0          (PUBL_REG_BASE+0x16*4) // R/W - Data Training Data Register 0
#define PUBL_CFG_DTDR1          (PUBL_REG_BASE+0x17*4) // R/W - Data Training Data Register 1
#define PUBL_CFG_DCUAR			(PUBL_REG_BASE+0X30*4) // R/W - DCU Address Resiter
#define PUBL_CFG_DCUDR			(PUBL_REG_BASE+0x31*4) // R/W - DCU Data Register
#define PUBL_CFG_DCURR			(PUBL_REG_BASE+0x32*4) // R/W - DCU Run Register
#define PUBL_CFG_DCULR			(PUBL_REG_BASE+0x33*4) // R/W - DCU Loop Register
#define PUBL_CFG_DCUGCR 		(PUBL_REG_BASE+0x34*4) // R/W - DCU General Configuration Register
#define PUBL_CFG_DCUTPR 		(PUBL_REG_BASE+0x35*4) // R/W - DCU Timing Parameters Registers
#define PUBL_CFG_DCUSR0 		(PUBL_REG_BASE+0x36*4) // R   - DCU Status Register 0
#define PUBL_CFG_DCUSR1 		(PUBL_REG_BASE+0x37*4) // R   - DCU Status Register 1
#define PUBL_CFG_BISTRR 		(PUBL_REG_BASE+0x40*4) // R/W - BIST Run Register
#define PUBL_CFG_BISTMSKR0  	(PUBL_REG_BASE+0x41*4) // R/W - BIST Mask Register 0
#define PUBL_CFG_BISTMSKR1  	(PUBL_REG_BASE+0x42*4) // R/W - BIST Mask Register 1
#define PUBL_CFG_BISTWCR 		(PUBL_REG_BASE+0x43*4) // R/W - BIST Word Count Register
#define PUBL_CFG_BISTLSR 		(PUBL_REG_BASE+0x44*4) // R/W - BIST LFSR Seed Register
#define PUBL_CFG_BISTAR0 		(PUBL_REG_BASE+0x45*4) // R/W - BIST Address Register 0
#define PUBL_CFG_BISTAR1 		(PUBL_REG_BASE+0x46*4) // R/W - BIST Address Register 1
#define PUBL_CFG_BISTAR2 		(PUBL_REG_BASE+0x47*4) // R/W - BIST Address Register 2
#define PUBL_CFG_BISTUDPR 		(PUBL_REG_BASE+0x48*4) // R/W - BIST User Data Pattern Register
#define PUBL_CFG_BISTGSR 		(PUBL_REG_BASE+0x49*4) // R   - BIST General Status Register
#define PUBL_CFG_BISTWER 		(PUBL_REG_BASE+0x4A*4) // R   - BIST Word Error Register
#define PUBL_CFG_BISTBER0 		(PUBL_REG_BASE+0x4B*4) // R   - BIST Bit Error Register 0
#define PUBL_CFG_BISTBER1 		(PUBL_REG_BASE+0x4C*4) // R   - BIST Bit Error Register 1
#define PUBL_CFG_BISTBER2 		(PUBL_REG_BASE+0x4D*4) // R   - BIST Bit Error Register 2
#define PUBL_CFG_BISTWCSR 		(PUBL_REG_BASE+0x4E*4) // R   - BIST Word Count Status Register
#define PUBL_CFG_BISTFWR0 		(PUBL_REG_BASE+0x4F*4) // R   - BIST Fail Word Register 0
#define PUBL_CFG_BISTFWR1 		(PUBL_REG_BASE+0x50*4) // R   - BIST Fail Word Register 1
#define PUBL_CFG_ZQ0CR0 		(PUBL_REG_BASE+0x60*4) // R/W - ZQ 0 Impedance Control Register 0
#define PUBL_CFG_ZQ0CR1 		(PUBL_REG_BASE+0x61*4) // R/W - ZQ 0 Impedance Control Register 1
#define PUBL_CFG_ZQ0SR0 		(PUBL_REG_BASE+0x62*4) // R   - ZQ 0 Impedance Status Register 0
#define PUBL_CFG_ZQ0SR1 		(PUBL_REG_BASE+0x63*4) // R   - ZQ 0 Impedance Status Register 1
#define PUBL_CFG_ZQ1CR0 		(PUBL_REG_BASE+0x64*4) // R/W - ZQ 1 Impedance Control Register 0
#define PUBL_CFG_ZQ1CR1 		(PUBL_REG_BASE+0x65*4) // R/W - ZQ 1 Impedance Control Register 1
#define PUBL_CFG_ZQ1SR0 		(PUBL_REG_BASE+0x66*4) // R   - ZQ 1 Impedance Status Register 0
#define PUBL_CFG_ZQ1SR1 		(PUBL_REG_BASE+0x67*4) // R   - ZQ 1 Impedance Status Register 1
#define PUBL_CFG_ZQ2CR0 		(PUBL_REG_BASE+0x68*4) // R/W - ZQ 2 Impedance Control Register 0
#define PUBL_CFG_ZQ2CR1 		(PUBL_REG_BASE+0x69*4) // R/W - ZQ 2 Impedance Control Register 1
#define PUBL_CFG_ZQ2SR0 		(PUBL_REG_BASE+0x6A*4) // R   - ZQ 2 Impedance Status Register 0
#define PUBL_CFG_ZQ2SR1 		(PUBL_REG_BASE+0x6B*4) // R   - ZQ 2 Impedance Status Register 1
#define PUBL_CFG_ZQ3CR0 		(PUBL_REG_BASE+0x6C*4) // R/W - ZQ 3 Impedance Control Register 0
#define PUBL_CFG_ZQ3CR1 		(PUBL_REG_BASE+0x6D*4) // R/W - ZQ 3 Impedance Control Register 1
#define PUBL_CFG_ZQ3SR0 		(PUBL_REG_BASE+0x6E*4) // R   - ZQ 3 Impedance Status Register 0
#define PUBL_CFG_ZQ3SR1 		(PUBL_REG_BASE+0x6F*4) // R   - ZQ 3 Impedance Status Register 1
#define PUBL_CFG_DX0GCR        	(PUBL_REG_BASE+0x70*4) // R/W - DATX8 0 General Configuration Register
#define PUBL_CFG_DX0GSR0       	(PUBL_REG_BASE+0x71*4) // R   - DATX8 0 General Status Register
#define PUBL_CFG_DX0GSR1       	(PUBL_REG_BASE+0x72*4) // R   - DATX8 0 General Status Register 1
#define PUBL_CFG_DX0DLLCR      	(PUBL_REG_BASE+0x73*4) // R   - DATX8 0 DLL Control Register
#define PUBL_CFG_DX0DQTR      	(PUBL_REG_BASE+0x74*4) // R/W - DATX8 0 DQ Timing Register
#define PUBL_CFG_DX0DQSTR      	(PUBL_REG_BASE+0x75*4) // R/W - DATX8 0 DQS Timing Register
#define PUBL_CFG_DX1GCR       	(PUBL_REG_BASE+0x80*4) // R   - DATX8 1 General Configration Register
#define PUBL_CFG_DX1GSR0       	(PUBL_REG_BASE+0x81*4) // R   - DATX8 1 General Status Register
#define PUBL_CFG_DX1GSR1       	(PUBL_REG_BASE+0x82*4) // R   - DATX8 1 General Status Register
#define PUBL_CFG_DX1DLLCR      	(PUBL_REG_BASE+0x83*4) // R   - DATX8 1 DLL Control Register
#define PUBL_CFG_DX1DQTR      	(PUBL_REG_BASE+0x84*4) // R/W - DATX8 1 DQ Timing Register
#define PUBL_CFG_DX1DQSTR      	(PUBL_REG_BASE+0x85*4) // R/W - DATX8 1 DQS Timing Register
#define PUBL_CFG_DX2GCR       	(PUBL_REG_BASE+0x90*4) // R   - DATX8 2 General Configration Register
#define PUBL_CFG_DX2GSR0       	(PUBL_REG_BASE+0x91*4) // R   - DATX8 2 General Status Register
#define PUBL_CFG_DX2GSR1       	(PUBL_REG_BASE+0x92*4) // R   - DATX8 2 General Status Register
#define PUBL_CFG_DX2DLLCR      	(PUBL_REG_BASE+0x93*4) // R   - DATX8 2 DLL Control Register
#define PUBL_CFG_DX2DQTR      	(PUBL_REG_BASE+0x94*4) // R/W - DATX8 2 DQ Timing Register
#define PUBL_CFG_DX2DQSTR      	(PUBL_REG_BASE+0x95*4) // R/W - DATX8 2 DQS Timing Register
#define PUBL_CFG_DX3GCR       	(PUBL_REG_BASE+0xA0*4) // R   - DATX8 3 General Configration Register
#define PUBL_CFG_DX3GSR0       	(PUBL_REG_BASE+0xA1*4) // R   - DATX8 3 General Status Register
#define PUBL_CFG_DX3GSR1       	(PUBL_REG_BASE+0xA2*4) // R   - DATX8 3 General Status Register
#define PUBL_CFG_DX3DLLCR      	(PUBL_REG_BASE+0xA3*4) // R   - DATX8 3 DLL Control Register
#define PUBL_CFG_DX3DQTR      	(PUBL_REG_BASE+0xA4*4) // R/W - DATX8 3 DQ Timing Register
#define PUBL_CFG_DX3DQSTR      	(PUBL_REG_BASE+0xA5*4) // R/W - DATX8 3 DQS Timing Register
#define PUBL_CFG_DX4GCR       	(PUBL_REG_BASE+0xB0*4) // R   - DATX8 4 General Configration Register
#define PUBL_CFG_DX4GSR0       	(PUBL_REG_BASE+0xB1*4) // R   - DATX8 4 General Status Register      
#define PUBL_CFG_DX4GSR1       	(PUBL_REG_BASE+0xB2*4) // R   - DATX8 4 General Status Register      
#define PUBL_CFG_DX4DLLCR      	(PUBL_REG_BASE+0xB3*4) // R   - DATX8 4 DLL Control Register         
#define PUBL_CFG_DX4DQTR      	(PUBL_REG_BASE+0xB4*4) // R/W - DATX8 4 DQ Timing Register           
#define PUBL_CFG_DX4DQSTR      	(PUBL_REG_BASE+0xB5*4) // R/W - DATX8 4 DQS Timing Register                                                   
#define PUBL_CFG_DX5GCR       	(PUBL_REG_BASE+0xC0*4) // R   - DATX8 5 General Configration Register
#define PUBL_CFG_DX5GSR0       	(PUBL_REG_BASE+0xC1*4) // R   - DATX8 5 General Status Register      
#define PUBL_CFG_DX5GSR1       	(PUBL_REG_BASE+0xC2*4) // R   - DATX8 5 General Status Register      
#define PUBL_CFG_DX5DLLCR      	(PUBL_REG_BASE+0xC3*4) // R   - DATX8 5 DLL Control Register         
#define PUBL_CFG_DX5DQTR      	(PUBL_REG_BASE+0xC4*4) // R/W - DATX8 5 DQ Timing Register           
#define PUBL_CFG_DX5DQSTR      	(PUBL_REG_BASE+0xC5*4) // R/W - DATX8 5 DQS Timing Register                                                   
#define PUBL_CFG_DX6GCR       	(PUBL_REG_BASE+0xD0*4) // R   - DATX8 6 General Configration Register
#define PUBL_CFG_DX6GSR0       	(PUBL_REG_BASE+0xD1*4) // R   - DATX8 6 General Status Register      
#define PUBL_CFG_DX6GSR1       	(PUBL_REG_BASE+0xD2*4) // R   - DATX8 6 General Status Register      
#define PUBL_CFG_DX6DLLCR      	(PUBL_REG_BASE+0xD3*4) // R   - DATX8 6 DLL Control Register         
#define PUBL_CFG_DX6DQTR      	(PUBL_REG_BASE+0xD4*4) // R/W - DATX8 6 DQ Timing Register           
#define PUBL_CFG_DX6DQSTR      	(PUBL_REG_BASE+0xD5*4) // R/W - DATX8 6 DQS Timing Register                    
#define PUBL_CFG_DX7GCR       	(PUBL_REG_BASE+0xE0*4) // R   - DATX8 7 General Configration Register
#define PUBL_CFG_DX7GSR0       	(PUBL_REG_BASE+0xE1*4) // R   - DATX8 7 General Status Register      
#define PUBL_CFG_DX7GSR1       	(PUBL_REG_BASE+0xE2*4) // R   - DATX8 7 General Status Register      
#define PUBL_CFG_DX7DLLCR      	(PUBL_REG_BASE+0xE3*4) // R   - DATX8 7 DLL Control Register         
#define PUBL_CFG_DX7DQTR      	(PUBL_REG_BASE+0xE4*4) // R/W - DATX8 7 DQ Timing Register           
#define PUBL_CFG_DX7DQSTR      	(PUBL_REG_BASE+0xE5*4) // R/W - DATX8 7 DQS Timing Register                                                   
#define PUBL_CFG_DX8GCR       	(PUBL_REG_BASE+0xF0*4) // R   - DATX8 8 General Configration Register
#define PUBL_CFG_DX8GSR0       	(PUBL_REG_BASE+0xF1*4) // R   - DATX8 8 General Status Register      
#define PUBL_CFG_DX8GSR1       	(PUBL_REG_BASE+0xF2*4) // R   - DATX8 8 General Status Register      
#define PUBL_CFG_DX8DLLCR      	(PUBL_REG_BASE+0xF3*4) // R   - DATX8 8 DLL Control Register         
#define PUBL_CFG_DX8DQTR      	(PUBL_REG_BASE+0xF4*4) // R/W - DATX8 8 DQ Timing Register           
#define PUBL_CFG_DX8DQSTR      	(PUBL_REG_BASE+0xF5*4) // R/W - DATX8 8 DQS Timing Register       


#define CTL_CURRENT_STATE	(REG32(UMCTL_CFG_STAT)&0X7)
#define PHY_CURRENT_STATE 	(REG32(PUBL_CFG_PGSR)&0XFF)


