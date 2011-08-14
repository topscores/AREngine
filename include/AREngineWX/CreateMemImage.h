#ifndef __CREATE_WXIMAGE_H__
#define __CREATE_WXIMAGE_H__

#include "wx/window.h"
#include "wx/image.h"
#include "arenginewx/Export"

#include <osg/Image>
#include <osgViewer/ViewerEventHandlers>
using namespace osgViewer;

namespace arenginewx
{
	class ARENGINEWX_EXPORT ProcessCaptureImageCallback 
	{
	public:
		virtual void OnCaptureFinish(wxImage &image, wxString imgName) = 0;
	};

	class ARENGINEWX_EXPORT CreateMemImage : public osgViewer::ScreenCaptureHandler::CaptureOperation
	{
	public:
		CreateMemImage(wxWindow *win, ProcessCaptureImageCallback *callback, int width=-1, int height=-1);
		virtual void operator()(const osg::Image& image, const unsigned int context_id);

	private:
		wxWindow *m_win;
		int m_width, m_height;
		ProcessCaptureImageCallback *m_callback;
	};

}

#endif