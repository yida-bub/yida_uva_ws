;; Auto-generated. Do not edit!


(when (boundp 'uva_control_topic::msg_data_struct)
  (if (not (find-package "UVA_CONTROL_TOPIC"))
    (make-package "UVA_CONTROL_TOPIC"))
  (shadow 'msg_data_struct (find-package "UVA_CONTROL_TOPIC")))
(unless (find-package "UVA_CONTROL_TOPIC::MSG_DATA_STRUCT")
  (make-package "UVA_CONTROL_TOPIC::MSG_DATA_STRUCT"))

(in-package "ROS")
;;//! \htmlinclude msg_data_struct.msg.html


(defclass uva_control_topic::msg_data_struct
  :super ros::object
  :slots (_x_vel _y_vel _z_vel _yaw ))

(defmethod uva_control_topic::msg_data_struct
  (:init
   (&key
    ((:x_vel __x_vel) 0.0)
    ((:y_vel __y_vel) 0.0)
    ((:z_vel __z_vel) 0.0)
    ((:yaw __yaw) 0.0)
    )
   (send-super :init)
   (setq _x_vel (float __x_vel))
   (setq _y_vel (float __y_vel))
   (setq _z_vel (float __z_vel))
   (setq _yaw (float __yaw))
   self)
  (:x_vel
   (&optional __x_vel)
   (if __x_vel (setq _x_vel __x_vel)) _x_vel)
  (:y_vel
   (&optional __y_vel)
   (if __y_vel (setq _y_vel __y_vel)) _y_vel)
  (:z_vel
   (&optional __z_vel)
   (if __z_vel (setq _z_vel __z_vel)) _z_vel)
  (:yaw
   (&optional __yaw)
   (if __yaw (setq _yaw __yaw)) _yaw)
  (:serialization-length
   ()
   (+
    ;; float64 _x_vel
    8
    ;; float64 _y_vel
    8
    ;; float64 _z_vel
    8
    ;; float64 _yaw
    8
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float64 _x_vel
       (sys::poke _x_vel (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _y_vel
       (sys::poke _y_vel (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _z_vel
       (sys::poke _z_vel (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _yaw
       (sys::poke _yaw (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float64 _x_vel
     (setq _x_vel (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _y_vel
     (setq _y_vel (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _z_vel
     (setq _z_vel (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _yaw
     (setq _yaw (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;;
   self)
  )

(setf (get uva_control_topic::msg_data_struct :md5sum-) "7619d5b1e99f6dcea4d66f2ce7b13c29")
(setf (get uva_control_topic::msg_data_struct :datatype-) "uva_control_topic/msg_data_struct")
(setf (get uva_control_topic::msg_data_struct :definition-)
      "float64	 x_vel
float64	 y_vel
float64	 z_vel
float64	 yaw

")



(provide :uva_control_topic/msg_data_struct "7619d5b1e99f6dcea4d66f2ce7b13c29")


