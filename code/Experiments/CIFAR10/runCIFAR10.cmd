@echo off
setlocal EnableDelayedExpansion

if "%1" == "" (
 set threadOpt=
) else (
 set threadOpt=-threads:%1
)

@echo Running with ThreadOpt= %threadOpt%

set STD_OPT=-affinity -startlayer:0 -zerosignalopt %threadOpt%

set SAMPLES=10000
set FP=24
set BP=84
set SCLS=83
set WU=89
set DCLS=67
set MODEL=CIFAR10
set sparseOpt=-forwardsparsity:%FP%  -backwardsparsity:%BP%  -signalcachelinesparsity:%SCLS% -weightupdatesparsity:%WU% -deltacachelinesparsity:%DCLS%
for /l %%i in (1,1,4) do (
 .\SparseCompute.exe %STD_OPT% -samples:%SAMPLES% -model:%MODEL% -kernel:%%i %sparseOpt%
)
 
endlocal
goto :eof