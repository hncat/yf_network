#include "config.h"

#include <fstream>
#include <iostream>
#include <string>

#include "json.h"
#include "stream_append.h"
#include "swapbyte.h"

using json = nlohmann::json;

int main(int argc, char **argv) {
  LOG_ADD_APPEND("out", new yf::log::outstream_append{});
  auto i_v = yf::config_ptr->look_up<int>("i_v", "int value", 10);
  auto array_v =
      yf::config_ptr->look_up<std::vector<int>>("array_v", "vector value");
  auto arr_arr_v =
      yf::config_ptr->look_up<std::vector<std::vector<int>>>("arr_arr_v", "vector vector value",{{123, 456}});
  auto map_v = yf::config_ptr->look_up<std::map<std::string, int>>("map_v", "map value");
  auto map_arr_v = yf::config_ptr->look_up<std::map<std::string, std::vector<int>>>("map_arr_v", "int value", {});
  LOG_INFO << "i_v: " << i_v->to_string();
  LOG_INFO << "array_v: " << array_v->to_string();
  LOG_INFO << "array_v: " << arr_arr_v->to_string();
  LOG_INFO << "map_v: " << map_v->to_string();
  LOG_INFO << "map_arr_v: " << map_arr_v->to_string();
  yf::config_ptr->load_config("test.json");
  LOG_INFO << "i_v: " << i_v->to_string();
  LOG_INFO << "array_v: " << array_v->to_string();
  LOG_INFO << "array_v: " << arr_arr_v->to_string();
  LOG_INFO << "map_v: " << map_v->to_string();
  LOG_INFO << "map_arr_v: " << map_arr_v->to_string();
  return 0;
}