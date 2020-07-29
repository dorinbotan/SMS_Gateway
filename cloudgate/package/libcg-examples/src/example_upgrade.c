/* EXAMPLE:
 *  cg_upgrade
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <libcg/cg_general.h>
#include <libcg/cg_upgrade.h>

// example_upgrade performs a device upgrade from a file
//Note
//    Switch off auto upgrade by the provisioning server to be sure
//    the manual provisioning will not be overruled by an automatic provisioning
//    from the provisioning server.

#define BUFFER_SIZE 16384

int main(int argc,char *argv[])
{
	cg_status_t cg_status;
	int f = -1;
	int level;
	cg_upgrade_t *handle=NULL;

	if(argc == 2) {
		if ((f = open(argv[1], O_RDONLY)) == -1) {
			fprintf(stderr, "Could not open image %s\n",argv[1]);
			return -1;
		}
	}
	else {
		fprintf(stderr, "usage: example_upgrade <file name>\n");
		return -1;
	}

	cg_init("example_upgrade");

	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	if(cg_prov_set_automatic(0) == CG_STATUS_OK) {
		printf("successfully disabled automatic provisioning\n");
	} else {
		fprintf(stderr,"Can't switch off automatic provisioning\n");
		goto error;
	}
	cg_status = cg_upgrade_start( &handle );
	switch (cg_status) {
		case CG_STATUS_OK:
			printf("Successfully started upgrade\n");
			break;
		case CG_STATUS_INVALID_PARAMETER:
			fprintf(stderr,"Upgrade invalid parameter\n");
			goto error;
		case CG_STATUS_RESOURCE_BUSY:
			fprintf(stderr,"Another upgrade is ongoing\n");
			goto error;
		case CG_STATUS_ERROR:
		default:
			fprintf(stderr,"Can't start Upgrade\n");
			goto error;
	}
	{
		int nbr_read;
		char *buffer;
		int return_code = -1;
		buffer = malloc (BUFFER_SIZE);
		if(buffer == NULL) {
			fprintf (stderr, "No memory available\n");
			goto error;
		}
		do {
			nbr_read = read(f,buffer,BUFFER_SIZE);
			if (nbr_read > 0 ) {
				if (cg_upgrade_data(handle, buffer, nbr_read) == CG_STATUS_OK)
					continue;
				else
					break;
			}
		} while (nbr_read > 0);
		if((cg_status = cg_upgrade_stop(handle)) == CG_STATUS_OK) {
			printf("upgrade is successful\n");
			return_code = 0;
		}
		else {
            if(cg_status==CG_STATUS_DOWNGRADE_PREVENTION) {
                fprintf(stderr,"downgrade prevention\n");
            }
            else
                fprintf(stderr,"manual upgrade failed\n");
		}
		free(buffer);
		close(f);
		cg_deinit();
		return return_code;
	}
error:
    cg_upgrade_stop(handle);
	if (f !=-1) close(f);
    cg_deinit();
	return -1;
}
