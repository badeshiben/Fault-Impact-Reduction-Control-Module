function timer = maxDerateTimers(ctrl, fault, timer, faultName)
%   set all other derate timers to max

    fields = fieldnames(fault);
    timer.derate = inf;
    for i=1:length(fields)  
        if strcmp(fault.(fields{i}).type, 'derate') && ~(strcmp(faultName, fields{i})) %for all other derate faults
            timer.(fields{i}) = ctrl.timerMax; %set timers to max
        end
    end