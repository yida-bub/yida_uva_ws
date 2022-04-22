# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "uva_control_topic: 1 messages, 0 services")

set(MSG_I_FLAGS "-Iuva_control_topic:/home/xyd/workspace/yida_uva_ws/src/uva_control_topic/msg;-Istd_msgs:/opt/ros/noetic/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(uva_control_topic_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/xyd/workspace/yida_uva_ws/src/uva_control_topic/msg/msg_data_struct.msg" NAME_WE)
add_custom_target(_uva_control_topic_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "uva_control_topic" "/home/xyd/workspace/yida_uva_ws/src/uva_control_topic/msg/msg_data_struct.msg" ""
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(uva_control_topic
  "/home/xyd/workspace/yida_uva_ws/src/uva_control_topic/msg/msg_data_struct.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/uva_control_topic
)

### Generating Services

### Generating Module File
_generate_module_cpp(uva_control_topic
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/uva_control_topic
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(uva_control_topic_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(uva_control_topic_generate_messages uva_control_topic_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/xyd/workspace/yida_uva_ws/src/uva_control_topic/msg/msg_data_struct.msg" NAME_WE)
add_dependencies(uva_control_topic_generate_messages_cpp _uva_control_topic_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(uva_control_topic_gencpp)
add_dependencies(uva_control_topic_gencpp uva_control_topic_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS uva_control_topic_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(uva_control_topic
  "/home/xyd/workspace/yida_uva_ws/src/uva_control_topic/msg/msg_data_struct.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/uva_control_topic
)

### Generating Services

### Generating Module File
_generate_module_eus(uva_control_topic
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/uva_control_topic
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(uva_control_topic_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(uva_control_topic_generate_messages uva_control_topic_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/xyd/workspace/yida_uva_ws/src/uva_control_topic/msg/msg_data_struct.msg" NAME_WE)
add_dependencies(uva_control_topic_generate_messages_eus _uva_control_topic_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(uva_control_topic_geneus)
add_dependencies(uva_control_topic_geneus uva_control_topic_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS uva_control_topic_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(uva_control_topic
  "/home/xyd/workspace/yida_uva_ws/src/uva_control_topic/msg/msg_data_struct.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/uva_control_topic
)

### Generating Services

### Generating Module File
_generate_module_lisp(uva_control_topic
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/uva_control_topic
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(uva_control_topic_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(uva_control_topic_generate_messages uva_control_topic_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/xyd/workspace/yida_uva_ws/src/uva_control_topic/msg/msg_data_struct.msg" NAME_WE)
add_dependencies(uva_control_topic_generate_messages_lisp _uva_control_topic_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(uva_control_topic_genlisp)
add_dependencies(uva_control_topic_genlisp uva_control_topic_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS uva_control_topic_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(uva_control_topic
  "/home/xyd/workspace/yida_uva_ws/src/uva_control_topic/msg/msg_data_struct.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/uva_control_topic
)

### Generating Services

### Generating Module File
_generate_module_nodejs(uva_control_topic
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/uva_control_topic
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(uva_control_topic_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(uva_control_topic_generate_messages uva_control_topic_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/xyd/workspace/yida_uva_ws/src/uva_control_topic/msg/msg_data_struct.msg" NAME_WE)
add_dependencies(uva_control_topic_generate_messages_nodejs _uva_control_topic_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(uva_control_topic_gennodejs)
add_dependencies(uva_control_topic_gennodejs uva_control_topic_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS uva_control_topic_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(uva_control_topic
  "/home/xyd/workspace/yida_uva_ws/src/uva_control_topic/msg/msg_data_struct.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/uva_control_topic
)

### Generating Services

### Generating Module File
_generate_module_py(uva_control_topic
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/uva_control_topic
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(uva_control_topic_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(uva_control_topic_generate_messages uva_control_topic_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/xyd/workspace/yida_uva_ws/src/uva_control_topic/msg/msg_data_struct.msg" NAME_WE)
add_dependencies(uva_control_topic_generate_messages_py _uva_control_topic_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(uva_control_topic_genpy)
add_dependencies(uva_control_topic_genpy uva_control_topic_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS uva_control_topic_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/uva_control_topic)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/uva_control_topic
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(uva_control_topic_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/uva_control_topic)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/uva_control_topic
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(uva_control_topic_generate_messages_eus std_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/uva_control_topic)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/uva_control_topic
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(uva_control_topic_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/uva_control_topic)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/uva_control_topic
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(uva_control_topic_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/uva_control_topic)
  install(CODE "execute_process(COMMAND \"/usr/bin/python3\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/uva_control_topic\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/uva_control_topic
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(uva_control_topic_generate_messages_py std_msgs_generate_messages_py)
endif()
