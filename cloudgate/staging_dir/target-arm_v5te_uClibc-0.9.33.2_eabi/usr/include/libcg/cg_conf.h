#ifndef __CG_CONF_H__
#define __CG_CONF_H__

#include <libcg/cgate.h>

CG_BEGIN_DECLS

/**
* \file cg_conf.h
* \brief Header file containing definitions for functions related to setting/getting configuration items
*
* This part of the API can be used to manage configuration items. All configuration items are divided in
* to files. A file is divided in to sections. Each section has a name and a type. Each section can contain
* multiple options. Options are key-value pairs.
*
* These functions return CG_STATUS_INVALID_PARAMETER if a provided parameter is not valid (e.g. NULL
* pointer where only valid pointers are allowed) or if a configuration item does not exist and should
* exist to be able to perform the operation (e.g. you want to get the option 'enabled' in the section
* 'general', but the section 'general' does not exist). If a function does not specify that a parameter is
* optional, you should assume the parameter is mandatory. Names for section or options are limited to
* alphanumeric and '_'. Values are not restricted to any characters.
* CG_STATUS_ERROR is returned when an unexpected situation arises (e.g. failure to open a file). */

/**
* \defgroup conf Configuration functions
* @{
*/

/**
* \struct cg_config_t
* \brief Structure to reference the configuration context.
*/
typedef struct cg_config cg_config_t;

/**
* \struct cg_conf_list_t
* \brief Structure to return a list of values from the configuration.
*/
typedef struct cg_conf_list {
	/** Number of values in this list */
	int number;
	/** Array of pointers to the values in the list */
	const char *values[];
} cg_conf_list_t;

/** Open a configuration context for a specific configuration file
* \param[in] file The name of the configuration file to open.
* \param[in] state If TRUE any changes made to this configuration file will
* only kept as a temporary state. These settings will only be visible if this
* file is opened with state set to TRUE. Upon reboot these settings are lost.
* Setting state to FALSE will persist all changes. Persistent configurations
* are available for reading when opening in a temporary state.
* \param[out] ctx The context pointer for this configuration file. This
* pointer should be used with every cg_conf call that operates on this file.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an
* invalid parameter or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_conf_open(const char *file, int state, cg_config_t **ctx);

/** Close the provided configuration context. All changes will be saved
* persistently unless cg_conf_open was called with state set to TRUE.
* The ctx pointer will be freed upon return.
* \param[in] ctx The configuration context previously opened with cg_conf_open.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an
* invalid parameter or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_conf_close(cg_config_t *ctx);

/** Revert all changes made to the configuration context since cg_conf_open.
* The other parameters can optionally be used to only revert a specific section
* or option.
* \param[in] ctx The configuration context previously opened with cg_conf_open.
* \param[in] section The name of the section that needs to be reverted. Can be
* left NULL.
* \param[in] option The name of the option that needs to be reverted. Can be left
* NULL. If not NULL, section MUST also be non-NULL.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an
* invalid parameter or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_conf_revert(cg_config_t *ctx, const char *section, const char *option);

/** Set a section and an option value in the configuration.
* \param[in] ctx The configuration context previously opened with cg_conf_open.
* \param[in] section The name of the section where we are setting a value. If this
* section does not exist, it will be created.
* \param[in] option The name of the option for which we are setting a value.
* \param[in] value The value which we want to set.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an
* invalid parameter or CG_STATUS_ERROR in case of an error.
*/ 
cg_status_t cg_conf_set(cg_config_t *ctx, const char *section, const char *option, const char *value);

/** Get a section type or an option value from the configuration.
* \param[in] ctx The configuration context previously opened with cg_conf_open.
* \param[in] section The name of the section where we are getting a value from.
* \param[in] option The name of the option from which we are getting a value.
* \param[out] value The value that is read from the configuration is stored here. This
* value is only usable until cg_conf_close is called. If a longer scope is necessary the
* user is responsible for making a copy of the value.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an
* invalid parameter or CG_STATUS_ERROR in case of an error.
*/ 
cg_status_t cg_conf_get(cg_config_t *ctx, const char *section, const char *option, const char **value);

/** Add a section to the configuration. If a section with the same name already exists,
* this will return CG_STATUS_ERROR.
* \param[in] ctx The configuration context previously opened with cg_conf_open.
* \param[in] name The name for the new section.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an
* invalid parameter or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_conf_add_section(cg_config_t *ctx, const char *name);

/** Delete a section or option from the configuration.
* \param[in] ctx The configuration context previously opened with cg_conf_open.
* \param[in] section The name of the section you wish to delete.
* \param[in] option The name of the option you wish to delete. Can be left NULL. 
* When left NULL cg_conf_del deletes the entire section. If not NULL, section 
* MUST also be non-NULL.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an
* invalid parameter or CG_STATUS_ERROR in case of an error.
*/ 
cg_status_t cg_conf_del(cg_config_t *ctx, const char *section, const char *option);

/** Get all sections from the configuration file.
* \param[in] ctx The configuration context previously opened with cg_conf_open.
* \param[out] sections The names of the sections in this configuration file. *sections has to be freed by the user.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an
* invalid parameter or CG_STATUS_ERROR in case of an error.
*/ 
cg_status_t cg_conf_get_sections(cg_config_t *ctx, cg_conf_list_t **sections);

/** List all options of the specified section from the configuration file.
* \param[in] ctx The configuration context previously opened with cg_conf_open.
* \param[in] section The section name for which you wish to retrieve the options.
* \param[out] options The names of the options in this section. *options has to be freed by the user.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an
* invalid parameter or CG_STATUS_ERROR in case of an error.
*/ 
cg_status_t cg_conf_get_options(cg_config_t *ctx, const char *section, cg_conf_list_t **options);

/** @} */

CG_END_DECLS

#endif /* __CG_CONF_H__*/
