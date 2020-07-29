#if !defined (PPPD_SETTINGS_H)
#define PPPD_SETTINGS_H

#define LENGTH_USERPASSW_STRING	     32+1   // see m2mweb username password setting and extra char for ending 0 


typedef struct pppoe_interface_settings {
	char username[LENGTH_USERPASSW_STRING];
	char password[LENGTH_USERPASSW_STRING];
	} pppoe_config_t;

#endif

