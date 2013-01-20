/* Copyright (c) 2012-2013 LevelDOWN contributors
 * See list at <https://github.com/rvagg/node-leveldown#contributing>
 * MIT +no-false-attribs License <https://github.com/rvagg/node-leveldown/blob/master/LICENSE>
 */

#ifndef LU_LEVELDOWN_H
#define LU_LEVELDOWN_H

#define LU_STR(key) \
  static Persistent<String> str_ ## key = Persistent<String>::New(String::New(#key));

#define LU_OPTION(key) \
  static Persistent<String> option_ ## key = Persistent<String>::New(String::New(#key));

#define LU_V8_METHOD(name) \
  static v8::Handle<v8::Value> name (const v8::Arguments& args);

#define CB_ERR_IF_NULL_OR_UNDEFINED(index, name) \
  if (args[index]->IsNull() || args[index]->IsUndefined()) { \
    Local<Value> argv[] = { \
      Local<Value>::New(Exception::Error(String::New("#name cannot be `null` or `undefined`"))) \
    }; \
    RunCallback(callback, argv, 1); \
    return Undefined(); \
  }

#define STRING_OR_BUFFER_TO_SLICE(to, from) \
  size_t to ## Sz_; \
  char* to ## Ch_; \
  if (Buffer::HasInstance(from->ToObject())) { \
    to ## Sz_ = Buffer::Length(from->ToObject()); \
    to ## Ch_ = Buffer::Data(from->ToObject()); \
  } else { \
    Local<String> to ## Str = from->ToString(); \
    to ## Sz_ = to ## Str->Utf8Length(); \
    to ## Ch_ = new char[to ## Sz_]; \
    to ## Str->WriteUtf8(to ## Ch_, -1, NULL, String::NO_NULL_TERMINATION); \
  } \
  Slice to(to ## Ch_, to ## Sz_);

#define BOOLEAN_OPTION_VALUE(optionsObj, opt) \
  bool opt = optionsObj->Has(option_ ## opt) && optionsObj->Get(option_ ## opt)->BooleanValue();
#define BOOLEAN_OPTION_VALUE_DEFTRUE(optionsObj, opt) \
  bool opt = !optionsObj->Has(option_ ## opt) || optionsObj->Get(option_ ## opt)->BooleanValue();
#define UINT32_OPTION_VALUE(optionsObj, opt, default) \
  uint32_t opt = optionsObj->Has(option_ ## opt) && optionsObj->Get(option_ ## opt)->IsUint32() ? optionsObj->Get(option_ ## opt)->Uint32Value() : default;

void RunCallback (v8::Persistent<v8::Function> callback, v8::Local<v8::Value> argv[], int length);

#endif