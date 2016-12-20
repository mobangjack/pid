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

void PID_Reset(PID_t *pid)
{
	pid->err[0] = 0;
	pid->err[1] = 0;
	pid->P = 0;
	pid->I = 0;
	pid->D = 0;
	pid->out = 0;
}

#define LIMIT(val,min,max) (val=val>max?max:(val<min?min:val))
void PID_Calc(PID_t *pid)
{
	pid->err[1] = pid->ref - pid->fdb; // calculate error
	pid->P = (pid->kp + pid->kp_offset) * pid->err[1]; // P
	LIMIT(pid->P, -pid->Pm, pid->Pm);
	pid->I+= (pid->ki + pid->ki_offset) * pid->err[1]; // I
	LIMIT(pid->I, -pid->Im, pid->Im);
	pid->D = (pid->kd + pid->kd_offset) * (pid->err[1] - pid->err[0]); // D
	LIMIT(pid->D, -pid->Dm, pid->Dm);
	pid->out = pid->P + pid->I + pid->D; // output
	LIMIT(pid->out, -pid->outmax, pid->outmax);
	pid->err[0] = pid->err[1]; // save the last err
}

