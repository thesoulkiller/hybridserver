#ifndef __DBHANDLER__
#define __DBHANDLER__


#include <pqxx/pqxx>

#include <boost/shared_ptr.hpp>

#include <iostream>
#include <string>

#include "Types.hpp"

/*
 * SYNCHRONOUS VERSION
 *
 */
class DBHandler
{
public:
  static DBHandler *instance();
  void init(std::string opts);

  pqxx::result query(std::string sql);
  bool putStatus(Status &status);


private:
  boost::shared_ptr<pqxx::connection> c;
  boost::shared_ptr<pqxx::work> w;
  pqxx::result r;


private:

  static DBHandler *s_instance;
  DBHandler() {}
};

#endif
