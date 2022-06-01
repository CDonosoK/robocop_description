# 🤖 RoboCop🤖 
Small Robot, with LIDAR and DepthCamera. Using ROS for Maping and Navigation

Made by Clemente Donoso, 📍 Chile 🇨🇱

| RoboCop  | Lateral |
| ------------- | ------------- |
|![alt text](https://github.com/CDonosoK/AutonomousRobot/blob/main/images/RoboCop.png) |	![alt text](https://github.com/CDonosoK/AutonomousRobot/blob/main/images/RoboCop%20Lateral.png)|

| Front |	Back|
| ------------- | ------------- |
|![alt text](https://github.com/CDonosoK/AutonomousRobot/blob/main/images/RoboCop%20Front.png)| ![alt text](https://github.com/CDonosoK/AutonomousRobot/blob/main/images/RoboCop%20Back.png)

## About the project: ✏️
My title work seeks the construction and creation of a small autonomous mobile robot, capable of navigating, mapping and locating in controlled environments.
It will use sensors such as lidars and depth cameras to locate itself in the environment. 

As progress is made, new features will be added. First, nodes will be made that allow teleoperation, then nodes will be added to control the camera and track lines, then the lidar will be integrated and the limits of the environment will be identified to finish with autonomous navigation by the robot.

I will upload the code and the documentation in the next update and in the next iteration. Fell free to contact me if you have any questions.

## Last Update: 📅
- 2022 - June - 01: Restructuring the code and the documentation, and upload new codes
- 
-

## How to setup your workspace: 📋
- First you need to install ROS. For this project we will use [ROS Noetic](http://wiki.ros.org/noetic/Installation/Ubuntu), if you are using windows, you can use WSL.
- **Then you need to install the packages that we will use:**
    -
- **Once you have installed ROS and your packages, create your catkin workspace (with the name that you want):**
    ```
        mkdir -p ~/catkin_ws/src
        cd ~/catkin_ws
        catkin build
    ```
- **Then copy the repository to your catkin workspace:**
    ```
        cd ~/catkin_ws/src
        git clone https://github.com/CDonosoK/robocop_description
    ```

- **Finally, you need to source your catkin workspace:**
    ```
        source ~/catkin_ws/devel/setup.bash
    ```

**If everything went well, your workspace should be structure like this:**
```
    ~/catkin_ws
        |- build
        |- devel
        |- logs
        |- src
            |- robocop_description
                |- codes
                |- gazebo
                |- images
                |- ...
            |- ...
        
        |- ...

```

## How it's estructured: 📚
### Codes:
- The codes are in the folder `codes`, in this folder you can find the codes that are represented as nodes for controlling the robot, controlling the camera and tracking lines and other things.

### Gazebo:
- The gazebo main files such the materials, the physics and the plugins are in the folder `gazebo`, in this folder you can find evrything that is related to the gazebo.

### Launch
- The launch files are in the folder `launch`, in this folder you can find the launch files that are used to start the robot and the camera, and everything that is related to the gazebo.¿ and rviz.

### Worlds
- The worlds are in the folder `worlds`, in this folder you can find the worlds that will create the environment for the robot.

### Maps
- The maps are in the folder `maps`, in this folder you can find the maps that the robot create from the world and navigate with them.

### URDF
- The URDF files are in the folder `urdf`, in this folder you can find the URDF files that are used to create the robot.

## Shopping list: 🛒
When using ROS, Gazebo and Rviz for the simulations it will not be necessary to have the robot physically, however, the shopping list of the components that are used will be left here.
- [Raspberry](https://es.aliexpress.com/item/4000054878108.html?spm=a2g0o.order_list.0.0.6018194dlblAcN&gatewayAdapt=glo2esp)
- [11.1V 5200 mAh Battery](https://es.aliexpress.com/item/4000473771386.html?spm=a2g0s.9042311.0.0.274263c0vpR9Ad)
- [XL4016](https://es.aliexpress.com/item/1005002823834416.html?spm=a2g0s.9042311.0.0.274263c0jXHXAX)
- [Small LIDAR](https://es.aliexpress.com/item/4000251359842.html?spm=a2g0s.9042311.0.0.274263c0L7Hq96)
- [Depth camera - Orbbec Pro](https://es.aliexpress.com/item/1005002763661050.html?spm=a2g0s.9042311.0.0.41c263c0t63anU)
- [Metal Chasis](https://www.amazon.com/dp/B09131Z41C?psc=1&ref=ppx_yo2ov_dt_b_product_details)
- [Screen](https://es.aliexpress.com/item/32894346094.html?spm=a2g0o.order_list.0.0.6018194dlblAcN&gatewayAdapt=glo2esp)
- [Raspberry Pico](https://es.aliexpress.com/item/1005004271337949.html?spm=a2g0o.productlist.0.0.19df7250awZbxG&algo_pvid=82875a74-614a-490e-8fbf-19c74911b32d&algo_exp_id=82875a74-614a-490e-8fbf-19c74911b32d-5&pdp_ext_f=%7B%22sku_id%22%3A%2212000028584736395%22%7D&pdp_npi=2%40dis%21CLP%21%212421.0%21%21%211478.0%21%21%402100bddd16540745059073292e3a20%2112000028584736395%21sea)

## Contribute:
If you want to contribute monetarily to the creation of the project, you can do so via PayPal: https://paypal.me/cdonosok
