#include "smm.h"


Smm::Smm(const char* filename)
{
	strcpy(m_path, filename);
	m_file.open(filename, ios::in|ios::binary);

	//read head
	memset(&m_head, 0, sizeof(m_head));
	m_file.seekg(0);
	m_file.read((char *)&m_head, sizeof(m_head));

	//read vertices
	m_file.seekg(m_head.vertOffset);
	m_vertForRender.resize(m_head.vertNum);
	for(int i = 0; i < m_head.vertNum; i++)
		m_vertList.push_back(readSmmVert(i));
	cout << "read vert finish: " << hex << showbase << m_file.tellg() << endl;

	//read meshs	
	m_file.seekg(m_head.meshOffset);
	for(int i = 0; i < m_head.meshNum; i++)
		m_meshList.push_back(readSmmMesh(i));
	cout << "read mesh finish: " << hex << showbase << m_file.tellg() << endl;

	//read material
	m_file.seekg(m_head.mtlOffset);
	for(int i = 0; i < m_head.mtlNum; i++)
		m_mtlList.push_back(readMaterial(i));
	cout << "read mtl finish: " << hex << showbase << m_file.tellg() << endl;
	//read node
	m_file.seekg(m_head.nodeOffset);
	uint32 nodeNum;
	m_file.read((char *)&nodeNum, sizeof(nodeNum));
	if(nodeNum != m_head.nodeNum)
		cout << "[SMM]ERROR node num " << nodeNum << m_head.nodeNum << endl;
	for(int i = 0; i < m_head.nodeNum; i++)
		m_nodeList.push_back(readSmmNode(i));
	cout << "read node finish: " << hex << showbase << m_file.tellg() << endl;


}


Smm::~Smm()
{
	m_file.close();
}


void Smm::showHeadInfo()
{
	cout << "=================================================" << endl;
	cout << "[Head]" << m_path << endl;
    cout << "Lable      " << hex << showbase << m_head.Lable       << endl; 
    cout << "Version    " << dec << showbase << (int)m_head.Version     << endl; 
    cout << "vertType   " << dec << showbase << (int)m_head.vertType    << endl; 
    cout << "vertNum    " << dec << showbase << m_head.vertNum     << endl;  
    cout << "vertOffset " << hex << showbase << m_head.vertOffset  << endl; 
    cout << "meshNum    " << dec << showbase << m_head.meshNum     << endl; 
    cout << "meshOffset " << hex << showbase << m_head.meshOffset  << endl; 
    cout << "mtlNum     " << dec << showbase << m_head.mtlNum      << endl; 
    cout << "mtlOffset  " << hex << showbase << m_head.mtlOffset   << endl; 
    cout << "nodeNum     " << dec << showbase << m_head.nodeNum      << endl; 
    cout << "nodeOffset  " << hex << showbase << m_head.nodeOffset   << endl; 
	cout << "=================================================" << endl;

}


SmmVert Smm::readSmmVert(int i)
{

	SmmVert t;
	m_file.read((char *)&t.pos, sizeof(t.pos));
	m_file.read((char *)&t.normal, sizeof(t.normal));
	//m_file.read((char *)&t.tangent, sizeof(t.tangent));
	//m_file.read((char *)&t.blendWeight, sizeof(t.blendWeight));
	//m_file.read((char *)&t.blendIndices, sizeof(t.blendIndices));
	m_file.read((char *)&t.uv, sizeof(t.uv));
	
	return t;
}

SmmMesh Smm::readSmmMesh(int idx)
{
	SmmMesh m;
	m_file.read((char *)&m.baseIndex, sizeof(m.baseIndex));
	m_file.read((char *)&m.mtlId, sizeof(m.mtlId));
	m_file.read((char *)&m.vertNum, sizeof(m.vertNum));
	m_file.read((char *)&m.max, sizeof(m.max));
	m_file.read((char *)&m.min, sizeof(m.min));

	uint32 n;
	m_file.read((char *)&n, sizeof(n));
	for(int i = 0; i < n; i++)
	{
		SmmTri t;
		m_file.read((char *)&t, sizeof(t));
		m.baseFaces.push_back(t);
	}
	
	m_file.read((char *)&m.lodNum, sizeof(m.lodNum));
	for(int i = 0; i < 4; i++)
	{
		uint32 n;
		m_file.read((char *)&n, sizeof(n));
		for(int j = 0; j < n; j++)
		{
			SmmTri t;
			m_file.read((char *)&t, sizeof(t));
			m.lodFaces[i].push_back(t);
		}
		if(n > 0)
			cout << "[LLLL]" << m_path << " lodNum " << i << " " << n <<endl;

	}

	cout << "[Smm]readSmmMesh(" << idx << ") base:" << n << " lods: " << m.lodNum << endl;
	
	return m;
}

Material Smm::readMaterial(int i)
{
	cout << "[Smm]readMaterial(" << i << ")" << endl;
	Material m;
	m_file.read((char *)&m, sizeof(m));

 
	for(int t = 0; t < EMap_Max; t++)
	{
		if(strlen(m.map[t])>0 && t > 0)
			cout << "[TTTT]" << m_path << "map" << t << " "<<m.map[t] <<endl;
	}
	
	return m;
}


void Smm::showMaterialInfo()
{
	cout << "[MMMM]"<< m_path << endl;
	int cc = 0;
	for(int i = 0; i < m_mtlList.size(); i++)
	{
		for(int t = 0; t < EMap_Max; t++)
		{
			if(strlen(m_mtlList[i].map[t])>0)
				cc ++;
		}
	}
	cout << dec << "[MMMM]"<< cc << endl;

	for(int i = 0; i < m_mtlList.size(); i++)
	{
		for(int t = 0; t < EMap_Max; t++)
		{
			if(strlen(m_mtlList[i].map[t])>0)
				cout << "[MMMM]" << m_mtlList[i].map[t] <<endl;
		}
	}
}

SmmNode Smm::readSmmNode(int i)
{
	SmmNode t;
	m_file.read((char *)&t, sizeof(t));
	return t;
}




void Smm::initWorldSpace()
{
	for(int i = 0; i < m_worlds.size(); i++)
		delete m_worlds[i];
	m_worlds.clear();
	m_worlds.resize(m_head.nodeNum);

	for(int i = 0; i < m_head.nodeNum; i++)
	{
		SmmNode data = m_nodeList[i];
		float scaleK = 1.0f;
		if(data.mirror)
			scaleK = -scaleK;

		Matrix4f rotat(Quaternion(data.rotat.x, data.rotat.y, data.rotat.z, data.rotat.w));
		Matrix4f trans = Matrix4f::TransMat(data.trans.x, data.trans.y, data.trans.z);
		Matrix4f scale = Matrix4f::ScaleMat(scaleK, scaleK, scaleK);

		Matrix4f world;
		world = rotat * world;
		world = trans * world;
		world = world * scale;

		m_worlds[i] = new Matrix4f(world);
	}

}

Matrix4f Smm::getWorldSpace(int i)
{
	if(i >= m_worlds.size())
	{
		Matrix4f tt;
		return tt;
	}
	else
		return *m_worlds[i];
}
