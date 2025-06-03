// VikingEditorDoc.h : interface of the CVikingEditorDoc class
//
/////////////////////////////////////////////////////////////////////////////
#ifndef VIKINGEDITOR
#define VIKINGEDITOR
#include "VikingHeader.h"
enum serpentine { HORIZONTAL,VERTICAL};
enum corner {CENTRE=0,TOPRIGHT=1,TOPLEFT=2,BOTTOMLEFT=3,BOTTOMRIGHT=4};
enum viewdelta {PRESENTDIE=1,HEADER=2};

union BinCodeUnion {
	int BinCode;
	struct {
		BOOL Dispense:1;
		BOOL SkipThickness:1;
		BOOL SkipWash:1;
		BOOL SkipWarmup:1;
		BOOL SkipDiameter:1;
		BOOL SkipStatValReset:1;
		BOOL NeedsProcessing:1;
		BOOL Active:1;	
	} DecodedBinCode;
};

#define ARRAYSIZE 40

class CVikingEditorDoc : public CDocument
{
//protected: // create from serialization only
public:	
	CVikingEditorDoc();
	DECLARE_DYNCREATE(CVikingEditorDoc)
	

// Attributes
public:
	void SetBitInMap(int BitNumber);
	void ClearBitInMap(int BitNumber);
	void ActiveListClear( );
	//void ActiveListRemove(unsigned char BinCode);
	BOOL ActiveListInsert(unsigned char BinCode);
	virtual void Serialize(CByteArray& MapBlob,BOOL Output);
	virtual void DispenseMapSerialize(CByteArray& MapBlob,BOOL Output);
	int SaveAsType;
	BOOL m_bOkToClose;
	int PresentColumn; //in array indexes
	int PresentRow; //in array indexes
	int OldColumn; //in array indexes
	int OldRow; //in array indexes

	int InsertBlobAsMap(CByteArray& MapBlob);
	void GetRCCount(int& RowCount, int& ColumnCount);
	void SetConversionFactor(double FactorX,double FactorY);
	void GetConversionFactor(double& FactorX,double& FactorY);
	BOOL FirstActiveLocation(void);
	BOOL NextLocation(void);
	BOOL FirstLocation();
	BOOL IsActiveBincode(int Bincode);
	void SetBincode(int Row, int Col, unsigned char Bincode); //SetBincode at a specific location
	void SetBincode(unsigned char Bincode); //Set Bincode at present location
	int GetBincode(int Row, int Col); //SetBincode at a specific location
	int GetBincode(void); //Set Bincode at present location
	void GetPresentCoordinates(double& X, double& Y); //Get present distance from start location in engineering units
	void GetPresentLocation(int& Row,int& Col); //Get present location in rows and column offset from start location
	BOOL NextActiveLocation(void); //move to next active location
	void GetStartLocation(int& Row,int & Col); //must be a corner location
	void SetStartLocation(corner, serpentine dir); //must be a corner location
	void SetiStatNumbering(); //use i-stat chip numbering scheme
	void SetEquipment(CString NewEquipment); 
	void GetEquipment(CString& EquipmentName);
	void SetMaterial(CString NewMaterial);
	void GetMaterial(CString& NewMaterial);
	void SetOperator(CString OperatorName);
	void GetOperator(CString& OperatorName);
	
	double XDieSize(void);
	double YDieSize(void);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVikingEditorDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CVikingEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CVikingEditorDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void FindRowExtent(int Row, int& Left, int& Right);
	void BuildHorizontalPath(int StartRow, int RowCount, char LeftRight);
	void BuildVerticalPath(int StartCol, int ColCount, char UpDown);
	int PresentIndex;
	int StartColumn; //in array indexes
	int StartRow; //in array indexes
	double ConversionFactorX;
	double ConversionFactorY;
	BOOL NumberingConvention; //not presently used (determines i-Stat or Viking numbering
	serpentine SerpentineDir; 
	corner StartCorner;
	int RowData[ARRAYSIZE][ARRAYSIZE];
	RowHeader RowHeaderData[ARRAYSIZE];
	RowHeader PathHeaderData[ARRAYSIZE];
	CByteArray MapData;
	VikingHeader MapHeader;
	void FindColumnExtent(int Column, int& Top, int& Bottom);
};

/////////////////////////////////////////////////////////////////////////////
#endif
