#pragma once
#include <opencv2\opencv.hpp>

// CSharpen 对话框

class CSharpen : public CDialogEx
{
	DECLARE_DYNAMIC(CSharpen)

public:
	CSharpen(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSharpen();

// 对话框数据
	enum { IDD = IDD_SHARPEN };
	cv::Mat old_img;
	cv::Mat sharpened_img;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRoberts();
	afx_msg void OnBnClickedPrewitt();
	afx_msg void OnBnClickedSobel();
	afx_msg void OnBnClickedLaplace3x3();
	afx_msg void OnBnClickedCompositeLaplacianMask5();
	afx_msg void OnBnClickedCompositeLaplacianMask9();

	void scroll();
};
