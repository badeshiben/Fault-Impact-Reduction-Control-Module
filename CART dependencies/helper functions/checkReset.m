function [timer,ctrl] = checkReset(ctrl, timer, data)
%checks to see if operator reset FIRC. If so, zeros mode bits, resets
%timers, and resets power command to nominal
if data.reset
    timer.TRP  = timer.tmax;
    timer.TS   = 0;
    timer.TNA  = 0;
    ctrl.RP_prev                = 0; %previous timestep WRP
    ctrl.T                      = 1; %torque command [pu]
    ctrl.Tfuture                = 1; %predicted torque when timer hits zero
    ctrl.genTemp.state          = 0; %sensor states, 0=nominal
    ctrl.power.state            = 0;
    ctrl.freqSensor.state       = 0;
    ctrl.torqueSensor.state     = 0;
    % ctrl.ax.state               = 0;
    ctrl.step                   = 0;
    ctrl.reduction              = 0; %1 if torque was ever reduced
    ctrl.mode       = [0 0 0 0 0 0];  %0 if off, 1 if on. All zeros = normal operation. [WNA WRP WS NS OLNS ES]
    ctrl.modeValue  = 0;  %value of binary ctrl.mode
end