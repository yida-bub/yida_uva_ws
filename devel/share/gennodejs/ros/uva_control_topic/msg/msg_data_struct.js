// Auto-generated. Do not edit!

// (in-package uva_control_topic.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------

class msg_data_struct {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.x_vel = null;
      this.y_vel = null;
      this.z_vel = null;
      this.yaw = null;
    }
    else {
      if (initObj.hasOwnProperty('x_vel')) {
        this.x_vel = initObj.x_vel
      }
      else {
        this.x_vel = 0.0;
      }
      if (initObj.hasOwnProperty('y_vel')) {
        this.y_vel = initObj.y_vel
      }
      else {
        this.y_vel = 0.0;
      }
      if (initObj.hasOwnProperty('z_vel')) {
        this.z_vel = initObj.z_vel
      }
      else {
        this.z_vel = 0.0;
      }
      if (initObj.hasOwnProperty('yaw')) {
        this.yaw = initObj.yaw
      }
      else {
        this.yaw = 0.0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type msg_data_struct
    // Serialize message field [x_vel]
    bufferOffset = _serializer.float64(obj.x_vel, buffer, bufferOffset);
    // Serialize message field [y_vel]
    bufferOffset = _serializer.float64(obj.y_vel, buffer, bufferOffset);
    // Serialize message field [z_vel]
    bufferOffset = _serializer.float64(obj.z_vel, buffer, bufferOffset);
    // Serialize message field [yaw]
    bufferOffset = _serializer.float64(obj.yaw, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type msg_data_struct
    let len;
    let data = new msg_data_struct(null);
    // Deserialize message field [x_vel]
    data.x_vel = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [y_vel]
    data.y_vel = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [z_vel]
    data.z_vel = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [yaw]
    data.yaw = _deserializer.float64(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 32;
  }

  static datatype() {
    // Returns string type for a message object
    return 'uva_control_topic/msg_data_struct';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '7619d5b1e99f6dcea4d66f2ce7b13c29';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    float64	 x_vel
    float64	 y_vel
    float64	 z_vel
    float64	 yaw
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new msg_data_struct(null);
    if (msg.x_vel !== undefined) {
      resolved.x_vel = msg.x_vel;
    }
    else {
      resolved.x_vel = 0.0
    }

    if (msg.y_vel !== undefined) {
      resolved.y_vel = msg.y_vel;
    }
    else {
      resolved.y_vel = 0.0
    }

    if (msg.z_vel !== undefined) {
      resolved.z_vel = msg.z_vel;
    }
    else {
      resolved.z_vel = 0.0
    }

    if (msg.yaw !== undefined) {
      resolved.yaw = msg.yaw;
    }
    else {
      resolved.yaw = 0.0
    }

    return resolved;
    }
};

module.exports = msg_data_struct;
