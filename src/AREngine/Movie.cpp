#include "arengine/Movie.h"
#include "arengine/Util.h"

#include <osgDB/ReadFile>
#include <osg/TextureRectangle>
#include <osg/Texture2D>

using namespace arengine;

MovieNode::MovieNode(string filename)
{
	osg::Image* img = osgDB::readImageFile(filename);
	mImageStream = dynamic_cast<osg::ImageStream*>(img);

	if (mImageStream.valid()) {
		Util::log(__FUNCTION__, 3, "Successfully load movie %s ", filename.c_str());
		//mImageStream->play();

		float aspectRatio = (float)img->t() / (float)img->s();
		float videoWidth = 180;
		float videoHeight = videoWidth * aspectRatio;

		mVideoGeode = new osg::Geode();

		bool texRect = true;

		if (texRect) {
			mVideoTexture = new osg::TextureRectangle(img);
			mVideoGeode->addDrawable(osg::createTexturedQuadGeometry(
				osg::Vec3(-videoWidth / 2, -videoHeight / 2, 0), 
				osg::Vec3(videoWidth, 0, 0), 
				osg::Vec3(0, videoHeight, 0), 
				0,  img->t(), img->s(), 0));

		} else {
			mVideoTexture = new osg::Texture2D(img);
			mVideoGeode->addDrawable(osg::createTexturedQuadGeometry(
				osg::Vec3(-videoWidth / 2, -videoHeight / 2, 0), 
				osg::Vec3(videoWidth, 0, 0), 
				osg::Vec3(0, videoHeight, 0), 
				0,  1,1, 0));


		}

		mVideoGeode->getOrCreateStateSet()->setTextureAttributeAndModes(0, mVideoTexture.get());	
		//mVideoGeode->getOrCreateStateSet()->setRenderBinDetails(150, "RenderBin");
		this->addChild(mVideoGeode.get());

	} else {
		Util::log(__FUNCTION__, 2, "Cannot load movie %s ", filename.c_str());
	}
}



void 
MovieNode::start()
{
	if (mImageStream.valid())
	{
		mImageStream->rewind();
		mImageStream->play();
		//mImageStream->seek(0.0);
		//if (mImageStream->getStatus() == osg::ImageStream::PAUSED)
		//{
		//	mImageStream->play();
		//}
	}

}

void
MovieNode::stop()
{
	if (mImageStream.valid())
	{
		mImageStream->rewind();
		//if (mImageStream->getStatus() == osg::ImageStream::PLAYING)
		//{
			mImageStream->pause();
		//}
	}
}


void
MovieNode::resume()
{
	if (mImageStream->valid()){
		//if (mImageStream->getStatus() == osg::ImageStream::PAUSED) 
		//{
			mImageStream->play();
		//}
	}
}


void 
MovieNode::pause() 
{
	if (mImageStream.valid()) 
	{
		if (mImageStream->getStatus() == osg::ImageStream::PLAYING) 
		{
			mImageStream->pause();
		}
	}
}

void
MovieNode::setLoopMode(bool loop)
{
	if (loop)
	{
		mImageStream->setLoopingMode(osg::ImageStream::LOOPING);
	}
	else
	{
		mImageStream->setLoopingMode(osg::ImageStream::NO_LOOPING);
	}
}


Movie::Movie(DataNode *movieNode)
{
	osg::Matrix transMat;
	osg::Matrix rotMat;
	osg::Matrix scaleMat;
	try
	{
		if (movieNode != NULL)
		{
			if (movieNode->getNodeName().compare("Movie") != 0)
			{
				throw Exception("Movie::Movie() : Not a Movie tag", 2);
			}

			if (!movieNode->getAttributeAsString("name").empty())
			{
				m_objName	= movieNode->getAttributeAsString("name");
			}
			else
			{
				throw Exception("Movie::Movie() : Name field cannot be empty", 2);
			}

			if (!movieNode->getAttributeAsString("fileName").empty())
			{
				m_fileName	= movieNode->getAttributeAsPath("fileName");
				m_orgNode   = new MovieNode(m_fileName);

				m_orgTranslation[0] = movieNode->getAttributeAsDouble("transX");
				m_orgTranslation[1] = movieNode->getAttributeAsDouble("transY");
				m_orgTranslation[2] = movieNode->getAttributeAsDouble("transZ");
				translateTo(m_orgTranslation);

				m_orgRotation[0] = movieNode->getAttributeAsDouble("rotX");
				m_orgRotation[1] = movieNode->getAttributeAsDouble("rotY");
				m_orgRotation[2] = movieNode->getAttributeAsDouble("rotZ");
				rotateTo(m_orgRotation);

				m_orgSize = movieNode->getAttributeAsDouble("size");

				// Calculate bounding box and size
				setScaleFromSize(m_orgSize);
				scaleTo(m_scale);

				m_unitTransform = new osg::MatrixTransform();
				osg::Matrix modelTranslate;
				modelTranslate.makeTranslate(-m_bs.center());
				m_unitTransform->postMult(modelTranslate);
				m_unitTransform->addChild(m_orgNode);

				this->addChild(m_unitTransform);
			}
			else
			{
				throw Exception("Movie::Movie() : FileName field cannot be empty", 2);
			}

			bool loop = movieNode->getAttributeAsBool("loop");
			((MovieNode*)m_orgNode.get())->setLoopMode(loop);
		}
		else
		{
			throw Exception("Movie::Movie() : Cannot create Movie object from NULL pointer", 2);
		}
	}
	catch (Exception err)
	{
		Util::log(err);
	}
}

void
Movie::setScaleFromSize(double size)
{
	if (m_orgNode.valid())
	{
		m_size = size;
		if (m_size == 0)
		{
			m_size = 50;
		}
		m_bs = m_orgNode->getBound();
		m_scale = m_size/m_bs.radius();
	}
}


void
Movie::start()
{
	((MovieNode*)m_orgNode.get())->start();
}


void
Movie::stop()
{
	((MovieNode*)m_orgNode.get())->stop();
}


void
Movie::resume()
{
	((MovieNode*)m_orgNode.get())->resume();
}


void
Movie::pause()
{
	((MovieNode*)m_orgNode.get())->pause();
}



