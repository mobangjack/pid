/**
 * Copyright (c) 2011-2016, Jack Mo (mobangjack@foxmail.com).
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include "pid.h"

void PID_Config(PID_t* pid, float kp, float ki, float kd, float Pmax, float Imax, float Dmax, float outmax)
{
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;
	pid->Pmax = Pmax;
	pid->Imax = Imax;
	pid->Dmax = Dmax;
	pid->outmax = outmax;
}

void PID_Reset(PID_t *pid)
{
	pid->P = 0;
	pid->I = 0;
	pid->D = 0;
	pid->ref = 0;
	pid->fdb = 0;
	pid->out = 0;
	pid->err[0] = 0;
	pid->err[1] = 0;
}

#define LIMIT(val,min,max) (val=val>max?max:(val<min?min:val))
void PID_Calc(PID_t *pid)
{
	pid->err[1] = pid->ref - pid->fdb; // calculate error
	pid->P = pid->kp * pid->err[1]; // P
	LIMIT(pid->P, -pid->Pmax, pid->Pmax); // limit P
	pid->I+= pid->ki * pid->err[1]; // I
	LIMIT(pid->I, -pid->Imax, pid->Imax); // limit I
	pid->D = pid->kd * (pid->err[1] - pid->err[0]); // D
	LIMIT(pid->D, -pid->Dmax, pid->Dmax); // limit D
	pid->out = pid->P + pid->I + pid->D; // output
	LIMIT(pid->out, -pid->outmax, pid->outmax); // limit output
	pid->err[0] = pid->err[1]; // save the last err
}

