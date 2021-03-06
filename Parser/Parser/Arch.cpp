// Arch.cpp : implementation file
//

#include "stdafx.h"
#include "ParserEngine.h"
#include "Arch.h"


const TCHAR* g_strArchFilename = _T("arch_obfuscate.obf");

// CArch
IMPLEMENT_SERIAL(CArch, CObject, 1)

CArch::CArch()
{
	m_pArr = NULL;
}

CArch::~CArch()
{
}


// CArch member functions

int CArch::writeArch(CString strPath, CPtrArray* pArr)
{
	//return 1 - error

	ASSERT(pArr);

	CFileException e;
	CFile f;
	if (strPath.GetLength() == 0)
		return 1;

	m_pArr = pArr;
	CString strFullPath = strPath+ g_strArchFilename;

	if (!f.Open(strFullPath, CFile::modeCreate | CFile::modeWrite | 
		CFile::typeBinary, &e))
	{
		getFileErrorArch(&e);
		return 1;
	}
	CArchive ar(&f, CArchive::store);
	Serialize(ar);
#ifndef _GOOGLE_TEST
	_tprintf(_T("Arch file was created: %s\n"), strFullPath);
#endif
	return 0;
}

int CArch::readArch(CString strPath, CPtrArray* pArr)
{
	//return 1 - error
	ASSERT(pArr);

	CFileException e;
	CFile f;
	if (strPath.GetLength() == 0)
		return 1;
	m_pArr = pArr;
	CString strFullPath = strPath + g_strArchFilename;

	if (!f.Open(strFullPath, CFile::modeRead | CFile::typeBinary, &e))
	{
		if (getFileErrorArch(&e) == CFileException::fileNotFound)
		{
#ifndef _GOOGLE_TEST
			_tprintf(_T("The project does not keep the obfuscated files.\n"));
#endif
			return 0;
		}
		else
		 return 1;
	}
	CArchive ar(&f, CArchive::load);
	Serialize(ar);
//	f.Close();
	return 0;
}

int CArch::getFileErrorArch(CFileException *e)
{
		switch(e->m_cause)
		{
		case CFileException::none:
		{
			return CFileException::none;
		}
		case CFileException::genericException : 
		{
#ifndef _GOOGLE_TEST
			_tprintf(_T("Arch file error: genericException\n"));
#endif
			return CFileException::genericException;
		}
		case CFileException::fileNotFound:     
		{
#ifndef _GOOGLE_TEST
			_tprintf(_T("File %s is not found\n"), g_strArchFilename);
#endif
			return CFileException::fileNotFound;
		}
		case CFileException::badPath:       
		{
#ifndef _GOOGLE_TEST
			_tprintf(_T("Arch file error: badPath;\n"));
#endif
			return CFileException::badPath;
		}
		case CFileException::tooManyOpenFiles:           
		{
#ifndef _GOOGLE_TEST
			_tprintf(_T("Arch file error: tooManyOpenFiles\n"));
#endif
			return CFileException::tooManyOpenFiles;
		}
		case CFileException::accessDenied:              
		{
#ifndef _GOOGLE_TEST
			_tprintf(_T("Arch file error: accessDenied\n"));
#endif
			return CFileException::accessDenied;
		}
		case CFileException::invalidFile:                 
		{
#ifndef _GOOGLE_TEST
			_tprintf(_T("Arch file error: invalidFile\n"));
#endif
			return CFileException::invalidFile;
		}
		case CFileException::removeCurrentDir:                  
		{
#ifndef _GOOGLE_TEST
			_tprintf(_T("Arch file error: removeCurrentDir\n"));
#endif
			return CFileException::removeCurrentDir;
		}
		case CFileException::directoryFull:                    
		{
#ifndef _GOOGLE_TEST
			_tprintf(_T("Arch file error: directoryFull\n"));
#endif
			return CFileException::directoryFull;
		}
		case CFileException::badSeek:                       
		{
#ifndef _GOOGLE_TEST
			_tprintf(_T("Arch file error: adSeek\n"));
#endif
			return CFileException::badSeek;
		}
		case CFileException::hardIO:                          
		{
#ifndef _GOOGLE_TEST
			_tprintf(_T("Arch file error: hardIO\n"));
#endif
			return CFileException::hardIO;
		}
		case CFileException::sharingViolation:                             
		{
#ifndef _GOOGLE_TEST
			_tprintf(_T("Arch file error: sharingViolation\n"));
#endif
			return CFileException::sharingViolation;
		}
		case CFileException::lockViolation:                                
		{
#ifndef _GOOGLE_TEST
			_tprintf(_T("Arch file error: lockViolation\n"));
#endif
			return CFileException::lockViolation;
		}
		case CFileException::diskFull:                                   
		{
#ifndef _GOOGLE_TEST
			_tprintf(_T("Arch file error: diskFull\n"));
#endif
			return CFileException::diskFull;
		}
		case CFileException::endOfFile:                                      
		{
#ifndef _GOOGLE_TEST
			_tprintf(_T("Arch file error: endOfFile\n"));
#endif
			return CFileException::endOfFile;
		}
		default:
		{
#ifndef _GOOGLE_TEST
			_tprintf(_T("Arch file error: unknown\n"));
#endif
			return -1;
		}
	}
}

void CArch::Serialize(CArchive &ar)
{
	ASSERT(m_pArr);
	
	int nSize = 0;
	int nFilesNumber = 0;
	if(ar.IsStoring())
	{//backup
		nSize = m_pArr->GetSize();
		if(nSize == 0)
			return;
		ar << nSize;
		for(int i = 0; i < nSize; i++)
		{
			CCodeDirectories* pDirs = (CCodeDirectories*)m_pArr->GetAt(i);
			ar << pDirs->m_strOriginalDir;
			nFilesNumber = pDirs->m_listFiles.GetSize();
			ar << nFilesNumber;
			for (int j = 0; j < nFilesNumber; j++)
			{
				CString strFileName = pDirs->m_listFiles.GetAt(j);
				ar << strFileName;
			}
		}
	}
	else
	{//restore
		ar >> nSize;
		if(nSize == 0)
			return;
		for(int i = 0; i < nSize; i++)
		{
			CCodeDirectories* pDirs = new CCodeDirectories;
			ASSERT(pDirs);
			ar >> pDirs->m_strOriginalDir;
			ar >> nFilesNumber;
			for (int j = 0; j < nFilesNumber; j++)
			{
				CString strFileName;
				ar >> strFileName;
				pDirs->m_listFiles.Add(strFileName);
			}
			m_pArr->Add(pDirs);
		}
	}
}