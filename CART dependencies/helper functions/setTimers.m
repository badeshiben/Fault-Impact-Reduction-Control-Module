function [ctrl, timer] = setTimers(ctrl, timer, dt)
% warning logic that sets timers, predicted torque command, and torque
% command

%If in fault mode, reset everything else
if ctrl.mode(6)||ctrl.mode(4) %if faulted
    disp('fault')
    ctrl.T = 0;
    ctrl.Tfuture = 0;
    ctrl.mode(1) = 0;
    ctrl.mode(2) = 0;
    ctrl.mode(3) = 0;
    timer.derate = timer.tmax;
    timer.wait = 0;
    timer.stop = 0;
    if ctrl.mode(6)
        ctrl.mode(4) = 0;
    end
    return 
end

%Set stop, wait timers to default if not in warning modes, else increment them
if ctrl.mode(1) == 0
    timer.wait = 0;
else
    timer.wait = timer.wait + dt;
end
if ctrl.mode(3) == 0
    timer.stop = 0;
else
    timer.stop = timer.stop + dt;
end

%Derate warning mode logic
if ctrl.T == 1 %not derated
    ctrl.Tfuture = 1;
    ctrl.step = 0; %reset steps
    if ctrl.mode(2) == 1 %if in derate warning mode
        ctrl.Tfuture = 1/2;
        timer.derate = timer.derate-dt; %decrement timer
         if timer.derate <= 0  %if timer hits zero, set to max and derate
             timer.derate = timer.tmax;
             ctrl.T = 1/2;
             ctrl.reduction = 1;
         end
    else %if no warning
        timer.derate = timer.tmax;
    end           
else % if derated
    timer.derate = timer.derate-dt; %decrement timer
    if ctrl.derate_prev %if warning last step
        if ctrl.mode(2) %if still RP warning
            ctrl.Tfuture = ctrl.T - 1/2^(min(ctrl.step + 2, 5));
            disp('-Tfuture=')
            disp(ctrl.Tfuture)
            if timer.derate <= 0 %if timer hits zero, set to max and step down torque
                timer.derate = timer.tmax;
                if ctrl.T > 1/2 %only increment steps if Torque change occuring
                    ctrl.step = min(ctrl.step + 1, 4);
                end
                disp('step')
                disp(ctrl.step)
                ctrl.T = ctrl.T - 1/2^(ctrl.step + 1);
            end
        else %if no warning
            timer.derate = timer.tmax;
            ctrl.Tfuture = ctrl.T + 1/2^(min(ctrl.step + 2, 5));
            disp('+Tfuture=')
            disp(ctrl.Tfuture)
        end
    else %no warning last step
        if ctrl.mode(2) %if derate warning
             timer.derate = timer.tmax;
             ctrl.Tfuture = ctrl.T - 1/2^(min(ctrl.step + 2, 5));
             disp('-Tfuture=')
             disp(ctrl.Tfuture)
        else %if still no warning
            ctrl.Tfuture = ctrl.T + 1/2^(min(ctrl.step + 2, 5));
            disp('+Tfuture=')
            disp(ctrl.Tfuture)
            if timer.derate <= 0  %if timer hits zero, set to max and step up torque
                timer.derate = timer.tmax;
                ctrl.step = min(ctrl.step + 1, 4);
                disp('step')
                disp(ctrl.step)
                ctrl.T = ctrl.T + 1/2^(ctrl.step + 1);
            end
        end                                
    end
    ctrl.T = min(.96875, max(ctrl.T, 0.5)); %limit T
    ctrl.Tfuture = min(.96875, max(ctrl.Tfuture, 0.5)); %limit T
end


