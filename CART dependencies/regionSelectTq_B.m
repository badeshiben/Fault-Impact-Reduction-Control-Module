function [genTorque, T1, region, w_target, Ptarget] = regionSelectTq(W_hss, deltaP, Pi, U, LSSTq, LSSa)
% This function adapted by Paul Fleming from one written by JW van Wingerden
%Currently does not contain any checks on pitch angle for fear
%future advanced codes will trip this accidentally
%
%Also does not contain region 1.5 as documented in Jonkman
%Also does not contain "optimal tracking rotor code"
%See unused Torque Controller Block for example of pitch influence

%Define constants
DENSITY = 1.225; %Measured 10/21/10
RADIUS = 20;
% CPZERO = .4658;
% TSRZERO = 7.1; %Higher TSR to get rid of awkward saturation region 2.5
CPZERO = .465;
TSRZERO = 6; %Higher TSR to get rid of awkward saturation region 2.5
MAXTORQUE = 3524.37;
MAXTORQUE = 3524.37;
RATIO = 43.164;
MAXRPM = 1600; %lowered RPM for stability
MAXw = MAXRPM*pi/30;
MAXPOWER = 6e5; %W, assumed
HSSRPM = W_hss;
HSSw   = HSSRPM*pi/30; %rad/s
LSSw = HSSw/RATIO; %rad/s
cps = [0.465 0.46 0.45 0.437 0.423 0.406 0.388 0.367 0.345 0.32 0.293... 
0.264 0.232 0.197 0.159 0.119 0.075]; %the other half...
tsrs = linspace(6, 14, 17);

%Calculate k
K = ((DENSITY*pi* RADIUS^5*CPZERO)/ (2.0*TSRZERO^3));

%Region 2
genTorque = K / (RATIO^3) * HSSw^2;
region = 2;

%Region 2.5 Code
if((MAXTORQUE/(MAXRPM*0.05))*(HSSRPM-MAXRPM*0.92) > genTorque)
    genTorque = (MAXTORQUE/(MAXRPM*0.05))*(HSSRPM-MAXRPM*0.92);
    region = 2.5;
end

%Region 3 code
if ( genTorque > MAXTORQUE || HSSRPM > 0.99 * MAXRPM)
    genTorque = MAXTORQUE;
    region = 3;
end

w_target = HSSw;
Pnominal = CPZERO*1/2*DENSITY*pi*RADIUS^2*U^3; %max power at current WS
Ptarget = min(Pnominal, MAXPOWER);
T1=0;
%Power derate for AS. Decrease T, increase TSR to move right along Cp-TSR
%curve
%When derated, remove region 2.5. Go directly from region 20-30. In 30,
%increase/decrease torque at constant w
if abs(deltaP) >= 1e-3
    region = 20;
    Pnominal = CPZERO*1/2*DENSITY*pi*RADIUS^2*U^3; %max power at current WS
    dP = deltaP * min(MAXPOWER, Pnominal);  % W
    Ptarget = min(MAXPOWER, Pnominal) + dP;
    Cptarget = Ptarget/(1/2*DENSITY*pi*RADIUS^2*U^3);
    TSRtarget = interp1(cps, tsrs, Cptarget);
    LSSw_target = TSRtarget*U/RADIUS; 
    K = ((DENSITY*pi* RADIUS^5*Cptarget)/ (2*TSRtarget^3));
    % use torque control from Aiaa98 paper
    G = 0; %gain
    J = 3899.7 + 46.01*RATIO^2; %hub + gen inertia, kg-m2
    Tshaft = LSSTq/1000; %Nm
    Taero = Tshaft + J*LSSa; %Nm
%     genTorque = (K*w_target_LSS^2 - G*(Taero - K*w^2))/RATIO;
    genTorque = (K*LSSw^2 - G*(Taero - K*LSSw^2))/RATIO;
    disp('dP=')
    disp(dP)
    disp('Pi=')
    disp(Pi)
    disp('Pnom=')
    disp(Pnominal)
    disp('Ptarget=')
    disp(Ptarget)
    disp('Cptarget =')
    disp(Cptarget)
    disp('TSRtarget =')
    disp(TSRtarget)
    w_target = LSSw_target*RATIO; 
    %Region 30 code
    if HSSw > (0.99 * MAXw)
        genTorque = Ptarget/HSSw; %originally HSSRPM. not making a difference
        region = 30;
        w_target = HSSw;
    end
end

%Region 1
if (HSSRPM < 10 * RATIO)
    genTorque = 0;
end

if genTorque > MAXTORQUE
    genTorque = MAXTORQUE;
end
disp("K=")
disp(K)
disp('Region=')
disp(region)
disp('GenTq=')
disp(genTorque)
disp('T1=')
disp(T1)
