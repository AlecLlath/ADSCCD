/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: olympic_games_registration.proto */

#ifndef PROTOBUF_C_olympic_5fgames_5fregistration_2eproto__INCLUDED
#define PROTOBUF_C_olympic_5fgames_5fregistration_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1003000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1003003 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct _Date Date;
typedef struct _StartTime StartTime;
typedef struct _Contest Contest;
typedef struct _Registration Registration;
typedef struct _Competitor Competitor;


/* --- enums --- */


/* --- messages --- */

struct  _Date
{
  ProtobufCMessage base;
  uint32_t year;
  uint32_t month;
  uint32_t day;
};
#define DATE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&date__descriptor) \
    , 0, 0, 0 }


struct  _StartTime
{
  ProtobufCMessage base;
  uint32_t hour;
  uint32_t minute;
};
#define START_TIME__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&start_time__descriptor) \
    , 0, 0 }


struct  _Contest
{
  ProtobufCMessage base;
  char *name;
  Date *date;
  StartTime *start_time;
};
#define CONTEST__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&contest__descriptor) \
    , (char *)protobuf_c_empty_string, NULL, NULL }


struct  _Registration
{
  ProtobufCMessage base;
  uint32_t bib_number;
  size_t n_contest;
  Contest **contest;
};
#define REGISTRATION__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&registration__descriptor) \
    , 0, 0,NULL }


struct  _Competitor
{
  ProtobufCMessage base;
  char *lastname;
  char *firstname;
  Date *birth_date;
};
#define COMPETITOR__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&competitor__descriptor) \
    , (char *)protobuf_c_empty_string, (char *)protobuf_c_empty_string, NULL }


/* Date methods */
void   date__init
                     (Date         *message);
size_t date__get_packed_size
                     (const Date   *message);
size_t date__pack
                     (const Date   *message,
                      uint8_t             *out);
size_t date__pack_to_buffer
                     (const Date   *message,
                      ProtobufCBuffer     *buffer);
Date *
       date__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   date__free_unpacked
                     (Date *message,
                      ProtobufCAllocator *allocator);
/* StartTime methods */
void   start_time__init
                     (StartTime         *message);
size_t start_time__get_packed_size
                     (const StartTime   *message);
size_t start_time__pack
                     (const StartTime   *message,
                      uint8_t             *out);
size_t start_time__pack_to_buffer
                     (const StartTime   *message,
                      ProtobufCBuffer     *buffer);
StartTime *
       start_time__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   start_time__free_unpacked
                     (StartTime *message,
                      ProtobufCAllocator *allocator);
/* Contest methods */
void   contest__init
                     (Contest         *message);
size_t contest__get_packed_size
                     (const Contest   *message);
size_t contest__pack
                     (const Contest   *message,
                      uint8_t             *out);
size_t contest__pack_to_buffer
                     (const Contest   *message,
                      ProtobufCBuffer     *buffer);
Contest *
       contest__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   contest__free_unpacked
                     (Contest *message,
                      ProtobufCAllocator *allocator);
/* Registration methods */
void   registration__init
                     (Registration         *message);
size_t registration__get_packed_size
                     (const Registration   *message);
size_t registration__pack
                     (const Registration   *message,
                      uint8_t             *out);
size_t registration__pack_to_buffer
                     (const Registration   *message,
                      ProtobufCBuffer     *buffer);
Registration *
       registration__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   registration__free_unpacked
                     (Registration *message,
                      ProtobufCAllocator *allocator);
/* Competitor methods */
void   competitor__init
                     (Competitor         *message);
size_t competitor__get_packed_size
                     (const Competitor   *message);
size_t competitor__pack
                     (const Competitor   *message,
                      uint8_t             *out);
size_t competitor__pack_to_buffer
                     (const Competitor   *message,
                      ProtobufCBuffer     *buffer);
Competitor *
       competitor__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   competitor__free_unpacked
                     (Competitor *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*Date_Closure)
                 (const Date *message,
                  void *closure_data);
typedef void (*StartTime_Closure)
                 (const StartTime *message,
                  void *closure_data);
typedef void (*Contest_Closure)
                 (const Contest *message,
                  void *closure_data);
typedef void (*Registration_Closure)
                 (const Registration *message,
                  void *closure_data);
typedef void (*Competitor_Closure)
                 (const Competitor *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor date__descriptor;
extern const ProtobufCMessageDescriptor start_time__descriptor;
extern const ProtobufCMessageDescriptor contest__descriptor;
extern const ProtobufCMessageDescriptor registration__descriptor;
extern const ProtobufCMessageDescriptor competitor__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_olympic_5fgames_5fregistration_2eproto__INCLUDED */
