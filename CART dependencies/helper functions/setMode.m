function ctrl = setMode(ctrl, fault, faultName)
%   set the turbine mode based on the command state
    if ctrl.(faultName).state == 2 %if faulted
        if strcmp(fault.(faultName).shutdown, 'ES') %if ES type
            ctrl.mode(6) = 1; %ES mode
        else
            ctrl.mode(4) = 1; %NS mode
        end
    end
    
    switch fault.(faultName).type
        case 'derate'
            if ctrl.(faultName).state == 1 %if warning
                ctrl.mode(2) = 1; %derate warning mode
            end
        case 'stop'
            if ctrl.(faultName).state == 1 %if warning
                ctrl.mode(3) = 1; %stop warning mode
            end
        case 'wait'
            if ctrl.(faultName).state == 1
                ctrl.mode(1) = 1; %wait warning mode
            end
    end