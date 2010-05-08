/*****************************************************************************
**
**  Name:           bta_jv_int.h
**
**  Description:    This is the private interface file for the BTA Java I/F
**
**  Copyright (c) 2006, Broadcom, All Rights Reserved.
**  Widcomm Bluetooth Core. Proprietary and confidential.
**
*****************************************************************************/
#ifndef BTA_JV_INT_H
#define BTA_JV_INT_H

#include "bta_sys.h"
#include "bta_api.h"
#include "bta_jv_api.h"
#include "rfcdefs.h"

/*****************************************************************************
**  Constants
*****************************************************************************/

enum
{
    /* these events are handled by the state machine */
    BTA_JV_API_ENABLE_EVT = BTA_SYS_EVT_START(BTA_ID_JV),
    BTA_JV_API_DISABLE_EVT,
    BTA_JV_API_SET_DISCOVERABILITY_EVT,
    BTA_JV_API_GET_LOCAL_DEVICE_ADDR_EVT,
    BTA_JV_API_GET_LOCAL_DEVICE_NAME_EVT,
    BTA_JV_API_GET_REMOTE_DEVICE_NAME_EVT,
    BTA_JV_API_SET_SERVICE_CLASS_EVT,
    BTA_JV_API_GET_SCN_EVT,
    BTA_JV_API_FREE_SCN_EVT,
    BTA_JV_API_START_DISCOVERY_EVT,
    BTA_JV_API_GET_SERVICES_LENGTH_EVT,
    BTA_JV_API_SERVICE_SELECT_EVT,
    BTA_JV_API_CREATE_RECORD_EVT,
    BTA_JV_API_UPDATE_RECORD_EVT,
    BTA_JV_API_ADD_ATTRIBUTE_EVT,
    BTA_JV_API_DELETE_ATTRIBUTE_EVT,
    BTA_JV_API_DELETE_RECORD_EVT,
    BTA_JV_API_L2CAP_CONNECT_EVT,
    BTA_JV_API_L2CAP_CLOSE_EVT,
    BTA_JV_API_L2CAP_START_SERVER_EVT,
    BTA_JV_API_L2CAP_STOP_SERVER_EVT,
    BTA_JV_API_L2CAP_READ_EVT,
    BTA_JV_API_L2CAP_WRITE_EVT,
    BTA_JV_API_RFCOMM_CONNECT_EVT,
    BTA_JV_API_RFCOMM_CLOSE_EVT,
    BTA_JV_API_RFCOMM_START_SERVER_EVT,
    BTA_JV_API_RFCOMM_STOP_SERVER_EVT,
    BTA_JV_API_RFCOMM_READ_EVT,
    BTA_JV_API_RFCOMM_WRITE_EVT,
    BTA_JV_MAX_INT_EVT
};

/* data type for BTA_JV_API_ENABLE_EVT */
typedef struct
{
    BT_HDR          hdr;      
    tBTA_JV_DM_CBACK   *p_cback;
} tBTA_JV_API_ENABLE;

/* data type for BTA_JV_API_SET_DISCOVERABILITY_EVT */
typedef struct
{
    BT_HDR              hdr;      
    tBTA_JV_DISC    disc_mode;
} tBTA_JV_API_SET_DISCOVERABILITY;


/* data type for BTA_JV_API_SET_SERVICE_CLASS_EVT */
typedef struct
{
    BT_HDR      hdr;  
    UINT32      service;
} tBTA_JV_API_SET_SERVICE_CLASS;

/* data type for BTA_JV_API_GET_REMOTE_DEVICE_NAME_EVT */
typedef struct
{
    BT_HDR      hdr;  
    BD_ADDR     bd_addr;
} tBTA_JV_API_GET_REMOTE_NAME;

/* data type for BTA_JV_API_START_DISCOVERY_EVT */
typedef struct
{
    BT_HDR      hdr;  
    BD_ADDR bd_addr;
    UINT16 num_uuid;
    tSDP_UUID uuid_list[BTA_JV_MAX_UUIDS];
    UINT16 num_attr;
    UINT16 attr_list[BTA_JV_MAX_ATTRS];    
} tBTA_JV_API_START_DISCOVERY;

/* data type for BTA_JV_API_GET_SERVICES_LENGTH_EVT */
typedef struct
{
    BT_HDR      hdr;  
    UINT16      *p_services_len;
} tBTA_JV_API_GET_SERVICES_LENGTH;

/* data type for BTA_JV_API_GET_SERVICE_RESULT_EVT */
typedef struct
{
    BT_HDR      hdr;  
    UINT8        **TLVs;
} tBTA_JV_API_GET_SERVICE_RESULT;

/* data type for BTA_JV_API_SERVICE_SELECT_EVT */
typedef struct
{
    BT_HDR      hdr;  
    UINT16        uuid;
} tBTA_JV_API_SERVICE_SELECT;

enum
{
    BTA_JV_ST_NONE = 0,
    BTA_JV_ST_CL_OPENING,
    BTA_JV_ST_CL_OPEN,
    BTA_JV_ST_SR_LISTEN,
    BTA_JV_ST_SR_OPEN,
    BTA_JV_ST_SR_CLOSING
} ;
typedef UINT8  tBTA_JV_STATE;

/* JV L2CAP control block */
typedef struct
{
    tBTA_JV_L2CAP_CBACK *p_cback;   /* the callback function */
    UINT16              psm;        /* the psm used for this server connection */
    tBTA_JV_STATE       state;      /* the state of this control block */
    tBTA_SERVICE_ID     sec_id;     /* service id */
    UINT16              handle;     /* the handle reported to java app (same as gap handle) */
    BOOLEAN             cong;       /* TRUE, if congested */
} tBTA_JV_L2C_CB;

/* JV RFCOMM control block */
typedef struct
{
    tBTA_JV_RFCOMM_CBACK *p_cback;  /* the callback function */
    UINT16              rfc_handle; /* port handle */
    tBTA_JV_STATE       state;      /* the state of this control block */
    tBTA_SERVICE_ID     sec_id;     /* service id */
    UINT8               handle;     /* index: the handle reported to java app */
    UINT8               scn;        /* the scn of the server */
    BOOLEAN             cong;       /* TRUE, if congested */
} tBTA_JV_RFC_CB;

/* data type for BTA_JV_API_L2CAP_CONNECT_EVT */
typedef struct
{
    BT_HDR          hdr;  
    tBTA_SEC        sec_mask;
    tBTA_JV_ROLE    role;
    UINT16          remote_psm;
    UINT16          rx_mtu;
    BD_ADDR         peer_bd_addr;
    tBTA_JV_L2CAP_CBACK *p_cback;
} tBTA_JV_API_L2CAP_CONNECT;

/* data type for BTA_JV_API_L2CAP_SERVER_EVT */
typedef struct
{
    BT_HDR              hdr;  
    tBTA_SEC            sec_mask;
    tBTA_JV_ROLE        role;
    UINT16              local_psm;
    UINT16              rx_mtu;
    tBTA_JV_L2CAP_CBACK *p_cback;
} tBTA_JV_API_L2CAP_SERVER;

/* data type for BTA_JV_API_L2CAP_CLOSE_EVT */
typedef struct
{
    BT_HDR          hdr;  
    UINT16          handle;
    tBTA_JV_L2C_CB  *p_cb;
} tBTA_JV_API_L2CAP_CLOSE;

/* data type for BTA_JV_API_L2CAP_READ_EVT */
typedef struct
{
    BT_HDR              hdr;  
    UINT16              handle;
    tBTA_JV_L2CAP_CBACK *p_cback;
    UINT8*              p_data;
    UINT16              len;    
} tBTA_JV_API_L2CAP_READ;

/* data type for BTA_JV_API_L2CAP_WRITE_EVT */
typedef struct
{
    BT_HDR              hdr;  
    UINT16              handle;
    tBTA_JV_L2C_CB      *p_cb;
    UINT8               *p_data;
    UINT16              len;    
} tBTA_JV_API_L2CAP_WRITE;

/* data type for BTA_JV_API_RFCOMM_CONNECT_EVT */
typedef struct
{
    BT_HDR          hdr;  
    tBTA_SEC        sec_mask;
    tBTA_JV_ROLE    role;
    UINT8           remote_scn;
    BD_ADDR         peer_bd_addr;
    tBTA_JV_RFCOMM_CBACK *p_cback;
} tBTA_JV_API_RFCOMM_CONNECT;

/* data type for BTA_JV_API_RFCOMM_SERVER_EVT */
typedef struct
{
    BT_HDR          hdr;  
    tBTA_SEC        sec_mask;
    tBTA_JV_ROLE    role;
    UINT8           local_scn;
    tBTA_JV_RFCOMM_CBACK *p_cback;
} tBTA_JV_API_RFCOMM_SERVER;

/* data type for BTA_JV_API_RFCOMM_READ_EVT */
typedef struct
{
    BT_HDR          hdr;  
    UINT16          handle;
    UINT8           *p_data;
    UINT16          len;
    tBTA_JV_RFC_CB  *p_cb;
} tBTA_JV_API_RFCOMM_READ;

/* data type for BTA_JV_API_RFCOMM_WRITE_EVT */
typedef struct
{
    BT_HDR          hdr;  
    UINT16          handle;
    UINT8           *p_data;
    UINT16          len;
    tBTA_JV_RFC_CB  *p_cb;
} tBTA_JV_API_RFCOMM_WRITE;

/* data type for BTA_JV_API_RFCOMM_CLOSE_EVT */
typedef struct
{
    BT_HDR          hdr;  
    UINT16          handle;
    tBTA_JV_RFC_CB  *p_cb;
} tBTA_JV_API_RFCOMM_CLOSE;

/* data type for BTA_JV_API_CREATE_RECORD_EVT */
typedef struct
{
    BT_HDR      hdr;  
    UINT16      *p_ids;
    UINT8       **p_values;
    INT32       *p_value_sizes;
    INT32       array_len;
} tBTA_JV_API_CREATE_RECORD;

/* data type for BTA_JV_API_UPDATE_RECORD_EVT */
typedef struct
{
    BT_HDR      hdr;  
    UINT32      handle;
    UINT16      *p_ids;
    UINT8       **p_values;
    INT32       *p_value_sizes;
    INT32       array_len;
} tBTA_JV_API_UPDATE_RECORD;

/* data type for BTA_JV_API_ADD_ATTRIBUTE_EVT */
typedef struct
{
    BT_HDR      hdr;  
    UINT32      handle;
    UINT16      attr_id;
    UINT8       *p_value;
    INT32       value_size;
} tBTA_JV_API_ADD_ATTRIBUTE;

/* data type for BTA_JV_API_FREE_SCN_EVT */
typedef struct
{
    BT_HDR      hdr;
    UINT8       scn;
} tBTA_JV_API_FREE_SCN;
/* union of all data types */
typedef union
{
    /* GKI event buffer header */
    BT_HDR                          hdr;      
    tBTA_JV_API_ENABLE              enable;
    tBTA_JV_API_SET_DISCOVERABILITY set_discoverability;
    tBTA_JV_API_GET_REMOTE_NAME     get_rmt_name;
    tBTA_JV_API_SET_SERVICE_CLASS   set_service;
    tBTA_JV_API_START_DISCOVERY     start_discovery;
    tBTA_JV_API_GET_SERVICES_LENGTH get_services_length;
    tBTA_JV_API_GET_SERVICE_RESULT  get_service_result;
    tBTA_JV_API_SERVICE_SELECT      service_select;
    tBTA_JV_API_FREE_SCN            free_scn;
    tBTA_JV_API_CREATE_RECORD       create_record;
    tBTA_JV_API_UPDATE_RECORD       update_record;
    tBTA_JV_API_ADD_ATTRIBUTE       add_attr;
    tBTA_JV_API_L2CAP_CONNECT       l2cap_connect;
    tBTA_JV_API_L2CAP_READ          l2cap_read;
    tBTA_JV_API_L2CAP_WRITE         l2cap_write;
    tBTA_JV_API_L2CAP_CLOSE         l2cap_close;
    tBTA_JV_API_L2CAP_SERVER        l2cap_server;
    tBTA_JV_API_RFCOMM_CONNECT      rfcomm_connect;
    tBTA_JV_API_RFCOMM_READ         rfcomm_read;
    tBTA_JV_API_RFCOMM_WRITE        rfcomm_write;
    tBTA_JV_API_RFCOMM_CLOSE        rfcomm_close;
    tBTA_JV_API_RFCOMM_SERVER       rfcomm_server;
} tBTA_JV_MSG;

#if SDP_FOR_JV_INCLUDED == TRUE
#define BTA_JV_L2C_FOR_SDP_HDL     GAP_MAX_CONNECTIONS
#endif

/* JV control block */
typedef struct
{
    UINT32                  psm;            /* last PSM used */
#if SDP_FOR_JV_INCLUDED == TRUE
    UINT32                  sdp_for_jv;     /* The SDP client connection handle */
    UINT32                  sdp_data_size;  /* the data len */
#endif
    /* the SDP handle reported to JV user is the (index + 1) to sdp_handle[].
     * if sdp_handle[i]==0, it's not used.
     * otherwise sdp_handle[i] is the stack SDP handle. */
    UINT32                  sdp_handle[BTA_JV_MAX_SDP_REC]; /* SDP records created */
    UINT8                   *p_sel_raw_data;/* the raw data of last service select */
    INT32                   sel_len;        /* the SDP record size of last service select */
    tBTA_JV_DM_CBACK        *p_dm_cback;
    tBTA_JV_L2C_CB          l2c_cb[BTA_JV_MAX_L2C_CONN];    /* index is GAP handle (index) */
    tBTA_JV_RFC_CB          rfc_cb[BTA_JV_MAX_RFC_CONN];
    UINT8                   sec_id[BTA_JV_NUM_SERVICE_ID];  /* service ID */
    BOOLEAN                 scn[BTA_JV_MAX_SCN];            /* SCN allocated by java */
} tBTA_JV_CB;

/* JV control block */
#if BTA_DYNAMIC_MEMORY == FALSE
extern tBTA_JV_CB bta_jv_cb;
#else
extern tBTA_JV_CB *bta_jv_cb_ptr;
#define bta_jv_cb (*bta_jv_cb_ptr)
#endif

/* config struct */
extern tBTA_JV_CFG *p_bta_jv_cfg;

/* this is defined in stack/sdp. used by bta jv */
extern UINT8 *sdpu_get_len_from_type (UINT8 *p, UINT8 type, UINT32 *p_len);

extern BOOLEAN bta_jv_sm_execute(BT_HDR *p_msg);

extern UINT32 bta_jv_get_sdp_handle(UINT32 sdp_id);
extern void bta_jv_enable (tBTA_JV_MSG *p_data);
extern void bta_jv_disable (tBTA_JV_MSG *p_data);
extern void bta_jv_set_discoverability (tBTA_JV_MSG *p_data);
extern void bta_jv_get_local_device_addr (tBTA_JV_MSG *p_data);
extern void bta_jv_get_local_device_name (tBTA_JV_MSG *p_data);
extern void bta_jv_get_remote_device_name (tBTA_JV_MSG *p_data);
extern void bta_jv_set_service_class (tBTA_JV_MSG *p_data);
extern void bta_jv_get_scn (tBTA_JV_MSG *p_data);
extern void bta_jv_free_scn (tBTA_JV_MSG *p_data);
extern void bta_jv_start_discovery (tBTA_JV_MSG *p_data);
extern void bta_jv_get_services_length (tBTA_JV_MSG *p_data);
extern void bta_jv_service_select (tBTA_JV_MSG *p_data);
extern void bta_jv_create_record (tBTA_JV_MSG *p_data);
extern void bta_jv_update_record (tBTA_JV_MSG *p_data);
extern void bta_jv_add_attribute (tBTA_JV_MSG *p_data);
extern void bta_jv_delete_attribute (tBTA_JV_MSG *p_data);
extern void bta_jv_delete_record (tBTA_JV_MSG *p_data);
extern void bta_jv_l2cap_connect (tBTA_JV_MSG *p_data);
extern void bta_jv_l2cap_close (tBTA_JV_MSG *p_data);
extern void bta_jv_l2cap_start_server (tBTA_JV_MSG *p_data);
extern void bta_jv_l2cap_stop_server (tBTA_JV_MSG *p_data);
extern void bta_jv_l2cap_read (tBTA_JV_MSG *p_data);
extern void bta_jv_l2cap_write (tBTA_JV_MSG *p_data);
extern void bta_jv_rfcomm_connect (tBTA_JV_MSG *p_data);
extern void bta_jv_rfcomm_close (tBTA_JV_MSG *p_data);
extern void bta_jv_rfcomm_start_server (tBTA_JV_MSG *p_data);
extern void bta_jv_rfcomm_stop_server (tBTA_JV_MSG *p_data);
extern void bta_jv_rfcomm_read (tBTA_JV_MSG *p_data);
extern void bta_jv_rfcomm_write (tBTA_JV_MSG *p_data);

#endif /* BTA_JV_INT_H */
