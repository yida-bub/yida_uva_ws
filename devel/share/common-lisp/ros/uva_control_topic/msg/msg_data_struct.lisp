; Auto-generated. Do not edit!


(cl:in-package uva_control_topic-msg)


;//! \htmlinclude msg_data_struct.msg.html

(cl:defclass <msg_data_struct> (roslisp-msg-protocol:ros-message)
  ((x_vel
    :reader x_vel
    :initarg :x_vel
    :type cl:float
    :initform 0.0)
   (y_vel
    :reader y_vel
    :initarg :y_vel
    :type cl:float
    :initform 0.0)
   (z_vel
    :reader z_vel
    :initarg :z_vel
    :type cl:float
    :initform 0.0)
   (yaw
    :reader yaw
    :initarg :yaw
    :type cl:float
    :initform 0.0))
)

(cl:defclass msg_data_struct (<msg_data_struct>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <msg_data_struct>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'msg_data_struct)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name uva_control_topic-msg:<msg_data_struct> is deprecated: use uva_control_topic-msg:msg_data_struct instead.")))

(cl:ensure-generic-function 'x_vel-val :lambda-list '(m))
(cl:defmethod x_vel-val ((m <msg_data_struct>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader uva_control_topic-msg:x_vel-val is deprecated.  Use uva_control_topic-msg:x_vel instead.")
  (x_vel m))

(cl:ensure-generic-function 'y_vel-val :lambda-list '(m))
(cl:defmethod y_vel-val ((m <msg_data_struct>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader uva_control_topic-msg:y_vel-val is deprecated.  Use uva_control_topic-msg:y_vel instead.")
  (y_vel m))

(cl:ensure-generic-function 'z_vel-val :lambda-list '(m))
(cl:defmethod z_vel-val ((m <msg_data_struct>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader uva_control_topic-msg:z_vel-val is deprecated.  Use uva_control_topic-msg:z_vel instead.")
  (z_vel m))

(cl:ensure-generic-function 'yaw-val :lambda-list '(m))
(cl:defmethod yaw-val ((m <msg_data_struct>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader uva_control_topic-msg:yaw-val is deprecated.  Use uva_control_topic-msg:yaw instead.")
  (yaw m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <msg_data_struct>) ostream)
  "Serializes a message object of type '<msg_data_struct>"
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'x_vel))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'y_vel))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'z_vel))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'yaw))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <msg_data_struct>) istream)
  "Deserializes a message object of type '<msg_data_struct>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'x_vel) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'y_vel) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'z_vel) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'yaw) (roslisp-utils:decode-double-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<msg_data_struct>)))
  "Returns string type for a message object of type '<msg_data_struct>"
  "uva_control_topic/msg_data_struct")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'msg_data_struct)))
  "Returns string type for a message object of type 'msg_data_struct"
  "uva_control_topic/msg_data_struct")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<msg_data_struct>)))
  "Returns md5sum for a message object of type '<msg_data_struct>"
  "7619d5b1e99f6dcea4d66f2ce7b13c29")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'msg_data_struct)))
  "Returns md5sum for a message object of type 'msg_data_struct"
  "7619d5b1e99f6dcea4d66f2ce7b13c29")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<msg_data_struct>)))
  "Returns full string definition for message of type '<msg_data_struct>"
  (cl:format cl:nil "float64	 x_vel~%float64	 y_vel~%float64	 z_vel~%float64	 yaw~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'msg_data_struct)))
  "Returns full string definition for message of type 'msg_data_struct"
  (cl:format cl:nil "float64	 x_vel~%float64	 y_vel~%float64	 z_vel~%float64	 yaw~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <msg_data_struct>))
  (cl:+ 0
     8
     8
     8
     8
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <msg_data_struct>))
  "Converts a ROS message object to a list"
  (cl:list 'msg_data_struct
    (cl:cons ':x_vel (x_vel msg))
    (cl:cons ':y_vel (y_vel msg))
    (cl:cons ':z_vel (z_vel msg))
    (cl:cons ':yaw (yaw msg))
))
