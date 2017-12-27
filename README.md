# CarND-Path-Planning-Project
Self-Driving Car Engineer Nanodegree Program
Author: Shanglin Yang

   
### Simulator.
You can download the Term3 Simulator which contains the Path Planning Project from the [releases tab (https://github.com/udacity/self-driving-car-sim/releases).

### Goals
In this project your goal is to safely navigate around a virtual highway with other traffic that is driving +-10 MPH of the 50 MPH speed limit. You will be provided the car's localization and sensor fusion data, there is also a sparse map list of waypoints around the highway. The car should try to go as close as possible to the 50 MPH speed limit, which means passing slower traffic when possible, note that other cars will try to change lanes too. The car should avoid hitting other cars at all cost as well as driving inside of the marked road lanes at all times, unless going from one lane to another. The car should be able to make one complete loop around the 6946m highway. Since the car is trying to go 50 MPH, it should take a little over 5 minutes to complete 1 loop. Also the car should not experience total acceleration over 10 m/s^2 and jerk that is greater than 10 m/s^3.

## Valid Trajectories
[OK]The top right screen of the simulator shows the current/best miles driven without incident. Incidents include exceeding acceleration/jerk/speed, collision, and driving outside of the lanes. 
![drawing](https://github.com/shangliy/Udacity_CarND-Path-Planning/blob/master/car_screen.png?raw=true)
[OK] The car doesn't drive faster than the speed limit. Also the car isn't driving much slower than speed limit unless obstructed by traffic.
[OK]The car does not exceed a total acceleration of 10 m/s2 and a jerk of 10 m/s3.
[OK]The car must not come into contact with any of the other cars on the road.
[OK]The car doesn't spend more than a 3 second length out side the lane lanes during changing lanes, and every other time the car stays inside one of the 3 lanes on the right hand side of the road.
[OK]The car is able to smoothly change lanes when it makes sense to do so, such as when behind a slower moving car and an adjacent lane is clear of other traffic.

## Reflection

### Path Generation
1. Work Flow
The total work flow shown in Figure Below;
It includes two parts: **Behavior Plan** & **Path Generation**
![flow](https://github.com/shangliy/Udacity_CarND-Path-Planning/blob/master/udacity_pathplan.jpg?raw=true)

2. Behavior Plan
This part is to answer **two questions**:
(1) If we need to change the **lane** and which lane we need to switch to?  
 (2) What **speed reference** we need to set?
 > - Calculate all other car's future location and compare it with our car's location; Check if there exist car which is **in the same lane** and **the distance** is smaller than threshhold;  
 > - If no such car in the current lane, we will keep lane and add ref_speed if too small;
 > - If the current lane is not safem we need to check if other lane safe,:
 >> - if no safe lane, we will keep the lane and decrease the ref_speed;
 >> - If exist safe lane and the current lane is 0 or 2m,we will switch the lane to 1; else, we switch to the lane with smaller cost;
>>> - The cost is the minus minimum absolute distance between  the other car and our car within same lane; In other words, we want to switch the the lane with larger distance to other cars(safe);
>>> **// The lane status flags and cost array
	vector<bool> lane_notsafe = {false, false, false};
	vector<double> lane_cost = {0, 0, 0};**

3. Path Generation
This part is to generate several points which correspond to our next path;
> - Use last Pre_loc, current location and next three target location (**Defined by the lane**) as the sample points, **transfer them the car coordinate**;

>> ref_x = previous_path_x[prev_size - 1];
>>ref_y = previous_path_y[prev_size - 1];
>> double ref_x_pre = previous_path_x[prev_size - 2];
>> double ref_y_pre = previous_path_y[prev_size - 2];
>> vector<double> next_wp0 = getXY(car_s + 30, (2 + 4*cur_lane), map_waypoints_s, map_waypoints_x, map_waypoints_y);
>> vector<double> next_wp1 = getXY(car_s + 60, (2 + 4*cur_lane), map_waypoints_s, map_waypoints_x, map_waypoints_y);
>> vector<double> next_wp2 = getXY(car_s + 90, (2 + 4*cur_lane), map_waypoints_s, map_waypoints_x, map_waypoints_y);

> - Use **spline lib** and the  **five points** to fit the function **F**;
>> s.set_points(ptsx, ptsy);
> - According to the **Ref_speed** and the **target destination**, we sample seveal x and use the function f to caculate the F to get corresponding y, which make the generated next path points;
>> double x_point = x_add_on + (target_x)/N;
      double y_point = s(x_point);
> - Add Left pre_path points and New generated point to the next path array;
> - Send the next path arrat;


## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./path_planning`.

## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets)
  * Run either `install-mac.sh` or `install-ubuntu.sh`.
  * If you install from source, checkout to commit `e94b6e1`, i.e.
    ```
    git clone https://github.com/uWebSockets/uWebSockets 
    cd uWebSockets
    git checkout e94b6e1
    ```

