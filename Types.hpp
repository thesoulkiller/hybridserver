#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>
#include <map>
#include <sstream>

#include <jsoncpp/json/json.h>
#include <jsoncpp/json/value.h>

struct Status
{
  uint64_t created;
  int remaining;
  bool happy;

  Status() :
    created(0),happy(0)
  {
  }

  //insert one record
  std::string insertSql()
  {
    std::stringstream ss;
    ss << "INSERT INTO STATUS (" << "REMAINING, HAPPY";
    ss << ") VALUES( ";
    ss << "'" << remaining << "',";
    ss << "'" << happy << "');";
    return ss.str();
  }

  std::string selectSql()
  {
    std::stringstream ss;
    ss << "SELECT * FROM STATUS ST ";
    ss << "WHERE ST.CREATED =( ";
    ss << "SELECT MAX( CREATED ) FROM STATUS );";
    return ss.str();
  }

  Json::Value toJson()
  {
    Json::Value json;
    json["remaining"] = remaining;
    json["happy"] = happy;

    return json;
  }
  std::string toJsonString()
  {
    Json::StreamWriterBuilder wbuilder;
    wbuilder["indentation"] = "\t";

    Json::Value json;
    json["remaining"] = remaining;
    json["happy"] = (int)happy;

    return Json::writeString(wbuilder, json);
  }
  void fromJson(Json::Value &json) {
    remaining = json["remaining"].asInt();
    happy = json["happy"].asBool();
  }
};

#endif
