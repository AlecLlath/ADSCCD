/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: earth_mars.proto */

/* Do not generate deprecated warnings for self */
#ifndef PROTOBUF_C__NO_DEPRECATED
#define PROTOBUF_C__NO_DEPRECATED
#endif

#include "earth_mars.pb-c.h"
void   date__init
                     (Date         *message)
{
  static const Date init_value = DATE__INIT;
  *message = init_value;
}
size_t date__get_packed_size
                     (const Date *message)
{
  assert(message->base.descriptor == &date__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t date__pack
                     (const Date *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &date__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t date__pack_to_buffer
                     (const Date *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &date__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Date *
       date__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Date *)
     protobuf_c_message_unpack (&date__descriptor,
                                allocator, len, data);
}
void   date__free_unpacked
                     (Date *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &date__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   astronaut__init
                     (Astronaut         *message)
{
  static const Astronaut init_value = ASTRONAUT__INIT;
  *message = init_value;
}
size_t astronaut__get_packed_size
                     (const Astronaut *message)
{
  assert(message->base.descriptor == &astronaut__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t astronaut__pack
                     (const Astronaut *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &astronaut__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t astronaut__pack_to_buffer
                     (const Astronaut *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &astronaut__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Astronaut *
       astronaut__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Astronaut *)
     protobuf_c_message_unpack (&astronaut__descriptor,
                                allocator, len, data);
}
void   astronaut__free_unpacked
                     (Astronaut *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &astronaut__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   rescue_demand__init
                     (RescueDemand         *message)
{
  static const RescueDemand init_value = RESCUE_DEMAND__INIT;
  *message = init_value;
}
size_t rescue_demand__get_packed_size
                     (const RescueDemand *message)
{
  assert(message->base.descriptor == &rescue_demand__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t rescue_demand__pack
                     (const RescueDemand *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &rescue_demand__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t rescue_demand__pack_to_buffer
                     (const RescueDemand *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &rescue_demand__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
RescueDemand *
       rescue_demand__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (RescueDemand *)
     protobuf_c_message_unpack (&rescue_demand__descriptor,
                                allocator, len, data);
}
void   rescue_demand__free_unpacked
                     (RescueDemand *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &rescue_demand__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   rescue_ack__init
                     (RescueAck         *message)
{
  static const RescueAck init_value = RESCUE_ACK__INIT;
  *message = init_value;
}
size_t rescue_ack__get_packed_size
                     (const RescueAck *message)
{
  assert(message->base.descriptor == &rescue_ack__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t rescue_ack__pack
                     (const RescueAck *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &rescue_ack__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t rescue_ack__pack_to_buffer
                     (const RescueAck *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &rescue_ack__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
RescueAck *
       rescue_ack__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (RescueAck *)
     protobuf_c_message_unpack (&rescue_ack__descriptor,
                                allocator, len, data);
}
void   rescue_ack__free_unpacked
                     (RescueAck *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &rescue_ack__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   from_mars__init
                     (FromMars         *message)
{
  static const FromMars init_value = FROM_MARS__INIT;
  *message = init_value;
}
size_t from_mars__get_packed_size
                     (const FromMars *message)
{
  assert(message->base.descriptor == &from_mars__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t from_mars__pack
                     (const FromMars *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &from_mars__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t from_mars__pack_to_buffer
                     (const FromMars *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &from_mars__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
FromMars *
       from_mars__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (FromMars *)
     protobuf_c_message_unpack (&from_mars__descriptor,
                                allocator, len, data);
}
void   from_mars__free_unpacked
                     (FromMars *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &from_mars__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   rescue_offer__init
                     (RescueOffer         *message)
{
  static const RescueOffer init_value = RESCUE_OFFER__INIT;
  *message = init_value;
}
size_t rescue_offer__get_packed_size
                     (const RescueOffer *message)
{
  assert(message->base.descriptor == &rescue_offer__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t rescue_offer__pack
                     (const RescueOffer *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &rescue_offer__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t rescue_offer__pack_to_buffer
                     (const RescueOffer *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &rescue_offer__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
RescueOffer *
       rescue_offer__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (RescueOffer *)
     protobuf_c_message_unpack (&rescue_offer__descriptor,
                                allocator, len, data);
}
void   rescue_offer__free_unpacked
                     (RescueOffer *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &rescue_offer__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   from_earth__init
                     (FromEarth         *message)
{
  static const FromEarth init_value = FROM_EARTH__INIT;
  *message = init_value;
}
size_t from_earth__get_packed_size
                     (const FromEarth *message)
{
  assert(message->base.descriptor == &from_earth__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t from_earth__pack
                     (const FromEarth *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &from_earth__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t from_earth__pack_to_buffer
                     (const FromEarth *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &from_earth__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
FromEarth *
       from_earth__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (FromEarth *)
     protobuf_c_message_unpack (&from_earth__descriptor,
                                allocator, len, data);
}
void   from_earth__free_unpacked
                     (FromEarth *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &from_earth__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
static const ProtobufCFieldDescriptor date__field_descriptors[3] =
{
  {
    "year",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(Date, year),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "month",
    2,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(Date, month),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "day",
    3,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(Date, day),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned date__field_indices_by_name[] = {
  2,   /* field[2] = day */
  1,   /* field[1] = month */
  0,   /* field[0] = year */
};
static const ProtobufCIntRange date__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 3 }
};
const ProtobufCMessageDescriptor date__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "Date",
  "Date",
  "Date",
  "",
  sizeof(Date),
  3,
  date__field_descriptors,
  date__field_indices_by_name,
  1,  date__number_ranges,
  (ProtobufCMessageInit) date__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCEnumValue astronaut__health_status__enum_values_by_number[3] =
{
  { "GOOD", "ASTRONAUT__HEALTH_STATUS__GOOD", 0 },
  { "AILING", "ASTRONAUT__HEALTH_STATUS__AILING", 1 },
  { "DEAD", "ASTRONAUT__HEALTH_STATUS__DEAD", 2 },
};
static const ProtobufCIntRange astronaut__health_status__value_ranges[] = {
{0, 0},{0, 3}
};
static const ProtobufCEnumValueIndex astronaut__health_status__enum_values_by_name[3] =
{
  { "AILING", 1 },
  { "DEAD", 2 },
  { "GOOD", 0 },
};
const ProtobufCEnumDescriptor astronaut__health_status__descriptor =
{
  PROTOBUF_C__ENUM_DESCRIPTOR_MAGIC,
  "Astronaut.HealthStatus",
  "HealthStatus",
  "Astronaut__HealthStatus",
  "",
  3,
  astronaut__health_status__enum_values_by_number,
  3,
  astronaut__health_status__enum_values_by_name,
  1,
  astronaut__health_status__value_ranges,
  NULL,NULL,NULL,NULL   /* reserved[1234] */
};
static const ProtobufCFieldDescriptor astronaut__field_descriptors[2] =
{
  {
    "name",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Astronaut, name),
    NULL,
    &protobuf_c_empty_string,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "health_status",
    2,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_ENUM,
    0,   /* quantifier_offset */
    offsetof(Astronaut, health_status),
    &astronaut__health_status__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned astronaut__field_indices_by_name[] = {
  1,   /* field[1] = health_status */
  0,   /* field[0] = name */
};
static const ProtobufCIntRange astronaut__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor astronaut__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "Astronaut",
  "Astronaut",
  "Astronaut",
  "",
  sizeof(Astronaut),
  2,
  astronaut__field_descriptors,
  astronaut__field_indices_by_name,
  1,  astronaut__number_ranges,
  (ProtobufCMessageInit) astronaut__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor rescue_demand__field_descriptors[3] =
{
  {
    "id",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(RescueDemand, id),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "ask_before",
    2,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    0,   /* quantifier_offset */
    offsetof(RescueDemand, ask_before),
    &date__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "astronaut",
    3,
    PROTOBUF_C_LABEL_REPEATED,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(RescueDemand, n_astronaut),
    offsetof(RescueDemand, astronaut),
    &astronaut__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned rescue_demand__field_indices_by_name[] = {
  1,   /* field[1] = ask_before */
  2,   /* field[2] = astronaut */
  0,   /* field[0] = id */
};
static const ProtobufCIntRange rescue_demand__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 3 }
};
const ProtobufCMessageDescriptor rescue_demand__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "RescueDemand",
  "RescueDemand",
  "RescueDemand",
  "",
  sizeof(RescueDemand),
  3,
  rescue_demand__field_descriptors,
  rescue_demand__field_indices_by_name,
  1,  rescue_demand__number_ranges,
  (ProtobufCMessageInit) rescue_demand__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCEnumValue rescue_ack__rescue_ack_choice__enum_values_by_number[2] =
{
  { "REJECTED", "RESCUE_ACK__RESCUE_ACK_CHOICE__REJECTED", 0 },
  { "ACCEPTED", "RESCUE_ACK__RESCUE_ACK_CHOICE__ACCEPTED", 1 },
};
static const ProtobufCIntRange rescue_ack__rescue_ack_choice__value_ranges[] = {
{0, 0},{0, 2}
};
static const ProtobufCEnumValueIndex rescue_ack__rescue_ack_choice__enum_values_by_name[2] =
{
  { "ACCEPTED", 1 },
  { "REJECTED", 0 },
};
const ProtobufCEnumDescriptor rescue_ack__rescue_ack_choice__descriptor =
{
  PROTOBUF_C__ENUM_DESCRIPTOR_MAGIC,
  "RescueAck.RescueAckChoice",
  "RescueAckChoice",
  "RescueAck__RescueAckChoice",
  "",
  2,
  rescue_ack__rescue_ack_choice__enum_values_by_number,
  2,
  rescue_ack__rescue_ack_choice__enum_values_by_name,
  1,
  rescue_ack__rescue_ack_choice__value_ranges,
  NULL,NULL,NULL,NULL   /* reserved[1234] */
};
static const ProtobufCFieldDescriptor rescue_ack__field_descriptors[2] =
{
  {
    "id",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(RescueAck, id),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "choice",
    2,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_ENUM,
    0,   /* quantifier_offset */
    offsetof(RescueAck, choice),
    &rescue_ack__rescue_ack_choice__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned rescue_ack__field_indices_by_name[] = {
  1,   /* field[1] = choice */
  0,   /* field[0] = id */
};
static const ProtobufCIntRange rescue_ack__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor rescue_ack__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "RescueAck",
  "RescueAck",
  "RescueAck",
  "",
  sizeof(RescueAck),
  2,
  rescue_ack__field_descriptors,
  rescue_ack__field_indices_by_name,
  1,  rescue_ack__number_ranges,
  (ProtobufCMessageInit) rescue_ack__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor from_mars__field_descriptors[2] =
{
  {
    "rescue_demand",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(FromMars, type_case),
    offsetof(FromMars, rescue_demand),
    &rescue_demand__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "rescue_ack",
    2,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(FromMars, type_case),
    offsetof(FromMars, rescue_ack),
    &rescue_ack__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned from_mars__field_indices_by_name[] = {
  1,   /* field[1] = rescue_ack */
  0,   /* field[0] = rescue_demand */
};
static const ProtobufCIntRange from_mars__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor from_mars__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "FromMars",
  "FromMars",
  "FromMars",
  "",
  sizeof(FromMars),
  2,
  from_mars__field_descriptors,
  from_mars__field_indices_by_name,
  1,  from_mars__number_ranges,
  (ProtobufCMessageInit) from_mars__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor rescue_offer__field_descriptors[3] =
{
  {
    "id",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(RescueOffer, id),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "date",
    2,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    0,   /* quantifier_offset */
    offsetof(RescueOffer, date),
    &date__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "nb_places",
    3,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(RescueOffer, nb_places),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned rescue_offer__field_indices_by_name[] = {
  1,   /* field[1] = date */
  0,   /* field[0] = id */
  2,   /* field[2] = nb_places */
};
static const ProtobufCIntRange rescue_offer__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 3 }
};
const ProtobufCMessageDescriptor rescue_offer__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "RescueOffer",
  "RescueOffer",
  "RescueOffer",
  "",
  sizeof(RescueOffer),
  3,
  rescue_offer__field_descriptors,
  rescue_offer__field_indices_by_name,
  1,  rescue_offer__number_ranges,
  (ProtobufCMessageInit) rescue_offer__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor from_earth__field_descriptors[1] =
{
  {
    "rescue_offer",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(FromEarth, type_case),
    offsetof(FromEarth, rescue_offer),
    &rescue_offer__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned from_earth__field_indices_by_name[] = {
  0,   /* field[0] = rescue_offer */
};
static const ProtobufCIntRange from_earth__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 1 }
};
const ProtobufCMessageDescriptor from_earth__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "FromEarth",
  "FromEarth",
  "FromEarth",
  "",
  sizeof(FromEarth),
  1,
  from_earth__field_descriptors,
  from_earth__field_indices_by_name,
  1,  from_earth__number_ranges,
  (ProtobufCMessageInit) from_earth__init,
  NULL,NULL,NULL    /* reserved[123] */
};
