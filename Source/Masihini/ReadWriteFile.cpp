// Fill out your copyright notice in the Description page of Project Settings.

#include "ReadWriteFile.h"
#include "FileHelper.h"
#include "Paths.h"
#include <stdlib.h>

bool UReadWriteFile::FileLoadCode(FString FileNameA, FString& SaveTextA)
{
	return FFileHelper::LoadFileToString(SaveTextA, *(FPaths::ProjectDir() + FileNameA));
}

bool UReadWriteFile::FileSaveCode(FString SaveTextB, FString FileNameB)
{
	return FFileHelper::SaveStringToFile(SaveTextB, *(FPaths::ProjectDir() + FileNameB));
}

void UReadWriteFile::TransferMasihini()
{
	FString pathCopy = FPaths::ProjectDir() + "copy.bat";
	const char* commandCopy = TCHAR_TO_ANSI(*pathCopy);
	if (system(NULL)) puts("Ok");
	else exit(EXIT_FAILURE);
	int i = system(commandCopy);
}

void UReadWriteFile::CompileMasihini()
{
	//FPlatformProcess::CreateProc(TEXT("E:\\Documents\\ITESM\\CompilerDesign\\Masihini\\run.exe"), nullptr, true, false, false, nullptr, 0, nullptr, nullptr);
	int i;
	FString pathRun = FPaths::ProjectDir() + "test.bat";
	const char* commandRun = TCHAR_TO_ANSI(*pathRun);
	i = system(commandRun);
	
}

