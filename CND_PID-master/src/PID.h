#ifndef PID_H
#define PID_H
#include <uWS/uWS.h>

class PID {
public:
  /*
  * Errors
  */
  double p_error;
  double i_error;
  double d_error;
  double total_error;

  double bestErr;

  /*
  * Coefficients
  */ 
  double Kp;
  double Ki;
  double Kd;

  double dKp;
  double dKi;
  double dKd;

  /*
  * Constructor
  */
  PID();

  /*
  * Destructor.
  */
  virtual ~PID();

  /*
  * Initialize PID.
  */
  void Init(double p, double i, double d);

  /*
  * Update the PID error variables given cross track error.
  */
  void UpdateError(double cte);

  /*
  * Calculate the total PID error.
  */
  double TotalError();

  /*
  * Record Running step
  */
  int totalstep;

  int para_id; 
  int optimize_dir;

  /*
  * Return Running step
  */
  int getStep();
  /*
  * Twiddle parameter
  */
  void twiddle();
  void Restart(uWS::WebSocket<uWS::SERVER> ws);
  void multiPara(int para_id, double scale);
  void addPara(int para_id, double value);
};

#endif /* PID_H */
