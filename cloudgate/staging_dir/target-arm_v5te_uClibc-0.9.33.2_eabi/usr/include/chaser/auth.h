#ifndef AUTH_H
#define AUTH_H

// The maximum length a username can have is limited by libc and the kernel
// we could use LOGIN_NAME_MAX ans sysconf, but we agreed upon 32 with the webfrontend
#define M2M_LOGIN_NAME_MAX 32

/* Buffer contains two null terminated strings. old_entry_length contains the strlen
 * of the first one, new_entry_length the strlen of the second one. */
typedef struct {
	int old_entry_length;
	int new_entry_length;
	char buffer[];
} double_char_t;

void init_auth_rpcs(int* fd);

#endif
