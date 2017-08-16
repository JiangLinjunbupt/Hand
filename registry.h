/**
 * Simple registry implementation in 2 that uses static variables to
 * register object creators during program initialization time.
 */
#ifndef __REGISTRY_H__
#define __REGISTRY_H__

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <memory>
#include <mutex>
#include <map>
#include <string>
#include <vector>
using namespace std;

namespace kurff {


typedef int64_t TIndex;

template <typename Key, typename Value>
using Map = std::map<Key, Value>;



template <typename KeyType>
inline void PrintOffendingKey(const KeyType& key) {
  printf("[key type printing not supported]\n");
}

template <>
inline void PrintOffendingKey(const std::string& key) {
  printf("Offending key: %s.\n", key.c_str());
}

/**
 * @brief A template class that allows one to register classes by keys.
 *
 * The keys are usually a string specifying the name, but can be anything that
 * can be used in a std::map.
 *
 * You should most likely not use the Registry class explicitly, but use the
 * helper macros below to declare specific registries as well as registering
 * objects.
 */
template <class SrcType, class ObjectType, class... Args>
class Registry {
 public:
  typedef std::function<std::unique_ptr<ObjectType> (Args ...)> Creator;

  Registry() : registry_() {}

  void Register(const SrcType& key, Creator creator) {
    // The if statement below is essentially the same as the following line:
    // CHECK_EQ(registry_.count(key), 0) << "Key " << key
    //                                   << " registered twice.";
    // However, CHECK_EQ depends on google logging, and since registration is
    // carried out at static initialization time, we do not want to have an
    // explicit dependency on glog's initialization function.
    std::lock_guard<std::mutex> lock(register_mutex_);
    if (registry_.count(key) != 0) {
      printf("Key already registered.\n");
      PrintOffendingKey(key);
      std::exit(1);
    }
    registry_[key] = creator;
  }

  void Register(const SrcType& key, Creator creator, const string& help_msg) {
    Register(key, creator);
    help_message_[key] = help_msg;
  }

  inline bool Has(const SrcType& key) { return (registry_.count(key) != 0); }

  unique_ptr<ObjectType> Create(const SrcType& key, Args ... args) {
    if (registry_.count(key) == 0) {
      // Returns nullptr if the key is not registered.
      return nullptr;
    }
    return registry_[key](args...);
  }

  /**
   * Returns the keys currently registered as a vector.
   */
  vector<SrcType> Keys() {
    vector<SrcType> keys;
    for (const auto& it : registry_) {
      keys.push_back(it.first);
    }
    return keys;
  }

  const Map<SrcType, std::string>& HelpMessage() const {
    return help_message_;
  }

  const char* HelpMessage(const SrcType& key) const {
    auto it = help_message_.find(key);
    if (it == help_message_.end()) {
      return nullptr;
    }
    return it->second.c_str();
  }

 private:
  Map<SrcType, Creator> registry_;
  Map<SrcType, std::string> help_message_;
  std::mutex register_mutex_;

  //DISABLE_COPY_AND_ASSIGN(Registry);
};

template <class SrcType, class ObjectType, class... Args>
class Registerer {
 public:
  Registerer(const SrcType& key,
             Registry<SrcType, ObjectType, Args...>* registry,
             typename Registry<SrcType, ObjectType, Args...>::Creator creator,
             const string& help_msg="") {
    registry->Register(key, creator, help_msg);
  }

  template <class DerivedType>
  static unique_ptr<ObjectType> DefaultCreator(Args ... args) {
    // TODO(jiayq): old versions of NVCC does not handle make_unique well
    // so we are forced to use a unique_ptr constructor here. Check if it is
    // fine to use make_unique in the future.
    // return make_unique<DerivedType>(args...);
    return std::unique_ptr<ObjectType>(new DerivedType(args...));
  }
};

/**
 * ANONYMOUS_VARIABLE(str) introduces an identifier starting with
 * str and ending with a number that varies with the line.
 * Pretty much a copy from 'folly/Preprocessor.h'
 */
#define CONCATENATE_IMPL(s1, s2) s1##s2
#define CONCATENATE(s1, s2) CONCATENATE_IMPL(s1, s2)
#ifdef __COUNTER__
#define ANONYMOUS_VARIABLE(str) CONCATENATE(str, __COUNTER__)
#else
#define ANONYMOUS_VARIABLE(str) CONCATENATE(str, __LINE__)
#endif

/**
 * DECLARE_TYPED_REGISTRY is a macro that expands to a function
 * declaration, as well as creating a convenient typename for its corresponding
 * registerer.
 */
#define DECLARE_TYPED_REGISTRY(RegistryName, SrcType, ObjectType, ...) \
  Registry<SrcType, ObjectType, ##__VA_ARGS__>* RegistryName();              \
  typedef Registerer<SrcType, ObjectType, ##__VA_ARGS__>                     \
      Registerer##RegistryName;

#define DEFINE_TYPED_REGISTRY(RegistryName, SrcType, ObjectType, ...) \
  Registry<SrcType, ObjectType, ##__VA_ARGS__>* RegistryName() {            \
    static Registry<SrcType, ObjectType, ##__VA_ARGS__>* registry =         \
        new Registry<SrcType, ObjectType, ##__VA_ARGS__>();                 \
    return registry;                                                        \
  }

// Note(Yangqing): The __VA_ARGS__ below allows one to specify a templated
// creator with comma in its templated arguments.
#define REGISTER_TYPED_CREATOR(RegistryName, key, ...)                  \
  namespace {                                                                 \
  static Registerer##RegistryName ANONYMOUS_VARIABLE(g_##RegistryName)( \
      key, RegistryName(), __VA_ARGS__);                                      \
  }

#define REGISTER_TYPED_CLASS(RegistryName, key, ...)                    \
  namespace {                                                                 \
  static Registerer##RegistryName ANONYMOUS_VARIABLE(g_##RegistryName)( \
      key,                                                                    \
      RegistryName(),                                                         \
      Registerer##RegistryName::DefaultCreator<__VA_ARGS__>);                                         \
  }

// DECLARE_REGISTRY and DEFINE_REGISTRY are hard-wired to use string
// as the key
// type, because that is the most commonly used cases.
#define DECLARE_REGISTRY(RegistryName, ObjectType, ...) \
  DECLARE_TYPED_REGISTRY(                               \
      RegistryName, std::string, ObjectType, ##__VA_ARGS__)

#define DEFINE_REGISTRY(RegistryName, ObjectType, ...) \
  DEFINE_TYPED_REGISTRY(                               \
      RegistryName, std::string, ObjectType, ##__VA_ARGS__)

// REGISTER_CREATOR and REGISTER_CLASS are hard-wired to use string
// as the key
// type, because that is the most commonly used cases.
#define REGISTER_CREATOR(RegistryName, key, ...) \
  REGISTER_TYPED_CREATOR(RegistryName, #key, __VA_ARGS__)

#define REGISTER_CLASS(RegistryName, key, ...) \
  REGISTER_TYPED_CLASS(RegistryName, #key, __VA_ARGS__)

}  // namespace caffe2
#endif  // CAFFE2_CORE_REGISTRY_H_
