###########################################################
# Makefile generated by xIDE                               
#                                                          
# Project: cvc_handsfree
# Configuration: Release
# Generated: �ܶ� 8�� 18 09:49:23 2015
#                                                          
# WARNING: Do not edit this file. Any changes will be lost 
#          when the project is rebuilt.                    
#                                                          
###########################################################

OUTPUT=cvc_handsfree
OUTDIR=C:/ADK3.5/apps/sink
DEFS=-DKALASM3 -DCVC_WIREDx -DFRAME_SYNC_DEBUGx -DBASE_REGISTER_MODEx -Duses_SSRx 
BOOTSTRAP=1
LIBS=security core frame_sync cbops_multirate cvc_modules spi_comm math plc100 audio_proc cvc_system sbc_wbs ssr_system 
ASMS=\
      cvc_handsfree_main.asm\
      cvc_handsfree_config.asm\
      cvc_BackEnd.asm\
      cvc_FrontEnd.asm
DEBUGTRANSPORT=SPITRANS=USB SPIPORT=0

# Project-specific options
LIB_SET=sdk
debugtransport=[SPITRANS=LPT SPIPORT=1]
separator_flags=0

-include cvc_handsfree.mak
include $(BLUELAB)/Makefile.dsp
