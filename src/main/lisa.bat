@echo off

rem LiSA start shell script
rem written by Frank Engelhardt

rem setting the LiSA home path
rem this nice command converts the current directory into 8.3 short format and saves in into LISAHOME 
for %%x in ("%cd%") do set LISAHOME=%%~sx

rem setting the LiSA config path
set LISACONFIG=%LISAHOME%/data/

rem starting LiSA
cd bin\
call "main.exe" "%LISACONFIG%default.xml"
cd ..