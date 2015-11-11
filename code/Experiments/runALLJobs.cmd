@echo off

@rem set Jobs=MNIST,CIFAR10,IMG1K,IMG22K
set Jobs=CIFAR10,IMG1K,IMG22K
set runScript=%~dp0%runJob.cmd
set server=xcg-wcs-24 
for %%i in (%Jobs%) do (
 pushd %%i
 %runScript% %server% %%i 
 popd
)

endlocal
goto :eof
