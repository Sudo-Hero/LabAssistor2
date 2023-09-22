#include "mysql_conn.h"

const string server = "tcp://localhost:3306";
const string username = "root";
const string password = "1122";
sql::Driver* driver;
sql::Connection* con;
sql::Statement* stmt;
sql::PreparedStatement* pstmt;

bool startMysqlConn(sql::Connection* conn) {
	
	FileLogger logger("log.txt");

	try
	{
		driver = get_driver_instance();
		con = driver->connect(server, username, password);
		logger.log("MYSQL Connected!!");
		return true;
	}
	catch (sql::SQLException e)
	{
		logger.log(e.what());
		return false;
	}


}