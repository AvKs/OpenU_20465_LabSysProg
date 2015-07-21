@echo OFF

set debugExe=MMN14.exe
set runOnFile=ps.as
set delFile1=ps.ob
set delFile2=Set.ent
set delFile3=Set.ext

tasklist /FI "IMAGENAME eq %debugExe%" 2>NUL | find /I /N "%debugExe%">NUL
if ("%ERRORLEVEL%"=="0") (
	taskkill /f /im %debugExe%
	echo Killed %debugExe% program
	echo.
)

if exist "%debugExe%" del "%debugExe%"
if exist "%delFile1%" del "%delFile1%"
if exist "%delFile2%" del "%delFile2%"
if exist "%delFile3%" del "%delFile3%"

echo Copying %debugExe% from "Debug" dir
copy ..\Debug\%debugExe% /Y
echo.

echo Running %debugExe% %runOnFile%
echo.

%debugExe% %runOnFile%

echo.
pause