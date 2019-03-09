#include <iostream>
#include <cmath>        // std::abs
#include "PID.h"
#define start_step 100

using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {}

PID::~PID() {}

void PID::Init(double p, double i, double d) {
    Kp = p;
    Ki = i;
    Kd = d;

    dKp = 0.01 ;
    dKi = 0.0001;
    dKd = 0.1;
    
    totalstep = 0;
    total_error = 0;
    optimize_dir = 0;
    p_error = 0;
    i_error = 0;
    d_error = 0;
    para_id = 0;
    bestErr = 1e9;
    
}

void PID::UpdateError(double cte) {
    
    d_error = cte - p_error;
    p_error = cte;
    i_error += cte;
    total_error += std::abs(cte);
    totalstep += 1;
}

double PID::TotalError() {
    return total_error/totalstep;
}

int PID::getStep(){
    return totalstep;
}

void PID::multiPara(int para_id, double scale){
    if (para_id == 0){
        dKp *= scale;
    }
    else if (para_id == 1){
         dKd *= scale;
    }
    else{
        dKi *= scale;
    }
        
}

void PID::addPara(int para_id, double value){
    if (para_id == 0){
        Kp += (value)*dKp;
    }
    else if (para_id == 1){
        Kd += (value)*dKd;
    }
    else{
        
        Ki += (value)*dKi;
    }
         
}

void PID::twiddle(){
    double err = TotalError();

    if (bestErr > 1e5){
        bestErr = err;
        //Kp += dKp;
        //Kd += dKd;
        Ki += dKi;
        return;
    }

    if (optimize_dir == 0){
        if(err < bestErr) {
            bestErr = err;
            multiPara(2, 1.1);
            para_id += 1;
            para_id = para_id%2;
            addPara(2,1);
        }
        else{
            //There was no improvement
            addPara(2,-2);
            optimize_dir = 1;
        }
    }  
    else{
        if(err < bestErr) {
            bestErr = err;
            multiPara(2, 1.05);
        }
        else{
            //There was no improvement
            addPara(2,1);
            multiPara(2, 0.9);
        } 
        optimize_dir = 0;
        para_id += 1;
        para_id = para_id%2;
        addPara(2,1);
    }
        
    
}

void PID::Restart(uWS::WebSocket<uWS::SERVER> ws){
    totalstep = 0;
    total_error = 0;
    p_error = 0;
    i_error = 0;
    d_error = 0;
    std::string reset_msg = "42[\"reset\",{}]";
    ws.send(reset_msg.data(), reset_msg.length(), uWS::OpCode::TEXT);
}

