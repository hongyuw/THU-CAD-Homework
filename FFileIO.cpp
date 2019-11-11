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
	std::fstream obj_stream;							//创建数据类型：文件流
	obj_stream.open(filename.c_str(),std::ios_base::in);   //打开文件
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
	while(!obj_stream.getline(linebuf,255).eof()) { //读取一行，存入linebuf eof意思是如果这行不为空

		sscanf_s(linebuf,"%s",buf,100); //从linebuf里读取第一个字符串，放到buf里 比如 vn 1.2 0.3 1.4就读vn，f 2.4 2.3 1.1就读f

		if ( (strlen(buf)==1) && !(strcmp(buf,"v")) )//(buf[0]=='v')  //点
		{
			sscanf_s(linebuf, "%s %f %f %f \n", buf,100, &xx, &yy, &zz);
			pnts.Append(F3fPoint(xx,yy,zz));
		}
		if ( (strlen(buf)==1) && !(strcmp(buf,"vn")) )//(buf[0]=='vn') //法向量
		{
			sscanf_s(linebuf, "%s %f %f %f \n", buf,100, &xx, &yy, &zz);
			norms.Append(F3fVector(xx,yy,zz));
		}
		else if ( (strlen(buf)==1) && (buf[0]=='f') )   //面
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
