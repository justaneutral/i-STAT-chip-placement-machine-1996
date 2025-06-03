typedef struct _machina_state_DS
{
    bool m_chip_is_picked_up;
	bool m_magazine_is_ready;
	bool m_wafer_is_loaded;
	int  m_wafer_position;	
	int  m_chip_position;
	char m_file_name[80];

} machina_state_DS;


bool write_data(char * filename, machina_state_DS * cs);
bool read_data(char * filename, machina_state_DS * cs);
