@echo off

rem LiSA start shell script
rem written by Frank Engelhardt

rem setting the LiSA home path
rem these nice commands put the current directory into LISAHOME in 8.3 short format
for %%x in (%cd%) do set LISAHOME=%%~sx
set LISAHOME=%LISAHOME%/

rem setting the LiSA config path
set LISACONFIG=%LISAHOME%data/

rem starting LiSA
cd bin\
call "main.exe" "%LISACONFIG%default.xml"
cd ..