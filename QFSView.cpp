// QFSView.cpp : implementation file
//
// Modified by Hoo, 8/16/1999 (Total: 6 items)
// Hoo's D3D View functions added by Nappe1, (2/11/2001) 

#include "stdafx.h"
#include "T3ED.h"
// #include "QFSView.h"  will be taken care of by T3EDDoc.h
#include "T3EDDoc.h"
#include "TextureProps.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQFSView dialog


CQFSView::CQFSView(CWnd* pParent /*=NULL*/)
	: CDialog(CQFSView::IDD, pParent)
{
	//{{AFX_DATA_INIT(CQFSView)
	m_numBmp = _T("");
	m_nBmp = 0;
	m_strLabel = _T("");
	//}}AFX_DATA_INIT
	pBitmaps=NULL;
	MakeStockBitmaps();
	promptLoad=FALSE;
	//HOO: Check if Qfs is loaded (1)
	bIsLoadQfs=FALSE;
	//HOO: Required by Track view to display texture (1)


}

#define QFS_OK 0
#define QFS_CANCELLED 1
#define QFS_CANTOPEN 2
#define QFS_BADFORMAT 3
#define QFS_TOOLARGE 4

void CQFSView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	if (promptLoad&&!pDX->m_bSaveAndValidate) {
		promptLoad=FALSE;
		switch (LoadQFS()) {
		case QFS_CANTOPEN:
			AfxMessageBox("Error loading QFS file.\nTextures will not be displayed.");
			break;
		case QFS_BADFORMAT:
			AfxMessageBox("QFS file format error.\nTextures will not be displayed.");
			Reset();
			break;
		case QFS_TOOLARGE:
			AfxMessageBox("QFS file too large.\nTextures will not be displayed.");
			Reset();
			break;
		//HOO: (2)
		case QFS_OK:
			bIsLoadQfs=TRUE;
			break;
		//HOO: (2)
		}
	}
	if (!pDX->m_bSaveAndValidate) m_numBmp.Format("%d",nColEntries);

	//{{AFX_DATA_MAP(CQFSView)
	DDX_Control(pDX, IDC_HSPROPS, m_hsprops);
	DDX_Control(pDX, IDC_SPIN1, m_spinCtrl);
	DDX_Control(pDX, IDC_BITMAP, m_bmpCtrl);
	DDX_Text(pDX, IDC_NUMBMP, m_numBmp);
	DDX_Text(pDX, IDC_EDIT1, m_nBmp);
	DDX_Text(pDX, ID_LABEL, m_strLabel);
	//}}AFX_DATA_MAP
	DDV_MinMaxInt(pDX, m_nBmp, 0, nColEntries-1);

	if (!pDX->m_bSaveAndValidate) {
		if (bHSMode) m_hsprops.ModifyStyle(WS_DISABLED,0);
		else m_hsprops.ModifyStyle(0,WS_DISABLED);
		m_spinCtrl.SetRange(0,nColEntries-1);
		m_spinCtrl.SetPos(m_nBmp);
		if (col[m_nBmp].islane) {
			if (col[m_nBmp].texture>=14)
				 m_bmpCtrl.SetBitmap(NULL);
			else m_bmpCtrl.SetBitmap(pStockBitmaps[col[m_nBmp].texture]);
		} else
		if ((pBitmaps==NULL)||(col[m_nBmp].texture>=nQfsEntries)||
			(pBitmaps[col[m_nBmp].texture]==NULL)) 
		{
			m_bmpCtrl.SetBitmap(NULL);
		} else {
			m_bmpCtrl.SetBitmap(pBitmaps[col[m_nBmp].texture]);
		}
	}
}

void CQFSView::Reset()
{
	if (pBitmaps!=NULL) {
		for (int i=0;i<nQfsEntries;i++)
			if (pBitmaps[i]!=NULL) { 
				::DeleteObject(pBitmaps[i]);
				pBitmaps[i]=NULL;
			}
		free(pBitmaps);
	}
	pBitmaps=NULL;
	//HOO: (3)
	bIsLoadQfs=FALSE;
	//HOO: (3)
}

#define BUFFER_LEN    0x80000  // what we malloc
#define BUFFER_KEEP   0x20000  // max. LZ77 offset
#define BUFFER_MARGIN 1040     // > what can be written in one pass

BOOL CQFSView::QFSReachOffset(CFile &qfs,int toreach,unsigned char *x,int &bufpos,int &bufshift)
{
	unsigned char pack_byte,a,b,c;
	unsigned int len;
	unsigned char *p,*q;
	int offset;

	if (toreach+BUFFER_MARGIN-bufshift>BUFFER_LEN) {
		if (toreach+BUFFER_MARGIN-(bufshift+bufpos-BUFFER_KEEP)>BUFFER_LEN) 
			return FALSE; // too large
		bufshift+=bufpos-BUFFER_KEEP;
		memmove(x,x+bufpos-BUFFER_KEEP,BUFFER_KEEP);
		bufpos=BUFFER_KEEP;
	}
    while (bufshift+bufpos<toreach) {
		if (qfs.Read(&pack_byte,1)!=1) return FALSE;
		if (pack_byte>=0xfc) { /* 0xFC-0xFF should end file */
			len=pack_byte&3;
			if (len) if (qfs.Read(x+bufpos,len)!=len) return FALSE;
			bufpos+=len;
			break;
        }
		if (pack_byte>=0xe0) { /* 0xE0-0xFB */
			len=(pack_byte&0x1f)*4+4;
			if (qfs.Read(x+bufpos,len)!=len) return FALSE;
			bufpos+=len;
			continue;
		}
		if (qfs.Read(&a,1)!=1) return FALSE;
        if (!(pack_byte & 0x80)) { /* 0x00-0x7F */
			len=pack_byte&3;
			if (len) if (qfs.Read(x+bufpos,len)!=len) return FALSE;
			bufpos+=len;
			len=((pack_byte&0x1c)>>2)+3;
			offset=(((int)pack_byte>>5)<<8)+a+1;
			p=x+bufpos; q=p-offset; bufpos+=len;
			while (len--) *(p++)=*(q++);
		} else { 
			if (qfs.Read(&b,1)!=1) return FALSE;
            if (!(pack_byte & 0x40)) { /* 0x80-0xBF */
                len=(a>>6)&3;
				if (len) if (qfs.Read(x+bufpos,len)!=len) return FALSE;
				bufpos+=len;
                len=(pack_byte&0x3f)+4;
                offset=((int)a&0x3f)*256+b+1;
				p=x+bufpos; q=p-offset; bufpos+=len;
				while (len--) *(p++)=*(q++);
            }
            else { /* 0xC0-0xDF */
				if (qfs.Read(&c,1)!=1) return FALSE;
                len=pack_byte&3;
				if (len) if (qfs.Read(x+bufpos,len)!=len) return FALSE;
				bufpos+=len;
                len=(((int)pack_byte>>2)&3)*256+c+5;
                offset=(((int)pack_byte&0x10)<<12)+256*(int)a+b+1;
				p=x+bufpos; q=p-offset; bufpos+=len;
				while (len--) *(p++)=*(q++);
            }
        }
	}
	return TRUE;
}

int CQFSView::LoadQFS()
{
	unsigned char *buf1;
	unsigned char u[16];
	int bufpos,bufshift,startofs[1000],fshlen,pos,palpos;
	int i,j,k,r,g,b,nsz,tgtno;
	struct MYBITMAPINFO {
		BITMAPINFOHEADER head;
		long palette[256];
	} bmi;
	unsigned char *bmpbits,*tgt;

	CFile qfs;
	CFileDialog fOpenDlg(TRUE,"qfs",defName,OFN_FILEMUSTEXIST|OFN_HIDEREADONLY,
		"Texture Files (*.qfs)|*.qfs|All Files (*.*)|*.*||");
	CQFSLoadProgress *qfsloadbox;

	fOpenDlg.m_ofn.lpstrInitialDir=defPath;
	if (fOpenDlg.DoModal()!=IDOK) return QFS_CANCELLED;
	fOpenDlg.GetPathName();
	if (!qfs.Open(fOpenDlg.GetPathName(),CFile::modeRead|CFile::shareDenyWrite)) {
		return QFS_CANTOPEN;
	}

	if (qfs.Read(u,5)!=5) return QFS_BADFORMAT;
    if (((u[0]&0xfe)!=0x10)||(u[1]!=0xfb)) return QFS_BADFORMAT;
	buf1=(unsigned char *)malloc(BUFFER_LEN); // window on FSH file
    if (u[0]&1) qfs.Read(u+5,3);   // align if necessary.
    bufpos=0;
	bufshift=0;
    if (!QFSReachOffset(qfs,16,buf1,bufpos,bufshift))
		{ free(buf1); return QFS_BADFORMAT; }
	if (buf1[0]!='S'||buf1[1]!='H'||buf1[2]!='P'||buf1[3]!='I')
		{ free(buf1); return QFS_BADFORMAT; }
	memcpy(&fshlen,buf1+4,4);
	memcpy(&nQfsEntries,buf1+8,4);
	if (nQfsEntries>=1000) { free(buf1); return QFS_TOOLARGE; }
	if (!QFSReachOffset(qfs,16+8*nQfsEntries,buf1,bufpos,bufshift))
		{ free(buf1); return QFS_BADFORMAT; }
	for (i=0;i<nQfsEntries;i++)
		memcpy(startofs+i,buf1+16+8*i+4,4);
	startofs[nQfsEntries]=fshlen;
	pBitmaps=(HBITMAP *)malloc(nQfsEntries*sizeof(HBITMAP));
	for (i=0;i<nQfsEntries;i++) pBitmaps[i]=NULL;

	qfsloadbox=new CQFSLoadProgress;
	qfsloadbox->Create(IDD_LOADING_QFS,NULL);
	qfsloadbox->ShowWindow(TRUE);
	qfsloadbox->UpdateData(FALSE);
	qfsloadbox->m_progress.SetRange(0,nQfsEntries);
	BeginWaitCursor();

	tgtno=0;
	for (i=0;i<nQfsEntries;i++) {
		qfsloadbox->m_progress.SetPos(i);
		if (startofs[i+1]-startofs[i]>BUFFER_LEN-BUFFER_MARGIN-BUFFER_KEEP)
			{ EndWaitCursor(); free(buf1); delete qfsloadbox; return QFS_TOOLARGE; }
		if (!QFSReachOffset(qfs,startofs[i+1],buf1,bufpos,bufshift))
			{ EndWaitCursor(); free(buf1); delete qfsloadbox; return QFS_BADFORMAT; }
		pos=startofs[i]-bufshift;
		memcpy(u,buf1+pos,16);
		pos+=16;
		bmi.head.biSize=sizeof(BITMAPINFOHEADER);
		bmi.head.biWidth=((int)u[5])*256+u[4];
		bmi.head.biHeight=((int)u[7])*256+u[6];
		bmi.head.biPlanes=1;
		bmi.head.biCompression=BI_RGB;
		bmi.head.biSizeImage=0;
		bmi.head.biXPelsPerMeter=0;
		bmi.head.biYPelsPerMeter=0;
		bmi.head.biClrUsed=0;
		bmi.head.biClrImportant=0;
		int ii=0; unsigned short temp=0; unsigned long temp2=0; //Nappe1
		double temp3=0; //Nappe1
		switch (u[0]) {
		case 0x7B: // 8-bit bitmap with 15-bit local palette
			palpos=((int)u[3])*65536+((int)u[2])*256+u[1]+(pos-16);
			memcpy(u,buf1+palpos,16);
			if (u[0]!=0x2D) continue; // wrong palette type
			if (bHSMode&&((u[1]!=0)||(u[2]!=0)||(u[3]!=0))) {
				// there is a comment ! What does it say ?
				j=u[1]+256*u[2]+65536*u[3];
				if (buf1[palpos+j]==111)
					if (!strncmp((char *)(buf1+palpos+j+8),"<mirrored>",10))
					{ tgtno--; break; }
			}
			bmi.head.biBitCount=8;
			bmi.head.biClrUsed=((int)u[5])*256+u[4];
			for (j=0;j<(int)bmi.head.biClrUsed;j++) {
				k=buf1[palpos+16+2*j]+256*buf1[palpos+16+2*j+1];
				b=(k&0x1F)<<3;
				g=((k>>5)&0x1F)<<3;
				r=((k>>10)&0x1F)<<3;
				bmi.palette[j]=(r<<16)+(g<<8)+b;
			}
			pBitmaps[tgtno]=::CreateDIBSection(NULL,// need a DC ?
				(BITMAPINFO *)&bmi,DIB_RGB_COLORS,(void **)&bmpbits,NULL,0);
			nsz=1+((bmi.head.biWidth-1)|3);
			tgt=bmpbits+bmi.head.biHeight*nsz;
			for (j=0;j<bmi.head.biHeight;j++) {
				tgt-=nsz;
				memcpy(tgt,buf1+pos,bmi.head.biWidth);
				pos+=bmi.head.biWidth;
			}
			break;

		case 0x7D: // 32-bit bitmap
			bmi.head.biBitCount=32;
			pBitmaps[tgtno]=::CreateDIBSection(NULL,// need a DC ?
				(BITMAPINFO *)&bmi,DIB_RGB_COLORS,(void **)&bmpbits,NULL,0);
			nsz=4*bmi.head.biWidth;
			tgt=bmpbits+bmi.head.biHeight*nsz;
			for (j=0;j<bmi.head.biHeight;j++) {
				tgt-=nsz;
				memcpy(tgt,buf1+pos,4*bmi.head.biWidth);
				pos+=4*bmi.head.biWidth;
			}
			break;

		case 0x7E: // 16-bit bitmap, Win32 coding
			bmi.head.biBitCount=16;
			pBitmaps[tgtno]=::CreateDIBSection(NULL,// need a DC ?
				(BITMAPINFO *)&bmi,DIB_RGB_COLORS,(void **)&bmpbits,NULL,0);
			nsz=2*(1+((bmi.head.biWidth-1)|1));
			tgt=bmpbits+bmi.head.biHeight*nsz;
			for (j=0;j<bmi.head.biHeight;j++) {
				tgt-=nsz;
				memcpy(tgt,buf1+pos,2*bmi.head.biWidth);
				pos+=2*bmi.head.biWidth;
			}
			break;
		
		case 0x78: // 16-bit bitmap, Win32 coding
			ii=0;
			temp=0;
			temp2=0; 
			temp3=0; 
			bmi.head.biBitCount=32;
			pBitmaps[tgtno]=::CreateDIBSection(NULL,// need a DC ?
				(BITMAPINFO *)&bmi,DIB_RGB_COLORS,(void **)&bmpbits,NULL,0);
			//nsz=2*(1+((bmi.head.biWidth-1)|1));
			//tgt=bmpbits+bmi.head.biHeight*nsz;
			//tgt=bmpbits+bmi.head.biHeight*nsz;
			
			for (j=bmi.head.biHeight-1;j>-1;j--) {
				for (ii=0;ii<bmi.head.biWidth;ii++){
				//11111 111111 11111
					memcpy(&temp,buf1+pos,2);
					unsigned char pad[4];
					
					/*unsigned char tempchar1 = (unsigned char) temp & 63488;
					unsigned char tempchar2 = (unsigned char) temp & 2016;
					unsigned char tempchar3 = (unsigned char) temp & 31;

					tempchar1 = (unsigned char)((double) tempchar1 / (double)31)*255;
					tempchar2 = (unsigned char)((double) tempchar2 / (double)63)*255;
					tempchar3 = (unsigned char)((double) tempchar3 / (double)31)*255;
					//temp3=(double) temp / (double)65536;
					//temp2=(unsigned int) (temp3* (double)16777216);
					temp2 = (((unsigned int) tempchar1 << 16)+((unsigned int) tempchar2 << 8)+((unsigned int) tempchar3));*/
					pad[3]=255; 
					pad[0]=(temp&0x1f)<<3;
					pad[1]=((temp>>5)&0x3f)<<2;
					pad[2]=((temp>>11)&0x1f)<<3;
					//temp2 = (unsigned int) pad;
					tgt=bmpbits+(j*bmi.head.biWidth*4)+(ii*4);
					memcpy(tgt,&pad,4 );
					pos++;
					pos++;
				}
			}

			//for (j=0;j<bmi.head.biHeight;j++) {
				//tgt-=nsz;
				//memcpy(tgt,buf1+pos,2*bmi.head.biWidth);
				//pos+=2*bmi.head.biWidth;
			//}
			break;
		default: ASSERT(FALSE);
		}
		tgtno++;
	}
	nQfsEntries=tgtno;

	EndWaitCursor();
	free(buf1);
	delete qfsloadbox;
	return QFS_OK;
}

void CQFSView::MakeStockBitmaps()
{
	int i,j,k,pattern[2];
	int *bmpbits;
	struct MYBITMAPINFO {
		BITMAPINFOHEADER head;
		long bgcolor,fgcolor;
	} bmi;

	bmi.head.biSize=sizeof(BITMAPINFOHEADER);
	bmi.head.biWidth=16;
	bmi.head.biHeight=128;
	bmi.head.biPlanes=1;
	bmi.head.biCompression=BI_RGB;
	bmi.head.biSizeImage=0;
	bmi.head.biXPelsPerMeter=0;
	bmi.head.biYPelsPerMeter=0;
	bmi.head.biClrUsed=2;
	bmi.head.biClrImportant=0;
	bmi.head.biBitCount=1;
	bmi.bgcolor=0x808080;
	for (i=0;i<14;i++) {
		if (i<=4) bmi.fgcolor=0xffff00;
		else if (i<=9) bmi.fgcolor=0xffffff;
		else bmi.fgcolor=0;
		pStockBitmaps[i]=::CreateDIBSection(NULL,
			(BITMAPINFO *)&bmi,DIB_RGB_COLORS,(void **)&bmpbits,NULL,0);
		if (((i%5)<=1)||(i>=10)) pattern[0]=0x8001; // single line 
		else pattern[0]=0x6006;                     // double line
		if (i>=12) pattern[1]=0;					// dashed
		else if (i>=10) pattern[1]=0x8001;			// solid
		else if ((i%5)==1) pattern[1]=0;			// dashed
		else if ((i%5)<=2) pattern[1]=pattern[0];	// solid
		else if ((i%5)==3) pattern[1]=0x6000;       // dashed-solid
		else pattern[1]=0x0006;                     // solid-dashed
		for (j=0;j<128;j++) bmpbits[j]=pattern[((j+8)>>4)&1];
	}
	//HOO: Create textures for the lanes on the road (4)
	long *bmpbitsl;
	bmi.head.biWidth=32;
	bmi.head.biHeight=32;
	bmi.head.biBitCount=8;
	bmi.head.biSizeImage=0;
	bmi.head.biXPelsPerMeter=0;
	bmi.head.biYPelsPerMeter=0;
	bmi.head.biClrUsed=2;
	bmi.head.biClrImportant=0;
	bmi.bgcolor=0x000000;
	//Yellow Single - 0x00f81f00
	bmi.fgcolor=0xffff00;
	pStockTextures[0]=::CreateDIBSection(NULL,
		(BITMAPINFO *)&bmi,DIB_RGB_COLORS,(void **)&bmpbitsl,NULL,0);
	for (j=0;j<32;j++)
	{
		for (k=0;k<2;k++) bmpbitsl[j*8+k]=0;
		bmpbitsl[j*8+2]=0x01010000;
		for (k=3;k<5;k++) bmpbitsl[j*8+k]=0x01010101;
		bmpbitsl[j*8+5]=0x00000101;
		for (k=6;k<8;k++) bmpbitsl[j*8+k]=0;
	}
	//White Single - 0x00f81f00;
	bmi.fgcolor=0xffffff;
	pStockTextures[5]=::CreateDIBSection(NULL,
		(BITMAPINFO *)&bmi,DIB_RGB_COLORS,(void **)&bmpbitsl,NULL,0);
	for (j=0;j<32;j++)
	{
		for (k=0;k<2;k++) bmpbitsl[j*8+k]=0;
		bmpbitsl[j*8+2]=0x01010000;
		for (k=3;k<5;k++) bmpbitsl[j*8+k]=0x01010101;
		bmpbitsl[j*8+5]=0x00000101;
		for (k=6;k<8;k++) bmpbitsl[j*8+k]=0;
	}
	//Yellow Double - 0xff03c0ff
	bmi.fgcolor=0xffff00;
	pStockTextures[2]=::CreateDIBSection(NULL,
		(BITMAPINFO *)&bmi,DIB_RGB_COLORS,(void **)&bmpbitsl,NULL,0);
	for (j=0;j<32;j++)
	{
		for (k=0;k<2;k++) bmpbitsl[j*8+k]=0x01010101;
		bmpbitsl[j*8+2]=0x00010101;
		for (k=3;k<5;k++) bmpbitsl[j*8+k]=0;
		bmpbitsl[j*8+5]=0x01010100;
		for (k=6;k<8;k++) bmpbitsl[j*8+k]=0x01010101;
	}
	//White Double - 0xff03c0ff
	bmi.fgcolor=0xffffff;
	pStockTextures[7]=::CreateDIBSection(NULL,
		(BITMAPINFO *)&bmi,DIB_RGB_COLORS,(void **)&bmpbitsl,NULL,0);
	for (j=0;j<32;j++)
	{
		for (k=0;k<2;k++) bmpbitsl[j*8+k]=0x01010101;
		bmpbitsl[j*8+2]=0x00010101;
		for (k=3;k<5;k++) bmpbitsl[j*8+k]=0;
		bmpbitsl[j*8+5]=0x01010100;
		for (k=6;k<8;k++) bmpbitsl[j*8+k]=0x01010101;
	}
	//Yellow Line-Dash - 0x0000c0ff
	bmi.fgcolor=0xffff00;
	pStockTextures[4]=::CreateDIBSection(NULL,
		(BITMAPINFO *)&bmi,DIB_RGB_COLORS,(void **)&bmpbitsl,NULL,0);
	for (j=0;j<16;j++)
	{
		for (k=0;k<2;k++) bmpbitsl[j*8+k]=0x01010101;
		bmpbitsl[j*8+2]=0x00010101;
		for (k=3;k<8;k++) bmpbitsl[j*8+k]=0;

	}
	for (j=16;j<32;j++)
	{
		for (k=0;k<2;k++) bmpbitsl[j*8+k]=0x01010101;
		bmpbitsl[j*8+2]=0x00010101;
		for (k=3;k<5;k++) bmpbitsl[j*8+k]=0;
		bmpbitsl[j*8+5]=0x01010100;
		for (k=6;k<8;k++) bmpbitsl[j*8+k]=0x01010101;
	}
	//White Line-Dash - 0x0000c0ff
	bmi.fgcolor=0xffffff;
	pStockTextures[9]=::CreateDIBSection(NULL,
		(BITMAPINFO *)&bmi,DIB_RGB_COLORS,(void **)&bmpbitsl,NULL,0);
	for (j=0;j<16;j++)
	{
		for (k=0;k<2;k++) bmpbitsl[j*8+k]=0x01010101;
		bmpbitsl[j*8+2]=0x00010101;
		for (k=3;k<8;k++) bmpbitsl[j*8+k]=0;

	}
	for (j=16;j<32;j++)
	{
		for (k=0;k<2;k++) bmpbitsl[j*8+k]=0x01010101;
		bmpbitsl[j*8+2]=0x00010101;
		for (k=3;k<5;k++) bmpbitsl[j*8+k]=0;
		bmpbitsl[j*8+5]=0x01010100;
		for (k=6;k<8;k++) bmpbitsl[j*8+k]=0x01010101;
	}
	//Yellow Dash-Line - 0xff030000
	bmi.fgcolor=0xffff00;
	pStockTextures[3]=::CreateDIBSection(NULL,
		(BITMAPINFO *)&bmi,DIB_RGB_COLORS,(void **)&bmpbitsl,NULL,0);
	for (j=0;j<16;j++)
	{
		for (k=0;k<5;k++) bmpbitsl[j*8+k]=0;
		bmpbitsl[j*8+5]=0x01010100;
		for (k=6;k<8;k++) bmpbitsl[j*8+k]=0x01010101;
	}
	for (j=16;j<32;j++)
	{
		for (k=0;k<2;k++) bmpbitsl[j*8+k]=0x01010101;
		bmpbitsl[j*8+2]=0x00010101;
		for (k=3;k<5;k++) bmpbitsl[j*8+k]=0;
		bmpbitsl[j*8+5]=0x01010100;
		for (k=6;k<8;k++) bmpbitsl[j*8+k]=0x01010101;
	}
	//White Dash-Line - 0xff030000
	bmi.fgcolor=0xffffff;
	pStockTextures[8]=::CreateDIBSection(NULL,
		(BITMAPINFO *)&bmi,DIB_RGB_COLORS,(void **)&bmpbitsl,NULL,0);
	for (j=0;j<16;j++)
	{
		for (k=0;k<5;k++) bmpbitsl[j*8+k]=0;
		bmpbitsl[j*8+5]=0x01010100;
		for (k=6;k<8;k++) bmpbitsl[j*8+k]=0x01010101;
	}
	for (j=16;j<32;j++)
	{
		for (k=0;k<2;k++) bmpbitsl[j*8+k]=0x01010101;
		bmpbitsl[j*8+2]=0x00010101;
		for (k=3;k<5;k++) bmpbitsl[j*8+k]=0;
		bmpbitsl[j*8+5]=0x01010100;
		for (k=6;k<8;k++) bmpbitsl[j*8+k]=0x01010101;
	}
	//Black Nothing
	bmi.fgcolor=0x111111;
	pStockTextures[10]=::CreateDIBSection(NULL,
		(BITMAPINFO *)&bmi,DIB_RGB_COLORS,(void **)&bmpbitsl,NULL,0);
	for (j=0;j<32;j++) for (k=0;k<8;k++) bmpbitsl[j*8+k]=0;
	//Yellow Dash
	bmi.fgcolor=0xffff00;
	pStockTextures[1]=::CreateDIBSection(NULL,
		(BITMAPINFO *)&bmi,DIB_RGB_COLORS,(void **)&bmpbitsl,NULL,0);
	for (j=0;j<16;j++)
	{
		for (k=0;k<2;k++) bmpbitsl[j*8+k]=0;
		bmpbitsl[j*8+2]=0x01010000;
		for (k=3;k<5;k++) bmpbitsl[j*8+k]=0x01010101;
		bmpbitsl[j*8+5]=0x00000101;
		for (k=6;k<8;k++) bmpbitsl[j*8+k]=0;
	}
	for (j=16;j<32;j++) for (k=0;k<8;k++) bmpbitsl[j*8+k]=0;
	//White Dash
	bmi.fgcolor=0xffffff;
	pStockTextures[6]=::CreateDIBSection(NULL,
		(BITMAPINFO *)&bmi,DIB_RGB_COLORS,(void **)&bmpbitsl,NULL,0);
	for (j=0;j<16;j++)
	{
		for (k=0;k<2;k++) bmpbitsl[j*8+k]=0;
		bmpbitsl[j*8+2]=0x01010000;
		for (k=3;k<5;k++) bmpbitsl[j*8+k]=0x01010101;
		bmpbitsl[j*8+5]=0x00000101;
		for (k=6;k<8;k++) bmpbitsl[j*8+k]=0;
	}
	for (j=16;j<32;j++) for (k=0;k<8;k++) bmpbitsl[j*8+k]=0;
	//HOO: Will be moved to TrackView.cpp (4)
}

BEGIN_MESSAGE_MAP(CQFSView, CDialog)
	//{{AFX_MSG_MAP(CQFSView)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	ON_BN_CLICKED(IDC_FILEOPEN, OnFileopen)
	ON_BN_CLICKED(IDC_HSPROPS, OnHsprops)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQFSView message handlers

void CQFSView::OnChangeEdit1() 
{
	BOOL isok;
	int res;

	if (!::IsWindow(m_bmpCtrl.m_hWnd)) return;
	res=GetDlgItemInt(IDC_EDIT1,&isok,TRUE);
	if (!isok||(res<0)||(res>=nColEntries)) {
		m_bmpCtrl.SetBitmap(NULL);
		return;
	}
	m_nBmp=res;
	if (col[m_nBmp].islane) {
		if (col[m_nBmp].texture>=14)
			 m_bmpCtrl.SetBitmap(NULL);
		else m_bmpCtrl.SetBitmap(pStockBitmaps[col[m_nBmp].texture]);
	} else
	if ((pBitmaps==NULL)||(col[m_nBmp].texture>=nQfsEntries)||
		(pBitmaps[col[m_nBmp].texture]==NULL)) 
	{
		m_bmpCtrl.SetBitmap(NULL);
	} else {
		m_bmpCtrl.SetBitmap(pBitmaps[col[m_nBmp].texture]);
	}
}

void CQFSView::OnFileopen() 
{
	m_bmpCtrl.SetBitmap(NULL);
	Reset();
	switch (LoadQFS()) {
	case QFS_CANTOPEN:
		AfxMessageBox("Error loading QFS file.\nTextures will not be displayed.");
		break;
	case QFS_BADFORMAT:
		AfxMessageBox("QFS file format error.\nTextures will not be displayed.");
		Reset();
		break;
	case QFS_TOOLARGE:
		AfxMessageBox("QFS file too large.\nTextures will not be displayed.");
		Reset();
		break;
	}
	OnChangeEdit1();
}


/////////////////////////////////////////////////////////////////////////////
// CQFSLoadProgress dialog


CQFSLoadProgress::CQFSLoadProgress(CWnd* pParent /*=NULL*/)
	: CDialog(CQFSLoadProgress::IDD, pParent)
{
	//{{AFX_DATA_INIT(CQFSLoadProgress)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CQFSLoadProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQFSLoadProgress)
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CQFSLoadProgress, CDialog)
	//{{AFX_MSG_MAP(CQFSLoadProgress)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQFSLoadProgress message handlers


void CQFSView::OnHsprops() 
{
	CTextureProps propsDlg;

	if (!bHSMode) return;
	propsDlg.m_rotation=(m_flags>>2)&3;
	propsDlg.m_tiling=(m_flags>>6)&7;
	//Nappe1: Setting up dialog box options from AnimData (START)
	__int8 quickie=texanimdata;
	if  (quickie!=0) 
		{ 
		
		propsDlg.m_animated=true;
		propsDlg.m_animlenght=(texanimdata&0x7);
		propsDlg.m_animduration=(texanimdata&0xF8)>>3;
		}
	else 
		{	
		propsDlg.m_animated=false;
		propsDlg.m_animlenght=0;
		propsDlg.m_animduration=0;
		}
	//Nappe1: (END)
	if (propsDlg.m_tiling>4) propsDlg.m_tiling=-1;
	propsDlg.m_fliph=(m_flags&0x10)?TRUE:FALSE;
	propsDlg.m_flipv=(m_flags&0x20)?TRUE:FALSE;
	propsDlg.m_twosided=(m_flags&0x8000)?TRUE:FALSE;
	

	if (propsDlg.DoModal()!=IDOK) return;
	bHasHitFlags=TRUE;
	if (propsDlg.m_rotation<0) propsDlg.m_rotation=0;
	if (propsDlg.m_tiling<0) propsDlg.m_tiling=0;
	m_flags=(propsDlg.m_rotation<<2)+(propsDlg.m_tiling<<6)
		+(propsDlg.m_fliph?0x10:0)+(propsDlg.m_flipv?0x20:0)
		+(propsDlg.m_twosided?0x8000:0);
	//Nappe1: Updating texture animation info (START)
	if (propsDlg.m_animated==false) texanimdata=0;
	else 
		{
		 texanimdata= propsDlg.m_animlenght+(propsDlg.m_animduration<<3);
		}
	//Nappe1: (END)
}

