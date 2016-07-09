// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: MsgUserInfo.proto

#ifndef PROTOBUF_MsgUserInfo_2eproto__INCLUDED
#define PROTOBUF_MsgUserInfo_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2005000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message_lite.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
// @@protoc_insertion_point(includes)

namespace NetMessage {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_MsgUserInfo_2eproto();
void protobuf_AssignDesc_MsgUserInfo_2eproto();
void protobuf_ShutdownFile_MsgUserInfo_2eproto();

class C2SReqUserInfo;
class S2CUserInfo;
class C2SReqCreateRole;
class S2CCreateRole;
class C2SReqEnterScene;
class S2CEnterScene;

enum S2CCreateRole_ERet {
  S2CCreateRole_ERet_OK = 1,
  S2CCreateRole_ERet_PlayerNameAlreadyExist = 2
};
bool S2CCreateRole_ERet_IsValid(int value);
const S2CCreateRole_ERet S2CCreateRole_ERet_ERet_MIN = S2CCreateRole_ERet_OK;
const S2CCreateRole_ERet S2CCreateRole_ERet_ERet_MAX = S2CCreateRole_ERet_PlayerNameAlreadyExist;
const int S2CCreateRole_ERet_ERet_ARRAYSIZE = S2CCreateRole_ERet_ERet_MAX + 1;

enum S2CEnterScene_ERet {
  S2CEnterScene_ERet_OK = 1,
  S2CEnterScene_ERet_Error = 2
};
bool S2CEnterScene_ERet_IsValid(int value);
const S2CEnterScene_ERet S2CEnterScene_ERet_ERet_MIN = S2CEnterScene_ERet_OK;
const S2CEnterScene_ERet S2CEnterScene_ERet_ERet_MAX = S2CEnterScene_ERet_Error;
const int S2CEnterScene_ERet_ERet_ARRAYSIZE = S2CEnterScene_ERet_ERet_MAX + 1;

// ===================================================================

class C2SReqUserInfo : public ::google::protobuf::MessageLite {
 public:
  C2SReqUserInfo();
  virtual ~C2SReqUserInfo();

  C2SReqUserInfo(const C2SReqUserInfo& from);

  inline C2SReqUserInfo& operator=(const C2SReqUserInfo& from) {
    CopyFrom(from);
    return *this;
  }

  static const C2SReqUserInfo& default_instance();

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  // Returns the internal default instance pointer. This function can
  // return NULL thus should not be used by the user. This is intended
  // for Protobuf internal code. Please use default_instance() declared
  // above instead.
  static inline const C2SReqUserInfo* internal_default_instance() {
    return default_instance_;
  }
  #endif

  void Swap(C2SReqUserInfo* other);

  // implements Message ----------------------------------------------

  C2SReqUserInfo* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const C2SReqUserInfo& from);
  void MergeFrom(const C2SReqUserInfo& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::std::string GetTypeName() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required uint64 accountID = 1;
  inline bool has_accountid() const;
  inline void clear_accountid();
  static const int kAccountIDFieldNumber = 1;
  inline ::google::protobuf::uint64 accountid() const;
  inline void set_accountid(::google::protobuf::uint64 value);

  // @@protoc_insertion_point(class_scope:NetMessage.C2SReqUserInfo)
 private:
  inline void set_has_accountid();
  inline void clear_has_accountid();

  ::google::protobuf::uint64 accountid_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  friend void  protobuf_AddDesc_MsgUserInfo_2eproto_impl();
  #else
  friend void  protobuf_AddDesc_MsgUserInfo_2eproto();
  #endif
  friend void protobuf_AssignDesc_MsgUserInfo_2eproto();
  friend void protobuf_ShutdownFile_MsgUserInfo_2eproto();

  void InitAsDefaultInstance();
  static C2SReqUserInfo* default_instance_;
};
// -------------------------------------------------------------------

class S2CUserInfo : public ::google::protobuf::MessageLite {
 public:
  S2CUserInfo();
  virtual ~S2CUserInfo();

  S2CUserInfo(const S2CUserInfo& from);

  inline S2CUserInfo& operator=(const S2CUserInfo& from) {
    CopyFrom(from);
    return *this;
  }

  static const S2CUserInfo& default_instance();

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  // Returns the internal default instance pointer. This function can
  // return NULL thus should not be used by the user. This is intended
  // for Protobuf internal code. Please use default_instance() declared
  // above instead.
  static inline const S2CUserInfo* internal_default_instance() {
    return default_instance_;
  }
  #endif

  void Swap(S2CUserInfo* other);

  // implements Message ----------------------------------------------

  S2CUserInfo* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const S2CUserInfo& from);
  void MergeFrom(const S2CUserInfo& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::std::string GetTypeName() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required uint64 playerID = 1;
  inline bool has_playerid() const;
  inline void clear_playerid();
  static const int kPlayerIDFieldNumber = 1;
  inline ::google::protobuf::uint64 playerid() const;
  inline void set_playerid(::google::protobuf::uint64 value);

  // optional string name = 2;
  inline bool has_name() const;
  inline void clear_name();
  static const int kNameFieldNumber = 2;
  inline const ::std::string& name() const;
  inline void set_name(const ::std::string& value);
  inline void set_name(const char* value);
  inline void set_name(const char* value, size_t size);
  inline ::std::string* mutable_name();
  inline ::std::string* release_name();
  inline void set_allocated_name(::std::string* name);

  // optional uint32 level = 3;
  inline bool has_level() const;
  inline void clear_level();
  static const int kLevelFieldNumber = 3;
  inline ::google::protobuf::uint32 level() const;
  inline void set_level(::google::protobuf::uint32 value);

  // optional uint32 exp = 4;
  inline bool has_exp() const;
  inline void clear_exp();
  static const int kExpFieldNumber = 4;
  inline ::google::protobuf::uint32 exp() const;
  inline void set_exp(::google::protobuf::uint32 value);

  // optional uint32 winCount = 5;
  inline bool has_wincount() const;
  inline void clear_wincount();
  static const int kWinCountFieldNumber = 5;
  inline ::google::protobuf::uint32 wincount() const;
  inline void set_wincount(::google::protobuf::uint32 value);

  // optional uint32 loseCount = 6;
  inline bool has_losecount() const;
  inline void clear_losecount();
  static const int kLoseCountFieldNumber = 6;
  inline ::google::protobuf::uint32 losecount() const;
  inline void set_losecount(::google::protobuf::uint32 value);

  // optional uint32 ranCount = 7;
  inline bool has_rancount() const;
  inline void clear_rancount();
  static const int kRanCountFieldNumber = 7;
  inline ::google::protobuf::uint32 rancount() const;
  inline void set_rancount(::google::protobuf::uint32 value);

  // repeated sint32 heros = 8;
  inline int heros_size() const;
  inline void clear_heros();
  static const int kHerosFieldNumber = 8;
  inline ::google::protobuf::int32 heros(int index) const;
  inline void set_heros(int index, ::google::protobuf::int32 value);
  inline void add_heros(::google::protobuf::int32 value);
  inline const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
      heros() const;
  inline ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
      mutable_heros();

  // @@protoc_insertion_point(class_scope:NetMessage.S2CUserInfo)
 private:
  inline void set_has_playerid();
  inline void clear_has_playerid();
  inline void set_has_name();
  inline void clear_has_name();
  inline void set_has_level();
  inline void clear_has_level();
  inline void set_has_exp();
  inline void clear_has_exp();
  inline void set_has_wincount();
  inline void clear_has_wincount();
  inline void set_has_losecount();
  inline void clear_has_losecount();
  inline void set_has_rancount();
  inline void clear_has_rancount();

  ::google::protobuf::uint64 playerid_;
  ::std::string* name_;
  ::google::protobuf::uint32 level_;
  ::google::protobuf::uint32 exp_;
  ::google::protobuf::uint32 wincount_;
  ::google::protobuf::uint32 losecount_;
  ::google::protobuf::RepeatedField< ::google::protobuf::int32 > heros_;
  ::google::protobuf::uint32 rancount_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(8 + 31) / 32];

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  friend void  protobuf_AddDesc_MsgUserInfo_2eproto_impl();
  #else
  friend void  protobuf_AddDesc_MsgUserInfo_2eproto();
  #endif
  friend void protobuf_AssignDesc_MsgUserInfo_2eproto();
  friend void protobuf_ShutdownFile_MsgUserInfo_2eproto();

  void InitAsDefaultInstance();
  static S2CUserInfo* default_instance_;
};
// -------------------------------------------------------------------

class C2SReqCreateRole : public ::google::protobuf::MessageLite {
 public:
  C2SReqCreateRole();
  virtual ~C2SReqCreateRole();

  C2SReqCreateRole(const C2SReqCreateRole& from);

  inline C2SReqCreateRole& operator=(const C2SReqCreateRole& from) {
    CopyFrom(from);
    return *this;
  }

  static const C2SReqCreateRole& default_instance();

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  // Returns the internal default instance pointer. This function can
  // return NULL thus should not be used by the user. This is intended
  // for Protobuf internal code. Please use default_instance() declared
  // above instead.
  static inline const C2SReqCreateRole* internal_default_instance() {
    return default_instance_;
  }
  #endif

  void Swap(C2SReqCreateRole* other);

  // implements Message ----------------------------------------------

  C2SReqCreateRole* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const C2SReqCreateRole& from);
  void MergeFrom(const C2SReqCreateRole& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::std::string GetTypeName() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required string name = 1;
  inline bool has_name() const;
  inline void clear_name();
  static const int kNameFieldNumber = 1;
  inline const ::std::string& name() const;
  inline void set_name(const ::std::string& value);
  inline void set_name(const char* value);
  inline void set_name(const char* value, size_t size);
  inline ::std::string* mutable_name();
  inline ::std::string* release_name();
  inline void set_allocated_name(::std::string* name);

  // optional uint64 accountID = 2;
  inline bool has_accountid() const;
  inline void clear_accountid();
  static const int kAccountIDFieldNumber = 2;
  inline ::google::protobuf::uint64 accountid() const;
  inline void set_accountid(::google::protobuf::uint64 value);

  // @@protoc_insertion_point(class_scope:NetMessage.C2SReqCreateRole)
 private:
  inline void set_has_name();
  inline void clear_has_name();
  inline void set_has_accountid();
  inline void clear_has_accountid();

  ::std::string* name_;
  ::google::protobuf::uint64 accountid_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  friend void  protobuf_AddDesc_MsgUserInfo_2eproto_impl();
  #else
  friend void  protobuf_AddDesc_MsgUserInfo_2eproto();
  #endif
  friend void protobuf_AssignDesc_MsgUserInfo_2eproto();
  friend void protobuf_ShutdownFile_MsgUserInfo_2eproto();

  void InitAsDefaultInstance();
  static C2SReqCreateRole* default_instance_;
};
// -------------------------------------------------------------------

class S2CCreateRole : public ::google::protobuf::MessageLite {
 public:
  S2CCreateRole();
  virtual ~S2CCreateRole();

  S2CCreateRole(const S2CCreateRole& from);

  inline S2CCreateRole& operator=(const S2CCreateRole& from) {
    CopyFrom(from);
    return *this;
  }

  static const S2CCreateRole& default_instance();

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  // Returns the internal default instance pointer. This function can
  // return NULL thus should not be used by the user. This is intended
  // for Protobuf internal code. Please use default_instance() declared
  // above instead.
  static inline const S2CCreateRole* internal_default_instance() {
    return default_instance_;
  }
  #endif

  void Swap(S2CCreateRole* other);

  // implements Message ----------------------------------------------

  S2CCreateRole* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const S2CCreateRole& from);
  void MergeFrom(const S2CCreateRole& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::std::string GetTypeName() const;

  // nested types ----------------------------------------------------

  typedef S2CCreateRole_ERet ERet;
  static const ERet OK = S2CCreateRole_ERet_OK;
  static const ERet PlayerNameAlreadyExist = S2CCreateRole_ERet_PlayerNameAlreadyExist;
  static inline bool ERet_IsValid(int value) {
    return S2CCreateRole_ERet_IsValid(value);
  }
  static const ERet ERet_MIN =
    S2CCreateRole_ERet_ERet_MIN;
  static const ERet ERet_MAX =
    S2CCreateRole_ERet_ERet_MAX;
  static const int ERet_ARRAYSIZE =
    S2CCreateRole_ERet_ERet_ARRAYSIZE;

  // accessors -------------------------------------------------------

  // required .NetMessage.S2CCreateRole.ERet ret = 1;
  inline bool has_ret() const;
  inline void clear_ret();
  static const int kRetFieldNumber = 1;
  inline ::NetMessage::S2CCreateRole_ERet ret() const;
  inline void set_ret(::NetMessage::S2CCreateRole_ERet value);

  // optional uint64 playerID = 2;
  inline bool has_playerid() const;
  inline void clear_playerid();
  static const int kPlayerIDFieldNumber = 2;
  inline ::google::protobuf::uint64 playerid() const;
  inline void set_playerid(::google::protobuf::uint64 value);

  // @@protoc_insertion_point(class_scope:NetMessage.S2CCreateRole)
 private:
  inline void set_has_ret();
  inline void clear_has_ret();
  inline void set_has_playerid();
  inline void clear_has_playerid();

  ::google::protobuf::uint64 playerid_;
  int ret_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  friend void  protobuf_AddDesc_MsgUserInfo_2eproto_impl();
  #else
  friend void  protobuf_AddDesc_MsgUserInfo_2eproto();
  #endif
  friend void protobuf_AssignDesc_MsgUserInfo_2eproto();
  friend void protobuf_ShutdownFile_MsgUserInfo_2eproto();

  void InitAsDefaultInstance();
  static S2CCreateRole* default_instance_;
};
// -------------------------------------------------------------------

class C2SReqEnterScene : public ::google::protobuf::MessageLite {
 public:
  C2SReqEnterScene();
  virtual ~C2SReqEnterScene();

  C2SReqEnterScene(const C2SReqEnterScene& from);

  inline C2SReqEnterScene& operator=(const C2SReqEnterScene& from) {
    CopyFrom(from);
    return *this;
  }

  static const C2SReqEnterScene& default_instance();

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  // Returns the internal default instance pointer. This function can
  // return NULL thus should not be used by the user. This is intended
  // for Protobuf internal code. Please use default_instance() declared
  // above instead.
  static inline const C2SReqEnterScene* internal_default_instance() {
    return default_instance_;
  }
  #endif

  void Swap(C2SReqEnterScene* other);

  // implements Message ----------------------------------------------

  C2SReqEnterScene* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const C2SReqEnterScene& from);
  void MergeFrom(const C2SReqEnterScene& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::std::string GetTypeName() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required uint64 accountID = 1;
  inline bool has_accountid() const;
  inline void clear_accountid();
  static const int kAccountIDFieldNumber = 1;
  inline ::google::protobuf::uint64 accountid() const;
  inline void set_accountid(::google::protobuf::uint64 value);

  // optional uint64 playerID = 2;
  inline bool has_playerid() const;
  inline void clear_playerid();
  static const int kPlayerIDFieldNumber = 2;
  inline ::google::protobuf::uint64 playerid() const;
  inline void set_playerid(::google::protobuf::uint64 value);

  // optional uint32 sceneID = 3;
  inline bool has_sceneid() const;
  inline void clear_sceneid();
  static const int kSceneIDFieldNumber = 3;
  inline ::google::protobuf::uint32 sceneid() const;
  inline void set_sceneid(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:NetMessage.C2SReqEnterScene)
 private:
  inline void set_has_accountid();
  inline void clear_has_accountid();
  inline void set_has_playerid();
  inline void clear_has_playerid();
  inline void set_has_sceneid();
  inline void clear_has_sceneid();

  ::google::protobuf::uint64 accountid_;
  ::google::protobuf::uint64 playerid_;
  ::google::protobuf::uint32 sceneid_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  friend void  protobuf_AddDesc_MsgUserInfo_2eproto_impl();
  #else
  friend void  protobuf_AddDesc_MsgUserInfo_2eproto();
  #endif
  friend void protobuf_AssignDesc_MsgUserInfo_2eproto();
  friend void protobuf_ShutdownFile_MsgUserInfo_2eproto();

  void InitAsDefaultInstance();
  static C2SReqEnterScene* default_instance_;
};
// -------------------------------------------------------------------

class S2CEnterScene : public ::google::protobuf::MessageLite {
 public:
  S2CEnterScene();
  virtual ~S2CEnterScene();

  S2CEnterScene(const S2CEnterScene& from);

  inline S2CEnterScene& operator=(const S2CEnterScene& from) {
    CopyFrom(from);
    return *this;
  }

  static const S2CEnterScene& default_instance();

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  // Returns the internal default instance pointer. This function can
  // return NULL thus should not be used by the user. This is intended
  // for Protobuf internal code. Please use default_instance() declared
  // above instead.
  static inline const S2CEnterScene* internal_default_instance() {
    return default_instance_;
  }
  #endif

  void Swap(S2CEnterScene* other);

  // implements Message ----------------------------------------------

  S2CEnterScene* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const S2CEnterScene& from);
  void MergeFrom(const S2CEnterScene& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::std::string GetTypeName() const;

  // nested types ----------------------------------------------------

  typedef S2CEnterScene_ERet ERet;
  static const ERet OK = S2CEnterScene_ERet_OK;
  static const ERet Error = S2CEnterScene_ERet_Error;
  static inline bool ERet_IsValid(int value) {
    return S2CEnterScene_ERet_IsValid(value);
  }
  static const ERet ERet_MIN =
    S2CEnterScene_ERet_ERet_MIN;
  static const ERet ERet_MAX =
    S2CEnterScene_ERet_ERet_MAX;
  static const int ERet_ARRAYSIZE =
    S2CEnterScene_ERet_ERet_ARRAYSIZE;

  // accessors -------------------------------------------------------

  // required .NetMessage.S2CEnterScene.ERet ret = 1;
  inline bool has_ret() const;
  inline void clear_ret();
  static const int kRetFieldNumber = 1;
  inline ::NetMessage::S2CEnterScene_ERet ret() const;
  inline void set_ret(::NetMessage::S2CEnterScene_ERet value);

  // @@protoc_insertion_point(class_scope:NetMessage.S2CEnterScene)
 private:
  inline void set_has_ret();
  inline void clear_has_ret();

  int ret_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  friend void  protobuf_AddDesc_MsgUserInfo_2eproto_impl();
  #else
  friend void  protobuf_AddDesc_MsgUserInfo_2eproto();
  #endif
  friend void protobuf_AssignDesc_MsgUserInfo_2eproto();
  friend void protobuf_ShutdownFile_MsgUserInfo_2eproto();

  void InitAsDefaultInstance();
  static S2CEnterScene* default_instance_;
};
// ===================================================================


// ===================================================================

// C2SReqUserInfo

// required uint64 accountID = 1;
inline bool C2SReqUserInfo::has_accountid() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void C2SReqUserInfo::set_has_accountid() {
  _has_bits_[0] |= 0x00000001u;
}
inline void C2SReqUserInfo::clear_has_accountid() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void C2SReqUserInfo::clear_accountid() {
  accountid_ = GOOGLE_ULONGLONG(0);
  clear_has_accountid();
}
inline ::google::protobuf::uint64 C2SReqUserInfo::accountid() const {
  return accountid_;
}
inline void C2SReqUserInfo::set_accountid(::google::protobuf::uint64 value) {
  set_has_accountid();
  accountid_ = value;
}

// -------------------------------------------------------------------

// S2CUserInfo

// required uint64 playerID = 1;
inline bool S2CUserInfo::has_playerid() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void S2CUserInfo::set_has_playerid() {
  _has_bits_[0] |= 0x00000001u;
}
inline void S2CUserInfo::clear_has_playerid() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void S2CUserInfo::clear_playerid() {
  playerid_ = GOOGLE_ULONGLONG(0);
  clear_has_playerid();
}
inline ::google::protobuf::uint64 S2CUserInfo::playerid() const {
  return playerid_;
}
inline void S2CUserInfo::set_playerid(::google::protobuf::uint64 value) {
  set_has_playerid();
  playerid_ = value;
}

// optional string name = 2;
inline bool S2CUserInfo::has_name() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void S2CUserInfo::set_has_name() {
  _has_bits_[0] |= 0x00000002u;
}
inline void S2CUserInfo::clear_has_name() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void S2CUserInfo::clear_name() {
  if (name_ != &::google::protobuf::internal::kEmptyString) {
    name_->clear();
  }
  clear_has_name();
}
inline const ::std::string& S2CUserInfo::name() const {
  return *name_;
}
inline void S2CUserInfo::set_name(const ::std::string& value) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  name_->assign(value);
}
inline void S2CUserInfo::set_name(const char* value) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  name_->assign(value);
}
inline void S2CUserInfo::set_name(const char* value, size_t size) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  name_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* S2CUserInfo::mutable_name() {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  return name_;
}
inline ::std::string* S2CUserInfo::release_name() {
  clear_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = name_;
    name_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void S2CUserInfo::set_allocated_name(::std::string* name) {
  if (name_ != &::google::protobuf::internal::kEmptyString) {
    delete name_;
  }
  if (name) {
    set_has_name();
    name_ = name;
  } else {
    clear_has_name();
    name_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// optional uint32 level = 3;
inline bool S2CUserInfo::has_level() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void S2CUserInfo::set_has_level() {
  _has_bits_[0] |= 0x00000004u;
}
inline void S2CUserInfo::clear_has_level() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void S2CUserInfo::clear_level() {
  level_ = 0u;
  clear_has_level();
}
inline ::google::protobuf::uint32 S2CUserInfo::level() const {
  return level_;
}
inline void S2CUserInfo::set_level(::google::protobuf::uint32 value) {
  set_has_level();
  level_ = value;
}

// optional uint32 exp = 4;
inline bool S2CUserInfo::has_exp() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void S2CUserInfo::set_has_exp() {
  _has_bits_[0] |= 0x00000008u;
}
inline void S2CUserInfo::clear_has_exp() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void S2CUserInfo::clear_exp() {
  exp_ = 0u;
  clear_has_exp();
}
inline ::google::protobuf::uint32 S2CUserInfo::exp() const {
  return exp_;
}
inline void S2CUserInfo::set_exp(::google::protobuf::uint32 value) {
  set_has_exp();
  exp_ = value;
}

// optional uint32 winCount = 5;
inline bool S2CUserInfo::has_wincount() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void S2CUserInfo::set_has_wincount() {
  _has_bits_[0] |= 0x00000010u;
}
inline void S2CUserInfo::clear_has_wincount() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void S2CUserInfo::clear_wincount() {
  wincount_ = 0u;
  clear_has_wincount();
}
inline ::google::protobuf::uint32 S2CUserInfo::wincount() const {
  return wincount_;
}
inline void S2CUserInfo::set_wincount(::google::protobuf::uint32 value) {
  set_has_wincount();
  wincount_ = value;
}

// optional uint32 loseCount = 6;
inline bool S2CUserInfo::has_losecount() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void S2CUserInfo::set_has_losecount() {
  _has_bits_[0] |= 0x00000020u;
}
inline void S2CUserInfo::clear_has_losecount() {
  _has_bits_[0] &= ~0x00000020u;
}
inline void S2CUserInfo::clear_losecount() {
  losecount_ = 0u;
  clear_has_losecount();
}
inline ::google::protobuf::uint32 S2CUserInfo::losecount() const {
  return losecount_;
}
inline void S2CUserInfo::set_losecount(::google::protobuf::uint32 value) {
  set_has_losecount();
  losecount_ = value;
}

// optional uint32 ranCount = 7;
inline bool S2CUserInfo::has_rancount() const {
  return (_has_bits_[0] & 0x00000040u) != 0;
}
inline void S2CUserInfo::set_has_rancount() {
  _has_bits_[0] |= 0x00000040u;
}
inline void S2CUserInfo::clear_has_rancount() {
  _has_bits_[0] &= ~0x00000040u;
}
inline void S2CUserInfo::clear_rancount() {
  rancount_ = 0u;
  clear_has_rancount();
}
inline ::google::protobuf::uint32 S2CUserInfo::rancount() const {
  return rancount_;
}
inline void S2CUserInfo::set_rancount(::google::protobuf::uint32 value) {
  set_has_rancount();
  rancount_ = value;
}

// repeated sint32 heros = 8;
inline int S2CUserInfo::heros_size() const {
  return heros_.size();
}
inline void S2CUserInfo::clear_heros() {
  heros_.Clear();
}
inline ::google::protobuf::int32 S2CUserInfo::heros(int index) const {
  return heros_.Get(index);
}
inline void S2CUserInfo::set_heros(int index, ::google::protobuf::int32 value) {
  heros_.Set(index, value);
}
inline void S2CUserInfo::add_heros(::google::protobuf::int32 value) {
  heros_.Add(value);
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
S2CUserInfo::heros() const {
  return heros_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
S2CUserInfo::mutable_heros() {
  return &heros_;
}

// -------------------------------------------------------------------

// C2SReqCreateRole

// required string name = 1;
inline bool C2SReqCreateRole::has_name() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void C2SReqCreateRole::set_has_name() {
  _has_bits_[0] |= 0x00000001u;
}
inline void C2SReqCreateRole::clear_has_name() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void C2SReqCreateRole::clear_name() {
  if (name_ != &::google::protobuf::internal::kEmptyString) {
    name_->clear();
  }
  clear_has_name();
}
inline const ::std::string& C2SReqCreateRole::name() const {
  return *name_;
}
inline void C2SReqCreateRole::set_name(const ::std::string& value) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  name_->assign(value);
}
inline void C2SReqCreateRole::set_name(const char* value) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  name_->assign(value);
}
inline void C2SReqCreateRole::set_name(const char* value, size_t size) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  name_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* C2SReqCreateRole::mutable_name() {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  return name_;
}
inline ::std::string* C2SReqCreateRole::release_name() {
  clear_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = name_;
    name_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void C2SReqCreateRole::set_allocated_name(::std::string* name) {
  if (name_ != &::google::protobuf::internal::kEmptyString) {
    delete name_;
  }
  if (name) {
    set_has_name();
    name_ = name;
  } else {
    clear_has_name();
    name_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// optional uint64 accountID = 2;
inline bool C2SReqCreateRole::has_accountid() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void C2SReqCreateRole::set_has_accountid() {
  _has_bits_[0] |= 0x00000002u;
}
inline void C2SReqCreateRole::clear_has_accountid() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void C2SReqCreateRole::clear_accountid() {
  accountid_ = GOOGLE_ULONGLONG(0);
  clear_has_accountid();
}
inline ::google::protobuf::uint64 C2SReqCreateRole::accountid() const {
  return accountid_;
}
inline void C2SReqCreateRole::set_accountid(::google::protobuf::uint64 value) {
  set_has_accountid();
  accountid_ = value;
}

// -------------------------------------------------------------------

// S2CCreateRole

// required .NetMessage.S2CCreateRole.ERet ret = 1;
inline bool S2CCreateRole::has_ret() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void S2CCreateRole::set_has_ret() {
  _has_bits_[0] |= 0x00000001u;
}
inline void S2CCreateRole::clear_has_ret() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void S2CCreateRole::clear_ret() {
  ret_ = 1;
  clear_has_ret();
}
inline ::NetMessage::S2CCreateRole_ERet S2CCreateRole::ret() const {
  return static_cast< ::NetMessage::S2CCreateRole_ERet >(ret_);
}
inline void S2CCreateRole::set_ret(::NetMessage::S2CCreateRole_ERet value) {
  assert(::NetMessage::S2CCreateRole_ERet_IsValid(value));
  set_has_ret();
  ret_ = value;
}

// optional uint64 playerID = 2;
inline bool S2CCreateRole::has_playerid() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void S2CCreateRole::set_has_playerid() {
  _has_bits_[0] |= 0x00000002u;
}
inline void S2CCreateRole::clear_has_playerid() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void S2CCreateRole::clear_playerid() {
  playerid_ = GOOGLE_ULONGLONG(0);
  clear_has_playerid();
}
inline ::google::protobuf::uint64 S2CCreateRole::playerid() const {
  return playerid_;
}
inline void S2CCreateRole::set_playerid(::google::protobuf::uint64 value) {
  set_has_playerid();
  playerid_ = value;
}

// -------------------------------------------------------------------

// C2SReqEnterScene

// required uint64 accountID = 1;
inline bool C2SReqEnterScene::has_accountid() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void C2SReqEnterScene::set_has_accountid() {
  _has_bits_[0] |= 0x00000001u;
}
inline void C2SReqEnterScene::clear_has_accountid() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void C2SReqEnterScene::clear_accountid() {
  accountid_ = GOOGLE_ULONGLONG(0);
  clear_has_accountid();
}
inline ::google::protobuf::uint64 C2SReqEnterScene::accountid() const {
  return accountid_;
}
inline void C2SReqEnterScene::set_accountid(::google::protobuf::uint64 value) {
  set_has_accountid();
  accountid_ = value;
}

// optional uint64 playerID = 2;
inline bool C2SReqEnterScene::has_playerid() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void C2SReqEnterScene::set_has_playerid() {
  _has_bits_[0] |= 0x00000002u;
}
inline void C2SReqEnterScene::clear_has_playerid() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void C2SReqEnterScene::clear_playerid() {
  playerid_ = GOOGLE_ULONGLONG(0);
  clear_has_playerid();
}
inline ::google::protobuf::uint64 C2SReqEnterScene::playerid() const {
  return playerid_;
}
inline void C2SReqEnterScene::set_playerid(::google::protobuf::uint64 value) {
  set_has_playerid();
  playerid_ = value;
}

// optional uint32 sceneID = 3;
inline bool C2SReqEnterScene::has_sceneid() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void C2SReqEnterScene::set_has_sceneid() {
  _has_bits_[0] |= 0x00000004u;
}
inline void C2SReqEnterScene::clear_has_sceneid() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void C2SReqEnterScene::clear_sceneid() {
  sceneid_ = 0u;
  clear_has_sceneid();
}
inline ::google::protobuf::uint32 C2SReqEnterScene::sceneid() const {
  return sceneid_;
}
inline void C2SReqEnterScene::set_sceneid(::google::protobuf::uint32 value) {
  set_has_sceneid();
  sceneid_ = value;
}

// -------------------------------------------------------------------

// S2CEnterScene

// required .NetMessage.S2CEnterScene.ERet ret = 1;
inline bool S2CEnterScene::has_ret() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void S2CEnterScene::set_has_ret() {
  _has_bits_[0] |= 0x00000001u;
}
inline void S2CEnterScene::clear_has_ret() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void S2CEnterScene::clear_ret() {
  ret_ = 1;
  clear_has_ret();
}
inline ::NetMessage::S2CEnterScene_ERet S2CEnterScene::ret() const {
  return static_cast< ::NetMessage::S2CEnterScene_ERet >(ret_);
}
inline void S2CEnterScene::set_ret(::NetMessage::S2CEnterScene_ERet value) {
  assert(::NetMessage::S2CEnterScene_ERet_IsValid(value));
  set_has_ret();
  ret_ = value;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace NetMessage

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_MsgUserInfo_2eproto__INCLUDED
