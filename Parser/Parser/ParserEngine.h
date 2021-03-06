#pragma once

#include "resource.h"

struct CCodeDirectories
{
	CString m_strOriginalDir;
	CStringArray m_listFiles;
	bool m_bTempDirCreated;

	CCodeDirectories()
	{
		m_strOriginalDir = _T("");
		m_bTempDirCreated = false;
	}

	~CCodeDirectories()
	{
		if (m_listFiles.GetSize() > 0)
			m_listFiles.RemoveAll();
	}
};

class ParserEngine
{
public:
	ParserEngine(void);
	~ParserEngine(void);

	int obfuscate(void);
	int restore(void);
#ifdef _GOOGLE_TEST
public:  
#else
private:  
#endif
	int findSubDirs(CPtrArray& listDirs);
	bool findCodesFiles(CPtrArray& listDirs);
	bool findFileByType(CCodeDirectories* pDirs, CString strTypeFile); 
	int parseFiles(CCodeDirectories* pDirs);
	int parseFile(const CString& strPath, const CString& strFilename, bool& bTempDirCreated);
	int getFileError(CFileException *e);
	CStringA encodeText(CStringA strText);
	bool createTempDir(const CString& strPath);

private: 
	CString m_strCurrentPath;
public:
	const TCHAR* getCurrentPath()
	{
		return m_strCurrentPath.GetBuffer();
	}
	void setCurrentPath(const std::wstring& strCurrentPath)
	{
		m_strCurrentPath = strCurrentPath.c_str();
	}
};

