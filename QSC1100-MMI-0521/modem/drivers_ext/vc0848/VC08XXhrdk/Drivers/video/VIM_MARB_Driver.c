
/*******************************************************************************
 *  All rights reserved, Copyright (C) vimicro LIMITED 2007                                                     
 * --------------------------------------------------------------------------------------------------------------
 * [File Name]          : VIM_MARB_Driver.c
 * [Description]        : ������578b.��marb��صĺ���   
 *				
 * [Author]                 : angela	
 * [Date Of Creation]  : 2007-03-19 16:01
 * [Platform]               : ƽ̨���
 * [Note]                 : 
 *
 * Modification History : 
 * ------------------------------------------------------------------------------
 * Date        			Author     		Modifications
 * ------------------------------------------------------------------------------
 * 2007-03-19 		angela      		 Created
 * 2007-05-21 		angela      		 add the tv and mpeg4 malloc
 *******************************************************************************/
/*******************************************************************************
 * This source code has been made available to you by VIMICRO on an
 * AS-IS basis. Anyone receiving this source code is licensed under VIMICRO
 * copyrights to use it in any way he or she deems fit, including copying it,
 * modifying it, compiling it, and redistributing it either with or without
 * modifications. Any person who transfers this source code or any derivative
 * work must include the VIMICRO copyright notice and this paragraph in
 * the transferred software.
 *******************************************************************************/
#include"VIM_COMMON.h"
#include "err.h"

VIM_MARB_RAM_INFO g_Marb_Sram_Info,g_Marb_SDram_Info;

/********************************************************************************
  Description:
  	logical address to physical address convert
  Parameters:
  	Log_addr: 
  		input logical address
  	pPhy_addr:
  		output physical address
  Returns:
  	void
  Remarks:
*********************************************************************************/
void VIM_MARB_RamInfo_Log2Phy(UINT32 Log_addr, UINT32* pPhy_addr)
{
	if(Log_addr>=0x20000000)
		*pPhy_addr = Log_addr;
	else
		*pPhy_addr = Log_addr|REMAP_PHY_ADDR;
}
/********************************************************************************
  Description:
  	physical address to logical address convert
  Parameters:
  	Phy_addr: 
  		input physical address
  	pLog_addr:
  		output logical address
  Returns:
  	void
  Remarks:
*********************************************************************************/
void VIM_MARB_RamInfo_Phy2Log(UINT32 Phy_addr, UINT32* pLog_addr)
{
	if((Phy_addr&REMAP_PHY_ADDR)!=0)
		*pLog_addr = Phy_addr&(~REMAP_PHY_ADDR);
	else
		*pLog_addr = Phy_addr;
}

/********************************************************************************
  Description:
  	marb memory allocation info initial
  Parameters:
  	sram_addr:	sram buffer start address
  	sram_len:	sram buffer len
  	sdram_addr:	sdram buffer start address
  	sdram_len:	sdram buffer len
  Returns:
  	void
  Remarks:
*********************************************************************************/
void VIM_MARB_RamInfo_Init(UINT32 sram_addr, UINT32 sram_len, UINT32 sdram_addr, UINT32 sdram_len)
{
	g_Marb_Sram_Info.Start_addr = sram_addr;
	g_Marb_Sram_Info.End_addr = g_Marb_Sram_Info.Start_addr+sram_len;
	g_Marb_Sram_Info.Top_Cursor = g_Marb_Sram_Info.End_addr;
	g_Marb_Sram_Info.Bottom_Cursor = g_Marb_Sram_Info.Start_addr;


	g_Marb_SDram_Info.Start_addr = sdram_addr;
	g_Marb_SDram_Info.End_addr = g_Marb_SDram_Info.Start_addr+sdram_len;
	g_Marb_SDram_Info.Top_Cursor = g_Marb_SDram_Info.End_addr;
	g_Marb_SDram_Info.Bottom_Cursor = g_Marb_SDram_Info.Start_addr;
}
/********************************************************************************
  Description:
  	reset allocate cursor, releas all allocated buffer
  Parameters:
  	flag:		buffer type
  			MARB_SRAM				
			MARB_SDRAM		
			
	direction: allocate direction
			MARB_FROM_TOP			
			MARB_FROM_BOTTOM		

  Returns:
  	void
  Remarks:
*********************************************************************************/
void VIM_MARB_RamInfo_Reset(UINT32 flag, UINT32 direction)
{
	if(flag==MARB_SRAM)
	{
		if(direction==MARB_FROM_TOP)
			g_Marb_Sram_Info.Top_Cursor = g_Marb_Sram_Info.End_addr;
		else
			g_Marb_Sram_Info.Bottom_Cursor = g_Marb_Sram_Info.Start_addr;
	}
	else
	{
		if(direction==MARB_FROM_TOP)
			g_Marb_SDram_Info.Top_Cursor = g_Marb_SDram_Info.End_addr;
		else
			g_Marb_SDram_Info.Bottom_Cursor = g_Marb_SDram_Info.Start_addr;
	}	
}

/********************************************************************************
  Description:
  	allocate memory from top to bottom direction
  Parameters:
  	flag:		buffer type
  			MARB_SRAM				
			MARB_SDRAM		
			
	len:		Allocate length

	addr_align:	start address align require ment 

	palloc_addr:	Allocated buffer start address			

  Returns:
  	error code
  Remarks:
*********************************************************************************/
UINT32 VIM_MARB_RamInfo_Alloc_FromTop(UINT32 flag, UINT32 len, UINT32 addr_align,UINT32* palloc_addr)
{
	UINT32 addr;

	*palloc_addr=NULL;	
	if(flag == MARB_SRAM)
	{
		addr = (g_Marb_Sram_Info.Top_Cursor-len)&(~(addr_align-1));
		if(g_Marb_Sram_Info.Bottom_Cursor>addr)
			return VIM_ERROR_MARB_RAMINFO_SRAM_NOSPACE;			
		
		*palloc_addr = addr;
		g_Marb_Sram_Info.Top_Cursor = addr;
		return VIM_SUCCEED;
	}
	if(flag == MARB_SDRAM)
	{
		addr = (g_Marb_SDram_Info.Top_Cursor-len)&(~(addr_align-1));
		if(g_Marb_SDram_Info.Bottom_Cursor>addr)
			return VIM_ERROR_MARB_RAMINFO_SDRAM_NOSPACE;			
		
		*palloc_addr = addr-V5_1TSRAM_BASE;
		g_Marb_SDram_Info.Top_Cursor = addr;
		return VIM_SUCCEED;
	}
	return VIM_ERROR_MARB_RAMINFO_TYPE;
}
/********************************************************************************
  Description:
  	allocate memory from bottom to top direction
  Parameters:
  	flag:		buffer type
  			MARB_SRAM				
			MARB_SDRAM		
			
	len:		Allocate length

	addr_align:	start address align require ment 

	palloc_addr:	Allocated buffer start address			

  Returns:
  	error code
  Remarks:
*********************************************************************************/
UINT32 VIM_MARB_RamInfo_Alloc_FromBottom(UINT32 flag, UINT32 len, UINT32 addr_align,UINT32* palloc_addr)
{
	UINT32 addr;

	*palloc_addr=NULL;	
	if(flag == MARB_SRAM)
	{
		addr = (g_Marb_Sram_Info.Bottom_Cursor+addr_align-1)&(~(addr_align-1));
		addr += len;
		if(g_Marb_Sram_Info.Top_Cursor<addr)
			return VIM_ERROR_MARB_RAMINFO_SRAM_NOSPACE;			
		
		*palloc_addr = addr;
		g_Marb_Sram_Info.Bottom_Cursor = addr;
		return VIM_SUCCEED;
	}
	if(flag == MARB_SDRAM)
	{
		addr = (g_Marb_SDram_Info.Bottom_Cursor+addr_align-1)&(~(addr_align-1));
		addr += len;
		if(g_Marb_SDram_Info.Top_Cursor<addr)
			return VIM_ERROR_MARB_RAMINFO_SDRAM_NOSPACE;			
		
		*palloc_addr = addr-V5_1TSRAM_BASE;
		g_Marb_SDram_Info.Bottom_Cursor= addr;
		return VIM_SUCCEED;
	}
	return VIM_ERROR_MARB_RAMINFO_TYPE;
}

/********************************************************************************
  Description:
  	get freesize in marb memory info
  Parameters:
  	flag:		buffer type
  			MARB_SRAM				
			MARB_SDRAM		
			
	pSize:	freesize

  Returns:
  	error code
  Remarks:
*********************************************************************************/
UINT32 VIM_MARB_RamInfo_GetFreesize(UINT32 flag, UINT32* pSize)
{
	*pSize=0;	
	if(flag == MARB_SRAM)
	{
		*pSize= g_Marb_Sram_Info.Top_Cursor -g_Marb_Sram_Info.Bottom_Cursor;
		return VIM_SUCCEED;
	}
	if(flag == MARB_SDRAM)
	{
		*pSize= g_Marb_SDram_Info.Top_Cursor -g_Marb_SDram_Info.Bottom_Cursor;
		return VIM_SUCCEED;
	}
	return VIM_ERROR_MARB_RAMINFO_TYPE;
}

/********************************************************************************
  Description:
  	get allocate cursor in marb memory info
  Parameters:
  	flag:		buffer type
  			MARB_SRAM				
			MARB_SDRAM		
			
	direction: allocate direction
			MARB_FROM_TOP			
			MARB_FROM_BOTTOM		

	pAddr:	cursor address			

  Returns:
  	error code
  Remarks:
*********************************************************************************/
UINT32 VIM_MARB_RamInfo_GetCursor(UINT32 flag, UINT32 direction, UINT32* pAddr)
{
	*pAddr=0;	
	if(flag == MARB_SRAM)
	{
		if(direction==MARB_FROM_TOP)
			*pAddr= g_Marb_Sram_Info.Top_Cursor;
		else
			*pAddr= g_Marb_Sram_Info.Bottom_Cursor;
		return VIM_SUCCEED;
	}
	if(flag == MARB_SDRAM)
	{
		if(direction==MARB_FROM_TOP)
			*pAddr= g_Marb_SDram_Info.Top_Cursor-V5_1TSRAM_BASE;
		else
			*pAddr= g_Marb_SDram_Info.Bottom_Cursor-V5_1TSRAM_BASE;
		return VIM_SUCCEED;
	}
	return VIM_ERROR_MARB_RAMINFO_TYPE;
}

///////////////////////////tools start ////////////////////////////////
static UINT32 VIM_MARB_GetBmemSize(VIM_DISP_BFORMAT fmt, TSize size)
{
	UINT32 len;

	len = (UINT32)size.cx & 0xffff;
	len *= (UINT32)size.cy & 0xffff;
	switch(fmt & 0xf)
	{
	case VIM_DISP_BLAYER_RGB1:
		len += 7;
		len >>= 3;
		break;
	case VIM_DISP_BLAYER_RGB2:
		len += 3;
		len >>= 2;
		break;
	case VIM_DISP_BLAYER_RGB4:
		len ++;
		len >>= 1;
		break;
	case VIM_DISP_BLAYER_RGB8:
		break;
	case VIM_DISP_BLAYER_RGB444:
		len *= 3;
		len += 1;
		len >>= 1;
		break;
	case VIM_DISP_BLAYER_RGB555:
	case VIM_DISP_BLAYER_RGB565:
		len <<= 1;
		break;
	case VIM_DISP_BLAYER_RGB666:
		len *= 18;
		len += 7;
		len >>= 3;
		break;
	case VIM_DISP_BLAYER_RGB24:
		len *= 3;
		break;
	case VIM_DISP_BLAYER_RGB32:
		len <<= 2;
		break;
	default:
		break;
	}
	return ((len + 3) >> 2) << 2;
}

static UINT32 VIM_MARB_GetAmemSize(VIM_DISP_BUFFERMODER fmt, TSize size)
{
	UINT32 len;

	len = (UINT32)(size.cx) & 0xffff;
	switch(fmt)
	{
	case VIM_DISP_LINEBUF:
		len <<= 5;
		break;
	case VIM_DISP_TWOFRAME:
		len *= (UINT32)size.cy & 0xffff;
		len <<= 2;
		break;
	case VIM_DISP_ONEFRAME:
		len *= (UINT32)size.cy & 0xffff;
		len += 3;
		len >>= 2;
		len <<= 3;
		break;
	case VIM_DISP_NODISP:
		len=0;
	default:
		break;
	}
	return len;
}
#if  0
static UINT32 VIM_MARB_GetEncLbufSize(VIM_JPEG_YUVMODE fmt, UINT16 width)
{
	TSize size;

	size.cy = 0;
	switch(fmt)
	{
	case JPEG_422:
		size.cx = ((width + 15) >> 4) << 4;
		size.cx *=4;
		size.cy = 8;
		break;
	case JPEG_411:
		size.cx = ((width + 31) >> 5) << 5;
		size.cx *=3;
		size.cy = 8;
		break;
	case JPEG_444:
		size.cx = ((width + 7) >> 3) << 3;
		size.cx *=4;
		size.cy = 8;
		break;
	case JPEG_400:
		size.cx = ((width + 7) >> 3) << 3;
		size.cx *=2;
		size.cy = 8;
		break;
	case JPEG_420:
		size.cx = ((width + 15) >> 4) << 4;
		size.cx *=3;
		size.cy = 8;
		break;
	default:
		break;
	}
	if(fmt == JPEG_444)
		fmt = JPEG_422;
	return (UINT32)(size.cy*size.cx);
}
#endif

static UINT32 VIM_MARB_GetDec0LbufSize(VIM_JPEG_YUVMODE fmt,VIM_JPEG_YUVTYPE type, UINT16 width)
{
	TSize size;

	size.cy = 0;
	switch(fmt)
	{
	case JPEG_422:
		size.cx = ((width + 15) >> 4) << 4;
		if(type!=VIM_JPEG_Y) // by lizg
			size.cx >>=1;
		size.cy = 8;
		break;
	case JPEG_411:
		size.cx = ((width + 31) >> 5) << 5;
		if(type==VIM_JPEG_Y)
		{
			//size.cx <<=1;
		}
		else
			size.cx >>=2;
		size.cy = 8;
		break;
	case JPEG_444:
		size.cx = ((width + 7) >> 3) << 3;
		if(type==VIM_JPEG_Y)
		{
			//size.cx <<=1;
		}
		else
			size.cx >>=1;
		size.cy = 8;
		break;
	case JPEG_400:
		size.cx = ((width + 7) >> 3) << 3;
		if(type==VIM_JPEG_Y)
		{
			//size.cx <<=1;
		}
		else
			size.cx=0;
		size.cy = 8;
		break;
	case JPEG_420:
		size.cx = ((width + 15) >> 4) << 4;
		if(type==VIM_JPEG_Y)
		{
			//size.cx <<=1;//angela 2007-5-21
			size.cy = 16;
		}
		else
		{
			size.cx >>=1;//angela 2007-5-21
			size.cy = 8;
		}
		break;
	default:
		break;
	}
	if(fmt == JPEG_444)
		fmt = JPEG_422;
	return (UINT32)(size.cy*size.cx);
}


static UINT32 VIM_MARB_GetDec1LbufSize(VIM_JPEG_YUVMODE fmt,VIM_JPEG_YUVTYPE type, UINT16 width)
{
	TSize size;
	size.cy =VIM_HIF_GetReg8(V5_REG_LBUF_BUF1_LINENUM);
	size.cx = width;

	switch(fmt)
	{
		case JPEG_422:
		case JPEG_411:
		case JPEG_444:
		case JPEG_400:
		case JPEG_420:
			 if(type==VIM_JPEG_Y) // by lizg
				size.cx <<=1;
			 break;
		default:
			 break;			
	}
	if(fmt == JPEG_444)
		fmt = JPEG_422;
	
	return (UINT32)(size.cy*size.cx);
}
//revised by youhai_mp4enc 2007.8.30	
#if (BOARD_TYPE==VC0820_FPGA)
#else
static UINT32 VIM_MARB_GetMpeg4LbufSize(VIM_JPEG_YUVMODE fmt,VIM_JPEG_YUVTYPE type, TSize Size)
{
UINT32 Len;
	switch(fmt)
	{
	case JPEG_422:
		break;
	case JPEG_411:
		break;
	case JPEG_444:
	case JPEG_400:
	case JPEG_420:
		if(type==VIM_JPEG_Y)
		{
			Len=Size.cx * 16;//0x3400;//
		}
		else
		{
			Len=Size.cx*Size.cy/4-Size.cx * 16;//0x4d00;//
		}
		break;
	default:
		break;
	}
	if(fmt == JPEG_444)
		fmt = JPEG_422;
	
	return (UINT32)(Len);
}
#endif
//////////////////////////tools  end///////////////////////////

/********************************************************************************
  Description:
	reset marb 's sub module
  Parameters:
	Module:
     	  //reset marb_biu module
	  VIM_MARB_SUB_RESET_BIU     = BIT0, 
   	  //reset marb_lbuf module
	  VIM_MARB_SUB_RESET_LBUF     = BIT1, 
   	  //reset marb_lcdc module
	  VIM_MARB_SUB_RESET_LCDC     = BIT2, 
  	  //reset marb_tvdc module
	  VIM_MARB_SUB_RESET_TVDC     = BIT3, 
 	  //reset marb_vdec module
	  VIM_MARB_SUB_RESET_VDEC     = BIT4, 
 	  //reset marb_venc module
	  VIM_MARB_SUB_RESET_VENC     = BIT5, 
	  //reset marb_jpeg module
	  VIM_MARB_SUB_RESET_JPEG     = BIT6, 
	  //reset marb_ipp module
	  VIM_MARB_SUB_RESET_IPP     = BIT7, 
	  //reset 1T-SRAM 0
	  VIM_MARB_SUB_RESET_1TSRAM0    = BIT8, 
	  //reset 1T-SRAM 1
	  VIM_MARB_SUB_RESET_1TSRAM1    = BIT9, 
	  //reset 1T-SRAM 2
	  VIM_MARB_SUB_RESET_1TSRAM2    = BIT10, 
	  //reset 1T-SRAM 2
	  VIM_MARB_SUB_RESET_1TSRAM3    = BIT11, 
	  //set ALL reset
	  VIM_MARB_RESET_ALL
  Returns:
  	void
  Remarks:
*********************************************************************************/

void VIM_MARB_ResetSubModule(VIM_MARB_SUB_RESET Module)
{
#if (BOARD_TYPE==VC0820_FPGA) //add by zhoujian
			UINT32 reset;
			reset=0x75a00000|Module;
			VIM_HIF_SetReg32(V5_REG_MARB_RESET,  reset);
			VIM_USER_DelayMs(2);
			reset=0x75a00000;
			VIM_HIF_SetReg32(V5_REG_MARB_RESET,  reset);
#else
			VIM_HIF_SetReg32(V5_REG_MARB_RESET,  Module);
			VIM_USER_DelayMs(2); 
			if(Module>VIM_MARB_SUB_RESET_IPP)
				VIM_HIF_SetReg32(V5_REG_MARB_RESET,  0);
#endif

}
/********************************************************************************
  Description:
	get address error flag and clear it.
  Parameters:
	void
  Returns:
  	void
  Remarks:
*********************************************************************************/
const VIM_RESULT gAddrErrorNum[8]={VIM_ERROR_MARB_BIU,VIM_ERROR_MARB_LBUF,
VIM_ERROR_MARB_TVDC,VIM_ERROR_MARB_VDEC,VIM_ERROR_MARB_VENC,VIM_ERROR_MARB_JPEG,
VIM_ERROR_MARB_IPP};

VIM_RESULT	VIM_MARB_GetAddressError(void)
{
UINT32	uValue=0,i;
	uValue=	VIM_HIF_GetReg32(V5_REG_MARB_ADDRERROR);
	for(i=0;i<8;i++)
	{
		if(uValue&(0X01<<i))
		{
			VIM_HIF_SetReg32(V5_REG_MARB_ADDRERROR,uValue);//clear error flag
			return gAddrErrorNum[i];
		}
	}
	VIM_HIF_SetReg32(V5_REG_MARB_ADDRERROR,uValue);//clear error flag
	return VIM_ERROR_MARB_ADDRUNKNOW;
}
/********************************************************************************
  Description:
	�������ȼ�����marb��������ȼ�
  Parameters:
	void
  Returns:
  	void
  Remarks:
*********************************************************************************/

const gMarbPriority[VIM_MARB_MODENUM][8]={
/* Preview mode �����ȼ�*/
{VIM_MARB_PRI_BIU,VIM_MARB_PRI_LBUF,VIM_MARB_PRI_LCDC,
VIM_MARB_PRI_JPEG,VIM_MARB_PRI_IPP,VIM_MARB_PRI_TVDC,VIM_MARB_PRI_VDEC,VIM_MARB_PRI_VENC},
/* capture still mode �����ȼ�*/
{VIM_MARB_PRI_BIU,VIM_MARB_PRI_LBUF,VIM_MARB_PRI_JPEG,VIM_MARB_PRI_LCDC,
VIM_MARB_PRI_IPP,VIM_MARB_PRI_TVDC,VIM_MARB_PRI_VDEC,VIM_MARB_PRI_VENC},
/*VIM_MARB_CAPTURESTILLTHUMB_MODE*/
{VIM_MARB_PRI_BIU,VIM_MARB_PRI_LBUF,VIM_MARB_PRI_JPEG,VIM_MARB_PRI_IPP,
VIM_MARB_PRI_LCDC,VIM_MARB_PRI_TVDC,VIM_MARB_PRI_VDEC,VIM_MARB_PRI_VENC},
/*VIM_MARB_CAPTUREMPG4_MODE*/
{VIM_MARB_PRI_BIU,VIM_MARB_PRI_LBUF,VIM_MARB_PRI_VENC,VIM_MARB_PRI_LCDC,
VIM_MARB_PRI_IPP,VIM_MARB_PRI_TVDC,VIM_MARB_PRI_VDEC,VIM_MARB_PRI_JPEG},
/*VIM_MARB_ENCODE_MODE*/
{VIM_MARB_PRI_BIU,VIM_MARB_PRI_LBUF,VIM_MARB_PRI_JPEG,VIM_MARB_PRI_LCDC,
VIM_MARB_PRI_IPP,VIM_MARB_PRI_TVDC,VIM_MARB_PRI_VDEC,VIM_MARB_PRI_VENC},
/*VIM_MARB_DISPLAYSTILL_MODE*/
{VIM_MARB_PRI_BIU,VIM_MARB_PRI_JPEG,VIM_MARB_PRI_LBUF,VIM_MARB_PRI_LCDC,
VIM_MARB_PRI_IPP,VIM_MARB_PRI_TVDC,VIM_MARB_PRI_VDEC,VIM_MARB_PRI_VENC},
/*VIM_MARB_DISPLAYSTILLBYPASS_MODE*/
{VIM_MARB_PRI_BIU,VIM_MARB_PRI_LBUF,VIM_MARB_PRI_LCDC,VIM_MARB_PRI_JPEG,
VIM_MARB_PRI_IPP,VIM_MARB_PRI_TVDC,VIM_MARB_PRI_VDEC,VIM_MARB_PRI_VENC},
/*VIM_MARB_DISPLAYMPG4_MODE*/
{VIM_MARB_PRI_BIU,VIM_MARB_PRI_VDEC,VIM_MARB_PRI_LBUF,VIM_MARB_PRI_LCDC,
VIM_MARB_PRI_IPP,VIM_MARB_PRI_TVDC,VIM_MARB_PRI_JPEG,VIM_MARB_PRI_VENC},
/*VIM_MARB_DECODEYUV_MODE*/
{VIM_MARB_PRI_BIU,VIM_MARB_PRI_JPEG,VIM_MARB_PRI_LBUF,VIM_MARB_PRI_LCDC,
VIM_MARB_PRI_IPP,VIM_MARB_PRI_TVDC,VIM_MARB_PRI_VDEC,VIM_MARB_PRI_VENC},
/*VIM_MARB_DECODERGB_MODE*/
{VIM_MARB_PRI_BIU,VIM_MARB_PRI_JPEG,VIM_MARB_PRI_LBUF,VIM_MARB_PRI_LCDC,
VIM_MARB_PRI_IPP,VIM_MARB_PRI_TVDC,VIM_MARB_PRI_VDEC,VIM_MARB_PRI_VENC},
/*VIM_MARB_VIDEOCONFERENCE_MODE*/
{VIM_MARB_PRI_BIU,VIM_MARB_PRI_JPEG,VIM_MARB_PRI_LBUF,VIM_MARB_PRI_LCDC,
	VIM_MARB_PRI_TVDC,VIM_MARB_PRI_IPP,VIM_MARB_PRI_VDEC,VIM_MARB_PRI_VENC},
/*VIM_MARB_DIRECTDISPLAY_MODE*/
{VIM_MARB_PRI_BIU,VIM_MARB_PRI_JPEG,VIM_MARB_PRI_LBUF,VIM_MARB_PRI_LCDC,
VIM_MARB_PRI_TVDC,VIM_MARB_PRI_IPP,VIM_MARB_PRI_VDEC,VIM_MARB_PRI_VENC},
/*VIM_MARB_BYPASS_MODE*/
{VIM_MARB_PRI_BIU,VIM_MARB_PRI_JPEG,VIM_MARB_PRI_LBUF,VIM_MARB_PRI_LCDC,
VIM_MARB_PRI_TVDC,VIM_MARB_PRI_IPP,VIM_MARB_PRI_VDEC,VIM_MARB_PRI_VENC}
};

void VIM_MARB_SetPriority(VIM_MARB_WORKMODE	mode)
{
#ifdef __NEW_SYS__
	//TBD, by lizg
#else
	UINT32 uTemp=0,i=1;
	for(i=0;i<8;i++)
	{
		uTemp|=((i+1)<<(gMarbPriority[mode][i]*4));
	}
	VIM_HIF_SetReg32(V5_REG_MARB_PRIOPITY,uTemp);//clear error flag
#endif	
}

/********************************************************************************
  Description:
    set the sram in marb ' mode
  Parameters:
  	Num:	
		VIM_MARB_1TSRAM_0=0,
		VIM_MARB_1TSRAM_1=4,
		VIM_MARB_1TSRAM_2=8,
		VIM_MARB_1TSRAM_3=12,
		VIM_MARB_1TSRAM_ALL=13
	Mode: 
		VIM_MARB_1TSRAM_POWERON=0,
		VIM_MARB_1TSRAM_STANDBY=1,
		VIM_MARB_1TSRAM_OFF=2
  Returns:
  	void
  Remarks: 
*********************************************************************************/

void VIM_MARB_Set1TSramMode(VIM_MARB_1TSRAM_NUM Num,VIM_MARB_1TSRAM_MODE Mode)
{
#if VC0820_FPGA
	//TBD, by lizg
#else
	UINT32 uTemp;
	uTemp=VIM_HIF_GetReg32(V5_REG_MARB_1T_CTRL);

	if(Num!=VIM_MARB_1TSRAM_ALL)
	{
		uTemp&=~(UINT32)((0x7)<<Num);
		uTemp|=(UINT32)((Mode)<<Num);
	}
	else
	{
		uTemp&=~(UINT32)((0x7)<<VIM_MARB_1TSRAM_0);
		uTemp&=~(UINT32)((0x7)<<VIM_MARB_1TSRAM_1);
		uTemp&=~(UINT32)((0x7)<<VIM_MARB_1TSRAM_2);
		uTemp&=~(UINT32)((0x7)<<VIM_MARB_1TSRAM_3);
		uTemp|=(UINT32)((Mode)<<VIM_MARB_1TSRAM_0);
		uTemp|=(UINT32)((Mode)<<VIM_MARB_1TSRAM_1);
		uTemp|=(UINT32)((Mode)<<VIM_MARB_1TSRAM_2);
		uTemp|=(UINT32)((Mode)<<VIM_MARB_1TSRAM_3);
	}
	
	VIM_HIF_SetReg32(V5_REG_MARB_1T_CTRL,uTemp);
	//VIM_HIF_SetReg32(V5_REG_MARB_1T_CTRL,0x10205555);
	uTemp=VIM_HIF_GetReg32(V5_REG_MARB_1T_CTRL);
#if(VIM_MARB_DEBUG)&&(VIM_5XRDK_DEBUG)
			VIM_USER_PrintHex("[VC0578B][MARB] V5_REG_MARB_1T_CTRL : ", uTemp);
#endif
	
	VIM_HIF_SetReg32(V5_REG_MARB_1T_CTRL,0x102f5555);
#endif
}
/********************************************************************************
  Description:
    	����1T��ECC�Ƿ���Ч
  Parameters:
  	Num:	
		VIM_MARB_1TSRAM_0=0,
		VIM_MARB_1TSRAM_1=4,
		VIM_MARB_1TSRAM_2=8,
		VIM_MARB_1TSRAM_3=12,
		VIM_MARB_1TSRAM_ALL=13
	Enable: 
		�Ƿ���Ч
  Returns:
  	void
  Remarks: 
*********************************************************************************/

void VIM_MARB_Set1TEcc(VIM_MARB_1TSRAM_NUM Num,BOOL Enable)
{
	UINT32 uTemp;
	uTemp=VIM_HIF_GetReg32(V5_REG_MARB_1T_CTRL);
	if(Num!=VIM_MARB_1TSRAM_ALL)
	{
		if(Enable==ENABLE)
			uTemp&=~(UINT32)((0x8)<<Num);
		else
			uTemp|=(UINT32)((0x8)<<Num);
	}
	else
	{
		if(Enable==ENABLE)
		{
			uTemp&=~(UINT32)((0x8)<<VIM_MARB_1TSRAM_0);
			uTemp&=~(UINT32)((0x8)<<VIM_MARB_1TSRAM_1);
			uTemp&=~(UINT32)((0x8)<<VIM_MARB_1TSRAM_2);
			uTemp&=~(UINT32)((0x8)<<VIM_MARB_1TSRAM_3);
		}
		else
		{
			uTemp|=(UINT32)((0x8)<<VIM_MARB_1TSRAM_0);
			uTemp|=(UINT32)((0x8)<<VIM_MARB_1TSRAM_1);
			uTemp|=(UINT32)((0x8)<<VIM_MARB_1TSRAM_2);
			uTemp|=(UINT32)((0x8)<<VIM_MARB_1TSRAM_3);
		}
	}
	VIM_HIF_SetReg32(V5_REG_MARB_1T_CTRL,uTemp);
}
/********************************************************************************
  Description:
    	����1T��CLOCK32��divider
  Parameters:
  	void
  Returns:
  	void
  Remarks: 
*********************************************************************************/


VIM_RESULT VIM_MARB_InitMarbRereshTime(void)
{
#if VC0820_FPGA
	//TBD, by lizg
	return 	VIM_SUCCEED;
#else
UINT32 dwOutValue=0;
	dwOutValue=VIM_HIF_GetReg8(V5_REG_CPM_CLK_CTRL+1);
	if(!dwOutValue)
	{
#if(VIM_MARB_DEBUG)&&(VIM_5XRDK_ERROR)
		VIM_USER_PrintHex("[VC0578B][MARB]  MCLK DIV : ", dwOutValue);
#endif
		return 	VIM_ERROR_MARB_DIVERROR;
	}
	dwOutValue=VIM_PLLOUT/dwOutValue;
	dwOutValue*=1000;//hz
	dwOutValue>>=15; // mclk/32767/
	if(dwOutValue)
		dwOutValue--;

#if(BOARD_TYPE!=FPGA)
	dwOutValue>>=3; 		//angela 1110 for up it to 256k
#endif
	VIM_HIF_SetReg32(V5_REG_MARB_CLK32_DIVIDER, (dwOutValue));
	return 	VIM_SUCCEED;
#endif
}


/********************************************************************************
  Description:
	set marb mode
  Parameters:
	mode:
		// Preview modes
	VIM_MARB_PREVIEW_MODE=0, // Auto Bit Rate Control enable

	// Capture modes
	VIM_MARB_CAPTURESTILL_MODE,
	VIM_MARB_CAPTURESTILLTHUMB_MODE,
	VIM_MARB_CAPTUREMPG4_MODE,

	// Encode modes
	VIM_MARB_ENCODE_MODE,

	// Display modes
	VIM_MARB_DISPLAYSTILL_MODE,
	VIM_MARB_DISPLAYSTILLBYPASS_MODE,
	VIM_MARB_DISPLAYMPG4_MODE,

	// Decode modes
	VIM_MARB_DECODEYUV_MODE, // Without IPP
	VIM_MARB_DECODERGB_MODE,
	
	// Special modes
	VIM_MARB_DIRECTDISPLAY_MODE,
	VIM_MARB_BYPASS_MODE
  Returns:
  	void
  Remarks:
*********************************************************************************/
void VIM_MARB_SetMode(VIM_MARB_WORKMODE bMode)
{
	UINT8 fifo0=0,fifo1=0;
#if(BOARD_TYPE==VC0820_FPGA)
	fifo0=VIM_HIF_GetReg16(V5_REG_MARB_VFIFO_CTRL_01)&(~(0x1f));
	fifo1=VIM_HIF_GetReg16(V5_REG_MARB_VFIFO_CTRL_01+2)&(~(0x1f));
#else
	fifo0=VIM_HIF_GetReg16(V5_REG_MARB_VFIFO_CTRL)&(~(0x1f));
	fifo1=VIM_HIF_GetReg16(V5_REG_MARB_VFIFO_CTRL+2)&(~(0x1f));
#endif
	
	//VIM_MARB_SetFifoEn(VIM_MARB_FIFO_0,DISABLE);
	//VIM_MARB_SetFifoEn(VIM_MARB_FIFO_1,DISABLE);
	VIM_MARB_SetFifoDisable(VIM_MARB_FIFO_0);
	VIM_MARB_SetFifoDisable(VIM_MARB_FIFO_1);
	
	switch(bMode)
	{
		case VIM_MARB_PREVIEW_MODE:
		case VIM_MARB_CAPTURESTILL_MODE:
		case VIM_MARB_ENCODE_MODE:
			fifo0|=BIT4;// ��Ҫͬ���źŲ�ȡ����
			//fifo0|=BIT1;// hold at large
			//fifo0|=BIT3;// hold at empty
			fifo0|=BIT2;// hold at full
			//VIM_MARB_SetFifoEn(VIM_MARB_FIFO_0,ENABLE);
			VIM_MARB_SetFifoChannel(VIM_MARB_PRI_JPEG,VIM_MARB_PRI_BIU,VIM_MARB_FIFO_0);
			break;
		case VIM_MARB_CAPTURESTILLTHUMB_MODE:
			fifo0|=BIT4;// ��Ҫͬ���źŲ�ȡ����
#if(BOARD_TYPE==VC0820_FPGA)//add by zhoujian

#else
			fifo0|=BIT1;// hold at large
#endif
			fifo0|=BIT3;// hold at empty
			fifo0|=BIT2;// hold at full
			//VIM_MARB_SetFifoEn(VIM_MARB_FIFO_0,ENABLE);
			//VIM_MARB_SetFifoEn(VIM_MARB_FIFO_1,ENABLE);
			VIM_MARB_SetFifoChannel(VIM_MARB_PRI_JPEG,VIM_MARB_PRI_BIU,VIM_MARB_FIFO_0);
#if(BOARD_TYPE==VC0820_FPGA)//add by zhoujian

#else
			VIM_MARB_SetFifoChannel(VIM_MARB_PRI_IPP,VIM_MARB_PRI_BIU,VIM_MARB_FIFO_1);
#endif
			break;
		case VIM_MARB_CAPTUREMPG4_MODE:
//refined by youhai 2007.12.27			
#if (BOARD_TYPE==VC0820_FPGA)			
#else
			fifo0|=BIT4;// ��Ҫͬ���źŲ�ȡ����
			fifo0|=BIT1;// hold at large
			fifo0|=BIT3;// hold at empty
			fifo0|=BIT2;// hold at full
			VIM_MARB_SetFifoChannel(VIM_MARB_PRI_VENC,VIM_MARB_PRI_BIU,VIM_MARB_FIFO_0);
#endif			
			break;
		case VIM_MARB_VIDEOCONFERENCE_MODE:
			fifo0|=BIT4;// ��Ҫͬ���źŲ�ȡ����
			fifo0|=BIT1;// hold at large
			//fifo0|=BIT3;// hold at empty
			fifo0|=BIT2;// hold at full
			VIM_MARB_SetFifoChannel(VIM_MARB_PRI_VENC,VIM_MARB_PRI_BIU,VIM_MARB_FIFO_0);
			//fifo1|=BIT3;// hold at empty
			VIM_MARB_SetFifoChannel(VIM_MARB_PRI_BIU,VIM_MARB_PRI_VDEC,VIM_MARB_FIFO_1);

			break;
		case VIM_MARB_DISPLAYSTILL_MODE:
		case VIM_MARB_DISPLAYSTILLBYPASS_MODE:
			fifo0|=BIT3;// hold at empty
			//VIM_MARB_SetFifoEn(VIM_MARB_FIFO_0,ENABLE);
			VIM_MARB_SetFifoChannel(VIM_MARB_PRI_BIU,VIM_MARB_PRI_JPEG,VIM_MARB_FIFO_0);
			break;
#if (BOARD_TYPE==VC0820_FPGA)
		case VIM_MARB_DECODEONLY_MODE:
		case VIM_MARB_DECODERGBBYPASS_MODE:
#endif
		case VIM_MARB_DECODEYUV_MODE:
		case VIM_MARB_DECODERGB_MODE:
			fifo0|=BIT3;// hold at empty
			//VIM_MARB_SetFifoEn(VIM_MARB_FIFO_0,ENABLE);
			VIM_MARB_SetFifoChannel(VIM_MARB_PRI_BIU,VIM_MARB_PRI_JPEG,VIM_MARB_FIFO_0);
			break;
#if(BOARD_TYPE==VC0820_FPGA)//add by zhoujian
		case VIM_MARB_DISPLAYDEBUG_MODE:
#endif
		case VIM_MARB_DISPLAYMPG4_MODE:
#if(BOARD_TYPE==VC0820_FPGA) 
			// VC820 vdec does not support virtual fifo mode
#else
			//fifo0|=BIT3;// hold at empty
			//VIM_MARB_SetFifoEn(VIM_MARB_FIFO_0,ENABLE);
			VIM_MARB_SetFifoChannel(VIM_MARB_PRI_BIU,VIM_MARB_PRI_VDEC,VIM_MARB_FIFO_0);
			VIM_MARB_SetFifoChannel(VIM_MARB_PRI_BIU,VIM_MARB_PRI_VDEC,VIM_MARB_FIFO_1);
#endif
			break;

		case VIM_MARB_DIRECTDISPLAY_MODE:
		case VIM_MARB_BYPASS_MODE:
			break;
		default:
			break;
	}
#if(BOARD_TYPE==VC0820_FPGA)
	VIM_HIF_SetReg16(V5_REG_MARB_VFIFO_CTRL_01,fifo0);
	VIM_HIF_SetReg16(V5_REG_MARB_VFIFO_CTRL_01+2,fifo1);
#else
	VIM_HIF_SetReg16(V5_REG_MARB_VFIFO_CTRL,fifo0);
	VIM_HIF_SetReg16(V5_REG_MARB_VFIFO_CTRL+2,fifo1);
#endif
}

/********************************************************************************
  Description:
  	set multi shot enable
  Parameters:
	UINT8 Num;
		����������������num��31��ʱ��
		�Ͳ�ֹͣ����������video
	Fifo:
		VIM_MARB_FIFO_0=0,
		VIM_MARB_FIFO_1=2,
  Returns:
  	void
  Remarks:
*********************************************************************************/

void VIM_MARB_SetCaptureNum(VIM_MARB_FIFONUM Fifo,UINT8 Num)
{
#if(BOARD_TYPE==VC0820_FPGA)
	VIM_HIF_SetReg8(V5_REG_MARB_VFIFO_CTRL_01+Fifo+1,Num);//fifo 0
#else
	VIM_HIF_SetReg8(V5_REG_MARB_VFIFO_CTRL+Fifo+1,Num);//fifo 0
#endif
}

/********************************************************************************
  Description:
  	������������֮�����֡��
  Parameters:
	UINT8 Num;
		0-7֡
	Fifo:
		VIM_MARB_FIFO_0=0,
		VIM_MARB_FIFO_1=2,

  Returns:
  	void
  Remarks:
*********************************************************************************/

void VIM_MARB_SetCaptureDropNum(UINT8 Num,VIM_MARB_FIFONUM Fifo)
{
	UINT8 temp=0;
#if(BOARD_TYPE==VC0820_FPGA)
	temp=VIM_HIF_GetReg8(V5_REG_MARB_VFIFO_CTRL_01+Fifo);//fifo 0
#else
	temp=VIM_HIF_GetReg8(V5_REG_MARB_VFIFO_CTRL+Fifo);//fifo 0
#endif
	temp&=0x1f;
	temp|=(Num<<5);
#if(BOARD_TYPE==VC0820_FPGA)
	VIM_HIF_SetReg8(V5_REG_MARB_VFIFO_CTRL_01+Fifo,Num);//fifo 0
#else	
	VIM_HIF_SetReg8(V5_REG_MARB_VFIFO_CTRL+Fifo,Num);//fifo 0
#endif	
}

/********************************************************************************
  Description:
  	������Ƭ�����size
  Parameters:
	UINT32 Length;
	Fifo:
		VIM_MARB_FIFO_0=0,
		VIM_MARB_FIFO_1=2,
  Returns:
  	VIM_ERROR_MARB_SETOVERLEN
  	VIM_SUCCEED
  Remarks:
*********************************************************************************/

VIM_RESULT VIM_MARB_SetCaptureMaxLen(VIM_MARB_FIFONUM Fifo,UINT32 Length)
{
	Length>>=10;	//    1k byte ��λ����/1024
	if(Length>65535)
		return VIM_ERROR_MARB_SETOVERLEN;
#if(BOARD_TYPE==VC0820_FPGA)
	if(Fifo==VIM_MARB_FIFO_0)
		VIM_HIF_SetReg32(V5_REG_MARB_VFIFO_TOO_LARGE_0,Length);
	else
		VIM_HIF_SetReg32(V5_REG_MARB_VFIFO_TOO_LARGE_1,Length);
#else
	if(Fifo==VIM_MARB_FIFO_0)
		VIM_HIF_SetReg32(V5_REG_MARB_VFIFO_TOO_LARGE0,Length);
	else
		VIM_HIF_SetReg32(V5_REG_MARB_VFIFO_TOO_LARGE1,Length);
#endif
	return VIM_SUCCEED;
}
/********************************************************************************
  Description:
  	set recapture enable
  Parameters:
	UINT8 Enable;
		ENABLE
		DISABLE
  Returns:
  	void
  Remarks:
*********************************************************************************/

void VIM_MARB_SetRecaptureEn(VIM_MARB_FIFONUM Fifo,BOOL Enable)
{
	UINT8 temp=0;
#if(BOARD_TYPE==VC0820_FPGA)
	temp=VIM_HIF_GetReg8(V5_REG_MARB_VFIFO_CTRL_01+Fifo);//fifo 0
	temp&=(~BIT1);
	temp|=(Enable<<1);
	VIM_HIF_SetReg8(V5_REG_MARB_VFIFO_CTRL_01+Fifo,temp);//fifo 0
#else
	temp=VIM_HIF_GetReg8(V5_REG_MARB_VFIFO_CTRL+Fifo);//fifo 0
	temp&=(~BIT1);
	temp|=(Enable<<1);
	VIM_HIF_SetReg8(V5_REG_MARB_VFIFO_CTRL+Fifo,temp);//fifo 0
#endif
}
/********************************************************************************
  Description:
  	����fifo ��ͨ��
  Parameters:
	Wt: дͨ��
	Rd:��ͨ��
	Fifo:
		VIM_MARB_FIFO_0=0,
		VIM_MARB_FIFO_1=2,
  Returns:
  	void
  Remarks:
*********************************************************************************/

void VIM_MARB_SetFifoChannel(VIM_MARB_CHANNEL Wt,VIM_MARB_CHANNEL Rd,VIM_MARB_FIFONUM Fifo)
{
#if (BOARD_TYPE==VC0820_FPGA)
	UINT8 Temp=0;
	// BIU write mode, change to ARMD write mode, in 820 bit(0~3) RD, bit(4~7) WT 
	if(Wt ==VIM_MARB_PRI_BIU)
	{
		//VIM_MARB_SetFifoMode(Fifo,MARB_MODE_ARMD_WRITE);
		VIM_MARB_SetFifoMode(Fifo,MARB_MODE_NORMAL);
		Temp=(VIM_MARB_PRI_BIU<<4)|Rd;
		VIM_HIF_SetReg8(V5_REG_MARB_VFIFO_CHANNELS+(Fifo/2),Temp);
	}

	// BIU read mode, change to ARMD read mode
	if(Rd ==VIM_MARB_PRI_BIU)
	{
		//VIM_MARB_SetFifoMode(Fifo,MARB_MODE_ARMD_READ);
		VIM_MARB_SetFifoMode(Fifo,MARB_MODE_NORMAL);
		Temp=(Wt<<4)|VIM_MARB_PRI_BIU;
		VIM_HIF_SetReg8(V5_REG_MARB_VFIFO_CHANNELS+(Fifo/2),Temp);
	}

#else
	UINT8 Temp=0;
	Temp=Wt|(Rd<<4);
	VIM_HIF_SetReg8(V5_REG_MARB_VFIFO_CHANNELS+(Fifo/2),Temp);
#endif
}
/********************************************************************************
  Description:
  	����fifo����
  Parameters:
	Fifo:
		VIM_MARB_FIFO_0=0,
		VIM_MARB_FIFO_1=2,
	Enable:
		�����
  Returns:
  	void
  Remarks:
*********************************************************************************/

void VIM_MARB_SetFifoEn(VIM_MARB_FIFONUM Fifo,BOOL Enable)
{
#if(BOARD_TYPE==VC0820_FPGA)
	// reset Fifo R/W cursor
	if(Enable)
	{
		gVc0578b_Info.MarbStatus.VFifo[Fifo/2].read_cur=0;
		gVc0578b_Info.MarbStatus.VFifo[Fifo/2].write_cur=0;
	}
#endif
	VIM_HIF_SetReg8(V5_REG_MARB_VFIFO_ENABLE+(Fifo/2),Enable<<1);//Reset
	VIM_HIF_SetReg8(V5_REG_MARB_VFIFO_ENABLE+(Fifo/2),Enable<<1);//Reset
	VIM_HIF_SetReg8(V5_REG_MARB_VFIFO_ENABLE+(Fifo/2),Enable);//enalbe

}
void VIM_MARB_SetFifoDisable(VIM_MARB_FIFONUM Fifo)
{
	VIM_HIF_SetReg8(V5_REG_MARB_VFIFO_ENABLE+(Fifo/2),0);//enalbe
}

/********************************************************************************
  Description:
  	ֹͣд���ݽ�fifo
  Parameters:
	Fifo:
		VIM_MARB_FIFO_0=0,
		VIM_MARB_FIFO_1=2,
	Enable:
		�����
  Returns:
  	void
  Remarks:
*********************************************************************************/

void VIM_MARB_SetFifoStop(VIM_MARB_FIFONUM Fifo)
{
	VIM_HIF_SetReg8(V5_REG_MARB_VFIFO_STOP+(Fifo/2),1);//Reset
}

/********************************************************************************
  Description:
  	��������ʱ��fifo���ܳ��ȵ�ʮ��֮�����ж�
  Parameters:
	UINT8 ratio;
		����
	Fifo:
		VIM_MARB_FIFO_0=0,
		VIM_MARB_FIFO_1=2,
  Returns:
  	void
  Remarks:
*********************************************************************************/

void VIM_MARB_SetJpegbufGapratio(UINT8 ratio,VIM_MARB_FIFONUM Fifo)
{
      UINT32 JbufLength;
      if(Fifo==VIM_MARB_FIFO_0)
		JbufLength=VIM_HIF_GetReg32(V5_REG_MARB_VFIFO_LENGTH_0);
	else
		JbufLength=VIM_HIF_GetReg32(V5_REG_MARB_VFIFO_LENGTH_1);
		
	JbufLength *= ratio;
	JbufLength /= VIM_MARB_JBUF_MAXBLOCK;
	JbufLength>>=10;//  1kb ��λ
	
      if(Fifo==VIM_MARB_FIFO_0)
      	{
#if(BOARD_TYPE==VC0820_FPGA)//add by zhoujian
		VIM_HIF_SetReg16(V5_REG_MARB_VFIFO_THRESHOLD_0,(UINT16)JbufLength);
		VIM_HIF_SetReg16(V5_REG_MARB_VFIFO_THRESHOLD_0+2,(UINT16)JbufLength);
#else
		VIM_HIF_SetReg16(V5_REG_MARB_VFIFO_THRESHOLD_0,(UINT16)(JbufLength + VIM_MARB_JBUF_GAPINTERVAL));
		VIM_HIF_SetReg16(V5_REG_MARB_VFIFO_THRESHOLD_0+2,(UINT16)(JbufLength -VIM_MARB_JBUF_GAPINTERVAL));		  
#endif
      	}
	else
	{
		VIM_HIF_SetReg16(V5_REG_MARB_VFIFO_THRESHOLD_1,(JbufLength + VIM_MARB_JBUF_GAPINTERVAL));
		VIM_HIF_SetReg16(V5_REG_MARB_VFIFO_THRESHOLD_1+2,(JbufLength -VIM_MARB_JBUF_GAPINTERVAL));
	}
}


/********************************************************************************
Description:
	implicate how much bytes in the fifo
parameters: 
	Fifo:
		VIM_MARB_FIFO_0=0,
		VIM_MARB_FIFO_1=2,
Return:
     	length
Remarks:
*********************************************************************************/
UINT32 VIM_MARB_GetFifoSize(VIM_MARB_FIFONUM Fifo)
{
   UINT32 JbufLength;
      if(Fifo==VIM_MARB_FIFO_0)
		JbufLength=VIM_HIF_GetReg32(V5_REG_MARB_VFIFO_COUNT_0);
	else
		JbufLength=VIM_HIF_GetReg32(V5_REG_MARB_VFIFO_COUNT_1);
		

#if(VIM_5XRDK_DEBUG)&&(VIM_MARB_DEBUG)
		VIM_USER_PrintDec(" [VC0578B][MARB]fifo length=",JbufLength);
#endif
	return JbufLength;
}

/********************************************************************************
Description:
	MARB get jbuf  
	start address is 0
	end  get from register
parameters: 
	Jbufpos: host point
Return:
     	void
Remarks:
*********************************************************************************/
UINT32 VIM_MARB_GetJbufRWSize(void)
{
	UINT32 fifolen,MaxLen; 
	UINT8 Temp;
	fifolen=VIM_MARB_GetFifoSize(VIM_MARB_FIFO_0);
#if (BOARD_TYPE==VC0820_FPGA) //add by zhoujian 
	Temp=VIM_HIF_GetReg8(V5_REG_MARB_VFIFO_CHANNELS)&0xf0;// fifo 0 write channle
#else
	Temp=VIM_HIF_GetReg8(V5_REG_MARB_VFIFO_CHANNELS)&0xf;// fifo 0 write channle.
#endif

	MaxLen=VIM_HIF_GetReg32(V5_REG_MARB_VFIFO_LENGTH_0);
	//if read position more than write position then error
#if(VIM_5XRDK_DEBUG)&&(VIM_MARB_DEBUG)
		VIM_USER_PrintHex("[VC0578B][MARB] MaxLen=",MaxLen);
		VIM_USER_PrintHex("[VC0578B][MARB] fifolen=",fifolen);
#endif

#if (BOARD_TYPE==VC0820_FPGA)//add by zhoujian
		Temp=(Temp>>4)&(~(0xf0));
#if(VIM_5XRDK_DEBUG)&&(VIM_MARB_DEBUG)
		VIM_USER_PrintHex("[VC0578B][MARB] Write Channel=",Temp);	
#endif
		if(VIM_MARB_PRI_BIU==Temp)
		{
			return ((MaxLen - fifolen));
		}
		//encode
		else	
		{
			return fifolen;
		}
#else
		//decode
		if(VIM_MARB_PRI_BIU==Temp)
		{
			return ((MaxLen - fifolen));
		}
		//encode
		else	
		{
			return fifolen;
		}
#endif


}
/********************************************************************************
Description:
	MARB get jbuf  
	start address is 0
	end  get from register
parameters: 
	Jbufpos: host point
Return:
     	void
Remarks:
*********************************************************************************/
UINT32 VIM_MARB_GetMp4RWSize(void)
{
#if(BOARD_TYPE==VC0820_FPGA) 
	return gVc0578b_Info.MarbStatus.vdec_bitstream_len;

#else
	UINT32 fifolen,MaxLen; 
	UINT8 Temp;
	fifolen=VIM_MARB_GetFifoSize(VIM_MARB_FIFO_1);
	Temp=(VIM_HIF_GetReg8(V5_REG_MARB_VFIFO_CHANNELS)&0xf00)>>8;// fifo 0 write channle.

	MaxLen=VIM_HIF_GetReg32(V5_REG_MARB_VFIFO_LENGTH_1);
	//if read position more than write position then error
#if(VIM_5XRDK_DEBUG)&&(VIM_MARB_DEBUG)
		VIM_USER_PrintHex("[VC0578B][MARB] MaxLen=",MaxLen);
		VIM_USER_PrintHex("[VC0578B][MARB] fifolen=",fifolen);
#endif

	//decode
	if(VIM_MARB_PRI_BIU==Temp)
	{
		return ((MaxLen - fifolen));
	}
	//encode
	else	{
		return fifolen;
	}
#endif
}
/********************************************************************************
  Description:
	when encode mode ,when(write point - read point) > this value ,interrupt will be generated
	when decode mode,when(read point - write point) > this value ,interrupt will be generated
  Parameters:
	Length: actual interval 
  Returns:
  	void
  Remarks:
*********************************************************************************/

UINT32 VIM_MARB_GetJbufUpThreshold(void)
{
UINT32 Length;
	Length=(UINT32)(VIM_HIF_GetReg16(V5_REG_MARB_VFIFO_THRESHOLD_0)<<10);
return Length;
}
/********************************************************************************
Description:
	�ж�buf�Ƿ������д�룬ֻ��decodeʱ����Ч
parameters: 
	Jbufpos: host point
Return:
     	void
Remarks:
*********************************************************************************/
UINT8 VIM_MARB_GetDeStatus(void)
{
	UINT32 fifolen,MaxLen; 
	UINT8 Temp;
	fifolen=VIM_MARB_GetFifoSize(VIM_MARB_FIFO_0);
#if (BOARD_TYPE==VC0820_FPGA) //add by zhoujian
			Temp=VIM_HIF_GetReg8(V5_REG_MARB_VFIFO_CHANNELS)&0xf0;// fifo 0 write channle
#else
			Temp=VIM_HIF_GetReg8(V5_REG_MARB_VFIFO_CHANNELS)&0xf;// fifo 0 write channle.
#endif

	MaxLen=VIM_HIF_GetReg32(V5_REG_MARB_VFIFO_LENGTH_0);
	//if read position more than write position then error
#if(VIM_5XRDK_DEBUG)&&(VIM_MARB_DEBUG)
		VIM_USER_PrintHex("[VC0578B][MARB] MaxLen=",MaxLen);
		VIM_USER_PrintHex("[VC0578B][MARB] fifolen=",fifolen);
#endif

		//decode
#if (BOARD_TYPE==VC0820_FPGA)//add by zhoujian
		Temp=(Temp>>4)&(~(0xf0));
#if(VIM_5XRDK_DEBUG)&&(VIM_MARB_DEBUG)
		VIM_USER_PrintHex("[VC0578B][MARB] Write Channel=",Temp);	
#endif
		if(VIM_MARB_PRI_ARMD==Temp)
		{
			if((MaxLen - fifolen)>(VIM_MARB_GetJbufUpThreshold()))
				return 1;
			else
				return 0;
		}
#else
		if(VIM_MARB_PRI_BIU==Temp)
		{
			if((MaxLen - fifolen)>(VIM_MARB_GetJbufUpThreshold()))
				return 1;
			else
				return 0;
		}
#endif

	return 0;
}
/********************************************************************************
	Description:
		ȡ���жϱ�־
	Parameters:
		Qt:0-31
	 	
	Remarks:
*********************************************************************************/
 VIM_MPEG4_DECINTFLAG VIM_MARB_GetFIFOIntFlag(void)
{
UINT32 temp;

	temp=VIM_HIF_GetReg32(V5_REG_MP4DEC_INT);
	return (VIM_MPEG4_DECINTFLAG)temp;
}

#define MALLOC1
//#define MALLOC2
/********************************************************************************
  Description:
  	set marb maping list
  Parameters:
  Returns:
  	void
  Remarks:
*********************************************************************************/
extern TSize gb0lcdsize ;
extern TSize gb1lcdsize;
VIM_RESULT VIM_MARB_SetMap(VIM_MARB_WORKMODE bWorkMode, VIM_DISP_BUFFERMODER Aformat,PVIM_MARB_Map Maping)
{
	VIM_DISP_BFORMAT fmt;
	TSize size;//,size1;
	UINT32 dwLength;
	UINT16 dwLength1;
	VIM_JPEG_YUVMODE YUVMode;
	UINT32 mp4_start,cursor;

	//------(1) common setting, LCDC B layer setting
	// Set b 1 layer memary;
	VIM_DISP_GetGbufFormat(&fmt);
	//modify  by lrx
	//VIM_DISP_GetBSize(VIM_DISP_B1LAYER,&size);
	//dwLength=VIM_MARB_GetBmemSize(fmt,size);
	
#ifdef	SET_MAP_ALL	
	dwLength	=VIM_MARB_GetBmemSize(fmt,gb1lcdsize);
	if(dwLength)
	{
		if(Maping->layerB1start!=0)
		{
			MMD_SYS_Free(Maping->layerB1start);
		}
		cursor=MMD_SYS_Malloc(dwLength);
		if(cursor==NULL)
			return VIM_ERROR_MARB_RAMINFO_SRAM_NOSPACE;
	}
	else
	{
		cursor=0;
	}
	Maping->layerB1start = cursor;
	VIM_HIF_SetReg32(V5_REG_LCDC_GBBA1,Maping->layerB1start+V5_1TSRAM_BASE);


	// Set b 0 layer memary;
	VIM_DISP_GetGbufFormat(&fmt);
	//VIM_DISP_GetBSize(VIM_DISP_B0LAYER,&size);
	//dwLength=VIM_MARB_GetBmemSize(fmt,size);
	dwLength=VIM_MARB_GetBmemSize(fmt,gb0lcdsize);
	if(dwLength)
	{
		if(Maping->layerB0start!=0)
		{
			MMD_SYS_Free(Maping->layerB0start);
		}
		cursor=MMD_SYS_Malloc(dwLength);
		if(cursor==NULL)
			return VIM_ERROR_MARB_RAMINFO_SRAM_NOSPACE;
	}
	else
	{
		cursor=0;
	}
	Maping->layerB0start = cursor;
	VIM_HIF_SetReg32(V5_REG_LCDC_GBBA0,Maping->layerB0start+V5_1TSRAM_BASE);

	#if 1
	{
		VIM_USER_PrintHex("[VC0578B][MARB]  b1 end=", Maping->layerB1end);
		VIM_USER_PrintHex("[VC0578B][MARB]  b1 start=", Maping->layerB1start);
		VIM_USER_PrintHex("[VC0578B][MARB]  b0 end=", Maping->layerB0end);
		VIM_USER_PrintHex("[VC0578B][MARB]  b0 start=", Maping->layerB0start);
	}		
	#endif
#endif


	//------(2) special setting, LCDC A layer setting
	switch(bWorkMode)
	{
	case VIM_MARB_DIRECTDISPLAY_MODE:
		break;
		
	case VIM_MARB_DISPLAYMPG4_MODE:
		// set a 0 layer
		#if VIM_USER_MPEG4_USE_MAXALAYER
			VIM_DISP_GetA1Winsize(&size);//2006-3-7
		#else			
			VIM_DISP_GetAWinsize(&size);//2006-3-7
		#endif

		if(((VIM_HIF_GetReg8(V5_REG_LCDC_CTRL+3)>>4)&0x7)%2)	
		{
			size.cx=size.cy;
			size.cy=VIM_HIF_GetReg16(V5_REG_LCDC_A0_SIZE_W)+1;
			dwLength = VIM_MARB_GetAmemSize(Aformat, size);
		}
		else
		{
			dwLength = VIM_MARB_GetAmemSize(Aformat, size);
		}
#if LCDCA0SDRAM//LCDCTEST
		cursor=LCDCSDRAMA0ADDR;
#else
		if(dwLength)
		{
			if(Maping->layerA0start!=0)
			{
				MMD_SYS_Free(Maping->layerA0start);
				Maping->layerA0start=0;
			}
			cursor=MMD_SYS_Malloc(dwLength);
			if(cursor==NULL)
				return VIM_ERROR_MARB_RAMINFO_SRAM_NOSPACE;
		}
		else
		{
			if(Maping->layerA0start!=0)
			{
				MMD_SYS_Free(Maping->layerA0start);
				Maping->layerA0start=0;
			}
			cursor=0;
		}
#endif
		
		Maping->layerA0start = cursor;
		VIM_DISP_SetBufferMode((VIM_DISP_BUFFERMODER)Aformat);
		VIM_HIF_SetReg32(V5_REG_LCDC_VBBA0,Maping->layerA0start+V5_1TSRAM_BASE);
		// Line mode and tow frame mode should set VBBA1
		VIM_HIF_SetReg32(V5_REG_LCDC_VBBA1,Maping->layerA0start + (dwLength >> 1)+V5_1TSRAM_BASE);
		break;

	case VIM_MARB_PREVIEW_MODE:
	case VIM_MARB_DISPLAYSTILL_MODE:		
	case VIM_MARB_DISPLAYSTILLBYPASS_MODE:
	case VIM_MARB_CAPTUREMPG4_MODE:		
	case VIM_MARB_DECODERGB_MODE:
	case VIM_MARB_CAPTURESTILLTHUMB_MODE:
	case VIM_MARB_DECODERGBBYPASS_MODE:
	case VIM_MARB_MP4_PREVIEW_MODE:	
	case VIM_MARB_DISPLAYDEBUG_MODE:  
		// preview and capture JPG use this mode 
		// set a 0 layer
		VIM_DISP_GetAWinsize(&size);//2006-3-7
		if(((VIM_HIF_GetReg8(V5_REG_LCDC_CTRL+3)>>4)&0x7)%2)	
		{
			size.cx=size.cy;
			size.cy=VIM_HIF_GetReg16(V5_REG_LCDC_A0_SIZE_W)+1;
			dwLength = VIM_MARB_GetAmemSize(Aformat, size);
		}
		else
		{

			dwLength = VIM_MARB_GetAmemSize(Aformat, size);
		}
		if(dwLength)
		{
			if(Maping->layerA0start!=0)
			{
				MMD_SYS_Free(Maping->layerA0start);
				Maping->layerA0start=0;
			}
			cursor=MMD_SYS_Malloc(dwLength);
			if(cursor==NULL)
				return VIM_ERROR_MARB_RAMINFO_SRAM_NOSPACE;
		}
		else
		{
			if(Maping->layerA0start!=0)
			{
				MMD_SYS_Free(Maping->layerA0start);
				Maping->layerA0start=0;
			}
			cursor=0;
		}
		Maping->layerA0start = cursor;
		VIM_DISP_SetBufferMode((VIM_DISP_BUFFERMODER)Aformat);
		VIM_HIF_SetReg32(V5_REG_LCDC_VBBA0,Maping->layerA0start+V5_1TSRAM_BASE);
		// Line mode and tow frame mode should set VBBA1
		VIM_HIF_SetReg32(V5_REG_LCDC_VBBA1,Maping->layerA0start + (dwLength >> 1)+V5_1TSRAM_BASE);
		break;

	case VIM_MARB_CAPTURESTILL_MODE:
		if(Maping->layerA0start!=0)
		{
			MMD_SYS_Free(Maping->layerA0start);
		}
		
		Maping->layerA0start=0;
		
		VIM_HIF_SetReg32(V5_REG_LCDC_VBBA0,Maping->layerA0start+V5_1TSRAM_BASE);
		// Line mode and tow frame mode should set VBBA1
		VIM_HIF_SetReg32(V5_REG_LCDC_VBBA1,Maping->layerA0start+V5_1TSRAM_BASE);
		break;
		
	default:
		break;
	}
	
#if 1
	VIM_USER_PrintHex("[VC0578B][MARB]  A1 end=", Maping->layerA1end);
	VIM_USER_PrintHex("[VC0578B][MARB]  A1 start=", Maping->layerA1start);
	VIM_USER_PrintHex("[VC0578B][MARB]  A0 end=", Maping->layerA0end);
	VIM_USER_PrintHex("[VC0578B][MARB]  A0 start=", Maping->layerA0start);
#endif

	//------(3) special setting,  LBuf, MC, VFifo, etc.
	switch(bWorkMode)
	{
	case VIM_MARB_DIRECTDISPLAY_MODE:
		break;
		
	case VIM_MARB_DISPLAYMPG4_MODE:
		//Get source file size and YUV format only support 422 now
		YUVMode=VIM_JPEG_GetYUVMode();
		VIM_MPEG4_GetDecVolSize(&size);

		dwLength=(size.cx*size.cy+0xff)>>8;
		dwLength+=2*((size.cx*size.cy/4+0xff)>>8);
#if VIM_USER_MPEG4_DEC_FRAMEMODE		
		//dwLength = (dwLength*2<<8)+size.cx*2*size.cy*2;
		dwLength = (dwLength*2<<8)+size.cx*2*size.cy;
#else
		dwLength = (dwLength*2<<8)+size.cx*2*24*2;
#endif
		// MP4 used buffer MC and Out buffer, relative position can not change
		if(Maping->Total_start!=0)
		{
			MMD_SYS_Free(Maping->Total_start);
			Maping->Total_start=0;
		}		
		cursor=MMD_SYS_Malloc(dwLength);
		if(cursor==NULL)
			return VIM_ERROR_MARB_RAMINFO_SRAM_NOSPACE;
		mp4_start=cursor;
		Maping->Total_start = mp4_start;
		
		//motion compensation buffers(reconstruction/reference frame) 
		//y0
		VIM_HIF_SetReg32(V5_REG_MP4DEC_MC_BASE0_ADDR,mp4_start+V5_1TSRAM_BASE);
		//y1		
		dwLength=((size.cx*size.cy+0xff)>>8)+((size.cx*size.cy/4+0xff)>>8)*2;		
		mp4_start+=dwLength<<8;
		VIM_HIF_SetReg32(V5_REG_MP4DEC_MC_BASE1_ADDR,mp4_start+V5_1TSRAM_BASE);
		// cb, cr offset
		dwLength=(size.cx*size.cy+0xff)>>8;
		VIM_HIF_SetReg32(V5_REG_MP4DEC_MC_CB_OFFSET,dwLength);
		dwLength=(size.cx*size.cy/4+0xff)>>8;		
		VIM_HIF_SetReg32(V5_REG_MP4DEC_MC_CR_OFFSET,dwLength+((size.cx*size.cy+0xff)>>8));

		// output decoded frame buffers, only support MBL, PINGPANG, YUV 422 process mode now
		// y0
		dwLength=((size.cx*size.cy+0xff)>>8)+((size.cx*size.cy/4+0xff)>>8)*2;		
		mp4_start+=dwLength<<8;
		VIM_HIF_SetReg32(V5_REG_MP4DEC_OUT0_Y_ADDR,mp4_start+V5_1TSRAM_BASE);
		VIM_HIF_SetReg32(V5_REG_LBUF_Y_INI0_DEC,mp4_start+V5_1TSRAM_BASE);
		// y1
		
#if VIM_USER_MPEG4_DEC_FRAMEMODE		
		//mp4_start+=size.cx*2*size.cy;
#else
		mp4_start+=size.cx*2*24;
#endif
		//MMDPrintf("out1y=%x\n",mp4_start+V5_1TSRAM_BASE);
		VIM_HIF_SetReg32(V5_REG_MP4DEC_OUT1_Y_ADDR,mp4_start+V5_1TSRAM_BASE);
		VIM_HIF_SetReg32(V5_REG_LBUF_Y_INI1_DEC,mp4_start+V5_1TSRAM_BASE);

		// cb, cr offset, YUV 422 no use, set same value as MC setting according VC820 mass example
		dwLength=(size.cx*size.cy+0xff)>>8;
		VIM_HIF_SetReg32(V5_REG_MP4DEC_OUT_CB_OFFSET,dwLength);
		dwLength=(size.cx*size.cy/4+0xff)>>8;		
		VIM_HIF_SetReg32(V5_REG_MP4DEC_OUT_CR_OFFSET,dwLength+((size.cx*size.cy+0xff)>>8));
		
		#if 1
		{
			UINT32 temp;
			temp = VIM_HIF_GetReg32(V5_REG_MP4DEC_MC_BASE0_ADDR);
			VIM_USER_PrintHex("[VC0578B][MARB]  MC Y0 =", temp);
			temp = VIM_HIF_GetReg32(V5_REG_MP4DEC_MC_BASE1_ADDR);
			VIM_USER_PrintHex("[VC0578B][MARB]  MC Y1 =", temp);
			temp = VIM_HIF_GetReg32(V5_REG_MP4DEC_OUT0_Y_ADDR);
			VIM_USER_PrintHex("[VC0578B][MARB]  OUT Y0 =", temp);
			temp = VIM_HIF_GetReg32(V5_REG_MP4DEC_OUT1_Y_ADDR);
			VIM_USER_PrintHex("[VC0578B][MARB]  OUT Y1 =", temp);
			VIM_USER_PrintHex("[VC0578B][MARB]  Total start =", Maping->Total_start);
			VIM_USER_PrintHex("[VC0578B][MARB]  Total size =", Maping->Total_end-Maping->Total_start);
		}
		#endif	
		break;
#if 0		
	case VIM_MARB_DISPLAYDEBUG_MODE:
			
		YUVMode=VIM_JPEG_GetYUVMode();
		VIM_MPEG4_GetDecSize(&size);

		dwLength=size.cx*size.cy;
		// MP4 used buffer MC and Out buffer, relative position can not change
		ret = VIM_MARB_RamInfo_Alloc_FromTop(MARB_SDRAM, dwLength*8, 4, &cursor);
		DRV_ASSERT(ret==VIM_SUCCEED);
		mp4_start=cursor;
		Maping->Total_start = mp4_start;
		//motion compensation buffers(reconstruction/reference frame) 
		//y0
		VIM_HIF_SetReg32(V5_REG_MP4DEC_MC_BASE0_ADDR,mp4_start+V5_1TSRAM_BASE);
		// cb, cr offset
		VIM_HIF_SetReg32(V5_REG_MP4DEC_MC_CB_OFFSET,dwLength>>8);
		VIM_HIF_SetReg32(V5_REG_MP4DEC_MC_CR_OFFSET,(dwLength+(dwLength>>2)+0xff)>>8);
	
		
		VIM_HIF_SetReg32(V5_REG_MP4DEC_MC_BASE1_ADDR,mp4_start+(dwLength*2)+V5_1TSRAM_BASE);
	
		// output decoded frame buffers, only support MBL, PINGPANG, YUV 422 process mode now
		// y0
		VIM_HIF_SetReg32(V5_REG_MP4DEC_OUT0_Y_ADDR,mp4_start+(dwLength*4)+V5_1TSRAM_BASE);
		
		// y1
		VIM_HIF_SetReg32(V5_REG_MP4DEC_OUT1_Y_ADDR,mp4_start+(dwLength*6)+V5_1TSRAM_BASE);
		
		// cb, cr offset, YUV 422 no use, set same value as MC setting according VC820 mass example
		VIM_HIF_SetReg32(V5_REG_MP4DEC_OUT_CB_OFFSET,dwLength>>8);
		VIM_HIF_SetReg32(V5_REG_MP4DEC_OUT_CR_OFFSET,dwLength>>8);		
		#if 1
		{
			UINT32 temp;
			temp = VIM_HIF_GetReg32(V5_REG_MP4DEC_MC_BASE0_ADDR);
			VIM_USER_PrintHex("[VC0578B][MARB]  MC Y0 =", temp);
			temp = VIM_HIF_GetReg32(V5_REG_MP4DEC_MC_BASE1_ADDR);
			VIM_USER_PrintHex("[VC0578B][MARB]  MC Y1 =", temp);
			temp = VIM_HIF_GetReg32(V5_REG_MP4DEC_OUT0_Y_ADDR);
			VIM_USER_PrintHex("[VC0578B][MARB]  OUT Y0 =", temp);
			temp = VIM_HIF_GetReg32(V5_REG_MP4DEC_OUT1_Y_ADDR);
			VIM_USER_PrintHex("[VC0578B][MARB]  OUT Y1 =", temp);
			VIM_USER_PrintHex("[VC0578B][MARB]  Total start =", Maping->Total_start);
			VIM_USER_PrintHex("[VC0578B][MARB]  Total size =", Maping->Total_end-Maping->Total_start);
		}
		#endif	
		break;
#endif
	case VIM_MARB_PREVIEW_MODE:
		// preview and capture JPG use this mode 
		// set line buf
		//line buf 0
		YUVMode=VIM_JPEG_GetYUVMode();
		VIM_JPEG_GetSize(&size);

		//set v0 buf
		dwLength=VIM_MARB_GetDec0LbufSize(YUVMode,VIM_JPEG_CR,size.cx);

		if(dwLength)
		{
			if(Maping->lbuf0Vstart!=0)
			{
				MMD_SYS_Free(Maping->lbuf0Vstart);
				Maping->lbuf0Vstart=0;
			}	

			cursor=MMD_SYS_Malloc(dwLength);
			if(cursor==NULL)
				return VIM_ERROR_MARB_RAMINFO_SRAM_NOSPACE;
		}
		else
		{
			if(Maping->lbuf0Vstart!=0)
			{
				MMD_SYS_Free(Maping->lbuf0Vstart);
				Maping->lbuf0Vstart=0;
			}	
			cursor=0;
		}

		Maping->lbuf0Vstart=cursor;
		//set u0 buf
		dwLength=VIM_MARB_GetDec0LbufSize(YUVMode,VIM_JPEG_CB,size.cx);
		
		if(dwLength)
		{

			if(Maping->lbuf0Ustart!=0)
			{
				MMD_SYS_Free(Maping->lbuf0Ustart);
				Maping->lbuf0Ustart=0;
			}	

			cursor=MMD_SYS_Malloc(dwLength);
			if(cursor==NULL)
				return VIM_ERROR_MARB_RAMINFO_SRAM_NOSPACE;
		}
		else
		{
			if(Maping->lbuf0Ustart!=0)
			{
				MMD_SYS_Free(Maping->lbuf0Ustart);
				Maping->lbuf0Ustart=0;
			}	
			cursor=0;
		}

		Maping->lbuf0Ustart=cursor;

		//set Y0 buf
		dwLength=VIM_MARB_GetDec0LbufSize(YUVMode,VIM_JPEG_Y,size.cx);
		
		if(dwLength)
		{
			if(Maping->lbuf0Ystart!=0)
			{
				MMD_SYS_Free(Maping->lbuf0Ystart);
				Maping->lbuf0Ystart=0;
			}	

			cursor=MMD_SYS_Malloc(dwLength);
			if(cursor==NULL)
				return VIM_ERROR_MARB_RAMINFO_SRAM_NOSPACE;
		}
		else
		{
			if(Maping->lbuf0Ystart!=0)
			{
				MMD_SYS_Free(Maping->lbuf0Ystart);
				Maping->lbuf0Ystart=0;
			}	
			cursor=0;
		}

		Maping->lbuf0Ystart=cursor;

		//set v1 buf
		dwLength=VIM_MARB_GetDec0LbufSize(YUVMode,VIM_JPEG_CR,size.cx);
		if(dwLength)
		{
			if(Maping->lbuf1Vstart!=0)
			{
				MMD_SYS_Free(Maping->lbuf1Vstart);
				Maping->lbuf1Vstart=0;
			}	

			cursor=MMD_SYS_Malloc(dwLength);
			if(cursor==NULL)
				return VIM_ERROR_MARB_RAMINFO_SRAM_NOSPACE;
		}
		else
		{
			if(Maping->lbuf1Vstart!=0)
			{
				MMD_SYS_Free(Maping->lbuf1Vstart);
				Maping->lbuf1Vstart=0;
			}	
			cursor=0;
		}	

		Maping->lbuf1Vstart=cursor;

		//set u1 buf
		dwLength=VIM_MARB_GetDec0LbufSize(YUVMode,VIM_JPEG_CB,size.cx);
		if(dwLength)
		{
			if(Maping->lbuf1Ustart!=0)
			{
				MMD_SYS_Free(Maping->lbuf1Ustart);
				Maping->lbuf1Ustart=0;
			}

			cursor=MMD_SYS_Malloc(dwLength);
			if(cursor==NULL)
				return VIM_ERROR_MARB_RAMINFO_SRAM_NOSPACE;
		}
		else
		{
			if(Maping->lbuf1Ustart!=0)
			{
				MMD_SYS_Free(Maping->lbuf1Ustart);
				Maping->lbuf1Ustart=0;
			}
			cursor=0;
		}	

		Maping->lbuf1Ustart=cursor;

		//set Y1 buf
		dwLength=VIM_MARB_GetDec0LbufSize(YUVMode,VIM_JPEG_Y,size.cx);
		
		if(dwLength)
		{
			if(Maping->lbuf1Ystart!=0)
			{
				MMD_SYS_Free(Maping->lbuf1Ystart);
				Maping->lbuf1Ystart=0;
			}
		
			cursor=MMD_SYS_Malloc(dwLength);
			if(cursor==NULL)
				return VIM_ERROR_MARB_RAMINFO_SRAM_NOSPACE;
		}
		else
		{
			if(Maping->lbuf1Ystart!=0)
			{
				MMD_SYS_Free(Maping->lbuf1Ystart);
				Maping->lbuf1Ystart=0;
			}
			cursor=0;
		}	

		Maping->lbuf1Ystart=cursor;


		VIM_HIF_SetReg32(V5_REG_LBUF_Y_INI0_ENC,(UINT32)Maping->lbuf0Ystart);
		VIM_HIF_SetReg32(V5_REG_LBUF_CB_INI0_ENC,(UINT32)Maping->lbuf0Ustart);
		VIM_HIF_SetReg32(V5_REG_LBUF_CR_INI0_ENC,(UINT32)Maping->lbuf0Vstart);

		VIM_HIF_SetReg32(V5_REG_LBUF_Y_INI1_ENC,(UINT32)Maping->lbuf1Ystart);
		VIM_HIF_SetReg32(V5_REG_LBUF_CB_INI1_ENC,(UINT32)Maping->lbuf1Ustart);
		VIM_HIF_SetReg32(V5_REG_LBUF_CR_INI1_ENC,(UINT32)Maping->lbuf1Vstart);

		#if 1
		{
			VIM_USER_PrintHex("[VC0578B][MARB]  LBuf end=", Maping->lbufend);
			VIM_USER_PrintHex("[VC0578B][MARB]  LBuf start=", Maping->lbuf1Ystart);			
			VIM_USER_PrintHex("[VC0578B][MARB]  Total start =", Maping->Total_start);
			VIM_USER_PrintHex("[VC0578B][MARB]  Total size =", Maping->Total_end-Maping->Total_start);
		}
		#endif	
		break;
	//case VIM_MARB_CAPTURESTILLWITHFRAME_MODE:
	case VIM_MARB_CAPTURESTILL_MODE:
		// set line buf

		//line buf 0
		YUVMode=VIM_JPEG_GetYUVMode();
		VIM_JPEG_GetSize(&size);

		//set v0 buf
		dwLength=VIM_MARB_GetDec0LbufSize(YUVMode,VIM_JPEG_CR,size.cx);

		if(dwLength)
		{
			if(Maping->lbuf0Vstart!=0)
			{
				MMD_SYS_Free(Maping->lbuf0Vstart);
				Maping->lbuf0Vstart=0;
			}	

			cursor=MMD_SYS_Malloc(dwLength);
			if(cursor==NULL)
				return VIM_ERROR_MARB_RAMINFO_SRAM_NOSPACE;
		}
		else
		{
			if(Maping->lbuf0Vstart!=0)
			{
				MMD_SYS_Free(Maping->lbuf0Vstart);
				Maping->lbuf0Vstart=0;
			}	
			cursor=0;
		}

		Maping->lbuf0Vstart=cursor;
		//set u0 buf
		dwLength=VIM_MARB_GetDec0LbufSize(YUVMode,VIM_JPEG_CB,size.cx);
		
		if(dwLength)
		{

			if(Maping->lbuf0Ustart!=0)
			{
				MMD_SYS_Free(Maping->lbuf0Ustart);
				Maping->lbuf0Ustart=0;
			}	

			cursor=MMD_SYS_Malloc(dwLength);
			if(cursor==NULL)
				return VIM_ERROR_MARB_RAMINFO_SRAM_NOSPACE;
		}
		else
		{
			if(Maping->lbuf0Ustart!=0)
			{
				MMD_SYS_Free(Maping->lbuf0Ustart);
				Maping->lbuf0Ustart=0;
			}	
			cursor=0;
		}

		Maping->lbuf0Ustart=cursor;

		//set Y0 buf
		dwLength=VIM_MARB_GetDec0LbufSize(YUVMode,VIM_JPEG_Y,size.cx);
		
		if(dwLength)
		{
			if(Maping->lbuf0Ystart!=0)
			{
				MMD_SYS_Free(Maping->lbuf0Ystart);
				Maping->lbuf0Ystart=0;
			}	

			cursor=MMD_SYS_Malloc(dwLength);
			if(cursor==NULL)
				return VIM_ERROR_MARB_RAMINFO_SRAM_NOSPACE;
		}
		else
		{
			if(Maping->lbuf0Ystart!=0)
			{
				MMD_SYS_Free(Maping->lbuf0Ystart);
				Maping->lbuf0Ystart=0;
			}	
			cursor=0;
		}

		Maping->lbuf0Ystart=cursor;

		//set v1 buf
		dwLength=VIM_MARB_GetDec0LbufSize(YUVMode,VIM_JPEG_CR,size.cx);
		if(dwLength)
		{
			if(Maping->lbuf1Vstart!=0)
			{
				MMD_SYS_Free(Maping->lbuf1Vstart);
				Maping->lbuf1Vstart=0;
			}	

			cursor=MMD_SYS_Malloc(dwLength);
			if(cursor==NULL)
				return VIM_ERROR_MARB_RAMINFO_SRAM_NOSPACE;
		}
		else
		{
		
			if(Maping->lbuf1Vstart!=0)
			{
				MMD_SYS_Free(Maping->lbuf1Vstart);
				Maping->lbuf1Vstart=0;
			}	
			cursor=0;
		}	
		
		Maping->lbuf1Vstart=cursor;

		//set u1 buf
		dwLength=VIM_MARB_GetDec0LbufSize(YUVMode,VIM_JPEG_CB,size.cx);
		if(dwLength)
		{
			if(Maping->lbuf1Ustart!=0)
			{
				MMD_SYS_Free(Maping->lbuf1Ustart);
				Maping->lbuf1Ustart=0;
			}

			cursor=MMD_SYS_Malloc(dwLength);
			if(cursor==NULL)
				return VIM_ERROR_MARB_RAMINFO_SRAM_NOSPACE;
		}
		else
		{
		
			if(Maping->lbuf1Ustart!=0)
			{
				MMD_SYS_Free(Maping->lbuf1Ustart);
				Maping->lbuf1Ustart=0;
			}
			cursor=0;
		}	
		Maping->lbuf1Ustart=cursor;

		//set Y1 buf
		dwLength=VIM_MARB_GetDec0LbufSize(YUVMode,VIM_JPEG_Y,size.cx);
		
		if(dwLength)
		{
			if(Maping->lbuf1Ystart!=0)
			{
				MMD_SYS_Free(Maping->lbuf1Ystart);
				Maping->lbuf1Ystart=0;
			}
		
			cursor=MMD_SYS_Malloc(dwLength);
			if(cursor==NULL)
				return VIM_ERROR_MARB_RAMINFO_SRAM_NOSPACE;
		}
		else
		{
			if(Maping->lbuf1Ystart!=0)
			{
				MMD_SYS_Free(Maping->lbuf1Ystart);
				Maping->lbuf1Ystart=0;
			}
			cursor=0;
		}	
		Maping->lbuf1Ystart=cursor;

		VIM_HIF_SetReg32(V5_REG_LBUF_Y_INI0_ENC,(UINT32)Maping->lbuf0Ystart);
		VIM_HIF_SetReg32(V5_REG_LBUF_CB_INI0_ENC,(UINT32)Maping->lbuf0Ustart);
		VIM_HIF_SetReg32(V5_REG_LBUF_CR_INI0_ENC,(UINT32)Maping->lbuf0Vstart);

		VIM_HIF_SetReg32(V5_REG_LBUF_Y_INI1_ENC,(UINT32)Maping->lbuf1Ystart);
		VIM_HIF_SetReg32(V5_REG_LBUF_CB_INI1_ENC,(UINT32)Maping->lbuf1Ustart);
		VIM_HIF_SetReg32(V5_REG_LBUF_CR_INI1_ENC,(UINT32)Maping->lbuf1Vstart);
		VIM_HIF_SetReg32(V5_REG_JPEG_MARB_STARTADDR,0x80000000);
		Maping->jbufsize = CAP_VFIFO_LEN;
		dwLength = Maping->jbufsize+1024;
		
		if(Maping->jbufstart!=0)
		{
			MMD_SYS_Free(Maping->jbufstart);
			Maping->jbufstart=0;
		}

		cursor=MMD_SYS_Malloc(dwLength);
		if(cursor==NULL)
			return VIM_ERROR_MARB_RAMINFO_SRAM_NOSPACE;
		//Maping->Total_start = cursor;
		Maping->jbufstart=cursor;
		cursor=(((cursor+1023)>>10)<<10);
		
		VIM_MARB_SetFifoAddLen(VIM_MARB_FIFO_0, cursor, Maping->jbufsize);
		// set JBUF up thur
		dwLength=Maping->jbufsize;
		dwLength=dwLength*Maping->jbufnowblock_int/VIM_MARB_JBUF_MAXBLOCK;
		dwLength1=(dwLength>>10) + VIM_MARB_JBUF_GAPINTERVAL;
		VIM_HIF_SetReg16(V5_REG_MARB_VFIFO_THRESHOLD_0,dwLength1);
		dwLength1=(dwLength>>10) -VIM_MARB_JBUF_GAPINTERVAL;
		VIM_HIF_SetReg16(V5_REG_MARB_VFIFO_THRESHOLD_0+2,dwLength1);
		
		if((gVc0578b_Info.CaptureStatus.ThumbSize.cx*gVc0578b_Info.CaptureStatus.ThumbSize.cy)>0)
		{
        		VIM_IPP_GetThumbSize(&size);
        		dwLength=VIM_MARB_GetBmemSize(VIM_DISP_BLAYER_RGB565,size);
        		if(dwLength)
        		{
        			if(Maping->thumbstart !=0)
        			{
        				MMD_SYS_Free(Maping->thumbstart);
        				Maping->thumbstart=0;
        			}
        		
        			cursor=MMD_SYS_Malloc(dwLength);
        			if(cursor==NULL)
        				return VIM_ERROR_MARB_RAMINFO_SRAM_NOSPACE;
        		}
        		else
        		{
        			if(Maping->thumbstart !=0)
        			{
        				MMD_SYS_Free(Maping->thumbstart);
        				Maping->thumbstart=0;
        			}
        			cursor=0;
        		}	
        		Maping->thumbsize  = dwLength;
        		Maping->thumbstart = cursor;
		}
		else
		{
			Maping->thumbsize  = 0;
        		Maping->thumbstart = 0;
		}
		VIM_IPP_SetThumbbufMem(Maping->thumbstart,Maping->thumbsize);	
		
		break;
		
	case VIM_MARB_CAPTURESTILLWITHFRAME_MODE:
		VIM_HIF_SetReg32(V5_REG_JPEG_MARB_STARTADDR,0x80000000);
		Maping->jbufsize = CAP_VFIFO_LEN_FRAME;
		dwLength = Maping->jbufsize+1024;
		if(Maping->jbufstart!=0)
		{
			MMD_SYS_Free(Maping->jbufstart);
			Maping->jbufstart=0;
		}

		cursor=MMD_SYS_Malloc(dwLength);
		if(cursor==NULL)
			return VIM_ERROR_MARB_RAMINFO_SRAM_NOSPACE;
		//Maping->Total_start = cursor;
		Maping->jbufstart=cursor;
		cursor=(((cursor+1023)>>10)<<10);
		
		VIM_MARB_SetFifoAddLen(VIM_MARB_FIFO_0, cursor, Maping->jbufsize);
		// set JBUF up thur
		dwLength=Maping->jbufsize;
		dwLength=dwLength*Maping->jbufnowblock_int/VIM_MARB_JBUF_MAXBLOCK;
		dwLength1=(dwLength>>10) + VIM_MARB_JBUF_GAPINTERVAL;
		VIM_HIF_SetReg16(V5_REG_MARB_VFIFO_THRESHOLD_0,dwLength1);
		dwLength1=(dwLength>>10) -VIM_MARB_JBUF_GAPINTERVAL;
		VIM_HIF_SetReg16(V5_REG_MARB_VFIFO_THRESHOLD_0+2,dwLength1);
		break;
	
#if (BOARD_TYPE==VC0820_FPGA)//add by zhoujian
	case VIM_MARB_DECODEONLY_MODE:
	case VIM_MARB_DISPLAYSTILLBYPASS_MODE:
#endif
	case VIM_MARB_DISPLAYSTILL_MODE:
		// display JPG use this mode 
		// set line buf, JPG display not use PINGPANG mode, only set one buf0 is OK
		//line buf 0
		YUVMode=VIM_JPEG_GetYUVMode();
		VIM_JPEG_GetSize(&size);
		
		//set v0 buf
		dwLength=VIM_MARB_GetDec0LbufSize(YUVMode,VIM_JPEG_CR,size.cx);
		
		if(dwLength)
		{
			if(Maping->lbuf0Vstart!=0)
			{
				MMD_SYS_Free(Maping->lbuf0Vstart);
				Maping->lbuf0Vstart=0;
			}	
		
			cursor=MMD_SYS_Malloc(dwLength);
			if(cursor==NULL)
				return VIM_ERROR_MARB_RAMINFO_SRAM_NOSPACE;
		}
		else
		{
			cursor=0;
		}
		Maping->lbuf0Vstart=cursor;
		//set u0 buf
		dwLength=VIM_MARB_GetDec0LbufSize(YUVMode,VIM_JPEG_CB,size.cx);
				
		if(dwLength)
		{
			if(Maping->lbuf0Ustart!=0)
			{
				MMD_SYS_Free(Maping->lbuf0Ustart);
				Maping->lbuf0Ustart=0;
			}	
		
			cursor=MMD_SYS_Malloc(dwLength);
			if(cursor==NULL)
				return VIM_ERROR_MARB_RAMINFO_SRAM_NOSPACE;
		}
		else
		{
			cursor=0;
		}
		Maping->lbuf0Ustart=cursor;
		
		//set Y0 buf
		dwLength=VIM_MARB_GetDec0LbufSize(YUVMode,VIM_JPEG_Y,size.cx);
				
		if(dwLength)
		{
			if(Maping->lbuf0Ystart!=0)
			{
				MMD_SYS_Free(Maping->lbuf0Ystart);
				Maping->lbuf0Ystart=0;
			}	
		
			cursor=MMD_SYS_Malloc(dwLength);
			if(cursor==NULL)
				return VIM_ERROR_MARB_RAMINFO_SRAM_NOSPACE;
		}
		else
		{
			cursor=0;
		}
		Maping->lbuf0Ystart=cursor;
		
			
		VIM_HIF_SetReg32(V5_REG_LBUF_Y_INI0_DEC,(UINT32)Maping->lbuf0Ystart);
		VIM_HIF_SetReg32(V5_REG_LBUF_CB_INI0_DEC,(UINT32)Maping->lbuf0Ustart);
		VIM_HIF_SetReg32(V5_REG_LBUF_CR_INI0_DEC,(UINT32)Maping->lbuf0Vstart);

		// set JPG Fifo , use virtual Fifo1 for JPG
		VIM_HIF_SetReg32(V5_REG_JPEG_MARB_STARTADDR,0x80000000);
		Maping->jbufsize = DISP_VFIFO_LEN;
		dwLength = Maping->jbufsize+1024;
        	if(Maping->jbufstart!=0)
		{
			MMD_SYS_Free(Maping->jbufstart);
			Maping->jbufstart=0;
		}
		cursor=MMD_SYS_Malloc(dwLength);
		if(cursor==NULL)
			return VIM_ERROR_MARB_RAMINFO_SRAM_NOSPACE;
		//Maping->Total_start = cursor;
		Maping->jbufstart=cursor;
		cursor=(((cursor+1023)>>10)<<10);
		
		// JPEG display should set FiFo in SRAM for hardware bug
		VIM_MARB_SetFifoAddLen(VIM_MARB_FIFO_0, cursor, Maping->jbufsize);

		// set JBUF up thur
		dwLength=Maping->jbufsize;
		dwLength=dwLength*Maping->jbufnowblock_int/VIM_MARB_JBUF_MAXBLOCK;
		dwLength1=(dwLength>>10) + VIM_MARB_JBUF_GAPINTERVAL;
		VIM_HIF_SetReg16(V5_REG_MARB_VFIFO_THRESHOLD_0,dwLength1);
		dwLength1=(dwLength>>10) -VIM_MARB_JBUF_GAPINTERVAL;
		VIM_HIF_SetReg16(V5_REG_MARB_VFIFO_THRESHOLD_0+2,dwLength1);
		/*VIM_HIF_SetReg16(V5_REG_MARB_VFIFO_THRESHOLD_0,(UINT16)(dwLength>>10 + VIM_MARB_JBUF_GAPINTERVAL));
		VIM_HIF_SetReg16(V5_REG_MARB_VFIFO_THRESHOLD_0+2,(UINT16)(dwLength>>10 -VIM_MARB_JBUF_GAPINTERVAL));*/
		#if 1
		{
			VIM_USER_PrintHex("[VC0578B][MARB]  LBuf end=", Maping->lbufend);
			VIM_USER_PrintHex("[VC0578B][MARB]  LBuf start=", Maping->lbuf0Ystart);			
			VIM_USER_PrintHex("[VC0578B][MARB]  Total start =", Maping->Total_start);
			VIM_USER_PrintHex("[VC0578B][MARB]  Total size =", Maping->Total_end-Maping->Total_start);
		}
		#endif	
		break;	
	case VIM_MARB_DECODERGBBYPASS_MODE:
	case VIM_MARB_DECODEYUV_MODE:
	case VIM_MARB_DECODERGB_MODE:
		// display JPG use this mode 
		// set line buf, JPG display not use PINGPANG mode, only set one buf0 is OK
		//line buf 0
		YUVMode=VIM_JPEG_GetYUVMode();
		VIM_JPEG_GetSize(&size);

		
		//set v0 buf
		dwLength=VIM_MARB_GetDec0LbufSize(YUVMode,VIM_JPEG_CR,size.cx);
		
		if(dwLength)
		{
			if(Maping->lbuf0Vstart!=0)
			{
				MMD_SYS_Free(Maping->lbuf0Vstart);
				Maping->lbuf0Vstart=0;
			}	
		
			cursor=MMD_SYS_Malloc(dwLength);
			if(cursor==NULL)
				return VIM_ERROR_MARB_RAMINFO_SRAM_NOSPACE;
		}
		else
		{
			cursor=0;
		}
		Maping->lbuf0Vstart=cursor;
		//set u0 buf
		dwLength=VIM_MARB_GetDec0LbufSize(YUVMode,VIM_JPEG_CB,size.cx);
				
		if(dwLength)
		{
			if(Maping->lbuf0Ustart!=0)
			{
				MMD_SYS_Free(Maping->lbuf0Ustart);
				Maping->lbuf0Ustart=0;
			}	
		
			cursor=MMD_SYS_Malloc(dwLength);
			if(cursor==NULL)
				return VIM_ERROR_MARB_RAMINFO_SRAM_NOSPACE;
		}
		else
		{
			cursor=0;
		}
		Maping->lbuf0Ustart=cursor;
		
		//set Y0 buf
		dwLength=VIM_MARB_GetDec0LbufSize(YUVMode,VIM_JPEG_Y,size.cx);
				
		if(dwLength)
		{
			if(Maping->lbuf0Ystart!=0)
			{
				MMD_SYS_Free(Maping->lbuf0Ystart);
				Maping->lbuf0Ystart=0;
			}	
		
			cursor=MMD_SYS_Malloc(dwLength);
			if(cursor==NULL)
				return VIM_ERROR_MARB_RAMINFO_SRAM_NOSPACE;
		}
		else
		{
			cursor=0;
		}

		Maping->lbuf0Ystart=cursor;

		VIM_IPP_GetDispalyWindowSize(&size.cx,&size.cy);		//maybe shoule be the display 
		
//set Y1 buf
		dwLength=VIM_MARB_GetDec1LbufSize(YUVMode,VIM_JPEG_Y,size.cx);
		if(dwLength)
		{
			if(Maping->lbuf1Ystart!=0)
			{
				MMD_SYS_Free(Maping->lbuf1Ystart);
				Maping->lbuf1Ystart=0;
			}	
		
			cursor=MMD_SYS_Malloc(dwLength);
			if(cursor==NULL)
				return VIM_ERROR_MARB_RAMINFO_SRAM_NOSPACE;
		}
		else
		{
			cursor=0;
		}
		Maping->lbuf1Ystart=cursor;

		VIM_HIF_SetReg32(V5_REG_LBUF_Y_INI0_DEC,(UINT32)Maping->lbuf0Ystart);
		VIM_HIF_SetReg32(V5_REG_LBUF_CB_INI0_DEC,(UINT32)Maping->lbuf0Ustart);
		VIM_HIF_SetReg32(V5_REG_LBUF_CR_INI0_DEC,(UINT32)Maping->lbuf0Vstart);

		VIM_HIF_SetReg32(V5_REG_LBUF_Y_INI1_DEC,(UINT32)Maping->lbuf1Ystart);
		//VIM_HIF_SetReg32(V5_REG_LBUF_CB_INI1_DEC,(UINT32)Maping->lbuf1Ustart);
		//VIM_HIF_SetReg32(V5_REG_LBUF_CR_INI1_DEC,(UINT32)Maping->lbuf1Vstart);

		// set JPG Fifo , use virtual Fifo1 for JPG
		VIM_HIF_SetReg32(V5_REG_JPEG_MARB_STARTADDR,0x80000000);
		Maping->jbufsize = DISP_VFIFO_LEN;
		dwLength = Maping->jbufsize+1024;
        	if(Maping->jbufstart!=0)
		{
			MMD_SYS_Free(Maping->jbufstart);
			Maping->jbufstart=0;
		}
		cursor=MMD_SYS_Malloc(dwLength);
		if(cursor==NULL)
			return VIM_ERROR_MARB_RAMINFO_SRAM_NOSPACE;
		//Maping->Total_start = cursor;
		Maping->jbufstart=cursor;
		cursor=(((cursor+1023)>>10)<<10);
		
		// JPEG display should set FiFo in SRAM for hardware bug
		VIM_MARB_SetFifoAddLen(VIM_MARB_FIFO_0, cursor, Maping->jbufsize);

		// set JBUF up thur
		dwLength=Maping->jbufsize;
		dwLength=dwLength*Maping->jbufnowblock_int/VIM_MARB_JBUF_MAXBLOCK;
		dwLength1=(dwLength>>10) + VIM_MARB_JBUF_GAPINTERVAL;
		VIM_HIF_SetReg16(V5_REG_MARB_VFIFO_THRESHOLD_0,dwLength1);
		dwLength1=(dwLength>>10) -VIM_MARB_JBUF_GAPINTERVAL;
		VIM_HIF_SetReg16(V5_REG_MARB_VFIFO_THRESHOLD_0+2,dwLength1);
		/*VIM_HIF_SetReg16(V5_REG_MARB_VFIFO_THRESHOLD_0,(UINT16)(dwLength>>10 + VIM_MARB_JBUF_GAPINTERVAL));
		VIM_HIF_SetReg16(V5_REG_MARB_VFIFO_THRESHOLD_0+2,(UINT16)(dwLength>>10 -VIM_MARB_JBUF_GAPINTERVAL));*/
		#if 1
		{
			VIM_USER_PrintHex("[VC0578B][MARB]  LBuf end=", Maping->lbufend);
			VIM_USER_PrintHex("[VC0578B][MARB]  LBuf start=", Maping->lbuf0Ystart);			
			VIM_USER_PrintHex("[VC0578B][MARB]  Total start =", Maping->Total_start);
			VIM_USER_PrintHex("[VC0578B][MARB]  Total size =", Maping->Total_end-Maping->Total_start);
		}
		#endif				
		break;	
		
	case VIM_MARB_ENCODE_MODE:
		VIM_HIF_SetReg32(V5_REG_JPEG_MARB_STARTADDR,0x80000000);
		Maping->jbufsize = CAP_VFIFO_LEN;
		dwLength = Maping->jbufsize+1024;
		if(Maping->jbufstart!=0)
		{
			MMD_SYS_Free(Maping->jbufstart);
			Maping->jbufstart=0;
		}

		cursor=MMD_SYS_Malloc(dwLength);
		if(cursor==NULL)
			return VIM_ERROR_MARB_RAMINFO_SRAM_NOSPACE;
		//Maping->Total_start = cursor;
		Maping->jbufstart=cursor;
		cursor=(((cursor+1023)>>10)<<10);
		
		VIM_MARB_SetFifoAddLen(VIM_MARB_FIFO_0, cursor, Maping->jbufsize);
		// set JBUF up thur
		dwLength=Maping->jbufsize;
		dwLength=dwLength*Maping->jbufnowblock_int/VIM_MARB_JBUF_MAXBLOCK;
		dwLength1=(dwLength>>10) + VIM_MARB_JBUF_GAPINTERVAL;
		VIM_HIF_SetReg16(V5_REG_MARB_VFIFO_THRESHOLD_0,dwLength1);
		dwLength1=(dwLength>>10) -VIM_MARB_JBUF_GAPINTERVAL;
		VIM_HIF_SetReg16(V5_REG_MARB_VFIFO_THRESHOLD_0+2,dwLength1);
		break;
		
	case VIM_MARB_CAPTUREMPG4_MODE:
	case VIM_MARB_MP4_PREVIEW_MODE:
		// Now  we only support MBL, PINGPANG mode, YUV420
		// LBuf set, MP4 enc hardware requires special memory map, so must setting as followed,
		// For details, please refer to VENC MAS P.34 "Figure 6.11 MBL PINGPANG buffer allocation"
		
		VIM_MPEG4_GetVolSize(&size);
		
		 //revised by youhai 2008.3.17

// Set rec/ref buffer
		dwLength = (((size.cx*size.cy+0x3)>>2)<<2)+(((size.cx*size.cy/4+0x3)>>2)<<2)*2;
		dwLength <<=1;
		if(dwLength)
		{
    		if(Maping->Total_start!=0)
    		{
    			MMD_SYS_Free(Maping->Total_start);
    			Maping->Total_start=0;
    		}		
		
			cursor=MMD_SYS_Malloc(dwLength);
			if(cursor==NULL)
				return VIM_ERROR_MARB_RAMINFO_SRAM_NOSPACE;
		}
		else
		{
			cursor=0;
		}

		mp4_start = cursor; 
		
		VIM_HIF_SetReg32(V5_REG_MP4ENC_REC_Y0_ADDR,mp4_start+V5_1TSRAM_BASE);
		mp4_start+=((size.cx*size.cy+0x3)>>2)<<2;
		VIM_HIF_SetReg32(V5_REG_MP4ENC_REC_CB0_ADDR,mp4_start+V5_1TSRAM_BASE);
		mp4_start+=((size.cx*size.cy/4+0x3)>>2)<<2;
		VIM_HIF_SetReg32(V5_REG_MP4ENC_REC_CR0_ADDR,mp4_start+V5_1TSRAM_BASE);
		mp4_start+=((size.cx*size.cy/4+0x3)>>2)<<2;
		VIM_HIF_SetReg32(V5_REG_MP4ENC_REC_Y1_ADDR,mp4_start+V5_1TSRAM_BASE);
		mp4_start+=((size.cx*size.cy+0x3)>>2)<<2;
		VIM_HIF_SetReg32(V5_REG_MP4ENC_REC_CB1_ADDR,mp4_start+V5_1TSRAM_BASE);
		mp4_start+=((size.cx*size.cy/4+0x3)>>2)<<2;
		VIM_HIF_SetReg32(V5_REG_MP4ENC_REC_CR1_ADDR,mp4_start+V5_1TSRAM_BASE);
		mp4_start+=((size.cx*size.cy/4+0x3)>>2)<<2;
		//cursor-=dwLength;
		Maping->Total_start= cursor;

           	dwLength=size.cx*size.cy/4;
           
           	if(dwLength)
           	{
           		if(Maping->lbuf0Ustart!=0)
           		{
           			MMD_SYS_Free(Maping->lbuf0Ustart);
           			Maping->lbuf0Ustart=0;
           		}	
           		
           		cursor=MMD_SYS_Malloc(dwLength);
           		if(cursor==NULL)
           			return VIM_ERROR_MARB_RAMINFO_SRAM_NOSPACE;
           	}
           	else
           	{
           			cursor=0;
           	}
       		
   		Maping->lbuf0Ustart=cursor;
   		Maping->lbuf1Ustart=cursor+size.cx*4;
   
   		
   		dwLength=size.cx*size.cy/4;
   			
   		if(dwLength)
   		{
   			if(Maping->lbuf0Vstart!=0)
   			{
   				MMD_SYS_Free(Maping->lbuf0Vstart);
   				Maping->lbuf0Vstart=0;
   			}	
   			
   			cursor=MMD_SYS_Malloc(dwLength);
   			if(cursor==NULL)
   				return VIM_ERROR_MARB_RAMINFO_SRAM_NOSPACE;
   		}
   		else
   		{
   			cursor=0;
   		}
   		
   		Maping->lbuf0Vstart=cursor;
   		Maping->lbuf1Vstart=cursor+size.cx*4;
   
   		
   		//dwLength=size.cx*size.cy;
   		dwLength=size.cx*32;
   	
   		if(dwLength)
   		{
   			if(Maping->lbuf0Ystart!=0)
   			{
   				MMD_SYS_Free(Maping->lbuf0Ystart);
   				Maping->lbuf0Ystart=0;
   			}	
   			
   			cursor=MMD_SYS_Malloc(dwLength);
   			if(cursor==NULL)
   				return VIM_ERROR_MARB_RAMINFO_SRAM_NOSPACE;
   		}
   		else
   		{
   			cursor=0;
   		}
   		
   		Maping->lbuf0Ystart=cursor;
   		Maping->lbuf1Ystart=cursor+size.cx*16;
		
		VIM_HIF_SetReg32(V5_REG_LBUF_Y_INI0_ENC,(UINT32)Maping->lbuf0Ystart+V5_1TSRAM_BASE);
		VIM_HIF_SetReg32(V5_REG_LBUF_CB_INI0_ENC,(UINT32)Maping->lbuf0Ustart+V5_1TSRAM_BASE);
		VIM_HIF_SetReg32(V5_REG_LBUF_CR_INI0_ENC,(UINT32)Maping->lbuf0Vstart+V5_1TSRAM_BASE);
		VIM_HIF_SetReg32(V5_REG_LBUF_Y_INI1_ENC,(UINT32)Maping->lbuf1Ystart+V5_1TSRAM_BASE);
		VIM_HIF_SetReg32(V5_REG_LBUF_CB_INI1_ENC,(UINT32)Maping->lbuf1Ustart+V5_1TSRAM_BASE);
		VIM_HIF_SetReg32(V5_REG_LBUF_CR_INI1_ENC,(UINT32)Maping->lbuf1Vstart+V5_1TSRAM_BASE);

		VIM_HIF_SetReg32(V5_REG_MP4ENC_SRC_Y_ADDR,(UINT32)Maping->lbuf0Ystart+V5_1TSRAM_BASE);
		VIM_HIF_SetReg32(V5_REG_MP4ENC_SRC_CB_ADDR,(UINT32)Maping->lbuf0Ustart+V5_1TSRAM_BASE);
		VIM_HIF_SetReg32(V5_REG_MP4ENC_SRC_CR_ADDR,(UINT32)Maping->lbuf0Vstart+V5_1TSRAM_BASE);
		VIM_HIF_SetReg32(V5_REG_MP4ENC_SRC_Y1_ADDR,(UINT32)Maping->lbuf1Ystart+V5_1TSRAM_BASE);
		VIM_HIF_SetReg32(V5_REG_MP4ENC_SRC_CB1_ADDR,(UINT32)Maping->lbuf1Ustart+V5_1TSRAM_BASE);
		VIM_HIF_SetReg32(V5_REG_MP4ENC_SRC_CR1_ADDR,(UINT32)Maping->lbuf1Vstart+V5_1TSRAM_BASE);


		#if 1
		{
			VIM_USER_PrintHex("[VC0578B][MARB]  LBuf end=", Maping->lbufend);
			VIM_USER_PrintHex("[VC0578B][MARB]  LBuf start=", Maping->lbuf0Ustart);			
			VIM_USER_PrintHex("[VC0578B][MARB]  Ref end=", mp4_start);
			VIM_USER_PrintHex("[VC0578B][MARB]  Ref start=", cursor);			
			VIM_USER_PrintHex("[VC0578B][MARB]  Total start =", Maping->Total_start);
			VIM_USER_PrintHex("[VC0578B][MARB]  Total size =", Maping->Total_end-Maping->Total_start);
		}
		#endif
		break;
#if 0
	case VIM_MARB_CAPTURESTILLTHUMB_MODE:
		//(1) set line buf
		ret = VIM_MARB_RamInfo_GetCursor(MARB_SDRAM, MARB_FROM_TOP,&cursor);
		DRV_ASSERT(ret==VIM_SUCCEED);
		Maping->lbufend=cursor;
		//line buf 0
		YUVMode=VIM_JPEG_GetYUVMode();
		VIM_JPEG_GetSize(&size);

		//set v0 buf
		dwLength=VIM_MARB_GetDec0LbufSize(YUVMode,VIM_JPEG_CR,size.cx);
		ret = VIM_MARB_RamInfo_Alloc_FromTop(MARB_SDRAM, dwLength, 4, &cursor);
		DRV_ASSERT(ret==VIM_SUCCEED);
		Maping->lbuf0Vstart=cursor;
		//set u0 buf
		dwLength=VIM_MARB_GetDec0LbufSize(YUVMode,VIM_JPEG_CB,size.cx);
		ret = VIM_MARB_RamInfo_Alloc_FromTop(MARB_SDRAM, dwLength, 4, &cursor);
		DRV_ASSERT(ret==VIM_SUCCEED);
		Maping->lbuf0Ustart=cursor;
		//set Y0 buf
		dwLength=VIM_MARB_GetDec0LbufSize(YUVMode,VIM_JPEG_Y,size.cx);
		ret = VIM_MARB_RamInfo_Alloc_FromTop(MARB_SDRAM, dwLength, 4, &cursor);
		DRV_ASSERT(ret==VIM_SUCCEED);
		Maping->lbuf0Ystart=cursor;

		//set v1 buf
		dwLength=VIM_MARB_GetDec0LbufSize(YUVMode,VIM_JPEG_CR,size.cx);
		ret = VIM_MARB_RamInfo_Alloc_FromTop(MARB_SDRAM, dwLength, 4, &cursor);
		DRV_ASSERT(ret==VIM_SUCCEED);
		Maping->lbuf1Vstart=cursor;
		//set u1 buf
		dwLength=VIM_MARB_GetDec0LbufSize(YUVMode,VIM_JPEG_CB,size.cx);
		ret = VIM_MARB_RamInfo_Alloc_FromTop(MARB_SDRAM, dwLength, 4, &cursor);
		DRV_ASSERT(ret==VIM_SUCCEED);
		Maping->lbuf1Ustart=cursor;
		//set Y1 buf
		dwLength=VIM_MARB_GetDec0LbufSize(YUVMode,VIM_JPEG_Y,size.cx);
		ret = VIM_MARB_RamInfo_Alloc_FromTop(MARB_SDRAM, dwLength, 4, &cursor);
		DRV_ASSERT(ret==VIM_SUCCEED);
		Maping->lbuf1Ystart=cursor;

		VIM_HIF_SetReg32(V5_REG_LBUF_Y_INI0_ENC,(UINT32)Maping->lbuf0Ystart+V5_1TSRAM_BASE);
		VIM_HIF_SetReg32(V5_REG_LBUF_CB_INI0_ENC,(UINT32)Maping->lbuf0Ustart+V5_1TSRAM_BASE);
		VIM_HIF_SetReg32(V5_REG_LBUF_CR_INI0_ENC,(UINT32)Maping->lbuf0Vstart+V5_1TSRAM_BASE);

		VIM_HIF_SetReg32(V5_REG_LBUF_Y_INI1_ENC,(UINT32)Maping->lbuf1Ystart+V5_1TSRAM_BASE);
		VIM_HIF_SetReg32(V5_REG_LBUF_CB_INI1_ENC,(UINT32)Maping->lbuf1Ustart+V5_1TSRAM_BASE);
		VIM_HIF_SetReg32(V5_REG_LBUF_CR_INI1_ENC,(UINT32)Maping->lbuf1Vstart+V5_1TSRAM_BASE);

		//(2) set thumbnail buffer
		VIM_IPP_GetThumbSize(&size);
		dwLength=VIM_MARB_GetBmemSize(VIM_DISP_BLAYER_RGB565,size);
		if(!dwLength)
			return VIM_ERROR_THUMB_MAP;
		Maping->thumbsize=dwLength+4;
		dwLength = Maping->thumbsize;
		ret = VIM_MARB_RamInfo_Alloc_FromTop(MARB_SDRAM, dwLength, 4, &cursor);
		DRV_ASSERT(ret==VIM_SUCCEED);
		Maping->thumbstart = cursor;
		VIM_IPP_SetThumbbufMem(Maping->thumbstart,Maping->thumbsize);

		//(3) set JPG Fifo , use virtual Fifo1 for JPG
		VIM_HIF_SetReg32(V5_REG_JPEG_MARB_STARTADDR,0x80000000);
		Maping->jbufsize = CAP_VFIFO_LEN;
		dwLength = Maping->jbufsize;
		// FiFo address should be aligned at 1K 
		ret = VIM_MARB_RamInfo_Alloc_FromTop(MARB_SDRAM, dwLength, 1024, &cursor);
		DRV_ASSERT(ret==VIM_SUCCEED);
		Maping->Total_start = cursor;
		VIM_MARB_SetFifoAddLen(VIM_MARB_FIFO_0, cursor+V5_1TSRAM_BASE, Maping->jbufsize);

		// set JBUF up thur
		dwLength=Maping->jbufsize;
		dwLength=dwLength*Maping->jbufnowblock_int/VIM_MARB_JBUF_MAXBLOCK;
		dwLength1=(dwLength>>10) + VIM_MARB_JBUF_GAPINTERVAL;
		VIM_HIF_SetReg16(V5_REG_MARB_VFIFO_THRESHOLD_0,dwLength1);
		dwLength1=(dwLength>>10) -VIM_MARB_JBUF_GAPINTERVAL;
		VIM_HIF_SetReg16(V5_REG_MARB_VFIFO_THRESHOLD_0+2,dwLength1);
		/*VIM_HIF_SetReg16(V5_REG_MARB_VFIFO_THRESHOLD_0,(UINT16)(dwLength>>10 + VIM_MARB_JBUF_GAPINTERVAL));
		VIM_HIF_SetReg16(V5_REG_MARB_VFIFO_THRESHOLD_0+2,(UINT16)(dwLength>>10 -VIM_MARB_JBUF_GAPINTERVAL));*/
		#if 1
		{
			VIM_USER_PrintHex("[VC0578B][MARB]  LBuf end=", Maping->lbufend);
			VIM_USER_PrintHex("[VC0578B][MARB]  LBuf start=", Maping->lbuf1Ystart);			
			VIM_USER_PrintHex("[VC0578B][MARB]  Thumb end=", Maping->thumbstart+Maping->thumbsize);
			VIM_USER_PrintHex("[VC0578B][MARB]  Thumb start=", Maping->thumbstart);			
			VIM_USER_PrintHex("[VC0578B][MARB]  Total start =", Maping->Total_start);
			VIM_USER_PrintHex("[VC0578B][MARB]  Total size =", Maping->Total_end-Maping->Total_start);
		}
		#endif	

		break;
#endif		
	default:
		break;
	}

	#if 1
	{
		UINT32 freesize,addr,ret;
		ret = VIM_MARB_RamInfo_GetCursor(MARB_SDRAM, MARB_FROM_TOP,&addr);
		ret = VIM_MARB_RamInfo_GetFreesize(MARB_SDRAM, &freesize);		
		VIM_USER_PrintHex("[VC0578B][MARB]  MARB_SDRAM freespace end=", addr);			
		VIM_USER_PrintHex("[VC0578B][MARB]  MARB_SDRAM freesize =", freesize);					

		ret = VIM_MARB_RamInfo_GetCursor(MARB_SRAM, MARB_FROM_TOP,&addr);
		ret = VIM_MARB_RamInfo_GetFreesize(MARB_SRAM, &freesize);		
		VIM_USER_PrintHex("[VC0578B][MARB]  MARB_SRAM freespace end=", addr);			
		VIM_USER_PrintHex("[VC0578B][MARB]  MARB_SRAM freesize =", freesize);					

	}
	
	#endif
	
	return VIM_SUCCEED;
}


/********************************************************************************
  Description:
  	set LCD maping list
  Parameters:
  Returns:
  	void
  Remarks:
*********************************************************************************/


VIM_RESULT VIM_MARB_SetLCDMap(PVIM_MARB_Map Maping)
{
	VIM_DISP_BFORMAT fmt;
	//TSize size;//,size1;
	UINT32 dwLength;//,dwLength1;
	//VIM_JPEG_YUVMODE YUVMode;
	UINT32 cursor;



	//------(1) common setting, LCDC B layer setting
	// Set b 1 layer memary;


	VIM_DISP_GetGbufFormat(&fmt);
	//modify  by lrx
	//VIM_DISP_GetBSize(VIM_DISP_B1LAYER,&size);
	//dwLength=VIM_MARB_GetBmemSize(fmt,size);
	
	dwLength=VIM_MARB_GetBmemSize(fmt,gb1lcdsize);

	if(dwLength)
	{
		if((Maping->layerB1start!=0  && Maping->layerB1start < 0x40000000) ||(Maping->layerB1start!=0  &&  MMD_SYS_Mode_Get() > 2) )
		{
			MMD_SYS_Free(Maping->layerB1start);
		}
#if LCDCA0SDRAM
		if(MMD_SYS_Mode_Get() <3)
		//cursor = 0x40000000;
			cursor = LCDCSDRAMB1ADDR;
		else
			cursor=MMD_SYS_Malloc(dwLength);
#else
		cursor=MMD_SYS_Malloc(dwLength);
#endif
		//printf("MMD_SYS_Malloc B buffer (0x%x)\n",cursor);
		if(cursor==NULL)
			return VIM_ERROR_MARB_RAMINFO_SRAM_NOSPACE;
	}
	else
	{
		cursor=0;
	}

	Maping->layerB1start = cursor;
	VIM_HIF_SetReg32(V5_REG_LCDC_GBBA1,Maping->layerB1start+V5_1TSRAM_BASE);

	// Set b 0 layer memary;
	
	VIM_DISP_GetGbufFormat(&fmt);
	//VIM_DISP_GetBSize(VIM_DISP_B0LAYER,&size);
	//dwLength=VIM_MARB_GetBmemSize(fmt,size);
	dwLength=VIM_MARB_GetBmemSize(fmt,gb0lcdsize);

	if(dwLength)
	{
		if((Maping->layerB0start!=0  && Maping->layerB0start < 0x40000000) ||(Maping->layerB0start!=0  &&  MMD_SYS_Mode_Get() > 2) )
		{
			MMD_SYS_Free(Maping->layerB0start);
		}
#if LCDCA0SDRAM
		if(MMD_SYS_Mode_Get() <3)
		//cursor = 0x40000000;
			cursor = LCDCSDRAMB0ADDR;
		else
			cursor=MMD_SYS_Malloc(dwLength);
#else
		cursor=MMD_SYS_Malloc(dwLength);
#endif
		//printf("MMD_SYS_Malloc B buffer (0x%x)\n",cursor);
		if(cursor==NULL)
			return VIM_ERROR_MARB_RAMINFO_SRAM_NOSPACE;
	}
	else
	{
		cursor=0;
	}
	
	Maping->layerB0start = cursor;
	//printf("Malloc Maping->layerB0start (0x%x) \n",Maping->layerB0start);
	VIM_HIF_SetReg32(V5_REG_LCDC_GBBA0,Maping->layerB0start+V5_1TSRAM_BASE);

	#if 1
	{
		VIM_USER_PrintHex("[VC0578B][MARB]  b1 end=", Maping->layerB1end);
		VIM_USER_PrintHex("[VC0578B][MARB]  b1 start=", Maping->layerB1start);
		VIM_USER_PrintHex("[VC0578B][MARB]  b0 end=", Maping->layerB0end);
		VIM_USER_PrintHex("[VC0578B][MARB]  b0 start=", Maping->layerB0start);
	}		
	#endif
	return VIM_SUCCEED;
}


/***************************************************************************************************
Description:
	MARB read jpeg data
	start address is 0
	end  get from register
parameters: 
	*Start:       jpeg data start position
	length:       data length
Return:
		VIM_SUCCEED: cacaulte ok
		VIM_ERROR_BUFLENS: buffer length error
Remarks:
*****************************************************************************************************/
UINT32 VIM_MARB_ReadJpegData(HUGE UINT8 *Start, UINT32 length)
{
	if(Start==NULL)
		return 0;

	return VIM_HIF_ReadSram(0x80000000,Start, length,VIM_HIF_SRAM_MODE);
}
/***************************************************************************************************
Description:
	MARB read thumb data
	start address is 0
	end  get from register
parameters: 
	*Start:       thumb data start position
	length:       data length
Return:
		VIM_SUCCEED: cacaulte ok
		VIM_ERROR_BUFLENS: buffer length error
Remarks:
*****************************************************************************************************/
void VIM_MARB_ReadThumbData(HUGE UINT8 *Start, UINT32 length)
{
	UINT32 dwStartAddr=0;
	dwStartAddr=VIM_HIF_GetReg32(V5_REG_MARB_VFIFO_STARTADDR_1);

	if(Start==NULL)
		return ;
	VIM_HIF_ReadSram(dwStartAddr,Start, length,VIM_HIF_FIFO_MODE);
}

/***************************************************************************************************
Description:
	MARB write jpeg data
	start address is 0
	end  get from register
parameters: 
	*Start:       jpeg data start position
	length:       data length
	pos:		   write position
Return:
		VIM_SUCCEED: cacaulte ok
		VIM_ERROR_BUFLENS: buffer length error
Remarks:
*****************************************************************************************************/
void VIM_MARB_WriteJpegData(HUGE UINT8 *Start,UINT32 length,UINT8 end)
{
	UINT32 dwStartAddr=0,LeftLen;

	UINT8 dat[128]={0},i;


	
	dwStartAddr=VIM_HIF_GetReg32(V5_REG_MARB_VFIFO_STARTADDR_0);

	if(!end)
	{
#if (BOARD_TYPE==VC0820_FPGA)//add by zhoujian
		VIM_HIF_WriteSram(dwStartAddr-V5_1TSRAM_BASE,Start, length,VIM_HIF_FIFO_0_MODE);
#else
		VIM_HIF_WriteSram(dwStartAddr-V5_1TSRAM_BASE,Start, length,VIM_HIF_FIFO_MODE);
#endif

		return ;
	}
	else 
	{
		LeftLen = (unsigned char)(length&0x7F);

		if(LeftLen)	//because multi16 write data by even, so when file end length maybe is odd
		{
#if(VIM_MARB_DEBUG)&&(VIM_5XRDK_DEBUG)
			VIM_USER_PrintDec("[VC0578B][MARB] LeftLen : ", LeftLen);
#endif
			dat[0]=Start[length-1];	

			for(i=0; i<(int)LeftLen; i++){
				dat[LeftLen-i-1] = Start[length-i-1];
			}
				length = (length>>7)<<7;
		}
		
#if (BOARD_TYPE==VC0820_FPGA)//add by zhoujian
		VIM_HIF_WriteSram(dwStartAddr-V5_1TSRAM_BASE,Start, length,VIM_HIF_FIFO_0_MODE);
#else
		VIM_HIF_WriteSram(dwStartAddr-V5_1TSRAM_BASE,Start, length,VIM_HIF_FIFO_MODE);
#endif

		if(LeftLen)
		{
#if (BOARD_TYPE==VC0820_FPGA)//add by zhoujian
		VIM_HIF_WriteSram(dwStartAddr-V5_1TSRAM_BASE,dat, 128,VIM_HIF_FIFO_0_MODE);
#else
		VIM_HIF_WriteSram(dwStartAddr-V5_1TSRAM_BASE,dat, 128,VIM_HIF_FIFO_MODE);
#endif

#if(VIM_MARB_DEBUG)&&(VIM_5XRDK_DEBUG)
			VIM_USER_PrintHex("[VC0578B][MARB]  WriteSram : ", 128);
#endif
		}
		return ;
	}
	
}
/***************************************************************************************************
Description:
	MARB write jpeg data
	start address is 0
	end  get from register
parameters: 
	*Start:       jpeg data start position
	length:       data length
	pos:		   write position
Return:
		VIM_SUCCEED: cacaulte ok
		VIM_ERROR_BUFLENS: buffer length error
Remarks:
*****************************************************************************************************/
void VIM_MARB_WriteMp4Data(HUGE UINT8 *Start,UINT32 length,UINT8 end)
{
#if(BOARD_TYPE==VC0820_FPGA)
	//VIM_USER_MemCpy((UINT8*)gVc0578b_Info.MarbStatus.vdec_bitstream_buf, Start, length);
	VIM_HIF_SetReg32(V5_REG_MP4DEC_BITSTREAM_INI_ADD,(UINT32)Start);
	VIM_USER_PrintHex("[VC0578B][MARB] Addr=:", (UINT32)Start);
	return;
#else
	UINT32 dwStartAddr=0,LeftLen;

	UINT8 dat[128]={0},i;


	
	dwStartAddr=VIM_HIF_GetReg32(V5_REG_MARB_VFIFO_STARTADDR_1);

	if(!end)
	{
		LeftLen = (unsigned char)(length&0x3);

		if(LeftLen)	//because multi16 write data by even, so when file end length maybe is odd
		{
#if(VIM_MARB_DEBUG)&&(VIM_5XRDK_DEBUG)
			VIM_USER_PrintDec("[VC0578B][MARB] LeftLen : *******************************", LeftLen);
#endif
			dat[0]=dat[1]=dat[2]=dat[3]=0xff;	

			for(i=0; i<(int)LeftLen; i++){
				dat[LeftLen-i-1] = Start[length-i-1];
			}
			length = (length>>3)<<3;
		}
#if(BOARD_TYPE==VC0820_FPGA)
		VIM_HIF_WriteSram(dwStartAddr-V5_1TSRAM_BASE,Start, length,VIM_HIF_FIFO_1_MODE);
		if(LeftLen)
			VIM_HIF_WriteSram(dwStartAddr-V5_1TSRAM_BASE,dat, 4,VIM_HIF_FIFO_1_MODE);
#else
		VIM_HIF_WriteSram(dwStartAddr-V5_1TSRAM_BASE,Start, length,VIM_HIF_FIFO_MODE);
		if(LeftLen)
			VIM_HIF_WriteSram(dwStartAddr-V5_1TSRAM_BASE,dat, 4,VIM_HIF_FIFO_MODE);
#endif
		return ;
	}
	else 
	{
		LeftLen = (unsigned char)(length&0x7F);

		if(LeftLen)	//because multi16 write data by even, so when file end length maybe is odd
		{
//#if(VIM_MARB_DEBUG)&&(VIM_5XRDK_DEBUG)
			VIM_USER_PrintDec("[VC0578B][MARB] LeftLen : *******************************", LeftLen);
//#endif
			dat[0]=Start[length-1];	

			for(i=0; i<(int)LeftLen; i++){
				dat[LeftLen-i-1] = Start[length-i-1];
			}
			length = (length>>7)<<7;
		}
#if(BOARD_TYPE==VC0820_FPGA)
		VIM_HIF_WriteSram(dwStartAddr-V5_1TSRAM_BASE,Start, length,VIM_HIF_FIFO_1_MODE);

		if(LeftLen)
		{
			VIM_HIF_WriteSram(dwStartAddr-V5_1TSRAM_BASE,dat, 128,VIM_HIF_FIFO_1_MODE);
#if(VIM_MARB_DEBUG)&&(VIM_5XRDK_DEBUG)
			VIM_USER_PrintHex("[VC0578B][MARB]  WriteSram : ", 128);
#endif
		}
#else
		VIM_HIF_WriteSram(dwStartAddr-V5_1TSRAM_BASE,Start, length,VIM_HIF_FIFO_MODE);

		if(LeftLen)
		{
			VIM_HIF_WriteSram(dwStartAddr-V5_1TSRAM_BASE,dat, 128,VIM_HIF_FIFO_MODE);
#if(VIM_MARB_DEBUG)&&(VIM_5XRDK_DEBUG)
			VIM_USER_PrintHex("[VC0578B][MARB]  WriteSram : ", 128);
#endif
		}

#endif
		return ;
	}
#endif	
}
/********************************************************************************
  Description:
  	interrupt of marb
  Parameters:
  Returns:
  	void
  Remarks:
*********************************************************************************/
static const UINT8 gMarbIntPriority[15]	= 
{ 
	MARB_INT_ADDERROR, MARB_INT_FIFO0_FULL, MARB_INT_FIFO1_FULL, MARB_INT_FIFO0_LARGE,MARB_INT_FIFO1_LARGE,
	MARB_INT_FIFO0_THRESH_UP, MARB_INT_FIFO1_THRESH_UP, MARB_INT_FIFO0_THRESH_LOW, MARB_INT_FIFO1_THRESH_LOW, 
	MARB_INT_FIFO0_FRAME_END, MARB_INT_FIFO1_FRAME_END, MARB_INT_FIFO0_DONE,  MARB_INT_FIFO1_DONE,MARB_INT_FIFO0_EMPTY,
	MARB_INT_FIFO1_EMPTY
};

void _ISR_MarbIntHandle(UINT16 wIntFlag)
{
	UINT8 i = 0;
	UINT16 bySecondLevelInt = 0;

	for(i=0; i<VIM_MARB_INT_NUM; i++)
	{
		bySecondLevelInt = gMarbIntPriority[i];
		if(TSTINT(wIntFlag, bySecondLevelInt))
		{
			if(gVc0578b_Isr.marbisr[bySecondLevelInt])
			{
				gVc0578b_Isr.marbisr[bySecondLevelInt]();
			}
		}
	}
}


/********************************************************************************
  Description:
  	register isr 
  Parameters:

  	bySecondLevelInt:
  			MARB_INT_JBUF_FIFOCNT		= 0,
			MARB_INT_JBUF_RESERV		= 1, //ignore
			MARB_INT_JPEG_DONE			= 2,
			MARB_INT_JBUF_ERR			= 3,
			MARB_INT_TBUF_FIFOCNT		= 4,
			MARB_INT_TBUF_RESERV		= 5, //ignore
			MARB_INT_THUMB_DONE		= 6,
			MARB_INT_THUMB_ERR			= 7,
			MARB_INT_MEM0_REF_ERR		= 8,
			MARB_INT_MEM1_REF_ERR		= 9, //ignore
			MARB_INT_MEM2_REF_ERR		= 10,
			MARB_INT_RECAP_ERROR		=11,
			MARB_INT_RECAP				=12,
			MARB_INT_TIMER				=13,
			MARB_INT_ALL			= VIM_MARB_INT_NUM
  Returns:
  	void
  Remarks:
*********************************************************************************/

void VIM_MARB_RegisterISR(VIM_MARB_INT_TYPE bySecondLevelInt, PVIM_Isr  Isr)
{
UINT8 i;
	if(bySecondLevelInt>=MARB_INT_ALL)
	{
		for(i=0;i<MARB_INT_ALL;i++)
			gVc0578b_Isr.marbisr[i] = Isr;
		return;
	}
	gVc0578b_Isr.marbisr[bySecondLevelInt] = Isr;
}
/********************************************************************************
  Description:
  	set enable marb interrupt
  Parameters:

  	bySecondLevelInt:
		MARB_INT_FIFO0_FULL		= 0,
		MARB_INT_FIFO0_EMPTY		= 1, //ignore
		MARB_INT_FIFO0_THRESH_UP			= 2,
		MARB_INT_FIFO0_THRESH_LOW			= 3,
		MARB_INT_FIFO0_LARGE		= 4,
		MARB_INT_FIFO0_FRAME_END	= 5, //ignore
		MARB_INT_FIFO0_DONE		= 6,
		MARB_INT_FIFO1_FULL			= 8,
		MARB_INT_FIFO1_EMPTY		= 9,
		MARB_INT_FIFO1_THRESH_UP		= 10, //ignore
		MARB_INT_FIFO1_THRESH_LOW		= 11,
		MARB_INT_FIFO1_LARGE		= 12,
		MARB_INT_FIFO1_FRAME_END	= 13, //ignore
		MARB_INT_FIFO1_DONE		= 14,
		MARB_INT_ADDERROR		= 15,
  Returns:
  	void
  Remarks:
*********************************************************************************/
void VIM_MARB_SetIntEnable(VIM_MARB_INT_TYPE bySecondLevelInt,BOOL Eanble)
{
	UINT16 bInten;
	if(bySecondLevelInt<MARB_INT_ALL)
	{
		bInten=VIM_HIF_GetIntEnableSec(VM_HIF_INT_MARB);
		if(Eanble==ENABLE)
			bInten |= (0x1<<bySecondLevelInt);
		else
			bInten &= (~(0x1<<bySecondLevelInt));	
		VIM_HIF_SetIntEnableSec(VM_HIF_INT_MARB,bInten);
		if(bInten)
			VIM_HIF_SetIntModuleEn(VM_HIF_INT_MARB,ENABLE);
		else
			VIM_HIF_SetIntModuleEn(VM_HIF_INT_MARB,DISABLE);
	}
	else
	{
		VIM_HIF_SetIntEnableSec(VM_HIF_INT_MARB,0xffff);
		VIM_HIF_SetIntModuleEn(VM_HIF_INT_MARB,ENABLE);
	}
}

#if(BOARD_TYPE==VC0820_FPGA)
/********************************************************************************
  Description:
  	set fifo work mode
  Parameters:

  	mode:
		MARB_MODE_NORMAL =0,
		MARB_MODE_ARMD_READ=2,
		MARB_MODE_ARMD_WRITE=3
  Returns:
  	void
  Remarks:
*********************************************************************************/
void VIM_MARB_SetFifoMode(VIM_MARB_FIFONUM fifoNum, VIM_MARB_FIFOMODE mode)
{
	UINT32 temp;
	temp = VIM_HIF_GetReg32(V5_REG_MARB_VFIFO_MODE);
	temp &=~(0x3<<fifoNum);
	temp |=(mode<<fifoNum);
	VIM_HIF_SetReg32(V5_REG_MARB_VFIFO_MODE, temp);
}
/********************************************************************************
  Description:
  	set fifo start address and length
  Parameters:
	address:
		Fifo used memory address
	length:
		Fifo length
  Returns:
  	void
  Remarks:
*********************************************************************************/
void VIM_MARB_SetFifoAddLen(VIM_MARB_FIFONUM fifoNum, UINT32 address, UINT32 length)
{
	if(fifoNum==VIM_MARB_FIFO_0)
	{
		VIM_HIF_SetReg32(V5_REG_MARB_VFIFO_STARTADDR_0,address);
		VIM_HIF_SetReg32(V5_REG_MARB_VFIFO_LENGTH_0,length);
		
	}
	else
	{
		VIM_HIF_SetReg32(V5_REG_MARB_VFIFO_STARTADDR_1,address);
		VIM_HIF_SetReg32(V5_REG_MARB_VFIFO_LENGTH_1,length);
	}
	// set virtual Fifo start address
	gVc0578b_Info.MarbStatus.VFifo[fifoNum/2].address=address;	
	gVc0578b_Info.MarbStatus.VFifo[fifoNum/2].length=length;			
	gVc0578b_Info.MarbStatus.VFifo[fifoNum/2].read_cur=0;
	gVc0578b_Info.MarbStatus.VFifo[fifoNum/2].write_cur=0;
}

/********************************************************************************
  Description:
  	set fifo  virtual address 
  Parameters:

  	vaddr:
		only 0x8~0xf	is valid
  Returns:
  	void
  Remarks:
*********************************************************************************/
void VIM_MARB_SetFifoVAddr(VIM_MARB_FIFONUM fifoNum, UINT32 vaddr)
{
	UINT32 temp;
	temp = VIM_HIF_GetReg32(V5_REG_MARB_VFIFO_VADDR);
	temp &=~(0xf<<(fifoNum*2));
	temp |=(vaddr<<(fifoNum*2));
	VIM_HIF_SetReg32(V5_REG_MARB_VFIFO_VADDR, temp);
}

/********************************************************************************
  Description:
  	set fifo count add/sub value, only used after ARMD_WRITE/ARMD_READ operation
  Parameters:

  	vaddr:
		only 0x8~0xf	is valid
  Returns:
  	void
  Remarks:
*********************************************************************************/
void VIM_MARB_SetFifoCount(VIM_MARB_FIFONUM fifoNum,VIM_MARB_FIFOMODE mode, UINT32 count)
{
	if(mode==MARB_MODE_ARMD_READ)
	{
		VIM_HIF_SetReg32(V5_REG_MARB_VFIFO_COUNT_SUB_0+fifoNum*4, count);
	}
	else //(mode==MARB_MODE_ARMD_WRITE)
	{
		VIM_HIF_SetReg32(V5_REG_MARB_VFIFO_COUNT_ADD_0+fifoNum*4, count);
	}
}


#endif

