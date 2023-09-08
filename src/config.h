#ifndef __YF_CONFIG_H__
#define __YF_CONFIG_H__

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

#include "json.h"
#include "lock.h"
#include "log.h"
#include "noncopyable.h"
#include "shared_mutex.h"

namespace yf {
/**
 * @brief config_var的基类
 * 不提供修改名称和描述的方法，设置的唯一时机在构造函数中。
 * 如果提供设置方法需要提供线程安全的处理
 */
class config_var_base : noncopyable {
 public:
  /**
   * @brief 构造config_var_base
   * @param name 名称
   * @param description 描述
   */
  config_var_base(const std::string &name = "",
                  const std::string &description = "")
      : _name{name}, _description{description} {}
  /**
   * @brief 析构
   */
  virtual ~config_var_base() = default;
  /**
   * @returns 名称
   */
  inline const std::string &name() const { return _name; }
  /**
   * @returns 描述
   */
  inline const std::string &description() const { return _description; }
  /**
   * @brief json转value
   * @param json jons数据
   */
  virtual void to_value(const nlohmann::json &json) = 0;
  /**
   * @brief config_var转可视化字符串
   * @returns std::string
   */
  virtual std::string to_string() = 0;

 protected:
  std::string _name;         // 配置名称
  std::string _description;  // 配置描述
};

template <typename T>
class config_var : public config_var_base {
 public:
  /**
   * @brief 构造config_var<T>
   * @param value 值
   * @param name 名称
   * @param description 描述
   */
  config_var(T &&value, const std::string &name = "",
             const std::string &description = "")
      : config_var_base{name, description}, _value{std::forward<T>(value)} {}
  /**
   * @returns value
  */
  inline const T &value() const { return _value; }
  /**
   * @brief json转value
   * @param json jons数据
   */
  inline void to_value(const nlohmann::json &json) override {
    try {
      _value = json.get<T>();
    } catch (std::exception &ex) {
      LOG_ERROR << "josn to value error, what: " << ex.what();
    }
  }
  /**
   * @brief config_var转可视化字符串
   * @returns std::string
   */
  inline std::string to_string() override {
    nlohmann::json json;
    json[_name] = _value;
    std::string json_str;
    try {
      json_str = json.dump(2);
    } catch (std::exception &ex) {
      LOG_ERROR << "json to string error, what: " << ex.what();
    }
    return json_str;
  }

 protected:
  T _value;
};

/**
 * @brief 线程安全的config，配置优于约定(约定既定义)
 * 通过使用读写锁实现多线程安全的config
 */
class config : noncopyable {
  SINGLETON_FRIEND_CLASS(config)

 public:
  virtual ~config() {
    for (auto it : _config_map) {
      delete it.second;
    }
    _config_map.clear();
  }
  /**
   * @brief 查找config_var
   * @param name config_var的名称
   */
  inline config_var_base *find(const std::string &name) {
    shared_lock<shared_mutex> lock{_mutex};
    auto it = _config_map.find(name);
    if (it == _config_map.end()) return nullptr;
    return it->second;
  }
  /**
   * @brief 定义配置信息
   * @param name 配置名称
   * @param descriotion 配置描述
   * @param v 配置的初始值
   * @returns config_var<T> *
   */
  template <typename T>
  inline config_var<T> *look_up(std::string name = "",
                                std::string description = "", T &&v = {}) {
    if (find(name)) {
      LOG_WARNING << "duplicate configuration definitions! config name: "
                  << name;
      return nullptr;
    }
    config_var<T> *var = nullptr;
    unique_lock<shared_mutex> lock{_mutex};
    {
      var = new config_var<T>{std::forward<T>(v), name, description};
      _config_map[name] = var;
    }
    return var;
  }
  /**
   * @brief 加载配置文件
   * @param filename 文件名称
   */
  void load_config(const std::string &filename) {
    std::fstream stream{filename};
    nlohmann::json conf;
    stream >> conf;
    for (auto it = conf.begin(); it != conf.end(); ++it) {
      auto var_base = find(it.key());
      if (var_base) {
        var_base->to_value(it.value());
      }
    }
  }

 private:
  config() = default;

 private:
  shared_mutex _mutex;
  std::unordered_map<std::string, config_var_base *> _config_map;  // 配置集合
};
typedef singleton<config> config_t;
static auto config_ptr = config_t::instance();
}  // namespace yf

#endif