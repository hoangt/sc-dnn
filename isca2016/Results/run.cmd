@echo off
setlocal EnableDelayedExpansion

set GPLOT="C:\Program Files (x86)\gnuplot\bin\gnuplot.exe"
set PYSCRIPT=%~dp0compute_sparsity.plt
set INPUT_SUFFIX=sparsity.txt
set OUTPUT_SUFFIX=sparsity.png 

for /f "eol=# tokens=1,2" %%i in (data.txt) do (
  @echo %%i%INPUT_SUFFIX%
  set inputFile=%%i%INPUT_SUFFIX%
  set outputFile=%%i%OUTPUT_SUFFIX%
  %GPLOT% -e "inputFile='!inputFile!'; outputFile='!outputFile!'; plotTitle='%%j'" %PYSCRIPT%
)

endlocal
goto :eof