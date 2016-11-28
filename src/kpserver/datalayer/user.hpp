#ifndef USER_HPP
#define USER_HPP

enum TypeUser {
    USER_GROUP = 1,
	PROVIDER_GROUP = 2,
    ADMIN_GROUP = 4,
    WAIT_PROVIDER_GROUP = 8
};

class User {
private:
    std::string email;
	unsigned char typeUser;
    std::string container_id;
	std::string _phone;
	std::string _address;
  std::string _kdesk_acc;
  //int _kdesk_port;
  std::string _public_key;
  std::string _description;
  double _time_usable;
  double _time_usage;

public:
    User();
    User(std::string e);
	User(std::string e, unsigned char type);
    ~User();
    void setEmail(std::string email_str);
	std::string getEmail() const;

	void setType(unsigned char type);
	unsigned char getType()const;

    void setContainerId(std::string id);
	std::string getContainerId()const;

	void setPhone(std::string phone);
	std::string getPhone()const;

  void setAddress(std::string address);
  std::string getAddress()const;

  void setKdeskAcc(std::string kdesk_acc);
  std::string getKdeskAcc()const;

//  void setKdeskPort(int port);
//  int getKdeskPort()const;

  void setPublicKey(std::string pk);
  std::string getPublicKey()const;

  double getTimeUsable()const;
  void setTimeUsable(double tu);

  double getTimeUsage()const;
  void setTimeUsage(double tu);

  void setDescription(std::string description);
	std::string getDescription()const;
};
#endif //USER_HPP
