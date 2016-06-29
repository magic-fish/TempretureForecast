#include "stdafx.h"
#include "SourceWRFactory.h"
#include "FileImpl.h"
#include "TempretureImpl.h"

CSourceWRFactory::CSourceWRFactory()
{
}


CSourceWRFactory::~CSourceWRFactory()
{
}


CDataStorage * CSourceWRFactory::CreateSourceWR(string name)
{
	CDataStorage*pStorage = NULL;
	if (name == "File"){
		pStorage = new CFileImpl();
	}
	if (name == "Mysql")
	{
		pStorage = new CTempretureImpl();
	}
	return pStorage;

}