#include "arengine/MouseHandler.h"
#include "arengine/SmartSingleton.h"
#include "arengine/ObjPool.h"
#include "arengine/Image.h"

#include<osg/Vec2d>

using namespace osg;

using namespace arengine;


MouseHandler::MouseHandler()
{
}


bool 
MouseHandler::handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa) 
{
	switch (ea.getEventType()) 
	{


		/** HANDLE MOUSE EVENTS:
		Mouse events have associated event names, and mouse
		position co-ordinates. **/
	case osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON: 
		{
			check2DIntersection(ea.getXnormalized(), ea.getYnormalized());
			check3DIntersection(ea.getXnormalized(), ea.getYnormalized());
			return true;
		}

	case osgGA::GUIEventAdapter::RELEASE:
		{

			map<string, bool>::iterator itr;
			for (itr = m_isPick.begin();itr != m_isPick.end();itr++)
			{
				itr->second = false;
			}
			return true;
		}

	default: return false;
	} 
} 


bool
MouseHandler::isMouseDown(string objname)
{
	return m_isPick[objname];
}


void
MouseHandler::check2DIntersection(int mx, int my)
{
	SceneObjPool *pool = Singleton<SceneObjPool>::getInstance();
	int n = pool->size();
	for (int i = 0;i < n;i++)
	{
		ref_ptr<arengine::Image> img = dynamic_cast<arengine::Image*>(pool->at(i).get());
		if (img)
		{
			Vec2d pos = img->getPosition();
			Vec2d size= img->getSize();

			// Intersect with img
			if ((mx >= pos.x() && mx < pos.x() + size.x()) 
				&& (my >= pos.y() && my <= pos.y()))
			{
				string name = img->getName();
				setMouseDown(name);
			}
		}
	}
}


void
MouseHandler::check3DIntersection(int mx, int my)
{


	/** PERFORM SOME ACTION:
	Once we have received the mouse event, we can
	perform some action. Here we perform mouse picking.

	Mouse picking can be thought of as shooting a "ray"
	from the mouse position into our scene, and determining
	which parts of the scene intersect with the ray.
	Typically we will only be interested in the first false
	(if any) object that the ray hits. We can then perform
	some action.

	In this example, we are going to interact with our
	car model - if the mouse has "picked" it, we will
	start a simple rotation on it. **/



	/** PERFORM MOUSE PICKING:
	In OSG, mouse picking can be done in a few slightly
	different ways, however, the OSG documentation recommends
	using the following steps: **/


	/** 1. Create either a PolytopeIntersector, or a LineIntersector
	using the normalized mouse co-ordinates.**/
	osg::ref_ptr<osgUtil::LineSegmentIntersector> lI = new
		osgUtil::LineSegmentIntersector(osgUtil::Intersector::PROJECTION,
		mx, my);

	/** 2. Create an IntersectionVisitor, passing the
	Intersector as parameter to the constructor. **/
	osgUtil::IntersectionVisitor iV(lI);

	/** 3. Launch the IntersectionVisitor on the root node of
	the scene graph. In an OSGART application, we can
	launch it on the osgART::Scene node. **/
	ref_ptr<ARScene> arscene = SmartSingleton<ARScene>::getInstance();
	arscene->getSceneData()->accept(iV);

	/** 4. If the Intersector contains any intersections
	obtain the NodePath and search it to find the
	Node of interest. **/

	if (lI->containsIntersections()) 
	{
		osg::NodePath nP = lI->getFirstIntersection().nodePath;
		int n = nP.size();
		for (int i = 0;i < n;i++)
		{
			SceneObj *node = dynamic_cast<SceneObj*>(nP[i]);
			if (node != NULL)
			{
				string name = node->getName();
				setMouseDown(name);
			}
		}
	}
}


void
MouseHandler::setMouseDown(string objName)
{
	map<string, bool>::iterator itr = m_isPick.find(objName);

	// m_isPick already contains an item represents object specified by name
	if (itr != m_isPick.end())
	{
		itr->second = true;
	}
	else
	{
		m_isPick.insert(pair<string, bool>(objName, true));
	}
}