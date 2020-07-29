/* EXAMPLE:
 * Show use of cg_wwan PIN API calls:
 * cg_status_t cg_wwan_pin_get_state(const char *dev_name, cg_wwan_pin_state_t *pin_state)
 * cg_status_t cg_wwan_pin_set_enabled(const char *dev_name, uint8_t enabled, const char *pin)
 * cg_status_t cg_wwan_submit_pin(const char *dev_name, const char *pin)
 * cg_status_t cg_wwan_change_pin(const char *dev_name, const char *old_pin_puk, const char *new_pin)
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <libcg/cg_general.h>
#include <libcg/cg_wwan.h>

char* dev_name = NULL;
#define uint_8 unsigned char
const char *erc_descr[] = {"success", "error", "resource busy", "invalid parameter", "resource unavailable"} ;

static void usage(char *program){
	printf("%s [ -s ] || [ -c <old PIN|PUK> <new PIN> ] || [ -u <PIN> ] || [ -e <PIN> <true|false> ] || [ -v <save|nosave> ]\n", program) ;
	printf("\t-s : get PIN state\n\t-c : change PIN\n\t-u : submit PIN\n\t-e : enable/disable PIN\n\t-v : save pin setting\n") ;
}

static void print_status(char *program, int erc) {
	if(erc == -1) {
		usage(program);
	}
	if (erc > 0){
		printf("response status: %s\n", erc_descr[erc]);
	}
}

static int check_pin_puk(const char s_pin_puk[]) {
	int length, i = 0;

	if(s_pin_puk == NULL) {
		goto bad_pin_puk;
	}

	if((length = strlen(s_pin_puk)) < 4 || length > 8) {
		goto bad_pin_puk;
	}

	for(i = 0; i < length; i++) {
		if(!isdigit(s_pin_puk[i])) goto bad_pin_puk;
	}
	return 0;
bad_pin_puk:
	printf("invalid code=%s\n", s_pin_puk);
	return -1;
}

static cg_status_t get_pin_state() {
	cg_status_t status ;
	cg_wwan_pin_state_t pin_state ;
	char *pin_type[] = {"unknown", "SIM not inserted", "ready", "SIM PIN", "SIM PUK" , "SIM failure", "locked", "not supported"};

	status = cg_wwan_pin_get_state(dev_name, &pin_state);

	if(status == CG_STATUS_OK) {
		printf("PIN type    = %s\n", (pin_state.pin_type > 7 || pin_state.pin_type < 0)?"error":pin_type[pin_state.pin_type]) ;
		printf("PIN retries = %d\n", pin_state.pin_retries) ;
		printf("PUK retries = %d\n", pin_state.puk_retries) ;
		printf("PIN enabled = %s\n", (pin_state.pin_enabled)?"enabled":"disabled") ;
	}

	return status ;
}

static cg_status_t set_pin_enabled(const char *s_enabled, const char *s_pin) {
	uint_8 enable_flag;

	if( check_pin_puk(s_pin) == -1 ) return -2 ;
	enable_flag = (uint_8) (!strcmp(s_enabled, "true"))?TRUE:FALSE;
	return cg_wwan_pin_set_enabled(dev_name, enable_flag, s_pin);
}

static cg_status_t submit_pin(const char *s_pin) {
	if( check_pin_puk(s_pin) == -1 ) return -2 ;
	return cg_wwan_submit_pin(dev_name, s_pin);
}

static int change_pin(const char *s_pin_old, const char *s_pin_new) {
	if( check_pin_puk(s_pin_old) == -1 ) return -2 ;
	if( check_pin_puk(s_pin_new) == -1 ) return -2 ;
	return cg_wwan_change_pin(dev_name, s_pin_old, s_pin_new);
}

static int set_pin_save(const char *s_save) {
	uint_8 save_flag;
	save_flag = (uint_8) (!strcmp(s_save, "save"))?TRUE:FALSE;
	return cg_wwan_save_pin(dev_name, save_flag);
}

int main(int argc, char **argv)
{
	int ret = -1 ;

	if (argc < 2) {
		usage(argv[0]) ;
		return 0 ;
	}

	cg_init("WWAN PIN examples");
	if (cg_wwan_get_active_primary_dev(&dev_name) != CG_STATUS_OK) {
		printf("Failed to get active primary device\n");
		return 0 ;
	}
	printf("Active primary device: '%s'\n", dev_name);

	if(!strncmp(argv[1], "-v", 2) && ( argc == 3))
		ret = set_pin_save(argv[2]);
	if(!strncmp(argv[1], "-c", 2) && ( argc == 4 ))
		ret = change_pin(argv[2], argv[3]);
	if(!strncmp(argv[1], "-u", 2) && ( argc == 3 ))
		ret = submit_pin(argv[2]);
	if(!strncmp(argv[1], "-e", 2) && ( argc == 4 ))
		ret = set_pin_enabled(argv[3], argv[2]);
	if(!strncmp(argv[1], "-s", 2))
		ret = get_pin_state();
	print_status(argv[0],ret) ;
	free(dev_name);
	cg_deinit();
	return 0;
}
