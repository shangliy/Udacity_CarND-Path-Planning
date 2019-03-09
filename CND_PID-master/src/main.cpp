#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include <math.h>

// If TWIDDLE_FLAG 0 , no twiddle, just run
// If TWIDDLE_FLAG 1 , twiddle the parameer
#define TWIDDLE_FLAG 0
#define Twiddle_Step 1000

// for convenience
using json = nlohmann::json;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
std::string hasData(std::string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != std::string::npos) {
    return "";
  }
  else if (b1 != std::string::npos && b2 != std::string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

int main()
{
  uWS::Hub h;

  PID pid;  
  // TODO: Initialize the pid variable.
  pid.Init(0.193, 0.00096 , 2.533);
  //pid.Init(0.2, 0.001 , 10);

  std::cout << " pid.Kp: " <<  pid.Kp << "  pid.Kd: " <<  pid.Kd << "  pid.Ki: " <<  pid.Ki<< std::endl;

  h.onMessage([&pid](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2')
    {
      auto s = hasData(std::string(data).substr(0, length));
      if (s != "") {
        auto j = json::parse(s);
        std::string event = j[0].get<std::string>();
        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<std::string>());
          double speed = std::stod(j[1]["speed"].get<std::string>());
          double angle = std::stod(j[1]["steering_angle"].get<std::string>());
          double steer_value;
          /*
          * TODO: Calcuate steering value here, remember the steering value is
          * [-1, 1].
          * NOTE: Feel free to play around with the throttle and speed. Maybe use
          * another PID controller to control the speed!
          */
          pid.UpdateError(cte);
          double curr_Step = pid.getStep();
          if (TWIDDLE_FLAG == 1 and curr_Step > Twiddle_Step){
              pid.twiddle();
              pid.Restart(ws);
          }
          
          steer_value = - pid.Kp * pid.p_error - pid.Kd * pid.d_error - pid.Ki * pid.i_error;
          
          std::cout << "Current Speed: " << speed<<std::endl;

          if (steer_value > 1){
            steer_value = 1;
          }
          if (steer_value < -1){
            steer_value = -1;
          }

          // DEBUG
          //std::cout << " pid.p_error: " <<  pid.p_error << "  pid.d_error: " <<  pid.d_error << "  pid.i_error: " <<  pid.i_error<< std::endl;
          std::cout << "Current Step: " << curr_Step <<" Total err "<<pid.TotalError()<< std::endl;
          std::cout << "Best err: " << pid.bestErr <<" Para_id "<<pid.para_id<< std::endl;
          std::cout << " pid.Kp: " <<  pid.Kp  << "  pid.Kd: " <<  pid.Kd << "  pid.Ki: " <<  pid.Ki<< std::endl;
          std::cout << " pid.dKp: " <<  pid.dKp << "  pid.dKd: " <<  pid.dKd << "  pid.dKi: " <<  pid.dKi<< std::endl;
          std::cout << "CTE: " << cte << " Steering Value: " << steer_value << std::endl;

          json msgJson;
          msgJson["steering_angle"] = steer_value;
          msgJson["throttle"] = 0.3;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }
      } else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }
  });

  // We don't need this since we're not using HTTP but if it's removed the program
  // doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
    const std::string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1)
    {
      res->end(s.data(), s.length());
    }
    else
    {
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port))
  {
    std::cout << "Listening to port " << port << std::endl;
  }
  else
  {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}
