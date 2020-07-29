/* EXAMPLE:
 *  cg_ui
 */
#include <stdio.h>

#include <libcg/cg_general.h>
#include <libcg/cg_ui.h>

static void session_cb(unsigned int session_id, cg_ui_session_action_t action)
{
	switch(action) {
		case CG_UI_SESSION_ADD:
			printf("A new user logged in via session id %08X\n", session_id);
			break;
		case CG_UI_SESSION_DEL:
			printf("A user logged out or got a timeout for session id %08X\n", session_id);
			break;
		case CG_UI_SESSION_PURGE:
			//session_id not valid in this case.
			printf("Some unexpected behaviour occured. All sessions are deleted\n");
			break;
	}
}

int main (void)
{
	cg_status_t cg_status;
	int32_t level;
	char c;

	cg_init("example_ui_session");

	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	cg_status = cg_ui_register_session_callback(session_cb);

	if (cg_status == CG_STATUS_OK)
		printf("Session callback successfully registered\n");
	else if (cg_status == CG_STATUS_INVALID_PARAMETER)
		printf("Session callback not registered due to an invalid paramter\n");
	else if (cg_status == CG_STATUS_ERROR)
		printf("Session callback not registered due to an error\n");

	printf("Press any key to exit ...\n");
	c = getchar();

	cg_ui_deregister_session_callback();
	cg_deinit();

	return 0;
}
