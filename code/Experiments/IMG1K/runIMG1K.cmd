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
set FP=37
set BP=93
set SCLS=54
set WU=31
set DCLS=5
set MODEL=IMG1K
set sparseOpt=-forwardsparsity:%FP%  -backwardsparsity:%BP%  -signalcachelinesparsity:%SCLS% -weightupdatesparsity:%WU% -deltacachelinesparsity:%DCLS%
for /l %%i in (1,1,4) do (
 .\SparseCompute.exe %STD_OPT% -samples:%SAMPLES% -model:%MODEL% -kernel:%%i %sparseOpt%
)
 
endlocal
goto :eof