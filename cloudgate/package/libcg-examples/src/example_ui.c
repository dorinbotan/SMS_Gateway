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


static char *json_analyser(char *json_data, int logged_in, void *context)
{
	char *retval;
	asprintf(&retval, "{\"request\":%s,\"context\":\"%s\",\"logged in\":%s}", json_data, (char *)context, logged_in?"true":"false");
	return retval;
}

int
main (void)
{
	cg_status_t cg_status;
	int32_t level;

	cg_init("example_ui");

	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	//See file cg_ui.h on how to get the same results without root permission.
	printf("Warning: following uci system calls need root permission.\n");

	//Add a new document root.
	//The config file is m2mweb. Create a new symlink customers which is mapped to examples.
	//NOTE: the path must exist.

	system("uci set m2mweb.customers=default");
	system("uci set m2mweb.customers.path=/rom/mnt/cust/examples");
	system("uci commit m2mweb");

	//Add a new replacement ui
	//The config file is m2mweb. Create a replacement ui. After logging in, the defined url is
	//loaded in an iframe. The url can be relative or absolute.

	system("uci set m2mweb.ui=replacement_ui");
	system("uci set m2mweb.ui.url=customers"); //Maps to the index.html file in the document root customers (see above).
	//OR
	system("uci set m2mweb.ui.url=http://www.option.com"); //Maps to an external website.
	//OR
	system("uci set m2mweb.ui.url=customers/install.html"); //Maps to the install.html file in the document root customers (see above).
	system("uci commit m2mweb");

	//Add a new gui replacement logo. DEPRECATED: please use GUI customization below.
	system("uci set m2mweb.logo=replacement_logo");
	//The url can be relative to the document root in /www or you can specify an image from an external website.
	system("uci set m2mweb.logo.url=http://www.option.com/wp-content/uploads/2014/09/logo.png");
	system("uci commit m2mweb");

	//GUI customization. Allows to change the brand logo, company logo, tagline, tab title and favicon.
	//Priority over the DEPRECATED replacement_logo above.
	//The url for brand_logo, company_logo and favicon can be relative to the document root in /www or you can specify an image from an external website.
	system("uci set m2mweb.custom_ui=ui");
	//Brand logo
	system("uci set m2mweb.custom_ui.brand_logo=http://www.option.com/wp-content/uploads/2014/09/logo.png");
	//Company logo
	system("uci set m2mweb.custom_ui.company_logo=http://www.option.com/wp-content/uploads/2014/09/logo.png");
	//Favicon
	system("uci set m2mweb.custom_ui.favicon=http://www.option.com/wp-content/uploads/2014/09/logo.png");
	//Tagline
	system("uci set m2mweb.custom_ui.tagline='My new tagline'");
	//Tab title
	system("uci set m2mweb.custom_ui.tabtitle='My new browser tab title'");
	system("uci commit m2mweb");

	printf("You have to reboot to apply the document root changes\n");

	cg_status = cg_ui_register_json_callback("example_page_1", &json_analyser, "The Context");
	if(cg_status != CG_STATUS_OK){
		printf("Error register callback\n");
		goto faulty;
	}

	//The url must be relative AND the path must exist.
	//Paths are in /www/your_relative_url
	cg_status = cg_ui_register_page("example_page_1", "customers/example_page_1.html");
	if(cg_status != CG_STATUS_OK){
		printf("Error registering example page 1\n");
		goto faulty;
	}

	printf("Press any key to continue. Example page 1 will be deregistered\n");
	getchar();

	cg_status = cg_ui_deregister_page("example_page_1");
	if(cg_status != CG_STATUS_OK){
		printf("Error deregistering example page 1\n");
		goto faulty;
	}

	cg_status = cg_ui_deregister_json_callback("example_page_1");
	if(cg_status != CG_STATUS_OK){
		printf("Error deregister JSON callback example page 1\n");
		goto faulty;
	}

	printf("Press any key to exit.");
	getchar();

/* Additional info to change/delete the document root/replacement UI.

	printf("Change/delete document root\n");
	//Change the document root.
	system("uci set m2mweb.customers.path=/root/second_website");
	system("uci commit m2mweb");

	//Remove the document root.
	system("uci delete m2mweb.customers");
	system("uci commit m2mweb");

	//Change the replacement UI
	system("uci set m2mweb.ui.url=your_new_url");
	system("uci commit m2mweb");

	//Remove the replacement UI
	system("uci delete m2mweb.ui");
	system("uci commit m2mweb");

	//Change the replacement logo
	system("uci set m2mweb.logo.url=your_new_url");
	system("uci commit m2mweb");

	//Remove the replacement logo
	system("uci delete m2mweb.logo");
	system("uci commit m2mweb");

	printf("Press any key to exit\n");
	getchar();
*/
faulty:
	cg_deinit();

	return 0;
}
