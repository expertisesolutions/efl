#ifndef _EFL_NET_H
#define _EFL_NET_H

#include <time.h>
#include <libgen.h>
#ifdef _WIN32
# include <ws2tcpip.h>
#else
# include <netdb.h>
#endif

#define __EFL_NET_IS_REQUIRED

#include <Efl_Core.h>

#include <ecore_con_api.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes the Ecore_Con library.
 * @return  Number of times the library has been initialised without being
 *          shut down.
 *
 * @note This function already calls ecore_init() internally, so you don't need
 * to call it explicitly.
 */
ECORE_CON_API int               ecore_con_init(void);

/**
 * @brief Shuts down the Ecore_Con library.
 * @return  Number of times the library has been initialised without being
 *          shut down.
 * @note This function already calls ecore_shutdown() internally, so you don't
 * need to call it explicitly unless you called ecore_init() explicitly too.
 */
ECORE_CON_API int               ecore_con_shutdown(void);

/**
 * @brief Initializes the Ecore_Con_Url library.
 * @return Number of times the library has been initialised without being
 *          shut down.
 *
 * @note This function doesn't call ecore_con_init(). You still need to call it
 * explicitly before calling this one.
 */
ECORE_CON_API int               ecore_con_url_init(void);

/**
 * @brief Shuts down the Ecore_Con_Url library.
 * @return  Number of calls that still uses Ecore_Con_Url
 *
 * @note This function doesn't call ecore_con_shutdown(). You still need to call
 * it explicitly after calling this one.
 */
ECORE_CON_API int               ecore_con_url_shutdown(void);

#ifdef EFL_BETA_API_SUPPORT
/** HTTP error: bad content encoding */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_BAD_CONTENT_ENCODING;

/** HTTP error: bad download resume */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_BAD_DOWNLOAD_RESUME;

/** HTTP error: bad function argument */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_BAD_FUNCTION_ARGUMENT;

/** HTTP error: chunk failed */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_CHUNK_FAILED;

/** HTTP error: conv failed */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_CONV_FAILED;

/** HTTP error: conv reqd */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_CONV_REQD;

/** HTTP error: failed init */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_FAILED_INIT;

/** HTTP error: could not read file */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_FILE_COULDNT_READ_FILE;

/** HTTP error: filesize exceeded */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_FILESIZE_EXCEEDED;

/** HTTP error: function not found */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_FUNCTION_NOT_FOUND;

/** HTTP error: got nothing */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_GOT_NOTHING;

/** HTTP error: http2 */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_HTTP2;

/** HTTP error: http2 stream */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_HTTP2_STREAM;

/** HTTP error: http post error */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_HTTP_POST_ERROR;

/** HTTP error: http returned error */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_HTTP_RETURNED_ERROR;

/** HTTP error: interface failed */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_INTERFACE_FAILED;

/** HTTP error: login denied */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_LOGIN_DENIED;

/** HTTP error: no connection available */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_NO_CONNECTION_AVAILABLE;

/** HTTP error: not built in */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_NOT_BUILT_IN;

/** HTTP error: operation timeout */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_OPERATION_TIMEDOUT;

/** HTTP error: partial file */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_PARTIAL_FILE;

/** HTTP error: peer failed verification */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_PEER_FAILED_VERIFICATION;

/** HTTP error: range error */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_RANGE_ERROR;

/** HTTP error: read error */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_READ_ERROR;

/** HTTP error: receive error */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_RECV_ERROR;

/** HTTP error: remote access denied */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_REMOTE_ACCESS_DENIED;

/** HTTP error: remote disk full */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_REMOTE_DISK_FULL;

/** HTTP error: remote file exists */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_REMOTE_FILE_EXISTS;

/** HTTP error: remote file not found */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_REMOTE_FILE_NOT_FOUND;

/** HTTP error: send error */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_SEND_ERROR;

/** HTTP error: send fail rewind */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_SEND_FAIL_REWIND;

/** HTTP error: SSL cacert */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_SSL_CACERT;

/** HTTP error: SSL cacert bad file */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_SSL_CACERT_BADFILE;

/** HTTP error: SSL certproblem */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_SSL_CERTPROBLEM;

/** HTTP error: SSL cipher */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_SSL_CIPHER;

/** HTTP error: SSL connect error */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_SSL_CONNECT_ERROR;

/** HTTP error: SSL crl bad file */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_SSL_CRL_BADFILE;

/** HTTP error: SSL engine init failed */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_SSL_ENGINE_INITFAILED;

/** HTTP error: SSL engine not found */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_SSL_ENGINE_NOTFOUND;

/** HTTP error: SSL engine set failed */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_SSL_ENGINE_SETFAILED;

/** HTTP error: SSL invalid cert status */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_SSL_INVALIDCERTSTATUS;

/** HTTP error: SSL issuer error */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_SSL_ISSUER_ERROR;

/** HTTP error: SSL pinned pub key does not match */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_SSL_PINNEDPUBKEYNOTMATCH;

/** HTTP error: SSL shutdown failed */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_SSL_SHUTDOWN_FAILED;

/** HTTP error: too many redirects */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_TOO_MANY_REDIRECTS;

/** HTTP error: unknown option */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_UNKNOWN_OPTION;

/** HTTP error: unsupported protocol */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_UNSUPPORTED_PROTOCOL;

/** HTTP error: upload failed */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_UPLOAD_FAILED;

/** HTTP error: URL mal-formatted */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_URL_MALFORMAT;

/** HTTP error: usage of SSL failed */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_USE_SSL_FAILED;

/** HTTP error: write error */
extern ECORE_CON_API ECORE_CON_WEAK_API Eina_Error EFL_NET_HTTP_ERROR_WRITE_ERROR;
#endif /* EFL_BETA_API_SUPPORT */

#include "efl_net_types.eot.h"

#include "efl_net_ip_address.eo.h"

#include "efl_net_socket.eo.h"
#include "efl_net_dialer.eo.h"
#include "efl_net_server.eo.h"

#include "efl_net_socket_simple.eo.h"
#include "efl_net_dialer_simple.eo.h"
#include "efl_net_server_simple.eo.h"

#include "efl_net_socket_fd.eo.h"
#include "efl_net_server_fd.eo.h"

#include "efl_net_server_ip.eo.h"

#include "efl_net_socket_tcp.eo.h"
#include "efl_net_dialer_tcp.eo.h"
#include "efl_net_server_tcp.eo.h"

#ifdef _WIN32
#include "efl_net_socket_windows.eo.h"
#include "efl_net_dialer_windows.eo.h"
#include "efl_net_server_windows.eo.h"
#else
#include "efl_net_socket_unix.eo.h"
#include "efl_net_dialer_unix.eo.h"
#include "efl_net_server_unix.eo.h"
#endif

#include "efl_net_socket_udp.eo.h"
#include "efl_net_dialer_udp.eo.h"
#include "efl_net_server_udp.eo.h"
#include "efl_net_server_udp_client.eo.h"

#include "efl_net_http_types.eot.h"

#include "efl_net_dialer_http.eo.h"
#include "efl_net_dialer_websocket.eo.h"

#include "efl_net_ssl_types.eot.h"

#include "efl_net_ssl_context.eo.h"
#include "efl_net_socket_ssl.eo.h"
#include "efl_net_dialer_ssl.eo.h"
#include "efl_net_server_ssl.eo.h"

#include "efl_net_control_technology.eo.h"
#include "efl_net_control_access_point.eo.h"
#include "efl_net_control_manager.eo.h"
#include "efl_net_session.eo.h"

#ifdef __cplusplus
}
#endif

#endif
