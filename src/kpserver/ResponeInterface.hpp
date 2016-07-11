#ifndef RESPONE_INTERFACE_HPP
#define RESPONE_INTERFACE_HPP
/*
enum DataMessage {
    DATA_SUCCESS,
    DATA_ERROR,
    DATA_ERROR_OPEN_DB,
    DATA_ERROR_CLOSE_DB,
    DATA_ERROR_CREATE_TB,
    DATA_ERROR_INSERT_DB,
    DATA_ERROR_SELECT_DB,
    DATA_ERROR_UPDATE_DB,
    DATA_ERROR_DELETE_DB,
    DATA_SELECT_EMPTY
};
*/
//std::string;

enum ResponseCode {
    DATA_SUCCESS,
    ACTION_WAIT_ICON,
	ACTION_WAIT_INPUT,
	ACTION_WAIT_TAR,
	ACTION_WAIT_RESPONSE,

    DATA_ERROR = 10,
    DATA_ERROR_OPEN_DB,
    DATA_ERROR_CLOSE_DB,
    DATA_ERROR_CREATE_TB,
    DATA_ERROR_INSERT_DB,
    DATA_ERROR_SELECT_DB = 15,
    DATA_ERROR_UPDATE_DB,
    DATA_ERROR_DELETE_DB,
    DATA_SELECT_EMPTY,
    DATA_ERROR_START_TRANSACTION,
    DATA_ERROR_END_TRANSACTION = 20,
    DATA_IS_EXISTED,

    ERROR_CREATE_JOB_FOLDER = 50,
    ERROR_SUBMIT_JOB,
    ERROR_JOB_DONT_INIT,
    ERROR_STOP_JOB,
    ERROR_JOB_NOT_EXIST,
    ERROR_DIR_NOT_EXIST,
    ERROR_DUPLICATE_PATH_EXCUTE,
    ERROR_FX10_RETURN,
    ERROR_IMAGE_NOT_EXIST,
  ERROR_CREATE_FORWARCER_CONNECTION,

    PERMISSION_DENIED = 100,

    REQUEST_SUCCESS = 150,
    REQUEST_UNKNOW_HOST,
    REQUEST_SOCKET_ERROR,
    REQUEST_CONNECT_ERROR,
    REQUEST_SEND_REQUEST_ERROR,
	REQUEST_RECEIVE_REQUEST_ERROR,
    REQUEST_DOCKER_ERROR,

	FILE_ACTION_SUCCESS = 200,
	FILE_ACTION_ERROR,

	DOCKER_CREATE_CON_SUCCESS = 250,
  DOCKER_CREATE_CON_ERROR,
  SSH_GENKEY_ERR,
  SSH_ADDKEY_ERR,
  SSH_SENDEMAIL_ERR,

  ERROR_GET_TIME_RESOURCE,
  ERROR_GET_JOB_STATUS,
  ERROR_UPDATE_PROVIDER,

  ERROR_TIMEQUOTA_OVER = 300,
  ERROR_KDESK_ACC_EXISTS,
  ERROR_SERVICE_INVALID_NAME = 350,
  ERROR_SERVICE_EXEPATH_NOTFOUND,
  ERROR_SERVICE_SHPATH_NOTFOUND,
  ERROR_SERVICE_SLAVEDAEMON_NOTFOUND,
  ERROR_SERVICE_STAGEINDIR_NOTFOUND,
  ERROR_SERVICE_PRESHPATH_NOTFOUND,
  ERROR_SERVICE_POSTSHPATH_NOTFOUND
 
};

enum TypeMessage {
	Text = 1,
	Binary,
	None,
};

enum ResultCode {
	Success = 1,
	Fail,
	UnknowFormat,
	ReadyReceiveFile,
};

typedef struct {
	TypeMessage typeMes;
	unsigned long sizeMes;
	std::string content;
} MessageStruct;

#endif
