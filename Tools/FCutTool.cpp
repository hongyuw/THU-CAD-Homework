#include "../external/FKernel/include/FKernelUtilities.h"
#include "../FUtilities.h"
#include "../MainDoc.h"
#include"../external/FKernel/include/Features/FPlane.h"
#include "FCutTool.h"


void FCutTool::Init()
{
	m_pSketch = NULL;
	m_pStroke = NULL;
	m_iKeyState = 0;
}

FCutTool::FCutTool(FMainDoc* pDoc, FEntity* pEnt, int sx, int sy)
	:FFeatureTool(pDoc, sx, sy),m_iNum(0)
{
	m_pDoc = pDoc;
	if (pEnt->GetEntType()==SKETCH_SKETCH)
		m_pSketch = (SketchSketch*) pEnt;
	else
	   Init();
}
FCutTool::~FCutTool()
{

}

void FCutTool::CreateToolUI()
{

}

int FCutTool::ProcessEvent(FEventType eventType, const FPickEvent& pe)
{
	F3dPoint tPnt;
	double tx, ty, tz;
	if (!m_pSketch)
	{
		m_pSketch = new SketchSketch;			
		m_pDoc->AddDispEnt(m_pSketch);
	}
	if (!m_pDoc)	return 0;
	if(eventType == MOUSE_BUTTON_DOWN)
	{
		if(pe.nFlags == MK_LBUTTON && m_iNum==0)
		{			
			m_iNum=1;
			m_pStroke = new SketchStroke;	
			pe.view->Screen2WCS(pe.x, pe.y, tPnt);

			m_pStroke->m_pnts.push_back(tPnt);
			m_pSketch->m_strokes.push_back(m_pStroke);
			
		}
	}
	if( eventType==MOUSE_MOVE && pe.nFlags==MK_LBUTTON && m_iNum==1)
	{
		pe.view->Screen2WCS(pe.x, pe.y, tx, ty, tz);
		m_pStroke->m_pnts.push_back(F3dPoint(tx, ty, tz));	

	}
	if(eventType == MOUSE_BUTTON_UP)
	{
		if (pe.nFlags == MK_LBUTTON && m_iNum == 1)
		{
			m_iNum = 0;
	        Cutopreation();
					
		}
		else if (pe.nFlags == MK_RBUTTON)
		{
			//int nStroke = m_pSketch->GetStrokeSize();
			//m_pSketch->RemoveStroke(nStroke-1);
			m_iNum=0;
		}
	}
	
	return 1;
}


void  FCutTool::Cutopreation()
{
	//得到stroke的首尾两点
	F3dPoint* first = &m_pStroke->m_pnts[0];
	F3dPoint* last = &m_pStroke->m_pnts.back();	
	//得到中点与法向量
	F3dPoint mid((first->x+last->x)/2,0/*(first->y+last->y)/2*/,(first->z+last->z)/2); 
	F3dVector strokevec(first->x-last->x,first->y-last->y,first->z-last->z);
	F3dVector viewvec(0,1,0);
	F3dVector normal=ON_CrossProduct(strokevec,viewvec);
	//计算平面
	m_pPlane = new FT::FPlane(mid,normal);
	m_pDoc->AddDispEnt((FEntity*)m_pPlane);
	m_pPlane->Update();
	m_pDoc->Update();

	//获取显示实体
	for(int m=0;m<m_pDoc->DispEntCount();m++)
	{
		F3DFeature* pFt(NULL);
		FEntity* pEntity=m_pDoc->GetDispEnt(m);

		if(pEntity->FeatureType()!=FT_3D) 
			continue;
		pFt = (F3DFeature*)pEntity;
		FBody*	Body=pFt->Body();
		if(!Body) 
			continue;

		int nsize=Body->m_faces.count(); 
		ON_Mesh *pMesh(NULL);

		for(int i=0;i<nsize;i++)
		{
			if(Body->m_faces[i]->GetSurface()==NULL)
				continue;
			pMesh=Body->m_faces[i]->GetSurface()->GetMesh()->OnMesh();
			if(!pMesh)
				continue;

			int nFace = pMesh->FaceCount();    //金字塔有1个body face  4个mesh face，兔子有1个body face，2091个mesh face
			vector<bool> facelist(nFace,0);

			for(int j=0;j<nFace;j++)
			{
				float re[3]={0};
				for(int k=0;k<3;k++)
				{
					int index = pMesh->m_F[j].vi[k];   //第J个mesh face有三个点，返回点的index
					F3dPoint pnt = pMesh->m_V[index];  //在m_V中输入index获得点的坐标
					F3dVector vec1(pnt.x-mid.x,pnt.y-mid.y,pnt.z-mid.z);
					re[k]=ON_DotProduct(vec1,normal);//vec1.x*normal.x+vec1.y*normal.y+vec1.z*normal.z
				}
				//符号判断，三个点都为正代表这个mesh face在平面法向量的同侧
				if(re[0]>=0&&re[1]>=0&&re[2]>=0)
				{
					//pMesh->m_F.Remove(j);
					facelist[j]=true;
				}
				else
					facelist[j]=false;
			}
			//删除点
			for(int m=nFace-1;m>=0;m--)
			{
				if(facelist[m])
					pMesh->m_F.Remove(m);
			}
			Body->m_faces[i]->GetSurface()->GetMesh()->Update();
		}
	}

	m_pDoc->Update();	
}


//void  FCutTool::Cutopreation()
//{
//	F3dPoint* first = &m_pStroke->m_pnts[0];
//	F3dPoint* last = &m_pStroke->m_pnts.back();	
//	//F3dPoint* first = m_pStroke->m_pntList.At(0);
//	//F3dPoint* last = m_pStroke->m_pntList.At(m_pStroke->m_pntList.size()-1);
//	F3dPoint mid((first->x+last->x)/2,0/*(first->y+last->y)/2*/,(first->z+last->z)/2); 
//	F3dVector strokevec(first->x-last->x,first->y-last->y,first->z-last->z);
//	//FViewType myview = pe.view->GetViewType();
//	F3dVector viewvec(0,1,0);
//		//switch(myview)
//		//{
//		//case VIEW_TOP:
//		//	viewvec.x=0;
//		//	viewvec.y=0;
//		//	viewvec.z=1;
//		//	break;
//		//}
//	F3dVector normal=ON_CrossProduct(strokevec,viewvec);
//
//	m_pPlane = new FT::FPlane(mid,normal);
//	m_pDoc->AddDispEnt((FEntity*)m_pPlane);
//	m_pPlane->Update();
//	m_pDoc->Update();
//	int kk = m_pDoc->DispEntCount();
//	for(int m=0;m<m_pDoc->DispEntCount();m++)
//	{
//		F3DFeature* pFt(NULL);
//		FEntity* pEntity=m_pDoc->GetDispEnt(m);
//
//		if(pEntity->FeatureType()!=FT_3D) 
//			continue;
//		pFt = (F3DFeature*)pEntity;
//		int nsize=Body->m_faces.count(); 
//		ON_Mesh *pMesh(NULL);
//			
//
//		//for(int n=0;n<nsize;n++)
//		//{
//		//	if(Body->m_faces[n]->GetSurface()==NULL)
//		//		continue;
//		//	pMesh=Body->m_faces[n]->GetSurface()->GetMesh()->OnMesh();
//		//	if(!pMesh)
//		//		continue;
//
//		//	int nVert=pMesh->VertexCount();
//		//	vector<bool> pntlist(nVert,0);
//
//		//	for(int m=0;m<nVert;m++)
//		//	{
//		//		F3dPoint pnt = pMesh->m_V[m];
//		//		F3dVector vec1(pnt.x-mid.x,pnt.y-mid.y,pnt.z-mid.z);
//		//		double a = ON_DotProduct(vec1,normal);
//		//		if(a>=0)
//		//			pntlist[m]=true;
//		//		else
//		//			pntlist[m]=false;
//		//	}
//		//	int nFace = pMesh->FaceCount();
//
//		//	for(int j=0;j<nFace;j++)
//		//	{
//		//		int index0=pMesh->m_F[j].vi[0];
//		//		int index1=pMesh->m_F[j].vi[1];
//		//		int index2=pMesh->m_F[j].vi[2];
//		//		if(pntlist[index0]||pntlist[index1]||pntlist[index2])//(pntlist[index0]&&pntlist[index1])||(pntlist[index1]&&pntlist[index2])||(pntlist[index0]&&pntlist[index2])
//		//			pMesh->m_F.Remove(j);
//		//	}
//		//	Body->m_faces[n]->GetSurface()->GetMesh()->Update();
//		//}
//
//		for(int i=0;i<nsize;i++)
//		{
//			if(Body->m_faces[i]->GetSurface()==NULL)
//				continue;
//			pMesh=Body->m_faces[i]->GetSurface()->GetMesh()->OnMesh();
//			if(!pMesh)
//				continue;
//
//			int nFace = pMesh->FaceCount();
//			vector<bool> facelist(nFace,0);
//
//			for(int j=0;j<nFace;j++)
//			{
//				float re[3]={0};
//				for(int k=0;k<3;k++)
//				{
//					int index = pMesh->m_F[j].vi[k];
//					F3dPoint pnt = pMesh->m_V[index];
//					//double x=pMesh->m_V[pMesh->m_F[j].vi[k]].x;
//					//double y=pMesh->m_V[pMesh->m_F[j].vi[k]].y;
//					//double z=pMesh->m_V[pMesh->m_F[j].vi[k]].z;
//					F3dVector vec1(pnt.x-mid.x,pnt.y-mid.y,pnt.z-mid.z);
//					re[k]=ON_DotProduct(vec1,normal);//vec1.x*normal.x+vec1.y*normal.y+vec1.z*normal.z
//				}
//				//符号判断
//				if(re[0]>=0&&re[1]>=0&&re[2]>=0)
//				{
//					//pMesh->m_F.Remove(j);
//					facelist[j]=true;
//				}
//				else
//					facelist[j]=false;
//			}
//			for(int m=nFace-1;m>=0;m--)
//			{
//				if(facelist[m])
//					pMesh->m_F.Remove(m);
//			}
//
//
//			Body->m_faces[i]->GetSurface()->GetMesh()->Update();
//		}
//	}
//	
//	m_pDoc->Update();	
//}
