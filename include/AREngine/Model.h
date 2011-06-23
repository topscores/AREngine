#ifndef __MODEL_H__
#define __MODEL_H__

#include "arengine/Export"
#include "arengine/DataNode.h"
#include "arengine/SceneObj.h"

#include <osg/Vec3d>
#include <osg/MatrixTransform>
#include <osg/Node>

#include <string>
#include <vector>

using namespace std;

namespace arengine
{

	class ARENGINE_EXPORT Model : public SceneObj
	{
	public:
		Model(DataNode *modelNode);
		~Model();

		virtual string getObjName(){return m_objName;}
		virtual EObjType getObjType(){return E_MODEL;}

		void	compileDisplayList();

		string	getFileName();

		void	setSize(double size);
		double	getSize();

	private:
		string		m_objName;
		string		m_fileName;
		double		m_size;

		int			m_id;
		vector<int> m_associatedMarkerIdList;

		osg::Vec3d m_orgTranslation;
		osg::Vec3d m_orgRotation;
		double	   m_orgSize;

		ref_ptr<MatrixTransform> m_unitTransform;
		osg::BoundingSphere m_bs;

	};

}

#endif
