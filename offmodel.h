#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <glm/glm.hpp>
#include <cmath>
#include <random>
#include <unordered_set>
#include "shader.h"
#include "modelBase.h"


/************************************************************************
   created: 2018/01/18
   class: 
   author: linyi
   usage: ����off�ļ���model���̳���Model����
************************************************************************/
class offModel:public ModelBase
{
public:
	float maxEdge;
	glm::vec3 box_center;
	offModel(std::string str);
	~offModel();
	virtual bool loadModel(std::string str);
	virtual void printInfo();
	virtual void setupModel();	
	virtual void drawModel(Shader *shader);
	virtual void findAdjactRelation();
	void setupModel2();	// ת��Ϊ�б���ʹ��EBO	
	void drawModel2(Shader *shader);	
	void setupModel3();	// ʹ�÷���
	void setupModel4();	// ��ȡ��Ե
	void findLocalMax();	// Ѱ�Ҿֲ�����
	void findTrianglesNormal();	// ���������η���
	void findDihedral();	// ��������
	void loadTriangleSDF(std::string);	// ����������SDF
	void readyToWork();		// ��ɳ�������
private:
	std::vector<glm::vec3> Points;	// ����Ϣ
	std::vector<glm::ivec3> Triangles;	// ������Ƭ
	std::vector<float> trianglesSDF;	// ������SDF
	std::vector<glm::vec3> trianglesNormal;	// �����η���
	std::vector<float> Dihedral;	// �����
	int pointsCnt;
	int trianglesCnt;
	int edgesCnt;
	int specialPointsCnt;
	GLuint pVAO;		// �����VAO
	GLuint pVBO;
	glm::vec3 box_min = glm::vec3(1e10);
	glm::vec3 box_max = glm::vec3(-1e10);	
	std::unordered_map<int, std::vector<int> > adjactRelation;	// �洢�ڽӹ�ϵ
};