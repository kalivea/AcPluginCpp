#include "stdafx.h"
#include "DwgFileInfo.h"

bool DwgFileInfo::GetDwgFileInfoWithExtension(TCHAR* szFileName, int nFileNameSize, TCHAR* szFilePath, int nFilePathSize)
{
	if (!szFileName || !szFilePath || nFileNameSize <= 0 || nFilePathSize <= 0)
		return false;

	resbuf rbFileName = { 0 }, rbFilePath = { 0 };
	bool bSuccess = false;

	if (acedGetVar(_T("DWGNAME"), &rbFileName) != RTNORM)
		goto cleanup;

	if (rbFileName.restype != RTSTR)
		goto cleanup;

	if (acedGetVar(_T("DWGPREFIX"), &rbFilePath) != RTNORM)
		goto cleanup;

	if (rbFilePath.restype != RTSTR)
		goto cleanup;

	size_t nNameRequired = _tcslen(rbFileName.resval.rstring) + 1;
	size_t nPathRequired = _tcslen(rbFilePath.resval.rstring) + 1;
	if (nNameRequired > static_cast<size_t>(nFileNameSize) ||
		nPathRequired > static_cast<size_t>(nFilePathSize))
		goto cleanup;

	_tcscpy_s(szFileName, nFileNameSize, rbFileName.resval.rstring);
	_tcscpy_s(szFilePath, nFilePathSize, rbFilePath.resval.rstring);
	bSuccess = true;

cleanup:

	if (rbFileName.restype == RTSTR)
		acutDelString(rbFileName.resval.rstring);

	if (rbFilePath.restype == RTSTR)
		acutDelString(rbFilePath.resval.rstring);

	return bSuccess;
}

bool DwgFileInfo::GetDwgFileNameWithoutExtension(TCHAR* szFileName, int nFileNameSize) {
	if (!szFileName || nFileNameSize <= 0) return false;

	resbuf rbFileName = { 0 };
	bool bSuccess = false;

	if (acedGetVar(_T("DWGNAME"), &rbFileName) != RTNORM) return false;
	if (rbFileName.restype != RTSTR) return false;

	TCHAR* pDot = _tcsrchr(rbFileName.resval.rstring, _T('.'));
	TCHAR* pSlash = _tcsrchr(rbFileName.resval.rstring, _T('\\'));

	if (pDot && (pSlash ? (pDot > pSlash) : true))
		*pDot = _T('\0');

	size_t nRequiredSize = _tcslen(rbFileName.resval.rstring) + 1;

	if (nRequiredSize <= static_cast<size_t>(nFileNameSize))
	{
		_tcscpy_s(szFileName, nFileNameSize, rbFileName.resval.rstring);
		bSuccess = true;
	}

	acutDelString(rbFileName.resval.rstring);
	return bSuccess;
}

