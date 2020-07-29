#ifndef __CG_UI_H__
#define __CG_UI_H__

#include <libcg/cgate.h>

CG_BEGIN_DECLS

/**
* \file cg_ui.h
* \brief Header file containing definitions for all UI-related functionality
*/

/**
* \defgroup ui UI
* @{
* Consider the following example to extend the CloudGate web UI.\n
* In your SDK directory, save below configuration settings as \c config/config/m2mweb:\n
*
\verbatim
    package m2mweb

    config default 'customers'
        option path '/rom/mnt/cust/examples'

    config replacement_ui 'ui'
        option url 'customers/install.html'

    config replacement_logo 'logo'
        option url 'http://www.option.com/wp-content/uploads/2014/09/logo.png'

    config ui 'custom_ui'
        option brand_logo 'http://www.option.com/wp-content/uploads/2014/09/logo.png'
        option company_logo 'http://www.option.com/wp-content/uploads/2014/09/logo.png'
        option favicon 'http://www.option.com/wp-content/uploads/2014/09/logo.png'
        option tagline '<font color="blue"><b>My new <u>tagline</u> in blue bold text</b></font>'
        option tabtitle 'My new browser tab title'

\endverbatim
*
* The first part shows how a new document root can be added:\n
* 	\li \b \c customers : the symlink which will be created in /www.\n
* 	\li \b \c path : absolute path to the new document root.
*
* 	\b NOTE \b 1: the path needs to be absolute and it must exist.\n
* 	\b NOTE \b 2: you can add multiple document roots.\n
* 	\b NOTE \b 3: you cannot create a document root with the name 'api'.
*
* The second part shows how the web UI, shown after logging in, can be replaced:\n
* 	\li \b \c replacement_ui : there can only be one replacement UI. The name is always 'ui'.\n
* 	\li \b \c path : relative or absolute path to the replacement page.
*
* DEPRECATED: The third part shows how the CloudGate logo, at the topleft of the web UI, can be replaced.\n
* This part is deprecated. Please use the custom_ui part.
* 	\li \b \c replacement_logo : there can only be one replacement logo. The name is always 'logo'.\n
* 	\li \b \c url : relative or absolute url to the new logo.
*
* The last part shows how the GUI can be extended. You can change the brand logo, company logo, favicon, tagline and browser tab title.\n
* The brand logo, company logo, favicon and tagline can also be hidden from the UI.
* 	\li \b \c brand_logo : the CloudGate logo, at the topleft of the web UI, can be replaced. Optimal size: 250 x 40 pixels.\n
* 	\li \b \c company_logo : the Option logo, at the topright of the web UI, can be replaced. Optimal size: 120 x 80 pixels.\n
* 	\li \b \c tagline : the 'Connecting THINGS to the cloud' text, at the topcenter of the web UI, can be replaced. HTML tags are supported. (Inline) CSS is not supported.\n
* 	\li \b \c favicon : the browser tab favicon can be replaced.\n
* 	\li \b \c tabtitle : the browser tab title can be replaced.\n\n
* To hide an item, set the value of an option to 'hidden'. For example, to hide the brand_logo do:\n
* config ui 'custom_ui'\n
* option brand_logo 'hidden'
*
*/

/** Callback for receiving JSON data. This function is not called for GET requests if a GET request handler is registered for the same identifier.
* \param[in] context Context as set in \ref cg_ui_register_json_callback
* \param[in] json_data JSON character data or an empty string if the request was not a POST request with content-type application/json.
* \param[in] logged_in FALSE if not logged in.
* \return A string that will be sent as a reply with content-type application/json. If the return value is NULL 'Status: 400 Bad Request' will be sent. This value will be freed by the caller.
*/
typedef char *(*cg_ui_json_cb_t)(char *json_data, int logged_in, void *context);

/** Callback for receiving GET request.
* \param[in] context Context as set in \ref cg_ui_register_get_callback
* \param[in] query_string Any data following the ? in the requested URL
* \param[in] logged_in FALSE if not logged in.
* \return A string that will be sent as a reply with content-type application/json. If the return value is NULL 'Status: 400 Bad Request' will be sent. This value will be freed by the caller.
*/
typedef char *(*cg_ui_get_cb_t)(char *query_string, int logged_in, void *context);

/** Register a new link in the custom menu of the Option CloudGate webUI.
* \param[in] name The text displayed in the menu.
* \param[in] url The destination URL of this link. This must be a RELATIVE url. It will be mapped to /www/your_relative_url. The path must exist!
* \return CG_STATUS_OK if successful else CG_STATUS_ERROR.
*/
cg_status_t cg_ui_register_page(const char *name, const char *url);

/** Deregister a link in the custom menu of the Option CloudGate webUI.
* \param[in] name The text displayed in the menu.
* \return CG_STATUS_OK if successful else CG_STATUS_ERROR.
*/
cg_status_t cg_ui_deregister_page(const char *name);

/** Register a callback for HTTP requests.
* \param[in] identifier Unique identifier used as the last part of the URL http://cloudgate/api/libcg/identifier
* \param[in] cb Callback for JSON data.
* \param[in] context Context data passed to cb when called.
* \return CG_STATUS_OK if successful. CG_STATUS_INVALID_PARAMETER if no callback has been provided. CG_STATUS_ERROR if there is a JSON callback registered for this identifier or when another failure occurs.
*/
cg_status_t cg_ui_register_json_callback(const char *identifier, cg_ui_json_cb_t cb, void *context);

/** Deregister a callback for HTTP requests.
* \param[in] identifier Unique identifier used as the last part of the URL http://cloudgate/api/libcg/identifier
* \return CG_STATUS_OK if successful. CG_STATUS_ERROR in case of failure.
*/
cg_status_t cg_ui_deregister_json_callback(const char *identifier);

/** Register a callback for HTTP GET requests.
* \param[in] identifier Unique identifier used as the last part of the URL http://cloudgate/api/libcg/identifier
* \param[in] cb Callback for GET data.
* \param[in] context Context data passed to cb when called.
* \return CG_STATUS_OK if successful. CG_STATUS_INVALID_PARAMETER if no callback has been provided. CG_STATUS_ERROR if there is a GET callback registered for this identifier or when another failure occurs.
*/
cg_status_t cg_ui_register_get_callback(const char *identifier, cg_ui_get_cb_t cb, void *context);

/** Deregister a callback for HTTP GET requests.
* \param[in] identifier Unique identifier used as the last part of the URL http://cloudgate/api/libcg/identifier
* \return CG_STATUS_OK if successful. CG_STATUS_ERROR in case of failure.
*/
cg_status_t cg_ui_deregister_get_callback(const char *identifier);

/**
 * \enum cg_ui_session_action_t
 * \brief Action to be taken on a session event
 */
typedef enum {
	/* A user has logged in to the web GUI. A new session is created. */
	CG_UI_SESSION_ADD,
	/* A user has logged out from the web GUI or the session is expired. A session is deleted. */
	CG_UI_SESSION_DEL,
	/* Some unexpected behaviour occured. All active sessions are deleted. */
	CG_UI_SESSION_PURGE
} cg_ui_session_action_t;

/** Callback for receiving session information when a user, for example, logs in via the web GUI.
 *  \param[in] session_id The session_id of the logged in/out user. Session id on only valid if action is CG_UI_SESSION_ADD or CG_UI_SESSION_DEL.
 *  \param[in] action A session can be added, deleted or becomes invalid. On CG_UI_SESSION_PURGE, all active sessions are invalid. */
typedef void (*cg_ui_session_cb)(unsigned int session_id, cg_ui_session_action_t action);

/** Register a callback to receive session notifications when a user logs in/out to/from the web GUI
 * \param[in] Call function of type cg_ui_session_cb
* \return CG_STATUS_OK if successful. CG_STATUS_INVALID_PARAMETER if no callback has been provided. CG_STATUS_ERROR if there is a session callback registered for this identifier or when another failure occurs.
 */
cg_status_t cg_ui_register_session_callback(cg_ui_session_cb cb);

/** Deregister the callback for receiving session notifications
* \return CG_STATUS_OK if successful. CG_STATUS_ERROR if a failure occurs.
 */
cg_status_t cg_ui_deregister_session_callback(void);

/** @} */

CG_END_DECLS

#endif /* __CG_UI_H__*/
