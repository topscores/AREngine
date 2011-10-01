#include "arenginewx/CreateMemImage.h"
#include "arengine/Util.h"

#include <sstream>
#include <osgDB/WriteFile>

using namespace std;
using namespace osg;
using namespace arenginewx;


CreateMemImage::CreateMemImage(wxWindow *win, ProcessCaptureImageCallback *callback,
							   int width, int height)
:m_win(win),
m_width(width),
m_height(height),
m_callback(callback)
{
	wxInitAllImageHandlers();
}


void 
CreateMemImage::operator()(const osg::Image& image, const unsigned int context_id)
{
	if (m_win != NULL && m_callback != NULL)
	{
		int time = arengine::Util::getElapseTimeInMilliSec();
		stringstream sstr;
		sstr << "images/" <<  time << ".jpg";
		wxString fileName = wxT("images/") + wxString::Format(wxT("%i"), time) + wxT(".jpg");

		// resize image if needed
		if (m_width > 0 && m_height > 0)
		{
			ref_ptr<osg::Image> resizeImage = (osg::Image*) image.clone(osg::CopyOp::SHALLOW_COPY);
			resizeImage->scaleImage(m_width, m_height, 1);
			osgDB::writeImageFile(*resizeImage.get(), sstr.str().c_str());
		}
		else
		{
			osgDB::writeImageFile(image, sstr.str().c_str());
		}

		wxImage wxImg;
		wxImg.LoadFile(fileName, wxBITMAP_TYPE_JPEG);
		
		//img->flipVertical();
		//wxImg.SetData(img->data());

		m_callback->OnCaptureFinish(wxImg, fileName);
	}
}