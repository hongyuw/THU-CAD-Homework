#include "FFileIO.h"

FFileIO::FFileIO(void){}

FFileIO::~FFileIO(void){}

FMesh* FFileIO::ImportOBJ(char* fname)
{
	//const std::string filename = "C:\program1\iDesignCAD\iDesignCAD-STU\hw\bunny.obj";	
	//std::ifstream obj_stream(filename.c_str());
	//if(!obj_stream.good())
	//{
	//    int a = 0;
	//}

	const std::string filename = fname;  //"../hw/bunny.obj";	
	std::fstream obj_stream;							//�����������ͣ��ļ���
	obj_stream.open(filename.c_str(),std::ios_base::in);   //���ļ�
	if(!obj_stream.is_open())
	{
		return NULL; 
	}
	char linebuf[256],buf[100];
	int i;
	float xx,yy,zz; 
	int x,y,z;
	F3fPointList pnts;
	F3fVectorList	norms;
	FMeshFaceList	faces;
	ON_MeshFace fi;
	sprintf_s(buf,100,"");
	while(!obj_stream.getline(linebuf,255).eof()) { //��ȡһ�У�����linebuf eof��˼��������в�Ϊ��

		sscanf_s(linebuf,"%s",buf,100); //��linebuf���ȡ��һ���ַ������ŵ�buf�� ���� vn 1.2 0.3 1.4�Ͷ�vn��f 2.4 2.3 1.1�Ͷ�f

		if ( (strlen(buf)==1) && !(strcmp(buf,"v")) )//(buf[0]=='v')  //��
		{
			sscanf_s(linebuf, "%s %f %f %f \n", buf,100, &xx, &yy, &zz);
			pnts.Append(F3fPoint(xx,yy,zz));
		}
		if ( (strlen(buf)==1) && !(strcmp(buf,"vn")) )//(buf[0]=='vn') //������
		{
			sscanf_s(linebuf, "%s %f %f %f \n", buf,100, &xx, &yy, &zz);
			norms.Append(F3fVector(xx,yy,zz));
		}
		else if ( (strlen(buf)==1) && (buf[0]=='f') )   //��
		{
			sscanf_s(linebuf, "%s %d %d %d \n", buf,100, &x, &y, &z);
			fi.vi[0] = x-1;
			fi.vi[1] = y-1;
			fi.vi[2] = z-1;
			fi.vi[3] = fi.vi[2];
			faces.Append(fi);
		}

		sprintf_s(buf,100,"");
		sprintf_s(linebuf,256,"");

	}

	FMesh* pMesh = new FMesh;
	pMesh->AddVert(pnts);
	pMesh->AddNorm(norms);
	pMesh->AddFace(faces);
	pMesh->Update();
	return pMesh;
}
