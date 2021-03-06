
// OpenCVPlatView.cpp : COpenCVPlatView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "OpenCVPlat.h"
#endif

#include "OpenCVPlatDoc.h"
#include "OpenCVPlatView.h"
#include "DialogVignette.h"
#include "MedianFilter.h"
#include "GaussianBlur.h"
#include "ColorBalance.h"
#include "Saturation.h"
#include "Sharpen.h"
#include "Liquify.h"
#include "Skin.h"
#include "Lomo.h"
#include "Inpainting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COpenCVPlatView

IMPLEMENT_DYNCREATE(COpenCVPlatView, CView)

BEGIN_MESSAGE_MAP(COpenCVPlatView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_VIGNETTE, &COpenCVPlatView::OnVignette)
	ON_COMMAND(ID_MEDIAN_FILTER, &COpenCVPlatView::OnMedianFilter)
	ON_COMMAND(ID_GAUSSIAN_BLUR, &COpenCVPlatView::OnGaussianBlur)
	ON_COMMAND(ID_COLOR_BALANCE, &COpenCVPlatView::OnColorBalance)
	ON_COMMAND(ID_SATURATION, &COpenCVPlatView::OnSaturation)
	ON_COMMAND(ID_SHARPEN, &COpenCVPlatView::OnSharpen)
	ON_COMMAND(ID_LIQUIFY, &COpenCVPlatView::OnLiquify)
	ON_COMMAND(ID_SKIN, &COpenCVPlatView::OnSkin)
	ON_COMMAND(ID_LOMO, &COpenCVPlatView::OnLomo)
	ON_COMMAND(ID_IMAGE_INPAINT, &COpenCVPlatView::OnImageInpaint)
END_MESSAGE_MAP()

// COpenCVPlatView 构造/析构

COpenCVPlatView::COpenCVPlatView()
{
	// TODO:  在此处添加构造代码
	start_inpaint = false;
}

COpenCVPlatView::~COpenCVPlatView()
{
}

BOOL COpenCVPlatView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// COpenCVPlatView 绘制

void COpenCVPlatView::OnDraw(CDC* pDC)
{
	COpenCVPlatDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO:  在此处为本机数据添加绘制代码
	cv::Mat &image = pDoc->image;
	if (image.cols > 0 && image.rows > 0)
	{
		RECT rect;
		GetClientRect(&rect);
		cv::Mat image_show(abs(rect.top - rect.bottom), abs(rect.right - rect.left), CV_8UC3);
		resize(image, image_show, image_show.size());

		ATL::CImage CI;
		int w = image_show.cols;//宽  
		int h = image_show.rows;//高  
		int channels = image_show.channels();//通道数  

		CI.Create(w, h, 8 * channels);
		if (channels == 1)
		{
			RGBQUAD* ColorTable;
			int MaxColors = 256;
			ColorTable = new RGBQUAD[MaxColors];
			CI.GetColorTable(0, MaxColors, ColorTable);
			for (int i = 0; i<MaxColors; i++)
			{
				ColorTable[i].rgbBlue = (BYTE)i;
				ColorTable[i].rgbGreen = (BYTE)i;
				ColorTable[i].rgbRed = (BYTE)i;
			}
			CI.SetColorTable(0, MaxColors, ColorTable);
			delete[]ColorTable;
		}

		uchar *pS;
		uchar *pImg = (uchar *)CI.GetBits(); 
		int step = CI.GetPitch();
		for (int i = 0; i < h; i++)
		{
			pS = image_show.ptr<uchar>(i);
			memcpy(pImg + i*step, pS, sizeof(uchar) * channels * w);
		}
		
		CI.Draw(*pDC, 0, 0);
		ReleaseDC(pDC);
		CI.Destroy();
	}
	if (start_inpaint)
	{
		inpaint();
	}
}


// COpenCVPlatView 打印

BOOL COpenCVPlatView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void COpenCVPlatView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  添加额外的打印前进行的初始化过程
}

void COpenCVPlatView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  添加打印后进行的清理过程
}


// COpenCVPlatView 诊断

#ifdef _DEBUG
void COpenCVPlatView::AssertValid() const
{
	CView::AssertValid();
}

void COpenCVPlatView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COpenCVPlatDoc* COpenCVPlatView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COpenCVPlatDoc)));
	return (COpenCVPlatDoc*)m_pDocument;
}
#endif //_DEBUG


// COpenCVPlatView 消息处理程序


BOOL COpenCVPlatView::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return CView::OnEraseBkgnd(pDC);
}


void COpenCVPlatView::OnVignette()
{
	// TODO:  在此添加命令处理程序代码
	CDialogVignette dialog_vignette;
	dialog_vignette.DoModal();
}


void COpenCVPlatView::OnMedianFilter()
{
	// TODO:  在此添加命令处理程序代码
	MedianFilter dialog_median_filter;
	dialog_median_filter.DoModal();
}


void COpenCVPlatView::OnGaussianBlur()
{
	// TODO:  在此添加命令处理程序代码
	CGaussianBlur dialog_gaussian_blur;
	dialog_gaussian_blur.DoModal();
}


void COpenCVPlatView::OnColorBalance()
{
	// TODO:  在此添加命令处理程序代码
	CColorBalance dialog_color_balance;
	dialog_color_balance.DoModal();
}


void COpenCVPlatView::OnSaturation()
{
	// TODO:  在此添加命令处理程序代码
	CSaturation dialog_saturation;
	dialog_saturation.DoModal();
}


void COpenCVPlatView::OnSharpen()
{
	// TODO:  在此添加命令处理程序代码
	CSharpen dialog_sharpen;
	dialog_sharpen.DoModal();
}


void COpenCVPlatView::OnLiquify()
{
	// TODO:  在此添加命令处理程序代码
	CLiquify dialog_liquify;
	dialog_liquify.DoModal();
}


void COpenCVPlatView::OnSkin()
{
	// TODO:  在此添加命令处理程序代码
	CSkin dialog_skin;
	dialog_skin.DoModal();
}


void COpenCVPlatView::OnLomo()
{
	// TODO:  在此添加命令处理程序代码
	CLomo dialog_lomo;
	dialog_lomo.DoModal();
}


void COpenCVPlatView::OnImageInpaint()
{
	// TODO:  在此添加命令处理程序代码
	COpenCVPlatDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	cv::Mat &image = pDoc->image;
	dst = image.clone();
	start_inpaint = true;
	Invalidate();
}

void COpenCVPlatView::inpaint()
{
	COpenCVPlatDoc* pDoc = GetDocument();
	cv::Mat &image = pDoc->image;
	int psize = 23;

	if (contains_hole(image))
	{
		initialize_hole_NNF(nnf, image, dst, psize, last_nnf);
		iterate_hole_NNF(nnf, psize, image, dst);
		reconstruct_from_hole_NNF_no_avg(nnf, image, dst, psize);
		last_nnf.release();
		last_nnf = nnf.clone();
	}
	else
	{
		initialize_hole_NNF(nnf, image, dst, psize, last_nnf);
		iterate_hole_NNF(nnf, psize, image, dst);
		reconstruct_from_hole_NNF(nnf, image, dst, 5);
		dst.release();
		nnf.release();
		last_nnf.release();

		start_inpaint = false;
	}
	Invalidate();
}


void pyramid_method(cv::Mat &image, int psize)
{
	cv::Mat srces[5];
	cv::Mat nnfs[5];
	cv::Mat old[5];
	cv::Mat temp;

	srces[4] = image.clone();
	for (int i = 4; i >= 1; i--)
		sample_down(srces[i], srces[i - 1]);
	for (int i = 0; i < 5; i++)
		old[i] = srces[i].clone();

	optimize_start(srces[0], 5);

	cv::resize(srces[0], temp, cv::Size(srces[1].cols, srces[1].rows));
	fill_image_with_image(srces[1], temp);

	for (int i = 1; i <= 3; i++)
	{
		initialize_NNF(nnfs[i], srces[i].cols, srces[i].rows);
		iterate_NNF(nnfs[i], psize, srces[i], old[i]);
		reconstruct_from_NNF_no_avg(nnfs[i], srces[i], old[i]);
		cv::resize(srces[i], temp, cv::Size(srces[i + 1].cols, srces[i + 1].rows));
		fill_image_with_image(srces[i + 1], temp);
	}

	initialize_NNF(nnfs[4], srces[4].cols, srces[4].rows);
	iterate_NNF(nnfs[4], psize, srces[4], old[4]);
	reconstruct_from_NNF_no_avg(nnfs[4], srces[4], old[4]);

	image.release();
	image = srces[4].clone();
	temp.release();
	for (int i = 0; i < 5; i++)
	{
		nnfs[i].release();
		srces[i].release();
		old[i].release();
	}
}


void optimize_start(cv::Mat &image, int psize)
{
	cv::Mat nnf, last_nnf;
	cv::Mat dst = image.clone();

	while (contains_hole(image))
	{
		initialize_hole_NNF(nnf, image, dst, psize, last_nnf);
		iterate_hole_NNF(nnf, psize, image, dst);
		reconstruct_from_hole_NNF_no_avg(nnf, image, dst, psize);
		last_nnf.release();
		last_nnf = nnf.clone();
	}

	initialize_hole_NNF(nnf, image, dst, psize, last_nnf);
	iterate_hole_NNF(nnf, psize, image, dst);
	reconstruct_from_hole_NNF(nnf, image, dst, 3);

	dst.release();
	nnf.release();
	last_nnf.release();
}