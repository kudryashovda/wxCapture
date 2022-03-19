#include <wx/wx.h>
#include <wx/timer.h>
#include <opencv2/opencv.hpp>
#include "convertmattowxbmp.h"

using namespace std;
using namespace cv;

constexpr int INTERNAL_CAM = 0;
constexpr int USB_CAM = 2;

class MyFrame: public wxFrame
{
	wxStaticBitmap *wx_static_bitmap;

	VideoCapture cap{ USB_CAM };
	Mat img_;
	wxBitmap wx_btm_img_;

	int timer_delay_ms_ = 40;
	wxTimer m_timer;
public:
	explicit MyFrame(const wxString &title);
	void OnTimer(wxTimerEvent &event);
};


MyFrame::MyFrame(const wxString &title): wxFrame(nullptr, wxID_ANY, title)
{
	cap.set(CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));

	cap.read(img_);
	constexpr int image_depth = 24;
	wx_btm_img_ = wxBitmap(img_.cols, img_.rows, image_depth);
	SetSize(wxSize(img_.cols, img_.rows));
	wx_static_bitmap = new wxStaticBitmap(this, wxID_ANY, wx_btm_img_);

	m_timer.Bind(wxEVT_TIMER, &MyFrame::OnTimer, this);
	m_timer.Start(timer_delay_ms_);
}

class MyApp: public wxApp
{
public:
	bool OnInit() override;
};

IMPLEMENT_APP(MyApp)
bool MyApp::OnInit()
{
	auto *frame = new MyFrame(wxT("wxCapture"));
	frame->Centre();
	frame->Show(true);

	return true;
}

void MyFrame::OnTimer(wxTimerEvent &event)
{
	m_timer.Stop();
	cap.read(img_);
	Mat img_blur, img_greyscale, img_out, img_canny;

	GaussianBlur(img_, img_blur, Size(3, 3), 3, 0);
	cvtColor(img_blur, img_greyscale, COLOR_BGR2GRAY);
	Canny(img_greyscale, img_canny, 50, 150);

	vector<vector<Point>> contours;
	vector<Vec4i> heirachy;
	findContours(img_canny, contours, heirachy, CV_RETR_TREE,
			CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	for (int i = 0; i < contours.size(); i++) {
		drawContours(img_, contours, i, Scalar(0, 255, 0), 1, 8,
				heirachy, 0, Point());
	}

	ConvertMatBitmapTowxBitmap(img_, wx_btm_img_);
	
	wx_static_bitmap->SetBitmap(wx_btm_img_);
	m_timer.Start(timer_delay_ms_);
}