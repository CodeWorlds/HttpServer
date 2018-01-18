#pragma  once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <glm/glm.hpp>
#include <algorithm>
using namespace std;
#define PI 3.1415926
float maxCount = 0;
float maxRadius = 0;

struct segDataStruct{
	vector<glm::vec3> data;
	vector<glm::vec3> color;
	glm::vec3 center;
	int count;
	float radius;
	segDataStruct(vector<glm::vec3> d, vector<glm::vec3> co, glm::vec3 c, int cnt, float r)
	{
		color = co;
		data = d;
		center = c;
		count = cnt;
		radius = r;
	}		
};

ostream & operator<<(ostream & onfs, const glm::vec3 &v)
{
	onfs << v[0] << " " << v[1] << " " << v[2];
	return onfs;
}

ostream & operator<<(ostream & onfs, const glm::ivec3 &v)
{
	onfs << v[0] << " " << v[1] << " " << v[2];
	return onfs;
}

bool cmp(vector<glm::vec3> a, vector<glm::vec3> b)
{
	return a.size() > b.size();
}

bool cmpStruct(const segDataStruct &a, const segDataStruct &b)
{
	float mid1 = a.count/maxCount + a.radius/maxRadius;
	float mid2 = b.count/maxCount + b.radius/maxRadius;
	if (mid1 > mid2)
		return true;
	return false;
}

// ��������ĳ���
float getVecMagnitude(glm::vec3 v)
{
	return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

// �������������
float getTriangleArea(glm::vec3 a, glm::vec3 b, glm::vec3 c){
	float area = 0;

	float side[3];//�洢�����ߵĳ���;  

	side[0] = sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
	side[1] = sqrt(pow(a.x - c.x, 2) + pow(a.y - c.y, 2) + pow(a.z - c.z, 2));
	side[2] = sqrt(pow(c.x - b.x, 2) + pow(c.y - b.y, 2) + pow(c.z - b.z, 2));

	//���ܹ���������;  
	if (side[0] + side[1] <= side[2] || side[0] + side[2] <= side[1] || side[1] + side[2] <= side[0]) return area;

	//���ú��׹�ʽ��s=sqr(p*(p-a)(p-b)(p-c));   
	float p = (side[0] + side[1] + side[2]) / 2; //���ܳ�;  
	area = sqrt(p*(p - side[0])*(p - side[1])*(p - side[2]));

	return area;
}

/*
x = sin(fi)cos(sita);
y = sin(fi)sin(sita);
z = cos(fi);
fi ����0~pi
sita ���� 0~2pi
*/
// ����һ��������sita ��fi�н�
glm::vec2 getVectorAngle(glm::vec3 x)
{
	glm::vec2 v;
	v[1] = acos(x[2]);		// ���fi
	x[1] /= sin(v[1]);
	x[0] /= sin(v[1]);
	// 
	float t1 = acos(x[0]);
	float t2 = asin(x[1]);
	if (abs(t1 - t2) < 0.01)
		v[0] = t1;
	else
	{
		if (t1 <= PI / 2 && t1 >= 0)
		{
			v[0] = 2 * PI - t1;
		}
		else
		{
			v[0] = t1;
		}
	}
	return v;
}

// ���㷨��
glm::vec3 getNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	glm::vec3 v1 = b - a;
	glm::vec3 v2 = c - b;
	return glm::normalize(glm::cross(v1, v2));
}

// �����ֱ��ͼ
void saveHistogram(string outfile, vector<glm::vec3> normal)
{
	int count = normal.size();
	int mat[180][90] = { 0 };
	for (int i = 0; i < count; i++)
	{
		glm::vec2 v1 = getVectorAngle(normal[i]);
		float mid = 2 * PI / 360;
		mid *= 2;
		mat[(int)(v1[0] / mid)][(int)(v1[1] / mid)]++;
	}
	ofstream onfs(outfile);
	for (int i = 0; i < 180; i++)
	{
		for (int j = 0; j < 90; j++)
		{
			onfs << mat[i][j] << " ";
		}
		onfs << endl;
	}
	onfs.close();
}

// ����ÿ��segment �ĵ÷�ָ��
struct gradeNode
{
	glm::vec3 centerPos;	// ����λ��
	float radius;			// �뾶
	float meanArea;			// ƽ��������Ƭ���
	int pointsCnt;			// ����������
	gradeNode(){}
	gradeNode(glm::vec3 c, float r, float m, int p)
	{
		centerPos = c;
		radius = r;
		meanArea = m;
		pointsCnt = p;
	}
};

class offLoader
{		
public:
	vector<glm::vec3> returnPoint()
	{
		return point;
	}

	vector<glm::ivec3> returnIndex()
	{
		return index;
	}

	vector<glm::vec3> returnNormal()
	{
		return normal;
	}

	vector<glm::vec3> returnData()
	{
		return pointDataTotal;
	}

	offLoader(){}

	// ����ģ��
	void loadOff(string filename)
	{
		ifstream infile(filename);
		if (!infile.is_open())
		{
			cout << "open file error\n";
			exit(0);
		}
		string s;
		while (infile >> s)
		{
			glm::ivec3 total;		// ����������������������
			infile >> total.x >> total.y >> total.z;			
			// �����㣬���Ҽ����Χ��
			for (int i = 0; i < total.x; i++)
			{
				glm::vec3 p;
				infile >> p.x >> p.y >> p.z;
				box_min.x = min(p.x, box_min.x);
				box_min.y = min(p.y, box_min.y);
				box_min.z = min(p.z, box_min.z);
				box_max.x = max(p.x, box_max.x);
				box_max.y = max(p.y, box_max.y);
				box_max.z = max(p.z, box_max.z);
				this->point.push_back(p);
			}	
			box_center = (box_min + box_max);
			box_center /= 2;
			for (int i = 0; i < point.size(); i++)
				this->point[i] -= this->box_center;
			box_center = glm::vec3(0);
			// ����������
			for (int i = 0; i < total.y; i++)
			{
				glm::ivec3 idx;
				int tmp;
				infile >> tmp >> idx.x >> idx.y >> idx.z;
				this->index.push_back(idx);
			}
		}
		infile.close();
		treatData();
		calcParameter();
	}

	void calcParameter()
	{		
		//cout << "box_center: " << box_center;
		//box_size = glm::vec3(box_max.x - box_min.x, box_max.y - box_min.y, box_max.z - box_min.z);
		box_size = box_max - box_min;
		//cout << "\nBox_size: " << box_size;
		//box_center = glm::vec3(0);
		viewX = box_center + glm::vec3(box_size.x,0,0);
		//cout << "\nviewX: " << viewX;
		viewY = box_center + glm::vec3(0, box_size.y, 0);
		//cout << "\nviewY: " << viewY;
		viewZ = box_center + glm::vec3(0, 0, box_size.z);
		//cout << "\nviewZ: " << viewZ;
	}

	// ��������������Ϊ����
	void treatData()
	{
		for (int i = 0; i < this->index.size(); i++)
		{
			this->pointDataTotal.push_back(point[index[i].x]);
			this->pointDataTotal.push_back(point[index[i].y]);
			this->pointDataTotal.push_back(point[index[i].z]);
		}
	}

	glm::vec3 box_center;	// ����
	glm::vec3 box_size;		// �ߴ�
	glm::vec3 viewX;		// �����ӵ�
	glm::vec3 viewY;
	glm::vec3 viewZ;

private:
	// ԭʼ������
	vector<glm::vec3> point;   // �洢ȫ���Ķ���
	vector<glm::ivec3> index;	// �洢�����ε�����
	vector<glm::vec3> normal;	// ������״��ʾ�ķ���

	// ��Χ��
	glm::vec3 box_min = glm::vec3(1e10);
	glm::vec3 box_max = glm::vec3(-1e10);	

	vector<glm::vec3> pointDataTotal;		// �洢Ҫ���Ķ��㣬������ת������
};
