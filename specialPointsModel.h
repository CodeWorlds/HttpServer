#pragma once
#include "model.h"
#include <fstream>
/************************************************************************
   created: 2018/01/22
   class: 
   author: linyi
   usage: �̳���modelbase�����ڻ������㣨λ�ã����ߣ�sdfֵ��
************************************************************************/

struct SpecialPoint
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec3 sdf;	
};

class SpecialPointsModel :public Model
{
public:
	SpecialPointsModel(std::string str);
	~SpecialPointsModel();
	virtual bool loadModel(std::string str);
	virtual void printInfo();
	virtual void setupModel();
	virtual void drawModel(Shader *shader);
	void readyToWork();
private:
	std::vector<SpecialPoint> specialPoints;
};
