/* ===========================================================================
 DESCRIPTION:  This file was auto-generated with the following command:
	 perl C:\Public\p4\msm6100\a\asic\msm6100\drivers\hw\msmreg.pl C:\Public\p4\msm6100\a\asic\msm6100\drivers\hw\CHIP_ADDRESS_FILE msm6100reg.h
-omHWIO
-i=internal_cougar.dat
-h=msmhwioreg.h
-init=bootreginit.c


 Date Generated: Wed Apr 17 11:02:38 2002
 Generated by  : Wed Apr 17 11:02:35 2002 C:\Public\p4\msm6100\a\asic\msm6100\drivers\hw\msmreg.pl 36662 bytes
                 Wed Apr 17 11:02:35 2002 msmreg.pm 20879 bytes
=========================================================================== */
void boot_msmreg_init(void)
{
	outp(DEMOD_RESET_WB, 0x00);
	outp(SRCH_CTL_WB, 0x00);
	outp(SRCH_TH_ENERGY_LOW_WB, 0x00);
	outp(SRCH_TH_ENERGY_HIGH_WB, 0x00);
	outp(SRCH_INTG_TIME_WB, 0x00);
	outp(SRCH_TH_TIME_WB, 0x00);
	outp(SRCH_OFFSET_LOW_WB, 0x00);
	outp(SRCH_OFFSET_HIGH_WB, 0x00);
	outp(SRCH_MASK_I_LOW_WB, 0x00);
	outp(SRCH_MASK_I_HIGH_WB, 0x00);
	outp(SRCH_MASK_Q_LOW_WB, 0x00);
	outp(SRCH_MASK_Q_HIGH_WB, 0x00);
	outp(SRCH_SLEW_LOW_WB, 0x00);
	outp(SRCH_SLEW_MID_WB, 0x00);
	outp(SRCH_SLEW_HIGH_WB, 0x00);
	outp(SRCH_ACC_PASS_WB, 0x00);
	outp(SRCH_NUM_LOW_WB, 0x00);
	outp(SRCH_NUM_HIGH_WB, 0x00);
	outp(SRCH_MAX_SELECT_WB, 0x00);
	outp(FFE_FINGER_PAGE_REG_WB, 0x00);
	outpw(FFE_MERGE_DETECT_CTRL0_WH, 0x0000);
	outpw(FFE_MERGE_DETECT_CTRL1_WH, 0x0000);
	outpw(FFE_MERGE_DETECT_CTRL2_WH, 0x0000);
	outp(FFE_FN_CTL1_WB, 0x00);
	outp(FFE_FN_CTL2_WB, 0x00);
	outp(FFE_FN_QOF_ENABLE_NOT_WB, 0x00);
	outpw(FFE_FN_BINARY_OFFSET_WH, 0x0000);
	outpw(FFE_FN_PN_I_MASK_WH, 0x0000);
	outpw(FFE_FN_PN_Q_MASK_WH, 0x0000);
	outp(FFE_FN_CODE_CHAN_ID_WB, 0x00);
	outpw(FFE_FN_SLEW_VALUE_LOWER_WH, 0x0000);
	outp(FFE_FN_SLEW_VALUE_UPPER_WB, 0x00);
	outpw(FFE_FN_CHAN_CTRL_WH, 0x0000);
	outp(FFE_FN_SUP_CODE1_WB, 0x00);
	outp(FFE_FN_SUP_CODE2_WB, 0x00);
	outp(FFE_FN_SUP_CODE3_WB, 0x00);
	outp(FFE_FN_SUP_CODE4_WB, 0x00);
	outp(FFE_FN_SUP_CODE5_WB, 0x00);
	outp(FFE_FN_SUP_CODE6_WB, 0x00);
	outp(FFE_FN_SUP_CODE7_WB, 0x00);
	outp(FFE_FN_CH0_QOF_SEL_WB, 0x00);
	outp(FFE_FN_CH1_QOF_SEL_WB, 0x00);
	outp(FFE_FN_CH2_QOF_SEL_WB, 0x00);
	outp(FFE_FN_CH4_QOF_SEL_WB, 0x00);
	outp(FFE_FN_CH6_QOF_SEL_WB, 0x00);
	outpw(FFE_FN_PILOT_CTRL_WH, 0x0000);
	outp(FFE_FINGER_CH2_TD_SIGN_WB, 0x00);
	outp(FFE_FN_CH3_QOF_SEL_WB, 0x00);
	outp(FFE_FINGER_CH1_TD_SIGN_WB, 0x00);
	outp(COMB_CHAN_CTL_WB, 0x00);
	outp(POWER_COMB_CTL_WB, 0x00);
	outpw(SYMB_COMB_CTL0_WH, 0x0000);
	outpw(SYMB_COMB_CTL1_WH, 0x0000);
	outp(SYMB_COMB_FIFO_DEPTH_WB, 0x00);
	outp(SYMB_COMB_POS_DUMP_WB, 0x00);
	outp(SYMB_COMB_FREQ_ADJ_LOWER_WB, 0x00);
	outp(SYMB_COMB_FREQ_ADJ_UPPER_WB, 0x00);
	outpw(SYMB_COMB_LONG_CODE_LD_0_WH, 0x0000);
	outpw(SYMB_COMB_LONG_CODE_LD_1_WH, 0x0000);
	outpw(SYMB_COMB_LONG_CODE_LD_2_WH, 0x0000);
	outpw(MAC_RND_CH1_WH, 0x0000);
	outp(DEM_CH1_SPR_WB, 0x00);
	outpw(SYMB_COMB_LONG_CODE_MASK_0_WH, 0x0000);
	outpw(SYMB_COMB_LONG_CODE_MASK_1_WH, 0x0000);
	outpw(SYMB_COMB_LONG_CODE_MASK_2_WH, 0x0000);
	outp(SYMB_COMB_LONG_CODE_MASK_SEL_WB, 0x00);
	outp(COMB_COUNT_INCR_UPPER_WB, 0x00);
	outpw(COMB_COUNT_INCR_LOWER_WH, 0x0000);
	outp(EPOCH_WR_WB, 0x00);
	outp(DEM_FRAME_CTL_WB, 0x00);
	outp(DEM_CTL_WB, 0x00);
	outpw(OOK_CTL_WH, 0x0000);
	outp(DEM_CH2_SPR_WB, 0x00);
	outp(FREQ_COMB_CTL_WB, 0x00);
	outpw(REFCNT_INCR_UPPER_WH, 0x0000);
	outpw(REFCNT_INCR_LOWER_WH, 0x0000);
	outp(STATUS_DUMP_CMD_WB, 0x00);
	outpw(TIME_INT1_MASK_WH, 0x0000);
	outpw(TIME_INT2_MASK_WH, 0x0000);
	outpw(MAC_RND_CH2_WH, 0x0000);
	outpw(FPC_CTL_WH, 0x0000);
	outpw(FPC_SEL_WH, 0x0000);
	outpw(WALSH_GAIN_WH, 0x0000);
	outpw(BETA_CTL_WH, 0x0000);
	outp(SW_BETA_CH0_WB, 0x00);
	outp(SW_BETA_CH2_WB, 0x00);
	outp(SYMB_COMB_CTL2_WB, 0x00);
	outp(DEM_PILOT_GATING_MASK_WB, 0x00);
	outpw(DEM_CTL2_WH, 0x0000);
	outp(SW_BETA_CH1_WB, 0x00);
	outp(SYMB_COMB_CPCCH_CTRL_2_WB, 0x00);
	outpw(SYMB_COMB_CPCCH_CTRL_1_WH, 0x0000);
	outp(FPC_CTL2_WB, 0x00);
	outp(SRCH2_CTL_WB, 0x00);
	outpw(SRCH2_QUEUE_WH, 0x0000);
	outp(SRCH_QOF_SEL_WB, 0x00);
	outp(SRCH_STATUS_DUMP_CTL_WB, 0x00);
	outp(SRCH_WALSH_NUM_LOW_WB, 0x00);
	outp(SRCH_WALSH_NUM_HIGH_WB, 0x00);
	outpw(TX_RATE_ADJ_WH, 0x0000);
	outpw(I_OFFSET_WR_WH, 0x0000);
	outpw(Q_OFFSET_WR_WH, 0x0000);
	outp(IQ_OFFSET_CTL_WB, 0x00);
	outpw(RAS_RAM_DATA_WH, 0x0000);
	outp(AGC_TEST_CTL_WB, 0x00);
	outp(AGC_RDWR_CTL_WB, 0x00);
	outp(AGC_CTL_WB, 0x00);
	outp(AGC_CTL2_WB, 0x00);
	outpw(RX_AGC_ADJ_WR_WH, 0x0000);
	outpw(TX_AGC_ADJ_WR_WH, 0x0000);
	outpw(AGC_VALUE_WR_WH, 0x0000);
	outp(AGC_VALUE_MAX_WB, 0x00);
	outp(AGC_VALUE_1_MIN_WB, 0x00);
	outp(AGC_CTL3_WB, 0x00);
	outp(AGC_CTL4_WB, 0x00);
	outpw(TX_GAIN_LIMIT_WR_WH, 0x0000);
	outpw(TX_ATTEN_LIMIT_WR_WH, 0x0000);
	outp(IM_STATE_CTL_WB, 0x00);
	outp(LNA_1_FALL_WB, 0x00);
	outp(LNA_1_RISE_WB, 0x00);
	outpw(LNA_FILT_WR_WH, 0x0000);
	outp(AGC_CTL5_WB, 0x00);
	outp(AGC_CTL6_WB, 0x00);
	outpw(LNA_GAIN_WR_WH, 0x0000);
	outp(LNA_GAIN_MIN_WB, 0x00);
	outpw(LNA_GAIN_PWR_MAX_WH, 0x0000);
	outpw(TX_OPEN_LOOP_WR_WH, 0x0000);
	outpw(TX_GAIN_ADJ_WR_WH, 0x0000);
	outp(PA_R1_RISE_WB, 0x00);
	outp(PA_R1_FALL_WB, 0x00);
	outp(PA_R2_RISE_WB, 0x00);
	outp(PA_R2_FALL_WB, 0x00);
	outp(PA_R3_RISE_WB, 0x00);
	outp(PA_R3_FALL_WB, 0x00);
	outp(PA_R_MAP_WB, 0x00);
	outp(AGC_CTL7_WB, 0x00);
	outp(PA_R_TIMER_WB, 0x00);
	outp(AGC_RESET_WB, 0x00);
	outp(TX_ALIGN_DELAY_WB, 0x00);
	outpw(AGC_PHASE_OFFSET_WH, 0x0000);
	outpw(AGC_TRK_LO_ADJ_WR_WH, 0x0000);
	outp(IM_LEVEL1_WB, 0x00);
	outp(IM_LEVEL2_WB, 0x00);
	outp(LNA_2_RISE_WB, 0x00);
	outp(LNA_2_FALL_WB, 0x00);
	outpw(LNA_1_OFFSET_WH, 0x0000);
	outpw(LNA_2_OFFSET_WH, 0x0000);
	outpw(LNA_2_BACKOFF_WH, 0x0000);
	outpw(LNA_1_BACKOFF_WH, 0x0000);
	outp(AGC_VALUE_2_MIN_WB, 0x00);
	outpw(PA_S0_OFFSET_WH, 0x0000);
	outpw(PA_S1_OFFSET_WH, 0x0000);
	outpw(PA_S2_OFFSET_WH, 0x0000);
	outpw(PA_S3_OFFSET_WH, 0x0000);
	outp(TX_PDM_DELAY_VAL_WB, 0x00);
	outp(RX_PDM_DELAY_VAL_WB, 0x00);
	outp(RF_ATTEN_PDM_DELAY_VAL_WB, 0x00);
	outp(PA_WARMUP_TIMER_WB, 0x00);
	outp(AGC_CTL8_WB, 0x00);
	outpw(SUBPCG_PA_WARMUP_DELAY_WH, 0x0000);
	outpw(VERY_EARLY_FRAME_DELAY_REG_WH, 0x0000);
	outp(AGC_CTL9_WB, 0x00);
	outp(AGC_VALUE_3_MIN_WB, 0x00);
	outp(AGC_VALUE_4_MIN_WB, 0x00);
	outp(RF_MODE_SEL_WB, 0x00);
	outp(IM_LEVEL3_WB, 0x00);
	outp(LNA_3_RISE_WB, 0x00);
	outp(LNA_3_FALL_WB, 0x00);
	outpw(LNA_3_OFFSET_WH, 0x0000);
	outpw(LNA_3_BACKOFF_WH, 0x0000);
	outp(IM_LEVEL4_WB, 0x00);
	outp(LNA_4_RISE_WB, 0x00);
	outp(LNA_4_FALL_WB, 0x00);
	outpw(LNA_4_OFFSET_WH, 0x0000);
	outpw(LNA_4_BACKOFF_WH, 0x0000);
	outpdw(AGC_SBI_PACKET_DATA_WW, 0x00000000);
	outp(AGC_DC_GAIN_WB, 0x00);
	outpw(LNA_BYPASS_TIMER_0_WH, 0x0000);
	outpw(LNA_BYPASS_TIMER_1_WH, 0x0000);
	outpw(LNA_BYPASS_TIMER_2_WH, 0x0000);
	outpw(LNA_BYPASS_TIMER_3_WH, 0x0000);
	outpw(LNA_NONBYPASS_TIMER_0_WH, 0x0000);
	outpw(LNA_NONBYPASS_TIMER_1_WH, 0x0000);
	outpw(LNA_NONBYPASS_TIMER_2_WH, 0x0000);
	outpw(LNA_NONBYPASS_TIMER_3_WH, 0x0000);
	outp(LNA_RANGE_DELAY_WB, 0x00);
	outp(AGC_TX_RCCCH_FRAME_DELAY_WB, 0x00);
	outp(AGC_TX_MASK_DATA_SEL_WB, 0x00);
	outp(DEM_TESTBUS1_SEL_WB, 0x00);
	outp(SAMP_RAM_BYPASS_WR_WB, 0x00);
	outp(DBG_SRCH4_WB, 0x00);
	outpw(MOD_PRMBL_GAIN_WH, 0x0000);
	outpw(MOD_FREQ_ERR_WH, 0x0000);
	outpw(MOD_ROTANGLE_0_WH, 0x0000);
	outpw(MOD_ROTANGLE_1_WH, 0x0000);
	outp(MOD_ROTANGLE_MSB_WB, 0x00);
	outp(MOD_RESET_WB, 0x00);
	outp(MOD_CH1_TIMING_CTL_WB, 0x00);
	outp(MOD_CH2_TIMING_CTL_WB, 0x00);
	outp(MOD_CH3_TIMING_CTL_WB, 0x00);
	outp(I_PN_STATE_0_WB, 0x00);
	outp(I_PN_STATE_1_WB, 0x00);
	outp(Q_PN_STATE_0_WB, 0x00);
	outp(Q_PN_STATE_1_WB, 0x00);
	outp(U_PN_STATE_0_WB, 0x00);
	outp(U_PN_STATE_1_WB, 0x00);
	outp(U_PN_STATE_2_WB, 0x00);
	outp(U_PN_STATE_3_WB, 0x00);
	outp(U_PN_STATE_4_WB, 0x00);
	outp(U_PN_STATE_5_WB, 0x00);
	outp(U_PN_MASK_0_WB, 0x00);
	outp(U_PN_MASK_1_WB, 0x00);
	outp(U_PN_MASK_2_WB, 0x00);
	outp(U_PN_MASK_3_WB, 0x00);
	outp(U_PN_MASK_4_WB, 0x00);
	outp(U_PN_MASK_5_WB, 0x00);
	outp(FRAME_OFF_WB, 0x00);
	outp(PA_WARMUP_WB, 0x00);
	outpw(MOD_STMR_MODIFIER_0_WH, 0x0000);
	outp(MOD_STMR_MODIFIER_1_WB, 0x00);
	outp(MOD_STMR_CMD_WB, 0x00);
	outpw(ENC_INT_ST_WH, 0x0000);
	outpw(MOD_CH1_ENC_CTL_WH, 0x0000);
	outpw(MOD_CH1_ENC_DATA_WH, 0x0000);
	outpw(MOD_CH1_CRC_POLY_WH, 0x0000);
	outp(MOD_CH1_PUNCT_PATN_1_WB, 0x00);
	outpw(MOD_CH1_PUNCT_PATN_0_WH, 0x0000);
	outpw(MOD_CH2_ENC_CTL_0_WH, 0x0000);
	outp(MOD_CH2_ENC_CTL_1_WB, 0x00);
	outpw(MOD_CH2_ENC_DATA_WH, 0x0000);
	outp(MOD_CH2_CRC_POLY_WB, 0x00);
	outp(MOD_CH2_PUNCT_PATN_1_WB, 0x00);
	outp(MOD_CH2_PUNCT_PATN_0_WB, 0x00);
	outp(MOD_CH3_ENC_CTL_0_WB, 0x00);
	outp(MOD_CH3_ENC_CTL_1_WB, 0x00);
	outpw(MOD_CH3_ENC_DATA_WH, 0x0000);
	outp(MOD_CH3_CRC_POLY_WB, 0x00);
	outp(MOD_CH3_PUNCT_PATN_1_WB, 0x00);
	outp(MOD_CH3_PUNCT_PATN_0_WB, 0x00);
	outp(MOD_SCH_LTU_CTL_WB, 0x00);
	outp(MOD_MISC_CTL_WB, 0x00);
	outp(MOD_PCH_GAIN_WB, 0x00);
	outp(MOD_DCCH_GAIN_WB, 0x00);
	outp(MOD_FCH_GAIN_WB, 0x00);
	outp(MOD_SCH_GAIN_WB, 0x00);
	outpw(MOD_PRMBL_CTL_0_WH, 0x0000);
	outpw(MOD_PRMBL_CTL_1_WH, 0x0000);
	outp(MOD_ROTATOR_MAP_WB, 0x00);
	outp(MOD_WSYM_STATE_WB, 0x00);
	outp(TX_VERY_EARLY_FRAME_CTL_WB, 0x00);
	outp(TX_2_EARLY_PCG_CTL_WB, 0x00);
	outp(TX_I_CLK_WB, 0x00);
	outp(MOD_CLK_CTL_WB, 0x00);
	outpw(TX_DATA_TEST_WH, 0x0000);
	outpw(TX_Q_DATA_TEST_WH, 0x0000);
	outpw(MOD_TEST_CTL_WH, 0x0000);
	outpw(MOD_PCH_PCBIT_DATA_WH, 0x0000);
	outpw(MOD_PCH_PCBIT_MASK_WH, 0x0000);
	outpw(MOD_WCOVER_SEL_WH, 0x0000);
	outp(TX_WARMUP_WB, 0x00);
	outpw(DFM_INITIAL_WH, 0x0000);
	outp(DFM_DC_OFFSET_GAIN_WB, 0x00);
	outp(DFM_AGC_REF_WB, 0x00);
	outp(DFM_AGC_ACC_MIN_WB, 0x00);
	outp(DFM_AGC_ACC_MAX_WB, 0x00);
	outp(DFM_AGC_GAIN_WB, 0x00);
	outp(DFM_FREQ_LOOP_CONFIG_WB, 0x00);
	outp(DFM_PDM_CONFIG_WB, 0x00);
	outp(DFM_DC_PDM_0_WB, 0x00);
	outp(DFM_DC_PDM_1_WB, 0x00);
	outp(DFM_DC_PDM_2_WB, 0x00);
	outp(DFM_RXAGC_PDM_0_WB, 0x00);
	outp(DFM_FREQ_PDM_0_WB, 0x00);
	outp(DFM_FREQ_PDM_1_WB, 0x00);
	outp(DFM_TXWBD_INTF_0_WB, 0x00);
	outp(DFM_TXWBD_INTF_1_WB, 0x00);
	outp(DFM_MAX_TX_PWR_0_WB, 0x00);
	outp(DFM_MAX_TX_PWR_1_WB, 0x00);
	outp(DFM_FREQ_SENS_GAIN_WB, 0x00);
	outp(DFM_TXFM_CONFIG_WB, 0x00);
	outp(DFM_MIN1_BYTE_0_WB, 0x00);
	outp(DFM_MIN1_BYTE_1_WB, 0x00);
	outp(DFM_MIN1_BYTE_2_WB, 0x00);
	outp(DFM_RXWBD_BANDWIDTH_WB, 0x00);
	outp(DFM_RXWBD_CONFIG_0_WB, 0x00);
	outp(DFM_RXWBD_WR_WB, 0x00);
	outp(DFM_RXAGC_PDM_1_WB, 0x00);
	outp(DFM_RXWBD_CONFIG_1_WB, 0x00);
	outp(DFM_SLOT_CTL_1_WB, 0x00);
	outp(DFM_SLOT_PDM_CTL_WB, 0x00);
	outp(DFM_SLOT_CTL_WB, 0x00);
	outp(DFM_DPLL_WU_TIMER_WB, 0x00);
	outp(DFM_SYNC_WU_TIMER_WB, 0x00);
	outp(DFM_DATA_WU_TIMER_WB, 0x00);
	outp(DFM_STREAM_SLOT_TIMER_WB, 0x00);
	outp(DFM_RF_WU_OFFSET_WB, 0x00);
	outp(DFM_NCO_TEST0_WB, 0x00);
	outp(DFM_NCO_TEST1_WB, 0x00);
	outp(DFM_LOOPBACK_CNTL_WB, 0x00);
	outp(DFM_LNA_CNTL_0_WB, 0x00);
	outpw(DFM_LNA_OFFSET_WH, 0x0000);
	outp(DFM_LNA_FALL_WB, 0x00);
	outp(DFM_LNA_RISE_WB, 0x00);
	outpw(DFM_LNA_BYPASS_TIMER_WH, 0x0000);
	outpw(DFM_LNA_NONBYPASS_TIMER_WH, 0x0000);
	outp(DFM_LNA_FOLLOWER_DELAY_WB, 0x00);
	outpw(DFM_LNA_RANGE_DELAY_WH, 0x0000);
	outp(DFM_AGC_ACC_MIN_1_WB, 0x00);
	outp(DFM_AGC_IM_GAIN_WB, 0x00);
	outp(DFM_AGC_DC_GAIN_WB, 0x00);
	outp(DFM_LNA_CNTL_1_WB, 0x00);
	outp(DFM_LNA_ORIDE_DATA_WB, 0x00);
	outpw(DFM_AGC_SBI_PACKET_WH, 0x0000);
	outp(DFM_DATA_FREEZE_CTL_WB, 0x00);
	outpw(DFM_DATA_FREEZE_DELAY_WH, 0x0000);
	outp(CALIBRATION_CTL_WB, 0x00);
	outp(DFM_LNA_DECISION_DELAY_WB, 0x00);
	outp(DFM_IM_LEVEL1_WB, 0x00);
	outp(SLEEP_CTL_WB, 0x00);
	outpdw(POWER_DOWN_SLEEP_INTERVAL_WW, 0x00000000);
	outpw(POWER_DOWN_WU_TIME_WH, 0x0000);
	outpw(POWER_DOWN_CHIPX8_SLEEP_TIME_WH, 0x0000);
	outpdw(PAD_HDRIVE_SEL_0_WW, 0x00000000);
	outp(PAD_HDRIVE_SEL_1_WB, 0x00);
	outpdw(PAD_HDRIVE_SEL_2_WW, 0x00000000);
	outp(TLMM_TEST_SEL_WB, 0x00);
	outpw(PAD_PUPD_N_WH, 0x0000);
	outpw(PAD_PUPD_EN_WH, 0x0000);
	outpw(WEB_MISC_WR_WH, 0x0000);
	outpw(WEB_MISC2_WR_WH, 0x0000);
	outp(PA_ON_CTL_WB, 0x00);
	outp(SYSTEM_MODE_WB, 0x00);
	outp(GPS_SETUP_WB, 0x00);
	outpw(GP_MN_CLK_MDIV_WH, 0x0000);
	outpw(GP_MN_CLK_NDIV_WH, 0x0000);
	outpw(GP_MN_CLK_DUTY_WH, 0x0000);
	outpw(RINGER_MN_A_DUTY_WH, 0x0000);
	outpw(RINGER_MN_A_NDIV_WH, 0x0000);
	outp(RINGER_MN_A_MDIV_WB, 0x00);
	outpw(RINGER_MN_B_DUTY_WH, 0x0000);
	outpw(RINGER_MN_B_NDIV_WH, 0x0000);
	outp(RINGER_MN_B_MDIV_WB, 0x00);
	outp(TIME_TICK_CTL_WB, 0x00);
	outp(GPTIMER_CTL_WB, 0x00);
	outp(TCXO_PDM_CTL_WB, 0x00);
	outp(PDM2_CTL_WB, 0x00);
	outp(PDM1_CTL_WB, 0x00);
	outp(TEST_ADIE_DATA_WB, 0x00);
	outp(TEST_ADIE_DATA_SEL_WB, 0x00);
	outp(PCM_PATH_CTL_WB, 0x00);
	outpw(CODEC_CTL_WH, 0x0000);
	outp(SLEEP_N_ADIE_WB, 0x00);
	outpw(TEST_POINT_SEL_WH, 0x0000);
	outpw(TEST_POINT_DIN_WH, 0x0000);
	outpw(TEST_MEM_SEL_WH, 0x0000);
	outpw(TEST_MEM_ADDR_WH, 0x0000);
	outpw(TEST_MEM_WRITE_WH, 0x0000);
	outpw(TEST_MEM_DIN_WH, 0x0000);
	outp(RXF_RESET_WB, 0x00);
	outpdw(RXF_CTL_WW, 0x00000000);
	outpdw(RXF_AMP_ADJ1_COEFF_WW, 0x00000000);
	outpw(RXF_AMP_ADJ2_COEFF_WH, 0x0000);
	outpdw(RXF_PHASE_COMP1_COEFF_WW, 0x00000000);
	outpdw(RXF_DC_CORR_FACTOR_WW, 0x00000000);
	outpdw(RXF_DVGA_CTL_WW, 0x00000000);
	outpw(RXF_DVGA_PARAMS_WH, 0x0000);
	outpdw(RXF_OFFSET_CTL_WW, 0x00000000);
	outpdw(RXF_DACC_CTL_WW, 0x00000000);
	outpdw(RXF_DC_IOFFSET_ADJ_WW, 0x00000000);
	outpdw(RXF_DC_QOFFSET_ADJ_WW, 0x00000000);
	outp(RXF_PDM_RESET_WB, 0x00);
	outpw(RXF_CG_LNA_ACQ_TIME_WH, 0x0000);
	outpw(RXF_CG_MIX_ACQ_TIME_WH, 0x0000);
	outpdw(DACC_ACQ_TIMER_WW, 0x00000000);
	outpdw(DACC_TRC_TIMER_WW, 0x00000000);
	outpw(DACC_CLR_TIMER_WH, 0x0000);
	outpdw(DACC_EST_THRESH_WW, 0x00000000);
	outp(MICRO_DACC_UPDATE_WB, 0x00);
	outpdw(DACC_SBI_INFO_WW, 0x00000000);
	outpw(DACC_GAIN_MULT_WH, 0x0000);
	outp(RXF_CG_ACQ_WB, 0x00);
	outpdw(RXF_MIS_COMP_A_COEFF_WW, 0x00000000);
	outpdw(RXF_MIS_COMP_B_COEFF_WW, 0x00000000);
	outpw(RXF_DFM_IF_OFFSET_WH, 0x0000);
	outpw(RXF_PHASE_DEL_WH, 0x0000);
	outpw(RXF_LNA_S0_PHASE_OFFSET_WH, 0x0000);
	outpw(RXF_LNA_S1_PHASE_OFFSET_WH, 0x0000);
	outpw(RXF_LNA_S2_PHASE_OFFSET_WH, 0x0000);
	outpw(RXF_LNA_S3_PHASE_OFFSET_WH, 0x0000);
	outpw(RXF_LNA_S4_PHASE_OFFSET_WH, 0x0000);
	outp(RXF_DF2_DC_OFFSET_WB, 0x00);
	outpdw(INT_CLEAR_0_WW, 0x00000000);
	outpdw(INT_CLEAR_1_WW, 0x00000000);
	outpdw(GPIO_INT_CLEAR_0_WW, 0x00000000);
	outpw(GPIO_INT_CLEAR_1_WH, 0x0000);
	outpdw(IRQ_EN_0_WW, 0x00000000);
	outp(IRQ_EN_1_WB, 0x00);
	outpdw(FIQ_EN_0_WW, 0x00000000);
	outp(FIQ_EN_1_WB, 0x00);
	outpdw(GPIO_INT_EN_0_WW, 0x00000000);
	outpw(GPIO_INT_EN_1_WH, 0x0000);
	outpdw(INT_POLARITY_0_WW, 0x00000000);
	outpw(INT_POLARITY_1_WH, 0x0000);
	outp(INT_POLARITY_2_WB, 0x00);
	outpdw(GPIO_INT_DETECT_CTL_0_WW, 0x00000000);
	outpw(GPIO_INT_DETECT_CTL_1_WH, 0x0000);
	outp(DINT_RESET_WB, 0x00);
	outpw(DINT_CFG_WH, 0x0000);
	outp(DINT_CH0_CFG_WB, 0x00);
	outp(DINT_CH1_CFG_WB, 0x00);
	outp(DINT_CH2_CFG_WB, 0x00);
	outpw(TD_BLOCK_SIZE_WH, 0x0000);
	outpw(TD_INTLV_CFG_LO_WH, 0x0000);
	outpw(TD_INTLV_CFG_HI_WH, 0x0000);
	outpw(TD_INTLV_SIZE_LO_WH, 0x0000);
	outpw(TD_INTLV_SIZE_HI_WH, 0x0000);
	outpw(TD_PUNCT_LO_WH, 0x0000);
	outpw(TD_PUNCT_HI_WH, 0x0000);
	outpw(TD_PARAMS_LO_WH, 0x0000);
	outpw(TD_PARAMS_HI_WH, 0x0000);
	outp(DINT_PKT_OFFSET_WB, 0x00);
	outpw(DINT_PKT_CFG_WH, 0x0000);
	outp(DINT_TASK_OFFSET_WB, 0x00);
	outpw(DINT_TASK_LIST_WH, 0x0000);
	outpw(TASK_LIST_LO_WH, 0x0000);
	outpw(TASK_LIST_HI_WH, 0x0000);
	outpw(TST_SYNC_DINT_WH, 0x0000);
	outpw(TST_CH0_SYMS_WH, 0x0000);
	outpw(TST_CH1_SYMS_WH, 0x0000);
	outpw(TST_CH2_SYMS_WH, 0x0000);
	outpw(DEC_OB_ADDRESS_WH, 0x0000);
	outp(TST_SEL_DINT_WB, 0x00);
	outp(DINT_OTD_CFG_WB, 0x00);
	outpw(TD_MIN_LLR_THRESH_WH, 0x0000);
	outpw(TD_INTLV_LEN_X2_WH, 0x0000);
	outpw(TD_INTLV_LEN_X4_WH, 0x0000);
	outpw(TD_INTLV_LEN_X8_WH, 0x0000);
	outp(TD_NUM_SLWIN_X2_WB, 0x00);
	outp(TD_NUM_SLWIN_X4_WB, 0x00);
	outp(TD_NUM_SLWIN_X8_WB, 0x00);
	outp(TD_RESET_WB, 0x00);
	outp(TD_TEST_START_WB, 0x00);
	outpw(TD_TEST_MODE_CFG_MSB_WH, 0x0000);
	outp(TD_TEST_MODE_CFG_LSB_WB, 0x00);
	outpw(TD_TEST_SYM_LOAD_WH, 0x0000);
	outpw(SBI_CLK_CTL_WH, 0x0000);
	outpdw(SBI_CTL_WW, 0x00000000);
	outpw(SBI_BYPASS_WR_WH, 0x0000);
	outpw(SBI_WR_WH, 0x0000);
	outpw(SBI_START_CTL_WH, 0x0000);
	outpw(SBI_SECOND_IDCODE_WH, 0x0000);
	outpw(I2C_WRITE_DATA_WH, 0x0000);
	outp(VD_RESET_WB, 0x00);
	outp(VD_MODE_WB, 0x00);
	outpw(VD_POLY2IJ_WH, 0x0000);
	outpw(VD_POLY3IJ_WH, 0x0000);
	outp(VD_POLY3K_WB, 0x00);
	outpw(VD_POLY4IJ_WH, 0x0000);
	outpw(VD_POLY4KL_WH, 0x0000);
	outp(VD_TESTCON_WB, 0x00);
	outp(UART2_CSR_WB, 0x00);
	outp(UART2_TF_WB, 0x00);
	outp(UART2_CR_WB, 0x00);
	outp(UART2_IMR_WB, 0x00);
	outp(UART2_IRDA_WB, 0x00);
	outpw(UART2_SIM_CFG_WH, 0x0000);
	outpw(SRCH4_CNTR_TEST_WH, 0x0000);
	outp(UART_CSR_WB, 0x00);
	outp(UART_TF_WB, 0x00);
	outp(UART_CR_WB, 0x00);
	outp(UART_IMR_WB, 0x00);
	outp(UART_IRDA_WB, 0x00);
	outp(UART3_CSR_WB, 0x00);
	outp(UART3_TF_WB, 0x00);
	outp(UART3_CR_WB, 0x00);
	outp(UART3_IMR_WB, 0x00);
	outp(UART3_IRDA_WB, 0x00);
	outpw(UART3_SIM_CFG_WH, 0x0000);
	outpw(GSLP_INT_CLEAR_WR_WH, 0x0000);
	outpdw(GSLP_ON_LINE_TIME_CONFIG_WW, 0x00000000);
	outpw(GSLP_WU_0_TIME_CONFIG_WH, 0x0000);
	outpw(GSLP_WU_0_ACT_TIME_0_CONFIG_WH, 0x0000);
	outpw(GSLP_WU_0_ACT_TIME_1_CONFIG_WH, 0x0000);
	outpdw(GSLP_FEE_NUM_SLEEP_CLK_WW, 0x00000000);
	outpdw(GSLP_WDOG_TEST_WW, 0x00000000);
	outpdw(GSLP_TEST_BUS_SEL_WW, 0x00000000);
	outpdw(GSM_TIME_LOAD_CTL_CMD_WW, 0x00000000);
	outpdw(GSM_TIME_TC_ADJ_CMD_WW, 0x00000000);
	outpdw(VFR_IRQ_ALIGN_CMD_WW, 0x00000000);
	outpdw(GO_TO_SLEEP_CMD_WW, 0x00000000);
	outpdw(GPIO_OUT_0_WW, 0x00000000);
	outp(GPIO_OUT_1_WB, 0x00);
	outpw(GPIO_OUT_4_WH, 0x0000);
	outpdw(GPIO_OE_0_WW, 0x00000000);
	outp(GPIO_OE_1_WB, 0x00);
	outpw(GPIO_OE_4_WH, 0x0000);
	outp(DBG_BUS_SEL_WB, 0x00);
	outpdw(DBG_BUS_OE_WW, 0x00000000);
	outp(GPIO_PAGE_WB, 0x00);
	outp(GPIO_CFG_WB, 0x00);
	outpw(ARM_TEST_MEM_SEL_WH, 0x0000);
	outpw(ARM_TEST_MEM_ADDR_WH, 0x0000);
	outpdw(ARM_TEST_MEM_WRITE_WW, 0x00000000);
	outpdw(SDAC_MNCNTR_NOT_N_M_WW, 0x00000000);
	outpw(SDAC_MNCNTR_M_WH, 0x0000);
	outpw(SDAC_MNCNTR_D_WH, 0x0000);
	outpdw(BT_MNCNTR_NOT_N_M_WW, 0x00000000);
	outpw(BT_MNCNTR_M_WH, 0x0000);
	outpw(BT_MNCNTR_D_WH, 0x0000);
	outpw(PLL_M_VAL_WH, 0x0000);
	outpw(PLL_N_VAL_WH, 0x0000);
	outpw(PLL_L_VAL_WH, 0x0000);
	outpdw(PLL_INTERNAL1_WW, 0x00000000);
	outpdw(PLL_INTERNAL2_WW, 0x00000000);
	outpw(PLL_MNCNTR_M_WH, 0x0000);
	outpw(PLL_MNCNTR_D_WH, 0x0000);
	outpw(PLL_MNCNTR_NOT_N_M_WH, 0x0000);
	outpw(GPS_MNCNTR_M_WH, 0x0000);
	outpw(GPS_MNCNTR_D_WH, 0x0000);
	outpw(GPS_MNCNTR_NOT_N_M_WH, 0x0000);
	outpw(CPLL_M_VAL_WH, 0x0000);
	outpw(CPLL_N_VAL_WH, 0x0000);
	outpw(CPLL_L_VAL_WH, 0x0000);
	outpdw(CPLL_INTERNAL1_WW, 0x00000000);
	outpdw(CPLL_INTERNAL2_WW, 0x00000000);
	outpw(CPLL_MNCNTR_M_WH, 0x0000);
	outpw(CPLL_MNCNTR_D_WH, 0x0000);
	outpw(CPLL_MNCNTR_NOT_N_M_WH, 0x0000);
	outpw(SBI_MNCNTR_M_WH, 0x0000);
	outpw(SBI_MNCNTR_D_WH, 0x0000);
	outpw(SBI_MNCNTR_NOT_N_M_WH, 0x0000);
	outpw(ADSP_MNCNTR_M_WH, 0x0000);
	outpw(ADSP_MNCNTR_D_WH, 0x0000);
	outpw(ADSP_MNCNTR_NOT_N_M_WH, 0x0000);
	outpw(RXDSP_MNCNTR_M_WH, 0x0000);
	outpw(RXDSP_MNCNTR_D_WH, 0x0000);
	outpw(RXDSP_MNCNTR_NOT_N_M_WH, 0x0000);
	outpw(CHIP_MNCNTR_M_WH, 0x0000);
	outpw(CHIP_MNCNTR_D_WH, 0x0000);
	outpw(CHIP_MNCNTR_NOT_N_M_WH, 0x0000);
	outpw(GSM_MNCNTR_M_WH, 0x0000);
	outpw(GSM_MNCNTR_D_WH, 0x0000);
	outpw(GSM_MNCNTR_NOT_N_M_WH, 0x0000);
	outpw(DPLL_L_VAL_WH, 0x0000);
	outpdw(DPLL_INTERNAL1_WW, 0x00000000);
	outpdw(DPLL_INTERNAL2_WW, 0x00000000);
	outpw(ICODEC_MNCNTR_M_WH, 0x0000);
	outpw(ICODEC_MNCNTR_D_WH, 0x0000);
	outpw(ICODEC_MNCNTR_NOT_N_M_WH, 0x0000);
	outpw(PAUSE_TIMER_WH, 0x0000);
	outp(DEFRAMER_CMD_WB, 0x00);
	outp(FRAMER_CMD_WB, 0x00);
	outp(FRAMER_ESC_DEL_WB, 0x00);
	outp(FRAMER_IGN_MSB_WB, 0x00);
	outp(FRAMER_CTL_WB, 0x00);
	outpdw(MSM_ACCESS_CFG_WW, 0x00000000);
	outpdw(USB_ACCESS_CFG_WW, 0x00000000);
	outpdw(GPIO2_ACCESS_CFG_WW, 0x00000000);
	outp(MDSP_INTF_CFG_WB, 0x00);
	outp(ADSP_INTF_CFG_WB, 0x00);
	outp(EBI1_MPMC_STDY_SEL_WB, 0x00);
	outp(EBI1_PSRAM_CRE_WB, 0x00);
	outpdw(EBI1_CS1_CFG0_WW, 0x00000000);
	outp(EBI1_CS1_CFG1_WB, 0x00);
	outpdw(EBI1_CS3_CFG0_WW, 0x00000000);
	outp(EBI1_CS3_CFG1_WB, 0x00);
	outp(EBI1_MEM_CTLR_SEL_CMD_WB, 0x00);
	outp(GP0_CFG1_WB, 0x00);
	outpdw(GP1_CFG0_WW, 0x00000000);
	outp(GP1_CFG1_WB, 0x00);
	outpdw(RAM2_CFG0_WW, 0x00000000);
	outp(RAM2_CFG1_WB, 0x00);
	outpdw(ROM2_CFG0_WW, 0x00000000);
	outp(ROM2_CFG1_WB, 0x00);
	outpdw(LCD_CFG0_WW, 0x00000000);
	outpdw(LCD_CFG1_WW, 0x00000000);
	outp(DBGEN_STATE_WB, 0x00);
	outp(DELAY_CNTL_WB, 0x00);
	outpdw(FRAMER_DATA_IN_WW, 0x00000000);
	outpdw(DEFRAMER_DATA_IN_WW, 0x00000000);
	outpdw(MDSP_CTL_WW, 0x00000000);
	outpdw(MICRO_TO_MDSP_IRQ_TEST_WW, 0x00000000);
	outpdw(MDSP_MEMI_CLK_CTL_WW, 0x00000000);
	outpdw(MDSP_MEMA_CLK_CTL_WW, 0x00000000);
	outpdw(MDSP_MEMB_CLK_CTL_WW, 0x00000000);
	outpdw(MDSP_MEMC_CLK_CTL_WW, 0x00000000);
	outpdw(ADSP_CTL_WW, 0x00000000);
	outpdw(MICRO_TO_ADSP_IRQ_TEST_WW, 0x00000000);
	outpdw(ADSP_MEMI_CLK_CTL_WW, 0x00000000);
	outpdw(ADSP_MEMA_CLK_CTL_WW, 0x00000000);
	outpdw(ADSP_MEMB_CLK_CTL_WW, 0x00000000);
	outpdw(ADSP_MEMC_CLK_CTL_WW, 0x00000000);
	outpw(GPIO_OE_2_WH, 0x0000);
	outpw(GPIO_OE_3_WH, 0x0000);
	outpw(GPIO_OUT_2_WH, 0x0000);
	outpw(GPIO_OUT_3_WH, 0x0000);
	outpw(GPIO_INT_DETECT_CTL_2_WH, 0x0000);
	outpw(GPIO_INT_DETECT_CTL_3_WH, 0x0000);
	outpw(INT_POLARITY_3_WH, 0x0000);
	outpw(INT_POLARITY_4_WH, 0x0000);
	outpw(GPIO_INT_EN_2_WH, 0x0000);
	outpw(GPIO_INT_EN_3_WH, 0x0000);
	outpw(GPIO_INT_CLEAR_2_WH, 0x0000);
	outpw(GPIO_INT_CLEAR_3_WH, 0x0000);
	outp(GPIO2_PAGE_WB, 0x00);
	outp(GPIO2_CFG_WB, 0x00);
	outp(GPIO2_TEST_POINT_SEL_WB, 0x00);
	outpw(GPIO2_TEST_POINT_DIN_WH, 0x0000);
	outp(MMCC_CLK_CTL_WB, 0x00);
	outp(MMCC_CLKRATE_WB, 0x00);
	outp(MMCC_SPIREG_WB, 0x00);
	outp(MMCC_CMDDAT_CTL_WB, 0x00);
	outp(MMCC_RESPONSE_TO_WB, 0x00);
	outpw(MMCC_READ_TO_WH, 0x0000);
	outpw(MMCC_BLKLEN_WH, 0x0000);
	outpw(MMCC_NOB_WH, 0x0000);
	outp(MMCC_INTMASK_WB, 0x00);
	outp(MMCC_CMD_WB, 0x00);
	outpw(MMCC_ARGH_WH, 0x0000);
	outpw(MMCC_ARGL_WH, 0x0000);
	outpw(MMCC_FIFO_WH, 0x0000);
	outp(MMCC_BUF_PART_FULL_WB, 0x00);
	outp(MMCC_DEBUG_CTL_WB, 0x00);
	outp(MMCC_MEM_TEST_MODE_WB, 0x00);
	outp(MMCC_MUXSEL_WB, 0x00);
	outp(MMCC_INTF_ERROR_WB, 0x00);
	outp(MMCC_WAIT_WB, 0x00);
	outpw(USB_INT_MASK_WR_WH, 0x0000);
	outpw(USB_INT_CLEAR_WH, 0x0000);
	outpw(USB_INT_SOF_MASK_WH, 0x0000);
	outpw(USB_INT_SOF_COUNT_WH, 0x0000);
	outpw(USB_CORE_CONFIG_WH, 0x0000);
	outpw(USB_EP_STALL_WH, 0x0000);
	outpw(USB_EP_STALL_RESET_WH, 0x0000);
	outpw(USB_SETUP_ADDR_WH, 0x0000);
	outpw(USB_CONTROL_CMD_WH, 0x0000);
	outpw(USB_IN_FIFO_CMD_WH, 0x0000);
	outpw(USB_OUT_FIFO_CMD_WH, 0x0000);
	outpw(USB_DEVICE_CONTROL_WH, 0x0000);
	outpw(USB_OUTPUT_CTL_WH, 0x0000);
	outpw(USB_HDR_INT_MASK_WH, 0x0000);
	outpw(USB_HDR_INT_CLEAR_WH, 0x0000);
	outpw(USB_HDR_FL_XFER_WH, 0x0000);
	outpw(USB_HDR_FL_CONFIG_WH, 0x0000);
	outpw(USB_HDR_FL_CMD_WH, 0x0000);
	outpw(USB_HDR_RL_HUNT_WH, 0x0000);
	outpw(USB_HDR_RL_CONFIG_WH, 0x0000);
	outpw(USB_HDR_RL_CMD_WH, 0x0000);
	outpw(USB_TEST_MEM_SEL_WH, 0x0000);
	outp(USB_TEST_MEM_ADDR_WB, 0x00);
	outpw(USB_TEST_MEM_WRITE_WH, 0x0000);
	outpw(USB_TEST_MEM_DIN_WH, 0x0000);
	outpdw(GRP_INTR_CLEAR_WW, 0x00000000);
	outp(NAND_FLASH_CMD_WB, 0x00);
	outpdw(NAND_FLASH_ADDR_WW, 0x00000000);
	outpw(NAND_FLASH_CFG_WH, 0x0000);
}
