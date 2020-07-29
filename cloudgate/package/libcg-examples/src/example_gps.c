/* EXAMPLE:
 *  cg_gps
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <libcg/cg_gps.h>
#include <libcg/cg_general.h>


// GPS NMEA DATA Callback function
// Gps function calls return CG_STATUS_ERROR if primary device is Hpex, because it doesn't support gps yet.
void cg_gps_nmea_cb(const char *nmea_data, void *context)
{
	if (nmea_data) printf("nmea_data: %s", nmea_data);
}



int main(void)
{
	cg_status_t cg_status;
	int32_t level;
	cg_gps_t *cg_gps;


	cg_init("example_gps");


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
		default:                       		printf("GPS error\n"); break;
	}

	cg_status = cg_gps_set_reporting_interval(10);
	switch (cg_status) {
		case CG_STATUS_OK:                printf("GPS reporting interval set\n"); break;
		case CG_STATUS_INVALID_PARAMETER: printf("GPS parameter fault\n"); break;
		case CG_STATUS_ERROR:
		default:                       		printf("GPS error\n"); break;
	}

	cg_status = cg_gps_set_enabled(TRUE);
	switch (cg_status) {
		case CG_STATUS_OK:                printf("GPS is enabled\n"); break;
		case CG_STATUS_INVALID_PARAMETER: printf("GPS parameter fault\n"); break;
		case CG_STATUS_ERROR:
		default:                       		printf("GPS error\n"); break;
	}


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
		default:                       		printf("GPS error\n"); break;
	}


	cg_status = cg_gps_register_nmea_callback( cg_gps_nmea_cb,NULL );
	switch (cg_status) {
		case CG_STATUS_OK:                printf("GPS callback registered\n"); break;
		case CG_STATUS_INVALID_PARAMETER: printf("GPS parameter fault\n"); break;
		case CG_STATUS_ERROR:
		default:                       		printf("GPS error\n"); break;
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
		default:                       		printf("GPS error\n"); break;
	}

	cg_deinit();
	return 0;
}
