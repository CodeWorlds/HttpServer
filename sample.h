#ifndef _SAMPLE_
#define _SAMPLE_
#include "callback.h"
#include "camera.h"
#include "shader.h"
#include "glCallback.h"
#include "offmodel.h"
#include "specialPointsModel.h"
#include <vector>

/************************************************************************
   created: 2018/01/18
   class: 
   author: linyi
   usage: ���ƻ��࣬�����������shader��model�ȶ���࣬�̳��Իص�����
************************************************************************/

class Sample :public ICallbacks
{
public:
	Sample();
	~Sample();
	virtual void MouseAction(double x, double y);
	virtual void KeyAction(int key,int action);
	virtual void RenderScene();
	void sampleInit();
	void sampleRun();
	void drawModel();
	void treatKey();
private:
	Camera * camera;
	Shader * shader;
	offModel * offmodel1;
	offModel * offmodel2;
	SpecialPointsModel * spModel1;
	SpecialPointsModel * spModel2;
	bool firstMouse;
	double lastX, lastY;
	int width, height;
	GLfloat deltaTime;
	GLfloat lastFrame;
	std::vector<bool> key;
};

#endif