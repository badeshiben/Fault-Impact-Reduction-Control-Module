clc
clear all

% CART3 baseline model initailization

input_fast          = 'CART3_V7.fst';
PathName            = cd;
FAST_InputFileName  = [PathName,'\',input_fast];
TMax                = 110;   %max time
DT                  = 0.0025;  %timestep for FAST
GenEff              = 1;  %generator efficiency

global az_prev counter;
az_prev = 0;
counter = 0;
PreviewTime=1;
PreviewNode=5;
AvgWindSpeed=15;
load PI_pitch

FilterSetup

% fault adaptive controller initialization

% initialize data structure
data.savgP   = 0;
data.genTemp = 0;
data.dwHSS   = 0;
data.dT      = 0;
% data.ax      = 0;
data.reset = 0;


% initialize ctrl structure
% constants
ctrl            = struct();
ctrl.mode       = [0 0 0 0 0 0];  %0 if off, 1 if on. All zeros = normal operation. [WNA Wderate WS NS OLNS ES]
ctrl.modeValue  = 0;  %value of binary ctrl.mode
ctrl.FIRC       = 0;  %TURNS FIRC ON/OFF
ctrl.ramps      = [600/37, inf, inf]; %expected power ramp rates for ES, OLS, NS in kW/s

% variables
ctrl.derate_prev            = 0; %previous timestep Wderate
ctrl.T                      = 1; %torque command [pu]
ctrl.Tfuture                = 1; %predicted torque when timer hits zero
ctrl.genTemp.state          = 0; %sensor states, 0=nominal
ctrl.power.state            = 0;
ctrl.freqSensor.state       = 0;
ctrl.torqueSensor.state     = 0;
% ctrl.ax.state               = 0;
ctrl.step                   = 0;
ctrl.reduction              = 0; %1 if torque was ever reduced
ctrl.rampFuture             = 0; %predicted power ramp rate in kW/s if shutdown occurs

% initialize faulty structure
faulty.genTemp.checkFun      = 'genTempWarnFun';
faulty.genTemp.type          = 'derate';
faulty.genTemp.shutdown      = 'NS';

faulty.power.checkFun        = 'powerWarnFun';
faulty.power.type            = 'derate';
faulty.power.shutdown        = 'ES';

% faulty.ax.checkFun           = 'axWarnFun';
% faulty.ax.type               = 'stop';
% faulty.ax.shutdown           = 'NS';

faulty.freqSensor.checkFun   = 'freqSensorWarnFun';
faulty.freqSensor.type       = 'stop';
faulty.freqSensor.shutdown   = 'NS';

faulty.torqueSensor.checkFun = 'torqueSensorWarnFun';
faulty.torqueSensor.type     = 'wait';
faulty.torqueSensor.shutdown = 'NS';

% initialize timer structures
% timers for modes
timer.tmax = 10; %Wderate default
timer.derate  = timer.tmax;
timer.stop   = 0;
timer.wait  = 0;

% initialize simulink buses (correspond to MATLAB structs)
bus1        = Simulink.Bus.createObject(timer);
bus2        = Simulink.Bus.createObject(ctrl);
bus3        = Simulink.Bus.createObject(data);
timerBus    = slBus1;
ctrlBus     = slBus2;
dataBus     = slBus3;