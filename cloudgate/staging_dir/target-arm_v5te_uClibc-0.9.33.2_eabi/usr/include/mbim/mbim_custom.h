#ifndef MBIM_CUSTOM_H
#define MBIM_CUSTOM_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <mbim/mbim_event.h>
#include <mbim/mbim.h>

/**
 * \file mbim_custom.h
 * \brief Public header file which contains all command id functionality for the
 * custom commands.
 */

/**
 * \defgroup auth_services custom functions (PUBLIC)
 * @{
 */

/* TELIT CUSTOM COMMANDS */

/*
	MBIM_CID_THERMAL_STATE as defined on:
	Telit_xN930_MBIM_Command_Functional_Specification_r1.pdf on p17
*/

/*
 * \struct mbim_thermal_threshold_setting_t
 * \brief thermal threshold settings.
 */
typedef struct {
	/* 0 to disable / unarm the threshold. When disabled, notifications are disabled */
	uint32_t enable;
	/* Temperature sensor id. MUST be 0 for the WWAN module */
	uint32_t temperature_sensor_id;
	/* One based index of the temperature threshold to be programmed. Typically DPTF
	will use indices 1 and 2 (for two programmable thresholds) */
	uint32_t alarm_id;
	/* Indicates the threshold temperature value to be compared against the
	current temperature value to generate notifications. The unit of
	this value is in 1/10th of Celsius Degree. */
	uint32_t threshold_value;
	/* Indicates the hysteresis that needs to be used for generating cross interrupt.
	In 1/10th of Celsius Degrees. */
	uint32_t hysteresis_value;
	/* Indicates the time interval that the firmware will use to monitor
	the thermal sensor when the modem is active. */
	uint32_t sampling_period;
} mbim_thermal_threshold_setting_t;

/*
 * \struct mbim_query_thermal_state_t
 * \brief struct used in the informationbuffer when sending a query.
 */
typedef struct {
	/* Temperature sensor id. MUST be 0 for the WWAN module */
	uint32_t temperature_sensor_id;
} mbim_query_thermal_state_t;

/*
 * \struct mbim_thermal_state_info_t
 * \brief response when doing a query or when a notification comes in.
 */
typedef struct {
	/* The current value of the temperature measured by the device. In 1/10th of Celcius Degrees */
	uint32_t current_temperature_value;
	/* The specific threshold that was crossed. When this structure is returned in response
		to a query, this structure SHALL BE ignored. */
	mbim_thermal_threshold_setting_t threshold;
	/* Is the threshold value valid? If 1, then the threshold is valid, otherwise it can
		be ignored.*/
	uint32_t threshold_valid;
} mbim_thermal_state_info_t;

/*
 * \struct mbim_thermal_config_t
 * \brief Struct used to set the threshold values for the temperature sensor.
 * You can set maximum 4 thresholds at a time. For example, the upper and
 * lower temperatures.
 */
typedef struct {
	mbim_thermal_threshold_setting_t threshold[4];
} mbim_thermal_config_t;

/*
 * \struct mbim_thermal_state_info_response_t
 * \brief Structure which is given back to the user data callback when doing a query or when
 * receiving a notification. The threshold is only valid when is_notification is set to 1.
 */

/**
 * Query the thermal state of the Telit wwan device.
 * The response will be an mbim_thermal_state_info_t struct.
 * \param[in] ctx The mbim device context.
 * \param[in] cb User data callback
 * \param[in] arg Extra argument for the user callback
 * \return MBIM_OK, MBIM_ERROR, MBIM_INVALID_PARAMETER.
 */
uint32_t mbim_get_telit_thermal_state(mbim_context_t *ctx, mbim_user_data_cb cb, void *arg);

/**
 * Query the thermal state of the device. This is a generic function.
 * The response will be an mbim_thermal_state_info_t struct.
 * \param[in] ctx The mbim device context.
 * \param[in] cb User data callback
 * \param[in] sensor id The sensor id to get the temperature from.
 * \param[in] arg Extra argument for the user callback
 * \return MBIM_OK, MBIM_ERROR, MBIM_INVALID_PARAMETER.
 */
uint32_t mbim_get_thermal_state(mbim_context_t *ctx, mbim_user_data_cb cb, uint32_t sensor_id, void *arg);

/**
 * Set the thermal state of the device.
 * The response will be an mbim_thermal_state_info_t struct.
 * \param[in] ctx The mbim device context.
 * \param[in] cb User data callback
 * \param[in] config Pointer to mbim_thermal_config_t struct.
 * \param[in] arg Extra argument for the user callback
 * \return MBIM_OK, MBIM_ERROR, MBIM_INVALID_PARAMETER.
 */
uint32_t mbim_set_thermal_state(mbim_context_t *ctx, mbim_user_data_cb cb, mbim_thermal_config_t *config, void *arg);

/**
 * Helper function to set one threshold in the mbim_thermal_config_t..
 * The response will be an mbim_thermal_state_info_t struct.
 * \param[in] ctx The mbim device context.
 * \param[in] cb User data callback
 * \param[in] config Pointer to mbim_thermal_config_t struct.
 * \param[in] arg Extra argument for the user callback
 * \return MBIM_OK, MBIM_ERROR, MBIM_INVALID_PARAMETER.
 */
uint32_t mbim_set_single_thermal_threshold(mbim_thermal_config_t *config, uint32_t index,
	uint32_t enable, uint32_t temperature_sensor_id, uint32_t alarm_id, uint32_t threshold_value,
		uint32_t hysteresis_value, uint32_t sampling_period);

/* TELIT END */


/** @} */

#endif
