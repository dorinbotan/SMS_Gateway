/* EXAMPLE:
 *  cg_ui
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <libcg/cg_general.h>
#include <libcg/cg_ui.h>
#include <libcg/cg_conf.h>


static char *json_callback(char *json_data, int logged_in, void *context)
{
	char *retval;
	asprintf(&retval, "{\"request\":%s,\"context\":\"%s\",\"logged in\":%s}", json_data, (char *)context, logged_in?"true":"false");

	return retval; // trigger 'Status: 200 OK' 
//	return NULL; // trigger 'Status: 400 Bad Request' 
}

static char *get_callback(char *query_string, int logged_in, void *context)
{
	char *retval;
	asprintf(&retval, "{\"request\":\"%s\",\"context\":\"%s\",\"logged in\":%s}", query_string, (char *)context, logged_in?"true":"false");
	return retval;
}

int
main (void)
{
	cg_status_t cg_status;
	int32_t level;

	cg_init("example_ui_simple");

	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	system("uci set m2mweb.customers=default");
	system("uci set m2mweb.customers.path=/rom/mnt/cust/examples");
	system("uci commit m2mweb");

	cg_status = cg_ui_register_json_callback("simple_json_backend", &json_callback, "The JSON Context");
	if(cg_status != CG_STATUS_OK){
		printf("Error registering JSON callback\n");
		goto faulty;
	}

	cg_status = cg_ui_register_get_callback("simple_json_backend", &get_callback, "The GET Context");
	if(cg_status != CG_STATUS_OK){
		printf("Error registering GET callback\n");
		goto faulty;
	}

	//The url must be relative AND the path must exists.
	//Paths are in /www/your_relative_url
	cg_status = cg_ui_register_page("ui_simple example page", "customers/example_ui_simple.html");
	if(cg_status != CG_STATUS_OK){
		printf("Error registering example page\n");
		goto faulty;
	}

	printf("Press any key to deregister and quit.\n");
	getchar();

	cg_status = cg_ui_deregister_page("ui_simple example page");
	if(cg_status != CG_STATUS_OK){
		printf("Error deregistering example page\n");
		goto faulty;
	}

	cg_status = cg_ui_deregister_json_callback("simple_json_backend");
	if(cg_status != CG_STATUS_OK){
		printf("Error deregister JSON callback\n");
		goto faulty;
	}

	cg_status = cg_ui_deregister_get_callback("simple_json_backend");
	if(cg_status != CG_STATUS_OK){
		printf("Error deregister GET callback\n");
		goto faulty;
	}

faulty:
	cg_deinit();

	return 0;
}
