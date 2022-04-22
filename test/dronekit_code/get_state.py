from dronekit import connect, VehicleMode
print("开启连接 connect, 飞控连接中...")
# ip = '192.168.77.75:14550'
ip = '/dev/ttyACM0'
vehicle = connect(ip, wait_ready=True)
print("飞控连接成功")
print("------------------------------")

print("无人机属性信息输出：")
# # 飞控软件版本
# print(">>> Autopilot Firmware version: %s" % vehicle.version)
# # 全球定位信息（经纬度，高度相对于平均海平面）
# print(">>> Global Location: %s" % vehicle.location.global_frame)
# # 全球定位信息（经纬度，高度相对于home点）
# print(">>> Global Location (relative altitude): %s" % vehicle.location.global_relative_frame)
# # 相对home点的位置信息（向北、向东、向下）；解锁之前返回None
# print(">>> Local Location: %s" % vehicle.location.local_frame)
# # 无人机朝向（欧拉角：roll，pitch，yaw，单位为rad，范围-π到+π）
# print(">>> Attitude: %s" % vehicle.attitude)
# # 三维速度（m/s）
# print(">>> Velocity: %s" % vehicle.velocity)
# # GPS信息
# print(">>> GPS: %s" % vehicle.gps_0)
# # 地速（m/s）
# print(">>> Groundspeed: %s" % vehicle.groundspeed)
# # 空速（m/s）
# print(">>> Airspeed: %s" % vehicle.airspeed)
# # 云台信息（得到的为当前目标的roll, pitch, yaw，而非测量值。单位为度）
# print(">>> Gimbal status: %s" % vehicle.gimbal)
# # 电池信息
# print(">>> Battery: %s" % vehicle.battery)
# # EKF（拓展卡曼滤波器）状态
# print(">>> EKF OK?: %s" % vehicle.ekf_ok)
# # 超声波或激光雷达传感器状态
# print(">>> Rangefinder: %s" % vehicle.rangefinder)
# # 无人机朝向（度）
# print(">>> Heading: %s" % vehicle.heading)
# # 是否可以解锁
# print(">>> Is Armable?: %s" % vehicle.is_armable)
# # 系统状态
# print(">>> System status: %s" % vehicle.system_status.state)
# # 当前飞行模式
# print(">>> Mode: %s" % vehicle.mode.name)
# # 解锁状态
# print(">>> Armed: %s" % vehicle.armed)

print(">>> 飞控软件版本: %s" % vehicle.version)
print(">>> 全球定位信息 海平面 : %s" % vehicle.location.global_frame)
print(">>> 全球定位信息 home : %s" % vehicle.location.global_relative_frame)
print(">>> 相对home点的位置信息(向北、向东、向下)%s" % vehicle.location.local_frame)
print(">>> 无人机朝向 rad -π~+π): %s" % vehicle.attitude)
print(">>> 三维速度(m/s): %s" % vehicle.velocity)
print(">>> GPS信息: %s" % vehicle.gps_0)
print(">>> 地速 m/s: %s" % vehicle.groundspeed)
print(">>> 空速 m/s %s" % vehicle.airspeed)
print(">>> 云台信息 单位为度: %s" % vehicle.gimbal)
print(">>> 电池信息: %s" % vehicle.battery)
print(">>> EKF（拓展卡曼滤波器）状态: %s" % vehicle.ekf_ok)
print(">>> 超声波或激光雷达传感器状态: %s" % vehicle.rangefinder)
print(">>> 无人机朝向（度）: %s" % vehicle.heading)
print(">>> 是否可以解锁: %s" % vehicle.is_armable)
print(">>> 系统状态: %s" % vehicle.system_status.state)
print(">>> 当前飞行模式: %s" % vehicle.mode.name)
print(">>> 解锁状态: %s" % vehicle.armed)

print("------------------------------")
print("关闭飞控连接 conect")
vehicle.close()