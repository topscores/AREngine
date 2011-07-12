#include "arenginewx/CreateMemImage.h"
#include "arengine/Util.h"
#include <osgDB/WriteFile>
using namespace arenginewx;

CreateMemImage::CreateMemImage(wxWindow *win, ProcessCaptureImageCallback *callback)
:m_win(win),
m_callback(callback)
{
}


void 
CreateMemImage::operator()(const osg::Image& image, const unsigned int context_id)
{
	int w, h;
	if (m_win != NULL && m_callback != NULL)
	{
		int time = arengine::Util::getTimeInMilliSec();
		stringstream sstr;
		sstr << "images/" <<  time << ".jpg";
		wxString fileName = wxT("images/") + wxString::Format(wxT("%i"), time) + wxT(".jpg");
		osgDB::writeImageFile(image, sstr.str().c_str());
		wxImage wxImg;
		wxImg.LoadFile(fileName, wxBITMAP_TYPE_JPEG);
		
		//img->flipVertical();
		//wxImg.SetData(img->data());

		m_callback->OnCaptureFinish(wxImg, fileName);
	}
}