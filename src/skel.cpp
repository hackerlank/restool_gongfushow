#include "skel.h"


Skel::Skel(const char* filename)
{
	cout << "Load Skel: " << filename << endl;
	strcpy(m_path, filename);
	m_file.open(filename, ios::in|ios::binary);

	//read head
	memset(&m_head, 0, sizeof(m_head));
	m_file.seekg(0);
	m_file.read((char *)&m_head, sizeof(m_head));

	//read info
	m_file.seekg(m_head.infoOffset);
	int n = 0;
	m_file.read((char *)&n, sizeof(n));
	cout << "BonesNum " << n << endl;

	char name[255];
	for(int i = 0; i < n; i++)
	{
		uint32 len;
		m_file.read((char *)&len, sizeof(len));
		m_file.read(name, len);
		m_info.boneNames.push_back(name);
		cout << "Bone[" << i << "]" << name << endl;
	}


	uint32 rootNum;
	m_file.read((char *)&rootNum, sizeof(rootNum));
	for(int i = 0; i < rootNum; i++)
	{
		uint32 rootIdx;
		m_file.read((char *)&rootIdx, sizeof(rootIdx));
		m_info.boneRoots.push_back(rootIdx);
		cout << "Root[" << i << "]" << rootIdx << endl;
	}

	//read frames
	m_file.seekg(m_head.frameOffset);
	for(int i = 0; i < m_head.frameNum; i++)
	{
		SkelFrame frame;

		memset(frame.name, 0, 5);
		m_file.read(frame.name, 4);
		//cout << "frame[" << i << "]" << frame.name << endl;

		uint32 boneNum;
		m_file.read((char *)&boneNum, sizeof(boneNum));
		//cout << "frame[" << i << "]" << "boneDatas->" << boneNum << endl;

		for(int j = 0; j < boneNum; j ++)
		{
			BoneData bone;
			m_file.read((char *)&bone.id, sizeof(bone.id));
			m_file.read((char *)&bone.parent, sizeof(bone.parent));
			uint32 len;
			m_file.read((char *)&len, sizeof(len));
			//cout << "frame[" << i << "]" << bone.id << " " << bone.parent << " " << len << endl;
			for(int m = 0; m < len; m++)
			{
				uint32 child;
				m_file.read((char *)&child, sizeof(child));
				bone.children.push_back(child);
			}

			m_file.read((char *)&bone.trans, sizeof(bone.trans));
			m_file.read((char *)&bone.rotat, sizeof(bone.rotat));
			m_file.read((char *)&bone.mirror, sizeof(bone.mirror));
			frame.boneDatas.push_back(bone);
			//cout << "frame[" << i << "]" << "bone[" << j << "]trans " << bone.trans.x << " " << bone.trans.y << " " << bone.trans.z << " " << (int)bone.mirror << endl;
			//cout << "frame[" << i << "]" << "bone[" << j << "]rotat" << bone.rotat.x << " " << bone.rotat.y << " " << bone.rotat.z << " " << bone.rotat.w << endl;
		}

		m_file.read((char *)&frame.time, sizeof(frame.time));
		//cout << "frame[" << i << "]" << "time->" << frame.time<< endl;

		uint32 nodeNum;
		m_file.read((char *)&nodeNum, sizeof(nodeNum));
		//cout << "frame[" << i << "]" << "boneNodes->" << nodeNum << endl;
		for(int k = 0; k < nodeNum; k ++)
		{
			BoneNode node;
			m_file.read((char *)&node, sizeof(node));
			frame.boneNodes.push_back(node);
		}

		m_frames.push_back(frame);
	}

}


Skel::~Skel()
{
	m_file.close();
}

void Skel::initWorldSpace(int frameId)
{
	for(int i = 0; i < m_worlds.size(); i++)
		delete m_worlds[i];
	m_worlds.clear();
	m_worlds.resize(m_info.boneNames.size());

	for(int i = 0; i < m_info.boneRoots.size(); i++)
	{
		initWorldSpace(frameId, m_info.boneRoots[i]);
	}

}
void Skel::initWorldSpace(int frameId, int boneId)
{
	if(frameId >= m_frames.size())
		return;
	SkelFrame frame = m_frames[frameId];
	if(boneId >= frame.boneDatas.size())
		return;

	BoneData data = frame.boneDatas[boneId];
	float scaleK = 1.0f;
	if(data.mirror)
		scaleK = -scaleK;

	Matrix4f rotat(Quaternion(data.rotat.x, data.rotat.y, data.rotat.z, data.rotat.w));
	Matrix4f trans = Matrix4f::TransMat(data.trans.x, data.trans.y, data.trans.z);
	Matrix4f scale = Matrix4f::ScaleMat(scaleK, scaleK, scaleK);

	Matrix4f world;
	world = rotat * world;
	world = trans * world;

	if(data.parent != -1)
		world = getWorldSpace(data.parent) * world;

	world = world * scale;

	m_worlds[boneId] = new Matrix4f(world);

	for(int i = 0; i < data.children.size(); i ++)
		initWorldSpace(frameId, data.children[i]);
}

Matrix4f Skel::getWorldSpace(int boneId)
{
	if(boneId >= m_worlds.size())
	{
		Matrix4f tt;
		return tt;
	}
	else
		return *m_worlds[boneId];
}

void Skel::showHeadInfo()
{
	cout << "=================================================" << endl;
	cout << "[Head]" << m_path << endl;
    cout << "Lable       " << hex << showbase << m_head.Lable        << endl; 
    cout << "Version     " << dec << showbase << (int)m_head.Version      << endl; 
    cout << "infoOffset  " << hex << showbase << m_head.infoOffset   << endl;  
    cout << "frameNum    " << dec << showbase << m_head.frameNum     << endl; 
    cout << "frameOffset " << hex << showbase << m_head.frameOffset  << endl; 
	cout << "=================================================" << endl;

}

