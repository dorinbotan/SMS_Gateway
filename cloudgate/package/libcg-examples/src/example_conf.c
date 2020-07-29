/* EXAMPLE:
 *  cg_conf
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <libcg/cg_general.h>
#include <libcg/cg_conf.h>


static void
tear_down (void)
{
	cg_deinit();
}

static void
show_entire (cg_config_t *ctx)
{
	cg_status_t cg_status;
	cg_conf_list_t *sections;
	int i;

	cg_status = cg_conf_get_sections(ctx, &sections);
	if(cg_status != CG_STATUS_OK){
		printf("Error while getting sections\n");
		return;
	}
	if(sections->number>0){
		printf("+ Printing %d sections\n",sections->number);
	}else{
		printf("No sections present\n");
	}
	for(i=0; i<sections->number; i++){
		cg_conf_list_t *options;
		int j;
		int lastsection;

		lastsection = (i == (sections->number-1));
		cg_status = cg_conf_get_options(ctx, sections->values[i], &options);
		if(cg_status != CG_STATUS_OK){
			printf("Error while getting options for \"%s\"\n", sections->values[i]);
			return;
		}
		printf("%c-%c- \"%s\" (%d)\n", lastsection?'\'':'+', options->number?'+':'-', sections->values[i], options->number);
		for(j=0; j<(options->number); j++){
			const char *value;
			cg_status = cg_conf_get(ctx, sections->values[i], options->values[j], &value);
			if(cg_status != CG_STATUS_OK){
				value = "Error while getting value\n";
			}
			printf("%c %c- \"%s\": %s\n", lastsection?' ':'|', (j == (options->number-1))?'\'':'+', options->values[j], value);
		}
		free(options);
	}
	free(sections);
}

int
main (void)
{
	cg_status_t cg_status;
	int32_t level;
	cg_config_t *ctx;

	cg_init("example_conf");

	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	cg_status = cg_conf_open("example_file", TRUE, &ctx);
	if(cg_status != CG_STATUS_OK){
		printf("Error opening temporary\n");
		goto faulty;
	}

	cg_status = cg_conf_set(ctx, "example_section1", "example_option1", "example_value1");
	if(cg_status != CG_STATUS_OK){
		printf("Error setting example_option1\n");
		goto undo;
	}

	cg_status = cg_conf_set(ctx, "example_section1", "example_option2", "example_value2");
	if(cg_status != CG_STATUS_OK){
		printf("Error setting example_option2\n");
		goto undo;
	}

	cg_status = cg_conf_add_section(ctx, "example_section2");
	if(cg_status != CG_STATUS_OK){
		if(cg_status == CG_STATUS_ERROR){
			cg_status = cg_conf_del(ctx, "example_section2", NULL);
			if(cg_status != CG_STATUS_OK){
				printf("Error deleting example_section2\n");
				goto undo;
			}
			printf("Cleaned up leftover example_section2, add_section only works when the section does not exist yet.\n");
			goto cleanup;
		}
		printf("Error creating example_section2\n");
		goto undo;
	}

	printf("Initialized config\n");

	show_entire(ctx);

	cg_status = cg_conf_close(ctx);
	if(cg_status != CG_STATUS_OK){
		printf("Error closing temporary\n");
		goto faulty;
	}

	printf("Press any key to continue\n");
	getchar();

	cg_status = cg_conf_open("example_file", FALSE, &ctx);
	if(cg_status != CG_STATUS_OK){
		printf("Error opening permanent\n");
		goto faulty;
	}

	printf("Config is opened permanently, temporary data is invisible.\n");

	show_entire(ctx);

	cg_status = cg_conf_set(ctx, "example_section1", "example_option3", "example_value3");
	if(cg_status != CG_STATUS_OK){
		printf("Error setting example_option3\n");
		goto undo;
	}

	cg_status = cg_conf_set(ctx, "example_section3", "example_option1", "example_value4");
	if(cg_status != CG_STATUS_OK){
		printf("Error setting example_option1\n");
		goto undo;
	}

	cg_status = cg_conf_set(ctx, "example_section3", "example_option2", "example_value5");
	if(cg_status != CG_STATUS_OK){
		printf("Error setting example_option2\n");
		goto undo;
	}

	printf("Some data is added and saved on close.\n");

	show_entire(ctx);

	cg_status = cg_conf_close(ctx);
	if(cg_status != CG_STATUS_OK){
		printf("Error closing permanent\n");
		goto faulty;
	}

	printf("Press any key to continue\n");
	getchar();

	cg_status = cg_conf_open("example_file", FALSE, &ctx);
	if(cg_status != CG_STATUS_OK){
		printf("Error opening permanent\n");
		goto faulty;
	}

	printf("Config is opened permanently again, the data is still here.\n");

	show_entire(ctx);

	cg_status = cg_conf_set(ctx, "example_section3", "example_option3", "example_value6");
	if(cg_status != CG_STATUS_OK){
		printf("Error setting example_option3\n");
		goto undo;
	}

	printf("One item is added.\n");

	show_entire(ctx);

	cg_status = cg_conf_revert(ctx, "example_section3", NULL);
	if(cg_status != CG_STATUS_OK){
		printf("Error reverting example_section3\n");
		goto undo;
	}

	printf("All changes to section3 since the previous close are undone.\n");

	show_entire(ctx);

	cg_status = cg_conf_close(ctx);
	if(cg_status != CG_STATUS_OK){
		printf("Error closing permanent\n");
		goto faulty;
	}

	printf("Press any key to continue\n");
	getchar();

	cg_status = cg_conf_open("example_file", TRUE, &ctx);
	if(cg_status != CG_STATUS_OK){
		printf("Error opening temporary\n");
		goto faulty;
	}

	printf("Temporary config is still present after close/open cycle, temporary open keeps the data until reboot.\nPermanent config is also visible but temporary sections have precedence.\n");

	show_entire(ctx);

	cg_conf_del(ctx, "example_section1", NULL);
	if(cg_status != CG_STATUS_OK){
		printf("Error deleting example_section1\n");
		goto undo;
	}

	printf("Now only section1 is deleted.\n");

	show_entire(ctx);

	cg_status = cg_conf_close(ctx);
	if(cg_status != CG_STATUS_OK){
		printf("Error closing temporary\n");
		goto faulty;
	}

	printf("Press any key to continue\n");
	getchar();

	cg_status = cg_conf_open("example_file", FALSE, &ctx);
	if(cg_status != CG_STATUS_OK){
		printf("Error opening permanent\n");
		goto faulty;
	}

	printf("Permanent config cleanup.\n");

	show_entire(ctx);

	cg_conf_del(ctx, "example_section3", NULL);
	if(cg_status != CG_STATUS_OK){
		printf("Error deleting example_section3\n");
		goto undo;
	}

	cg_conf_del(ctx, "example_section1", NULL);
	if(cg_status != CG_STATUS_OK){
		printf("Error deleting example_section1\n");
		goto undo;
	}

	printf("Now empty.\n");

	show_entire(ctx);

	cg_status = cg_conf_close(ctx);
	if(cg_status != CG_STATUS_OK){
		printf("Error closing permanent\n");
		goto faulty;
	}

	printf("Press any key to exit\n");
	getchar();

	tear_down();

	return 0;

undo:
	cg_status = cg_conf_revert(ctx, NULL, NULL);
	if(cg_status != CG_STATUS_OK){
		printf("Error reverting for undo\n");
	}

cleanup:
	cg_status = cg_conf_close(ctx);
	if(cg_status != CG_STATUS_OK){
		printf("Error closing for cleanup\n");
	}

faulty:
	tear_down();

	return 0;
}
