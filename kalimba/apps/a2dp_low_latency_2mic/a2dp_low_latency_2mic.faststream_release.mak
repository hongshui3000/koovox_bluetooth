###########################################################
# Makefile generated by xIDE                               
#                                                          
# Project: a2dp_low_latency_2mic
# Configuration: Faststream_Release
# Generated: �ܶ� 8�� 18 09:49:24 2015
#                                                          
# WARNING: Do not edit this file. Any changes will be lost 
#          when the project is rebuilt.                    
#                                                          
###########################################################

OUTPUT=a2dp_low_latency_2mic
OUTDIR=C:/ADK3.5/apps/sink
DEFS=-DKALASM3 -DAUDIO_CBUFFER_SIZE=320 -DCODEC_CBUFFER_SIZE=1000 -DGOOD_WORKING_BUFFER_LEVEL=0.30 -DFASTSTREAM_ENABLE -DxAPTX_ACL_SPRINT_ENABLE -DLATENCY_REPORTINGx 
BOOTSTRAP=1
LIBS=security cvc_system cvc_modules core cbops_multirate codec faststream frame_sync audio_proc spi_comm math sbc_wbs_acl 
ASMS=\
      sr_adjustment_gaming.asm\
      frontend.asm\
      main.asm\
      receive_proc.asm\
      send_proc.asm\
      backend.asm\
      mips_profile.asm\
      latency_reporting_ll.asm
DEBUGTRANSPORT=SPITRANS=USB SPIPORT=0

# Project-specific options
LIB_SET=sdk
debugtransport=[SPITRANS=KALSIM SPIPORT=1]
warpfilter=1
warpratehighcoefficient=0.01
warpratelowcoefficient=0.001
warpratemaxramp=0.005
warpratetransitionlevelwords=100
zerodataratebuffering=200000
zerodataratestopping=150000

-include a2dp_low_latency_2mic.mak
include $(BLUELAB)/Makefile.dsp
