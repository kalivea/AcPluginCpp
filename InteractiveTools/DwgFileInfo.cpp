#include "stdafx.h"
#include "DwgFileInfo.h"

bool DwgFileInfo::GetDwgFileInfoWithExtension(TCHAR* name, int name_size, TCHAR* path, int path_size)
{
    if (!name || !path || name_size <= 0 || path_size <= 0) return false;
    *name = *path = _T('\0');
    resbuf file_name = {}, file_path = {};
    bool success = acedGetVar(_T("DWGNAME"), &file_name) == RTNORM && file_name.restype == RTSTR && file_name.resval.rstring;
    if (success) success = acedGetVar(_T("DWGPREFIX"), &file_path) == RTNORM && file_path.restype == RTSTR && file_path.resval.rstring;
    if (success) success = _tcslen(file_name.resval.rstring) < static_cast<size_t>(name_size) && _tcslen(file_path.resval.rstring) < static_cast<size_t>(path_size);
    if (success)
    {
        success = _tcscpy_s(name, name_size, file_name.resval.rstring) == 0 && _tcscpy_s(path, path_size, file_path.resval.rstring) == 0;
    }
    if (file_name.restype == RTSTR) acutDelString(file_name.resval.rstring);
    if (file_path.restype == RTSTR) acutDelString(file_path.resval.rstring);
    if (!success) *name = *path = _T('\0');
    return success;
}

bool DwgFileInfo::GetDwgFileNameWithoutExtension(TCHAR* name, int name_size)
{
    if (!name || name_size <= 0) return false;
    *name = _T('\0');
    resbuf file_name = {};
    bool success = acedGetVar(_T("DWGNAME"), &file_name) == RTNORM && file_name.restype == RTSTR && file_name.resval.rstring;
    if (success)
    {
        TCHAR* dot = _tcsrchr(file_name.resval.rstring, _T('.'));
        TCHAR* slash = _tcsrchr(file_name.resval.rstring, _T('\\'));
        if (dot && (!slash || dot > slash)) *dot = _T('\0');
        success = _tcslen(file_name.resval.rstring) < static_cast<size_t>(name_size);
        if (success) success = _tcscpy_s(name, name_size, file_name.resval.rstring) == 0;
    }
    if (file_name.restype == RTSTR) acutDelString(file_name.resval.rstring);
    return success;
}
