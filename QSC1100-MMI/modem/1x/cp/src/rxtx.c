/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                     R X T X     T A S K

GENERAL DESCRIPTION
  The rxtx task is responsible for layer2 protocol processing.

EXTERNALIZED FUNCTIONS
  rxtx_task - This is the RXTX task, started up by rex_def_task.

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 1990-2005 by QUALCOMM, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$PVCSPath: L:/src/asw/MSM5100/CP_REL_A/vcs/rxtx.c_v   1.6   06 Sep 2002 09:25:16   varavamu  $
$Header: //source/qcom/qct/modem/1x/cp/rel/1h08/src/rxtx.c#1 $ $DateTime: 2009/05/29 03:54:56 $ $Author: prabhuk $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/09/08   ag      Corrected the problem of not freeing the rxtx cmd buffer 
		   when a message fails parsing.
03/29/08   bb      Fixing Klocwork errors
10/18/07   pg      Corrected the problem of sending 2 reject orders when an
                   invalid TC msg is received.
09/24/07   sns     corrected the problem of adding 2 entries to the rxtx free queue
                   when invalid message is received while being on traffic channel.
09/29/06   fh      Added a new parameter to caix_int_to_ext to indicate whether
                   the function call is for calculating access channel message 
                   size or for parsing before transmitting the message.
09/14/06   fh      Moved AES field setting from caix.c back into mccsa.c and rxtx.c
09/13/06   fh      Changed send_ra_msg and send_nra_msg to set up integrity/MACI
                   related fields correctly when AKA/MACI is defined.
05/17/06   fh      Merged in new development of AKA/AES features
05/12/06   fh      Changes under AES feature: the parser function caix_int_to_ext
                   carries an extra argument to indicate whether to just
                   get the buffer length for access channel messages, or parsing
                   and AES encrypting the whole message.
05/08/06   ht      Allow L2 ACK when message parsing failed.
05/01/06   an      Fixed Lint Errors
04/16/06   fc      Added the update of command status for RXTX_END_GPS_F.
03/06/06   fh      Added AES support
01/20/06   fc      Merged the following :
           fh      Added support for AKA and MI.
07/11/05   fh      Cleaned up header file including from MUX:
                     changed txc.h to txcmc.h
                     changed rxc.h to rxcmc.h
07/05/05   sb      Added MEID support.
12/09/04   pg      Took out #ifdef FEATURE_GPS_MODE around rxtx_flush_rx_q()
                   so that it can be used without the feature defined.
11/17/04   pg      Made some global variables static.
                   Set rxtx_ignore_rx_msg to FALSE when receive RXTX_TC_F in
                   TC state.
10/29/04   pg      Added support for bypass 2 good frames at TCI state.
07/08/04   sb      Featurized confirmation_type under Release B.
06/18/04   yll     Added support for TC message notificatioN.
06/16/04   va      Added rxtx_ignore_rx_msg flag to support new changes for
                   GPS/CFS txn
10/17/03   sb      Fixed update of confirmation_type.
10/16/03   sb      Fixed mainlining of IS2000.
10/16/03   sb      Fixed IS2000 mainlining.
10/07/03   ph      check queue presence before returning item to free queues.
08/06/03   sb      Added confirmation_type to the payload of TXC_MSG_F.
06/10/03   bkm     Lint cleanup.
05/13/03   sb      Added message in the case of a duplicate.
11/07/02   az      Mainlined FEATURE_IS95B
09/06/02   va      Cleaned up lint errors.
06/07/02   jqi     Updated with multimode task operation interface.
10/02/01   yll     Merged the following from common archive:
  08/23/01   lcc     Changed 2 ERR_FATALs when caix_int_to_ext returns error to
                     ERR since the error is not fatal.
04/12/01   fc      Renamed mcc_report_event_timer to mclog_report_event_timer
                   and mcc_report_event_counter to mclog_report_event_counter.
03/06/01   fc      Added support of events report: Timer expired and Counter
                   threshold.
02/26/01   mai     Changed FEATURE_GPSONE to FEATURE_GPS_MODE to move code needed
                   only for GPS part of position location to a new #define.
10/23/00   yll     Added check if any messages or ACKs need to be transmitted
                   when processing RXTX_START_GPS_F command.
07/06/00   yll     Added GPS support:
                   - Added RXTX_START_GPS_F
                   - Added flushing rxtx_rx_q.
04/10/00   va      Added support all HDMs in rxtx_check_immediate_ack
03/06/00   ry      Added IS2000 featurization
11/15/99   va      IS2000 Changes:
                   Support for new RXTX_HO_F comand.
02/13/99   doj     Added support for General Handoff Direction Message (GHDM)
                   and FEATURE_SPECIAL_MDR in rxtx_check_immediate_ack.
10/22/98   pms     Backed out nearing of variables.
10/02/98   pms     Neared variables to save ROM space.
06/26/97   lh      Fixed buffer leak on rxtx_buf_q.
08/23/96   jca     Frame number now correct in rx messages not requiring ack.
07/18/96   jca     Fixed bug where Layer 2 acks were txed too late.
07/17/96   rdh     Checked in Gwain's RXTX buffer size increase from 3 to 5.
05/22/96   jca     Fixed bug where retries were being txed before T1m expired.
02/29/96   jca     Fixed CAI_LAYER2_RTC2 and CAI_LAYER2_RTC3 updates for
                   cases where N1m is set to a non-spec compliant value.
01/31/96   jca     Fixed Hard Handoff Layer 2 Reset bug.
04/25/94   gb      Parameterised all IS95 timeouts in cai_tmo.
01/06/94   jah     Changed watchdog iface to use dog_report().
12/22/93   jca     Modified for IS-95 Phase 1 upgrade.
10/16/92   jai     Added clearing of signals when entering CDMA state.
09/17/92   jai     Added code for unit test environment and fixed bugs found
                   during unit test.
09/10/91   jai     Create file.

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "qw.h"
#include "bit.h"
#include "queue.h"
#include "cai.h"
#include "caii.h"
#include "caix.h"
#include "rex.h"
#include "task.h"
#include "err.h"
#include "txcmc.h"
#include "rxcmc.h"
#include "mccrxtx.h"
#include "rxtx.h"
#include "dog.h"
#include "mc.h"
#include "parm.h"
#include "msg.h"
#include "mccdma.h"
#if( TG == T_PC )
#error code not present
#endif
#include "mclog.h"

#if defined(FEATURE_IS2000_REL_C_AKA) || defined(FEATURE_IS2000_REL_A_AES)
#error code not present
#endif /* FEATURE_IS2000_REL_C_AKA || FEATURE_IS2000_REL_A_AES */

#ifdef FEATURE_IS2000_REL_A_AES
#error code not present
#endif /* FEATURE_IS2000_REL_A_AES */

/*lint -e818 */
/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/


#if( TG == T_PC )
#error code not present
#else
#define RXTX_NRA_TMO (cai_tmo.t3m/20)
  /* number of frames in which messages received which do not require
     acknowledgements with the same msg_seq will be considered the same
     message */
#endif

#if( TG == T_PC )
#error code not present
#else
#define RXTX_RA_TMO (cai_tmo.t1m/20)
  /* number of frames between retransmissions of messages requiring
     acknowledgements */
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* States of RXTX state machine */
typedef enum
{
  RXTX_START_STATE,  /* startup state */
  RXTX_TC_STATE      /* Traffic Channel message processing state */
}rxtx_state_type;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Queue for commands from MC                                              */
q_type rxtx_mc_q;

/* Free queue for commands from MC                                         */
q_type rxtx_mc_free_q;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Queue for messages from RX */
q_type rxtx_rx_q;

/* Free queue for buffers to be placed on rxtx_rx_q */
q_type rxtx_rx_free_q;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Timers */
/* Watchdog Report timer */
rex_timer_type rxtx_rpt_timer;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*          TRAFFIC CHANNEL PROCCESSING VARIABLES                          */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* DATA STRUCTURES FOR MESSAGES SENT REQUIRING ACKNOWLEDGEMENT */

/* Type to hold information for messages which require acknowledgement */
typedef struct
{
  boolean active;         /* indicator of whether item is active */
  rxtx_cmd_type *cmd_ptr; /* command to send message */
  word len;               /* length of message in bits */
  word retry_cnt;         /* number of times to repeat sending message */
  int num_frames;         /* number of frames to wait between sending msg */
  boolean tx_in_progress; /* message is either on the rxtx_tx_wait_q or
                             currently being transmitted by TX */
  boolean ack_waiting;    /* indicator of whether an acknowledgement has
                             been received for the message */
  cai_tx_msg_type msg;    /* message to transmit in external format */
}rxtx_ack_type;

#define                      RXTX_MAX_ACKS 4
rxtx_ack_type rxtx_ack_msgs[ RXTX_MAX_ACKS ];
  /* Array of buffers to hold information regarding messages which require
     acknowledgement */

/* Queue for commands to send messages requiring acks when no msg_seq is
   available */
q_type rxtx_ack_wait_q;

/* Next msg_seq to be used for a message requiring an ack */
byte rxtx_msg_seq_ack;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* DATA STRUCTURES FOR MESSAGES SENT NOT REQUIRING ACKNOWLEDGEMENT */

/* Type to hold information for messages which do not
   require acknowledgement */
typedef struct
{
  boolean active;        /* indicator of whether element is active */
  word len;              /* length of message in bits */
  cai_tx_msg_type msg;   /* message to transmit in external format */
  rxtx_cmd_type *cmd_ptr; /* command to send message */
}rxtx_noack_type;

#define                          RXTX_MAX_NOACKS 4
rxtx_noack_type rxtx_noack_msgs[ RXTX_MAX_NOACKS ];

/* Queue for commands to send messages not requiring acks when no buffer
   is available */
q_type rxtx_noack_wait_q;

/* Next msg_seq to be use for a message NOT requiring an ack */
byte rxtx_msg_seq_noack;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Buffers for messages generated by the layer 2 task */
#define                   RXTX_NUM_BUFS 5
rxtx_cmd_type  rxtx_bufs[ RXTX_NUM_BUFS ];

q_type rxtx_buf_q;           /* queue for rxtx_bufs */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Structure for keeping track of msg_seqs to put in ack_seq field */
struct
{
  byte ack_seq;  /* ack_seq to put into message */
  int cnt_dwn;   /* frames until automatic ack is sent */
} rxtx_auto_ack[ RXTX_MAX_ACKS + 1];

byte rxtx_next_ack;
  /* next item to use in rxtx_auto_ack array */
byte rxtx_last_ack;
  /* oldest ack_seq in rxtx_auto_ack array */

/* Macro to find next available index into rxtx_auto_ack */
#define NEXT_ACK( index )\
  ( (index + 1 ) % (RXTX_MAX_ACKS + 1) )
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Structure for messages waiting to be sent to TX                         */

typedef struct
{
  q_link_type link;    /* link type */
  boolean ack_req;     /* indicator of whether ack is required or not */
  byte index;          /* index into rxtx_ack_msgs if ack_req is TRUE,
                          index into rxtx_noack_msgs if ack_req is FALSE */
} rxtx_tx_wait_type;

q_type rxtx_tx_wait_q; /* Queue for items to be sent to TX */
q_type rxtx_tx_wait_free_q;  /* Queue for items to be placed on the
                                rxtx_tx_wait_q */

/* buffers to be placed on the rxtx_tx_wait_free_q */
rxtx_tx_wait_type rxtx_tx_wait_bufs[ RXTX_MAX_ACKS + RXTX_MAX_NOACKS ];

txc_cmd_type rxtx_tx_cmd;   /* command to be sent to TX */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* RECEIVED MESSAGE VARIABLES */

#define                    RXTX_NUM_SEQ  8
word rxtx_noack_tmo_cnts [ RXTX_NUM_SEQ ];
  /* number of frames that messages which were received which did NOT
     require an acknowledgement will be considered duplicates */

boolean rxtx_msg_seq_rcvd[ RXTX_NUM_SEQ ];
  /* indicator of whether message requiring an acknowledgement should be
     considered a duplicate */

/* Macro to determine next msg_seq to use */
#define RXTX_NEXT_SEQ( seq ) \
  ( ( seq + 1 ) & ( RXTX_NUM_SEQ - 1 ) )

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
static rxtx_state_type rxtx_state;
  /* state of state machine */
static boolean rxtx_tx_on;
  /* indicator of whether the transmitter is on */
static rxtx_tx_wait_type *rxtx_tx_ptr;
  /* pointer to message currently being transmitted by TX */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Buffers to be placed on the rxtx_mc_free_q */
#define                           RXTX_MAX_CMDS 5
rxtx_cmd_type rxtx_cmd_free_bufs[ RXTX_MAX_CMDS ];

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Buffers to be placed on the rxtx_rx_free_q */
#define                              RXTX_MAX_RX 5
rxtx_rx_msg_type  rxtx_rx_free_bufs[ RXTX_MAX_RX ];
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Indicator of whether a layer 2 reset has occured while a message was
   being transmitted */
static boolean rxtx_reset;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Current ENCRYPT_MODE                                                    */
static byte rxtx_encrypt_mode;

/* Flag to remember that we need to ignore RX messages,may be becos we are tuning
   for GPS for eg */
static boolean rxtx_ignore_rx_msg;

#ifdef FEATURE_IS2000_REL_D
#error code not present
#endif /* FEATURE_IS2000_REL_D */

#ifdef FEATURE_RXTX_TC_MSG_NOTIFY
/* Functions to notify the received or sent messages */
static void ( *rxtx_rx_msg_notifier )( void );
static void ( *rxtx_tx_msg_notifier )( uint16 retry_count );
#endif

void rxtx_tc_clear_queues(void);

#ifdef FEATURE_IS2000_REL_C_AKA
#error code not present
#endif /* FEATURE_IS2000_REL_C_AKA */

#if (TG == T_PC)
#error code not present
#endif


#ifdef FEATURE_RXTX_TC_MSG_NOTIFY
/*===========================================================================

FUNCTION RXTXSRCH_REG_RX_MSG_NOTIFIER

DESCRIPTION
  This registers a function to notify the received message.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxtxsrch_reg_rx_msg_notifier
(
  void ( *rx_msg_notifier )( void )
)
{
  rxtx_rx_msg_notifier = rx_msg_notifier;
}


/*===========================================================================

FUNCTION RXTXSRCH_REG_TX_MSG_NOTIFIER

DESCRIPTION
  This registers a function to notify the sent message.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxtxsrch_reg_tx_msg_notifier
(
  void ( *tx_msg_notifier )( uint16 retry_count )
)
{
  rxtx_tx_msg_notifier = tx_msg_notifier;
}
#endif /* FEATURE_RXTX_TC_MSG_NOTIFY */


/* <EJECT> */
/*===========================================================================

FUNCTION RXTX_WAIT

DESCRIPTION
  This function continues to reset the Watchdog signal and timer while
  waiting for any signal in a specified signal mask to be set. If this
  procedure detects that any signal in the specified signal mask is set,
  it will return the entire mask of signals set in the task'stcb.

DEPENDENCIES
  None.

RETURN VALUE
  Mask of signals returned by the rex_wait call.

SIDE EFFECTS
  None.

===========================================================================*/

word rxtx_wait
(
  word wait_sigs
    /* signals to wait on */
)
{
  word sigs;
    /* signals returned by rex_wait call */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -- - - - -*/

  /* Kick watchdog while waiting for requested signals */
  while ( ( (sigs = rex_get_sigs( rex_self()) ) & wait_sigs) == 0 ) /*lint !e734 loss of precision */
  {
    /*  Check in with Watchdog task. */
    dog_report(DOG_RXTX_RPT);
    (void)rex_set_timer ( &rxtx_rpt_timer, DOG_RXTX_RPT_TIME );

    (void)rex_wait( wait_sigs | RXTX_RPT_TIMER_SIG );
  }

  /* kick watchdog one more time on the way out */
  dog_report(DOG_RXTX_RPT);
  (void)rex_set_timer ( &rxtx_rpt_timer, DOG_RXTX_RPT_TIME );

  return( sigs );

} /* rxtx_wait */

/* <EJECT> */
/*===========================================================================

FUNCTION RXTX_TC_INIT

DESCRIPTION
  This function initializes variables for the Traffic Channel state of
  the RXTX state machine.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Traffic Channel state is read to run after this procedure is completed.

===========================================================================*/

void rxtx_tc_init( void )
{
  word i;  /* index */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

  /*initialize sequence numbers */
  rxtx_msg_seq_ack = 0;    /* next msg_seq for msg requiring an ack */
  rxtx_msg_seq_noack = 0;  /* next msg_seq for msg NOT requring an ack */

  /* Mark rxtx_auto_ack buffers as inactive */
  for(i=0; i< RXTX_MAX_ACKS + 1; i++ )  {
    rxtx_auto_ack[i].cnt_dwn = -1;
  }

  rxtx_auto_ack[0].ack_seq = 7;  /* initialize for ack_seq */
  rxtx_last_ack = 0;             /* use item 0 for now */
  rxtx_next_ack = 1;             /* next place to put ack_seq = item 1 */

  /* initialize all of the noack buffers to inactive */
  for(i=0; i< RXTX_MAX_NOACKS; i++ )
  {
    rxtx_noack_msgs[i].active = FALSE;
  }

  /* initialize all of the ack buffers to inactive */
  for(i=0; i< RXTX_MAX_ACKS; i++ )
  {
    rxtx_ack_msgs[i].active = FALSE;
  }

  /* initialize all of the counts for NRA duplicate detection to 0 */
  for(i=0; i< RXTX_NUM_SEQ; i++ )
  {
    rxtx_noack_tmo_cnts[i] = 0;
  }

  /* initialize all sequence numbers requiring acks to not be duplicates */
  for(i=0; i< RXTX_NUM_SEQ; i++ )
  {
    rxtx_msg_seq_rcvd[i] = FALSE;
  }

  /* initialize transmitter on indicator to on */
  rxtx_tx_on = TRUE;

  /* initialize current message pointer to indicate that no message is
     currently being transmitted by TX */
  rxtx_tx_ptr = NULL;

  /* initialize layer 2 reset in progress to FALSE */
  rxtx_reset = FALSE;

  /* Don't ignore any rcvd messages */
  rxtx_ignore_rx_msg = FALSE;

}/* rxtx_tc_init */

/* <EJECT> */
/*===========================================================================

FUNCTION RXTX_TC_RESET

DESCRIPTION
  This function performs an acknowledgement procedures reset.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void rxtx_tc_reset( void )
{
  byte i;  /* index */
  rxtx_tx_wait_type *wait_ptr;  /* pointer to items on rxtx_tx_wait_q */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

  /* Reinitialize sequence numbers */
  rxtx_msg_seq_ack = 0;    /* next msg_seq for msg requiring an ack */
  rxtx_msg_seq_noack = 0;  /* next msg_seq for msg NOT requring an ack */

  /* Mark rxtx_auto_ack buffers as inactive */
  for(i=0; i< RXTX_MAX_ACKS + 1; i++ )
  {
    rxtx_auto_ack[i].cnt_dwn = -1;
  }

  rxtx_auto_ack[0].ack_seq = 7;  /* initialize for ack_seq */
  rxtx_last_ack = 0;             /* use item 0 for now */
  rxtx_next_ack = 1;             /* next place to put ack_seq = item 1 */

  /* clear all items off the rxtx_wait_tx q - messages that require an ack
     need new MSG_SEQ and ACK_SEQ fields - messages that don't require an
     ack will not be sent to the new base station */

  while( (wait_ptr = (rxtx_tx_wait_type*) q_get(&rxtx_tx_wait_q)) != NULL )
  {
    if(!wait_ptr->ack_req )
    {
      rxtx_noack_msgs[ wait_ptr->index].active = FALSE;
      if( rxtx_noack_msgs[ wait_ptr->index].cmd_ptr != NULL )
      {
        /* signal task back that message not requiring ack has been
           transmitted */
        rxtx_noack_msgs[wait_ptr->index].cmd_ptr->tc_msg.hdr.status =
           RXTX_DONE_S;
        /* Put message back on queue and set signal if requested */
        rxtx_noack_msgs[wait_ptr->index].cmd_ptr->hdr.status =
            RXTX_DONE_S;
        cmd_done( &rxtx_noack_msgs[wait_ptr->index].cmd_ptr->hdr.cmd_hdr );
        rxtx_noack_msgs[wait_ptr->index].cmd_ptr = NULL;
      }
    }
    /* put item back on free queue */
    q_put( &rxtx_tx_wait_free_q, &wait_ptr->link );
  }/* while */

  /* Set up rxtx_ack_msgs structure so that messages have the right MSG_SEQ
     and ACK_SEQ fields and the retry counts are 0 */
  for (i=0; i < RXTX_MAX_ACKS; i++)
  {
	//check valid value before access for rxtx_ack_msgs[] 
    if ((rxtx_ack_msgs[i].active) && (rxtx_msg_seq_ack < RXTX_MAX_ACKS))
    {
      rxtx_ack_msgs[rxtx_msg_seq_ack] = rxtx_ack_msgs[i];

      /* Set up new MSG_SEQ and ACK_SEQ */
      b_packb( 7,
               rxtx_ack_msgs[rxtx_msg_seq_ack].msg.tc.body,
               FPOS( cai_gen_tc_type, hdr.ack_seq ),  /*lint !e734 loss of precision */
               FSIZ( cai_gen_tc_type, hdr.ack_seq ) ); /*lint !e413 likely use of null pointer */
      b_packb( rxtx_msg_seq_ack,
               rxtx_ack_msgs[rxtx_msg_seq_ack].msg.tc.body,
               FPOS( cai_gen_tc_type, hdr.msg_seq ), /*lint !e734 loss of precision */
               FSIZ( cai_gen_tc_type, hdr.msg_seq ) );/*lint !e413 likely use of null pointer */

      /* Reset retransmission counter that msg will be transmitted 3 times */
      rxtx_ack_msgs[rxtx_msg_seq_ack].retry_cnt = cai_tmo.n1m;
      rxtx_ack_msgs[rxtx_msg_seq_ack].num_frames = -1;

      if (rxtx_ack_msgs[i].tx_in_progress)
      {
        /* Record pending layer 2 reset for this item */
        rxtx_reset = TRUE;

        /* Change index to match new MSG_SEQ value */
        rxtx_tx_ptr->index = rxtx_msg_seq_ack;
      }
      else
      {
        /* Set up wait type */
        if ((wait_ptr =
                (rxtx_tx_wait_type*) q_get( &rxtx_tx_wait_free_q )) == NULL)
        {
          ERR( "Empty rxtx_tx_wait_free_q", 0, 0, 0 );
        }
        else
        {
          wait_ptr->ack_req = TRUE;
          wait_ptr->index = rxtx_msg_seq_ack;

          if (rxtx_tx_on && (rxtx_tx_ptr == NULL ) 
#ifdef FEATURE_IS2000_REL_D
#error code not present
#endif /* FEATURE_IS2000_REL_D */
             )
          {
            /* transmitter on and no message in progress - send message */
            rxtx_tx_ptr = wait_ptr;
            rxtx_tx_cmd.msg.len = rxtx_ack_msgs[wait_ptr->index].len;
#ifdef FEATURE_IS2000_REL_B 
            /* RXTX is waiting for an ACK, so it does not need to know
               when the message was sent, only when it was built (so it
               can schedule the next message). */
            rxtx_tx_cmd.msg.confirmation_type = CONFIRM_MSG_BUILT;
#endif /* FEATURE_IS2000_REL_B */
            rxtx_tx_cmd.msg.msg_ptr =
              rxtx_ack_msgs[wait_ptr->index].msg.tc.body;
            txc_cmd( &rxtx_tx_cmd );
            rxtx_ack_msgs[wait_ptr->index].tx_in_progress = TRUE;
          }
          else
          {
            /* put message on wait_q */
            q_put(&rxtx_tx_wait_q, &wait_ptr->link );
            rxtx_ack_msgs[ wait_ptr->index ].tx_in_progress = FALSE;
          }
        }
      }

      rxtx_msg_seq_ack = RXTX_NEXT_SEQ( rxtx_msg_seq_ack );

    } /* end if (rxtx_ack_msgs[i].active) */
  } /* end for (i=0; i < RXTX_MAX_ACKS; i++) */

  for (i=rxtx_msg_seq_ack; i < RXTX_MAX_ACKS; i++)
  {
    rxtx_ack_msgs[i].active = FALSE;
  }

  /* initialize all of the counts for NRA duplicate detection to 0 */
  for(i=0; i< RXTX_NUM_SEQ; i++ )
  {
    rxtx_noack_tmo_cnts[i] = 0;
  }

  /* initialize all sequence numbers requiring acks to not be duplicates */
  for(i=0; i< RXTX_NUM_SEQ; i++ )
  {
    rxtx_msg_seq_rcvd[i] = FALSE;
  }

}/* rxtx_tc_reset */

/* <EJECT> */
/*===========================================================================

FUNCTION RXTX_START

DESCRIPTION
  This task performs layer 2 processing during the start state.

DEPENDENCIES
  None.

RETURN VALUE
  The next state to be processed.

SIDE EFFECTS
  None.

===========================================================================*/

rxtx_state_type rxtx_start( void )
{
  rxtx_state_type new_state = RXTX_START_STATE;
    /* next state to be processed */
  rxtx_cmd_type *cmd_ptr;
    /* pointer to command from MC */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  while(new_state == RXTX_START_STATE )
  {
    (void)rex_clr_sigs(&rxtx_tcb, RXTX_MC_Q_SIG );
    if( (cmd_ptr= (rxtx_cmd_type*) q_get(&rxtx_mc_q)) != NULL )
    {
      switch(cmd_ptr->hdr.command )
      {

        case RXTX_START_F:
        {
          /* start to start transition ok */
          cmd_ptr->hdr.status = RXTX_DONE_S;
          break;
        }

        case RXTX_TC_F:
        {
          /* Switch to Traffic Channel state */
          ( void ) rex_clr_sigs( &rxtx_tcb,  RXTX_RX_Q_SIG | RXTX_FRAME_SIG |
                                    RXTX_TX_CMD_SIG );
          /* clear signals that might have been set in previous call */

          /* Initialize for Traffic Channel processing */
          rxtx_tc_init();
          new_state = RXTX_TC_STATE;
          cmd_ptr->hdr.status = RXTX_DONE_S;
        #ifdef FEATURE_IS2000_REL_D
#error code not present
        #endif /* FEATURE_IS2000_REL_D */
          break;
        }

        case RXTX_ENCRYPT_MODE_F:
        {
          /* Set encrypt mode */
          rxtx_encrypt_mode = cmd_ptr->encrypt_mode.encrypt_mode;
          cmd_ptr->hdr.status = RXTX_DONE_S;
          break;
        }

        default:
        {
          ERR_FATAL("Invalid START cmd %d", cmd_ptr->hdr.command,0,0 );
        }
      } /* switch */ /*lint !e788 */

      /* Put message back on queue and set signal if requested */
      cmd_done(&cmd_ptr->hdr.cmd_hdr );
    }
    else
    {
      if( rxtx_wait( RXTX_MC_Q_SIG | TASK_OFFLINE_SIG | TASK_STOP_SIG ) &
                   ( TASK_OFFLINE_SIG | TASK_STOP_SIG ) )
      {
        /* Process task ack signal from task controller. */
        task_ack();

        ( void )rex_clr_sigs( &rxtx_tcb,  TASK_OFFLINE_SIG | TASK_STOP_SIG );
      }

    }
  } /* while */
  return(new_state );
} /* rxtx_start */

/* <EJECT> */
/*===========================================================================

FUNCTION SEND_NRA_MSG

DESCRIPTION
  This function sends a message not requiring an acknowledgement.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Message is sent if a request buffer is available.

===========================================================================*/

void send_nra_msg
(
  rxtx_tc_msg_type *msg_ptr  /* pointer to message to transmit */
)
{
  byte index;
    /* Index into rxtx_noacks_msg table */
  word status;
    /* Status returned from call to caix_int_to_ext() */
/*lint -esym(550,status) */
  rxtx_tx_wait_type *wait_ptr;
    /* Pointer to type to put on rxtx_tx_wait_q */
  word parm_stat;
    /* Status returned by parm_inc call */
  boolean return_cmd = TRUE;
    /* Indicator of whether a the command block should be returned to the
       requested queue - assume it will be returned  */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  msg_ptr->msg.gen_tc.hdr.ack_req = FALSE;
  #ifdef FEATURE_IS2000_REL_A_AES
#error code not present
  #else
  msg_ptr->msg.gen_tc.hdr.encryption = rxtx_encrypt_mode;
  #endif /* FEATURE_IS2000_REL_A_AES */
  msg_ptr->msg.gen_tc.hdr.msg_seq = rxtx_msg_seq_noack;
  rxtx_msg_seq_noack = RXTX_NEXT_SEQ( rxtx_msg_seq_noack );

  /* search for free request item */
  index = rxtx_msg_seq_noack % 4;

  if (!rxtx_noack_msgs[index].active)
  {
    #ifdef FEATURE_IS2000_REL_C
    /* before parsing the message, set up the integrity fields here */
    /* by default, set maci_incl to be FALSE. It only be true when 
     * MACI feature is defined and P_REV in use is 10 or above
     */
    msg_ptr->msg.gen_tc.hdr.msg_int.maci_incl = FALSE;

    #ifdef FEATURE_IS2000_REL_C_MI
#error code not present
    #endif /* FEATURE_IS2000_REL_C_MI */

    #endif /* FEATURE_IS2000_REL_C */

    /* now set up AES fields */
    if (msg_ptr->msg.gen_tc.hdr.encryption == CAI_EXTENDED_ENCRYPT_CALL_CTL_MSG)
    {
      #ifdef FEATURE_IS2000_REL_A
      msg_ptr->msg.gen_tc.hdr.sdu_encrypt_mode = CAI_ENC_MODE_DISABLED;
      #ifdef FEATURE_IS2000_REL_A_AES
#error code not present
      #endif /* FEATURE_IS2000_REL_A_AES */
      #endif /* FEATURE_IS2000_REL_A */
    }

    /* Fill in buffer */
    if ((status = caix_int_to_ext(
           &rxtx_noack_msgs[ index ].len, CAIX_TC, &msg_ptr->msg, /*lint !e641 */
           &rxtx_noack_msgs[ index ].msg,
           CAIX_PARSE_MSG
       )) != CAIX_DONE_S)
    {
      ERR( "Bad TC translation: msg %d,status %d", msg_ptr->msg.gen.msg_type, status, 0 );
    }
    else
    {
      /* Get a wait_type */
      if ((wait_ptr =
              (rxtx_tx_wait_type*) q_get( &rxtx_tx_wait_free_q )) == NULL)
      {
        ERR( "Empty rxtx_tx_wait_free_q", 0, 0, 0 );
      }
      else
      {
        rxtx_noack_msgs[index].active = TRUE;
        if( msg_ptr->hdr.cmd_hdr.task_ptr != NULL )
        {
          /* task wants to be signalled back when message is transmitted */
          rxtx_noack_msgs[index].cmd_ptr = ( rxtx_cmd_type * )msg_ptr;
          return_cmd = FALSE;
        }
        else
        {
          rxtx_noack_msgs[index].cmd_ptr = NULL;
        }
        wait_ptr->ack_req = FALSE;
        wait_ptr->index = index;

        if( (rxtx_tx_on) && (rxtx_tx_ptr == NULL) 
#ifdef FEATURE_IS2000_REL_D
#error code not present
#endif /* FEATURE_IS2000_REL_D */
        )
        {
#ifdef FEATURE_IS2000_REL_B 
          if ( msg_ptr->hdr.cmd_hdr.task_ptr != NULL )
          {
            /* RXTX wants to be signalled when the message has been
               sent (so processing such as shutting down the transmitter
               can safely take place).  This usually takes a frame,
               since the message is sent in the frame after it is built. */
            rxtx_tx_cmd.msg.confirmation_type = CONFIRM_MSG_SENT;
          }
          else
          {
            /* RXTX wants to be signalled when the message has been
               built so it can schedule the next message. */
            rxtx_tx_cmd.msg.confirmation_type = CONFIRM_MSG_BUILT;
          }
#endif /* FEATURE_IS2000_REL_B */

          /* transmitter on and no message currently being transmitted */
          rxtx_tx_cmd.msg.len = rxtx_noack_msgs[ index ].len;
          rxtx_tx_cmd.msg.msg_ptr = rxtx_noack_msgs[index].msg.tc.body;
          rxtx_tx_ptr = wait_ptr;
          txc_cmd(&rxtx_tx_cmd );
        }
        else
        {
          /* put message on wait q */
          q_put(&rxtx_tx_wait_q, &wait_ptr->link );
        }

        /* Increment number of messages sent not requiring ack */
        if ((parm_stat = parm_inc( CAI_LAYER2_RTC5_ID, 1 )) != PARM_DONE_S)
        {
          ERR("Invalid parameter increment: status %d", parm_stat, 0, 0 );
        }
      }
    }

    if (return_cmd)
    {
      msg_ptr->hdr.status = RXTX_DONE_S;
      /* Put message back on queue and set signal if requested */
      cmd_done(&((rxtx_cmd_type *)msg_ptr)->hdr.cmd_hdr );
    }

  }
  else
  {
    /* put request on rxtx_noack_wait_q until buffer frees up */
    q_put( &rxtx_noack_wait_q, &msg_ptr->hdr.cmd_hdr.link );
  }

}/* send_nra_msg */

/* <EJECT> */
/*===========================================================================

FUNCTION SEND_RA_MSG

DESCRIPTION
  This function attempts to send a message requriring an ack.  If no buffer
  can be found to send the message it places the message on the wait_q.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  rxtx_ra_msgs is set up or the command is placed on the wait_q. Command
  is placed back on the free queue if necessary.

===========================================================================*/

void send_ra_msg
(
  rxtx_tc_msg_type *msg_ptr  /* pointer to message to transmit */
)
{
  byte index;
    /* index into rxtx_ack_msgs */
  boolean return_cmd = TRUE;
    /* indicator of whether the input command should be returned to the
        done_q */
  word status;
    /* status returned by caix_ext_to_int procedure */
  rxtx_tx_wait_type *wait_ptr;
    /* pointer to type to be placed on the rxtx_tx_wait_q */
  word parm_stat;
    /* status returned by parm services */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* calculate index into rxtx_ack_msgs table */
  index = rxtx_msg_seq_ack % 4;

  /* check if msg_seq is available */
  if( rxtx_ack_msgs[ index ].active )
  {
    /* next_msg_seq + 4 % 8 still outstanding - can't send msg yet */
    /* Put message on rxtx_ack_wait_q */

    q_put(&rxtx_ack_wait_q, &msg_ptr->hdr.cmd_hdr.link );
    return_cmd = FALSE;
  }
  else
  {
    /* next msg_seq is ok */
    msg_ptr->msg.gen_tc.hdr.ack_req = TRUE;
    #ifdef FEATURE_IS2000_REL_A_AES
#error code not present
    #else
    msg_ptr->msg.gen_tc.hdr.encryption = rxtx_encrypt_mode;
    #endif /* FEATURE_IS2000_REL_A_AES */

    msg_ptr->msg.gen_tc.hdr.msg_seq = rxtx_msg_seq_ack;
    rxtx_msg_seq_ack =  RXTX_NEXT_SEQ( rxtx_msg_seq_ack );

    #ifdef FEATURE_IS2000_REL_C
    /* before parsing the message, set up the integrity fields here */
    /* by default, set maci_incl to be FALSE. It only be true when 
     * MACI feature is defined and P_REV in use is 10 or above
     */
    msg_ptr->msg.gen_tc.hdr.msg_int.maci_incl = FALSE;

    #ifdef FEATURE_IS2000_REL_C_MI
#error code not present
    #endif /* FEATURE_IS2000_REL_C_MI */

    #endif /* FEATURE_IS2000_REL_C */

    /* now set up AES fields */
    if (msg_ptr->msg.gen_tc.hdr.encryption == CAI_EXTENDED_ENCRYPT_CALL_CTL_MSG)
    {
      #ifdef FEATURE_IS2000_REL_A
      msg_ptr->msg.gen_tc.hdr.sdu_encrypt_mode = CAI_ENC_MODE_DISABLED;
      #ifdef FEATURE_IS2000_REL_A_AES
#error code not present
      #endif /* FEATURE_IS2000_REL_A_AES */
      #endif /* FEATURE_IS2000_REL_A */
    }

    /* Translate message */
    if ((status = caix_int_to_ext(
                    &rxtx_ack_msgs[ index ].len, CAIX_TC, /*lint !e641 */
                    &msg_ptr->msg, &rxtx_ack_msgs[ index ].msg,
                    CAIX_PARSE_MSG
                    )) !=
                       CAIX_DONE_S)
    {
      ERR( "Bad TC translation: msg %d,status %d", msg_ptr->msg.gen.msg_type, status, 0 );
    }
    else
    {
      /* set up remainder of buffer */
      rxtx_ack_msgs[ index ].active = TRUE;
      rxtx_ack_msgs[ index ].ack_waiting = TRUE;
      rxtx_ack_msgs[ index ].retry_cnt = cai_tmo.n1m;

      /* -----------------------------------------------------
      ** This field will get reset after TXC signals back that
      ** the message has been transmitted
      ** ----------------------------------------------------- */
      rxtx_ack_msgs[index].num_frames = -1;

      if( msg_ptr->hdr.cmd_hdr.task_ptr != NULL )
      {
        /* hold on to command block until ack comes in */
        rxtx_ack_msgs[ index].cmd_ptr = (rxtx_cmd_type *)msg_ptr;
        return_cmd = FALSE;
      }
      else
      {
        rxtx_ack_msgs[ index].cmd_ptr = NULL;
            /* indicate that no signal needs to be set */
      }

      /* Set up wait type */
      if ((wait_ptr =
              (rxtx_tx_wait_type*) q_get( &rxtx_tx_wait_free_q )) == NULL)
      {
        ERR( "Empty rxtx_tx_wait_free_q", 0, 0, 0 );
      }
      else
      {
        wait_ptr->ack_req = TRUE;
        wait_ptr->index = index;

        if( rxtx_tx_on && (rxtx_tx_ptr == NULL ) 
#ifdef FEATURE_IS2000_REL_D
#error code not present
#endif /* FEATURE_IS2000_REL_D */
        )
        {
          /* transmitter on and no message in progress - send message */
          rxtx_tx_ptr = wait_ptr;
#ifdef FEATURE_IS2000_REL_B 
          /* RXTX is waiting for an ACK, so it does not need to know
             when the message was sent, only when it was built (so it
             can schedule the next message). */
          rxtx_tx_cmd.msg.confirmation_type = CONFIRM_MSG_BUILT;
#endif /* FEATURE_IS2000_REL_B */
          rxtx_tx_cmd.msg.len = rxtx_ack_msgs[index].len;
          rxtx_tx_cmd.msg.msg_ptr = rxtx_ack_msgs[index].msg.tc.body;
          txc_cmd( &rxtx_tx_cmd );
          rxtx_ack_msgs[ index ].tx_in_progress = TRUE;
        }
        else
        {
          /* put message on wait_q */
          q_put(&rxtx_tx_wait_q, &wait_ptr->link );
          rxtx_ack_msgs[ index ].tx_in_progress = FALSE;
        }

        /* Increment parameter that a msg requiring an ack was sent once */
        if ((parm_stat = parm_inc( CAI_LAYER2_RTC1_ID, 1 )) != PARM_DONE_S)
        {
          ERR( "Invalid parameter increment: status %d", parm_stat, 0, 0 );
        }
      }
    }
  }

  if (return_cmd)
  {
    msg_ptr->hdr.status = RXTX_DONE_S;
      /* Put message back on queue and set signal if requested */
    cmd_done(&((rxtx_cmd_type *)msg_ptr)->hdr.cmd_hdr );
  }

} /* send_ra_msg */

/*===========================================================================

FUNCTION RXTX_TC_TX

DESCRIPTION
  This function performs layer 2 transmit processing for the Traffic Channel.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  The input message will be placed on a free queue if requested.

===========================================================================*/

void rxtx_tc_tx
(
  rxtx_tc_msg_type *msg_ptr  /* pointer to message to transmit */
)
{

  #if defined ( FEATURE_IS2000_REL_C_AKA ) && defined ( FEATURE_IS2000_REL_C_MI )
#error code not present
  #endif /* FEATURE_IS2000_REL_C_AKA && FEATURE_IS2000_REL_C_MI */

  /* ack_seq is always last msg_seq received which requested an ack */
  msg_ptr->msg.gen_tc.hdr.ack_seq =
    rxtx_auto_ack[ rxtx_last_ack].ack_seq;

  /* mark item as already responded to */
  rxtx_auto_ack[ rxtx_last_ack ].cnt_dwn = -1;

  /* move to next ack_seq if there is one pending */
  if ( NEXT_ACK(rxtx_last_ack) != rxtx_next_ack)
  {
    rxtx_last_ack = NEXT_ACK( rxtx_last_ack );
  }

  if(msg_ptr->ack_req)
  {
    /* send message requiring acknowledgement */
    send_ra_msg( msg_ptr );
  }
  else
  {
    /* send message not requiring acknowledgement */
    send_nra_msg( msg_ptr );
  }

}/* rxtx_tc_tx */

/*===========================================================================

FUNCTION CHECK_ACK_WAIT_Q

DESCRIPTION
  This function calls rxtx_tc_tx to process as many pending commands
  as possible given the current state of the rxtx_msg_seq_ack and
  rxtx_ack_msgs structure.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void check_ack_wait_q( void )
{
  rxtx_tc_msg_type *wait_ptr;
    /* pointer to command found on rxtx_ack_wait_q */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  while( !rxtx_ack_msgs[ rxtx_msg_seq_ack % 4 ].active )
  {
    /* msg_seq is available - process pending command */
    if( (wait_ptr = (rxtx_tc_msg_type*) q_get(&rxtx_ack_wait_q)) != NULL )
    {
      rxtx_tc_tx(wait_ptr);
    }
    else
    {
      break; /* no more entries in wait_q */
    }
  } /* while */

}/* check_ack_wait_q */

/* <EJECT> */
/*===========================================================================

FUNCTION BUILD_MS_ACK

DESCRIPTION
  This function builds a Mobile Station Acknowledgement message.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void build_ms_ack
(
  caii_tx_msg_type *msg_ptr
    /* pointer to where to build mobile station ack message */
)
{
  msg_ptr->tc_ord.gen.msg_type = CAI_TC_REV_ORD_MSG;
  msg_ptr->tc_ord.gen.order = CAI_MS_ACK_ORD;

} /* build_ms_ack */

/*===========================================================================

FUNCTION RXTX_CHECK_IMMEDIATE_ACK

DESCRIPTION
 Queue an immediate MS Ack if this message needs it.  This layering violation
 is mostly for Handoff Direction Messages (see requirement in
 IS-95 6.6.6.2.5.1).  This all happens before the message is handed to MC for
 layer 3 processing.

 Note that this does NOT guarantee that the specified msg_ptr is acknowledged.
 If there's an earlier Forward Traffic Channel message waiting for
 acknowledgement in rxtx_auto_ack[], then it will be acknowledged first -
 some other message will have to acknowledge the message in msg_ptr.

 Note also that MC's HARD handoff processing disables the phone's transmitter,
 and any Ack generated here might not be transmitted before the transmitter is
 turned off.  This still meets the spec, however, which says "...shall
 acknowledge the message... unless there is insufficient time to transmit...".

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void rxtx_check_immediate_ack
(
  mccrxtx_cmd_type *msg_ptr
)
{
  rxtx_cmd_type *ack_ptr;
    /* pointer to buffer to place mobile station ack ack */

  if (msg_ptr->msg.msg.gen_tc.hdr.ack_req)
  {
    if ((msg_ptr->msg.msg.gen_tc.msg_type == CAI_HO_DIR_MSG)
        || (msg_ptr->msg.msg.gen_tc.msg_type == CAI_EXT_HO_DIR_MSG)
        || (msg_ptr->msg.msg.gen_tc.msg_type == CAI_GEN_HO_DIR_MSG)
        || (msg_ptr->msg.msg.gen_tc.msg_type == CAI_UNIV_HO_DIR_MSG)
#ifdef FEATURE_MEID_SUPPORT
        || (msg_ptr->msg.msg.gen_tc.msg_type == CAI_MEID_UNIV_HO_DIR_MSG)
#endif
       )
    {
      /* Send a mobile station ack message */
      if ((ack_ptr = (rxtx_cmd_type*) q_get( &rxtx_buf_q )) == NULL)
      {
        ERR( "No buffers on rxtx_buf_q", 0, 0, 0 );
      }
      else
      {
        /* build mobile station ack message */
        build_ms_ack( &ack_ptr->tc_msg.msg );

        ack_ptr->tc_msg.ack_req = FALSE;

        rxtx_tc_tx( &ack_ptr->tc_msg );
      }

      /* One other thing that could be done here is to assign an EXPLICIT */
      /* action time to a _hard_ handoff message that arrived with an     */
      /* implicit action time.  The explicit assignment should have an    */
      /* action time roughly 80ms away so that the layer2 ack is sent on  */
      /* the old channel(s) before we turn off our transmitter.  This     */
      /* might not be necessary, though, since lab testing has shown that */
      /* the phone is able to transmit the Ack before the transmitter is  */
      /* disabled (when there is no Reverse Link voice activity).         */

    } /* end if message needs an _immediate_ ack  */
  }   /* end if ack_req                           */

} /* rxtx_check_immediate_ack */

/*===========================================================================

FUNCTION RXTX_TC_RX

DESCRIPTION
  This function performs layer 2 receive processing for the Traffic Channel.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  A translated message may be put on the queue for the Main Control task.

===========================================================================*/

void rxtx_tc_rx
(
  rxtx_rx_msg_type *rx_ptr   /* pointer to received external message */
)
{
  word status;
    /* Status returned by call to caix_ext_to_int() */
  mccrxtx_cmd_type *msg_ptr;
    /* Pointer to buffer to place translated message in */
  word index;
    /* Index into rxtx_ack_msgs and msg_rcvd tables */
  rxtx_cmd_type *rej_ptr;
    /* Pointer to command buffer to put reject order in */
  boolean send_l2_ack;
    /* send_l2_ack flag defaulted to TRUE */

  #if defined(FEATURE_IS2000_REL_C_MI) || defined(FEATURE_IS2000_REL_A_AES)
#error code not present
  #endif /* FEATURE_IS2000_REL_C_MI || FEATURE_IS2000_REL_A_AES */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

  send_l2_ack = TRUE;  /* default to send L2 Ack */

  if ((rx_ptr->chn == RXC_CDMA_TC) && !rxtx_ignore_rx_msg) /*lint !e641 */
  {
    /* Message from Traffic Channel */
    if ((msg_ptr = (mccrxtx_cmd_type*) q_get( &mcc_rxtx_free_q )) == NULL)
    {
      ERR( "No buffers on mcc_rxtx_free_q", 0, 0, 0 );
    }
    else
    {
      #ifdef FEATURE_RXTX_TC_MSG_NOTIFY
      /* If RX msg notifier is registered, call and notify */
      if ( rxtx_rx_msg_notifier )
      {
        rxtx_rx_msg_notifier();
      }
      #endif

      status = caix_ext_to_int( rx_ptr->length, CAIX_TC, &rx_ptr->msg, &msg_ptr->msg.msg ); /*lint !e734 !e641 */

      #if defined(FEATURE_IS2000_REL_C_MI) || defined(FEATURE_IS2000_REL_A_AES)
#error code not present
      #endif /* FEATURE_IS2000_REL_C_MI || FEATURE_IS2000_REL_A_AES */

      if (status != CAIX_DONE_S) /*lint !e734 */
      {
        /* Translation didn't work - generate MS Reject Order and send it */

        MSG_HIGH( "Bad TC msg %d translation status %d", rx_ptr->msg.tc.body[0], status, 0 );

        #if defined(FEATURE_IS2000_REL_C_MI) || defined(FEATURE_IS2000_REL_A_AES)
#error code not present
        #endif /* FEATURE_IS2000_REL_C_MI || FEATURE_IS2000_REL_A_AES */
        {          
          /* Even though parsing failed, check if we should send L2 ACK. 
             The msg_type is one byte and the needed info for L2 ACK is
             another two bytes */
          if (rx_ptr->length >= (CAIX_MIN_LEN_FOR_L2_ACK * 8)) 
          {
            /* extract the layer 2 info */
            xlate_ext_tc_hdr(rx_ptr->msg.tc.body, &msg_ptr->msg.msg.tc_ord.gen.hdr);
          }
          else 
          {
            /* No good msg header - generate MS Reject Order
            and send it */

            if ((rej_ptr = (rxtx_cmd_type*) q_get( &rxtx_buf_q )) == NULL)
            {
              MSG_ERROR( "No free buffers on rxtx_buf_q", 0, 0, 0 );
            }
            else
            {
              /* Build MS Reject Order */
              rej_ptr->tc_msg.msg.tc_ord.rej.fix.msg_type = CAI_TC_REV_ORD_MSG;
              rej_ptr->tc_msg.msg.tc_ord.rej.fix.order = CAI_MS_REJECT_ORD;
              rej_ptr->tc_msg.msg.tc_ord.rej.fix.ordq =
                 ((status == CAIX_INV_MSG_S) ? CAI_REJ_CODE : CAI_REJ_STRUCT);
              rej_ptr->tc_msg.msg.tc_ord.rej.fix.rej_msg_type =
                msg_ptr->msg.msg.gen_tc.msg_type;
      
              rej_ptr->tc_msg.ack_req = FALSE;
      
              rej_ptr->hdr.command = RXTX_TC_MSG_F;

              /* Transmit the message */
              rxtx_tc_tx( (rxtx_tc_msg_type *) rej_ptr );
            }

            /* Not enough info to send L2 ACK */
            send_l2_ack = FALSE;
            MSG_HIGH("Not enough info len=%d status=%d", rx_ptr->msg.tc.length, status,0);

            /* We won't be using the msg_ptr, or the buffer pointed by it for now;
             * Put item back on to free q
             */
            if (msg_ptr->msg.hdr.cmd_hdr.done_q_ptr != NULL)
            {
              q_put( msg_ptr->msg.hdr.cmd_hdr.done_q_ptr,
                     &msg_ptr->msg.hdr.cmd_hdr.link );
            }
          }

        } /* if status is NOT a MACI failure */

      } /* if (caix_ext_to_int(...)) */

      if (send_l2_ack) 
      {
        /* Process Layer 2 */
        /* Use acq_seq to update available msg_seq */

        if( (status == CAIX_DONE_S) &&
            ( ( (rxtx_msg_seq_ack >= 4 ) &&
                ((msg_ptr->msg.msg.gen_tc.hdr.ack_seq >= rxtx_msg_seq_ack - 4) &&
                 (msg_ptr->msg.msg.gen_tc.hdr.ack_seq < rxtx_msg_seq_ack) ) ) ||
              ( (rxtx_msg_seq_ack < 4 ) &&
                ((msg_ptr->msg.msg.gen_tc.hdr.ack_seq < rxtx_msg_seq_ack) ||
                 (msg_ptr->msg.msg.gen_tc.hdr.ack_seq >= rxtx_msg_seq_ack + 4))) ) )
        {
          /* Calculate index into rxtx_ack_msgs table */
          index = msg_ptr->msg.msg.gen_tc.hdr.ack_seq % 4;

          if( rxtx_ack_msgs[index].active )
          {
            /* Message is acknowleging message requiring ack */
            rxtx_ack_msgs[index].ack_waiting = FALSE;

            /* Check to see if task wants to be signaled */
            if( rxtx_ack_msgs[index].cmd_ptr != NULL )
            {
              /* Signal back task */
              rxtx_ack_msgs[index].cmd_ptr->hdr.status = RXTX_DONE_S;
              cmd_done( &rxtx_ack_msgs[index].cmd_ptr->hdr.cmd_hdr );

              /* Mark command as no longer active */
              rxtx_ack_msgs[index].cmd_ptr = NULL;
            }

            if (!rxtx_ack_msgs[ index ].tx_in_progress)
            {
              /* Mark item as inactive */
              rxtx_ack_msgs[ index ].active = FALSE;
            }

            /* Check to see whether we were waiting for this ack to send
               another msg requiring an ack */
            if (index == rxtx_msg_seq_ack % 4)
            {
              /* We may have been waiting to send a new message -
                   check rxtx_wait_ack_q */
              check_ack_wait_q();
            }
          }
        }

        if (msg_ptr->msg.msg.gen_tc.hdr.ack_req)
        {
          /* Set up for automatic mobile station ack */
          rxtx_auto_ack[ rxtx_next_ack].ack_seq =
            msg_ptr->msg.msg.gen_tc.hdr.msg_seq;

          /* Give layer 3 60 ms to return response */
          rxtx_auto_ack[ rxtx_next_ack].cnt_dwn = 3;

          if (rxtx_auto_ack[rxtx_last_ack].cnt_dwn == -1)
          {
            /* Last ack already counted down switch to point to this item */
            rxtx_last_ack = rxtx_next_ack;
          }
          rxtx_next_ack = NEXT_ACK( rxtx_next_ack );

          if (rxtx_msg_seq_rcvd[msg_ptr->msg.msg.gen_tc.hdr.msg_seq])
          {
            /* Message is a duplicate - don't send to layer 3 */
            MSG_MED("Duplicate, Ack Reqd. Msg_seq: %d",
              msg_ptr->msg.msg.gen_tc.hdr.msg_seq, 0, 0);

            if (msg_ptr->msg.hdr.cmd_hdr.done_q_ptr != NULL)
            {
              q_put( msg_ptr->msg.hdr.cmd_hdr.done_q_ptr,
                     &msg_ptr->msg.hdr.cmd_hdr.link );
            }
          }
          else /* Message is not a duplicate */
          {
            /* Next time you see msg_seq + 4 % 8 it will not be a duplicate */
            index = (msg_ptr->msg.msg.gen_tc.hdr.msg_seq + 4) & 0x7;
            rxtx_msg_seq_rcvd[index] = FALSE;

            /* Next time this message is seen it will be a duplicate */
            rxtx_msg_seq_rcvd[msg_ptr->msg.msg.gen_tc.hdr.msg_seq] = TRUE;

            /* Queue an immediate MS Ack if this message needs it.  This    */
            /* layering violation is mostly for Handoff Direction Messages  */
            /* (see requirement in IS-95 6.6.6.2.5.1).                      */
            rxtx_check_immediate_ack(msg_ptr);

            if (status != CAIX_DONE_S) 
            {
              /* Parsing failed. Don't Send the message to MC.  Send a reject
                 order here instead. */
              if ((rej_ptr = (rxtx_cmd_type*) q_get( &rxtx_buf_q )) == NULL)
              {
                MSG_ERROR( "No free buffers on rxtx_buf_q", 0, 0, 0 );
              }
              else
              {
                /* Build MS Reject Order */
                rej_ptr->tc_msg.msg.tc_ord.rej.fix.msg_type = CAI_TC_REV_ORD_MSG;
                rej_ptr->tc_msg.msg.tc_ord.rej.fix.order = CAI_MS_REJECT_ORD;
                rej_ptr->tc_msg.msg.tc_ord.rej.fix.ordq =
                  ((status == CAIX_INV_MSG_S) ? CAI_REJ_CODE : CAI_REJ_STRUCT);
                rej_ptr->tc_msg.msg.tc_ord.rej.fix.rej_msg_type =
                  msg_ptr->msg.msg.gen_tc.msg_type;

                rej_ptr->tc_msg.ack_req = FALSE;

                rej_ptr->hdr.command = RXTX_TC_MSG_F;

                /* Transmit the message */
                rxtx_tc_tx( (rxtx_tc_msg_type *) rej_ptr );
              }

              /* We won't be using the msg_ptr, or the buffer pointed by it for now;
               * Put item back on to free q
               */
              if (msg_ptr->msg.hdr.cmd_hdr.done_q_ptr != NULL)
              {
                q_put( msg_ptr->msg.hdr.cmd_hdr.done_q_ptr,
                       &msg_ptr->msg.hdr.cmd_hdr.link );
              }

            }
            else 
            {
              /* Send message to MC */
              msg_ptr->msg.hdr.command = MCCRXTX_MSG_F;

              msg_ptr->msg.chn = RXC_CDMA_TC; /*lint !e641 */
              qw_equ( msg_ptr->msg.frm.frame, rx_ptr->msg.tc.frame_num );

              msg_ptr->msg.hdr.cmd_hdr.done_q_ptr = &mcc_rxtx_free_q;
              msg_ptr->msg.hdr.cmd_hdr.task_ptr = NULL;

              /* Send message and set signal for MC */
              mccrxtx_cmd( msg_ptr);
            }

          }

        }
        else
        {
          /* message did not request an ack */
          /* check for duplicate */
          if( rxtx_noack_tmo_cnts[ msg_ptr->msg.msg.gen_tc.hdr.msg_seq] == 0 )
          {

            /* message is not a duplicate */
            if (status != CAIX_DONE_S) 
            {
              /* Parsing failed. Don't Send the message to MC.  Send a reject
                 order here instead. */
              if ((rej_ptr = (rxtx_cmd_type*) q_get( &rxtx_buf_q )) == NULL)
              {
                MSG_ERROR( "No free buffers on rxtx_buf_q", 0, 0, 0 );
              }
              else
              {
                /* Build MS Reject Order */
                rej_ptr->tc_msg.msg.tc_ord.rej.fix.msg_type = CAI_TC_REV_ORD_MSG;
                rej_ptr->tc_msg.msg.tc_ord.rej.fix.order = CAI_MS_REJECT_ORD;
                rej_ptr->tc_msg.msg.tc_ord.rej.fix.ordq =
                  ((status == CAIX_INV_MSG_S) ? CAI_REJ_CODE : CAI_REJ_STRUCT);
                rej_ptr->tc_msg.msg.tc_ord.rej.fix.rej_msg_type =
                  msg_ptr->msg.msg.gen_tc.msg_type;

                rej_ptr->tc_msg.ack_req = FALSE;

                rej_ptr->hdr.command = RXTX_TC_MSG_F;

                /* Transmit the message */
                rxtx_tc_tx( (rxtx_tc_msg_type *) rej_ptr );
              }

              /* We won't be using the msg_ptr, or the buffer pointed by it for now;
               * Put item back on to free q
               */
              if (msg_ptr->msg.hdr.cmd_hdr.done_q_ptr != NULL)
              {
                q_put( msg_ptr->msg.hdr.cmd_hdr.done_q_ptr,
                       &msg_ptr->msg.hdr.cmd_hdr.link );
              }

            }
            else 
            { 
              msg_ptr->msg.hdr.command = MCCRXTX_MSG_F;
              rxtx_noack_tmo_cnts[ msg_ptr->msg.msg.gen_tc.hdr.msg_seq] =
                RXTX_NRA_TMO;
              /* set up frame number to detect duplicate */
              msg_ptr->msg.chn = RXC_CDMA_TC; /*lint !e641 */ /* message from Traffic Channel */
              qw_equ( msg_ptr->msg.frm.frame, rx_ptr->msg.tc.frame_num );

              /* put message on queue for MC */
              mccrxtx_cmd( msg_ptr);
            }
          }
          else
          {
            /* message is a duplicate - put back on free_q */
            if( msg_ptr->msg.hdr.cmd_hdr.done_q_ptr != NULL )
            {
              q_put(msg_ptr->msg.hdr.cmd_hdr.done_q_ptr,
                    &msg_ptr->msg.hdr.cmd_hdr.link );
            }
          
          } /* message is a duplicate - put back on free_q */

        } /* message did not request an ack */
   
      } /* end of if(send_l2_ack) */
    
    } /* q_get( &mcc_rxtx_free_q ) == NULL */

  } /* if ((rx_ptr->chn == RXC_CDMA_TC) && !rxtx_ignore_rx_msg) */


  if (rxtx_ignore_rx_msg)
  {
    MSG_ERROR("Rxed Message ignored by RXTX",0,0,0);
  }
  /* Place buffer from RX back on to free q if requested */
  if( rx_ptr->return_q_ptr != NULL )
  {
    q_put(rx_ptr->return_q_ptr, &rx_ptr->link );
  }

} /* rxtx_tc_rx */

/*===========================================================================

FUNCTION RXTX_TC_CHK

DESCRIPTION
  This function performs layer 2 periodic checking every 20 ms for the
  Traffic Channel.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  A command may be sent to Tx to transmit a message.

===========================================================================*/

void rxtx_tc_chk( void )
{
  byte i;
    /* index through rxtx_ack_mgs buffer */
  mccrxtx_cmd_type *msg_ptr;
   /* Pointer to message to send to MC */
  rxtx_cmd_type *ack_ptr;
    /* pointer to buffer to place mobile station ack ack */
  rxtx_tx_wait_type *wait_ptr;
    /* pointer to type for message retransmission */
  word parm_stat;
    /* status returned by parm_inc */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

  /* check to see if retransmission of messages requiring
     acknowledgements is required */
  for(i=0; i< RXTX_MAX_ACKS; i++ )
  {
    if( rxtx_ack_msgs[i].active )
    {
      if ( rxtx_ack_msgs[i].num_frames > 0 )
      {
        rxtx_ack_msgs[i].num_frames--;
      }

      if ((rxtx_ack_msgs[ i ].num_frames == 0) &&
          (!rxtx_ack_msgs[ i ].tx_in_progress))
      {
        /* Event report: Timer T1m expired */
        mclog_report_event_timer(CAI_TIMER_T1M);

        if (( rxtx_ack_msgs[ i ].retry_cnt > 0 ) && rxtx_tx_on )
        {
          #ifdef FEATURE_RXTX_TC_MSG_NOTIFY
          /* If TX msg notifier is registered, call and notify */
          if ( rxtx_tx_msg_notifier )
          {
            rxtx_tx_msg_notifier( cai_tmo.n1m - rxtx_ack_msgs[ i ].retry_cnt );
          }
          #endif

          /* Time to send the message again */
          if ((wait_ptr =
                  (rxtx_tx_wait_type*) q_get( &rxtx_tx_wait_free_q )) == NULL)
          {
            ERR( "Empty rxtx_tx_wait_free_q", 0, 0, 0 );
          }
          else
          {
            /* -----------------------------------------------------
            ** This field will get reset after TXC signals back that
            ** the message has been transmitted
            ** ----------------------------------------------------- */
            rxtx_ack_msgs[i].num_frames = -1;

            wait_ptr->ack_req = TRUE;
            wait_ptr->index = i;

            /* -----------------------------------------------------------
            ** Update the ACK_SEQ field of the message to be retransmitted
            ** if needed
            ** ----------------------------------------------------------- */
             if ((b_unpackb( rxtx_ack_msgs[i].msg.tc.body,
                             FPOS( cai_gen_tc_type, hdr.ack_seq ), /*lint !e734 */
                             FSIZ( cai_gen_tc_type, hdr.ack_seq ))) !=
                    (rxtx_auto_ack[rxtx_last_ack].ack_seq)) /*lint !e413*/
             {
               /* ------------------------------------------------------
               ** Set the ACK_SEQ field equal to the MSG_SEQ field value
               ** of the last message received which requested an ack
               ** ------------------------------------------------------ */
               b_packb( rxtx_auto_ack[rxtx_last_ack].ack_seq,
                        rxtx_ack_msgs[i].msg.tc.body,
                        FPOS( cai_gen_tc_type, hdr.ack_seq ), /*lint !e734 */
                        FSIZ( cai_gen_tc_type, hdr.ack_seq )); /*lint !e413 */

               /* Mark item as already responded to */
                rxtx_auto_ack[rxtx_last_ack].cnt_dwn = -1;

               /* Move to next ack_seq if there is one pending */
               if (NEXT_ACK( rxtx_last_ack ) != rxtx_next_ack)
               {
                 rxtx_last_ack = NEXT_ACK( rxtx_last_ack );
               }
             }

            if (( rxtx_tx_ptr == NULL ) 
#ifdef FEATURE_IS2000_REL_D
#error code not present
#endif /* FEATURE_IS2000_REL_D */
            )
            {
              /* No message currently being transmitted by TX */
              rxtx_tx_ptr = wait_ptr;
              rxtx_tx_cmd.msg.len = rxtx_ack_msgs[i].len;
#ifdef FEATURE_IS2000_REL_B 
              /* RXTX is waiting for an ACK, so it does not need to know
                 when the message was sent, only when it was built (so it
                 can schedule the next message). */
              rxtx_tx_cmd.msg.confirmation_type = CONFIRM_MSG_BUILT;
#endif /* FEATURE_IS2000_REL_B */
              rxtx_tx_cmd.msg.msg_ptr = rxtx_ack_msgs[i].msg.tc.body;

              txc_cmd(&rxtx_tx_cmd );

              rxtx_ack_msgs[i].tx_in_progress = TRUE;
            }
            else
            {
              /* put on wait_q until TX is ready */
              q_put(&rxtx_tx_wait_q, &wait_ptr->link );
            }
            if (rxtx_ack_msgs[ i ].retry_cnt == (cai_tmo.n1m - 1))
            {
              /* This is the 2nd message transmission */
              if ((parm_stat =
                      parm_inc( CAI_LAYER2_RTC2_ID, 1 )) != PARM_DONE_S)
              {
                ERR( "Invalid parm increment status %d", parm_stat, 0, 0 );
              }
            }
            else if (rxtx_ack_msgs[ i ].retry_cnt == (cai_tmo.n1m - 2))
            {
              /* This is the 3rd message transmission */
              if ((parm_stat =
                      parm_inc( CAI_LAYER2_RTC3_ID, 1 )) != PARM_DONE_S)
              {
                ERR( "Invalid parm increment status %d", parm_stat, 0, 0 );
              }
            }
          }
        }
        else if (rxtx_ack_msgs[i].retry_cnt == 0)
        {
          /* Event report: Counter N1m threshold reached */
          mclog_report_event_counter(CAI_COUNTER_N1M);

          /* Send message to MC indicating that message requiring ack
              has timed out */
          if ((msg_ptr = (mccrxtx_cmd_type*) q_get( &mc_cmd_free_q )) == NULL)
          {
            ERR( "No buffers on mc_cmd_free_q", 0, 0, 0 );
          }
          else
          {
            MSG_HIGH("Ack tmo index %d ", i,0,0);
            msg_ptr->ack_tmo.hdr.command = MCCRXTX_ACK_TMO_F;
            msg_ptr->ack_tmo.hdr.cmd_hdr.done_q_ptr = &mc_cmd_free_q;
            msg_ptr->ack_tmo.hdr.cmd_hdr.task_ptr = NULL;
            mccrxtx_cmd( msg_ptr);
            rxtx_ack_msgs[i].active = FALSE; /* set item to inactive */

            /* Increment number of calls aborted as result of signaling
                timeout */
            if ((parm_stat =
                    parm_inc( CAI_LAYER2_RTC4_ID, 1 )) != PARM_DONE_S)
            {
              ERR( "Invalid parm increment status %d", parm_stat, 0, 0 );
            }
          }

          if( rxtx_ack_msgs[i].cmd_ptr != NULL )
          {
            /*  holding buffer - need to put back on free q */
            if( rxtx_ack_msgs[i].cmd_ptr->tc_msg.hdr.cmd_hdr.done_q_ptr
                != NULL )
            {
              q_put( &rxtx_mc_free_q,
                      &rxtx_ack_msgs[i].cmd_ptr->tc_msg.hdr.cmd_hdr.link );
            }
            rxtx_ack_msgs[i].cmd_ptr = NULL;
            /* mark command as no longer active */
          }
        }     /* end if retry_count has counted down to zero (timeout)    */
      }       /* end if this msg's num_frames has counted down to zero    */
    }         /* end if rxtx_ack_msgs[i] is active                        */
  }           /* end for loop over rxtx_ack_msgs[] to retransmit/timeout  */

  /* Check if its time to send a mobile station ack message               */
  for( i=0; i< RXTX_MAX_ACKS + 1; i++ )
  {
    /* Is the an active counter?                                          */
    if (rxtx_auto_ack[i].cnt_dwn > 0 )
    {
      /* We can count down even if the transmitter is off, but we can't   */
      /* send the MS ack until the transmitter is on.  So don't get down  */
      /* to zero unless the transmitter is on.                            */
      if (( rxtx_auto_ack[i].cnt_dwn > 1 ) || rxtx_tx_on)
      {
        --rxtx_auto_ack[i].cnt_dwn;
      }

      if( rxtx_auto_ack[i].cnt_dwn == 0 )
      {
        /* Time to send a mobile station ack message */

        if ((ack_ptr = (rxtx_cmd_type*) q_get( &rxtx_buf_q )) == NULL)
        {
          ERR( "No buffers on rxtx_buf_q", 0, 0, 0 );
          rxtx_auto_ack[i].cnt_dwn = 1;
        }
        else
        {
          /* build mobile station ack message */
          build_ms_ack( &ack_ptr->tc_msg.msg );

          ack_ptr->tc_msg.ack_req = FALSE;

          rxtx_tc_tx( &ack_ptr->tc_msg );

        }
      }     /* end if we reached the frame where an MS Ack is necessary */
    }       /* end if this is an active counter.                        */
  }         /* end for loop over rxtx_auto_ack[]                        */

  /* Decrement countdowns for messages not requiring acknowledgement */
  for(i=0; i< 8; i++ )
  {
    if( rxtx_noack_tmo_cnts[i] > 0 )
    {
      rxtx_noack_tmo_cnts[i]--;
    }
  } /* for */

} /* rxtx_tc_chk */

/* <EJECT> */
/*===========================================================================

FUNCTION CHECK_TX_WAIT_Q

DESCRIPTION
  This function checks the rxtx_tx_wait_q for pending messages and sends
  a command to TX to transmit a message if it finds a pending message.

DEPENDENCIES
  Rxtx_tx_ptr should be null indicating that TX is NOT in process of sending
  a message.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void check_tx_wait_q( void )
{
  if( rxtx_tx_ptr != NULL )
  {
    ERR_FATAL( "Bad tx_wait_q check", 0, 0, 0 );
  }
  else if( (rxtx_tx_ptr =
               (rxtx_tx_wait_type*) q_get(&rxtx_tx_wait_q)) != NULL )
  {
    if( rxtx_tx_ptr->ack_req )
    {
      /* fill in TX command */
      rxtx_tx_cmd.msg.len = rxtx_ack_msgs[rxtx_tx_ptr->index].len;
      rxtx_tx_cmd.msg.msg_ptr = rxtx_ack_msgs[rxtx_tx_ptr->index].msg.tc.body;
#ifdef FEATURE_IS2000_REL_B 
      /* RXTX is waiting for an ACK, so it does not need to know
         when the message was sent, only when it was built (so it
         can schedule the next message). */      
      rxtx_tx_cmd.msg.confirmation_type = CONFIRM_MSG_BUILT;
#endif /* FEATURE_IS2000_REL_B */
      rxtx_ack_msgs[rxtx_tx_ptr->index].tx_in_progress = TRUE;
    }
    else
    {
      rxtx_tx_cmd.msg.len = rxtx_noack_msgs[rxtx_tx_ptr->index ].len;
      rxtx_tx_cmd.msg.msg_ptr =
        rxtx_noack_msgs[rxtx_tx_ptr->index].msg.tc.body;
#ifdef FEATURE_IS2000_REL_B 
      if ( rxtx_noack_msgs[rxtx_tx_ptr->index].cmd_ptr == NULL )
      {
        /* RXTX wants to be signalled when the message has been
           built so it can schedule the next message. */
        rxtx_tx_cmd.msg.confirmation_type = CONFIRM_MSG_BUILT;
      }
      else
      {
        /* RXTX wants to be signalled when the message has been
           sent (so processing such as shutting down the transmitter
           can safely take place).  This usually takes a frame,
           since the message is sent in the frame after it is built. */
        rxtx_tx_cmd.msg.confirmation_type = CONFIRM_MSG_SENT;
      }
#endif /* FEATURE_IS2000_REL_B */
    }
    txc_cmd(&rxtx_tx_cmd );
  }

} /* check_tx_wait_q */

/* <EJECT> */
/*===========================================================================

FUNCTION RXTX_FLUSH_RX_Q

DESCRIPTION
  This function flushes the rxtx_rx_q.

DEPENDENCIES
  rxtx_rx_q is initialized.

RETURN VALUE
  None.

SIDE EFFECTS
  All the messages in rxtx_rx_q will be thrown away.

===========================================================================*/
void rxtx_flush_rx_q
(
  void
)
{
  rxtx_rx_msg_type *msg_ptr;

  /*-----------------------------------------------------------------------*/

  while ( (msg_ptr= (rxtx_rx_msg_type*) q_get(&rxtx_rx_q)) != NULL )
  {
    MSG_MED ("Throw away message from rxtx_rx_q", 0, 0, 0);

    /* Place buffer from RX back on to free q if requested */
    if( msg_ptr->return_q_ptr != NULL )
    {
      q_put(msg_ptr->return_q_ptr, &msg_ptr->link );
    }
    else
    {
      MSG_ERROR("cannot release q resource.", 0, 0, 0);
    }
  }
} /* rxtx_flush_rx_q */


/* <EJECT> */
#ifdef FEATURE_GPS_MODE
#error code not present
#endif /* FEATURE_GPS_MODE */

/* <EJECT> */
/*===========================================================================

FUNCTION RXTX_TC_CMD

DESCRIPTION
  This function processes command other than those to send messages
  received from the Main Control task during the Traffic Channel state.

DEPENDENCIES
  None.

RETURN VALUE
  Next state to be processed.

SIDE EFFECTS
  Status field of command buffer will be filled in and command will be
  placed on the free queue if requested.

===========================================================================*/

rxtx_state_type rxtx_tc_cmd
(
  rxtx_cmd_type *cmd_ptr  /* command received from MC */
)
{
  rxtx_state_type next_state = RXTX_TC_STATE;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

  switch( cmd_ptr->hdr.command )
  {

    case RXTX_START_F:
    {
      /* Go back to Start */
      next_state = RXTX_START_STATE;
      cmd_ptr->hdr.status = RXTX_DONE_S;
      break;
    }

    #ifdef FEATURE_IS2000_REL_D
#error code not present
    #endif /* FEATURE_IS2000_REL_D */

    case RXTX_START_TX_F:
    {
      /* turn transmitter indicator on */
      rxtx_tx_on = TRUE;
      /* TX is on, don't ignore any messages */
      rxtx_ignore_rx_msg = FALSE;

      /* If TX is not in process of sending a message check to see if there
         is one pending to send */
      if( rxtx_tx_ptr == NULL )
      {
        check_tx_wait_q();
      }

      cmd_ptr->hdr.status = RXTX_DONE_S;
      break;
    }

    case RXTX_STOP_TX_F:
    {
      /* turn transmitter indicator off */
      rxtx_tx_on = FALSE;
      cmd_ptr->hdr.status = RXTX_DONE_S;
      break;
    }

    case RXTX_ENCRYPT_MODE_F:
    {
      /* Set encrypt mode */
      rxtx_encrypt_mode = cmd_ptr->encrypt_mode.encrypt_mode;
      cmd_ptr->hdr.status = RXTX_DONE_S;
      break;
    }

    case RXTX_HO_F:
    {
      /* reset acknowledgement procedures */
      if (cmd_ptr->ho.reset_l2)
      {
        rxtx_tc_reset();
      }
      #ifdef FEATURE_AUTH
      rxtx_encrypt_mode = cmd_ptr->ho.encrypt_mode;
      #endif
      cmd_ptr->hdr.status = RXTX_DONE_S;
      break;
    }

#ifdef FEATURE_GPS_MODE
#error code not present
#endif /* FEATURE_GPS_MODE */

#ifdef FEATURE_IS2000_REL_D
#error code not present
#endif /* FEATURE_IS2000_REL_D */

#ifdef FEATURE_IS2000_REL_C_AKA
#error code not present
#endif /* FEATURE_IS2000_REL_C_AKA */

    default:
    {
      ERR_FATAL ( "Invalid TC cmd %d", cmd_ptr->hdr.command,0,0 );
    }
  } /* switch */ /*lint !e788 */

  cmd_done( &cmd_ptr->hdr.cmd_hdr); /* return message to queue if requested */

  return(next_state);

}/* rxtx_tc_cmd */

/*===========================================================================

FUNCTION RXTX_TX_SIG

DESCRIPTION
  This task performs processing necessary after TX signals back that a
  Traffic Channel message has been transmitted.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void rxtx_tx_sig( void )
{
  rxtx_tc_msg_type *wait_ptr;
    /* Pointer to command found on rxtx_ack_wait_q or rxtx_noack_wait_q */
  boolean ack_req = FALSE;
    /* Indicator of whether message that was transmitted required and ack */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if( rxtx_tx_ptr == NULL )
  {
    ERR_FATAL("rxtx_tx_ptr error",0,0,0 );
  }
  else
  {
    /* clean up from transmitted message */

    if( rxtx_tx_ptr->ack_req )
    {
      if (rxtx_reset)
      {
        /* --------------------------------------------------------
        ** There has been a reset of layer 2 while this message was
        ** being transmitted
        ** -------------------------------------------------------- */
        rxtx_reset = FALSE;

        rxtx_ack_msgs[rxtx_tx_ptr->index].tx_in_progress = FALSE;

        if (rxtx_ack_msgs[ rxtx_tx_ptr->index].ack_waiting)
        {
          /* Queue this message up as the next message to be transmitted */
          q_put( &rxtx_tx_wait_q, &rxtx_tx_ptr->link );
        }
        else
        {
          /* Put this item back on free queue */
          q_put( &rxtx_tx_wait_free_q, &rxtx_tx_ptr->link );
        }

        rxtx_tx_ptr = NULL;
      }
      else
      {
        /* No layer 2 reset occured during this transmission...
           perform normal processing */
        ack_req = TRUE;
        rxtx_ack_msgs[ rxtx_tx_ptr->index ].tx_in_progress = FALSE;
        if(!rxtx_ack_msgs[ rxtx_tx_ptr->index].ack_waiting)
        {
          /* no longer waiting for ack - mark item as inactive */
          rxtx_ack_msgs[ rxtx_tx_ptr->index].active = FALSE;

        }
        else
        {
          /* Ack is still waiting so reset frame counts */
          rxtx_ack_msgs[ rxtx_tx_ptr->index].retry_cnt--;
          rxtx_ack_msgs[ rxtx_tx_ptr->index].num_frames = (int) RXTX_RA_TMO;
        }
        q_put(&rxtx_tx_wait_free_q, &rxtx_tx_ptr->link );
        rxtx_tx_ptr = NULL;
      }
    }
    else
    {
      /* transmitted message did not require ack */
      ack_req = FALSE;
      if( rxtx_noack_msgs[ rxtx_tx_ptr->index].cmd_ptr != NULL )
      {
        /* signal task back that message not requiring ack has been
           transmitted */
        rxtx_noack_msgs[rxtx_tx_ptr->index].cmd_ptr->tc_msg.hdr.status =
           RXTX_DONE_S;
        /* Put message back on queue and set signal if requested */
        rxtx_noack_msgs[rxtx_tx_ptr->index].cmd_ptr->hdr.status =
            RXTX_DONE_S;
        cmd_done( &rxtx_noack_msgs[rxtx_tx_ptr->index].cmd_ptr->hdr.cmd_hdr );
        rxtx_noack_msgs[rxtx_tx_ptr->index].cmd_ptr = NULL;
      }

      rxtx_noack_msgs[ rxtx_tx_ptr->index ].active = FALSE;
      q_put(&rxtx_tx_wait_free_q, &rxtx_tx_ptr->link );
      rxtx_tx_ptr = NULL;
    }
  }

  /* check rxtx_tx_wait_q for pending messages */
  check_tx_wait_q();

  /* Check to see if commands from MC have been queued up */
  if( ack_req )
  {
    check_ack_wait_q();
  }
  else
  {
    if( (wait_ptr = (rxtx_tc_msg_type*) q_get(&rxtx_noack_wait_q)) != NULL )
    {
      rxtx_tc_tx(wait_ptr);
    }
  }
} /* rxtx_tx_sig */

/*===========================================================================

FUNCTION RXTX_TC_CLEAR_QUEUES

DESCRIPTION
  This function clears all the TC transmission queues

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  All the transmission queues are cleared

===========================================================================*/
void rxtx_tc_clear_queues(void)
{
  int i;

  rxtx_tc_msg_type *wait_ptr;
  /* pointer to command found on rxtx_ack_wait_q or rxtx_noack_wait_q */

  rxtx_tx_wait_type *tx_ptr;
  /* pointer to command found on rxtx_tx_wait_q */

  /* Clean up queues and place buffers back on rxtx_mc_free_q */

  /* check to make sure that no buffers have been left in rxtx_ack_msgs */
  for( i=0; i< RXTX_MAX_ACKS; i++ )
  {
    if( rxtx_ack_msgs[i].cmd_ptr != NULL )
    {
      if( rxtx_ack_msgs[i].cmd_ptr->tc_msg.hdr.cmd_hdr.done_q_ptr != NULL )
      {
        q_put( &rxtx_mc_free_q,
               &rxtx_ack_msgs[i].cmd_ptr->tc_msg.hdr.cmd_hdr.link );
          /* put messages on rxtx_mc_free_q instead of done_q because
             done_q will be for queue to post message on after the ack is
             received */
      }
      rxtx_ack_msgs[i].cmd_ptr = NULL;
    }
  }

  /* check to make sure that no buffers have been left in rxtx_noack_msgs */
  for( i=0; i< RXTX_MAX_NOACKS; i++ )
  {
    if( rxtx_noack_msgs[i].cmd_ptr != NULL )
    {
      if( rxtx_noack_msgs[i].cmd_ptr->tc_msg.hdr.cmd_hdr.done_q_ptr != NULL )
      {
        q_put( &rxtx_mc_free_q,
               &rxtx_noack_msgs[i].cmd_ptr->tc_msg.hdr.cmd_hdr.link );
          /* put messages on rxtx_mc_free_q instead of done_q because
             done_q will be for queue to post message on after message is
             transmitted */
      }
      rxtx_noack_msgs[i].cmd_ptr = NULL;
    }
  }

  /* Remove any old items from the rxtx_ack_wait_q */
  while( (wait_ptr = (rxtx_tc_msg_type*) q_get(&rxtx_ack_wait_q)) != NULL )
  {
    if (wait_ptr->hdr.cmd_hdr.done_q_ptr != NULL)
    {
      q_put(wait_ptr->hdr.cmd_hdr.done_q_ptr, &wait_ptr->hdr.cmd_hdr.link );      
    }
  }

  /* Remove any old items from the rxtx_noack_wait_q */
  while( (wait_ptr = (rxtx_tc_msg_type*) q_get(&rxtx_noack_wait_q)) != NULL )
  {
    if (wait_ptr->hdr.cmd_hdr.done_q_ptr != NULL)
    {
      q_put(wait_ptr->hdr.cmd_hdr.done_q_ptr, &wait_ptr->hdr.cmd_hdr.link );      
    }
  }

  /* Remove items from the rxtx_wait_tx_q */
  while( (tx_ptr = (rxtx_tx_wait_type*) q_get(&rxtx_tx_wait_q)) != NULL )
  {
    q_put(&rxtx_tx_wait_free_q, &tx_ptr->link );
  }

  if( rxtx_tx_ptr )
  {
    /* put item pointed to by wait pointer back on free queue */
    q_put( &rxtx_tx_wait_free_q, &rxtx_tx_ptr->link );
    rxtx_tx_ptr = NULL;
  }

} /* rxtx_tc_clear_queues */

#ifdef FEATURE_IS2000_REL_C_AKA
#error code not present
#endif /* FEATURE_IS2000_REL_C_AKA */

/*===========================================================================

FUNCTION RXTX_TC

DESCRIPTION
  This task performs layer 2 Traffic Channel processing.

DEPENDENCIES
  None.

RETURN VALUE
  The next state to be processed.

SIDE EFFECTS
  None.

===========================================================================*/

rxtx_state_type rxtx_tc( void )
{
  rxtx_state_type new_state = RXTX_TC_STATE;
    /* next state to be processed */
  rxtx_cmd_type *cmd_ptr;
    /* pointer to command from MC */
  rxtx_rx_msg_type *msg_ptr;
    /* pointer to message received from RX */
  word sigs;
    /* signals received from rx_wait call */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  while(new_state == RXTX_TC_STATE )
  {
    (void)rex_clr_sigs(&rxtx_tcb, RXTX_RX_Q_SIG );
    if( (msg_ptr= (rxtx_rx_msg_type*) q_get(&rxtx_rx_q)) != NULL )
    {
      /* process message received from rx */
      rxtx_tc_rx(msg_ptr);
    }
    else
    {
      (void)rex_clr_sigs(&rxtx_tcb, RXTX_MC_Q_SIG );
      if( (cmd_ptr= (rxtx_cmd_type*) q_get(&rxtx_mc_q)) != NULL )
      {
        if( cmd_ptr->hdr.command == RXTX_TC_MSG_F )
        {
          /* Process request to send a message */
          rxtx_tc_tx(&cmd_ptr->tc_msg);

          #ifdef FEATURE_RXTX_TC_MSG_NOTIFY
          /* If TX msg notifier is registered, call and notify */
          if ( rxtx_tx_msg_notifier )
          {
            rxtx_tx_msg_notifier( 0 );
          }
          #endif
        }
        else
        {
          /* Process change of state command from MC */
          new_state = rxtx_tc_cmd(cmd_ptr);
        }

      }
      else
      {
        /* Wait for command from MC, message from RX, periodic frame
           signal or signal from TX  */
#if(TG == T_PC)
#error code not present
#endif
        if( (sigs = rxtx_wait( RXTX_MC_Q_SIG | RXTX_RX_Q_SIG |
                    RXTX_FRAME_SIG | RXTX_TX_CMD_SIG ))
                   & RXTX_TX_CMD_SIG )
        {
          (void)rex_clr_sigs(&rxtx_tcb, RXTX_TX_CMD_SIG );

          /* TX is signaling back that a message has been transmitted */
          rxtx_tx_sig();

        }
        if( sigs & RXTX_FRAME_SIG )
        {
          /* time to do 20 ms periodic checking */
          (void)rex_clr_sigs(&rxtx_tcb, RXTX_FRAME_SIG);
          rxtx_tc_chk();
        }
      }
    }
  } /* while */

  /* clear all the TC transmission queues */
  rxtx_tc_clear_queues();

  return(new_state );
} /* rxtx_tc */

/* <EJECT> */
/*===========================================================================

FUNCTION RXTX_INIT

DESCRIPTION
  This task performs initialization for queues and variables.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void rxtx_init( void )
{
  word i;   /* index through array elements */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* initialize timers */
  rex_def_timer (&rxtx_rpt_timer, &rxtx_tcb, RXTX_RPT_TIMER_SIG );

  /* Initialize queues */
  (void)q_init(&rxtx_rx_q);
  (void)q_init(&rxtx_rx_free_q );
  (void)q_init(&rxtx_mc_q);
  (void)q_init(&rxtx_mc_free_q );
  (void)q_init(&rxtx_ack_wait_q);
  (void)q_init(&rxtx_noack_wait_q);
  (void)q_init(&rxtx_buf_q);
  (void)q_init(&rxtx_tx_wait_q);
  (void)q_init(&rxtx_tx_wait_free_q);

  /* Fill rxtx_buf_q with rxtx_bufs */
  for( i=0; i< RXTX_NUM_BUFS ; i++ )
  {
    rxtx_bufs[i].hdr.cmd_hdr.done_q_ptr = &rxtx_buf_q;
    q_put( &rxtx_buf_q, q_link( &rxtx_bufs[i],
           &rxtx_bufs[i].hdr.cmd_hdr.link ) );
  }

  /* fill rxtx_tx_wait_free_q with rxtx_tx_wait_bufs */
  for( i=0; i< (RXTX_MAX_ACKS + RXTX_MAX_NOACKS); i++ )
  {
    q_put(&rxtx_tx_wait_free_q, q_link( &rxtx_tx_wait_bufs[i],
          &rxtx_tx_wait_bufs[i].link ));
  }

  /* initialize tx command buffer */
  rxtx_tx_cmd.hdr.command = TXC_MSG_F;
  rxtx_tx_cmd.hdr.cmd_hdr.task_ptr = &rxtx_tcb;
  rxtx_tx_cmd.hdr.cmd_hdr.sigs = RXTX_TX_CMD_SIG;
  rxtx_tx_cmd.hdr.cmd_hdr.done_q_ptr = NULL;

  /* Initialize free command buffers */
  for(i=0; i< RXTX_MAX_CMDS; i++ )
  {
    rxtx_cmd_free_bufs[i].hdr.cmd_hdr.done_q_ptr = &rxtx_mc_free_q;
    q_put( &rxtx_mc_free_q, q_link( &rxtx_cmd_free_bufs[i],
           &rxtx_cmd_free_bufs[i].hdr.cmd_hdr.link ) );
  }

  /* Fill rxtx_rx_free_q with rxtx_rx_free_bufs */
  for(i=0; i< RXTX_MAX_RX; i++ )
  {
    rxtx_rx_free_bufs[i].return_q_ptr = &rxtx_rx_free_q;
    q_put( &rxtx_rx_free_q, q_link( &rxtx_rx_free_bufs[i],
           &rxtx_rx_free_bufs[i].link ) );
  }
  /* Mark to not ignore any rcvd messages */
  rxtx_ignore_rx_msg = FALSE;

  #ifdef FEATURE_RXTX_TC_MSG_NOTIFY
  /* Initialize the RX and TX msg notifier function. */
  rxtx_rx_msg_notifier = NULL;
  rxtx_tx_msg_notifier = NULL;
  #endif

} /* rxtx_init */

/* <EJECT> */
/*===========================================================================

FUNCTION RXTX_TASK

DESCRIPTION
  This task performs layer 2 processing.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void rxtx_task
(
  dword dummy
    /* Required for REX, ignore */
    /*lint -esym(715,dummy)
    ** Have lint not complain about the ignored parameter 'dummy' which is
    ** specified to make this routine match the template for rex_def_task().
    */
)
{

  rxtx_init();  /* initialize queues and variables */

  /* Process task startup procedure from task controller. */
  task_start( RXTX_RPT_TIMER_SIG, DOG_RXTX_RPT, &rxtx_rpt_timer);

  rxtx_state = RXTX_START_STATE;

  for( ;; )
  {
    switch( rxtx_state )
    {

      case RXTX_START_STATE:
      {

        /* Initialize encrypt mode to disabled */
        rxtx_encrypt_mode = CAI_ENCRYPTION_DISABLED;

        rxtx_state = rxtx_start();
        break;
      }

      case RXTX_TC_STATE:
      {
        rxtx_state = rxtx_tc();
        break;
      }

      default:
      {
        ERR_FATAL( "Invalid state %d",rxtx_state, 0,0  );
      }
    }/* switch */
  }/* while */

}/* rxtx_task */

/* <EJECT> */
/*===========================================================================

FUNCTION RXTX_CMD

DESCRIPTION
  This function should be called to send a command to the Layer 2 task.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Item will be enqueued for Layer 2 task and signal will be set to indicate
  that the item has been enqueued.

===========================================================================*/

void  rxtx_cmd
(
  rxtx_cmd_type *cmd_ptr
    /* pointer to command to be enqueued */
)
{
  cmd_ptr->hdr.status = RXTX_BUSY_S;

  /* initialize link field */
  (void)q_link(cmd_ptr, &cmd_ptr->hdr.cmd_hdr.link );

  /* put item on queue for rx_task and set signal for task */
  q_put( &rxtx_mc_q, &cmd_ptr->hdr.cmd_hdr.link );
  (void)rex_set_sigs( &rxtx_tcb, RXTX_MC_Q_SIG );

} /* rxtx_cmd */

/*lint +e818 */
