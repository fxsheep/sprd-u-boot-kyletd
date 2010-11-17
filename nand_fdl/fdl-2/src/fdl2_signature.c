
const unsigned char FDL2_signature[][24]=
{
#ifdef DEV_MANAGE_SUPPORT
    {'#','$','D','E','V','I','C','E','_','M','A','N','A','G','E','R','$','#',0,0,0,0,0,0},
#endif
#ifdef PRELOAD_SUPPORT
    {'#','*','P','R','E','L','O','A','D','S','U','P','P','O','R','T','*','#',0,0,0,0,0,0},
#endif
#ifdef VM_SUPPORT
    {'#','*','D','E','M','A','N','D','P','A','G','I','N','G','*','#', 0,  0, 0,0,0,0,0,0},
#endif
#ifdef USB_VCOM_AUTORUN_SUPPORT
    {'#','*','U','S','B','C','O','M','A','U','T','O','R','U','N','*','#', 0, 0,0,0,0,0,0},
#endif
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};




