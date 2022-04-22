// Generated by gencpp from file uva_control_topic/msg_data_struct.msg
// DO NOT EDIT!


#ifndef UVA_CONTROL_TOPIC_MESSAGE_MSG_DATA_STRUCT_H
#define UVA_CONTROL_TOPIC_MESSAGE_MSG_DATA_STRUCT_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace uva_control_topic
{
template <class ContainerAllocator>
struct msg_data_struct_
{
  typedef msg_data_struct_<ContainerAllocator> Type;

  msg_data_struct_()
    : x_vel(0.0)
    , y_vel(0.0)
    , z_vel(0.0)
    , yaw(0.0)  {
    }
  msg_data_struct_(const ContainerAllocator& _alloc)
    : x_vel(0.0)
    , y_vel(0.0)
    , z_vel(0.0)
    , yaw(0.0)  {
  (void)_alloc;
    }



   typedef double _x_vel_type;
  _x_vel_type x_vel;

   typedef double _y_vel_type;
  _y_vel_type y_vel;

   typedef double _z_vel_type;
  _z_vel_type z_vel;

   typedef double _yaw_type;
  _yaw_type yaw;





  typedef boost::shared_ptr< ::uva_control_topic::msg_data_struct_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::uva_control_topic::msg_data_struct_<ContainerAllocator> const> ConstPtr;

}; // struct msg_data_struct_

typedef ::uva_control_topic::msg_data_struct_<std::allocator<void> > msg_data_struct;

typedef boost::shared_ptr< ::uva_control_topic::msg_data_struct > msg_data_structPtr;
typedef boost::shared_ptr< ::uva_control_topic::msg_data_struct const> msg_data_structConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::uva_control_topic::msg_data_struct_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::uva_control_topic::msg_data_struct_<ContainerAllocator> >::stream(s, "", v);
return s;
}


template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator==(const ::uva_control_topic::msg_data_struct_<ContainerAllocator1> & lhs, const ::uva_control_topic::msg_data_struct_<ContainerAllocator2> & rhs)
{
  return lhs.x_vel == rhs.x_vel &&
    lhs.y_vel == rhs.y_vel &&
    lhs.z_vel == rhs.z_vel &&
    lhs.yaw == rhs.yaw;
}

template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator!=(const ::uva_control_topic::msg_data_struct_<ContainerAllocator1> & lhs, const ::uva_control_topic::msg_data_struct_<ContainerAllocator2> & rhs)
{
  return !(lhs == rhs);
}


} // namespace uva_control_topic

namespace ros
{
namespace message_traits
{





template <class ContainerAllocator>
struct IsMessage< ::uva_control_topic::msg_data_struct_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::uva_control_topic::msg_data_struct_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::uva_control_topic::msg_data_struct_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::uva_control_topic::msg_data_struct_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::uva_control_topic::msg_data_struct_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::uva_control_topic::msg_data_struct_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::uva_control_topic::msg_data_struct_<ContainerAllocator> >
{
  static const char* value()
  {
    return "7619d5b1e99f6dcea4d66f2ce7b13c29";
  }

  static const char* value(const ::uva_control_topic::msg_data_struct_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x7619d5b1e99f6dceULL;
  static const uint64_t static_value2 = 0xa4d66f2ce7b13c29ULL;
};

template<class ContainerAllocator>
struct DataType< ::uva_control_topic::msg_data_struct_<ContainerAllocator> >
{
  static const char* value()
  {
    return "uva_control_topic/msg_data_struct";
  }

  static const char* value(const ::uva_control_topic::msg_data_struct_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::uva_control_topic::msg_data_struct_<ContainerAllocator> >
{
  static const char* value()
  {
    return "float64	 x_vel\n"
"float64	 y_vel\n"
"float64	 z_vel\n"
"float64	 yaw\n"
;
  }

  static const char* value(const ::uva_control_topic::msg_data_struct_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::uva_control_topic::msg_data_struct_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.x_vel);
      stream.next(m.y_vel);
      stream.next(m.z_vel);
      stream.next(m.yaw);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct msg_data_struct_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::uva_control_topic::msg_data_struct_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::uva_control_topic::msg_data_struct_<ContainerAllocator>& v)
  {
    s << indent << "x_vel: ";
    Printer<double>::stream(s, indent + "  ", v.x_vel);
    s << indent << "y_vel: ";
    Printer<double>::stream(s, indent + "  ", v.y_vel);
    s << indent << "z_vel: ";
    Printer<double>::stream(s, indent + "  ", v.z_vel);
    s << indent << "yaw: ";
    Printer<double>::stream(s, indent + "  ", v.yaw);
  }
};

} // namespace message_operations
} // namespace ros

#endif // UVA_CONTROL_TOPIC_MESSAGE_MSG_DATA_STRUCT_H
