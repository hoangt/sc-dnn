@echo off
setlocal EnableDelayedExpansion

set GPLOT="C:\Program Files (x86)\gnuplot\bin\gnuplot.exe"
set PYSCRIPT=%~dp0multi_data_sparsity.plt
set INPUT_SUFFIX=_data_sparsity.txt
set OUTPUT_SUFFIX=_datasparsity.png 

set wordFile=word%INPUT_SUFFIX%
set cacheLineFile=cacheline%INPUT_SUFFIX%
set outFile=multi%OUTPUT_SUFFIX%
set GOPT=-e "wordFile='%wordFile%'; cacheLineFile='%cacheLineFile%'; outFile='%outFile%'"

%GPLOT% %GOPT% %PYSCRIPT%

start %outFile%

endlocal
goto :eof