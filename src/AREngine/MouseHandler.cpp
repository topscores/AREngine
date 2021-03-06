#include "arengine/MouseHandler.h"
#include "arengine/SmartSingleton.h"
#include "arengine/Image.h"
#include "arengine/ARScene.h"
#include "arengine/ARRoot.h"
#include "arengine/HUDRoot.h"

#include<osg/Vec2d>

using namespace osg;

using namespace arengine;


MouseHandler::MouseHandler()
{
}


bool 
MouseHandler::handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa) 
{
	// Check for MouseOver whenever we receive mouse related event
	resetMouseOver();
	string objName;
	vector<string> nameList;
	nameList = check2DIntersection(ea.getXnormalized(), ea.getYnormalized());
	for (int i = 0;i < nameList.size();i++)
	{
		setMouseOver(nameList[i]);
	}

	objName = "";
	objName = check3DIntersection(ea.getXnormalized(), ea.getYnormalized());
	if (objName != "")
	{
		setMouseOver(objName);
	}

	switch (ea.getEventType()) 
	{

		/** HANDLE MOUSE EVENTS:
		Mouse events have associated event names, and mouse
		position co-ordinates. **/

	case osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON: 
		{
			nameList = check2DIntersection(ea.getXnormalized(), ea.getYnormalized());
			for (int i = 0;i < nameList.size();i++)
			{
				setMouseDown(nameList[i]);
			}

			objName = "";
			objName = check3DIntersection(ea.getXnormalized(), ea.getYnormalized());
			if (objName != "")
			{
				setMouseDown(objName);
			}
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


bool
MouseHandler::isMouseOver(string objname)
{
	return m_isOver[objname];
}


vector<string>
MouseHandler::check2DIntersection(double mx, double my)
{
	vector<string> nameList;

	mx = ((mx - (-1)) / 2.0) * 800;
	my = ((my - (-1)) / 2.0) * 600;

	vector< ref_ptr<SceneObj> > hudlist;
	ref_ptr<ARScene> arscene = SmartSingleton<ARScene>::getInstance();

	if (arscene.valid())
	{
		ARRoot* arroot = dynamic_cast<ARRoot*>(arscene->getSceneData().get());
		if (arroot)
		{
			ref_ptr<Scene> activescene = arroot->getScene(arroot->getActiveSceneIdx());
			if (activescene.valid())
			{
				ref_ptr<HUDRoot> hudroot = activescene->getHUDRoot();
				hudlist = hudroot->getHUDList();
			}
		}
	}

	if (hudlist.size() != 0)
	{
		int n = hudlist.size();
		for (int i = 0;i < n;i++)
		{
			Image *img = dynamic_cast<Image*>(hudlist[i].get());

			if (img)
			{
				Vec2d pos = img->getPosition();
				Vec2d size= img->getSize();

				// Intersect with img
				if ((mx >= pos.x() && mx < pos.x() + size.x()) 
					&& (my >= pos.y() && my <= pos.y() + size.y()))
				{
					string name = img->getName();
					// setMouseDown(name);
					nameList.push_back(name);
				}
			}
		}
	}

	return nameList;
}


string
MouseHandler::check3DIntersection(double mx, double my)
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
				// setMouseDown(name);
				return name;
			}
		}
	}
	return "";
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


void
MouseHandler::resetMouseOver()
{
	map<string, bool>::iterator itr;
	for (itr = m_isOver.begin();itr != m_isOver.end();itr++)
	{
		itr->second = false;
	}
}


void
MouseHandler::setMouseOver(string objName)
{
	map<string, bool>::iterator itr = m_isOver.find(objName);

	// m_isPick already contains an item represents object specified by name
	if (itr != m_isOver.end())
	{
		itr->second = true;
	}
	else
	{
		m_isOver.insert(pair<string, bool>(objName, true));
	}
}