/*****************************************************************
Copyright (C) Cambridge Silicon Radio Ltd. 2011-2014
*/

#ifndef _WECHAT_PRIVATE_H_
#define _WECHAT_PRIVATE_H_

#define MESSAGE_PMAKE(NAME,TYPE) TYPE * const NAME = PanicUnlessNew(TYPE)


#include "wechat.h"

/* configure which transports to include in the library
 * start with RFCOMM defined unless removed by VARIANTS */
#ifdef WECHAT_TRANSPORT_NO_RFCOMM
#undef WECHAT_TRANSPORT_RFCOMM
#else
#define WECHAT_TRANSPORT_RFCOMM
#endif

/* include the MFi header if we're having this transport */
#ifdef WECHAT_TRANSPORT_MFI
#include <iap.h>
#endif

/* include the SPP Client header if we're having this transport */
#ifdef WECHAT_TRANSPORT_SPP
#include <sppc.h>
#endif

/* sanity check; we must have at least one of the transports defined */
#if !defined(WECHAT_TRANSPORT_RFCOMM) && !defined(WECHAT_TRANSPORT_SPP) && !defined(WECHAT_TRANSPORT_MFI)
#error : no transport defined
#endif

#define MAX_SUPPORTED_WECHAT_TRANSPORTS   4           /*!< Maximum number of transports Wechat can support */
/* end transport configuration */

/* assume we have (SQIF) PFS unless denied by VARIANTS */
#ifdef WECHAT_NO_PARTITION_FILESYSTEM
#undef HAVE_PARTITION_FILESYSTEM
#undef HAVE_DFU_FROM_SQIF
#else
#define HAVE_PARTITION_FILESYSTEM
#define HAVE_DFU_FROM_SQIF
#endif

#define WECHAT_RAW_CHUNK_SIZE (240)

/* configure what debug is generated */
#ifdef DEBUG_WECHAT
#include <stdio.h>
#include <panic.h>
#define WECHAT_DEBUG(x) printf x
#define WECHAT_PANIC() Panic()
#else
#define WECHAT_DEBUG(x)
#define WECHAT_PANIC()
#endif

#ifdef DEBUG_WECHAT_COMMAND
#include <stdio.h>
#define WECHAT_CMD_DEBUG(x) printf x
#else
#define WECHAT_CMD_DEBUG(x)
#endif

#ifdef DEBUG_WECHAT_TRANSPORT                         /* transport specific debug */
#include <stdio.h>
#define WECHAT_TRANS_DEBUG(x) printf x
#else
#define WECHAT_TRANS_DEBUG(x)
#endif

#ifdef DEBUG_WECHAT_DFU
#include <stdio.h>
#define WECHAT_DFU_DEBUG(x) printf x
#else
#define WECHAT_DFU_DEBUG(x)
#endif

#ifdef DEBUG_WECHAT_THRESHOLDS
#include <stdio.h>
#define WECHAT_THRES_DEBUG(x) printf x
#else
#define WECHAT_THRES_DEBUG(x)
#endif

/* end debug configuration */

typedef enum
{
    PARTITION_INVALID,
    PARTITION_UNMOUNTED,
    PARTITION_MOUNTED
} partition_state;

#define BAD_SINK ((Sink) 0xFFFF)
#define BAD_SINK_CLAIM (0xFFFF)

#define PS_MAX_BOOTMODE (7)
#define PFS_MAX_PARTITION (15)
#define DFU_MAX_ENABLE_PERIOD (60)
#define DFU_PACKET_TIMEOUT (5)
#define DFU_PARTITION_OVERHEAD (16)

#define PSKEY_BDADDR (0x0001)
#define PSKEY_LOCAL_DEVICE_NAME (0x0108)
#define PSKEY_MODULE_ID (0x0259)
#define PSKEY_MODULE_DESIGN (0x025A)
#define PSKEY_MOD_MANUF0 (0x025E)
#define PSKEY_USB_VENDOR_ID (0x02BE)

#define BIGGISH_SLOT_SIZE (94)
#define DEB_BUFFER_SIZE BIGGISH_SLOT_SIZE

/* file packet header + data to fit in a 64-word pmalloc slot */
#define FILE_PACKET_HEADER_SIZE (5)
#define FILE_BUFFER_SIZE (58)

#define TATA_TIME (1000)
#define APP_BUSY_WAIT_MILLIS (500)

#define HIGH(x) (x >> 8)
#define LOW(x) (x & 0xFF)
#define W16(x) (((*(x)) << 8) | (*((x) + 1)))
#define SEXT(x) ((x) & 0x80 ? (x) | 0xFF00 : (x))

/*  It's that diagram again ... Wechat V1 protocol packet
 *  0 bytes  1        2        3        4        5        6        7        8          9    len+8      len+9
 *  +--------+--------+--------+--------+--------+--------+--------+--------+ +--------+--/ /---+ +--------+
 *  |  SOF   |VERSION | FLAGS  | LENGTH |    VENDOR ID    |   COMMAND ID    | | PAYLOAD   ...   | | CHECK  |
 *  +--------+--------+--------+--------+--------+--------+--------+--------+ +--------+--/ /---+ +--------+
 */
#define WECHAT_OFFS_SOF (0)
#define WECHAT_OFFS_VERSION (1)
#define WECHAT_OFFS_FLAGS (2)
#define WECHAT_OFFS_PAYLOAD_LENGTH (3)

#define WECHAT_OFFS_VENDOR_ID (4)
#define WECHAT_OFFS_VENDOR_ID_H WECHAT_OFFS_VENDOR_ID
#define WECHAT_OFFS_VENDOR_ID_L (WECHAT_OFFS_VENDOR_ID + 1)

#define WECHAT_OFFS_COMMAND_ID (6)
#define WECHAT_OFFS_COMMAND_ID_H WECHAT_OFFS_COMMAND_ID
#define WECHAT_OFFS_COMMAND_ID_L (WECHAT_OFFS_COMMAND_ID + 1)

#define WECHAT_OFFS_PAYLOAD (8)


#define WECHAT_SOF (0xFF)
#define WECHAT_VERSION_01 (1)
#define WECHAT_VERSION WECHAT_VERSION_01

#define WECHAT_INVALID_ID (0xFFFF)

#define WECHAT_ACK_NOTIFICATION (WECHAT_EVENT_NOTIFICATION | WECHAT_ACK_MASK)

#define WECHAT_RSSI_HYSTERESIS (2)
#define WECHAT_BATT_HYSTERESIS (10)
#define WECHAT_CHECK_THRESHOLDS_PERIOD (D_SEC(5))

#define WECHAT_STATUS_NONE (0x7FFE)

#define WECHAT_API_VERSION_MINOR_MAX (15)

/*  The protocol defines feature bits 0..63 but they aren't all implemented  */
#define WECHAT_IMP_MAX_FEATURE_BIT (31)
#define WECHAT_IMP_MAX_EVENT_BIT (15)


/*  Internal message ids  */
typedef enum
{
    WECHAT_INTERNAL_INIT = 1,
    WECHAT_INTERNAL_MORE_DATA,
    WECHAT_INTERNAL_SEND_REQ,
    WECHAT_INTERNAL_BATTERY_MONITOR_TIMER,
    WECHAT_INTERNAL_CHECK_BATTERY_THRESHOLD_REQ,
    WECHAT_INTERNAL_CHECK_RSSI_THRESHOLD_REQ,
    WECHAT_INTERNAL_DISCONNECT_REQ,
    WECHAT_INTERNAL_REBOOT_REQ,
    WECHAT_INTERNAL_POWER_OFF_REQ,
    WECHAT_INTERNAL_DFU_REQ,
    WECHAT_INTERNAL_DFU_TIMEOUT
} wechat_internal_message;

typedef struct _wechat_transport wechat_transport;
typedef struct _WECHAT_T WECHAT_T;


typedef struct
{
    wechat_transport *transport;
} WECHAT_INTERNAL_MORE_DATA_T;

    
typedef struct
{
    Task task;
    wechat_transport *transport;
    uint16 length;
    uint8 *data;
} WECHAT_INTERNAL_SEND_REQ_T;


typedef struct
{
    wechat_transport *transport;
} WECHAT_INTERNAL_DISCONNECT_REQ_T;

/*! @brief definition of internal timer message to check battery level for host on specified transport.
 */
typedef struct
{
    wechat_transport *transport;
} WECHAT_INTERNAL_CHECK_BATTERY_THRESHOLD_REQ_T;

/*! @brief definition of internal timer message to check RSSI on a specified transport.
 */
typedef struct
{
    wechat_transport *transport;
} WECHAT_INTERNAL_CHECK_RSSI_THRESHOLD_REQ_T;


#if defined WECHAT_TRANSPORT_RFCOMM || defined WECHAT_TRANSPORT_SPP
/*! @brief Wechat SPP transport state.
 */
typedef struct
{
    uint16 rfcomm_channel;          /*!< RFCOMM channel used by this transport. */
    Sink sink;                  /*!< Stream sink of this transport. */
} wechat_transport_spp_data;

/* helper macro to get SPP specific transport data from generic wechat_transport pointer */
#define SPP_TRANSPORT_DATA(transport)   ((wechat_transport_spp_data*)((transport)->transport_state))

#endif

#ifdef WECHAT_TRANSPORT_MFI
/*! @brief Wechat MFi transport state.
 */
typedef struct
{
    iap_link *link;                     /*!< Pointer to an iap link used by this transport. */
    iap_accessory_config config;        /*!< An iap configuration for use with the iap_link of this transport */
    uint16 data_session_id;             /*!< ID of data session with iOS device */
    uint8 *client_data_ptr;             /*!< pointer to the clients' data that we need to pass back in WECHAT_SEND_PACKET_CFM */

    unsigned transport_configured:1;    /*!< has this transport been configured by the client task yet? */
    unsigned data_session_was_open:1;   /*!< was a data session open (and since closed) but we stayed conn/auth? */    
    unsigned remote_connection:1;       /*!< is this a locally initiated connection (0) or a remote one (1)? */
    unsigned client_discon_req:1;       /*!< did the client task request the disconnect? */
    unsigned transmit_cfm_to_client:1;  /*!< is a WECHAT_SEND_PACKET_CFM message to the client task requried? */
    unsigned unused:11;
} wechat_transport_mfi_data;

/* helper macro to get SPP specific transport data from generic wechat_transport pointer */
#define MFI_TRANSPORT_DATA(transport)   ((wechat_transport_mfi_data*)((transport)->transport_state))

#endif

/*! @brief Generic Wechat transport data structure.
 */
struct _wechat_transport
{
    wechat_transport_type type;       /*!< the transport type of this instance. */
    unsigned connected:1;           /*!< is this transport connected? */
    unsigned flags:1;               /*!< does this link require Wechat checksum? */
    unsigned enabled:1;             /*!< is this session enabled? */
    unsigned unused:13;             /*!< explicitly track unused bits in this word */

    int16 battery_lo_threshold[2];
    int16 battery_hi_threshold[2];
    
    int rssi_lo_threshold[2];
    int rssi_hi_threshold[2];
    
    unsigned threshold_count_lo_rssi:2;
    unsigned threshold_count_hi_rssi:2;
    
    unsigned threshold_count_lo_battery:2;
    unsigned threshold_count_hi_battery:2;
    
    unsigned sent_notification_lo_battery_0:1;
    unsigned sent_notification_lo_battery_1:1;
    unsigned sent_notification_hi_battery_0:1;
    unsigned sent_notification_hi_battery_1:1;
    
    unsigned sent_notification_lo_rssi_0:1;
    unsigned sent_notification_lo_rssi_1:1;
    unsigned sent_notification_hi_rssi_0:1;
    unsigned sent_notification_hi_rssi_1:1;
    
    union
    {
#if defined WECHAT_TRANSPORT_RFCOMM || defined WECHAT_TRANSPORT_SPP
        wechat_transport_spp_data spp;
#endif
        
#ifdef WECHAT_TRANSPORT_MFI
        wechat_transport_mfi_data mfi;
#endif
        
    } state;
  
};


/*  WARNING: if adding values to this enum make sure that the pfs_state bitfield
 *  below is wide enough
 */
typedef enum
{
    PFS_NONE,
    PFS_PARTITION,
    PFS_FILE,
    PFS_DFU
} pfs_state;


/*  WARNING: if adding values to this enum make sure that the dfu_state bitfield
 *  below is wide enough
 */
typedef enum
{
    DFU_IDLE,
    DFU_WAITING,
    DFU_READY,
    DFU_DOWNLOAD,
    DFU_UPDATE
} dfu_state;


/*! @brief Wechat library main task and state structure. */
struct _WECHAT_T
{
    TaskData task_data;
    Task app_task;
    uint32 command_locus_bits;
    uint16 event_locus_bits;
    wechat_transport *outstanding_request;
    int16 battery_reference;
    int16 battery_voltage;
    int16 battery_trend;

    uint32 spp_sdp_handle;      /* not per-transport, there can be only one  */
    uint16 spp_listen_channel;  /* not per-transport, there can be only one  */

    /* 1st bitfield set */
    unsigned api_minor:4;
    unsigned no_vdd_sense:1;
    unsigned rebooting:1;
    unsigned custom_sdp:1;
    unsigned iap_lib_init:1;
    unsigned have_pfs:1;
    pfs_state pfs_state:2;
    unsigned pfs_open_stream:4;
    unsigned :1;
    
    /* 2nd bitfield set */
    dfu_state dfu_state:3;
    unsigned :13;

    union
    {
        Source source;
        Sink sink;
    } pfs;
    
    uint32 pfs_sequence;
    uint32 pfs_raw_size;

    uint16 transport_count;
    wechat_transport transport[1];    /*!< storage for the wechat transports */
};

extern WECHAT_T* wechat; 

/* Common Wechat packet handler, called by a transport once it has got one from whatever source */
void process_packet(wechat_transport *transport, uint8 *packet);

#endif /* ifdef _WECHAT_PRIVATE_H_ */
