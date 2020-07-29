#ifndef OXDM_LOG_H
#define OXDM_LOG_H

#pragma pack(push,1)
typedef struct {
	uint8_t sector_id[16]; //128bit sector address af the serving sector
	uint8_t subnetmask; //sector subnet identifier
	uint8_t color_code; //sector's color code
	uint16_t pn_offset; //sector's PN offset
} log_packet_1080_t;

typedef struct {
	uint8_t numActive_set_pilots; //Number of pilots in the active set after the set transition
	uint16_t predicted_PN; //PN for the pilot predicted by the predictor; 0xFFFF indicates an invalid PN
	uint8_t predicted_DRC_cover_index; //DRC cover of the predicted pilot; valid only if PredictedPN is not equal to 0xFFFF
	uint8_t predicted_MAC_index; //RPC index associated with the predicted pilot; valid only if PredictedPN is not equal to 0xFFFF
	uint16_t predicted_PNSINR; //SINR of the predicted pilot; valid only if PredictedPN is not equal to 0xFFFF
	uint16_t serving_PN; //DRC cover of the serving pilot; 0xFFFF indicates an invalid PN
	uint8_t serving_DRC_cover_index; //DRC cover of the best serving pilot; valid only if ServingPN is not equal to 0xFFFF
	uint8_t serving_RPC_index; //MAC index associated with the serving pilot; valid only if ServingPN is not equal to 0xFFFF
	uint16_t serving_SINR; //SINR of the serving pilot; valid only if ServingPN is not equal to 0xFFFF
} log_packet_105f_t;

typedef struct {
	uint16_t channel;//Bit 0 to Bit 10 – Channel number,  Bit 11 to Bit 15 – Band class
	int8_t RxAGC0; //total receive power as seen by Antenna0, in dBm
	int8_t RxAGC1; //total receive power as seen by Antenna1, in dBm
	int8_t TxAGC; //transmit signal strength in the Connected state, in dBm (128 dBm indicates the value is not applicable)
	uint16_t ServingPN; //PN offset of the serving active set pilot
	uint16_t AsetPN[6]; // PN offsets for all pilots in the active set; 0xFFFF indicates an invalid value
	uint8_t SleepMode; /* 1xEV-DO Sleep mode; valid values are:
												0 – 426 ms sleep (SCC)
												1 – 5.12 sec sleep (CCC)
												2 – 40.96 sec sleep (long sleep)
												3 – Until reacquisition handoff
												4 – Release A enhanced idle protocol sleep (see Release A SCI field description) */
	uint8_t SCI; /* Bit 0 to Bit 2 – Release 0 slot cycle SCI
									This indicates the period for which the AT has gone to
									sleep; valid values are:
									 0 – 5.12 sec
									 1 – 10.24 sec
									 2 – 20.48 sec
									 3 – 40.96 sec
									 4 – Not applicable (possible if the AT is operating on Release A SCIs)
									Bit 3 to Bit 7 – Release A slot cycle SCI
									This indicates the Slot Cycle Index for which the AN has
									assigned the AT to sleep. This is supported for Release A
									only.
									For 0 ≤ relA_sci ≤ 6:
									Sleep Period = (2^relA_sci) x 4 x 80 / 48 ms
									For 7 ≤ relA_sci ≤ 15:
									Sleep Period = (2^(relA_sci-7)) x 4 x 80 / 48 ms */
	uint8_t SrchState; //Denotes the state of thesearcher task; see Table 5-7 for nomenclature
	uint8_t RxDiv; /* Receive diversity; valid values are:
										 0 – Single diversity (Rx0 only)
										 1 – Dual diversity */
	uint8_t ProtState; /* Summary of the AT protocol states; valid values are:
												 0 – Inactive (AT switched to 1X or is in Deep Sleep)
												 1 – Acquisition
												 2 – Sync
												 3 – Idle
												 4 – Access
												 5 – Connected
												All other values are reserved */
	uint8_t SessionState; /* Summary of the Session layer states; valid values are:
													  0 – Closed
													  1 – AMP setup
													  2 – AT-initiated negotiation
													  3 – Initiated negotiation
													  4 – Open
													 All other values are reserved */
	uint32_t UATIInfo; /* Bit 0 to Bit 23 – 24-bit UATI for the AT
                       Bit 24 to Bit 31 – Color code of the subnet on which AT is located */
	uint16_t StreamConfig[4]; /* Stream configuration (Rel0). Valid values for each stream are:
															  0x0000 – Default signaling application
															  0x0001 – Default packet application for AN
															  0x0002 – Default packet application for SN
															  0x0003 – Default test application
																0xFFFF – Stream not used
																0x0BAD – No information */
} log_packet_1183_t;

#pragma pack(pop)

#endif
