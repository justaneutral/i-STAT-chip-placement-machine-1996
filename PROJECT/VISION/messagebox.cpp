#include <stdio.h>
#include <windows.h>

int fMessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
{
	FILE *ef = fopen("../error.log","w+");
	fprintf(ef,"%s in %s\n",lpText,lpCaption);
    fclose(ef);
	return 0;
}
