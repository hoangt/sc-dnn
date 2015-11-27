@echo off
setlocal EnableDelayedExpansion

set GPLOT="C:\Program Files (x86)\gnuplot\bin\gnuplot.exe"
set PYSCRIPT=%~dp0compute_scaling.plt
set INPUT_SUFFIX=.txt
set OUTPUT_SUFFIX=.png 

set mnistFile=mnist%INPUT_SUFFIX%
set cifar10File=cifar10%INPUT_SUFFIX%
set img1KFile=IMG1K%INPUT_SUFFIX%
set img22KFile=IMG22K%INPUT_SUFFIX%
set outFile=compute_scaling%OUTPUT_SUFFIX%
set GOPT=-e "mnistFile='%mnistFile%'; cifar10File='%cifar10File%'; img1KFile='%img1KFile%'; img22KFile='%img22KFile%'; outFile='%outFile%'"

%GPLOT% %GOPT% %PYSCRIPT%

start %outFile%

endlocal
goto :eof