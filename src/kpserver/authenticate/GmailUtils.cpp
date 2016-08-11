#include "GmailUtils.hpp"
#include "CSmtp.h"

extern std::string g_admin_email_addr;
extern std::string g_admin_email_pass;

bool GmailUtils::GenLoginID(std::string& id)
{
    /// Part 1: date
    id = "";

    time_t rawtime;

    struct tm * timeinfo;

    char buffer [80];

    time ( &rawtime );

    timeinfo = localtime ( &rawtime );

    strftime (buffer,80,"%Y%m%d%H%M%S",timeinfo);

    id = string(buffer);

    //cout << "RequestId par 1:" << id << endl;

    srand (time(NULL));

    string appendStr = "";

    FILE *urandom;
    unsigned int seed;

    /// Part 2: random number
    for(int i = 0; i < 10; i++)
    {
        urandom = fopen ("/dev/urandom", "r");

        if (urandom == NULL) {
            fprintf (stderr, "Cannot open /dev/urandom!\n");
        }
        else
        {
            fread (&seed, sizeof (seed), 1, urandom);
            fclose (urandom);
        }

        srand (seed);

        int re = rand() % 10;

        stringstream ssret;

        ssret.str("");

        ssret << re;

        appendStr += ssret.str();
    }

    id += appendStr;

    //cout << "RequestId par 1 + 2:" << id << endl;

    return true;
}

/*
 * Process each chunk data receive
 */
size_t GmailUtils::write_to_string(void *ptr, size_t size, size_t count, void *stream) {
  ((string*)stream)->append((char*)ptr, 0, size*count);
  return size*count;
}


std::string Exec(const char* cmd) {
  std::string data;
  FILE * stream;
  const int max_buffer = 256;
  char buffer[max_buffer];
  std::string cmd_str = cmd;
  cmd_str.append(" 2>&1");

  stream = popen(cmd_str.c_str(), "r");
  if (stream) {
    while (!feof(stream))
    if (fgets(buffer, max_buffer, stream) != NULL)
      data.append(buffer);
    pclose(stream);
  }
  return data;
         /*
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while (!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != NULL)
            result += buffer;
    }
    return result;*/
}

/*
 *	Check email & token matched
 */
bool GmailUtils::Oauth20(std::string email, std::string token)
{
	std::cout << "Oauth20(email = " << email.c_str() << ", token = " << token.c_str() << ")"<< std::endl;
	///
	/// Confirm with curl command
	//
	std::string cmd = std::string("curl https://www.googleapis.com/oauth2/v1/userinfo?access_token=") + token;
	std::cout << "Cmd: " << cmd.c_str() << std::endl;
	std::string stdout = Exec(cmd.c_str());
	if (stdout.find(email) != std::string::npos) {
                        std::cout << "confirming gmail done!\n";
                        //result = true;
			return true;
                } else {
                        std::cout << "Email not matched!" << '\n';
                        std::cout << "Response string: " << stdout;
                }

	///
	return false;

	///
	/// Confirm use lib curl
	///
	std::stringstream ss;
	string response;
	std::string url;
	bool result = false;
	CURL *curl;
	CURLcode res;

	/// Request token to get gmail info
	curl_global_init(CURL_GLOBAL_DEFAULT);

	curl = curl_easy_init();
	if(curl) {
		ss << "https://www.googleapis.com/oauth2/v1/userinfo?access_token=" << token;
		url = ss.str();
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		std::cout << "Url: " << url << std::endl;
		//"https://www.googleapis.com/oauth2/v1/userinfo?access_token=ya29.EAFnh1eYjbTXykaiu33AwouVrHjDmzbLgCnNgN1aUHPITOUyYkiX3E73CU050p6WdlDpJLR-uDivXA");
		//access_token=ya29.EAFnh1eYjbTXykaiu33AwouVrHjDmzbLgCnNgN1aUHPITOUyYkiX3E73CU050p6WdlDpJLR-uDivXA&token_type=Bearer&expires_in=3600

		//#ifdef SKIP_PEER_VERIFICATION
		/*
		* If you want to connect to a site who isn't using a certificate that is
		* signed by one of the certs in the CA bundle you have, you can skip the
		* verification of the server's certificate. This makes the connection
		* A LOT LESS SECURE.
		*
		* If you have a CA cert for the server stored someplace else than in the
		* default bundle, then the CURLOPT_CAPATH option might come handy for
		* you.
		*/
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		//#endif


#ifdef SKIP_HOSTNAME_VERIFICATION
		/*
		* If the site you're connecting to uses a different host name that what
		* they have mentioned in their server certificate's commonName (or
		* subjectAltName) fields, libcurl will refuse to connect. You can skip
		* this check, but this will make the connection less secure.
		*/
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif
		/*
		 * a callback function to receive incoming data chunks
		 */
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_string);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);


		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);

		/* Check for errors */
		if(res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

		/* always cleanup */
		curl_easy_cleanup(curl);
	}
	else
	{
		std::cout << "ERR: curl can not create curl\n";
	}
	curl_global_cleanup();

	/// Check email invalid
	if(res == 0){
		if (response.find(email) != std::string::npos) {
			 result = true;
		} else {
			std::cout << "Email not matched!" << '\n';
			std::cout << "Response string: " << response;
		}
	} else {
		std::cout << "ERROR: Request get account by token fail!" << '\n';
	}
	result = true; //justtest
    return result;

}

bool GmailUtils::ClientLoginAuth(std::string userId, std::string pwd)
{
	bool result = false;

	curl_global_init( CURL_GLOBAL_ALL );
	CURL * myHandle = curl_easy_init ( );
	CURLcode curlCode;
	// Set up a couple initial paramaters that we will not need to mofiy later.
	curl_easy_setopt(myHandle, CURLOPT_USERAGENT, "Mozilla/4.0");
	curl_easy_setopt(myHandle, CURLOPT_AUTOREFERER, 1 );
	curl_easy_setopt(myHandle, CURLOPT_FOLLOWLOCATION, 1 );
	curl_easy_setopt(myHandle, CURLOPT_COOKIEFILE, "");


	// Visit the login page once to obtain a PHPSESSID cookie
	curl_easy_setopt(myHandle, CURLOPT_URL, "https://www.google.com/accounts/ClientLogin");

	// Now, can actually login. First we forge the HTTP referer field, or HTS will deny the login
	curl_easy_setopt(myHandle, CURLOPT_REFERER, "https://www.google.com/accounts/ClientLogin");


	// Next we tell LibCurl what HTTP POST data to submit
	//char *data="accountType=GOOGLE&Email=youremail@gmail.com&Passwd=yourpassword&service=apps&source=Google-cURL-Example";	
	const char url[] = "accountType=GOOGLE&Email=%s&Passwd=%s&service=apps&source=Google-cURL-Example";
	//printf("size = %i + %i + %i = %i\n", sizeof(url), userId.length(), pwd.length(), sizeof(url) + userId.length() + pwd.length());
    char *data = new char[sizeof(url) + userId.size() + pwd.length() + 1];
	sprintf(data, url, userId.c_str(), pwd.c_str());
	//printf("Data: %s\n", data);

	curl_easy_setopt(myHandle, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(myHandle, CURLOPT_POST, true);
	//curl_easy_setopt(myHandle, CURLOPT_RETURNTRANSFER, true);
	curl_easy_setopt(myHandle, CURLOPT_POSTFIELDS, data);
	
	//res = curl_easy_perform( myHandle );
	//CURLcode res = curl_easy_perform(curl);
	//long httpCode = 0;
	//curl_easy_getinfo(myHandle, CURLINFO_RESPONSE_CODE, &httpCode);
	//std::cout<<"httpCode:"<< httpCode << std::endl;

	//printf("res:%i\n", res, httpCode);
	//if(res != CURLE_OK)
	      //fprintf(stderr, "curl_easy_perform() failed: %s\n",
        	//      curl_easy_strerror(res));
	//else 
	//	result = true;

	curlCode = curl_easy_perform (myHandle);
	long httpCode = 0;
	curl_easy_getinfo (myHandle, CURLINFO_RESPONSE_CODE, &httpCode);
    std::cout<<"httpCode:"<< httpCode << ", responseCod:"<< curlCode << endl;
	
	if (httpCode == 200 && curlCode != CURLE_ABORTED_BY_CALLBACK)
	{
		//Succeeded
		result = true;
	}
	else
	{
		//Failed
		result = false;
	}
	curl_easy_cleanup( myHandle );
	if(data != NULL)
	{
		delete[] data;
		data = NULL;
	}
	return result;
	
}

/* This is a simple example showing how to send mail using libcurl's SMTP
 * capabilities. It builds on the smtp-mail.c example to add authentication
 * and, more importantly, transport security to protect the authentication
 * details from being snooped.
 *
 * Note that this example requires libcurl 7.20.0 or above.
 */


struct upload_data {
	int lines_read;
	const char** lines;
};

size_t GmailUtils::payload_source(void *ptr, size_t size, size_t nmemb, void *userp)
{
	struct upload_data *upload_ctx = (struct upload_data *)userp;
	const char *data;

	if((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
		return 0;
	}

	data = upload_ctx -> lines[upload_ctx->lines_read];

	if(data) {
		size_t len = strlen(data);
		memcpy(ptr, data, len);
		upload_ctx->lines_read++;
		return len;
	}
	return 0;
}

/*
 * Send email to provider notify create image success
 * Args:
 *  to: email receieve
 *  name: image name
 *  url: url download
 */

bool GmailUtils::SendEmail(std::string to, std::string name, std::string url, const std::string public_key_str){
  std::cout << "GmailUtils::SendEmail( to = " << to << ", name = " << name << ")\n";
    ///
    /// Confirm with curl command
    //
    /// Create message
    std::stringstream ss; std::string line;

    ss << "From: " << "Kportal admin <" << g_admin_email_addr << ">\r\n";
    ss << "To: <" << to << ">\r\n";
    ss << "Subject: [kportal] Accept provider register\r\n";
    ss << "Hi, " << to << "\r\n";
    ss << "Your email has accepted to Provider. You should logout and login again.\r\n";
    ss << "\r\n";
    ss << "Below is docker image base, you can use to create other image and upload to server. \r\n";
    ss << "Image name (repository): " << name << "\r\n";
    ss << "Download url: " << url << "\r\n";
    ss << "\r\n";
    ss << "And one more, please accept below public keys.\r\n" ;
    ss << public_key_str << "\r\n";
    ss << "\r\n";
    ss << "Regards\r\n";
    ss << "Kportal Admin\r\n";
    std::string msg = ss.str();

    std::string cmd = std::string(" echo \"" + msg + "\" | curl --url \"smtps://smtp.gmail.com:465\"  \
                                         --ssl-reqd   --mail-from \"" + g_admin_email_addr + "\" \
                                         --mail-rcpt \"" + to + "\"   \
                                         --user \"" + g_admin_email_addr + ":" + g_admin_email_pass + "\" --insecure;");
    std::cout << "Cmd: " << cmd.c_str() << std::endl;
    std::string stdout = Exec(cmd.c_str());
    if( stdout == ""){
      std::cout << "Send email have done!\n";
      return true;
    } else {
      std::cout << "[ERR] Send email got fail." << '\n';
      std::cout << "Response string: " << stdout;
    }

    return false;


  // Websocketpp error when send email & do something with tls
  bool bError = false;

  try
  {
    CSmtp mail;

//#define test_gmail_tls

//#if defined(test_gmail_tls)
    mail.SetSMTPServer("smtp.gmail.com",587);
    mail.SetSecurityType(USE_TLS);
//#elif defined(test_gmail_ssl)
//    mail.SetSMTPServer("smtp.gmail.com",465);
//    mail.SetSecurityType(USE_SSL);
//#elif defined(test_hotmail_TLS)
//    mail.SetSMTPServer("smtp.live.com",25);
//    mail.SetSecurityType(USE_TLS);
//#elif defined(test_aol_tls)
//    mail.SetSMTPServer("smtp.aol.com",587);
//    mail.SetSecurityType(USE_TLS);
//#elif defined(test_yahoo_ssl)
//    mail.SetSMTPServer("plus.smtp.mail.yahoo.com",465);
//    mail.SetSecurityType(USE_SSL);
//#endif

    mail.SetLogin(g_admin_email_addr.c_str());
    mail.SetPassword(g_admin_email_pass.c_str());
    mail.SetSenderName("Kportal Admin");
    mail.SetSenderMail(g_admin_email_pass.c_str());
    //mail.SetReplyTo(to);
    mail.SetSubject("[kportal] Accept provider register");
    mail.AddRecipient(to.c_str());
    mail.SetXPriority(XPRIORITY_NORMAL);
      //mail.SetXMailer("The Bat! (v3.02) Professional");

    /// Create message
    std::stringstream ss; std::string line;
    ss.str(""); ss << "Hi, " << to << ", your email has accepted to Provider. You should logout and login again.\r\n"; line = ss.str(); mail.AddMsgLine(line.c_str());
    mail.AddMsgLine("");
    line = std::string("Below is docker image base, you can use to create other image and upload to server. \r\n"); line = ss.str(); mail.AddMsgLine(line.c_str());
    ss.str(""); ss << "Image name (repository): " << name << "\r\n"; line = ss.str(); mail.AddMsgLine(line.c_str());
    ss.str(""); ss << "Download url: " << url << "\r\n"; line = ss.str(); mail.AddMsgLine(line.c_str());
    mail.AddMsgLine("");
    ss.str(""); ss << "And one more, please accept below public keys.\r\n" ;line = ss.str(); mail.AddMsgLine(line.c_str());
    mail.AddMsgLine(public_key_str.c_str());
    mail.AddMsgLine("");
    mail.AddMsgLine("Regards");
    mail.DelMsgLine(2);
    mail.AddMsgLine("Kportal Admin");


      //mail.AddAttachment("../test1.jpg");
      //mail.AddAttachment("c:\\test2.exe");
    //mail.AddAttachment("c:\\test3.txt");
    mail.Send();
  } catch(ECSmtp e) {
    std::cout << "Error: " << e.GetErrorText().c_str() << ".\n";
    bError = true;
  }
  if(!bError)
    std::cout << "Mail was send successfully.\n";
  return !bError;
}

