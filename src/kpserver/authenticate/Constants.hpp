#ifndef KP_SERVER_CONSTANTS_HPP
#define KP_SERVER_CONSTANTS_HPP

#define DEFAULT_REQUEST_ID_STR          "0"
#define DEFAULT_JOB_ID_STR              "0"
#define DEFAULT_PROVIDER_ID_STR         "0"

/// EH0004
#define ERROR_FILE              -10
#define ERR_CREATE_FILE         -11
#define ERR_OPEN_FILE           -12
#define ERR_WRITE_FILE          -13
/// EH0004 end

/*
 * Header binary data xxx byte
 */
//#define NUM_OF_BYTES_HEADER         16
#define NOB_REQUEST_ID              8   /// Number of requestId byte
#define NOB_SENT_BYTE               4   /// Number of sent byte
#define NOB_TOTAL_BYTE              4   /// Number of total byte
#define NOB_PARAM_ID_BYTE           4   /// Number of paramid



#endif // KP_SERVER_CONSTANTS_HPP
