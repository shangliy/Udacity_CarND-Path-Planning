# CarND-Controls-PID
Self-Driving Car Engineer Nanodegree Program

Final demo:

[DEMO](https://youtu.be/JPwS5MAKEGc)

---

## Project Specification

1. Compilation:

   The Code  compile without errors with `cmake` and `make`.

2. Implementation:

   The basic algorithm follows the update error and calculate steering angle using PID parameters  as introduced in lessons. 

   ​	Update error: 	PID::UpdateError ( PID.cpp line: 36-43) 

   ​	Calculate steering angle:  main.cpp line 74

3. Reflection:

   3.1: Effect each of the P, I, D

   ​

   3.1.1  Effect of P:

   ​	The proportional term produces an output value that is proportional to the current error value. A high proportional gain results in a large change in the output for a  given change in the error. If the proportional gain is too high, the system can become unstable.In contrast, a small gain results in a small output response to a large input error, and a less responsive or less sensitive controller.

   The real effect shown in the video below:

   | Ki= 0.0001, Kd= 0.1 | Kp = 0.0001                              | Kp = 1.0                                 |
   | ------------------- | ---------------------------------------- | ---------------------------------------- |
   |                     | [demo1](https://github.com/shangliy/CND_PID/blob/master/videos/k_0001.mp4) | [demo2](https://github.com/shangliy/CND_PID/blob/master/videos/k_1%20(online-video-cutter.com).mp) |

   ​

   3.1.2  Effect of D:

   ​	The derivative of the process error is calculated by determining the slope of the 
   error over time and multiplying this rate of change by the derivative 
   gain *K*d. The more the controller tries to change, the more it counteracts the effort. Thus, the derivative help to weak the oscillations. If the value is too small, it can not take effect. If the value is too large, it also cause more time to converge.

   | Kp= 0.1, Ki= 0.0001 | Kd = 0.1                                 | Kd = 5                                   |
   | ------------------- | ---------------------------------------- | ---------------------------------------- |
   |                     | [demo3](https://github.com/shangliy/CND_PID/blob/master/videos/k_0501.mp4) | [demo4](https://github.com/shangliy/CND_PID/blob/master/videos/k_0550.mp4) |

   ​

   3.1.3  Effect of I:

   ​	The contribution from the integral term is proportional to both the magnitude of the error and the duration of the error.The integral term accelerates the movement of the process towards setpoint and eliminates the residual steady-state error that occurs with a pure proportional controller. 

   If the I value too small, the steady-state error exit. However, if the value is too large, it can cause the present value to overshoot the setpoint value and oscillations.

   | Kp= 0.1, Kd= 1 | Ki = 0.0001                              | Ki = 1.0                                 |
   | -------------- | ---------------------------------------- | ---------------------------------------- |
   |                | [demo5](https://github.com/shangliy/CND_PID/blob/master/videos/k_02200001.mp4) | [demo6](https://github.com/shangliy/CND_PID/blob/master/videos/k_022001.mp4) |

   ​

   3.2: Hyperparameters Optimization

   Here I choose both manual tuning and twiddle.

   The first step is to use  manual tuning to find a good initial start point. Te reason to do that is twiddle is time consuming and we already have knowledge of these parameters. The guide line:

   - If the steering angle is not large enough to pass the turn, increase Kp by 0.01;


   - If the car can not run n stable sate in a straight line, increase Kd by 0.1;

   If the car can not run in the middle of the road, increase the Ki by 0.0001;

   Finally, my start point is (Kp,Ki,Kd) = (0.10.001,1.0)

   ​

   The second step is to use twiddle find the require parameter:

   The start gradients are (dKp,dKid,Kd) = (0.010.0001,0.1);

   I calculate the average err within the first 1000 steps as the guidance, the end condition is the error less than 0.25;

   The code implemented in PID::twiddle() (PID.cpp line: 80-119)

   The final  (Kp,Ki,Kd) = (0.193, 0.00096 , 2.533) , best_err =0. 2412

4. Simulation

   No tire may the drivable portion of the track surface. The car not pop up onto ledges or roll over any surfaces that would otherwise be considered unsafe.

   ​



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
* [uWebSockets](https://github.com/uWebSockets/uWebSockets) == 0.13, but the master branch will probably work just fine
  * Follow the instructions in the [uWebSockets README](https://github.com/uWebSockets/uWebSockets/blob/master/README.md) to get setup for your platform. You can download the zip of the appropriate version from the [releases page](https://github.com/uWebSockets/uWebSockets/releases). Here's a link to the [v0.13 zip](https://github.com/uWebSockets/uWebSockets/archive/v0.13.0.zip).
  * If you run OSX and have homebrew installed you can just run the ./install-mac.sh script to install this
* Simulator. You can download these from the [project intro page](https://github.com/udacity/CarND-PID-Control-Project/releases) in the classroom.

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`. 

## 


