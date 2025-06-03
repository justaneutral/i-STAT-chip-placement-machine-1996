

#pragma pack(push,1)
	struct VikingHeader {
		char MaterialId[32];
		char EquipId[8];
		char Operator[16];
		unsigned short NumberOfRefPoints;

		//Reference Point Array
		struct RefPoint {
			signed short X; //using Viking Co-ordinates
			signed short Y; //using Viking Co-ordinates
		} ReferencePoint[16];

		char DieUOM;
		long XDieSize;
		long YDieSize;
		unsigned short ColumnCount;
		unsigned short RowCount;
		char NullBincode;
		unsigned long NumberProcessDie;
		unsigned short NumberProcessBins;
		//Array of bincodes and counts go here
		struct ActDie {
			unsigned char bincode;
			unsigned short count;
		} ActiveDie[32];

		char MapFormat;
		char OriginLocationCode;
		unsigned short FlatLocation;
		unsigned short FrameLocation;
		char ProcessType;
		char ProcessStatus;
		char Reserved[10];
	};
#pragma pack(pop)

#pragma pack(push,1)
	struct RowHeader{
		signed short X; //using Viking Co-ordinates
		signed short Y; //using Viking Co-ordinates
		unsigned short DieInRow;
		char RowDir;	//0 L->R, 1 R->L
						//0 T->B, 1 B->T
										
	};
#pragma pack(pop)
