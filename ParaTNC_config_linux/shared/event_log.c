/*
 * This is a little bit stubbed implementation of event_log.h. It doesn't
 * implement init and any function using flash i/o subsystem on real target.
 *
 * event_log.c
 *
 *  Created on: Nov 6, 2024
 *      Author: mateusz
 */

#include "event_log.h"
#include "event_log_strings.h"
#include "time.h"	// PC specific
#include <stdio.h>

/// ==================================================================================================
///	LOCAL VARIABLES
/// ==================================================================================================

static char * event_log_string_unknown		= "UNKNOWN??\0";

/// ==================================================================================================
///	GLOBAL FUNCTIONS
/// ==================================================================================================

/**
 * Initializes everything log related
 */
void event_log_init (void) {
	; // stubbed. does nothing on purspose
}

/**
 * Stores new event asynchronously. Events are written into all volatile, RAM mapped areas
 * immediately, but FLASH based areas are synchronized periodically.
 * @param severity
 * @param source
 * @param wparam
 * @param lparam
 * @param lparam2
 */
int8_t event_log (event_log_severity_t severity,
				event_log_source_t source,
				uint8_t event_id,
				uint8_t param,
				uint8_t param2,
				uint16_t wparam,
				uint16_t wparam2,
				uint32_t lparam,
				uint32_t lparam2)
{
	// does nothing on purpose. performs flash i/o on real target
	return 0;
}

/**
 * Stores an event synchronously to all targer areas
 * @param severity
 * @param source
 * @param event_id
 * @param param
 * @param param2
 * @param wparam
 * @param wparam2
 * @param lparam
 * @param lparam2
 * @return
 */
int8_t event_log_sync (event_log_severity_t severity,
					 event_log_source_t source,
					 uint8_t event_id,
					 uint8_t param,
					 uint8_t param2,
					 uint16_t wparam,
					 uint16_t wparam2,
					 uint32_t lparam,
					 uint32_t lparam2)
{
	// does nothing on purpose. performs flash i/o on real target
	return 0;
}

const char * event_log_severity_to_str(event_log_severity_t severity)
{
	char * out;

	switch (severity)
	{
		case EVENT_DEBUG:		out = (char*)event_log_str_severity_debug; break;
		case EVENT_INFO:		out = (char*)event_log_str_severity_info; break;
		case EVENT_INFO_CYCLIC:	out = (char*)event_log_str_severity_info_cyclic; break;
		case EVENT_WARNING:		out = (char*)event_log_str_severity_warning; break;
		case EVENT_ERROR:		out = (char*)event_log_str_severity_error; break;
		case EVENT_ASSERT:		out = (char*)event_log_str_severity_assert; break;
		case EVENT_BOOTUP:		out = (char*)event_log_str_severity_bootup; break;
		case EVENT_TIMESYNC:	out = (char*)event_log_str_severity_timesync; break;
		default: 				out = (char*)event_log_string_unknown; break;
	}

	return out;
}

/**
 * Returns a pointer to a string representing event source
 * @param src
 * @return
 */
const char * event_log_source_to_str(event_log_source_t src)
{
	switch (src) {
		case EVENT_SRC_MAIN:				return event_log_str_src_main; break;
		case EVENT_SRC_WX_HANDLER:			return event_log_str_src_wx_handler; break;
		case EVENT_SRC_PWR_SAVE:			return event_log_str_src_pwr_save; break;
		case EVENT_SRC_PACKET_TX_HANDLER:	return event_log_str_src_packet_tx_handler; break;
		case EVENT_SRC_APRSIS:				return event_log_str_src_aprsis; break;
		case EVENT_SRC_KISS:				return event_log_str_src_kiss; break;
		case EVENT_SRC_APRS_RF:				return event_log_str_src_aprs_rf; break;
		case EVENT_SRC_GSM_GPRS:			return event_log_str_src_gsm_gprs; break;
		case EVENT_SRC_TCPIP:				return event_log_str_src_tcpip; break;
		case EVENT_SRC_HTTP_CLIENT:			return event_log_str_src_http_client; break;
		case EVENT_SRC_MODBUS:				return event_log_str_src_modbus; break;
		case EVENT_SRC_UMB:					return event_log_str_src_umb; break;
		case EVENT_SRC_DRV_ANEMOMETER:		return event_log_str_src_drv_anemometer; break;
		case EVENT_SRC_DRV_I2C:				return event_log_str_src_drv_i2c; break;
		case EVENT_SRC_DRV_UART:			return event_log_str_src_drv_uart; break;
		case EVENT_SRC_DRV_SPI:				return event_log_str_src_drv_spi; break;
		default: return event_log_default;
	}
}

/**
 *
 * @param source
 * @param event_id
 * @return
 */
const char * event_id_to_str(event_log_source_t source, uint8_t event_id)
{
	const char * out = event_log_default;

	switch (source) {
		case EVENT_SRC_MAIN: {

			if (event_id == EVENTS_MAIN_BOOTUP_COMPLETE) {
				out = event_log_str_main_bootup_complete;
			}
			else if (event_id == EVENTS_MAIN_TIMESYNC_BOOTUP) {
				out = event_log_str_main_timesync_bootup;
			}
			else if (event_id == EVENTS_MAIN_TIMESYNC_PERIODIC) {
				out = event_log_str_main_timesync_periodic;
			}
			else if (event_id == EVENTS_MAIN_TIMESYNC_NTP) {
				out = event_log_str_main_timesync_ntp;
			}
			else if (event_id == EVENTS_MAIN_CYCLIC) {
				out = event_log_str_main_info_cyclic;
			}
			break;
		}
		case EVENT_SRC_WX_HANDLER:	{

			if (event_id == EVENTS_WX_HANDLER_WARN_TEMPERATURE_INT_FAILED) {
				out = event_log_str_wx_handler_temperature_int_failed;
			}
			else if (event_id == EVENTS_WX_HANDLER_WARN_TEMPERATURE_DALLAS_DEGR) {
				out = event_log_str_wx_handler_temperature_dallas_degraded;
			}
			else if (event_id == EVENTS_WX_HANDLER_WARN_TEMPERATURE_DALLAS_NAV) {
				out = event_log_str_wx_handler_temperature_dallas_not_avble;
			}
			else if (event_id == EVENTS_WX_HANDLER_WARN_TEMPERATURE_EXCESIVE_SLEW) {
				out = event_log_str_wx_handler_temperature_excesive_slew;
			}
			else if (event_id == EVENTS_WX_HANDLER_WARN_PRESSURE_FAILED) {
				out = event_log_str_wx_handler_temperature_pressure_fail;
			}
			else if (event_id == EVENTS_WX_HANDLER_WARN_HUMIDITY_FAILED) {
				out = event_log_str_wx_handler_temperature_humidity_fail;
			}
			///////
			///
			else if (event_id == EVENTS_WX_HANDLER_ERROR_RTE_CHECK_ANEM_TIMER_HAS_BEEN_FIRED) {
				out = event_log_str_wx_handler_error_rte_check_anem_timer_has_been_fired;
			}
			else if (event_id == EVENTS_WX_HANDLER_ERROR_RTE_CHECK_SLEW_LIMIT) {
				out = event_log_str_wx_handler_error_rte_check_slew_limit;
			}
			else if (event_id == EVENTS_WX_HANDLER_ERROR_RTE_CHECK_DEBOUCING) {
				out = event_log_str_wx_handler_error_rte_check_debouncing;
			}
			else if (event_id == EVENTS_WX_HANDLER_ERROR_RTE_CHECK_UF_CONVERTER_FAIL) {
				out = event_log_str_wx_handler_error_rte_check_uf_converter_fail;
			}
			else if (event_id == EVENTS_WX_HANDLER_ERROR_RTE_CHECK_WINDSPEED_BUFFERS) {
				out = event_log_str_wx_handler_error_rte_check_windspeed_buffers;
			}

			break;
		}
		case EVENT_SRC_PWR_SAVE:
			if (event_id == EVENTS_PWR_SAVE_BATT_LOW_GOING_SLEEP) {
				out = event_log_str_pwr_save_going_sleep;
			}
			break;
		case EVENT_SRC_PACKET_TX_HANDLER:	 break;
		case EVENT_SRC_APRSIS: {
			if (event_id == EVENTS_APRSIS_ERROR_IM_NOT_OK_LAST_KEEPALIVE) {
				out = event_log_str_aprsis_im_not_ok_last_keepalive;
			}
			else if (event_id == EVENTS_APRSIS_ERROR_IM_NOT_OK_LAST_TRANSMIT_LONG) {
				out = event_log_str_aprsis_im_not_ok_last_transmit;
			}
			else if (event_id == EVENTS_APRSIS_WARN_AUTH_FAILED) {
				out = event_log_str_aprsis_warn_auth_failed;
			}
			else if (event_id == EVENTS_APRSIS_WARN_TIMEOUT_WAITING_AUTH) {
				out = event_log_str_aprsis_warn_timeout_waiting_auth;
			}
			else if (event_id == EVENTS_APRSIS_WARN_NO_HELLO_MESSAGE) {
				out = event_log_str_aprsis_warn_no_hello_message;
			}
			else if (event_id == EVENTS_APRSIS_WARN_TIMEOUT_WAITING_HELLO_MSG) {
				out = event_log_str_aprsis_warn_timeout_waiting_hello_msg;
			}
			else if (event_id == EVENTS_APRSIS_WARN_CONNECT_FAILED) {
				out = event_log_str_aprsis_warn_connect_failed;
			}
			else if (event_id == EVENTS_APRSIS_WARN_WRONG_STATE) {
				out = event_log_str_aprsis_warn_wrong_state;
			}
			else if (event_id == EVENTS_APRSIS_WARN_DEAD_KEEPALIVE) {
				out = event_log_str_aprsis_warn_dead_keepalive;
			}
			else if (event_id == EVENTS_APRSIS_WARN_DEAD_TRANSMIT) {
				out = event_log_str_aprsis_warn_dead_transmit;
			}
			break;
		}
		case EVENT_SRC_KISS:				 break;
		case EVENT_SRC_APRS_RF:				 break;
		case EVENT_SRC_GSM_GPRS:
			if (event_id == EVENTS_GSM_GPRS_ERR_APN_CONFIGURATION_MISSING) {
				out = event_log_str_tcpip_error_apn_config_missing;
			}
			else if (event_id == EVENTS_GSM_GPRS_WARN_ASYNC_MSG_DETECTED) {
				out = event_log_str_tcpip_warn_async_msg_detected;
			}
			else if (event_id == EVENTS_GSM_GPRS_ERR_SIM_CARD_STATUS) {
				out = event_log_str_tcpip_error_sim_card_status;
			}
			else if (event_id == EVENTS_GSM_GPRS_WARN_NOT_REGISTERED_TO_NETWORK) {
				out = event_log_str_tcpip_warn_not_registered_to_nework;
			}
			else if (event_id == EVENTS_GSM_GPRS_REGISTERED_NETWORK) {
				out = event_log_str_tcpip_bootup_registered_network;
			}
			else if (event_id == EVENTS_GSM_GPRS_SIGNAL_LEVEL) {
				out = event_log_str_tcpip_bootup_signal_level;
			}
			else if (event_id == EVENTS_GSM_GPRS_IMSI) {
				out = event_log_str_tcpip_bootup_imsi;
			}
			else if (event_id == EVENTS_GSM_GPRS_IP_ADDRESS) {
				out = event_log_str_tcpip_bootup_ip_address;
			}

			break;
		case EVENT_SRC_TCPIP:
			if (event_id == EVENTS_TCPIP_ERR_CONNECTING) {
				out = event_log_str_tcpip_error_connecting;
			}
			else if (event_id == EVENTS_TCPIP_ERR_CONNECTING_NO_MODEM_RESPONSE) {
				out = event_log_str_tcpip_error_connecting_no_modem_resp;
			}

			break;
		case EVENT_SRC_HTTP_CLIENT:			 break;
		case EVENT_SRC_MODBUS:				 break;
		case EVENT_SRC_UMB:		{
			if (event_id == EVENTS_UMB_WARN_CRC_FAILED_IN_RECEIVED_FRAME) {
				out = event_log_str_umb_warn_crc_failed_in_received_frame;
			}
			else if (event_id == EVENTS_UMB_WARN_RECEIVED_FRAME_MALFORMED) {
				out = event_log_str_umb_warn_received_frame_malformed;
			}
			else if (event_id == EVENTS_UMB_WARN_NOK_STATUS_IN_GET_STATUS_RESP) {
				out = event_log_str_umb_warn_nok_sensor_status_in_get_status_data;
			}
			else if (event_id == EVENTS_UMB_WARN_NOK_STATUS_IN_OFFLINE_DATA_RESP) {
				out = event_log_str_umb_warn_nok_sensor_status_in_offline_data;
			}
			else if (event_id == EVENTS_UMB_ERROR_RECEIVING) {
				out = event_log_str_umb_error_receiving;
			}
			else if (event_id == EVENTS_UMB_ERROR_UNEXPECTED_ROUTINE_ID) {
				out = event_log_str_umb_error_unexp_routine_id;
			}
			else if (event_id == EVENTS_UMB_ERROR_QF_NOT_AVAILABLE) {
				out = event_log_str_umb_error_quality_factor_not_avail;
			}

			break;
		}
		case EVENT_SRC_DRV_ANEMOMETER: {

			if (event_id == EVENTS_DRV_ANEMOMETER_WARN_NO_PULSES_INT_FIRED) {
				out = event_log_str_drv_anemometer_no_pulses_int_fired;
			}
			else if (event_id == EVENTS_DRV_ANEMOMETER_WARN_EXCESIVE_SLEW) {
				out = event_log_str_drv_anemometer_excesive_slew_rate;
			}
			else if (event_id == EVENTS_DRV_ANEMOMETER_ERROR_UF_CONV_NOT_WORKING) {
				out = event_log_str_drv_anemometer_uf_conv_not_working;
			}
			else if (event_id == EVENTS_DRV_ANEMOMETER_ERROR_UF_FREQ_TOO_HI) {
				out = event_log_str_drv_anemometer_uf_freq_to_hi;
			}
			else if (event_id == EVENTS_DRV_ANEMOMETER_WARN_QF_NOT_FULL) {
				out = event_log_str_drv_anemometer_qf_not_full;
			}

			break;
		}
		case EVENT_SRC_DRV_I2C:				 break;
		case EVENT_SRC_DRV_UART:			 break;
		case EVENT_SRC_DRV_SPI:				 break;
		default: out = event_log_default;
	}

	return out;
}

/**
 * Generates string representation of given event log in exposed form
 * @param exposed pointer to an event to be converted
 * @param output char buffer to place a string into
 * @param output_ln maximum length of output string
 * @return length of assembled string
 */
uint16_t event_exposed_to_string(const event_log_exposed_t * exposed, char * output, uint16_t output_ln)
{
	uint16_t out = 0u;

	struct tm time_from_exposed = {0u};

#ifdef __linux__
	uint16_t minutes_from_midnight = exposed->event_rtc & 0x7FFu;
	uint16_t days_from_new_year = (uint16_t)((uint32_t)(exposed->event_rtc >> 16) & 0x1FFu);
	uint8_t years = (uint8_t)((exposed->event_rtc >> 25) & 0x7Fu);

	time_from_exposed.tm_hour = minutes_from_midnight / 60;
	time_from_exposed.tm_min = minutes_from_midnight - (time_from_exposed.tm_hour * 60);
	time_from_exposed.tm_sec = 0;

	time_from_exposed.tm_year = years + 100;

	struct tm tempo = {0u};			// temporal struct
	char temp_string[9] = {0u};		// temporal string

	snprintf(temp_string, 9, "%d", days_from_new_year);
	strptime( temp_string, "%j", &tempo );
	time_from_exposed.tm_mon = tempo.tm_mon;
	time_from_exposed.tm_mday = tempo.tm_mday;
	time_from_exposed.tm_wday = tempo.tm_wday;


#else
	// assume windows here
#endif

	const char * severity = event_log_severity_to_str(exposed->severity);

	if (exposed->severity == EVENT_TIMESYNC) {
		out = snprintf(output, output_ln,
						"[%s][Log Counter:%u][Seconds since powerup:%u][RTC_TIMESYNC][DATE:%d/%d/%d][TIME:%d:%d:%d]",
						severity,
						exposed->event_counter_id,
						exposed->event_master_time,
						(uint16_t)exposed->param2,
						(uint16_t)exposed->param,
						(uint16_t)exposed->wparam,
						(uint16_t)exposed->wparam2,
						(uint16_t)exposed->lparam,
						(uint16_t)exposed->lparam2);
	}
	else {
		// if pointer to event string is set
		if (exposed->event_str_name != NULL) {
			out = snprintf(output, output_ln,
							"[%s][Log Counter:%u][Seconds since powerup:%u][Source: %s][Event: %s][param: 0x%X, param2: 0x%X, wparam: 0x%X, wparam2: 0x%X, lparam: 0x%X, lparam2: 0x%X]",
							severity,
							exposed->event_counter_id,
							exposed->event_master_time,
							exposed->source_str_name,
							exposed->event_str_name,
							exposed->param,
							exposed->param2,
							exposed->wparam,
							exposed->wparam2,
							exposed->lparam,
							exposed->lparam2);
		}
		else {
			out = snprintf(output, output_ln,
							"[%s][Log Counter:%u][Seconds since powerup:%u][Source: %s][Unknown event: 0x%X][param: 0x%X, param2: 0x%X, wparam: 0x%X, wparam2: 0x%X, lparam: 0x%X, lparam2: 0x%X]",
							severity,
							exposed->event_counter_id,
							exposed->event_master_time,
							exposed->source_str_name,
							exposed->event_id,
							exposed->param,
							exposed->param2,
							exposed->wparam,
							exposed->wparam2,
							exposed->lparam,
							exposed->lparam2);
		}
	}


	return out;
}

