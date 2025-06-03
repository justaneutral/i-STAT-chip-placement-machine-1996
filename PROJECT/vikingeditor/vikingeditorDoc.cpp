// VikingEditorDoc.cpp : implementation of the CVikingEditorDoc class
//

#include "stdafx.h"


#include "VikingEditorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVikingEditorDoc

IMPLEMENT_DYNCREATE(CVikingEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CVikingEditorDoc, CDocument)
	//{{AFX_MSG_MAP(CVikingEditorDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVikingEditorDoc construction/destruction

CVikingEditorDoc::CVikingEditorDoc()
{
	// TODO: add one-time construction code here
	m_bOkToClose=TRUE;

	int i, j;
	for(i=0; i<ARRAYSIZE; i++){
		for(j=0; j<ARRAYSIZE; j++){
				RowData[i][j]=-1;
		}
	}

	SaveAsType=0; //Viking
	NumberingConvention=0;
	serpentine SerpentineDir=HORIZONTAL;
	OldColumn=0;
	OldRow=0;
	PresentColumn=0;
	PresentRow=0;
	//MapHeader.;
		//char MaterialId[32];
		//char EquipId[8];
		//char Operator[16];
		//unsigned short NumberOfRefPoints;

		//Reference Point Array
		//struct RefPoint {
			//signed short X;
			//signed short Y;
		//} ReferencePoint[16];

		MapHeader.DieUOM=0;
		//long XDieSize;
		//long YDieSize;
		MapHeader.ColumnCount=2;
		MapHeader.RowCount=2;
		MapHeader.NullBincode='\0';;
		//unsigned long NumberProcessDie;
		//unsigned short NumberProcessBins;
		//Array of bincodes and counts go here
		//struct ActDie {
			//unsigned char bincode;
			//unsigned short count;
		//} ActiveDie[32];

		//char MapFormat;
		//char OriginLocationCode;
		//unsigned short FlatLocation;
		//unsigned short FrameLocation;
		//char ProcessType;
		MapHeader.ProcessStatus=' ';
		MapHeader.Reserved[0]='\0';
		MapHeader.Reserved[1]='\0';
		MapHeader.Reserved[2]='\0';
		MapHeader.Reserved[3]='\0';
		MapHeader.Reserved[4]='\0';
		MapHeader.Reserved[5]='\0';
		MapHeader.Reserved[6]='\0';
		MapHeader.Reserved[7]='\0';
		MapHeader.Reserved[8]='\0';
		MapHeader.Reserved[9]='\0';
	SetStartLocation(TOPLEFT, HORIZONTAL);
	
}

CVikingEditorDoc::~CVikingEditorDoc()
{
}

BOOL CVikingEditorDoc::OnNewDocument()
{
	
	if (!CDocument::OnNewDocument()){
		
		return FALSE;
	}

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CVikingEditorDoc serialization

void CVikingEditorDoc::Serialize(CArchive& ar)
{
	int i, j;
	if (ar.IsStoring())
	{
		CByteArray MyByteArray;
		SaveAsType=0; //Code not written yet to save printmaps
		if(SaveAsType==0){
			Serialize(MyByteArray,TRUE);
		}else{
			DispenseMapSerialize(MyByteArray,TRUE);
		}
		ar.Write(&(MyByteArray.ElementAt(0)),MyByteArray.GetSize());
	}
	else
	{
		for(i=0; i<ARRAYSIZE; i++){
			for (j=0; j<ARRAYSIZE; j++){
				RowData[i][j]=-1;
			}
		}
		CByteArray MyByteArray;
		int MyLength=ar.GetFile()->GetLength();
		MyByteArray.SetSize(MyLength);
		ar.Read(&(MyByteArray.ElementAt(0)),MyLength);
		if(MyByteArray.ElementAt(0)==(BYTE)'\x0a'){
			DispenseMapSerialize(MyByteArray,FALSE);
		}else{
			Serialize(MyByteArray,FALSE);
		}

		SetStartLocation(TOPLEFT,VERTICAL);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CVikingEditorDoc diagnostics

#ifdef _DEBUG
void CVikingEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CVikingEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVikingEditorDoc commands


void CVikingEditorDoc::GetMaterial(CString& ReturnValue)
{	
	// TODO: Finish this function
	CString AString( ' ', sizeof(MapHeader.MaterialId) );
	int i;
	for(i=0; i<sizeof(MapHeader.MaterialId); i++){
		AString.SetAt(i,MapHeader.MaterialId[i]);
	}
	ReturnValue=AString;
}


void CVikingEditorDoc::GetOperator(CString& ReturnValue)
{
	// TODO: Finish this function
	CString AString( ' ', sizeof(MapHeader.Operator) );
	int i;
	for(i=0; i<sizeof(MapHeader.Operator); i++){
		AString.SetAt(i,MapHeader.Operator[i]);
	}
	ReturnValue=AString;
	
}


void CVikingEditorDoc::GetEquipment(CString& ReturnValue)
{
	// TODO: Finish this function
	CString AString( ' ', sizeof(MapHeader.EquipId) );
	int i;
	for(i=0; i<sizeof(MapHeader.EquipId); i++){
		AString.SetAt(i,MapHeader.EquipId[i]);
	}
	ReturnValue=AString;
	
}


void CVikingEditorDoc::SetMaterial(CString NewMaterial)
{
	// TODO: Finish this function
	int i,looplength;
	looplength=((NewMaterial.GetLength() < sizeof(MapHeader.MaterialId)) ? NewMaterial.GetLength() : sizeof(MapHeader.MaterialId));
	for(i=0; i<looplength; i++){
		MapHeader.MaterialId[i]=NewMaterial.GetAt(i);
	}
	for(i=looplength; i< sizeof(MapHeader.MaterialId); i++){
		MapHeader.MaterialId[i]='\x20';
	}

}


void CVikingEditorDoc::SetEquipment(CString NewEquipment)
{	
	// TODO: Finish this function
	int i,looplength;
	looplength=((NewEquipment.GetLength() < sizeof(MapHeader.EquipId)) ? NewEquipment.GetLength() : sizeof(MapHeader.EquipId));
	for(i=0; i<looplength; i++){
		MapHeader.EquipId[i]=NewEquipment.GetAt(i);
	}
	for(i=looplength; i< sizeof(MapHeader.EquipId); i++){
		MapHeader.EquipId[i]='\x20';
	}


}

void CVikingEditorDoc::SetOperator(CString NewOperator)
{
	// TODO: Finish this function
	int i,looplength;
	looplength=((NewOperator.GetLength() < sizeof(MapHeader.Operator)) ? NewOperator.GetLength() : sizeof(MapHeader.Operator));
	for(i=0; i<looplength; i++){
		MapHeader.Operator[i]=NewOperator.GetAt(i);
	}
	for(i=looplength; i< sizeof(MapHeader.Operator); i++){
		MapHeader.Operator[i]='\x20';
	}


}

void CVikingEditorDoc::SetiStatNumbering()
{
	// TODO: Finish this function
}

//void CVikingEditorDoc::SetVikingNumbering()
//{
	// TODO: Finish this function
//}

void CVikingEditorDoc::SetStartLocation(corner WafCorner, serpentine dir)
{
	// TODO: Finish this function
	StartCorner=WafCorner;
	SerpentineDir=dir;

	switch(WafCorner){

	case TOPLEFT:
		if(SerpentineDir==HORIZONTAL){
			BuildHorizontalPath(0, MapHeader.RowCount, '\0');
			PresentRow=PathHeaderData[0].Y;
			PresentColumn=PathHeaderData[0].X;		
		}else if(SerpentineDir==VERTICAL){
			BuildVerticalPath(0, MapHeader.ColumnCount, '\0');
			PresentRow=PathHeaderData[0].Y;
			PresentColumn=PathHeaderData[0].X;		
		}
		UpdateAllViews( NULL );
		break;

	case TOPRIGHT:
		if(SerpentineDir==HORIZONTAL){
			BuildHorizontalPath(0, MapHeader.RowCount, '\1');
			PresentRow=PathHeaderData[0].Y;
			PresentColumn=PathHeaderData[0].X;			
		}else if(SerpentineDir==VERTICAL){
			BuildVerticalPath(MapHeader.ColumnCount-1, MapHeader.ColumnCount, '\0');
			PresentRow=PathHeaderData[0].Y;
			PresentColumn=PathHeaderData[0].X;
		}
		UpdateAllViews( NULL );
		break;

	case BOTTOMLEFT:
		if(SerpentineDir==HORIZONTAL){
			BuildHorizontalPath(MapHeader.RowCount-1, MapHeader.RowCount, '\0');
			PresentRow=PathHeaderData[0].Y;
			PresentColumn=PathHeaderData[0].X;			

		}else if(SerpentineDir==VERTICAL){
			BuildVerticalPath(0, MapHeader.ColumnCount, '\1');
			PresentRow=PathHeaderData[0].Y;
			PresentColumn=PathHeaderData[0].X;
		}
		UpdateAllViews( NULL );
		break;

	case BOTTOMRIGHT:
		if(SerpentineDir==HORIZONTAL){
			BuildHorizontalPath(MapHeader.RowCount-1, MapHeader.RowCount, '\1');
			PresentRow=PathHeaderData[0].Y;
			PresentColumn=PathHeaderData[0].X;	
			
		}else if(SerpentineDir==VERTICAL){
			BuildVerticalPath(MapHeader.ColumnCount-1, MapHeader.ColumnCount, '\1');
			PresentRow=PathHeaderData[0].Y;
			PresentColumn=PathHeaderData[0].X;
			
		}
		UpdateAllViews( NULL );
		break;
	
	}
}

void CVikingEditorDoc::GetStartLocation(int& Row,int & Col)
{
	// TODO: Finish this function
	Row=PathHeaderData[0].Y+1;
	Col=PathHeaderData[0].X+1;
}


BOOL CVikingEditorDoc::NextActiveLocation(void)
{
	// TODO: Finish this function

	int i,j,dir,endchip,startchip,Bincode;
	OldRow=PresentRow;
	OldColumn=PresentColumn;

	if(SerpentineDir==HORIZONTAL){
		for(i=PresentIndex; i<MapHeader.RowCount; i++){
			dir=(PathHeaderData[i].RowDir ? -1 : 1);
			startchip=(i==PresentIndex ? PresentColumn+dir: PathHeaderData[i].X);
			endchip=PathHeaderData[i].X + (dir*PathHeaderData[i].DieInRow);
			for(j=startchip; j!=endchip; j+=dir){
				Bincode=RowData[PathHeaderData[i].Y][j];
				if(IsActiveBincode(Bincode)){
					PresentIndex=i;
					PresentRow=PathHeaderData[i].Y;
					PresentColumn=j;
					UpdateAllViews( NULL ,(LPARAM)PRESENTDIE);
					return TRUE;

				}
			}
		}
	}else{
		for(i=PresentIndex; i<MapHeader.ColumnCount; i++){
			dir=(PathHeaderData[i].RowDir ? -1 : 1);
			startchip=(i==PresentIndex ? PresentRow+dir: PathHeaderData[i].Y);
			endchip=PathHeaderData[i].Y + (dir*PathHeaderData[i].DieInRow);
			for(j=startchip; j!=endchip; j+=dir){
				Bincode=RowData[j][PathHeaderData[i].X];
				if(IsActiveBincode(Bincode)){
					PresentIndex=i;
					PresentRow=j;
					PresentColumn=PathHeaderData[i].X;
					UpdateAllViews( NULL ,(LPARAM)PRESENTDIE);
					return TRUE;

				}
			}
		}
	}
	
	return FALSE;
}

void CVikingEditorDoc::GetPresentLocation(int& Row,int& Col)
{
	// TODO: Finish this function
	Row=PresentRow+1;
	Col=PresentColumn+1;
}

void CVikingEditorDoc::GetPresentCoordinates(double& X, double& Y)
{

	// TODO: Finish this function
	Y=(PresentRow*MapHeader.YDieSize)*ConversionFactorY;
	X=(PresentColumn*MapHeader.XDieSize)*ConversionFactorX;

	//addition...
	//Y+=(MapHeader.YDieSize*ConversionFactorY*1.0/*5*/);
	//X+=(MapHeader.XDieSize*ConversionFactorX*1.0/*5*/);
}

void CVikingEditorDoc::SetBincode(unsigned char Bincode)
{

	// TODO: Finish this function
	RowData[PresentRow][PresentColumn]=Bincode;
}

void CVikingEditorDoc::SetBincode(int Row, int Col, unsigned char Bincode)
{
	// TODO: Finish this function
	RowData[Row-1][Col-1]=Bincode;

}

int CVikingEditorDoc::GetBincode()
{

	// TODO: Finish this function
	return RowData[PresentRow][PresentColumn];
}

int CVikingEditorDoc::GetBincode(int Row, int Col)
{
	// TODO: Finish this function
	return RowData[Col-1][Row-1];
}

BOOL CVikingEditorDoc::IsActiveBincode(int Bincode)
{	
	// TODO: Finish this function
	int i;
	for(i=0; i< MapHeader.NumberProcessBins; i++){
		if(Bincode==(int)MapHeader.ActiveDie[i].bincode) return TRUE;
	}
	return FALSE;

}

BOOL CVikingEditorDoc::NextLocation(void)
{
	int i,j,dir,endchip,startchip;
	OldRow=PresentRow;
	OldColumn=PresentColumn;
	if(SerpentineDir==HORIZONTAL){
		for(i=PresentIndex; i<MapHeader.RowCount; i++){
			dir=(PathHeaderData[i].RowDir ? -1 : 1);
			startchip=(i==PresentIndex ? PresentColumn+dir: PathHeaderData[i].X);
			endchip=PathHeaderData[i].X + (dir*PathHeaderData[i].DieInRow);
			for(j=startchip; j!=endchip; j+=dir){
				PresentIndex=i;
				PresentRow=PathHeaderData[i].Y;
				PresentColumn=j;
				UpdateAllViews( NULL ,(LPARAM)PRESENTDIE);
				return TRUE;
			}
		}
	}else{
		for(i=PresentIndex; i<MapHeader.ColumnCount; i++){
			dir=(PathHeaderData[i].RowDir ? -1 : 1);
			startchip=(i==PresentIndex ? PresentRow+dir: PathHeaderData[i].Y);
			endchip=PathHeaderData[i].Y + (dir*PathHeaderData[i].DieInRow);
			for(j=startchip; j!=endchip; j+=dir){
				PresentIndex=i;
				PresentRow=j;
				PresentColumn=PathHeaderData[i].X;
				UpdateAllViews( NULL ,(LPARAM)PRESENTDIE);
				return TRUE;
			}
		}
	}
	
	return FALSE;
}

BOOL CVikingEditorDoc::FirstActiveLocation(void)
{
	FirstLocation();
	if(IsActiveBincode(GetBincode())){
		UpdateAllViews( NULL );
		return TRUE;
	}else{
		return NextActiveLocation();
	}
}

void CVikingEditorDoc::GetConversionFactor(double& FactorX, double& FactorY)
{
	FactorX=ConversionFactorX;
	FactorY=ConversionFactorY;
}

void CVikingEditorDoc::SetConversionFactor(double FactorX, double FactorY)
{
	ConversionFactorX=FactorX;
	ConversionFactorY=FactorY;
}

void CVikingEditorDoc::FindColumnExtent(int Column, int& Top, int& Bottom)
{
	int i;
	for (Top=i=0; i<MapHeader.RowCount; i++){
		if(RowData[i][Column]!=-1){
			Top=i;
			break;
		}
	}
	for (Bottom=0,i=MapHeader.RowCount-1; i>=0; i--){
		if(RowData[i][Column]!=-1){
			Bottom=i;
			break;
		}
	}
}

BOOL CVikingEditorDoc::FirstLocation()
{
	if(SerpentineDir==HORIZONTAL){
		PresentRow=PathHeaderData[0].Y;
		PresentColumn=PathHeaderData[0].X;
	}else{
		PresentRow=PathHeaderData[0].Y;
		PresentColumn=PathHeaderData[0].X;
	}
	PresentIndex=0;
	UpdateAllViews( NULL );
	return TRUE;
}

void CVikingEditorDoc::BuildVerticalPath(int StartCol, int ColCount, char UpDown)
{
	int LeftRight;
	int i,j;
	int Top, Bottom;

	LeftRight=(StartCol==0 ? 1: -1);

	//PresentRowDir='\1';
	for (j=0, i=StartCol; j<ColCount; i+=LeftRight, j++){

		FindColumnExtent(i,Top,Bottom);

		PathHeaderData[j].X=(short)i;
		PathHeaderData[j].RowDir=UpDown;
		UpDown=(UpDown=='\1'? '\0' : '\1' );
		PathHeaderData[j].DieInRow=(short)(Bottom-Top+1);
		PathHeaderData[j].Y=(short)(PathHeaderData[j].RowDir ? Bottom : Top );
			
	}
	PresentIndex=0;
}

void CVikingEditorDoc::BuildHorizontalPath(int StartRow, int RowCount, char LeftRight)
{
	int UpDown;
	int i,j;

	UpDown=(StartRow==0 ? 1: -1);

	for (j=0, i=StartRow; j< RowCount; i+=UpDown, j++){
		PathHeaderData[j].Y=(short)i;
		PathHeaderData[j].RowDir=LeftRight;
		LeftRight=(LeftRight=='\1'? '\0' : '\1' );
		PathHeaderData[j].DieInRow=RowHeaderData[i].DieInRow;

		if(RowHeaderData[i].RowDir==PathHeaderData[j].RowDir){
			PathHeaderData[j].X=RowHeaderData[i].X;
		}else if(PathHeaderData[j].RowDir=='\1'){
			PathHeaderData[j].X=(short)(RowHeaderData[i].X+(RowHeaderData[i].DieInRow-1));
		}else{
			PathHeaderData[j].X=(short)(RowHeaderData[i].X-(RowHeaderData[i].DieInRow-1));
		}
	}
	PresentIndex=0;
}

void CVikingEditorDoc::FindRowExtent(int Row, int& Left, int& Right)
{
	if(RowHeaderData[Row].RowDir==0){ //L->R
		Left=RowHeaderData[Row].X;
		Right=RowHeaderData[Row].X + (RowHeaderData[Row].DieInRow-1);
	}else{
		Left=RowHeaderData[Row].X - (RowHeaderData[Row].DieInRow-1);
		Right=RowHeaderData[Row].X;
	}

}

void CVikingEditorDoc::GetRCCount(int& RowCount, int& ColumnCount)
{
	RowCount=MapHeader.RowCount;
	ColumnCount=MapHeader.ColumnCount;
}


int CVikingEditorDoc::InsertBlobAsMap(CByteArray& MapBlob)
{
	if(MapBlob.ElementAt(0)==(BYTE)'\x0a'){
		DispenseMapSerialize(MapBlob,FALSE);
	}else{
		Serialize(MapBlob,FALSE);
	}
	SetStartLocation(BOTTOMRIGHT,VERTICAL);
	return(1);
}

void CVikingEditorDoc::DispenseMapSerialize(CByteArray& MapBlob,BOOL Output)
{
	int OffsetCount;
	int i, j, Col, StepDir, RowStartLocation;
	unsigned char ReadValue, WriteValue;
	int MinCol, MaxCol;

	if(Output){
		MapBlob.SetSize(sizeof(MapHeader)+sizeof(RowData[0][0])*MapHeader.ColumnCount*MapHeader.RowCount);
		memcpy(&(MapBlob.ElementAt(0)),&MapHeader,sizeof(MapHeader));

		for(i=0,OffsetCount=256; i<MapHeader.RowCount; i++){
			memcpy(&(MapBlob.ElementAt(OffsetCount)),&RowHeaderData[i],sizeof(RowHeader));
			OffsetCount+=sizeof(RowHeader);
			RowStartLocation= RowHeaderData[i].X;
			StepDir=(RowHeaderData[i].RowDir ? -1 : 1);
			for (j=0; j<RowHeaderData[i].DieInRow; j++){
				Col=RowStartLocation+(j*StepDir);
				WriteValue=(unsigned char)RowData[i][Col];
				MapBlob.SetAt(OffsetCount+j,WriteValue);


			}
			OffsetCount+=RowHeaderData[i].DieInRow;
		}
		MapBlob.SetSize(OffsetCount);
	}else{
		memcpy(&MapHeader.XDieSize,&(MapBlob.ElementAt(1)),sizeof(MapHeader.XDieSize));
		memcpy(&MapHeader.YDieSize,&(MapBlob.ElementAt(5)),sizeof(MapHeader.YDieSize));
		memcpy(&MapHeader.ColumnCount,&(MapBlob.ElementAt(9)),sizeof(MapHeader.ColumnCount));
		memcpy(&MapHeader.RowCount,&(MapBlob.ElementAt(11)),sizeof(MapHeader.RowCount));
		MapHeader.DieUOM=MapBlob.ElementAt(14);
		MapHeader.XDieSize=-MapHeader.XDieSize;
		//fill in
		memset(&MapHeader.MaterialId[0],32,sizeof(MapHeader.MaterialId));
		memset(&MapHeader.EquipId[0],32,sizeof(MapHeader.EquipId));
		memset(&MapHeader.Operator[0],32,sizeof(MapHeader.Operator));

		MapHeader.NumberOfRefPoints=0;
		for(i=0; i<16; i++){
			MapHeader.ReferencePoint[i].X=0;
			MapHeader.ReferencePoint[i].X=0;
		}

		MapHeader.NullBincode=0;
		ActiveListClear();
		ActiveListInsert(unsigned char('\xff'));

		//MapHeader.MapFormat;
		//MapHeader.OriginLocationCode=;
		//MapHeader.FlatLocation=;
		//MapHeader.FrameLocation=;
		//MapHeader.ProcessType=;
		//MapHeader.ProcessStatus=;

		for(i=0; i<ARRAYSIZE; i++){
			for (j=0; j<ARRAYSIZE; j++){
				RowData[i][j]=-1;
			}
		}

	
		for(i=MapHeader.RowCount-1,OffsetCount=32; i>=0; i--,OffsetCount+=MapHeader.ColumnCount){
			RowHeaderData[i].Y=short(-i); //using Viking Co-ordinates
			RowHeaderData[i].RowDir=char(i & 0x1);	//0 L->R, 1 R->L
			MinCol=ARRAYSIZE;
			MaxCol=0;
			
			for (j=MapHeader.ColumnCount-1; j>=0; j--){
				//RowHeaderData[i].DieInRow
				//Col=RowStartLocation+(j*StepDir);
				ReadValue=unsigned char(MapBlob.GetAt(OffsetCount+MapHeader.ColumnCount-j-1));
				if(ReadValue!='\0'){
					RowData[i][j]=int(ReadValue);
					MinCol=min(MinCol,j);
					MaxCol=max(MaxCol,j);
					if(ReadValue!=unsigned char(255)){
						if(ReadValue&0x80){
							ActiveListInsert(ReadValue);
						}
					}
				}
			}

			if(MinCol==ARRAYSIZE){
				RowHeaderData[i].X=0; //using Viking Co-ordinates
				RowHeaderData[i].DieInRow=0;
			}else{
				RowHeaderData[i].X=short(i & 0x1 ? MaxCol :MinCol); //using Viking Co-ordinates
				RowHeaderData[i].DieInRow=unsigned short(MaxCol-MinCol+1);
			}
		}
	}
}


void CVikingEditorDoc::Serialize(CByteArray& MapBlob,BOOL Output)
{
	int OffsetCount;
	int i, j, Col, StepDir, RowStartLocation;
	unsigned char ReadValue, WriteValue;

	if(Output){
		MapBlob.SetSize(sizeof(MapHeader)+sizeof(RowData[0][0])*MapHeader.ColumnCount*MapHeader.RowCount);
		memcpy(&(MapBlob.ElementAt(0)),&MapHeader,sizeof(MapHeader));

		for(i=0,OffsetCount=256; i<MapHeader.RowCount; i++){
			memcpy(&(MapBlob.ElementAt(OffsetCount)),&RowHeaderData[i],sizeof(RowHeader));
			OffsetCount+=sizeof(RowHeader);
			RowStartLocation= RowHeaderData[i].X;
			StepDir=(RowHeaderData[i].RowDir ? -1 : 1);
			for (j=0; j<RowHeaderData[i].DieInRow; j++){
				Col=RowStartLocation+(j*StepDir);
				WriteValue=(unsigned char)RowData[i][Col];
				MapBlob.SetAt(OffsetCount+j,WriteValue);


			}
			OffsetCount+=RowHeaderData[i].DieInRow;
		}
		MapBlob.SetSize(OffsetCount);
	}else{
		memcpy(&MapHeader,&(MapBlob.ElementAt(0)),256);
	
		for(i=0; i<ARRAYSIZE; i++){
			for (j=0; j<ARRAYSIZE; j++){
				RowData[i][j]=-1;
			}
		}

	
		for(i=0,OffsetCount=256; i<MapHeader.RowCount; i++){
			memcpy(&RowHeaderData[i],&(MapBlob.ElementAt(OffsetCount)),sizeof(RowHeader));
			OffsetCount+=sizeof(RowHeader);
			
			RowStartLocation= RowHeaderData[i].X;
			StepDir=(RowHeaderData[i].RowDir ? -1 : 1);

			for (j=0; j<RowHeaderData[i].DieInRow; j++){
				Col=RowStartLocation+(j*StepDir);
				ReadValue=MapBlob.GetAt(OffsetCount+j);
				RowData[i][Col]=(int)ReadValue;
			}
			OffsetCount+=RowHeaderData[i].DieInRow;
		}

	}
}

BOOL CVikingEditorDoc::ActiveListInsert(unsigned char BinCode)
{
	if(IsActiveBincode( BinCode)){
		return TRUE;
	}else if(MapHeader.NumberProcessBins>=32){
		return FALSE;
	}else{
		MapHeader.NumberProcessBins++;
		MapHeader.ActiveDie[MapHeader.NumberProcessBins-1].bincode=BinCode;
		return TRUE;
	}
	return TRUE;
}

//void CVikingEditorDoc::ActiveListRemove(unsigned char BinCode)
//{
//	int i;
//	for(i=0; i< MapHeader.NumberProcessBins; i++){
//		if(Bincode==(int)MapHeader.ActiveDie[i].bincode) return TRUE;
//	}

//}

void CVikingEditorDoc::ActiveListClear( )
{
	int i;
	MapHeader.NumberProcessDie=0;
	MapHeader.NumberProcessBins=0;
	for(i=0; i< 32; i++){
		MapHeader.ActiveDie[i].bincode='\0';
		MapHeader.ActiveDie[i].count=0;
	}


}

void CVikingEditorDoc::ClearBitInMap(int BitNumber)
{
	int x,y;
	int BitPattern= ~(1<<BitNumber);
	BinCodeUnion Bincode;
	for(x=0; x< ARRAYSIZE; x++){
		for(y=0; y< ARRAYSIZE; y++){
			Bincode.BinCode=RowData[x][y];
			if(Bincode.BinCode!=-1){
				if(Bincode.DecodedBinCode.Active){
					RowData[x][y]&=BitPattern;
				}
			}
		}
	}

}

void CVikingEditorDoc::SetBitInMap(int BitNumber)
{
	int x,y;
	int BitPattern= 1<<BitNumber;
	BinCodeUnion Bincode;
	for(x=0; x< ARRAYSIZE; x++){
		for(y=0; y< ARRAYSIZE; y++){
			Bincode.BinCode=RowData[x][y];
			if(Bincode.BinCode!=-1){
				if(Bincode.DecodedBinCode.Active){
					RowData[x][y]|=BitPattern;
				}
			}
		}
	}
}

BOOL CVikingEditorDoc::CanCloseFrame(CFrameWnd* pFrame) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if(!m_bOkToClose){
		return FALSE;
	}else{
		return CDocument::CanCloseFrame(pFrame);
	}
}

double CVikingEditorDoc::XDieSize(void)
{
   return (double)(MapHeader.XDieSize)*ConversionFactorX;// * 1.0035;// * (1.0 + 0.015*(double(MapHeader.XDieSize)/double(MapHeader.YDieSize)));
}

double CVikingEditorDoc::YDieSize(void)
{
   return (double)(MapHeader.YDieSize)*ConversionFactorY;// * 1.0035;// * (1.0 + 0.03*(double(MapHeader.YDieSize)/double(MapHeader.XDieSize)));
}
