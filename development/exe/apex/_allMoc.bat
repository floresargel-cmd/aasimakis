@echo off
setlocal EnableDelayedExpansion

::################################## ERROR CHECKS #########################################
::check if we are running script from server
cd /D  %~dp0 2> nul || echo SCRIPT IS RUNNING FROM SERVER SIDE && echo IGNORE THE ABOVE PATH ERROR && pushd %~dp0 && echo: && echo:

::bat number of arguments check
set ARGC=0
for %%x in (%*) do Set /A ARGC+=1
IF !ARGC! GTR 2 (
	echo _allMoc MORE THAN 2 ARGUMENTS FOUND
	GOTO :EXIT_WITH_ERRORS
)

::name (first argument) check
for %%f in (%0) do set EXECUTABLE_NAME=%%~nxf
IF "!EXECUTABLE_NAME!" NEQ "_allMoc.bat" (
	IF "!EXECUTABLE_NAME!" NEQ "_allMoc" (
		echo WRONG BATCH FILE NAME. NAME SHOULD BE _allMoc.bat
		GOTO :EXIT_WITH_ERRORS
	)
)

::version (second argument) check
SET PARAM_1=%~1
IF "%PARAM_1%"=="" (
		echo _allMoc.bat NEEDS MOC VERSION AS ARGUMENT TO RUN.
		GOTO :EXIT_WITH_ERRORS
)

::spaces (third argument optional) check
SET PARAM_2=%~2
SET SPACES=
SET NEST=
SET SPACES_FOR_NEXT_BATCH="1"
IF "!PARAM_2!" =="1" (
	SET SPACES=" "
	SET NEST=└^>
	SET SPACES_FOR_NEXT_BATCH="2"
) ELSE IF "!PARAM_2!" =="2" (
	SET SPACES="     "
	SET NEST=└^>
	SET SPACES_FOR_NEXT_BATCH="3"
) ELSE IF "!PARAM_2!" NEQ "" (
	echo SECOND ARGUMENT SHOULD BE 1 OR 2 OR EMPTY
	GOTO :EXIT_WITH_ERRORS
	ELSE (
		SET SPACES_FOR_NEXT_BATCH="1"
	)
)
IF "!SPACES!" NEQ "" (
	SET SPACES=!SPACES:"=!
)

chcp 65001 >NUL

::################################## MAIN LOOP #########################################
FOR /f  %%G IN ('dir /b /ad-h') DO (
	echo !SPACES!!NEST!%%G
	set TRY_LEGACY_MOC="FALSE"
	IF NOT EXIST %%G\_moc.%PARAM_1%.bat (
	  echo !SPACES!  └^>Could Not Find _moc.%PARAM_1%.bat
		IF "%PARAM_1%"=="5.7.0" (set TRY_LEGACY_MOC="TRUE")
	) ELSE (
	  call %%G\_moc.%PARAM_1%.bat NPRINT !SPACES_FOR_NEXT_BATCH!
	)

	IF !TRY_LEGACY_MOC!=="TRUE" (
		echo !SPACES!  └^>Trying Legacy _m.bat
		IF NOT EXIST %%G\_m.bat (
			echo !SPACES!    └^>Could Not Find _m.bat.bat
		) ELSE (
			set LEGACY_MOC_FOUND="TRUE"
                        echo ******************************************************
			echo ******************* _m.bat output ********************
                        echo ******************************************************
			cd %%G
			call _m.bat
                        echo ******************************************************
		)
		echo:
	)
)

::################################## EXIT #########################################
GOTO EXIT

:EXIT_WITH_ERRORS
echo:
echo #######################################################
echo ################ ERRORS ENCOUNTERED ###################
echo #######################################################
echo                PRESS ANY KEY TO CLOSE
set /p dummyInput=
echo:

:EXIT
