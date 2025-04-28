#include "stdafx.h"
#include "TestClass.h"
#include "CAcadApplication.h"
#include "CAcadDocument.h"
#include "CAcadModelSpace.h"
void TestClass::TestClassInit()
{
	acedRegCmds->addCommand(_T("tmpGroupName"), _T("TestCOM"), _T("TestCOM"), ACRX_CMD_MODAL, Test);
}
void TestClass::TestClassUnload()
{
	acedRegCmds->removeGroup(_T("tmpGroupName"));
}
void TestClass::Test()
{
	CAcadApplication IApp;
	CAcadDocument IDoc;
	CAcadModelSpace IModelSpace;

	IDispatch* disp = acedGetAcadWinApp()->GetIDispatch(true);
	IApp.AttachDispatch(disp);
	IApp.put_Visible(true);

	disp = IApp.get_ActiveDocument();
	IDoc.AttachDispatch(disp);

	disp = IDoc.get_ModelSpace();
	IModelSpace.AttachDispatch(disp);

	SAFEARRAYBOUND rgsaBound;
	rgsaBound.lLbound = 0L;
	rgsaBound.cElements = 3;
	SAFEARRAY* start_point = NULL;
	start_point = SafeArrayCreate(VT_R8, 1, &rgsaBound);

	long i = 0;
	double value = 4.0;
	SafeArrayPutElement(start_point, &i, &value);
	i++;
	value = 2.0;
	SafeArrayPutElement(start_point, &i, &value);
	i++;
	value = 0.0;
	SafeArrayPutElement(start_point, &i, &value);

	VARIANT pt1;
	VariantInit(&pt1);
	V_VT(&pt1) = VT_ARRAY | VT_R8;
	V_ARRAY(&pt1) = start_point;

	IModelSpace.AddCircle(pt1, 2.0);

}