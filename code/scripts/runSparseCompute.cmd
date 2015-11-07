@echo off
setlocal EnableDelayedExpansion


if "%1" == "" (
 set passOpt=
) else (
 set passOpt=-pass:%1
)

@echo Running with PassOpt = %passOpt%

set STD_OPT=-affinity -startlayer:0 %passOpt% -zerosignalopt

set SAMPLES=100000
set FP=30
set BP=83
set SCLS=81
set WU=5
set DCLS=0
set MODEL=MNIST
set sparseOpt=-forwardsparsity:%FP%  -backwardsparsity:%BP%  -signalcachelinesparsity:%SCLS% -weightupdatesparsity:%WU% -deltacachelinesparsity:%DCLS%
for /l %%i in (1,1,4) do (
 .\SparseCompute.exe %STD_OPT% -samples:%SAMPLES% -model:%MODEL% -kernel:%%i %sparseOpt%
)
 
set FP=24
set BP=84
set SCLS=83
set WU=89
set DCLS=67
set SAMPLES=10000
set MODEL=CIFAR10
set sparseOpt=-forwardsparsity:%FP%  -backwardsparsity:%BP%  -signalcachelinesparsity:%SCLS% -weightupdatesparsity:%WU% -deltacachelinesparsity:%DCLS%
for /l %%i in (1,1,4) do (
 .\SparseCompute.exe %STD_OPT% -samples:%SAMPLES% -model:%MODEL% -kernel:%%i %sparseOpt%
)

set FP=37
set BP=93
set SCLS=54
set WU=31
set DCLS=5
set SAMPLES=1000
set MODEL=IMG1K
set sparseOpt=-forwardsparsity:%FP%  -backwardsparsity:%BP%  -signalcachelinesparsity:%SCLS% -weightupdatesparsity:%WU% -deltacachelinesparsity:%DCLS%
for /l %%i in (1,1,4) do (
  .\SparseCompute.exe %STD_OPT% -samples:%SAMPLES% -model:%MODEL% -kernel:%%i %sparseOpt%
)

set FP=30
set BP=92
set SCLS=63
set WU=7
set DCLS=0
set SAMPLES=1000
set MODEL=IMG22K
set sparseOpt=-forwardsparsity:%FP%  -backwardsparsity:%BP%  -signalcachelinesparsity:%SCLS% -weightupdatesparsity:%WU% -deltacachelinesparsity:%DCLS%
for /l %%i in (1,1,4) do (
 .\SparseCompute.exe %STD_OPT% -samples:%SAMPLES% -model:%MODEL% -kernel:%%i %sparseOpt%
)


endlocal
goto :eof