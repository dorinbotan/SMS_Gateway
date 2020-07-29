#ifndef MBIM_PHONEBOOK_H
#define MBIM_PHONEBOOK_H

/**
 * \file mbim_phonebook.h
 * \brief Public header file which contains all command id functionality for the
 * phonebook
 */

/**
 * \defgroup Phonebook functions (PUBLIC)
 * @{
 */

/**
 * \enum mbim_phonebook_state_t
 * \brief States of the phonebook.
 */
typedef enum {
	MBIM_PHONEBOOK_NOT_INITIALIZED,
	MBIM_PHONEBOOK_INITIALIZED
} mbim_phonebook_state_t;

/**
 * \struct mbim_phonebook_configuration_info_t
 * \brief The struct used in query/notification command for MBIM_CID_PHONEBOOK_CONFIGURATION.
 */
typedef struct __attribute__ ((packed, aligned(4))) {
	/** The current state of the phonebook. */
	uint32_t phonebook_state;
	/** Total number of entries in the phonebook */
	uint32_t total_nbr_of_entries;
	/** Number of phonebook entries that are currently used */
	uint32_t used_entries;
	/** Maximum number of bytes for the phone number of a phonebook entry */
	uint32_t max_number_length;
	/** Maximum number of bytes for the name of a phonebook entry */
	uint32_t max_name_length;
} mbim_phonebook_configuration_info_t;

/**
 * Request the phonebook configuration info.
 * \param[in] ctx pointer to the context of the mbim device
 * \param[in] cb user callback to be called when phonebook configuration info is received
 * \param[in] arg extra arguments.
 */
uint32_t mbim_get_phonebook_configuration_info(mbim_context_t *ctx, mbim_user_data_cb cb, void *arg);

/** @} */

#endif
