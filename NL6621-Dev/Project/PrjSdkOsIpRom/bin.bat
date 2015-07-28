d:\Keil\ARM\BIN40\fromelf.exe --bin -o OUTPUT OUTPUT\OBJ\SdkFw.axf
d:\Keil\ARM\BIN40\fromelf.exe -c -s -o OUTPUT\SdkFw.lst OUTPUT\OBJ\SdkFw.axf
copy OUTPUT\LIST\SdkFw.map OUTPUT\SdkFw.map /B/Y
del cmpl.fed
copy ..\..\Tool\fblink.exe .\fblink.exe
fblink.exe
del fblink.exe
del link.fed
copy ..\..\Tool\GenBootBins.exe OUTPUT\GenBootBins.exe
cd OUTPUT
GenBootBins.exe fwCodeSRAM
del GenBootBins.exe

set Softver=V010101
set Time_h=%TIME:~0,2%
if /i %Time_h% LSS 10 (set Time_h=0%TIME:~1,1%)
set DTM=%DATE:~2,2%%DATE:~5,2%%DATE:~8,2%%Time_h%
set Pro=ROM
set FLAG=DBG
set Firmware=NuAgent_%Softver%_%DTM%_%Pro%%FLAG%.bin

copy nft_sd_uapsta.bin ..\..\..\Tool\%Firmware%

