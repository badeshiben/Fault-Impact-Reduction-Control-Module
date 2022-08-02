function ctrl = rampFuture(ctrl, fault, faultName)
%   set the turbine mode based on the command state
if ctrl.(faultName).state > 0
    if strcmp(fault.(faultName).shutdown, 'ES') %if ES type
        ctrl.rampFuture = max(ctrl.rampFuture, ctrl.ramps(1)); 
    elseif strcmp(fault.(faultName).shutdown, 'OLS') %if OLS type
        ctrl.rampFuture = max(ctrl.rampFuture, ctrl.ramps(2)); 
    else strcmp(fault.(faultName).shutdown, 'NS') %if NS type
        ctrl.rampFuture = max(ctrl.rampFuture, ctrl.ramps(3));
    end
end
    
end