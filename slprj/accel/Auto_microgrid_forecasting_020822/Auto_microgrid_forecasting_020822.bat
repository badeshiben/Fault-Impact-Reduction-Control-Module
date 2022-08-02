@echo off
set MATLAB=C:\Program Files\MATLAB\R2020a
"%MATLAB%\bin\win64\gmake" -f Auto_microgrid_forecasting_020822.mk  OPTS="-DTID01EQ=0"
