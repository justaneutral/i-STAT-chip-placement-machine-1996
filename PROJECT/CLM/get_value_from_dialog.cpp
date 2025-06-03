#include "very_common_dialog.h"   
double get_value_from_dialog(char *ParamName)
{ 
  //Here I want to ask...
  very_common_dialog *dialoque = new very_common_dialog(&CString(ParamName));
  double answer = 0.0;

  if(dialoque->DoModal() == IDOK)
  {
    MessageBeep(0xffffffff);
    answer = dialoque->m_double;
  }

  delete dialoque;
  return answer;
}
