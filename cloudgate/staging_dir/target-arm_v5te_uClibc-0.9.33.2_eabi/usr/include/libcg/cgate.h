#ifndef __CGATE_H_
#define __CGATE_H_

#include <stdint.h>

/** \file cgate.h
*	\brief Main SDK header file
*/

/* Guard C code in headers, while including them from C++ */
#ifdef  __cplusplus
# define CG_BEGIN_DECLS  extern "C" {
# define CG_END_DECLS    }
#else
# define CG_BEGIN_DECLS
# define CG_END_DECLS
#endif

/* Non-prefixed definitions for some commonly used macros.
 * These are only provided if they haven't already
 * been defined. It is assumed that if they are already
 * defined then the current definition is correct.
 */

/* Commonly used NULL */
#ifndef NULL
#  ifdef __cplusplus
#    define NULL        (0L)
#  else /* !__cplusplus */
#    define NULL        ((void*) 0)
#  endif /* !__cplusplus */
#endif

/* These can be used with cg_bool */
#ifndef FALSE
#define FALSE   (0)
#endif

#ifndef TRUE
#define TRUE    (!FALSE)
#endif

#if     __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ > 4)
#define CG_GNUC_PRINTF( format_idx, arg_idx )    \
  __attribute__((__format__ (__printf__, format_idx, arg_idx)))
#else   /* !__GNUC__ */
#define CG_GNUC_PRINTF( format_idx, arg_idx )
#endif  /* !__GNUC__ */


CG_BEGIN_DECLS


/**
* \enum cg_status_t
* \brief List of possible return statuses used throughout the CloudGate SDK
*/
typedef enum {
	/** The SDK function completed successfully. */
	CG_STATUS_OK = 0,
	/** The SDK function completed with an error. */
	CG_STATUS_ERROR,
	/**
	 * The SDK function could not be completed because a device was busy.
	 * I.e. a device is too cold (and is heating up) or too hot (and is cooling down).
	 */
	CG_STATUS_RESOURCE_BUSY,
	/**
	 * The SDK function could not be completed because an invalid parameter was provided.
	 * I.e. a NULL pointer, wrong device name, ...
	 */
	CG_STATUS_INVALID_PARAMETER,
	/** The SDK function could not be completed because a device or resource is temporary unavailable. */
	CG_STATUS_RESOURCE_UNAVAILABLE,
 	/** The SDK function could not be completed because of device downgrade prevention. */   
  CG_STATUS_DOWNGRADE_PREVENTION,
} cg_status_t;


/** Get a description of the last error encountered by the SDK
* \return A string describing the error
*/
char *cg_get_last_error(void);

CG_END_DECLS

#endif /* __CGATE_H_ */
