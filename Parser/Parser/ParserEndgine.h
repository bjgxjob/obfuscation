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

class ParserEndgine
{
public:
	ParserEndgine(void);
	~ParserEndgine(void);

	int obfuscate(void);
	int restore(void);
private:
	int findSubDirs(CPtrArray& listDirs);
	bool findCodesFiles(CPtrArray& listDirs);
	bool findFileByType(CCodeDirectories* pDirs, CString strTypeFile); 
	int parseFiles(CCodeDirectories* pDirs);
	int parseFile(CString strPath, CString strFilename, bool& bTempDirCreated);
	int getFileError(CFileException *e);
	CStringA encodeText(CStringA strText);
	bool createTempDir(CString strPath);

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

