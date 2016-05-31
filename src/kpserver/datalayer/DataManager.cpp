#include "../stdafx.h"
#include "DataManager.hpp"
#define VIEW_TABLE "view table"
#define CHECK_TABLE_EXIST "check table exist"
extern std::map<std::string,ImageDocker> SampleImages;

DataManager::DataManager() {
    dbFilePath = "";
    db = NULL;
    isConnect = false;
    //changeLock = false;
}

DataManager::DataManager(std::string pathFile) {
    dbFilePath = pathFile;
    db = NULL;
    isConnect = false;
    //changeLock = false;
}

DataManager::~DataManager() {
    if (isConnect) {
        if (db != NULL) {
            sqlite3_close(db);
            std::cout << "Disconnect database successfully\n" ;
			db = NULL;
        }
        isConnect = false;
    }
}

void DataManager::setDBPath(std::string pathFile) {
    dbFilePath = pathFile;
}

static int callback(void *data, int argc, char **argv, char **azColName){
    //if (strcmp((char*)data, VIEW_TABLE) == 0) {
        //numberOfResult += 1;
    /*
        int i;
        fprintf(stderr, "%s: ", (const char*)data);
        for(i=0; i<argc; i++){
            printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        }
        printf("\n");
    */
    //} else if (strcmp((char*)data, CHECK_TABLE_EXIST) == 0) {
        //numberOfResult += 1;
    //}
    return 0;
}

ResponseCode DataManager::createDB() {
    char *zErrMsg = 0;
    int  rc;
    char *sql;

    /* Open database */
    rc = sqlite3_open(dbFilePath.c_str(), &db);
    if( rc ){
       fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
       return DATA_ERROR_OPEN_DB;
    }else{
       fprintf(stdout, "Opened database successfully\n");
    }

    /* Create user table */
    sql = "CREATE TABLE USER("  \
        "EMAIL          NVARCHAR(100) PRIMARY KEY   NOT NULL," \
        "CONTAINER_ID   NVARCHAR(100)               NOT NULL," \
        "PHONE		      NVARCHAR(100)               NOT NULL," \
        "ADDRESS		    NVARCHAR(100)               NOT NULL," \
        "KDESKACC		    NVARCHAR(100)               NOT NULL," \
        "DESCRIPTION	  TEXT	        					    NOT NULL," \
        "TYPE           INT                         NOT NULL," \
        "TIMEUSAGE      REAL                        NOT NULL," \
        "TIMEUSABLE     REAL                        NOT NULL," \
        "PUBLICKEY	    NVARCHAR(2000)              NOT NULL);";
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        return DATA_ERROR_CREATE_TB;
    } else {
        fprintf(stdout, "Table user created successfully\n");
    }

    /* Create service table */
    sql = "CREATE TABLE SERVICE("  \
            "SERVICE_ID             NVARCHAR(100) PRIMARY KEY       NOT NULL," \
            "SERVICE_NAME           NVARCHAR(100)                   NOT NULL," \
            "PRODUCER               NVARCHAR(50)                    NOT NULL," \
            "STATUS                 BOOLEAN                         NOT NULL," \
            "NUMBER_OF_USER         INT                             NOT NULL," \
            "NUMBER_OF_WAIT_USER    INT                             NOT NULL," \
            "DESCRIPTION            TEXT                            NOT NULL," \
            "PATH_EXCUTE            NVARCHAR(255)                   NOT NULL," \
            "PATH_SH                NVARCHAR(255)                   NOT NULL," \
            "SH_TEMPLATE            TEXT                            NOT NULL," \
            "STAGE_IN_DIRS          NVARCHAR(1255)                  NOT NULL," \
            "STAGE_OUT_DIRS         NVARCHAR(1255)                  NOT NULL," \
            "IMAGE_ID               NVARCHAR(100)                   NOT NULL," \
            "EMAIL_PROVIDER         NVARCHAR(100)                   NOT NULL," \
            "NUMBER_OF_NODE         INT                             NOT NULL," \
            "MAX_ELAPSE_TIME        INT                             NOT NULL," \
            "FOREIGN KEY(EMAIL_PROVIDER) REFERENCES USER(EMAIL) ON DELETE CASCADE);";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        //fprintf(stderr, "SQL error: %s\n sql command: %s\n", zErrMsg, s);
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        return DATA_ERROR_CREATE_TB;
    } else {
        fprintf(stdout, "Table service created successfully\n");
    }

    /* Create parameter table */
    sql = "CREATE TABLE PARAMETER("  \
            "PARAM_ID       CHAR(20)        NOT NULL," \
            "SERVICE_ID     NVARCHAR(100)   NOT NULL," \
            "IS_OUTPUT      BOOLEAN         NOT NULL," \
            "OPTION_NAME    CHAR(50)        NOT NULL," \
            "IS_REQUIRED    BOOLEAN         NOT NULL," \
            "DESCRIPTION    TEXT            NOT NULL," \
            "TYPE           CHAR(20)        NOT NULL," \
            "NAME           NVARCHAR(50)    NOT NULL," \
            "DEFAULT_VALUE  NVARCHAR(100)   NOT NULL," \
            "PRIMARY KEY (PARAM_ID, SERVICE_ID)," \
            "FOREIGN KEY(SERVICE_ID) REFERENCES SERVICE(SERVICE_ID) ON DELETE CASCADE);";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        return DATA_ERROR_CREATE_TB;
    } else {
        fprintf(stdout, "Table parameter created successfully\n");
    }

    /* Create user_service table */
    sql = "CREATE TABLE USER_SERVICE("  \
            "USER_EMAIL     NVARCHAR(100)   NOT NULL," \
            "SERVICE_ID     NVARCHAR(100)   NOT NULL," \
            "STATUS_USER    INT             NOT NULL," \
            "PRIMARY KEY (USER_EMAIL, SERVICE_ID)," \
            "FOREIGN KEY (SERVICE_ID) REFERENCES SERVICE(SERVICE_ID) ON DELETE CASCADE);";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        return DATA_ERROR_CREATE_TB;
    } else {
        fprintf(stdout, "Table parameter created successfully\n");
    }

    /* Create images table */
    sql = "CREATE TABLE IMAGES("  \
            "IMAGE_ID       NVARCHAR(100)   PRIMARY KEY NOT NULL," \
            "IMAGE_NAME     NVARCHAR(100)                       ," \
            "PROVIDER_EMAIL NVARCHAR(100)               NOT NULL," \
            "IS_PUBLIC      INT                         NOT NULL," \
            "COMMENT        NVARCHAR(1000)                      ," \
            "FOREIGN KEY (PROVIDER_EMAIL) REFERENCES USER(EMAIL) ON DELETE CASCADE);";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        return DATA_ERROR_CREATE_TB;
    } else {
        fprintf(stdout, "Table parameter created successfully\n");
    }

    //create trigger after insert
    std::stringstream ss;
    ss << "CREATE  TRIGGER insert_user AFTER INSERT ON USER_SERVICE"
            << " BEGIN "
            << " UPDATE SERVICE SET NUMBER_OF_USER=("
            << " SELECT COUNT(*) FROM USER_SERVICE WHERE SERVICE_ID=NEW.SERVICE_ID AND STATUS_USER=" << READY << "),"
            << " NUMBER_OF_WAIT_USER=("
            << " SELECT COUNT(*) FROM USER_SERVICE WHERE SERVICE_ID=NEW.SERVICE_ID AND STATUS_USER=" << WAITING << ")"
            << " WHERE SERVICE_ID=NEW.SERVICE_ID;"
            << " END;";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, ss.str().c_str(), callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        return DATA_ERROR_CREATE_TB;
    } else {
        fprintf(stdout, "Trigger after insert USER_SERVICE created successfully\n");
    }

    //create trigger after update
	//table USER_SERVICE
	ss.str("");
    ss << "CREATE  TRIGGER update_user AFTER UPDATE ON USER_SERVICE"
            << " BEGIN "
            << " UPDATE SERVICE SET NUMBER_OF_USER=("
            << " SELECT COUNT(*) FROM USER_SERVICE WHERE SERVICE_ID=NEW.SERVICE_ID AND STATUS_USER=" << READY << "),"
            << " NUMBER_OF_WAIT_USER=("
            << " SELECT COUNT(*) FROM USER_SERVICE WHERE SERVICE_ID=NEW.SERVICE_ID AND STATUS_USER=" << WAITING << ")"
            << " WHERE SERVICE_ID=NEW.SERVICE_ID;"
            << " END;";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, ss.str().c_str(), callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        return DATA_ERROR_CREATE_TB;
    } else {
        fprintf(stdout, "Trigger after update USER_SERVICE created successfully\n");
    }

	//table USER
	ss.str("");
	ss << "CREATE  TRIGGER update_provider AFTER UPDATE ON USER"
			<< " WHEN (OLD.TYPE = " << (int)PROVIDER_GROUP << " OR OLD.TYPE = " << (int)(PROVIDER_GROUP | ADMIN_GROUP)
			<< " OR OLD.TYPE = " << (int)(PROVIDER_GROUP | USER_GROUP)
			<< " OR OLD.TYPE = " << (int)(PROVIDER_GROUP | ADMIN_GROUP | USER_GROUP)
			<< ") "
			<< " AND"
			<< " (NEW.TYPE <> " << (int)PROVIDER_GROUP << " AND NEW.TYPE <> " << (int)(PROVIDER_GROUP | ADMIN_GROUP)
			<< " AND NEW.TYPE <> " << (int)(PROVIDER_GROUP | USER_GROUP)
			<< " AND NEW.TYPE <> " << (int)(PROVIDER_GROUP | ADMIN_GROUP | USER_GROUP)
			<< ")"
			<< " BEGIN "
            << " DELETE FROM SERVICE WHERE (EMAIL_PROVIDER=NEW.EMAIL);"
            << " DELETE FROM IMAGES WHERE (PROVIDER_EMAIL=NEW.EMAIL);"
			<< " END;";
	std::cout << "Trigger delete service when provider is deleted : \n" << ss.str();

	/* Execute SQL statement */
	rc = sqlite3_exec(db, ss.str().c_str(), callback, 0, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		sqlite3_close(db);
		return DATA_ERROR_CREATE_TB;
	} else {
		fprintf(stdout, "Trigger after update USER_SERVICE created successfully\n");
	}

    //create trigger after delete
    ss.str("");;
    ss << "CREATE  TRIGGER delete_user AFTER DELETE ON USER_SERVICE"
            << " BEGIN "
            << " UPDATE SERVICE SET NUMBER_OF_USER=("
            << " SELECT COUNT(*) FROM USER_SERVICE WHERE SERVICE_ID=OLD.SERVICE_ID AND STATUS_USER=" << READY << "),"
            << " NUMBER_OF_WAIT_USER=("
            << " SELECT COUNT(*) FROM USER_SERVICE WHERE SERVICE_ID=OLD.SERVICE_ID AND STATUS_USER=" << WAITING << ")"
            << " WHERE SERVICE_ID=OLD.SERVICE_ID;"
            << " END;";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, ss.str().c_str(), callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        return DATA_ERROR_CREATE_TB;
    } else {
        fprintf(stdout, "Trigger after delete USER_SERVICE created successfully\n");
    }
    sqlite3_close(db);
    return DATA_SUCCESS;
}

bool    DataManager::dbIsExist() {
  std::cout << "File .db full name: " << dbFilePath << std::endl;
    if( !dbFilePath.empty() )
    {
        if( access(dbFilePath.c_str(), 0) == 0 )
        {
           struct stat status;
           stat( dbFilePath.c_str(), &status );
           if( status.st_mode & S_IFREG ) {
           } else {
               return false;
           }

        } else {
            return false;
        }
    } else {
        return false;
    }

    int rc;
    rc = sqlite3_open(dbFilePath.c_str(), &db);
    if( rc ){
        return false;
    }

    char* sql = "SELECT name FROM sqlite_master WHERE type='table' AND name='USER';";

    sqlite3_stmt *statement;
    if ( sqlite3_prepare(db, sql, -1, &statement, 0 ) == SQLITE_OK )
    {
        int res = 0;
        res = sqlite3_step(statement);
        if ( res != SQLITE_ROW ) {
            sqlite3_finalize(statement);
            sqlite3_close(db);
            return false;
        }
    } else {
        sqlite3_close(db);
        return false;
    }
    sqlite3_finalize(statement);
    sqlite3_close(db);

    return true;
}

ResponseCode DataManager::connectDB() {
    if (!dbIsExist()) {
        std::cout << "Start create DB\n";
        ResponseCode ret = createDB();
        if (ret != DATA_SUCCESS) {
            return ret;
        }
    }

    int rc;

    rc = sqlite3_open(dbFilePath.c_str(), &db);

    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return DATA_ERROR_OPEN_DB;
    } else {
        fprintf(stderr, "Opened database successfully\n");
    }
/*
    std::string s = "PRAGMA foreign_keys = ON;";
    char *zErrMsg = 0;
    rc = sqlite3_exec(db, s.c_str(), callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
    }
*/
    isConnect = true;
    return DATA_SUCCESS;
}

ResponseCode DataManager::disConnectDB() {
    if (isConnect) {
        if (db != NULL) {
            sqlite3_close(db);
			db = NULL;
      printf("Disconnect database successfully\n") ;
        }
        isConnect = false;
    }
    return DATA_SUCCESS;
}


//Control
ResponseCode DataManager::beginTransaction() {
    std::stringstream strm;
    strm << "BEGIN TRANSACTION;";

    std::string s = strm.str();
    char *str = &s[0];

    char *zErrMsg = 0;
    int rc = sqlite3_exec(db, str, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);

        return DATA_ERROR_START_TRANSACTION;
    } else {
        std::cout << "Start transaction success.\n";
    }
    return DATA_SUCCESS;

}

ResponseCode DataManager::endTransaction() {
    std::stringstream strm;
    strm << "END TRANSACTION;";

    std::string s = strm.str();
    char *str = &s[0];

    char *zErrMsg = 0;
    int rc = sqlite3_exec(db, str, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        return DATA_ERROR_END_TRANSACTION;
    } else {
        std::cout << "End transaction success.\n";
    }
    return DATA_SUCCESS;

}

ResponseCode DataManager::rollback() {
    std::stringstream strm;
    strm << "ROLLBACK;";

    std::string s = strm.str();
    char *str = &s[0];

    char *zErrMsg = 0;
    int rc = sqlite3_exec(db, str, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        return DATA_ERROR_END_TRANSACTION;
    } else {
        std::cout << "Rollback success.\n";
    }
    return DATA_SUCCESS;
}

ResponseCode DataManager::insertUserDb(User &user) {
	/*
	"EMAIL          NVARCHAR(100) PRIMARY KEY   NOT NULL," \
	"CONTAINER_ID   NVARCHAR(100)               NOT NULL," \
  "PHONE			NVARCHAR(100)               NOT NULL," \
  "ADDRESS		NVARCHAR(100)               NOT NULL," \
  "KDESKACC		NVARCHAR(100)               NOT NULL," \
	"DESCRIPTION	TEXT						NOT NULL," \
  "TYPE           INT                         NOT NULL
  "TIMEUSAGE      REAL                        NOT NULL," \
  "TIMEUSABLE     REAL                        NOT NULL," \
  "PUBLICKEY	    NVARCHAR(2000)              NOT NULL);";
	*/
    std::stringstream strm;
  strm << "insert into USER(EMAIL,CONTAINER_ID,PHONE,ADDRESS,KDESKACC,DESCRIPTION,TYPE,TIMEUSAGE,TIMEUSABLE,PUBLICKEY) values('"
       << user.getEmail()
       << "','" << user.getContainerId()
       << "','" << user.getPhone()
       << "','" << user.getAddress()
       << "','" << user.getKdeskAcc()
       << "','" << user.getDescription()
       << "'," << (int)user.getType()
       << "," <<  user.getTimeUsage()//user.getKdeskPort()
       << "," << user.getTimeUsable()
       << ",'" << user.getPublicKey()
       << "');";
    std::string s = strm.str();
	std::cout << "query insert user: " << s << std::endl;
    char *str = &s[0];

    char *zErrMsg = 0;
    int rc = sqlite3_exec(db, str, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        return DATA_ERROR_INSERT_DB;
    } else {
        std::cout << "Insert user: " << user.getEmail() << " success \n";
    }
    return DATA_SUCCESS;
}

ResponseCode DataManager::updateUserDb(User &user) {
	/*
	"EMAIL          NVARCHAR(100) PRIMARY KEY   NOT NULL," \
	"CONTAINER_ID   NVARCHAR(100)               NOT NULL," \
	"PHONE			NVARCHAR(100)               NOT NULL," \
  "ADDRESS		NVARCHAR(100)               NOT NULL," \
  "KDESKACC		NVARCHAR(100)               NOT NULL," \
	"DESCRIPTION	TEXT						NOT NULL," \
  "TYPE           INT                         NOT NULL," \
  "TIMEUSAGE      REAL                        NOT NULL," \
  "TIMEUSABLE     REAL                        NOT NULL," \
  "PUBLICKEY	    NVARCHAR(2000)              NOT NULL);";
	*/
    std::stringstream strm;
    strm << "UPDATE USER SET CONTAINER_ID='" << user.getContainerId()
		 << "', PHONE='" << user.getPhone()
		 << "', ADDRESS='" << user.getAddress()
     << "', KDESKACC='" << user.getKdeskAcc()
     << "', DESCRIPTION='" << user.getDescription()
     << "', TYPE=" << (int)user.getType()
     //<< ", KDESKPORT=" << user.getKdeskPort()
     << ", TIMEUSAGE=" << user.getTimeUsage()
     << ", TIMEUSABLE=" << user.getTimeUsable()
     << ", PUBLICKEY='" << user.getPublicKey()
     << "' WHERE EMAIL='" << user.getEmail() << "';";
    std::string s = strm.str();
	std::cout << "query database: " << s << std::endl;
    char *str = &s[0];

    char *zErrMsg = 0;
    int rc = sqlite3_exec(db, str, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        return DATA_ERROR_UPDATE_DB;
    } else {
		std::cout << "Update user: " << user.getEmail() << " success \n";
    }
    return DATA_SUCCESS;
}

ResponseCode DataManager::checkKdeskAccExists(std::string email, std::string kdeskacc, bool& isExists) {
    std::stringstream strm;
    strm << "SELECT * FROM USER WHERE KDESKACC='" << kdeskacc << "' AND EMAIL <> '" << email << "';";

    std::string s = strm.str();
    std::cout << "Query: " << s << std::endl;
    char *str = &s[0];

    sqlite3_stmt *statement;
    isExists = false;
    char *query = str;
    if ( sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK )
    {
        int res = 0;

        while ( 1 )
        {
            res = sqlite3_step(statement);

            if ( res == SQLITE_ROW )
            {
                isExists = true;
                break;
            }

            if ( res == SQLITE_DONE || res==SQLITE_ERROR)
            {
                std::cout << "Check path excute file finish " << std::endl;
                break;
            }
        }
        sqlite3_finalize(statement);
    } else {
        return DATA_ERROR_SELECT_DB;
    }
    return DATA_SUCCESS;
}


ResponseCode DataManager::deleteUserDb(User &user) {
	/*
	"EMAIL          NVARCHAR(100) PRIMARY KEY   NOT NULL," \
	"CONTAINER_ID   NVARCHAR(100)               NOT NULL," \
	"PHONE			NVARCHAR(100)               NOT NULL," \
	"ADDRESS		NVARCHAR(100)               NOT NULL," \
  "KDESKACC		NVARCHAR(100)               NOT NULL," \
  "DESCRIPTION	TEXT						NOT NULL," \
  "TYPE           INT                         NOT NULL," \
  "TIMEUSAGE      REAL                        NOT NULL," \
  "TIMEUSABLE     REAL                        NOT NULL," \
  "PUBLICKEY	    NVARCHAR(2000)              NOT NULL);
	*/
    std::stringstream strm;
    strm << "DELETE FROM USER WHERE EMAIL='" << user.getEmail() << "';";

    std::string s = strm.str();
    char *str = &s[0];

    char *zErrMsg = 0;
    int rc = sqlite3_exec(db, str, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        return DATA_ERROR_DELETE_DB;
    } else {
		std::cout << "Delete user: " << user.getEmail() << " success \n";
    }
    return DATA_SUCCESS;
}

ResponseCode DataManager::getUserDb(User &user) {
    std::stringstream strm;
    strm << "SELECT * FROM USER WHERE EMAIL='" << user.getEmail() << "';";

    std::string s = strm.str();
    std::cout << "query : " << s << std::endl;
    char *str = &s[0];

    sqlite3_stmt *statement;

    char *query = str;
    if ( sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK )
    {
        int res = 0;

        res = sqlite3_step(statement);
        if ( res == SQLITE_ROW )
        {
            user.setEmail((char*)sqlite3_column_text(statement, 0));
			std::cout << "email = " << user.getEmail() << std::endl;
            user.setContainerId((char*)sqlite3_column_text(statement, 1));
      user.setPhone((char*)sqlite3_column_text(statement, 2));
      user.setAddress((char*)sqlite3_column_text(statement, 3));
      user.setKdeskAcc((char*)sqlite3_column_text(statement, 4));
      user.setDescription((char*)sqlite3_column_text(statement, 5));
      user.setType((BYTE)sqlite3_column_int(statement, 6));
      //user.setKdeskPort(sqlite3_column_int(statement, 7));
      user.setTimeUsage(sqlite3_column_double(statement, 7));
      std::cout << "in object value: " << user.getTimeUsage() << " vs orgin value : " << sqlite3_column_double(statement, 7) << endl;
      user.setTimeUsable(sqlite3_column_double(statement, 8));
      user.setPublicKey((char*)sqlite3_column_text(statement, 9));
			std::cout << "user type = " << (int)user.getType() << std::endl;
        } else {
			std::cout << "user " << user.getEmail() << " is not exist\n";
            return DATA_SELECT_EMPTY;
        }

        sqlite3_finalize(statement);
    } else {
        return DATA_ERROR_SELECT_DB;
    }
    return DATA_SUCCESS;
}

ResponseCode DataManager::insertUser(User &user) {
	User temp(user.getEmail());
	ResponseCode res = getUserDb(temp);
    if (res == DATA_SELECT_EMPTY) {
		res = insertUserDb(user);
	} else if (res == DATA_SUCCESS) {
		BYTE check = (temp.getType() & user.getType());
		if (check == user.getType()) {
            std::cout << "User is existed\n";
			res = DATA_IS_EXISTED;
        } else {
			BYTE old_type = temp.getType();
			BYTE new_type = user.getType();
			if ((old_type & WAIT_PROVIDER_GROUP) && (new_type & PROVIDER_GROUP)) {
                new_type = (old_type - WAIT_PROVIDER_GROUP) | new_type;
            } else {
                new_type = old_type | new_type;
            }

			temp.setType(new_type);
			std::cout << "new_type = " << (int) temp.getType() << std::endl;
			if (user.getPhone().size() > 0) {
				temp.setPhone(user.getPhone());
      }
      if (user.getAddress().size() > 0) {
        temp.setAddress(user.getAddress());
      }
      if (user.getKdeskAcc().size() > 0) {
        temp.setKdeskAcc(user.getKdeskAcc());
      }
			if (user.getDescription().size() > 0) {
				temp.setDescription(user.getDescription());
			}
      if (user.getType() & USER_GROUP) {
        temp.setContainerId(user.getContainerId());
      }
//      std::cout << "desk port :" << user.getKdeskPort() << std::endl;
//      if (user.getKdeskPort() > 0) {
//        temp.setKdeskPort(user.getKdeskPort());
//      }
      std::cout << "public key :" << user.getPublicKey() << std::endl;
      if (user.getPublicKey().size() > 0) {
        temp.setPublicKey(user.getPublicKey());
      }
      if (user.getTimeUsage() > 0) {
        temp.setTimeUsage(user.getTimeUsage());
      }
      if (user.getTimeUsable() > 0) {
        temp.setTimeUsable(user.getTimeUsable());
      }
      res = updateUserDb(temp);
    }
    }
    return res;
}

ResponseCode DataManager::updateUser(User &user) {
    return updateUserDb(user);
}

ResponseCode DataManager::deleteUser(User &user) {
	User temp(user.getEmail());
    ResponseCode res = getUserDb(temp);
    if (res == DATA_SELECT_EMPTY) {
        res = DATA_ERROR_DELETE_DB;
    } else if (res == DATA_SUCCESS) {
        if ((temp.getType() & user.getType()) != user.getType()) {
            std::cout << "User is not existed\n";
            res = DATA_ERROR_DELETE_DB;
        } else {
            temp.setType(temp.getType() & (0xFF - user.getType()));
            if (user.getType() & (BYTE)USER_GROUP) {
                temp.setContainerId("");
            }
			if(user.getType() & (BYTE)PROVIDER_GROUP){
				temp.setAddress("");
				temp.setPhone("");
				temp.setDescription("");
				temp.setKdeskAcc("");
			}
            res = updateUserDb(temp);
        }
    }
    return res;
}

ResponseCode DataManager::getAllUser(std::vector<User> &listUser) {
    sqlite3_stmt *statement;

    std::stringstream ss;
    ss << "SELECT * FROM USER WHERE TYPE=" << (int)(USER_GROUP)
							 << " OR TYPE=" << (int)(USER_GROUP | PROVIDER_GROUP)
							 << " OR TYPE=" << (int)(USER_GROUP | ADMIN_GROUP)
							 << " OR TYPE=" << (int)(USER_GROUP | PROVIDER_GROUP | ADMIN_GROUP)
							 << " OR TYPE=" << (int)(USER_GROUP | WAIT_PROVIDER_GROUP)
							 << " OR TYPE=" << (int)(USER_GROUP | WAIT_PROVIDER_GROUP | ADMIN_GROUP)
                             << ";";
    const char *query = ss.str().c_str();

    if ( sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK )
    {
        int res = 0;

        while ( 1 )
        {
            res = sqlite3_step(statement);

            if ( res == SQLITE_ROW )
            {
                User user;
                user.setEmail((char*)sqlite3_column_text(statement, 0));
                user.setContainerId((char*)sqlite3_column_text(statement, 1));
        user.setPhone((char*)sqlite3_column_text(statement, 2));
        user.setAddress((char*)sqlite3_column_text(statement, 3));
        user.setKdeskAcc((char*)sqlite3_column_text(statement, 4));
        user.setDescription((char*)sqlite3_column_text(statement, 5));
        user.setType((BYTE)sqlite3_column_bytes(statement, 6));
        //user.setKdeskPort(sqlite3_column_int(statement, 7));
        user.setTimeUsage(sqlite3_column_double(statement, 7));
        user.setTimeUsable(sqlite3_column_double(statement, 8));
        user.setPublicKey((char*)sqlite3_column_text(statement, 9));

                listUser.push_back(user);
            }

            if ( res == SQLITE_DONE || res==SQLITE_ERROR)
            {
                std::cout << "Get all user finish " << std::endl;
                break;
            }
        }
        sqlite3_finalize(statement);
    } else {
        return DATA_ERROR_SELECT_DB;
    }
    return DATA_SUCCESS;
}

ResponseCode DataManager::getUser(User &user) {
    std::stringstream strm;
    strm << "SELECT * FROM USER WHERE EMAIL='" << user.getEmail() << "' AND (TYPE=" << (int)(USER_GROUP)
		 << " OR TYPE=" << (int)(USER_GROUP | PROVIDER_GROUP)
		 << " OR TYPE=" << (int)(USER_GROUP | ADMIN_GROUP)
		 << " OR TYPE=" << (int)(USER_GROUP | PROVIDER_GROUP | ADMIN_GROUP)
		 << " OR TYPE=" << (int)(USER_GROUP | WAIT_PROVIDER_GROUP)
		 << " OR TYPE=" << (int)(USER_GROUP | WAIT_PROVIDER_GROUP | ADMIN_GROUP)
         << ");";

    std::string s = strm.str();
    char *str = &s[0];

    sqlite3_stmt *statement;

    char *query = str;

    if ( sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK )
    {
        int res = 0;

        res = sqlite3_step(statement);
        if ( res == SQLITE_ROW )
        {
            user.setEmail((char*)sqlite3_column_text(statement, 0));
            user.setContainerId((char*)sqlite3_column_text(statement, 1));
      user.setPhone((char*)sqlite3_column_text(statement, 2));
      user.setAddress((char*)sqlite3_column_text(statement, 3));
      user.setKdeskAcc((char*)sqlite3_column_text(statement, 4));
      user.setDescription((char*)sqlite3_column_text(statement, 5));
      user.setType((BYTE)sqlite3_column_bytes(statement, 6));
      //user.setKdeskPort(sqlite3_column_int(statement, 7));
      user.setTimeUsage(sqlite3_column_double(statement, 7));
      user.setTimeUsable(sqlite3_column_double(statement, 8));
      user.setPublicKey((char*)sqlite3_column_text(statement, 9));
        } else {
            std::cout << "user " << user.getEmail() << "is not exist\n";
            return DATA_SELECT_EMPTY;
        }

        sqlite3_finalize(statement);
    } else {
        return DATA_ERROR_SELECT_DB;
    }
    return DATA_SUCCESS;
}

ResponseCode DataManager::getAllAdmin(std::vector<User> &listAdmin) {
    sqlite3_stmt *statement;

    std::stringstream ss;
    ss << "SELECT * FROM USER WHERE TYPE=" << (int)(ADMIN_GROUP)
							 << " OR TYPE=" << (int)(ADMIN_GROUP | PROVIDER_GROUP)
							 << " OR TYPE=" << (int)(ADMIN_GROUP | USER_GROUP)
							 << " OR TYPE=" << (int)(ADMIN_GROUP | PROVIDER_GROUP | USER_GROUP)
							 << " OR TYPE=" << (int)(ADMIN_GROUP | WAIT_PROVIDER_GROUP)
							 << " OR TYPE=" << (int)(ADMIN_GROUP | WAIT_PROVIDER_GROUP | USER_GROUP)
                             << ";";
    const char *query = ss.str().c_str();

    if ( sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK )
    {
        int res = 0;

        while ( 1 )
        {
            res = sqlite3_step(statement);

            if ( res == SQLITE_ROW )
            {
                User admin;
                admin.setEmail((char*)sqlite3_column_text(statement, 0));
                admin.setContainerId((char*)sqlite3_column_text(statement, 1));
				admin.setPhone((char*)sqlite3_column_text(statement, 2));
				admin.setAddress((char*)sqlite3_column_text(statement, 3));
        admin.setKdeskAcc((char*)sqlite3_column_text(statement, 4));
        admin.setDescription((char*)sqlite3_column_text(statement, 5));
        admin.setType((BYTE)sqlite3_column_bytes(statement, 6));
        //admin.setKdeskPort(sqlite3_column_int(statement, 7));
        admin.setTimeUsage(sqlite3_column_double(statement, 7));
        admin.setTimeUsable(sqlite3_column_double(statement, 8));
        admin.setPublicKey((char*)sqlite3_column_text(statement, 9));

                listAdmin.push_back(admin);
            }

            if ( res == SQLITE_DONE || res==SQLITE_ERROR)
            {
                std::cout << "Get all admin finish " << std::endl;
                break;
            }
        }
        sqlite3_finalize(statement);
    } else {
        return DATA_ERROR_SELECT_DB;
    }
    return DATA_SUCCESS;
}

ResponseCode DataManager::getAdmin(User &admin) {
    std::stringstream strm;
    strm << "SELECT * FROM USER WHERE EMAIL='" << admin.getEmail() << "' AND (TYPE=" << (int)(USER_GROUP)
		 << " OR TYPE=" << (int)(ADMIN_GROUP | PROVIDER_GROUP)
		 << " OR TYPE=" << (int)(ADMIN_GROUP | USER_GROUP)
		 << " OR TYPE=" << (int)(ADMIN_GROUP | PROVIDER_GROUP | USER_GROUP)
		 << " OR TYPE=" << (int)(ADMIN_GROUP | WAIT_PROVIDER_GROUP)
		 << " OR TYPE=" << (int)(ADMIN_GROUP | WAIT_PROVIDER_GROUP | USER_GROUP)
         << ");";

    std::string s = strm.str();
    char *str = &s[0];

    sqlite3_stmt *statement;

    char *query = str;

    if ( sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK )
    {
        int res = 0;

        res = sqlite3_step(statement);
        if ( res == SQLITE_ROW )
        {
            admin.setEmail((char*)sqlite3_column_text(statement, 0));
            admin.setContainerId((char*)sqlite3_column_text(statement, 1));
			admin.setPhone((char*)sqlite3_column_text(statement, 2));
			admin.setAddress((char*)sqlite3_column_text(statement, 3));
      admin.setKdeskAcc((char*)sqlite3_column_text(statement, 4));
      admin.setDescription((char*)sqlite3_column_text(statement, 5));
      admin.setType((BYTE)sqlite3_column_bytes(statement, 6));
      //admin.setKdeskPort(sqlite3_column_int(statement, 7));
      admin.setTimeUsage(sqlite3_column_double(statement, 7));
      admin.setTimeUsable(sqlite3_column_double(statement, 8));
      admin.setPublicKey((char*)sqlite3_column_text(statement, 9));

        } else {
            std::cout << "admin " << admin.getEmail() << "is not exist\n";
            return DATA_SELECT_EMPTY;
        }

        sqlite3_finalize(statement);
    } else {
        return DATA_ERROR_SELECT_DB;
    }
    return DATA_SUCCESS;
}

ResponseCode DataManager::getAllProvider(std::vector<User> &listProvider) {
	sqlite3_stmt *statement;

	std::stringstream ss;
	ss << "SELECT * FROM USER WHERE (TYPE=" << (int)(PROVIDER_GROUP)
							 << " OR TYPE=" << (int)(PROVIDER_GROUP | ADMIN_GROUP)
							 << " OR TYPE=" << (int)(PROVIDER_GROUP | USER_GROUP)
							 << " OR TYPE=" << (int)(PROVIDER_GROUP | ADMIN_GROUP | USER_GROUP)
							 << ");";
	const char *query = ss.str().c_str();
	std::cout << "query : " << query << std::endl;

	if ( sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK )
	{
		int res = 0;

		while ( 1 )
		{
			res = sqlite3_step(statement);

			if ( res == SQLITE_ROW )
			{
				User provider;
				provider.setEmail((char*)sqlite3_column_text(statement, 0));
				provider.setContainerId((char*)sqlite3_column_text(statement, 1));
				provider.setPhone((char*)sqlite3_column_text(statement, 2));
				provider.setAddress((char*)sqlite3_column_text(statement, 3));
        provider.setKdeskAcc((char*)sqlite3_column_text(statement, 4));
        provider.setDescription((char*)sqlite3_column_text(statement, 5));
        provider.setType((BYTE)sqlite3_column_bytes(statement, 6));
        //provider.setKdeskPort(sqlite3_column_int(statement, 7));
        provider.setTimeUsage(sqlite3_column_double(statement, 7));
        provider.setTimeUsable(sqlite3_column_double(statement, 8));
        provider.setPublicKey((char*)sqlite3_column_text(statement, 9));

				listProvider.push_back(provider);
			}

			if ( res == SQLITE_DONE || res==SQLITE_ERROR)
			{
        std::cout << "Get all wait provider finish." << std::endl;
				break;
			}
		}
		sqlite3_finalize(statement);
	} else {
		return DATA_ERROR_SELECT_DB;
	}
	return DATA_SUCCESS;
}

ResponseCode DataManager::getProvider(User &provider) {
    std::stringstream strm;
	strm << "SELECT * FROM USER WHERE (EMAIL='" << provider.getEmail() << "' AND (TYPE=" << (int)(PROVIDER_GROUP)
		 << " OR TYPE=" << (int)(PROVIDER_GROUP | ADMIN_GROUP)
		 << " OR TYPE=" << (int)(PROVIDER_GROUP | USER_GROUP)
		 << " OR TYPE=" << (int)(PROVIDER_GROUP | ADMIN_GROUP | USER_GROUP)
		 << "));";

    std::string s = strm.str();
    char *str = &s[0];

    sqlite3_stmt *statement;

    char *query = str;

    if ( sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK )
    {
        int res = 0;

        res = sqlite3_step(statement);
        if ( res == SQLITE_ROW )
        {
            provider.setEmail((char*)sqlite3_column_text(statement, 0));
            provider.setContainerId((char*)sqlite3_column_text(statement, 1));
			provider.setPhone((char*)sqlite3_column_text(statement, 2));
			provider.setAddress((char*)sqlite3_column_text(statement, 3));
      provider.setKdeskAcc((char*)sqlite3_column_text(statement, 4));
      provider.setDescription((char*)sqlite3_column_text(statement, 5));
      provider.setType((BYTE)sqlite3_column_bytes(statement, 6));
      //provider.setKdeskPort(sqlite3_column_int(statement, 7));
      provider.setTimeUsage(sqlite3_column_double(statement, 7));
      provider.setTimeUsable(sqlite3_column_double(statement, 8));
      provider.setPublicKey((char*)sqlite3_column_text(statement, 9));
        } else {
            std::cout << "provider " << provider.getEmail() << "is not exist\n";
            return DATA_SELECT_EMPTY;
        }

        sqlite3_finalize(statement);
    } else {
        return DATA_ERROR_SELECT_DB;
    }
    return DATA_SUCCESS;
}

ResponseCode DataManager::getAllWaitProviders(std::vector<User> &listWaitProvides) {
    sqlite3_stmt *statement;

    std::stringstream ss;
	ss << "SELECT * FROM USER WHERE (TYPE=" << (int)(WAIT_PROVIDER_GROUP)
							 << " OR TYPE=" << (int)(WAIT_PROVIDER_GROUP | ADMIN_GROUP)
							 << " OR TYPE=" << (int)(WAIT_PROVIDER_GROUP | USER_GROUP)
							 << " OR TYPE=" << (int)(WAIT_PROVIDER_GROUP | ADMIN_GROUP | USER_GROUP)
							 << ");";
    std::string str = ss.str();
	const char *query = str.c_str();
	std::cout << "query : " << query << std::endl;

    if ( sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK )
    {
        int res = 0;

        while ( 1 )
        {
            res = sqlite3_step(statement);

            if ( res == SQLITE_ROW )
            {
                User waitProvider;
                waitProvider.setEmail((char*)sqlite3_column_text(statement, 0));
                waitProvider.setContainerId((char*)sqlite3_column_text(statement, 1));
				waitProvider.setPhone((char*)sqlite3_column_text(statement, 2));
				waitProvider.setAddress((char*)sqlite3_column_text(statement, 3));
        waitProvider.setKdeskAcc((char*)sqlite3_column_text(statement, 4));
        waitProvider.setDescription((char*)sqlite3_column_text(statement, 5));
        waitProvider.setType((BYTE)sqlite3_column_bytes(statement, 6));
        //waitProvider.setKdeskPort(sqlite3_column_int(statement, 7));
        waitProvider.setTimeUsage(sqlite3_column_double(statement, 7));
        waitProvider.setTimeUsable(sqlite3_column_double(statement, 8));
        waitProvider.setPublicKey((char*)sqlite3_column_text(statement, 9));

                listWaitProvides.push_back(waitProvider);
            }

            if ( res == SQLITE_DONE || res==SQLITE_ERROR)
            {
                std::cout << "Get all wait provider finish " << std::endl;
                break;
            }
        }
        sqlite3_finalize(statement);
    } else {
        return DATA_ERROR_SELECT_DB;
    }
    return DATA_SUCCESS;
}

ResponseCode DataManager::insertService(Service &sv) {
  std::cout << "DataManager::insertService\n";
    /*
    "SERVICE_ID             NVARCHAR(100) PRIMARY KEY       NOT NULL," \
    "SERVICE_NAME           NVARCHAR(100)                   NOT NULL," \
    "PRODUCER               NVARCHAR(50)                    NOT NULL," \
    "STATUS                 BOOLEAN                         NOT NULL," \
    "NUMBER_OF_USER         INT                             NOT NULL," \
    "NUMBER_OF_WAIT_USER    INT                             NOT NULL," \
    "DESCRIPTION            TEXT                            NOT NULL," \
    "PATH_EXCUTE            NVARCHAR(255)                   NOT NULL," \
    "PATH_SH                NVARCHAR(255)                   NOT NULL," \
    "SH_TEMPLATE            TEXT                            NOT NULL," \
    "STAGE_IN_DIRS          NVARCHAR(1255)                  NOT NULL," \
    "STAGE_OUT_DIRS         NVARCHAR(1255)                  NOT NULL," \
    "IMAGE_ID               NVARCHAR(100)                   NOT NULL," \
    "EMAIL_PROVIDER         NVARCHAR(100)                   NOT NULL," \
    "NUMBER_OF_NODE         INT                             NOT NULL," \
    "MAX_ELAPSE_TIME        INT                             NOT NULL," \
    */
    std::stringstream strm, stginss, stgoutss;
    std::string stginstr, stgoutstr;
    for(unsigned int i = 0; i < sv.getStageinDirs()->size(); i++){
      stginss << sv.getStageinDirs()->at(i) << ";";
    }
    stginstr = stginss.str();
    for(unsigned int i = 0; i < sv.getStageoutDirs()->size(); i++){
      stgoutss << sv.getStageoutDirs()->at(i) << ";";
    }
    stgoutstr = stgoutss.str();
    //std::cout << "stgin-dirs: " << stginstr << ", stgout-dirs: " << stgoutstr << std::endl;
    strm << "insert into SERVICE values("
         << "'" << sv.getServiceID() << "',"
         << "'" << sv.getServiceName() << "',"
         << "'" << sv.getProducer() << "',"
         << "" << sv.getStatus() << ","
         << "" << sv.getNumberOfUser() << ","
         << "" << sv.getNumberOfWaitUser() << ","
         << "'" << sv.getDescription() << "',"
         << "'" << sv.getPathExcuteFile() << "',"
         << "'" << sv.getPathShFile() << "',"
         << "'" << sv.getShTemplate() << "',"
         << "'" << stginss.str() << "',"
         << "'" << stgoutss.str() << "',"
         << "'" << sv.getImageId() << "',"
         << "'" << sv.getProvider() <<  "',"
         << "" << sv.getNumberOfNode() << ","
         << "" << sv.getMaxElapseTime() << ");";

    std::string s = strm.str();
    std::cout << "Insert command: " << s.c_str() << std::endl;

    char *str = &s[0];

    char *zErrMsg = 0;
    int rc = sqlite3_exec(db, str, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        return DATA_ERROR_INSERT_DB;
    } else {
        std::cout << "Insert service: " << sv.getServiceID() << " success \n";
    }
    return DATA_SUCCESS;
}

ResponseCode DataManager::updateService(Service &sv) {
    /*
    "SERVICE_ID             NVARCHAR(100) PRIMARY KEY       NOT NULL," \
    "SERVICE_NAME           NVARCHAR(100)                   NOT NULL," \
    "PRODUCER               NVARCHAR(50)                    NOT NULL," \
    "STATUS                 BOOLEAN                         NOT NULL," \
    "NUMBER_OF_USER         INT                             NOT NULL," \
    "NUMBER_OF_WAIT_USER    INT                             NOT NULL," \
    "DESCRIPTION            TEXT                            NOT NULL," \
    "PATH_EXCUTE            NVARCHAR(255)                   NOT NULL," \
    "PATH_SH                NVARCHAR(255)                   NOT NULL," \
    "SH_TEMPLATE            TEXT                            NOT NULL," \
    "STAGE_IN_DIRS          NVARCHAR(1255)                  NOT NULL," \
    "STAGE_OUT_DIRS         NVARCHAR(1255)                  NOT NULL," \
    "IMAGE_ID               NVARCHAR(100)                   NOT NULL," \
    "EMAIL_PROVIDER         NVARCHAR(100)                   NOT NULL," \
    "NUMBER_OF_NODE         INT                             NOT NULL," \
    "MAX_ELAPSE_TIME        INT                             NOT NULL," \
    */
    std::stringstream strm, stginss, stgoutss;
    for(unsigned int i = 0; i < sv.getStageinDirs()->size(); i++){
      stginss << sv.getStageinDirs()->at(i).c_str() << ";";
    }
    for(unsigned int i = 0; i < sv.getStageoutDirs()->size(); i++){
      stgoutss << sv.getStageoutDirs()->at(i).c_str() << ";";
    }

    strm << "UPDATE SERVICE SET "
         << "SERVICE_NAME='" << sv.getServiceName() << "',"
         << "PRODUCER='" << sv.getProducer() << "',"
         << "STATUS=" << sv.getStatus() << ","
         << "NUMBER_OF_USER=" << sv.getNumberOfUser() << ","
         << "NUMBER_OF_WAIT_USER=" << sv.getNumberOfWaitUser() << ","
         << "DESCRIPTION='" << sv.getDescription() << "',"
         << "PATH_EXCUTE='" << sv.getPathExcuteFile() << "',"
         << "PATH_SH='" << sv.getPathShFile() << "',"
         << "SH_TEMPLATE='" << sv.getShTemplate() << "',"
         << "STAGE_IN_DIRS='" << stginss.str() << "',"
         << "STAGE_OUT_DIRS='" << stgoutss.str() << "',"
         << "IMAGE_ID='" << sv.getImageId() << "',"
         << "EMAIL_PROVIDER='" << sv.getProvider() << "',"
         << "NUMBER_OF_NODE=" << sv.getNumberOfNode() << ","
         << "MAX_ELAPSE_TIME=" << sv.getMaxElapseTime()// << ","
		 << " WHERE (SERVICE_ID='" << sv.getServiceID() <<"');";

    std::string s = strm.str();
    char *str = &s[0];

    char *zErrMsg = 0;
    int rc = sqlite3_exec(db, str, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n  sql cmd: %s", zErrMsg, s.c_str());
        return DATA_ERROR_UPDATE_DB;
    } else {
        std::cout << "Update service: " << sv.getServiceID() << " success \n";
    }
    return DATA_SUCCESS;
}

ResponseCode DataManager::deleteService(Service &sv) {
    std::stringstream strm;
	strm << "DELETE FROM SERVICE WHERE (SERVICE_ID='" << sv.getServiceID() << "');";

    std::string s = strm.str();
    char *str = &s[0];

    char *zErrMsg = 0;
    int rc = sqlite3_exec(db, str, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        return DATA_ERROR_DELETE_DB;
    } else {
        std::cout << "Delete service: " << sv.getServiceID() << " success \n";
    }
    return DATA_SUCCESS;
}

ResponseCode DataManager::getAllService(std::vector<Service> &listService) {
    /*
    "SERVICE_ID             NVARCHAR(100) PRIMARY KEY       NOT NULL," \
    "SERVICE_NAME           NVARCHAR(100)                   NOT NULL," \
    "PRODUCER               NVARCHAR(50)                    NOT NULL," \
    "STATUS                 BOOLEAN                         NOT NULL," \
    "NUMBER_OF_USER         INT                             NOT NULL," \
    "NUMBER_OF_WAIT_USER    INT                             NOT NULL," \
    "DESCRIPTION            TEXT                            NOT NULL," \
    "PATH_EXCUTE            NVARCHAR(255)                   NOT NULL," \
    "PATH_SH                NVARCHAR(255)                   NOT NULL," \
    "SH_TEMPLATE            TEXT                            NOT NULL," \
    "STAGE_IN_DIRS          NVARCHAR(1255)                  NOT NULL," \
    "STAGE_OUT_DIRS         NVARCHAR(1255)                  NOT NULL," \
    "IMAGE_ID               NVARCHAR(100)                   NOT NULL," \
    "EMAIL_PROVIDER         NVARCHAR(100)                   NOT NULL," \
    "NUMBER_OF_NODE         INT                             NOT NULL," \
    "MAX_ELAPSE_TIME        INT                             NOT NULL," \
    */
    sqlite3_stmt *statement;

    char query[] = "SELECT * FROM SERVICE WHERE (STATUS=1)";

    if ( sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK )
    {
        int res = 0;

        while ( 1 )
        {
            res = sqlite3_step(statement);

            if ( res == SQLITE_ROW )
            {
                Service service;
                service.setServiceID((char*)sqlite3_column_text(statement, 0));
                service.setServiceName((char*)sqlite3_column_text(statement, 1));
                service.setProducer((char*)sqlite3_column_text(statement, 2));
                service.setStatus((bool)sqlite3_column_int(statement, 3));
                service.setNumberOfUser(sqlite3_column_int(statement, 4));
                service.setNumberOfWaitUser(sqlite3_column_int(statement, 5));
                service.setDescription((char*)sqlite3_column_text(statement, 6));
                service.setPathExcuteFile((char*)sqlite3_column_text(statement, 7));
                service.setPathShFile((char*)sqlite3_column_text(statement, 8));
                service.setShTemplate((char*)sqlite3_column_text(statement, 9));
                std::string stginstr = (char*)sqlite3_column_text(statement, 10);
                std::vector<std::string> dirs;
                if(stginstr.size() > 0){
                  std::stringstream ss(stginstr);
                  std::string path = "";
                  while (std::getline(ss, path, ';')) {
                      dirs.push_back(path);
                  }
                }
                service.setStageinDirs(dirs);

                std::string stgoutstr = (char*)sqlite3_column_text(statement, 11);
                std::vector<std::string> dirs2;
                if(stgoutstr.size() > 0){
                  std::stringstream ss(stgoutstr);
                  std::string path;
                  while (std::getline(ss, path, ';')) {
                    dirs2.push_back(path);
                  }
                }
                service.setStageoutDirs(dirs2);

                service.setImageId((char*)sqlite3_column_text(statement, 12));
                service.setProvider((char*)sqlite3_column_text(statement, 13));
                service.setNumberOfNode(sqlite3_column_int(statement, 14));
                service.setMaxElapseTime((unsigned int)sqlite3_column_int(statement, 15));
                listService.push_back(service);
                std::cout << "service name = " << service.getServiceName() << std::endl;
            }

            if ( res == SQLITE_DONE || res==SQLITE_ERROR)
            {
                std::cout << "Get all service finish " << std::endl;
                break;
            }
        }
        sqlite3_finalize(statement);
    } else {
        return DATA_ERROR_SELECT_DB;
    }
    return DATA_SUCCESS;
}

ResponseCode DataManager::getAllServiceOfProvider(User &provider, std::vector<Service> &listService) {
    /*
    "SERVICE_ID             NVARCHAR(100) PRIMARY KEY       NOT NULL," \
    "SERVICE_NAME           NVARCHAR(100)                   NOT NULL," \
    "PRODUCER               NVARCHAR(50)                    NOT NULL," \
    "STATUS                 BOOLEAN                         NOT NULL," \
    "NUMBER_OF_USER         INT                             NOT NULL," \
    "NUMBER_OF_WAIT_USER    INT                             NOT NULL," \
    "DESCRIPTION            TEXT                            NOT NULL," \
    "PATH_EXCUTE            NVARCHAR(255)                   NOT NULL," \
    "PATH_SH                NVARCHAR(255)                   NOT NULL," \
    "SH_TEMPLATE            TEXT                            NOT NULL," \
    "STAGE_IN_DIRS          NVARCHAR(1255)                  NOT NULL," \
    "STAGE_OUT_DIRS         NVARCHAR(1255)                  NOT NULL," \
    "IMAGE_ID               NVARCHAR(100)                   NOT NULL," \
    "EMAIL_PROVIDER         NVARCHAR(100)                   NOT NULL," \
    "NUMBER_OF_NODE         INT                             NOT NULL," \
    "MAX_ELAPSE_TIME        INT                             NOT NULL," \
    */
    std::stringstream strm;
    strm << "SELECT * FROM SERVICE WHERE EMAIL_PROVIDER='" << provider.getEmail() << "';";

    std::string s = strm.str();
    char *str = &s[0];

    sqlite3_stmt *statement;

    char *query = str;

    if ( sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK )
    {
        int res = 0;

        while ( 1 )
        {
            res = sqlite3_step(statement);

            if ( res == SQLITE_ROW )
            {
                Service service;
                service.setServiceID((char*)sqlite3_column_text(statement, 0));
                service.setServiceName((char*)sqlite3_column_text(statement, 1));
                service.setProducer((char*)sqlite3_column_text(statement, 2));
                service.setStatus((bool)sqlite3_column_int(statement, 3));
                service.setNumberOfUser(sqlite3_column_int(statement, 4));
                service.setNumberOfWaitUser(sqlite3_column_int(statement, 5));
                service.setDescription((char*)sqlite3_column_text(statement, 6));
                service.setPathExcuteFile((char*)sqlite3_column_text(statement, 7));
                service.setPathShFile((char*)sqlite3_column_text(statement, 8));
                service.setShTemplate((char*)sqlite3_column_text(statement, 9));
                std::string stginstr = (char*)sqlite3_column_text(statement, 10);
                std::vector<std::string> dirs;
                if(stginstr.size() > 0){
                  std::stringstream ss(stginstr);
                  std::string path = "";
                  while (std::getline(ss, path, ';')) {
                      dirs.push_back(path);
                  }
                }
                service.setStageinDirs(dirs);

                std::string stgoutstr = (char*)sqlite3_column_text(statement, 11);
                std::vector<std::string> dirs2;
                if(stgoutstr.size() > 0){
                  std::stringstream ss(stgoutstr);
                  std::string path;
                  while (std::getline(ss, path, ';')) {
                    dirs2.push_back(path);
                  }
                }
                service.setStageoutDirs(dirs2);

                service.setImageId((char*)sqlite3_column_text(statement, 12));
                service.setProvider((char*)sqlite3_column_text(statement, 13));
                service.setNumberOfNode(sqlite3_column_int(statement, 14));
                service.setMaxElapseTime((unsigned int)sqlite3_column_int(statement, 15));
                listService.push_back(service);
            }

            if ( res == SQLITE_DONE || res==SQLITE_ERROR)
            {
                std::cout << "Get service by provider email finish " << std::endl;
                break;
            }
        }
        sqlite3_finalize(statement);
    } else {
        return DATA_ERROR_SELECT_DB;
    }
    return DATA_SUCCESS;
}

ResponseCode DataManager::getAllServiceOfProviderHaveCandidate(User &provider, std::vector<Service> &listService) {
    std::stringstream strm;
    strm << "SELECT * "
         << "FROM ("
         << "(SELECT * FROM SERVICE WHERE EMAIL_PROVIDER='" << provider.getEmail() << "') t1 "
         << "INNER JOIN "
         << "USER_SERVICE ON t1.SERVICE_ID=USER_SERVICE.SERVICE_ID ) t"
         << "WHERE t.STATUS_USER=" << WAITING
         << ";";

    std::string s = strm.str();
    std::cout << "query: " << s << std::endl;
    char *str = &s[0];

    sqlite3_stmt *statement;

    char *query = str;

    if ( sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK )
    {
        int res = 0;

        while ( 1 )
        {
            res = sqlite3_step(statement);

            if ( res == SQLITE_ROW )
            {
                Service service;
                service.setServiceID((char*)sqlite3_column_text(statement, 0));
                service.setServiceName((char*)sqlite3_column_text(statement, 1));
                service.setProducer((char*)sqlite3_column_text(statement, 2));
                service.setStatus((bool)sqlite3_column_int(statement, 3));
                service.setNumberOfUser(sqlite3_column_int(statement, 4));
                service.setNumberOfWaitUser(sqlite3_column_int(statement, 5));
                service.setDescription((char*)sqlite3_column_text(statement, 6));
                service.setPathExcuteFile((char*)sqlite3_column_text(statement, 7));
                service.setPathShFile((char*)sqlite3_column_text(statement, 8));
                service.setShTemplate((char*)sqlite3_column_text(statement, 9));

                std::string stginstr = (char*)sqlite3_column_text(statement, 10);
                std::vector<std::string> dirs;
                if(stginstr.size() > 0){
                  std::stringstream ss(stginstr);
                  std::string path = "";
                  while (std::getline(ss, path, ';')) {
                      dirs.push_back(path);
                  }
                }
                service.setStageinDirs(dirs);

                std::string stgoutstr = (char*)sqlite3_column_text(statement, 11);
                std::vector<std::string> dirs2;
                if(stgoutstr.size() > 0){
                  std::stringstream ss(stgoutstr);
                  std::string path;
                  while (std::getline(ss, path, ';')) {
                    dirs2.push_back(path);
                  }
                }
                service.setStageoutDirs(dirs2);

                service.setImageId((char*)sqlite3_column_text(statement, 12));
                service.setProvider((char*)sqlite3_column_text(statement, 13));
                service.setNumberOfNode(sqlite3_column_int(statement, 14));
                service.setMaxElapseTime((unsigned int)sqlite3_column_int(statement, 15));
                listService.push_back(service);
            }

            if ( res == SQLITE_DONE || res==SQLITE_ERROR)
            {
                std::cout << "Get service has candidate by provider email finish " << std::endl;
                break;
            }
        }
        sqlite3_finalize(statement);
    } else {
        return DATA_ERROR_SELECT_DB;
    }
    return DATA_SUCCESS;
}

ResponseCode DataManager::getInfoOfService(Service &sv) {
    ResponseCode ret;
    std::stringstream strm;
    strm << "SELECT * FROM SERVICE WHERE SERVICE_ID='" << sv.getServiceID() << "';";

    std::string s = strm.str();
    char *str = &s[0];

    sqlite3_stmt *statement;

    char *query = str;

    if ( sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK )
    {
        int res = 0;

        res = sqlite3_step(statement);
        if ( res == SQLITE_ROW )
        {
            //Service service;
            sv.setServiceID((char*)sqlite3_column_text(statement, 0));
            sv.setServiceName((char*)sqlite3_column_text(statement, 1));
            sv.setProducer((char*)sqlite3_column_text(statement, 2));
            sv.setStatus((bool)sqlite3_column_int(statement, 3));
            sv.setNumberOfUser(sqlite3_column_int(statement, 4));
            sv.setNumberOfWaitUser(sqlite3_column_int(statement, 5));
            sv.setDescription((char*)sqlite3_column_text(statement, 6));
            sv.setPathExcuteFile((char*)sqlite3_column_text(statement, 7));
            sv.setPathShFile((char*)sqlite3_column_text(statement, 8));
            sv.setShTemplate((char*)sqlite3_column_text(statement, 9));

            std::string stginstr = (char*)sqlite3_column_text(statement, 10);
            std::vector<std::string> dirs;
            if(stginstr.size() > 0){
              std::stringstream ss(stginstr);
              std::string path = "";
              while (std::getline(ss, path, ';')) {
                  dirs.push_back(path);
              }
            }
            sv.setStageinDirs(dirs);

            std::string stgoutstr = (char*)sqlite3_column_text(statement, 11);
            std::vector<std::string> dirs2;
            if(stgoutstr.size() > 0){
              std::stringstream ss(stgoutstr);
              std::string path;
              while (std::getline(ss, path, ';')) {
                dirs2.push_back(path);
              }
            }
            sv.setStageoutDirs(dirs2);

            sv.setImageId((char*)sqlite3_column_text(statement, 12));
            sv.setProvider((char*)sqlite3_column_text(statement, 13));
            sv.setNumberOfNode(sqlite3_column_int(statement, 14));
            sv.setMaxElapseTime((unsigned int)sqlite3_column_int(statement, 15));
            ret = DATA_SUCCESS;
        } else {
            ret = DATA_SELECT_EMPTY;
        }
        sqlite3_finalize(statement);
    } else {
        ret = DATA_ERROR_SELECT_DB;
    }
    return ret;
}

ResponseCode DataManager::checkPathExcute(std::string pathExeFile, bool &result) {
    std::stringstream strm;
    strm << "SELECT * FROM SERVICE WHERE PATH_EXCUTE='" << pathExeFile << "';";

    std::string s = strm.str();
    char *str = &s[0];

    sqlite3_stmt *statement;

    char *query = str;

    if ( sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK )
    {
        int res = 0;

        while ( 1 )
        {
            res = sqlite3_step(statement);

            if ( res == SQLITE_ROW )
            {
                result = true;
                break;
            }

            if ( res == SQLITE_DONE || res==SQLITE_ERROR)
            {
                std::cout << "Check path excute file finish " << std::endl;
                break;
            }
        }
        sqlite3_finalize(statement);
    } else {
        return DATA_ERROR_SELECT_DB;
    }
    return DATA_SUCCESS;
}

ResponseCode DataManager::insertParam(Parameter &param) {
    /*
    "CREATE TABLE PARAMETER("  \
            "PARAM_ID       CHAR(20)        NOT NULL," \
            "SERVICE_ID     NVARCHAR(100)   NOT NULL," \
            "IS_OUTPUT      BOOLEAN         NOT NULL," \
            "OPTION_NAME    CHAR(50)        NOT NULL," \
            "IS_REQUIRED    BOOLEAN         NOT NULL," \
            "DESCRIPTION    TEXT            NOT NULL," \
            "TYPE           CHAR(20)        NOT NULL," \
            "NAME           NVARCHAR(50)    NOT NULL," \
            "DEFAULT_VALUE  NVARCHAR(100)   NOT NULL," \
            "PRIMARY KEY (PARAM_ID, SERVICE_ID)," \
            "FOREIGN KEY(SERVICE_ID) REFERENCES SERVICE(SERVICE_ID));";
    */
    std::stringstream strm;
    strm << "insert into PARAMETER values("
         << "'" << param.getParamID() << "',"
         << "'" << param.getServiceID() << "',"
         << "" << param.getIsOutput() << ","
         << "'" << param.getOptionName() << "',"
         << "" << param.getIsRequired() << ","
         << "'" << param.getDescription() << "',"
         << "'" << param.getType() << "',"
         << "'" << param.getName() << "',"
         << "'" << param.getDefaultValue() << "');";

    std::string s = strm.str();
    char *str = &s[0];

    char *zErrMsg = 0;
    int rc = sqlite3_exec(db, str, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        return DATA_ERROR_INSERT_DB;
    } else {
        std::cout << "Insert parameter: " << param.getParamID() << " of service: " << param.getServiceID() << " success \n";
    }
    return DATA_SUCCESS;

}

ResponseCode DataManager::updateParam(Parameter &param) {
    std::stringstream strm;
    strm << "UPDATE PARAMETER SET "
         << "IS_OUTPUT=" << param.getIsOutput() << ","
         << "OPTION_NAME='" << param.getOptionName() << "',"
         << "IS_REQUIRED=" << param.getIsRequired() << ","
         << "DESCRIPTION='" << param.getDescription() << "',"
         << "TYPE='" << param.getType() << "',"
         << "NAME='" << param.getName() << "',"
         << "DEFAULT_VALUE='" << param.getDefaultValue() << "'"
         << " WHERE PARAM_ID='" << param.getParamID() << "' AND SERVICE_ID='" << param.getServiceID() <<"';";

    std::string s = strm.str();
    char *str = &s[0];

    char *zErrMsg = 0;
    int rc = sqlite3_exec(db, str, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        return DATA_ERROR_UPDATE_DB;
    } else {
        std::cout << "Update param: " << param.getParamID() << " of service: " << param.getServiceID() << " success \n";
    }
    return DATA_SUCCESS;
}

ResponseCode DataManager::deleteParam(Parameter &param) {
    std::stringstream strm;
    strm << "DELETE FROM PARAMETER WHERE PARAM_ID='" << param.getParamID() << "' AND SERVICE_ID='" << param.getServiceID() << "';";

    std::string s = strm.str();
    char *str = &s[0];

    char *zErrMsg = 0;
    int rc = sqlite3_exec(db, str, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        return DATA_ERROR_DELETE_DB;
    } else {
        std::cout << "Delete parameter: " << param.getParamID() << " of service: " << param.getServiceID() << " success \n";
    }
    return DATA_SUCCESS;
}

ResponseCode DataManager::getParamOfService(Service &sv, std::vector<Parameter> &listParam) {
    /*
    "CREATE TABLE PARAMETER("  \
            "PARAM_ID       CHAR(20)        NOT NULL," \
            "SERVICE_ID     NVARCHAR(100)   NOT NULL," \
            "IS_OUTPUT      BOOLEAN         NOT NULL," \
            "OPTION_NAME    CHAR(50)        NOT NULL," \
            "IS_REQUIRED    BOOLEAN         NOT NULL," \
            "DESCRIPTION    TEXT            NOT NULL," \
            "TYPE           CHAR(20)        NOT NULL," \
            "NAME           NVARCHAR(50)    NOT NULL," \
            "DEFAULT_VALUE  NVARCHAR(100)   NOT NULL," \
            "PRIMARY KEY (PARAM_ID, SERVICE_ID)," \
            "FOREIGN KEY(SERVICE_ID) REFERENCES SERVICE(SERVICE_ID));";
    */
    std::stringstream strm;
    strm << "SELECT * FROM PARAMETER WHERE SERVICE_ID='" << sv.getServiceID() << "';";

    std::string s = strm.str();
    char *str = &s[0];

    sqlite3_stmt *statement;

    char *query = str;

    if ( sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK )
    {
        int res = 0;

        while ( 1 )
        {
            res = sqlite3_step(statement);

            if ( res == SQLITE_ROW )
            {
                Parameter param;
                param.setParamID((char*)sqlite3_column_text(statement, 0));
                param.setServiceID((char*)sqlite3_column_text(statement, 1));
                param.setIsOutput((bool)sqlite3_column_int(statement, 2));
                param.setOptionName((char*)sqlite3_column_text(statement, 3));
                param.setIsRequired((bool)sqlite3_column_int(statement, 4));
                param.setDescription((char*)sqlite3_column_text(statement, 5));
                param.setType((char*)sqlite3_column_text(statement, 6));
                param.setName((char*)sqlite3_column_text(statement, 7));
                param.setDefaultValue((char*)sqlite3_column_text(statement, 8));
                listParam.push_back(param);
            }

            if ( res == SQLITE_DONE || res==SQLITE_ERROR)
            {
                std::cout << "Get parameter of service " << sv.getServiceID() << " finish " << std::endl;
                break;
            }
        }
        sqlite3_finalize(statement);
    } else {
        return DATA_ERROR_SELECT_DB;
    }
    return DATA_SUCCESS;
}

ResponseCode DataManager::insertUserService(User_Service &us) {
    /*
    "CREATE TABLE USER_SERVICE("  \
                "USER_EMAIL     NVARCHAR(100)   NOT NULL," \
                "SERVICE_ID     NVARCHAR(100)   NOT NULL," \
                "STATUS_USER    INT8             NOT NULL," \
                "PRIMARY KEY (USER_EMAIL, SERVICE_ID)," \
                "FOREIGN KEY (SERVICE_ID) REFERENCES SERVICE(SERVICE_ID) ON DELETE CASCADE);";
    */
    std::stringstream strm;
    strm << "insert into USER_SERVICE values("
         << "'" << us.getEmail() << "',"
         << "'" << us.getServiceId() << "',"
         << "" << us.getStatus() << ");";

    std::string s = strm.str();
    char *str = &s[0];

    char *zErrMsg = 0;
    int rc = sqlite3_exec(db, str, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        return DATA_ERROR_INSERT_DB;
    } else {
        std::cout << "Insert user: " << us.getEmail() << " of service: " << us.getServiceId() << " success \n";
    }
    return DATA_SUCCESS;
}

ResponseCode DataManager::updateUserService(User_Service &us) {
    std::stringstream strm;
    strm << "UPDATE USER_SERVICE SET "
         << "STATUS_USER=" << us.getStatus()
         << " WHERE USER_EMAIL='" << us.getEmail() << "' AND SERVICE_ID='" << us.getServiceId() <<"';";

    std::string s = strm.str();
    char *str = &s[0];

    char *zErrMsg = 0;
    int rc = sqlite3_exec(db, str, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        return DATA_ERROR_UPDATE_DB;
    } else {
        std::cout << "Update user: " << us.getEmail() << " of service: " << us.getServiceId() << " success \n";
    }
    return DATA_SUCCESS;
}

ResponseCode DataManager::deleteUserService(User_Service &us) {
    std::stringstream strm;
    strm << "DELETE FROM USER_SERVICE WHERE USER_EMAIL='" << us.getEmail() << "' AND SERVICE_ID='" << us.getServiceId() << "';";

    std::string s = strm.str();
    char *str = &s[0];

    char *zErrMsg = 0;
    int rc = sqlite3_exec(db, str, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        return DATA_ERROR_DELETE_DB;
    } else {
        std::cout << "Delete user: " << us.getEmail() << " of service: " << us.getServiceId() << " success \n";
    }
    return DATA_SUCCESS;
}

ResponseCode DataManager::getInfoOfUserService(User_Service &us) {
    ResponseCode ret;
    std::stringstream strm;
    strm << "SELECT * FROM USER_SERVICE WHERE USER_EMAIL='" << us.getEmail() << "' AND SERVICE_ID='" << us.getServiceId() << "';";

    std::string s = strm.str();
    char *str = &s[0];

    sqlite3_stmt *statement;

    char *query = str;

    if ( sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK )
    {
        int res = 0;

        res = sqlite3_step(statement);
        if ( res == SQLITE_ROW )
        {
            us.setUserEmail((char*)sqlite3_column_text(statement, 0));
            us.setServiceId((char*)sqlite3_column_text(statement, 1));
            us.setStatus((StatusUSer)sqlite3_column_int(statement, 2));
            ret = DATA_SUCCESS;
        } else {
            ret = DATA_SELECT_EMPTY;
        }
        sqlite3_finalize(statement);
    } else {
        ret = DATA_ERROR_SELECT_DB;
    }
    std::cout << "get info of user service finish\n";
    return ret;
}

ResponseCode DataManager::getAllReadyUserOfService(std::string serviceId, std::vector<std::string> &listUser) {
    std::stringstream strm;
    strm << "SELECT * FROM USER_SERVICE WHERE SERVICE_ID='" << serviceId << "' AND STATUS_USER=" << READY << ";";

    std::string s = strm.str();
    char *str = &s[0];

    sqlite3_stmt *statement;

    char *query = str;

    if ( sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK )
    {
        int res = 0;

        while ( 1 )
        {
            res = sqlite3_step(statement);

            if ( res == SQLITE_ROW )
            {
                std::string user = (char*)sqlite3_column_text(statement, 0);
                listUser.push_back(user);
            }

            if ( res == SQLITE_DONE || res==SQLITE_ERROR)
            {
                std::cout << "Get user of service finish " << std::endl;
                break;
            }
        }
        sqlite3_finalize(statement);
    } else {
        return DATA_ERROR_SELECT_DB;
    }
    return DATA_SUCCESS;
}

ResponseCode DataManager::getAllWaitUserOfService(std::string serviceId, std::vector<std::string> &listWaitUser) {
    std::stringstream strm;
    strm << "SELECT * FROM USER_SERVICE WHERE SERVICE_ID='" << serviceId << "' AND STATUS_USER=" << WAITING << ";";

    std::string s = strm.str();
    char *str = &s[0];

    sqlite3_stmt *statement;

    char *query = str;

    if ( sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK )
    {
        int res = 0;

        while ( 1 )
        {
            res = sqlite3_step(statement);

            if ( res == SQLITE_ROW )
            {
                std::string user = (char*)sqlite3_column_text(statement, 0);
                listWaitUser.push_back(user);
            }

            if ( res == SQLITE_DONE || res==SQLITE_ERROR)
            {
                std::cout << "Get wait user of service finish " << std::endl;
                break;
            }
        }
        sqlite3_finalize(statement);
    } else {
        return DATA_ERROR_SELECT_DB;
    }
    return DATA_SUCCESS;
}

ResponseCode DataManager::getAllReadyServiceOfUser(std::string userEmail, std::vector<Service> &listReadyService) {
    std::stringstream strm;
    strm << "SELECT * FROM "
         << "SERVICE "
         << "INNER JOIN "
         << "(SELECT SERVICE_ID FROM USER_SERVICE WHERE USER_EMAIL='" << userEmail << "' AND STATUS_USER=" << READY << ") t1 "
         << "ON SERVICE.SERVICE_ID=t1.SERVICE_ID "
         << ";";

    std::string s = strm.str();
    char *str = &s[0];

    sqlite3_stmt *statement;

    char *query = str;

    if ( sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK )
    {
        int res = 0;

        while ( 1 )
        {
            res = sqlite3_step(statement);

            if ( res == SQLITE_ROW )
            {
                Service service;
                service.setServiceID((char*)sqlite3_column_text(statement, 0));
                service.setServiceName((char*)sqlite3_column_text(statement, 1));
                service.setProducer((char*)sqlite3_column_text(statement, 2));
                service.setStatus((bool)sqlite3_column_int(statement, 3));
                service.setNumberOfUser(sqlite3_column_int(statement, 4));
                service.setNumberOfWaitUser(sqlite3_column_int(statement, 5));
                service.setDescription((char*)sqlite3_column_text(statement, 6));
                service.setPathExcuteFile((char*)sqlite3_column_text(statement, 7));
                service.setPathShFile((char*)sqlite3_column_text(statement, 8));
                service.setShTemplate((char*)sqlite3_column_text(statement, 9));

                std::string stginstr = (char*)sqlite3_column_text(statement, 10);
                std::vector<std::string> dirs;
                if(stginstr.size() > 0){
                  std::stringstream ss(stginstr);
                  std::string path = "";
                  while (std::getline(ss, path, ';')) {
                      dirs.push_back(path);
                  }
                }
                service.setStageinDirs(dirs);

                std::string stgoutstr = (char*)sqlite3_column_text(statement, 11);
                std::vector<std::string> dirs2;
                if(stgoutstr.size() > 0){
                  std::stringstream ss(stgoutstr);
                  std::string path;
                  while (std::getline(ss, path, ';')) {
                    dirs2.push_back(path);
                  }
                }
                service.setStageoutDirs(dirs2);

                service.setImageId((char*)sqlite3_column_text(statement, 12));
                service.setProvider((char*)sqlite3_column_text(statement, 13));
                service.setNumberOfNode(sqlite3_column_int(statement, 14));
                service.setMaxElapseTime((unsigned int)sqlite3_column_int(statement, 15));

                listReadyService.push_back(service);
            }

            if ( res == SQLITE_DONE || res==SQLITE_ERROR)
            {
                std::cout << "Get services of user finish " << std::endl;
                break;
            }
        }
        sqlite3_finalize(statement);
    } else {
        return DATA_ERROR_SELECT_DB;
    }
    return DATA_SUCCESS;
}

ResponseCode DataManager::getAllWaitServiceOfUser(std::string userEmail, std::vector<Service> &listWaitService) {
    std::stringstream strm;
    strm << "SELECT * FROM "
         << "SERVICE "
         << "INNER JOIN "
         << "(SELECT SERVICE_ID FROM USER_SERVICE WHERE USER_EMAIL='" << userEmail << "' AND STATUS_USER=" << WAITING << ") t1 "
         << "ON SERVICE.SERVICE_ID=t1.SERVICE_ID "
         << ";";


    std::string s = strm.str();
    char *str = &s[0];

    sqlite3_stmt *statement;

    char *query = str;

    if ( sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK )
    {
        int res = 0;

        while ( 1 )
        {
            res = sqlite3_step(statement);

            if ( res == SQLITE_ROW )
            {
                Service service;
                service.setServiceID((char*)sqlite3_column_text(statement, 0));
                service.setServiceName((char*)sqlite3_column_text(statement, 1));
                service.setProducer((char*)sqlite3_column_text(statement, 2));
                service.setStatus((bool)sqlite3_column_int(statement, 3));
                service.setNumberOfUser(sqlite3_column_int(statement, 4));
                service.setNumberOfWaitUser(sqlite3_column_int(statement, 5));
                service.setDescription((char*)sqlite3_column_text(statement, 6));
                service.setPathExcuteFile((char*)sqlite3_column_text(statement, 7));
                service.setPathShFile((char*)sqlite3_column_text(statement, 8));
                service.setShTemplate((char*)sqlite3_column_text(statement, 9));

                std::string stginstr = (char*)sqlite3_column_text(statement, 10);
                std::vector<std::string> dirs;
                if(stginstr.size() > 0){
                  std::stringstream ss(stginstr);
                  std::string path = "";
                  while (std::getline(ss, path, ';')) {
                      dirs.push_back(path);
                  }
                }
                service.setStageinDirs(dirs);

                std::string stgoutstr = (char*)sqlite3_column_text(statement, 11);
                std::vector<std::string> dirs2;
                if(stgoutstr.size() > 0){
                  std::stringstream ss(stgoutstr);
                  std::string path;
                  while (std::getline(ss, path, ';')) {
                    dirs2.push_back(path);
                  }
                }
                service.setStageoutDirs(dirs2);

                service.setImageId((char*)sqlite3_column_text(statement, 12));
                service.setProvider((char*)sqlite3_column_text(statement, 13));
                service.setNumberOfNode(sqlite3_column_int(statement, 14));
                service.setMaxElapseTime((unsigned int)sqlite3_column_int(statement, 15));
                listWaitService.push_back(service);
            }

            if ( res == SQLITE_DONE || res==SQLITE_ERROR)
            {
                std::cout << "Get wait services of user email finish " << std::endl;
                break;
            }
        }
        sqlite3_finalize(statement);
    } else {
        return DATA_ERROR_SELECT_DB;
    }
    return DATA_SUCCESS;
}

ResponseCode DataManager::checkNameService(std::string name) {
	ResponseCode ret;
	std::stringstream strm;
	strm << "SELECT * FROM SERVICE WHERE SERVICE_NAME='" << name << "';";

	std::string s = strm.str();
	char *str = &s[0];

	sqlite3_stmt *statement;

	char *query = str;

	if ( sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK )
	{
		int res = 0;

		res = sqlite3_step(statement);
		if ( res == SQLITE_ROW )
		{
			ret = DATA_IS_EXISTED;
		} else {
			ret = DATA_SELECT_EMPTY;
		}
		sqlite3_finalize(statement);
	} else {
		ret = DATA_ERROR_SELECT_DB;
	}
	std::cout << "datalayer: checkNameService finish\n";
	return ret;
}

ResponseCode DataManager::getAllServiceByImage(std::string imageId, std::vector<Service> &listService) {
    std::stringstream strm;
    strm << "SELECT * FROM SERVICE WHERE IMAGE_ID='" << imageId << "'"
         << ";";


    std::string s = strm.str();
    char *str = &s[0];

    sqlite3_stmt *statement;

    char *query = str;

    if ( sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK )
    {
        int res = 0;

        while ( 1 )
        {
            res = sqlite3_step(statement);

            if ( res == SQLITE_ROW )
            {
                Service service;
                service.setServiceID((char*)sqlite3_column_text(statement, 0));
                service.setServiceName((char*)sqlite3_column_text(statement, 1));
                service.setProducer((char*)sqlite3_column_text(statement, 2));
                service.setStatus((bool)sqlite3_column_int(statement, 3));
                service.setNumberOfUser(sqlite3_column_int(statement, 4));
                service.setNumberOfWaitUser(sqlite3_column_int(statement, 5));
                service.setDescription((char*)sqlite3_column_text(statement, 6));
                service.setPathExcuteFile((char*)sqlite3_column_text(statement, 7));
                service.setPathShFile((char*)sqlite3_column_text(statement, 8));
                service.setShTemplate((char*)sqlite3_column_text(statement, 9));

                std::string stginstr = (char*)sqlite3_column_text(statement, 10);
                std::vector<std::string> dirs;
                if(stginstr.size() > 0){
                  std::stringstream ss(stginstr);
                  std::string path = "";
                  while (std::getline(ss, path, ';')) {
                      dirs.push_back(path);
                  }
                }
                service.setStageinDirs(dirs);

                std::string stgoutstr = (char*)sqlite3_column_text(statement, 11);
                std::vector<std::string> dirs2;
                if(stgoutstr.size() > 0){
                  std::stringstream ss(stgoutstr);
                  std::string path;
                  while (std::getline(ss, path, ';')) {
                    dirs2.push_back(path);
                  }
                }
                service.setStageoutDirs(dirs2);

                service.setImageId((char*)sqlite3_column_text(statement, 12));
                service.setProvider((char*)sqlite3_column_text(statement, 13));
                service.setNumberOfNode(sqlite3_column_int(statement, 14));
                service.setMaxElapseTime((unsigned int)sqlite3_column_int(statement, 15));
                listService.push_back(service);
            }

            if ( res == SQLITE_DONE || res==SQLITE_ERROR)
            {
                std::cout << "Get services by image finish " << std::endl;
                break;
            }
        }
        sqlite3_finalize(statement);
    } else {
        return DATA_ERROR_SELECT_DB;
    }
    return DATA_SUCCESS;
}

ResponseCode DataManager::insertImageDocker(ImageDocker &imgDocker) {
    /*
    "IMAGE_ID       NVARCHAR(100)   PRIMARY KEY NOT NULL," \
    "IMAGE_NAME     NVARCHAR(100)                       ," \
    "PROVIDER_EMAIL NVARCHAR(100)               NOT NULL," \
	"IS_PUBLIC      INT							NOT NULL," \
    "COMMENT        NVARCHAR(1000)                      ," \
    */
    std::stringstream strm;
    strm << "insert into IMAGES values("
         << "'" << imgDocker.getImageId() << "',"
         << "'" << imgDocker.getImageName() << "',"
         << "'" << imgDocker.getProviderEmail() << "',"
         << "" << imgDocker.getIsPublic() << ","
         << "'" << imgDocker.getComment() << "');";

    std::string s = strm.str();
    char *str = &s[0];

    char *zErrMsg = 0;
    int rc = sqlite3_exec(db, str, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n sql command: %s\n", zErrMsg, s.c_str());
        return DATA_ERROR_INSERT_DB;
    } else {
        std::cout << "Insert new image success \n";
    }
    return DATA_SUCCESS;
}

ResponseCode DataManager::updateImageDocker(ImageDocker &imgDocker) {
    std::stringstream strm;
    strm << "UPDATE IMAGES SET "
         << "IMAGE_NAME='" << imgDocker.getImageName()
         << "',PROVIDER_EMAIL='" << imgDocker.getProviderEmail()
         << "',IS_PUBLIC=" << imgDocker.getIsPublic()
         << ",COMMENT='" << imgDocker.getComment()
         << "' WHERE IMAGE_ID='" << imgDocker.getImageId() << "';";

    std::string s = strm.str();
    char *str = &s[0];

    char *zErrMsg = 0;
    int rc = sqlite3_exec(db, str, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        //fprintf(stderr, "SQL error: %s\n", zErrMsg);\

      fprintf(stderr, "SQL error: %s\n sql command: %s\n", zErrMsg, s.c_str());
        return DATA_ERROR_UPDATE_DB;
    } else {
        std::cout << "Update image success \n";
    }
    return DATA_SUCCESS;
}

ResponseCode DataManager::deleteImageDocker(ImageDocker &imgDocker) {
    std::stringstream strm;
    strm << "DELETE FROM IMAGES WHERE IMAGE_ID='" << imgDocker.getImageId() << "';";

    std::string s = strm.str();
    char *str = &s[0];

    char *zErrMsg = 0;
    int rc = sqlite3_exec(db, str, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        //fprintf(stderr, "SQL error: %s\n", zErrMsg);
        fprintf(stderr, "SQL error: %s\n sql command: %s\n", zErrMsg, str);
        return DATA_ERROR_DELETE_DB;
    } else {
        std::cout << "Delete image success \n";
    }
    return DATA_SUCCESS;
}

ResponseCode DataManager::getAllImageDockerOfProvider(std::string providerEmail, std::vector<ImageDocker>& imgDocker) {
	std::stringstream strm;
	strm << "SELECT * FROM IMAGES WHERE PROVIDER_EMAIL='" << providerEmail << "';";

	std::string s = strm.str();
	char *str = &s[0];

	sqlite3_stmt *statement;

	char *query = str;

	if ( sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK )
	{
        int res = 0;
		while ( 1 )
		{
			res = sqlite3_step(statement);

			if ( res == SQLITE_ROW )
			{

				ImageDocker image;
				image.setImageId((char*)sqlite3_column_text(statement, 0));
				image.setImageName((char*)sqlite3_column_text(statement, 1));
				image.setProviderEmail((char*)sqlite3_column_text(statement, 2));
				image.setIsPublic((bool)sqlite3_column_int(statement, 3));
				image.setComment((char*)sqlite3_column_text(statement, 4));
				imgDocker.push_back(image);
			}

			if ( res == SQLITE_DONE || res==SQLITE_ERROR)
			{
				std::cout << "getAllImageDockerOfProvider finish " << std::endl;
				break;
			}
		}
		sqlite3_finalize(statement);
	} else {
		return DATA_ERROR_SELECT_DB;
	}
	return DATA_SUCCESS;
}

ResponseCode DataManager::checkIdImageDocker(std::string id) {
	ResponseCode ret;
	std::stringstream strm;
	strm << "SELECT * FROM IMAGES WHERE IMAGE_ID='" << id << "';";

	std::string s = strm.str();
	char *str = &s[0];

	sqlite3_stmt *statement;

	char *query = str;

	if ( sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK )
	{
		int res = 0;

		res = sqlite3_step(statement);
		if ( res == SQLITE_ROW )
		{
			ret = DATA_IS_EXISTED;
		} else {
			ret = DATA_SELECT_EMPTY;
			for (std::map<std::string,ImageDocker>::iterator it=SampleImages.begin(); it!=SampleImages.end(); ++it) {
				if (it->second.getImageId().compare(id) == 0) {
					ret = DATA_IS_EXISTED;
					break;
				}
			}
		}
		sqlite3_finalize(statement);
	} else {
		ret = DATA_ERROR_SELECT_DB;
	}
	std::cout << "datalayer: checkIdImageDocker finish\n";
	return ret;
}

ResponseCode DataManager::checkNameImageDocker(std::string name) {
	ResponseCode ret;
	std::stringstream strm;
	strm << "SELECT * FROM IMAGES WHERE IMAGE_NAME='" << name << "' COLLATE NOCASE;";

	std::string s = strm.str();
	char *str = &s[0];

	sqlite3_stmt *statement;

	char *query = str;

	if ( sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK )
	{
		int res = 0;

		res = sqlite3_step(statement);
		if ( res == SQLITE_ROW )
		{
			ret = DATA_IS_EXISTED;
		} else {
            ret = DATA_SELECT_EMPTY;
            for(std::map<std::string, ImageDocker>::iterator it = SampleImages.begin(); it != SampleImages.end(); ++it) {
                ImageDocker id = it->second;
                if(id.getImageName().compare(name) == 0){
                    ret = DATA_IS_EXISTED;
                    break;
                }
            }
		}
		sqlite3_finalize(statement);
	} else {
		ret = DATA_ERROR_SELECT_DB;
	}
	std::cout << "datalayer: checkNameImageDocker finish\n";
	return ret;
}

ResponseCode DataManager::getImageDockerById(std::string id, ImageDocker &outImageDocker) {
	ResponseCode ret;
	std::stringstream strm;
	strm << "SELECT * FROM IMAGES WHERE IMAGE_ID='" << id << "';";

	std::string s = strm.str();
	char *str = &s[0];

	sqlite3_stmt *statement;

	char *query = str;

	if ( sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK )
	{
		int res = 0;

		res = sqlite3_step(statement);
		if ( res == SQLITE_ROW )
		{
			outImageDocker.setImageId((char*)sqlite3_column_text(statement, 0));
			outImageDocker.setImageName((char*)sqlite3_column_text(statement, 1));
			outImageDocker.setProviderEmail((char*)sqlite3_column_text(statement, 2));
			outImageDocker.setIsPublic((bool)sqlite3_column_int(statement, 3));
			outImageDocker.setComment((char*)sqlite3_column_text(statement, 4));
		} else {
            std::map<std::string,ImageDocker>::iterator it = SampleImages.find(id);
			if (it!=SampleImages.end()) {
				outImageDocker = it->second;
			} else {
				ret = DATA_SELECT_EMPTY;
			}
		}
		sqlite3_finalize(statement);
	} else {
		ret = DATA_ERROR_SELECT_DB;
	}
	std::cout << "datalayer: getImageDockerById finish\n";
	return ret;
}

ResponseCode DataManager::getImageDockerByName(std::string name, ImageDocker &outImageDocker) {
    ResponseCode ret = DATA_SUCCESS;
	std::stringstream strm;
	strm << "SELECT * FROM IMAGES WHERE IMAGE_NAME='" << name << "' COLLATE NOCASE;";

	std::string s = strm.str();
	char *str = &s[0];

	sqlite3_stmt *statement;

	char *query = str;

	if ( sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK )
	{
		int res = 0;

		res = sqlite3_step(statement);
		if ( res == SQLITE_ROW )
		{
			outImageDocker.setImageId((char*)sqlite3_column_text(statement, 0));
			outImageDocker.setImageName((char*)sqlite3_column_text(statement, 1));
			outImageDocker.setProviderEmail((char*)sqlite3_column_text(statement, 2));
			outImageDocker.setIsPublic((bool)sqlite3_column_int(statement, 3));
			outImageDocker.setComment((char*)sqlite3_column_text(statement, 4));
		} else {
            ret = DATA_SELECT_EMPTY;
            for(std::map<std::string, ImageDocker>::iterator it = SampleImages.begin(); it != SampleImages.end(); ++it) {
                ImageDocker id = it->second;
                if(id.getImageName().compare(name) == 0){
                    outImageDocker = id;
                    break;
                }
            }
		}
		sqlite3_finalize(statement);
	} else {
		ret = DATA_ERROR_SELECT_DB;
	}
	std::cout << "datalayer: getImageDockerByName finish\n";
	return ret;
}

ResponseCode DataManager::getAllPublicImageDocker(std::vector<ImageDocker> &listImages) {
	std::stringstream strm;
	strm << "SELECT * FROM IMAGES WHERE (IS_PUBLIC=1);";

	std::string s = strm.str();
	char *str = &s[0];

	sqlite3_stmt *statement;

	char *query = str;

	if ( sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK )
	{
		int res = 0;

		while ( 1 )
		{
			res = sqlite3_step(statement);
			if ( res == SQLITE_ROW )
			{
				ImageDocker image;
				image.setImageId((char*)sqlite3_column_text(statement, 0));
				image.setImageName((char*)sqlite3_column_text(statement, 1));
				image.setProviderEmail((char*)sqlite3_column_text(statement, 2));
				image.setIsPublic((bool)sqlite3_column_int(statement, 3));
				image.setComment((char*)sqlite3_column_text(statement, 4));
				listImages.push_back(image);
			}

			if ( res == SQLITE_DONE || res==SQLITE_ERROR)
			{
				std::cout << "getAllPublicImageDocker finish " << std::endl;
				break;
			}
		}
		sqlite3_finalize(statement);
	} else {
		return DATA_ERROR_SELECT_DB;
	}
	return DATA_SUCCESS;
}
