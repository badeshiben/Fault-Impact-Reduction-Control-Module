%% plot scopes
close all
clear all
clc
%%%%%%%%%%%%%%%%%%CASE 1%%%%%%%%%%%%%%%%%%%%%%%%%%
% load('CASE1.mat')
% time = CASE1.time;
% genPwr = CASE1.data(:,1);
% Pderate = CASE1.data(:,2);
% Ppredict = CASE1.data(:,3);
% P1s = CASE1.data(:,4);
% dw = CASE1.data(:,5);
% dT = CASE1.data(:,6);
% genTemp = CASE1.data(:,7);
% mode = CASE1.data(:,8);
% TRP = CASE1.data(:,9);
% TNA = CASE1.data(:,10);
% TS = CASE1.data(:,11);
% P1sFault = 1.5*ones(length(time),1);
% P1sWarn = 1.25*ones(length(time),1);
% 
% figure(1)
% subplot(2,2,1)
% plot(time, P1s, 'b')
% hold on
% plot(time, P1sFault, 'r--')
% plot(time, P1sWarn,'g-.')
% grid on
% xlim([20,100])
% ylim([0.5, 1.6])
% ylabel('rated  power  (pu)')
% legend('1-s avg. power', 'fault', 'warning')
% 
% subplot(2,2,2)
% plot(time, mode)
% grid on
% legend('mode value')
% yticks([0 1 2])
% xlim([20,100])
% ylim([-0.2, 2.2])
% 
% subplot(2,2,3)
% plot(time, genPwr, 'b')
% hold on
% plot(time, Pderate, 'r--')
% plot(time, Ppredict, 'g-.')
% grid on
% xlim([20,100])
% ylim([0.4, 1.4])
% xlabel('seconds')
% ylabel('rated  power  (pu)')
% legend('power', 'power derate', 'power derate predict')
% 
% subplot(2,2,4)
% plot(time, TRP, 'b')
% grid on
% hold on
% plot(time, TS, 'r--')
% plot(time, TNA, 'g-.')
% xlim([20,100])
% ylim([-1, 11])
% xlabel('seconds')
% ylabel('seconds')
% legend('derate countdown', 'stop timer', 'wait timer')

%%%%%%%%%%%%%%%%%%%CASE 2%%%%%%%%%%%%%%%%%%%%%%%%%%
load('CASE2.mat')
time = plots.time;
genPwr = plots.data(:,1);
Pderate = plots.data(:,2);
Ppredict = plots.data(:,3);
P1s = plots.data(:,4);
dw = plots.data(:,5);
dT = plots.data(:,6);
genTemp = plots.data(:,7);
mode = plots.data(:,8);
TRP = plots.data(:,9);
TNA = plots.data(:,10);
TS = plots.data(:,11);
dwFault = 0.1*ones(length(time),1);
dwWarn = 0.08*ones(length(time),1);
dTFault = 0.4*ones(length(time),1);
dTWarn = 0.3*ones(length(time),1);

figure(2)
subplot(3,2,1)
plot(time, dw, 'b')
hold on
plot(time, dwFault, 'r--')
plot(time, dwWarn,'g-.')
grid on
xlim([20,60])
ylim([-0.01, 0.11])
ylabel('rated  speed  (pu)')
legend('speed diff.', 'fault', 'warning')

subplot(3,2,2)
plot(time, mode)
grid on
legend('mode value')
xlim([20,60])
ylim([-0.5, 8.5])

subplot(3,2,3)
plot(time, dT, 'b')
hold on
plot(time, dTFault, 'r--')
plot(time, dTWarn,'g-.')
grid on
xlim([20,60])
ylim([-0.05, 0.45])
ylabel('rated   torque   (pu)')
legend('torque diff.', 'fault', 'warning')

subplot(3,2,4)
plot(time, TRP, 'b')
hold on
plot(time, TS, 'r--')
plot(time, TNA, 'g-.')
grid on
xlim([20,60])
ylim([-2, 22])
xlabel('seconds')
ylabel('seconds')
legend('derate countdown', 'stop timer', 'wait timer')

subplot(3,2,5)
plot(time, genPwr, 'b')
hold on
plot(time, Pderate, 'r--')
plot(time, Ppredict, 'g-.')
grid on
xlim([20,60])
ylim([-0.1, 1.1])
xlabel('seconds')
ylabel('rated  power  (pu)')
legend('power', 'power derate', 'power derate predict')

%%%%%%%%%%%%%%%%%%%CASE 3%%%%%%%%%%%%%%%%%%%%%%%%%%
% load('CASE3.mat')
% time = plots.time;
% genPwr = plots.data(:,1);
% Pderate = plots.data(:,2);
% Ppredict = plots.data(:,3);
% P1s = plots.data(:,4);
% dw = plots.data(:,5);
% dT = plots.data(:,6);
% genTemp = plots.data(:,7);
% mode = plots.data(:,8);
% TRP = plots.data(:,9);
% TNA = plots.data(:,10);
% TS = plots.data(:,11);
% P1sFault = 1.5*ones(length(time),1);
% P1sWarn = 1.25*ones(length(time),1);
% genTempFault = 130*ones(length(time),1);
% genTempWarn = 120*ones(length(time),1);
% 
% figure(3)
% subplot(3,2,1)
% plot(time, P1s, 'b')
% hold on
% plot(time, P1sFault, 'r--')
% plot(time, P1sWarn,'g-.')
% grid on
% xlim([20,115])
% ylim([0.5, 1.6])
% ylabel('rated  power  (pu)')
% legend('1-s avg. power', 'fault', 'warning')
% 
% subplot(3,2,2)
% plot(time, mode)
% grid on
% legend('mode')
% xlim([20,115])
% ylim([-0.2, 2.2])
% 
% subplot(3,2,3)
% plot(time, genTemp, 'b')
% hold on
% plot(time, genTempFault, 'r--')
% plot(time, genTempWarn,'g-.')
% grid on
% xlim([20,115])
% ylim([108, 132])
% ylabel('\circ C')
% legend('generator temp.', 'fault', 'warning')
% 
% subplot(3,2,4)
% plot(time, TRP, 'b')
% hold on
% plot(time, TS, 'r--')
% plot(time, TNA, 'g-.')
% grid on
% xlim([20,115])
% ylim([-1, 11])
% xlabel('seconds')
% ylabel('seconds')
% legend('derate countdown', 'stop timer', 'wait timer')
% 
% subplot(3,2,5)
% plot(time, genPwr, 'b')
% hold on
% plot(time, Pderate, 'r--')
% plot(time, Ppredict, 'g-.')
% grid on
% xlim([20,115])
% ylim([0.45, 1.35])
% xlabel('seconds')
% ylabel('rated  power  (pu)')
% legend('power', 'power derate', 'power derate predict')
