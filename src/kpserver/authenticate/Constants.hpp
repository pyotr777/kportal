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


#define TAG_NUMBER_OF_NODE          "[NUMBER_OF_NODE]"
#define TAG_MAX_ELAPSE_TIME         "[MAX_ELAPSE_TIME]"
#define TAG_EXE_FILE                "[EXE_FILE]"
#define TAG_STAGE_IN_DIR            "[STAGE_IN_DIR]"
#define TAG_STAGE_OUT_DIR           "[STAGE_OUT_DIR]"
#define TAG_STAGE_EXE_ARGS          "[EXE_ARGS]"

const unsigned int USF_NAME      = 0x01; // hex for 0000 0001
const unsigned int USF_EXEPATH   = 0x02; // hex for 0000 0010
const unsigned int USF_SHPATH    = 0x04; // hex for 0000 0100
const unsigned int USF_IMAGE     = 0x08; // hex for 0000 1000
const unsigned int USF_STGINDIR  = 0x10; // hex for 0001 0000
const unsigned int USF_STGOUTDIR = 0x20; // hex for 0010 0000
const unsigned int USF_NONODE    = 0x40; // hex for 0100 0000
const unsigned int USF_ETIME     = 0x80; // hex for 1000 0000
const unsigned int USF_PRESHPATH = 0x100;    // hex for 0000 0001 0000 0000
const unsigned int USF_POSTSHPATH= 0x200;    // hex for 0000 0010 0000 0000
const unsigned int USF_ISNEW     = 0xFFFF;   // hex for 1111 1111 1111 1111
#endif // KP_SERVER_CONSTANTS_HPP
