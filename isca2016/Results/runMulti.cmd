@echo off
setlocal EnableDelayedExpansion

set GPLOT="C:\Program Files (x86)\gnuplot\bin\gnuplot.exe"
set PYSCRIPT=%~dp0multi_compute_sparsity.plt
set INPUT_SUFFIX=sparsity.txt
set OUTPUT_SUFFIX=sparsity.png 

set forwardFile=forward%INPUT_SUFFIX%
set backwardFile=backward%INPUT_SUFFIX%
set weightFile=weightupdate%INPUT_SUFFIX%
set outFile=multi_compute%OUTPUT_SUFFIX%
set GOPT=-e "forwardFile='%forwardFile%'; backwardFile='%backwardFile%'; weightFile='%weightFile%'; outFile='%outFile%'"

%GPLOT% %GOPT% %PYSCRIPT%

endlocal
goto :eof