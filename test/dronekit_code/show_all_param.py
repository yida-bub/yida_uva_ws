import time
from dronekit import connect, VehicleMode
print("开启连接 connect, 飞控连接中...")
vehicle = connect('/dev/ttyACM0', wait_ready=True, baud=921600)
print("飞控连接成功")
print("------------------------------")

print("\nPrint all parameters (iterate `vehicle.parameters`):")
for key, value in vehicle.parameters.iteritems():
    print(" Key:%s Value:%s"% (key,value))

print("------------------------------")
print("关闭飞控连接 conect")
vehicle.close()
