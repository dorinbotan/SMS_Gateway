/* EXAMPLE:
 *  cg_gps
 */

/* Important notes on the support of AGPS with a secure SUPL server
 *
 * In general, at the time of writing, AGPS is only supported with the
 * Gobi3k 3G modem, found in CG0112.
 *
 * This code shows the use of Google's secure SUPL server as an example.
 * Unfortunately Gobi3k is no longer able to setup a connection to Google's
 * secure SUPL server since Google started using a server certificate that uses
 * a SHA256RSA signature algorithm.
 * Even though this example doesn't work we decided to keep it. It still serves
 * as guidance of what needs to be programmed to work with a secure SUPL AGPS
 * server.
 *
 * */


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <libcg/cg_gps.h>
#include <libcg/cg_general.h>

/* Geotrust : Equifax_Secure_Certificate_Authority
	from http://www.geotrust.com/resources/root-certificates/
	in DER format
*/
const uint8_t root_certificate[] = {
        0x30, 0x82, 0x03, 0x20, 0x30, 0x82, 0x02, 0x89,   0xA0, 0x03, 0x02, 0x01, 0x02, 0x02, 0x04, 0x35,
        0xDE, 0xF4, 0xCF, 0x30, 0x0D, 0x06, 0x09, 0x2A,   0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x05,
        0x05, 0x00, 0x30, 0x4E, 0x31, 0x0B, 0x30, 0x09,   0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x55,
        0x53, 0x31, 0x10, 0x30, 0x0E, 0x06, 0x03, 0x55,   0x04, 0x0A, 0x13, 0x07, 0x45, 0x71, 0x75, 0x69,
        0x66, 0x61, 0x78, 0x31, 0x2D, 0x30, 0x2B, 0x06,   0x03, 0x55, 0x04, 0x0B, 0x13, 0x24, 0x45, 0x71,
        0x75, 0x69, 0x66, 0x61, 0x78, 0x20, 0x53, 0x65,   0x63, 0x75, 0x72, 0x65, 0x20, 0x43, 0x65, 0x72,
        0x74, 0x69, 0x66, 0x69, 0x63, 0x61, 0x74, 0x65,   0x20, 0x41, 0x75, 0x74, 0x68, 0x6F, 0x72, 0x69,
        0x74, 0x79, 0x30, 0x1E, 0x17, 0x0D, 0x39, 0x38,   0x30, 0x38, 0x32, 0x32, 0x31, 0x36, 0x34, 0x31,
        0x35, 0x31, 0x5A, 0x17, 0x0D, 0x31, 0x38, 0x30,   0x38, 0x32, 0x32, 0x31, 0x36, 0x34, 0x31, 0x35,
        0x31, 0x5A, 0x30, 0x4E, 0x31, 0x0B, 0x30, 0x09,   0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x55,
        0x53, 0x31, 0x10, 0x30, 0x0E, 0x06, 0x03, 0x55,   0x04, 0x0A, 0x13, 0x07, 0x45, 0x71, 0x75, 0x69,
        0x66, 0x61, 0x78, 0x31, 0x2D, 0x30, 0x2B, 0x06,   0x03, 0x55, 0x04, 0x0B, 0x13, 0x24, 0x45, 0x71,
        0x75, 0x69, 0x66, 0x61, 0x78, 0x20, 0x53, 0x65,   0x63, 0x75, 0x72, 0x65, 0x20, 0x43, 0x65, 0x72,
        0x74, 0x69, 0x66, 0x69, 0x63, 0x61, 0x74, 0x65,   0x20, 0x41, 0x75, 0x74, 0x68, 0x6F, 0x72, 0x69,
        0x74, 0x79, 0x30, 0x81, 0x9F, 0x30, 0x0D, 0x06,   0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01,
        0x01, 0x01, 0x05, 0x00, 0x03, 0x81, 0x8D, 0x00,   0x30, 0x81, 0x89, 0x02, 0x81, 0x81, 0x00, 0xC1,
        0x5D, 0xB1, 0x58, 0x67, 0x08, 0x62, 0xEE, 0xA0,   0x9A, 0x2D, 0x1F, 0x08, 0x6D, 0x91, 0x14, 0x68,
        0x98, 0x0A, 0x1E, 0xFE, 0xDA, 0x04, 0x6F, 0x13,   0x84, 0x62, 0x21, 0xC3, 0xD1, 0x7C, 0xCE, 0x9F,
        0x05, 0xE0, 0xB8, 0x01, 0xF0, 0x4E, 0x34, 0xEC,   0xE2, 0x8A, 0x95, 0x04, 0x64, 0xAC, 0xF1, 0x6B,
        0x53, 0x5F, 0x05, 0xB3, 0xCB, 0x67, 0x80, 0xBF,   0x42, 0x02, 0x8E, 0xFE, 0xDD, 0x01, 0x09, 0xEC,
        0xE1, 0x00, 0x14, 0x4F, 0xFC, 0xFB, 0xF0, 0x0C,   0xDD, 0x43, 0xBA, 0x5B, 0x2B, 0xE1, 0x1F, 0x80,
        0x70, 0x99, 0x15, 0x57, 0x93, 0x16, 0xF1, 0x0F,   0x97, 0x6A, 0xB7, 0xC2, 0x68, 0x23, 0x1C, 0xCC,
        0x4D, 0x59, 0x30, 0xAC, 0x51, 0x1E, 0x3B, 0xAF,   0x2B, 0xD6, 0xEE, 0x63, 0x45, 0x7B, 0xC5, 0xD9,
        0x5F, 0x50, 0xD2, 0xE3, 0x50, 0x0F, 0x3A, 0x88,   0xE7, 0xBF, 0x14, 0xFD, 0xE0, 0xC7, 0xB9, 0x02,
        0x03, 0x01, 0x00, 0x01, 0xA3, 0x82, 0x01, 0x09,   0x30, 0x82, 0x01, 0x05, 0x30, 0x70, 0x06, 0x03,
        0x55, 0x1D, 0x1F, 0x04, 0x69, 0x30, 0x67, 0x30,   0x65, 0xA0, 0x63, 0xA0, 0x61, 0xA4, 0x5F, 0x30,
        0x5D, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55,   0x04, 0x06, 0x13, 0x02, 0x55, 0x53, 0x31, 0x10,
        0x30, 0x0E, 0x06, 0x03, 0x55, 0x04, 0x0A, 0x13,   0x07, 0x45, 0x71, 0x75, 0x69, 0x66, 0x61, 0x78,
        0x31, 0x2D, 0x30, 0x2B, 0x06, 0x03, 0x55, 0x04,   0x0B, 0x13, 0x24, 0x45, 0x71, 0x75, 0x69, 0x66,
        0x61, 0x78, 0x20, 0x53, 0x65, 0x63, 0x75, 0x72,   0x65, 0x20, 0x43, 0x65, 0x72, 0x74, 0x69, 0x66,
        0x69, 0x63, 0x61, 0x74, 0x65, 0x20, 0x41, 0x75,   0x74, 0x68, 0x6F, 0x72, 0x69, 0x74, 0x79, 0x31,
        0x0D, 0x30, 0x0B, 0x06, 0x03, 0x55, 0x04, 0x03,   0x13, 0x04, 0x43, 0x52, 0x4C, 0x31, 0x30, 0x1A,
        0x06, 0x03, 0x55, 0x1D, 0x10, 0x04, 0x13, 0x30,   0x11, 0x81, 0x0F, 0x32, 0x30, 0x31, 0x38, 0x30,
        0x38, 0x32, 0x32, 0x31, 0x36, 0x34, 0x31, 0x35,   0x31, 0x5A, 0x30, 0x0B, 0x06, 0x03, 0x55, 0x1D,
        0x0F, 0x04, 0x04, 0x03, 0x02, 0x01, 0x06, 0x30,   0x1F, 0x06, 0x03, 0x55, 0x1D, 0x23, 0x04, 0x18,
        0x30, 0x16, 0x80, 0x14, 0x48, 0xE6, 0x68, 0xF9,   0x2B, 0xD2, 0xB2, 0x95, 0xD7, 0x47, 0xD8, 0x23,
        0x20, 0x10, 0x4F, 0x33, 0x98, 0x90, 0x9F, 0xD4,   0x30, 0x1D, 0x06, 0x03, 0x55, 0x1D, 0x0E, 0x04,
        0x16, 0x04, 0x14, 0x48, 0xE6, 0x68, 0xF9, 0x2B,   0xD2, 0xB2, 0x95, 0xD7, 0x47, 0xD8, 0x23, 0x20,
        0x10, 0x4F, 0x33, 0x98, 0x90, 0x9F, 0xD4, 0x30,   0x0C, 0x06, 0x03, 0x55, 0x1D, 0x13, 0x04, 0x05,
        0x30, 0x03, 0x01, 0x01, 0xFF, 0x30, 0x1A, 0x06,   0x09, 0x2A, 0x86, 0x48, 0x86, 0xF6, 0x7D, 0x07,
        0x41, 0x00, 0x04, 0x0D, 0x30, 0x0B, 0x1B, 0x05,   0x56, 0x33, 0x2E, 0x30, 0x63, 0x03, 0x02, 0x06,
        0xC0, 0x30, 0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48,   0x86, 0xF7, 0x0D, 0x01, 0x01, 0x05, 0x05, 0x00,
        0x03, 0x81, 0x81, 0x00, 0x58, 0xCE, 0x29, 0xEA,   0xFC, 0xF7, 0xDE, 0xB5, 0xCE, 0x02, 0xB9, 0x17,
        0xB5, 0x85, 0xD1, 0xB9, 0xE3, 0xE0, 0x95, 0xCC,   0x25, 0x31, 0x0D, 0x00, 0xA6, 0x92, 0x6E, 0x7F,
        0xB6, 0x92, 0x63, 0x9E, 0x50, 0x95, 0xD1, 0x9A,   0x6F, 0xE4, 0x11, 0xDE, 0x63, 0x85, 0x6E, 0x98,
        0xEE, 0xA8, 0xFF, 0x5A, 0xC8, 0xD3, 0x55, 0xB2,   0x66, 0x71, 0x57, 0xDE, 0xC0, 0x21, 0xEB, 0x3D,
        0x2A, 0xA7, 0x23, 0x49, 0x01, 0x04, 0x86, 0x42,   0x7B, 0xFC, 0xEE, 0x7F, 0xA2, 0x16, 0x52, 0xB5,
        0x67, 0x67, 0xD3, 0x40, 0xDB, 0x3B, 0x26, 0x58,   0xB2, 0x28, 0x77, 0x3D, 0xAE, 0x14, 0x77, 0x61,
        0xD6, 0xFA, 0x2A, 0x66, 0x27, 0xA0, 0x0D, 0xFA,   0xA7, 0x73, 0x5C, 0xEA, 0x70, 0xF1, 0x94, 0x21,
        0x65, 0x44, 0x5F, 0xFA, 0xFC, 0xEF, 0x29, 0x68,   0xA9, 0xA2, 0x87, 0x79, 0xEF, 0x79, 0xEF, 0x4F,
        0xAC, 0x07, 0x77, 0x38,
};


// GPS NMEA DATA Callback function
void cg_gps_nmea_cb(const char *nmea_data, void *context)
{
	if (nmea_data) printf("nmea_data: %s", nmea_data);
}

// GPS SUPL SERVER connection status callback
void cg_gps_assisted_cb(cg_gps_assisted_connection_status_t status)
{
	switch (status) {
		case CG_GPS_ASSISTED_CONNECTION_SUCCESS: printf("AGPS SUPL server connection passed\n"); break;
		case CG_GPS_ASSISTED_CONNECTION_FAILURE: printf("AGPS SUPL server connection failure\n"); break;
	}
}

int main(void)
{
	cg_status_t cg_status;
	int32_t level;
	cg_gps_t *cg_gps;

	cg_init("example_agps_secure_link");

	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	cg_status = cg_gps_get_status(&cg_gps);
	switch (cg_status) {
		case CG_STATUS_OK:
			printf("GPS status received\n");
			if (cg_gps) {
				printf("GPS status.reporting interval %d\n", cg_gps->reporting_interval);
				printf("GPS status.status %s%s\n",
					(cg_gps->status & CG_GPS_PRESENT ? "PRESENT,":""),
					(cg_gps->status & CG_GPS_ENABLED ? "ENABLED,":"") );
				free(cg_gps);
			}
			break;
		case CG_STATUS_INVALID_PARAMETER: printf("GPS parameter fault\n"); break;
		case CG_STATUS_ERROR:
		default:                       		printf("GPS error cg_gps_get_status\n"); break;
	}

	cg_status = cg_gps_set_reporting_interval(30);
	switch (cg_status) {
		case CG_STATUS_OK:                printf("GPS reporting interval set\n"); break;
		case CG_STATUS_INVALID_PARAMETER: printf("GPS parameter fault\n"); break;
		case CG_STATUS_ERROR:
		default:                       		printf("GPS error cg_gps_set_reporting_interval\n"); break;
	}

	cg_gps_certificate_t certificate;
	certificate.len = sizeof(root_certificate);
	memcpy(certificate.cert, root_certificate, certificate.len);

	cg_status = cg_gps_set_assisted(CG_GPS_ASSISTED_SECURE_SUPL, "supl.google.com:7275", 1, &certificate);
	switch (cg_status) {
		case CG_STATUS_OK:                printf("AGPS is enabled\n"); break;
		case CG_STATUS_INVALID_PARAMETER: printf("AGPS parameter fault\n"); break;
		case CG_STATUS_ERROR:
		default:                       		printf("AGPS error\n"); break;
	}

	cg_status = cg_gps_set_enabled(TRUE);
	switch (cg_status) {
		case CG_STATUS_OK:                printf("GPS is enabled\n"); break;
		case CG_STATUS_INVALID_PARAMETER: printf("GPS parameter fault\n"); break;
		case CG_STATUS_ERROR:
		default:                       		printf("GPS error\n"); break;
	}

	cg_status = cg_gps_register_nmea_callback( cg_gps_nmea_cb,NULL );
	switch (cg_status) {
		case CG_STATUS_OK:                printf("GPS callback registered\n"); break;
		case CG_STATUS_INVALID_PARAMETER: printf("GPS parameter fault\n"); break;
		case CG_STATUS_ERROR:
		default:                       		printf("GPS error\n"); break;
	}

	cg_status = cg_gps_register_supl_connection_status_callback(cg_gps_assisted_cb);
	switch (cg_status) {
		case CG_STATUS_OK:                printf("AGPS callback registered\n"); break;
		case CG_STATUS_INVALID_PARAMETER: printf("AGPS parameter fault\n"); break;
		case CG_STATUS_ERROR:
		default:                       		printf("AGPS error\n"); break;
	}

	printf ("Press any key to exit\n");
	getchar();


	cg_status = cg_gps_set_enabled(FALSE);
	switch (cg_status) {
		case CG_STATUS_OK:                printf("GPS is disabled (for this client)\n"); break;
		case CG_STATUS_INVALID_PARAMETER: printf("GPS parameter fault\n"); break;
		case CG_STATUS_ERROR:
		default:                       		printf("GPS error\n"); break;
	}


	cg_status = cg_gps_deregister_nmea_callback( cg_gps_nmea_cb,NULL );
	switch (cg_status) {
		case CG_STATUS_OK:                printf("GPS callback de-registered\n"); break;
		case CG_STATUS_INVALID_PARAMETER: printf("GPS parameter fault\n"); break;
		case CG_STATUS_ERROR:
		default:                       		printf("GPS error\n"); break;
	}

	cg_status = cg_gps_deregister_supl_connection_status_callback();
	switch (cg_status) {
		case CG_STATUS_OK:                printf("AGPS callback de-registered\n"); break;
		case CG_STATUS_INVALID_PARAMETER: printf("AGPS parameter fault\n"); break;
		case CG_STATUS_ERROR:
		default:                       		printf("AGPS error\n"); break;
	}

	cg_deinit();
	return 0;
}
