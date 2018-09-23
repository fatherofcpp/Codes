// SimpleResize.h: interface for the SimpleResize class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMPLERESIZE_H__1B6B7F9A_67C9_4568_A62A_592FDECEFA59__INCLUDED_)
#define AFX_SIMPLERESIZE_H__1B6B7F9A_67C9_4568_A62A_592FDECEFA59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class SimpleResize 
{
public:
    SimpleResize(long oldw,long oldh, long neww,long newh,int* res);// ¹¹ÔìÆ÷
    virtual    ~SimpleResize();
    void resize(BYTE* src,BYTE* dst);

private:
    long newwidth;
    long newheight;
    long oldwidth;
    long oldheight;
    unsigned int* hControl;        // weighting masks, alternating dwords for Y & UV
	// 1 qword for mask, 1 dword for src offset, 1 unused dword
    unsigned int* vOffsets;        // Vertical offsets of the source lines we will use
    unsigned int* vWeights;        // weighting masks, alternating dwords for Y & UV
    unsigned int* vOffsetsUV;        // Vertical offsets of the source lines we will use
    unsigned int* vWeightsUV;        // weighting masks, alternating dwords for Y & UV
    unsigned int* vWorkY;        // weighting masks 0Y0Y 0Y0Y...
    unsigned int* vWorkUV;        // weighting masks UVUV UVUV...
	
    void InitTables_YV12(void);
public:
    void GetFrame_YV12(BYTE* src, BYTE* dst, int srcpitch,int dstpitch,int Planar_Type,int dstheight);
};

#endif // !defined(AFX_SIMPLERESIZE_H__1B6B7F9A_67C9_4568_A62A_592FDECEFA59__INCLUDED_)
