#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <libcg/cg_general.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

/*
 * Example_timezone demonstrates setting and retrieving timezone
 * Time can be set and retrieved in local or UTC time
 * Provide internet connectivity in order to have a successful Ntp time syncronisation.
 */

int max_retry=3;
// Stop sleeping if exit_program is 1
int exit_program=0;
// Exit code program
int exit_code = 0;

typedef struct {
	/* number of time zones in table */
	uint32_t number_of_timezones;
	/* time zone table contains number_of_timezone entries */
	cg_timezone_t timezones[1];
} timezone_table_t;

// just an arbitrary context
static void *context = (void *)0x132;

int get_timezone_table(timezone_table_t **tz) {
	int number_of_timezones=0;
	int n;
	cg_timezone_t *timezones;
	timezone_table_t *mytz=NULL;
	char line[256];
	char *l;
	if (tz == NULL)
	{
		printf("Invalid parameter");
		return -1;
	}
	*tz = NULL;
	FILE *f = fopen("/etc/timezones","r");
	if(f) {
		*tz = malloc(sizeof(timezone_table_t));
		(*tz)->number_of_timezones = 0;
		timezones =&(*tz)->timezones[0];
		while(1) {
			if ( (l=fgets (line , 255 , f)) == NULL ) {
				break;
			}
			if(l) {
				n=sscanf(l,"UTC%03d:%02d,%04d,%3d,%3d,%2d,%2d,%3d,%2d,%2d, \"%[^\"]\"\n",
				&timezones->utc_offset_h, &timezones->utc_offset_m, &timezones->utc_offset_h, &timezones->utc_offset_m,
				&timezones->dst_start_m, &timezones->dst_start_n, &timezones->dst_start_d,
				&timezones->dst_end_m, &timezones->dst_end_n, &timezones->dst_end_d,
				&timezones->desc[0]);

				if(n==11) {
					(*tz)->number_of_timezones = ++number_of_timezones;

					mytz=realloc(*tz,sizeof(timezone_table_t)+ number_of_timezones * sizeof(cg_timezone_t));
					if(!mytz) {
						free (*tz);
						*tz=NULL;
						printf("Allocation error");
                        fclose(f);
						return -1;
					}
					printf ( "UTC%+03d:%02d,%+4d,%3d,%3d,%2d,%2d,%3d,%2d,%2d, \"%s\"\n",
					timezones->utc_offset_h,timezones->utc_offset_m,timezones->utc_offset_h,timezones->utc_offset_m,
					timezones->dst_start_m,timezones->dst_start_n,timezones->dst_start_d,
					timezones->dst_end_m,timezones->dst_end_n,timezones->dst_end_d,
					timezones->desc);
					*tz=mytz;
					timezones =&(*tz)->timezones[number_of_timezones];
				}
			}
		}
		fclose(f);
		return 0;
	} else {
		printf("Can't open time zone file");
		return -1;
	}
}

void cg_system_sync_ntp_cb(cg_system_sync_ntp_state_t status, void *ctx) {
	cg_status_t cg_status;
	printf("ntp_status %d\n", status);
	exit_program = 1;
	fflush(stdout);
	switch(status) {
		case CG_NTP_SYNC_SUCCESS:
			printf("Successfully synched NTP date\n");
			break;
		case CG_NTP_SYNC_FAILURE:
			if(--max_retry) {
				 exit_program = 0;
				 cg_status = cg_system_sync_ntp();
				switch (cg_status) {
				case CG_STATUS_OK:
					printf("Successfully called synced ntp script\n");
					fflush(stdout);
					break;
				case CG_STATUS_INVALID_PARAMETER:
					printf("Invalid parameter found\n");
					break;
				case CG_STATUS_ERROR:
					printf("Error syncing ntp server, no script found\n");
					break;
				default:
					printf("Unknown error\n");
				}
			}
			else {
				printf("Cannot synchronize NTP date\n");
				exit_code = -1;
				exit_program = 1;
			}
			break;
		case CG_NTP_SYNC_NO_SERVER:
			printf("No NTP server name found\n");
			exit_code = -1;
			break;
		default:
			printf("Unknown error\n");
			exit_code = -1;
	}
}

int main() {
	cg_status_t status;
	int32_t level;
	timezone_table_t *tz=NULL;
	int i,found=0;

	cg_init("example_timezone");

	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	status =get_timezone_table(&tz);
	if(status!=CG_STATUS_OK) {
		printf("Can't read time zone table\n");
		exit(-1);
	}
	// set to time zone with location Astana
	for(i=0;i<tz->number_of_timezones;i++) {
		if(strstr(tz->timezones[i].desc,"Astana")) {
			printf("timezone UTC%03d:%02d description %s\n",tz->timezones[i].utc_offset_h, tz->timezones[i].utc_offset_m, tz->timezones[i].desc);
			found = i;
			break;
		}
	}
	if(found>=0) {
		// set timezone for Astana
		status = cg_system_set_timezone(&tz->timezones[found]);
		switch (status) {
		case CG_STATUS_OK:
			printf("Time zone OK\n");
			free(tz);
			break;
		case CG_STATUS_INVALID_PARAMETER:
			printf("Invalid parameter found\n");
			exit_code = -1;
			break;
		case CG_STATUS_ERROR:
			printf("Error setting time zone\n");
			exit_code = -1;
			break;
		default:
			printf("Unknown error\n");
			exit_code = -1;
		}
	} else {
			printf("Can't find timezone of Astana");
			free(tz);
			cg_deinit();
			exit(-1);
	}

	// set local time
	{
		cg_date_time_t date_time;
		date_time.date_time.tm_year=2017-1900;
		date_time.date_time.tm_mday=5;
		date_time.date_time.tm_mon=3; // April
		date_time.date_time.tm_hour=15;
		date_time.date_time.tm_min=0;
		date_time.date_time.tm_sec=0;
		date_time.time_format=CG_LOCAL_TIME;
		status = cg_system_set_date_time(&date_time);
		switch (status) {
		case CG_STATUS_OK:
			printf("%s time set to %s",(date_time.time_format == CG_LOCAL_TIME ? "Local" : "UTC"),asctime(&date_time.date_time));
			break;
		case CG_STATUS_INVALID_PARAMETER:
			printf("Invalid parameter found\n");
			exit_code = -1;
			break;
		case CG_STATUS_ERROR:
			printf("Error setting time\n");
			exit_code = -1;
			break;
		default:
			printf("Unknown error\n");
			exit_code = -1;
		}
		{
			cg_date_time_t *date_time;
			status = cg_system_get_date_time(CG_UTC_TIME,&date_time);
			switch(status) {
				case CG_STATUS_OK:
					printf("%s time is %s",(date_time->time_format == CG_LOCAL_TIME ? "Local" : "UTC"),asctime(&date_time->date_time));
					free(date_time);
					break;
				default:
					printf("Error occurred");
					exit_code = -1;
			}
			status = cg_system_get_date_time(CG_LOCAL_TIME,&date_time);
			switch(status) {
				case CG_STATUS_OK:
					printf("%s time is %s",(date_time->time_format == CG_LOCAL_TIME ? "Local" : "UTC"),asctime(&date_time->date_time));
					free(date_time);
					break;
				default:
					printf("Error occurred");
					exit_code = -1;
			}
		}
	}
	{
		cg_date_time_t date_time;
		date_time.date_time.tm_year=2016-1900;
		date_time.date_time.tm_mday=5;
		date_time.date_time.tm_mon=3; // April
		date_time.date_time.tm_hour=15;
		date_time.date_time.tm_min=0;
		date_time.date_time.tm_sec=0;
		date_time.time_format=CG_UTC_TIME;
		status = cg_system_set_date_time(&date_time);
		switch (status) {
		case CG_STATUS_OK:
			printf("%s time set to %s",(date_time.time_format == CG_LOCAL_TIME ? "Local" : "UTC"),asctime(&date_time.date_time));
			break;
		case CG_STATUS_INVALID_PARAMETER:
			printf("Invalid parameter found\n");
			exit_code = -1;
			break;
		case CG_STATUS_ERROR:
			printf("Error setting time\n");
			exit_code = -1;
			break;
		default:
			printf("Unknown error\n");
			exit_code = -1;
		}
	}
	{
		cg_date_time_t *date_time;
		status = cg_system_get_date_time(CG_UTC_TIME,&date_time);
		switch(status) {
			case CG_STATUS_OK:
				printf("%s time is %s",(date_time->time_format == CG_LOCAL_TIME ? "Local" : "UTC"),asctime(&date_time->date_time));
				free(date_time);
			   break;
			default:
				printf("Error occurred\n");
				exit_code = -1;
		}
		status = cg_system_get_date_time(CG_LOCAL_TIME,&date_time);
		switch(status) {
			case CG_STATUS_OK:
				printf("%s time is %s",(date_time->time_format == CG_LOCAL_TIME ? "Local" : "UTC"),asctime(&date_time->date_time));
				free(date_time);
			   break;
			default:
				printf("Error occurred\n");
				exit_code = -1;
		}
	}
	{
		char *ntp_server;
		status = cg_system_get_ntp_server(&ntp_server);
		switch (status) {
		case CG_STATUS_OK:
			printf("Current npt server is %s\n",ntp_server);
			free(ntp_server);
			break;
		case CG_STATUS_INVALID_PARAMETER:
			printf("Invalid parameter found\n");
			exit_code = -1;
			break;
		case CG_STATUS_ERROR:
			printf("Error setting time\n");
			exit_code = -1;
			break;
		default:
			printf("Unknown error\n");
			exit_code = -1;
		}

		printf("Set ntp server pool.ntp.org\n");
		status = cg_system_set_ntp_server("pool.ntp.org");
		switch (status) {
		case CG_STATUS_OK:
			printf("Successfully set ntp server pool.ntp.org\n");
			break;
		case CG_STATUS_INVALID_PARAMETER:
			printf("Invalid parameter found\n");
			exit_code = -1;
			break;
		case CG_STATUS_ERROR:
			printf("Error setting ntp server\n");
			exit_code = -1;
			break;
		default:
			printf("Unknown error\n");
			exit_code = -1;
		}

		status = cg_system_sync_ntp_register_callback(cg_system_sync_ntp_cb, context);
		switch (status) {
		case CG_STATUS_OK:
			printf("Successfully registered callback\n");
			break;
		case CG_STATUS_INVALID_PARAMETER:
			printf("Invalid parameter found\n");
			exit_code = -1;
			break;
		case CG_STATUS_ERROR:
			printf("Error registering callback\n");
			exit_code = -1;
			break;
		default:
			printf("Unknown error\n");
			exit_code = -1;
		}
	}

	// Please provide internet connectivity, otherwise NTP time syncronisation will fail.
	status = cg_system_sync_ntp();
	switch (status) {
	case CG_STATUS_OK:
		printf("Successfully called synced ntp script\n");
		break;
	case CG_STATUS_INVALID_PARAMETER:
		printf("Invalid parameter found\n");
		exit_code = -1;
		break;
	case CG_STATUS_ERROR:
		printf("Error syncing ntp server, no script found\n");
		exit_code = -1;
		break;
	default:
		printf("Unknown error\n");
		exit_code = -1;
	}
	printf ("Awaiting NTP synchronisation callback\n");
	while(!exit_program) {
		sleep(1);
	}
	status = cg_system_sync_ntp_deregister_callback(cg_system_sync_ntp_cb, context);
		switch (status) {
		case CG_STATUS_OK:
			printf("Successfully de deregistered callback\n");
			break;
		case CG_STATUS_INVALID_PARAMETER:
			printf("Invalid parameter found\n");
			exit_code = -1;
			break;
		case CG_STATUS_ERROR:
			printf("Error deregistering callback\n");
			exit_code = -1;
			break;
		default:
			printf("Unknown error\n");
			exit_code = -1;
	}
	printf("exiting %d\n",exit_code);
	cg_deinit();
	exit(exit_code);
}
