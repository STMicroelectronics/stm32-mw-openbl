@echo off
GOTO :END_COMMENT
/**
  * @file Script called for the code generation
  * @license
  * @attention
  *
  * Copyright (c) 2025-2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  */
:END_COMMENT

REM Get the absolute path of the batch script
set "script_path=%~dp0"
echo "********************"
echo script_path -- %script_path%
echo "********************"
REM Assign command line arguments to variables
set "gpdsctemplateName=%1"
echo "********************"
echo script_path -- %gpdsctemplateName%
echo "********************"
set "generatorid=%2"
echo "********************"
echo generatorid -- %generatorid%
echo "********************"
set "generatorinputfile=%3"
echo "********************"
echo generatorinputfile -- %generatorinputfile%
echo "********************"
set "dryRunFlag=%4"
echo "********************"
echo dryRunFlag -- %dryRunFlag%
echo "********************"
REM Check if cube is installed
where cube >nul 2>nul
if %errorlevel% neq 0 (
    echo [GEN-ERROR] cube wrapper not found: STOP
    exit /b 1
)
REM Check if codegen is installed
cube --list | findstr /i "codegen" >nul
if %errorlevel% neq 0 (
    echo [GEN-ERROR] codegen not found: STOP
    exit /b 2
)
:: GPDSC Generation in stdout (if a generator is dry-run capable)
if "%dryRunFlag%"=="--dry-run" (
echo [DRY-RUN] cube codegen generategpdsc --path %generatorinputfile% --generatorId "%generatorid%" --templatePath "%script_path%\%gpdsctemplateName%" --dry-run
cube codegen generategpdsc --path %generatorinputfile% --generatorId "%generatorid%" --templatePath "%script_path%\%gpdsctemplateName%" --dry-run
) else (
:: Code Generation Step
echo [STEP 1/2: CODE-GEN] cube codegen generatefromlockfile --path %generatorinputfile% --generatorId "%generatorid%"
cube codegen generatefromlockfile --path %generatorinputfile% --generatorId "%generatorid%"
:: GPDSC Generation Step
echo [STEP 2/2: GPDSC-GEN] cube codegen generategpdsc --path %generatorinputfile% --generatorId "%generatorid%" --templatePath "%script_path%\%gpdsctemplateName%"
cube codegen generategpdsc --path %generatorinputfile% --generatorId "%generatorid%" --templatePath "%script_path%\%gpdsctemplateName%"
)
