/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


                     G S T K    M O R E    T I M E


GENERAL DESCRIPTION
  This source file contains functions to process more time
  proactive command


FUNCTIONS
  gstk_more_time_req
    This function parses the more time Proactive Command from the card
    and sends it to the client.

  gstk_more_time_cnf
    This function packs the more time terminal response and send
    the command to UIM server.

  gstk_more_time_cnf_cleanup
    This function cleans up any allocated pointer on the stack.

INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2003-2008 QUALCOMM, Incorporated and its licensors.  All Rights
Reserved.  QUALCOMM Proprietary.  Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //source/qcom/qct/modem/uim/gstk/rel/07H1_2/src/gstk_more_time.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/04/08   sk      reset timer for client response when a TR is being sent to
                   the card.
04/19/07   nk      Added check to avoid buffer overflow
04/12/07   jk      replace mem_malloc & mem_free with gstk_malloc & gstk_free
10/23/06   sk      Fixed lint errors.
06/24/06   sk      Lint fixes
11/14/05   sk      Fixed lint errors
11/09/05   sk      Fixed Lint Errors
05/11/05   sst     Lint fixes
03/05/05   tml     Added tag comprehension support
08/13/04   tml     Centralize release memory condition in the request function
08/03/04   tml     Memory leak fix
05/26/04   tml     Ensure successfully send TR to card when parsing error 
                   occurs.  Added More debug messages 
05/16/03   tml     linted
04/17/03   tml     lint
03/06/03   tml     Updated enum names
02/13/03   tml     Added/Changed DM messages
02/07/03   tml     Initial Version

===========================================================================*/


/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "gstk.h"
#include "gstk_proactive_cmd.h"
#include "gstkutil.h"
#include "gstk_terminal_rsp.h"

#include "err.h"
#include "string.h"
#include "tmc.h"

/*===========================================================================
FUNCTION gstk_more_time_req_cleanup

DESCRIPTION
  This function clean up any stack allocated pointer.

PARAMETERS
  request_ptr : [Input] Pointer to gstk_cmd_from_card_type
  cmd_data_ptr: [Input] Pointer to cmd_data_ptr

DEPENDENCIES
  None

RETURN VALUE
  None

COMMENTS
  None

SIDE EFFECTS
  None

SEE ALSO
  None
===========================================================================*/
static void gstk_more_time_req_cleanup(
  gstk_cmd_from_card_type *request_ptr, 
  uint8                   *cmd_data_ptr
)
{
  gstk_util_release_card_cmd_memory(request_ptr);
  if (cmd_data_ptr != NULL) {
    gstk_free(cmd_data_ptr);
  }
} /* gstk_more_time_req_cleanup */

/*===========================================================================
FUNCTION gstk_more_time_cnf_cleanup

DESCRIPTION
  This function cleans up any allocated pointer on the stack.

PARAMETERS
  more_time_cnf_ptr: [Input] Pointer to message that is required to
                             be processed and sent to UIM
  STK_response_ptr : [Input] Pointer to the STK intermediate message

DEPENDENCIES
  None

RETURN VALUE
  None

COMMENTS
  None

SIDE EFFECTS
  None

SEE ALSO
  None
===========================================================================*/
static void gstk_more_time_cnf_cleanup(
  gstk_more_time_cnf_type           *more_time_cnf_ptr,
  more_time_terminal_response_type  *STK_response_ptr)
{
  if(STK_response_ptr == NULL)
  {
    MSG_ERROR("STK_response_ptr ERR:NULL",0,0,0);
    return;
  }

  if(more_time_cnf_ptr == NULL)
  {
    MSG_ERROR("more_time_cnf_ptr ERR:NULL",0,0,0);
    return;
  }
  /* free more time cnf and tlv */
  if (STK_response_ptr->data.result.additional_info != NULL) {
    gstk_free(STK_response_ptr->data.result.additional_info);
    STK_response_ptr->data.result.additional_info = NULL;
  }
  gstk_util_release_upper_layer_cmd_memory(
    (gstk_cmd_type*)more_time_cnf_ptr);/*lint !e826 area too small */
} /* gstk_more_time_cnf_cleanup */

/*===========================================================================
FUNCTION gstk_more_time_req

DESCRIPTION
  This function parses the more time proactive command, restart the various
  proactive command timers within gstk.  The function also sends the
  more time command to its clients.

PARAMETERS
  command: [Input] Pointer to message parsed and sent to clients

DEPENDENCIES
  None

RETURN VALUE
  gstk_status_enum_type

COMMENTS
  None

SIDE EFFECTS
  None

SEE ALSO
  None
===========================================================================*/
gstk_status_enum_type gstk_more_time_req (
  gstk_proactive_cmd_type* command_ptr)
{
  uint8                              *cmd_data_ptr        = NULL;
  int32                               cmd_data_len        = 0;
  uint32                              cmd_details_ref_id  = 0;
  gstk_status_enum_type               gstk_status         = GSTK_SUCCESS;
  gstk_additional_info_type           additional_info     = {0};
  gstk_general_result_enum_type       general_result      = GSTK_COMMAND_PERFORMED_SUCCESSFULLY;
  uint32                              i                   = 0;
  gstk_terminal_rsp_extra_param_type  dummy_extra_param   = {0};

  /* Cannot do {0x00} in declaration because of compilation warning 
     on certain target: "enumerated type mixed with another type" */
  gstk_cmd_from_card_type             request;

  MSG_HIGH(" IN GSTK_MORE_TIME_REQ ",0,0,0);

  if(command_ptr == NULL)
  {
    MSG_ERROR("command ERR:NULL",0,0,0);
    return GSTK_BAD_PARAM;
  }

  /* more time cmd from card type contains header only, there is
      no need to initialize request command part */
  memset(&request, 0x00, sizeof(gstk_cmd_from_card_type));

  /* Check and populate command details into the cmd_details_ref_table
  ** Check Device ID validity
  ** put the rest of the command into the cmd_data_ptr, and specifies the length of
  ** the cmd_data_ptr in the cmd_data_len
  */
  gstk_status = gstk_preprocess_proactive_cmd((gstk_proactive_cmd_type*)command_ptr,
                                              &cmd_data_ptr,
                                              &cmd_data_len,
                                              &cmd_details_ref_id );

  /* GSTK_SUCCESS from preprocess_command */
  if(gstk_status == GSTK_SUCCESS)
  {
    /* No error has been encountered in parsing */
    /* restart all the proactive command timers */
    for (i = 0; i< GSTK_MAX_PROACTIVE_COMMAND; i++) {
      if (!command_details_ref_table[i].free_slot) {
        if (i != cmd_details_ref_id) {
          gstk_util_start_client_response_timer(i);
        }
      }
    }

    /* build Message to clients */
    request.hdr_cmd.command_id = GSTK_MORE_TIME_REQ;
    request.hdr_cmd.cmd_detail_reference = cmd_details_ref_id;

    /* command number */
    request.hdr_cmd.command_number =
            command_details_ref_table[cmd_details_ref_id].command_details.command_number;

    /* Send command to clients */
    gstk_status = gstk_util_send_message_to_clients(&request);

    if(gstk_status != GSTK_SUCCESS)
    {
      /* Send Terminal Response to Card */
      /* ME_CURRENTLY_UNABLE_TO_PROCESS_COMMAND */
      MSG_HIGH("More Time send client error", 0, 0, 0);

      /* Send Terminal Response to Card */
      gstk_set_additional_info(&additional_info, GSTK_NO_SPECIFIC_CAUSE_CAN_BE_GIVEN);
      general_result = GSTK_ME_CURRENTLY_UNABLE_TO_PROCESS_COMMAND;
    }
    else
    {
      MSG_MED("More Time Success", 0, 0, 0);
      /* Send terminal response success to card */
      general_result = GSTK_COMMAND_PERFORMED_SUCCESSFULLY;
      /* start the more time command timer */
      gstk_util_start_client_response_timer(cmd_details_ref_id);
      /* reset gstk_status to SUCCESS since there has been no error*/
      /* This is needed since some of the TLVs are optional, which might
      ** cause the various parsing functions to return invalid_command */
      gstk_status = GSTK_SUCCESS;
    }

  } /* if no error from preprocess command */
  else {
    ERR(" More Time parsing error", 0, 0, 0);
    /* Send Terminal Response to Card */
    /* COMMAND_DATA_NOT_UNDERSTOOD_BY_ME */
    general_result = GSTK_COMMAND_DATA_NOT_UNDERSTOOD_BY_ME;

  }

  /* send the Terminal Response */
  gstk_status = gstk_send_terminal_response( GSTK_CLIENT_ID, /* client_id */
                                0,              /* user_data */
                                cmd_details_ref_id, /* command_details_ref */
                                command_details_ref_table[cmd_details_ref_id].command_details.command_number,  /* command_num */
                                GSTK_MORE_TIME_CNF, /* command response */
                                general_result, /* general_result */
                                &additional_info, /* additional result */
                                &dummy_extra_param /* extra param */ );
  if(gstk_status != GSTK_SUCCESS) {
    MSG_ERROR("TP fail, %d", gstk_status, 0, 0);
  }
 
  /* release memory allocation for more time command if any */
  gstk_more_time_req_cleanup(&request, cmd_data_ptr);
  return gstk_status;
} /* gstk_more_time_req */

/*===========================================================================
FUNCTION gstk_more_time_cnf

DESCRIPTION
  This function packs the more time terminal response and send
  the command to UIM server.

PARAMETERS
  more_time_cnf_ptr: [Input] Pointer to message that is required to
                            be processed and sent to UIM

DEPENDENCIES
  None

RETURN VALUE
  gstk_status_enum_type

COMMENTS
  None

SIDE EFFECTS
  None

SEE ALSO
  None
===========================================================================*/
gstk_status_enum_type gstk_more_time_cnf(
  gstk_more_time_cnf_type* more_time_cnf_ptr)
{

  gstk_status_enum_type                    gstk_status  = GSTK_SUCCESS;
  more_time_terminal_response_type         STK_response = {0};
  uim_cmd_type                             *uim_cmd_ptr = NULL;
  int                                      offset       = 0;
  int                                      i            = 0;
  uint32                                   ref_id       = 0;

  MSG_HIGH("IN GSTK more time cnf ",0,0,0);

  if(more_time_cnf_ptr == NULL)
  {
    MSG_ERROR("more_time_cnf_ptr ERR:NULL",0,0,0);
    return GSTK_BAD_PARAM;
  }

  /* initialize STK_response */
  memset(&STK_response, 0x00, sizeof(more_time_terminal_response_type));

  /* Pack various TLVs */

  /* Command details */
  /* from the cached table */
  gstk_status = gstk_packer_cmd_details_tlv(
      more_time_cnf_ptr->cmd_details_ref_id,
      more_time_cnf_ptr->message_header.command_id,
      more_time_cnf_ptr->command_number,
      &STK_response.data.command_details );

  if (gstk_status != GSTK_SUCCESS) {
    gstk_more_time_cnf_cleanup(more_time_cnf_ptr, &STK_response);
    return gstk_status;
  }

  /* device ID */
  gstk_status = gstk_packer_dev_id_tlv(
      GSTK_ME_DEVICE,
      GSTK_UICC_SIM_DEVICE,
      &STK_response.data.device_id );


  if (gstk_status != GSTK_SUCCESS) {
    gstk_more_time_cnf_cleanup(more_time_cnf_ptr, &STK_response);
    return gstk_status;
  }

  /* result tag */
  gstk_status = gstk_packer_result_tlv(
      more_time_cnf_ptr->command_result,
      more_time_cnf_ptr->result_additional_info,
      &STK_response.data.result);

  if (gstk_status != GSTK_SUCCESS) {
    gstk_more_time_cnf_cleanup(more_time_cnf_ptr, &STK_response);
    return gstk_status;
  }

  /* header length:  command details data + 2   (2 is for the tag and length field)
                      + device id data + 2
                      + result data + 2
  */
  STK_response.num_bytes = STK_response.data.command_details.length +
                          STK_response.data.device_id.device_tag_length +
                          STK_response.data.result.result_tag_length +
                          6;

  /* Send response to STK application on SIM */
  /* Send Message to UIM */
  /* get a buffer for the request */
  uim_cmd_ptr = gstk_get_uim_buf();
  if (uim_cmd_ptr != NULL ) {
    gstk_status = gstk_util_populate_uim_buf_hdr_info(UIM_TERMINAL_RESPONSE_F,  &uim_cmd_ptr);
    if (gstk_status == GSTK_SUCCESS) {
      if (uim_cmd_ptr != NULL) {
        ref_id = more_time_cnf_ptr->cmd_details_ref_id;

        /* clear the timer for client response */
        (void) rex_clr_timer( &(command_details_ref_table[ref_id].tp_rpt_time));

        /* clear the command detail table reference slot before since we are about to reply to
          the card */
        gstk_util_cmd_details_ref_table_free_slot(more_time_cnf_ptr->cmd_details_ref_id);

        /* set user_data */
        uim_cmd_ptr->hdr.user_data               = 0;
        /* populate the terminal_response info */
        uim_cmd_ptr->terminal_response.num_bytes = STK_response.num_bytes;

        /* copy command details */
        gstk_byte_offset_memcpy(uim_cmd_ptr->terminal_response.data,
                              &STK_response.data.command_details,
                              offset,
                              (STK_response.data.command_details.length + 2),
                              UIM_MAX_CHARS,
                              sizeof(gstk_command_details_tlv_type));
        offset = (int)(offset + STK_response.data.command_details.length + 2);
        /* copy device */
        gstk_byte_offset_memcpy(uim_cmd_ptr->terminal_response.data,
                              &STK_response.data.device_id,
                              offset,
                              (STK_response.data.device_id.device_tag_length + 2),
                              UIM_MAX_CHARS,
                              sizeof(gstk_device_identities_tag_tlv_type));
        offset = (int)(offset + STK_response.data.device_id.device_tag_length + 2);
        /* copy result */
        gstk_byte_offset_memcpy(uim_cmd_ptr->terminal_response.data,
                               &STK_response.data.result,
                               offset,
                               3, /* result tag +  length + general result */
                               UIM_MAX_CHARS,
                               sizeof(gstk_result_tag_tlv_type));
        offset = offset + 3;
        if(STK_response.data.result.result_tag_length > 1 ) { /* has additional info */
          gstk_byte_offset_memcpy(uim_cmd_ptr->terminal_response.data,
                            STK_response.data.result.additional_info,
                            offset,
                            (STK_response.data.result.result_tag_length - 1),
                            UIM_MAX_CHARS,
                            (more_time_cnf_ptr->result_additional_info.length * 
                            sizeof(byte)));
          offset = (int)(offset + STK_response.data.result.result_tag_length - 1);
        }

        MSG_MED("More Time: TR 0x%x bytes", uim_cmd_ptr->terminal_response.num_bytes, 0, 0);
        for(i = 0; i< uim_cmd_ptr->terminal_response.num_bytes; i++) {
          MSG_MED("uim_cmd_ptr[%d] = 0x%x", i, uim_cmd_ptr->terminal_response.data[i], 0);
        }

        gstk_send_cmd_to_uim_server(uim_cmd_ptr); /* clear signal and call uim_cmd */
      }
      else {
        MSG_MED("Never here", 0, 0, 0);
      }
    }
  } /* uim_cmd_ptr != NULL */
  else {
    MSG_ERROR ("no UIM cmd buf", 0, 0, 0);
    gstk_status = GSTK_MEMORY_ERROR;
  }

  gstk_more_time_cnf_cleanup(more_time_cnf_ptr, &STK_response);
  return gstk_status;

} /* gstk_more_time_cnf*/

