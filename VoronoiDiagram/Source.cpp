#include <iostream>
#include <vector>
#include <math.h>
#include <time.h>

#include <edgePoint.h>

#include <GLUT/glut.h>
#include <GLM/glm/glm.hpp>
#include <GLM/glm/gtc/matrix_transform.hpp>
#include <GLM/glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;


const float width = 800, height = 600;
const float paperR = 0.992f, paperG = 0.96f, paperB = 0.9f;
const float pointSize = 5.0f;
const float threshold = 0.006f;
const int iterationNum = 50, pointNum = 20;

vector<vec2> generators;
vector<edgePoint> edgePoints;

vector<vec2> newGenerators;
vector<edgePoint> newEdgePoints;

float myNormalize(float f, float l)
{
	return f / l * 2.0f - 1.0f;
}

float mylength(vec2 a, vec2 b)
{
	return sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2));
}

void generatePoints(int pointNum)
{
	srand(time(NULL));

	for (int i = 0; i < pointNum; i++)
	{

		float x = rand() % 800;
		float y = rand() % 600;

		//cout << x << "," << y << endl;

		x = myNormalize(x, width);
		y = myNormalize(y, height);

		vec2 pos = vec2(x, y);
		generators.push_back(pos);
	}

	newGenerators = generators;
}

void genVor()
{
	for (float i = -1.0f; i <= 1.0f; i = i + 2.0f / width)
	{
		for (float j = -1.0f; j <= 1.0f; j = j + 2.0f / height)
		{
			vec2 pos = vec2(i, j);
			float min = 10000.0f;
			float min2 = 20000.0f;

			int tempK, tempK2;

			for (int k = 0; k < generators.size(); k++)
			{

				if (mylength(pos, generators[k]) < min)
				{
					min = mylength(pos, generators[k]);
					tempK = k;
				}

				if (mylength(pos, generators[k]) < min2 && mylength(pos, generators[k]) != min)
				{
					min2 = mylength(pos, generators[k]);
					tempK2 = k;
				}
			}

			if (fabs(min - min2) <= threshold)
			{
				edgePoint ep = edgePoint();
				ep.pos = vec2(i, j);
				if (tempK <= tempK2)
				{
					ep.p.first = tempK;
					ep.p.second = tempK2;
				}
				else
				{
					ep.p.first = tempK2;
					ep.p.second = tempK;
				}
				edgePoints.push_back(ep);
			}

		}
	}

	newEdgePoints = edgePoints;
}

void updateEdgePoints()
{
	newEdgePoints.clear();

	for (float i = -1.0f; i <= 1.0f; i = i + 2.0f / width)
	{
		for (float j = -1.0f; j <= 1.0f; j = j + 2.0f / height)
		{
			vec2 pos = vec2(i, j);
			float min = 10000.0f;
			float min2 = 20000.0f;

			int tempK, tempK2;

			for (int k = 0; k < newGenerators.size(); k++)
			{

				if (mylength(pos, newGenerators[k]) < min)
				{
					min = mylength(pos, newGenerators[k]);
					tempK = k;
				}

				if (mylength(pos, newGenerators[k]) < min2 && mylength(pos, newGenerators[k]) != min)
				{
					min2 = mylength(pos, newGenerators[k]);
					tempK2 = k;
				}
			}

			if (fabs(min - min2) <= threshold)
			{
				edgePoint ep = edgePoint();
				ep.pos = vec2(i, j);
				if (tempK <= tempK2)
				{
					ep.p.first = tempK;
					ep.p.second = tempK2;
				}
				else
				{
					ep.p.first = tempK2;
					ep.p.second = tempK;
				}
				newEdgePoints.push_back(ep);
			}

		}
	}
}

void updateVor()
{
	for (int k = 0; k < iterationNum; k++)
	{
		for (int i = 0; i < newGenerators.size(); i++)
		{
			vec2 newPos = vec2(0.0f, 0.0f);
			float posNum = 0.0f;

			for (int j = 0; j < newEdgePoints.size(); j++)
			{
				if (newEdgePoints[j].p.first == i || newEdgePoints[j].p.second == i)
				{
					newPos += newEdgePoints[j].pos;
					posNum++;
				}
			}

			newPos /= posNum;

			newGenerators[i] = newPos;
		}

		updateEdgePoints();
	}
}

void Display(void)
{
	glClearColor(paperR, paperG, paperB, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for(int i = 0; i < edgePoints.size(); i++)
	{
		glEnable(GL_POINT_SMOOTH);
		glBegin(GL_POINTS);

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex2f(edgePoints[i].pos.x, edgePoints[i].pos.y);
		glEnd();
	}

	for(int i = 0; i < generators.size(); i++)
	{
		glPointSize(pointSize);

		glEnable(GL_POINT_SMOOTH);
		glBegin(GL_POINTS);

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex2f(generators[i].x, generators[i].y);
		glEnd();
	}

	glutSwapBuffers();
}

void Display2(void)
{
	glClearColor(paperR, paperG, paperB, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < newEdgePoints.size(); i++)
	{
		glEnable(GL_POINT_SMOOTH);
		glBegin(GL_POINTS);

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex2f(newEdgePoints[i].pos.x, newEdgePoints[i].pos.y);
		glEnd();
	}

	for (int i = 0; i < newGenerators.size(); i++)
	{
		glPointSize(pointSize);

		glEnable(GL_POINT_SMOOTH);
		glBegin(GL_POINTS);

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex2f(newGenerators[i].x, newGenerators[i].y);
		glEnd();
	}

	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	cout << "Waiting for computing Voronoi Diagram..." << endl;

	generatePoints(pointNum);
	genVor();

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Voronoi Diagram");
	glutDisplayFunc(Display);

	updateVor();

	glutInitWindowPosition(1000, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Centroidal Voronoi Diagram");
	glutDisplayFunc(Display2);


	glutMainLoop();
	return 0;
}