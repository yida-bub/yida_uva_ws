
(cl:in-package :asdf)

(defsystem "uva_control_topic-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "msg_data_struct" :depends-on ("_package_msg_data_struct"))
    (:file "_package_msg_data_struct" :depends-on ("_package"))
  ))