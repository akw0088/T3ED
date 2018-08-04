@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by T3ED.HPJ. >"hlp\T3ED.hm"
echo. >>"hlp\T3ED.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\T3ED.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\T3ED.hm"
echo. >>"hlp\T3ED.hm"
echo // Prompts (IDP_*) >>"hlp\T3ED.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\T3ED.hm"
echo. >>"hlp\T3ED.hm"
echo // Resources (IDR_*) >>"hlp\T3ED.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\T3ED.hm"
echo. >>"hlp\T3ED.hm"
echo // Dialogs (IDD_*) >>"hlp\T3ED.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\T3ED.hm"
echo. >>"hlp\T3ED.hm"
echo // Frame Controls (IDW_*) >>"hlp\T3ED.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\T3ED.hm"
REM -- Make help for Project T3ED


start /wait hcw /C /E /M "hlp\T3ED.hpj"
if errorlevel 1 goto :Error 
if not exist "hlp\T3ED.hlp" goto :Error 
copy  "hlp\T3ED.hlp" "Release"
goto :Done
:Error 
echo   hlp\T3ED.hpj(1) : error: 
type "hlp\T3ED.log" 
:Done