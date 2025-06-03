//shared memory.

__declspec(dllexport) VOID SetSharedMem(ULONG pos, LPVOID buf, ULONG len);
__declspec(dllexport) VOID GetSharedMem(ULONG pos, LPVOID buf, ULONG len);


//camera
__declspec(dllexport) void get_hwnd(HWND *hw);
__declspec(dllexport) void set_hwnd(HWND hwnd);
__declspec(dllexport) void get_double(double *x,ULONG pos);
__declspec(dllexport) void set_double(double data, ULONG pos);
__declspec(dllexport) void get_x(double *x);
__declspec(dllexport) void get_y(double *y);
__declspec(dllexport) void get_angle(double *angle);
__declspec(dllexport) void get_x_angle(double *x_angle);
__declspec(dllexport) void get_y_angle(double *y_angle);
__declspec(dllexport) void get_checksum(double *chs);
__declspec(dllexport) void set_x(double x);
__declspec(dllexport) void set_y(double y);
__declspec(dllexport) void set_angle(double angle);
__declspec(dllexport) void set_x_angle(double x_angle);
__declspec(dllexport) void set_y_angle(double y_angle);
__declspec(dllexport) void set_checksum(double x, double y, double angle);
__declspec(dllexport) double calculate_base_coordinates(double x, double y, double angle);
__declspec(dllexport) void get_base_coordinates(double *x, double *y, double *angle);
__declspec(dllexport) void get_wafer_coordinates(double *x, double *y, double *angle, double *x_angle, double *y_angle);
__declspec(dllexport) void set_int_array(int len, int *x);
__declspec(dllexport) void get_int_array(int len, int *x);
__declspec(dllexport) void calculate_top_edge();
__declspec(dllexport) void calculate_bottom_edge();
__declspec(dllexport) void calculate_left_edge();
__declspec(dllexport) void calculate_right_edge();
__declspec(dllexport) double get_base_light_level();
__declspec(dllexport) BOOL VisionSystemIsStarted();
__declspec(dllexport) VOID set_camera(int camera);
__declspec(dllexport) void get_hall_coordinates(double *x, double *y, double *angle);
__declspec(dllexport) void get_hall_coordinates_2(double *x, double *y, double *angle);
__declspec(dllexport) void chip_position_matching(char *mode);

