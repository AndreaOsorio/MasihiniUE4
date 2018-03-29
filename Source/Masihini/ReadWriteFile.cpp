// Fill out your copyright notice in the Description page of Project Settings.

#include "ReadWriteFile.h"
#include "FileHelper.h"
#include "Paths.h"

bool UReadWriteFile::FileLoadCode(FString FileNameA, FString& SaveTextA)
{
	return FFileHelper::LoadFileToString(SaveTextA, *(FPaths::ProjectDir() + FileNameA));
}

bool UReadWriteFile::FileSaveCode(FString SaveTextB, FString FileNameB)
{
	return FFileHelper::SaveStringToFile(SaveTextB, *(FPaths::ProjectDir() + FileNameB));
}




