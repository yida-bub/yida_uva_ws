# yida_uva_ws

## 写在最前

- 本项目起源于作者本科毕业设计：无人机视觉智能感知系统开发
- 目前项目的控制算法存在很多不足之处，需要进一步优化
- 本项目基于Linux、ROS、Yolov5、C/C++

## Yolov5

- yolov5的使用：[目标检测 YOLOv5 开源代码项目调试与讲解实战【土堆 x 布尔艺数】-哔哩哔哩](https://www.bilibili.com/video/BV1tf4y1t7ru?spm_id_from=333.337.search-card.all.click)
  - `detect.py` 目标检测程序
    - 部分参数介绍
      - `--weights` 网络模型
      - `--source` 需要识别的图片或视频路径，或相机编号
      - `--imgsz` 图像大小
      - `--conf-thres` 相似度阀值
      - `--save-txt` 保存识别数据至 `.txt` 文件
      - `--view-img` 实时显示检测的图像
  - `train.py` 模型训练程序
    - 参数介绍，祥见视频
  - 数据集制作、模型训练
- Yolov5官方代码：https://github.com/ultralytics/yolov5
- 数据集制作web工具：[roboflow](https://roboflow.com/?ref=ultralytics)

## 目录说明

### `learning_topic` 目录

- 古月21讲课程中的自定义消息订阅与发布示例程序

### `object_track` 目录

- 目标识别跟踪任务
  - `object_track.hpp` 头文件，申明了各种头文件、宏、函数声明、变量等
  - `object_track_fun.cpp` 部分函数程序文件，函数的代码实现
  - `object_track_node.cpp` 目标识别跟踪主程序

### `offboard` 目录

- `takeoff.cpp` 与PX4官方mavros起飞示程序相似，offboard的板外控制自动起飞程序
- `offboard_test.cpp` 程序编写定义，功能步骤：起飞>x方向前进>类似在空中走“8”字路线>手动降落
- `velpoes_control.cpp` 这个不用管，懒得删了

### `uva_control_topic` 目录

- `data_dispose_publisher.cpp` 读取Yolov5目标识别的保存的数据文件测试程序，自己用来调试读取的数据是否正确

## 项目的使用

### takeoff起飞

- 机载电脑与PX飞控建立连接

  ```bash
  roslaunch mavros px4.launch
  ```

- 运行 `takeoff` 节点

  ```c
  rosrun offboard takeoff
  ```

- 将遥控器offboard开关闭合，油门推至中位，等待无人机起飞

### offboard 程序自动控制飞行

- 机载电脑与PX飞控建立连接

  ```bash	
  roslaunch mavros px4.launch
  ```

- 运行 `offboard_test` 节点

  ```bash
  rosrun offboard offboard_test
  ```

- 将遥控器offboard开关闭合，油门推至中位，无人机开始执行程序规划的路线：

  - 先起飞，起飞高度为 `TAKOFF_HIGH`
  - 向机体x方向 `1m/s` 移动 8s
  - 向机体x方向 `1m/s` 且偏航速度 `0.5` 移动 15s（往左画圈）
  - 向机体x方向 `1m/s` 且偏航速度 `-0.5` 移动 15s（晚右画圈）
  - 返回起飞点

- 程序退出方式：

  - 方法一（程序内写好的）：拨动6通道，使6通道的值大于1500
  - 方法二：` ctrl+c`

### 目标识别飞行（初步实现）

- 机载电脑与PX飞控建立连接

  ```bash
  roslaunch mavros px4.launch
  ```

- 运行Yolov5目标检测程序，如：

  ```bash
  python3 detect.py --weights yolov5s.pt --source 0 --view-img --conf-thres 0.40 --save-txt --classes 0
  ```

- 运行 `object_track_node` 目标跟踪节点

  ```bash
  rosrun object_track object_track_node
  ```

- 将遥控器offboard开关闭合，油门推至中位，无人机开始执行程序

  - 程序刚开始默认锁定的目标点在图像的中心，即 `(0.5, 0.5)` ，需要将目标中心点移至到图像中心，程序才会自动锁定目标
  - 待目标成功锁定后，目标移动，无人机将进行跟踪任务
  - ........

- **补充：控制算法需优化，程序代码需优化.......**

## 补充

- 代码的实现，详见程序代码，相对有较多的**中文**注释！