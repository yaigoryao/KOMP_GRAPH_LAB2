// подключаем заголовочные файлы библиотек
#include "glew.h"
#include "glut.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <string>
#include <vector>

GLfloat wSize = 150.f;

GLfloat xRot = 0.f, yRot = 0.f;
struct RGB
{
	RGB(float R, float G, float B) : r{ R }, g{ G }, b{ B } {};
	RGB() { setWhite(); };
	void setWhite() { this->r = this->g = this->b = 1; };
	float r = 1, g = 1, b = 1;
};

std::vector<GLfloat> sizesV = std::vector<GLfloat>();
std::vector<RGB> colorsV = std::vector<RGB>();
std::vector<GLfloat> sizesLV = std::vector<GLfloat>();

//установка размеров окна
void changeSizeHandler(GLsizei w, GLsizei h)
{
	/*GLfloat size = wSize;*/
	if (h == 0) h = 1;
	//устанавливаем размер окна просмотра соотв. размеру физического окна
	glViewport(0, 0, w, h);

	//очищаем стек матрицы проекций
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	//изменяем размер трехмерной канвы до квадратной
	if (w <= h)
	{
		glOrtho(-wSize, wSize, -h * wSize / w, h * wSize / w, -wSize, wSize);
	}
	else
	{
		glOrtho(-w * wSize / h, w * wSize / h, -wSize, wSize, -wSize, wSize);
	}

	//очищаем стек матрицы просмотра
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Render1()
{
	//предустановка параметров - установка цвета отрисовки, очистка экрана с учетом битов глубины и буфера, сохранение матрицы
	GLfloat a = 0, x = 0, y = 0, z = 0, angle = 0;
	glColor3f(1.f, 1.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	//поворот матрицы в зависимости от углов вращения
	glRotatef(xRot, 1, 0, 0);
	glRotatef(yRot, 0, 1, 0);
	//начало рисования системы из точек
	glBegin(GL_POINTS);

	z = -50.f;
	a = -50.f;
	for (int i = 0; i < 30; i++)
	{
		for (angle = -(2.f * M_PI) - 1; angle < (2.f * M_PI) + 1; angle += .1f)
		{
			x = 16 * pow(sin(angle), 3);
			y = 13 * cos(angle) - 5 * cos(2 * angle) - 2 * cos(3 * angle) - cos(4 * angle);
			glVertex3f(x, y, z);
			z += 0.1f;
		}
		a += 5.f;
	}
	//команда окончания задания системы точек
	glEnd();
	//возвращение матрицы
	glPopMatrix();
	//смена буферов кадра (двойная буферизация)
	glutSwapBuffers();
}

void SpecialKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
		xRot -= 5.0f;
	if (key == GLUT_KEY_DOWN)
		xRot += 5.0f;
	if (key == GLUT_KEY_LEFT)
		yRot -= 5.0f;
	if (key == GLUT_KEY_RIGHT)
		yRot += 5.0f;
	if (key > 356.0f)
		xRot = 0.0f;
	if (key < -1.0f)
		xRot = 355.0f;
	if (key > 356.0f)
		yRot = 0.0f;
	if (key < -1.0f)
		yRot = 355.0f;
	// Обновляем окно
	glutPostRedisplay();
}

std::vector<std::vector<GLfloat>> coords = {};

void Render2()
{
	GLfloat x, y, z, angle; // Место хранения координат и углов
	GLfloat sizes[2]; //Запоминаем диапазон размеров поддерживаемых точек
	GLfloat step; // Запоминаем поддержи-ваемый инкремент размеров точек
	GLfloat curSize; // Записываем размер текущих точек
	// Очищаем окно текущим цветом очистки
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Записывается состояние матрицы и выполняются повороты
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	// Получаем диапазон размеров поддерживаемых то-чек и размер шага
	glGetFloatv(GL_POINT_SIZE_RANGE, sizes);
	glGetFloatv(GL_POINT_SIZE_GRANULARITY, &step);
	// Задаем исходный размер точки
	curSize = sizes[0];
	// Задаем начальную координату z
	// Циклический проход по окружности три раза
	int i = 0;
	for (auto& c : coords)
	{
		glPointSize(sizesV[i]);
		glColor3f(colorsV[i].r, colorsV[i].g, colorsV[i].b);
		// Рисуем точку
		glBegin(GL_POINTS);
		glVertex3f(c[0], c[1], c[2]);
		//if (std::sqrt(std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2) < 50.f*50.f)) glVertex3f(x, y, z);
		glEnd();

		//sizes[0] + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (sizes[1] / 4 - sizes[0])));
		i++;
	}
	//for (angle = -(2.0f * M_PI) * 3.0f; angle <= (2.0f * M_PI) * 3.0f; angle += 0.1f)
	//{
	//	// Расчет значений x и у точек окружности
	//	//x = 50 * sin(angle);
	//	x = -50.f + std::rand() % 101;
	//	y = -50.f + std::rand() % 101;
	//	z = -50.f + std::rand() % 101;
	//	//y = 50 * cos(angle);
	//	// Задаем размер точки перед указанием при-митива
	//	glPointSize(sizesV[i]);
	//	//задаем цвет точки
	//	glColor3f(colorsV[i].r, colorsV[i].g, colorsV[i].b);
	//	// Рисуем точку
	//	glBegin(GL_POINTS);
	//	if (std::sqrt(std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2) < 50.f*50.f)) glVertex3f(x, y, z);
	//	glEnd();
	//	// Увеличиваем значение z и размер точки
	//	//z += 0.5f;
	//	// sizes[0] + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (sizes[1] / 4 - sizes[0])));
	//	//изменяем индексную переменную для изменения цветов и размеров
	//	i++;
	//}
	//восстанавливаем изначальный размер точек
	glPointSize(sizes[0]);
	// Восстанавливаем преобразования
	glPopMatrix();
	// Очищаем стек команд рисования
	glutSwapBuffers();
}

void Render3()
{
	//предустановка параметров - установка цвета отрисовки, очистка экрана с учетом битов глубины и буфера, сохранение матрицы
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	//вращение матрицы в зависимости от углов отклонения
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	//активация возможности использования фактуры линии
	glEnable(GL_LINE_STIPPLE);
	GLfloat x = 0.0f, y = 0.0f, z = 0.0f;
	//задаем паттерн линии
	auto pattern = 0b0011001100110011;
	int i = 0;
	for (int i = 0; i < 50; i+=2)
	{
		//устанавливаем размер ширины линии
		glLineWidth(sizesLV[0] + i % 20);
		//изменяем паттерн
		pattern <= 0 ? pattern = 0b0000000011111111 : pattern <<= 8;
		//устанавливаем множитель паттерна и сам паттерн 
		glLineStipple(i % 10, pattern);
		glBegin(GL_LINES);
		//рисуем спираль, отклоняя конечный угол от начального и изменяя дальнейшие положения линий
		/*x = 30 * angle * sin(15 + angle);
		y = 30 * angle * cos(15 + angle);
		z += 1.f;*/
		glColor3f(colorsV[i].r, colorsV[i].g, colorsV[i].b);
		glVertex3f(coords[i][0]*2, coords[i][1]*2, coords[i][2]*2); // Первая конечная точка отрезка
		/*x = 30 * angle * sin(20 + angle);
		y = 30 * angle * cos(20 + angle);
		z += 2.f;*/
		glColor3f(colorsV[i+1].r, colorsV[i + 1].g, colorsV[i + 1].b);
		glVertex3f(coords[i + 1][0]*2, coords[i + 1][1]*2, coords[i + 1][2]*2); // Первая конечная точка отрезка
		glEnd();
	}
	//сбрасываем паттерн и ширину линии
	glLineStipple(1, 0b1111111111111111);
	glLineWidth(sizesLV[0]);
	//восстанавливаем состояние матрицы
	glPopMatrix();
	// Очищаем стек команд рисования
	glutSwapBuffers();
}

GLfloat approxAspect = 1.f;
void Render4()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	GLfloat x = 0.0f, y = 0.0f, z = -50.0f;
	
	glBegin(GL_LINE_STRIP);
	glColor3f(1.f, 1.f, 1.f);
	for (float angle = -(2.0f * M_PI) * 3.0f; angle <= (2.0f * M_PI) * 3.0f; angle += 0.1f*approxAspect)
	{
		x = 20.0f * angle;
		y = 20.0f * sin(angle);
		glVertex3f(x, y, z);
	}
	glEnd();
	glPopMatrix();
	glutSwapBuffers();
}


float to_rad(float angle)
{
	return (M_PI * angle / 180.f);
}

void Render5()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	GLfloat x = 0.0f, y = 0.0f, z = -50.0f;
	glColor3f(0.1, 0.76, 0.32);
	glBegin(GL_LINE_LOOP);
	bool flag = false;
	for (float angle = -2 * M_PI; angle < 2 * M_PI; angle += .5f)
	{
		glVertex3f(50.f * cos(angle), 50.f * sin(angle), ( flag ? -25.f : 25.f));
		flag = !flag;
	}
	
	glEnd();
	float beg_z = -25.f;
	glColor3b(127, 64, 32);
	glBegin(GL_LINE_STRIP);
	for (float angle = -2 * M_PI; angle < 2 * M_PI; angle += .5f)
	{
		glVertex3f(75.f * cos(angle), 75.f * sin(angle), z);
		z += 5.f;

	}
	glEnd();

	
	/*for (int i = 0; i < 100; i++)
	{
		for (float angle = 0.f; angle <= ((2.0f * M_PI) / 2); angle += 0.1f)
		{
			x = 50.f * sin(angle - i/10);
			y = 50.0f * cos(angle + i/10);
			glVertex3f(x, y, z);
		}
		z += 3.f;
	}*/
	glPopMatrix();
	glutSwapBuffers();
}

bool bCull = true, bDepth = true, bOutline = false, bShadeModel = false;

void Render6()
{
	//предустановка параметров - установка цвета отрисовки, очистка экрана с учетом битов глубины и буфера, сохранение матрицы
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	GLfloat x, y, angle; // Здесь хранятся координаты и углы
	int iPivot = 1; // Используется, чтобы от-мечать
	// чередующиеся цвета
	// Очищаем окно и буфер глубины
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	//устанавливаем заполнение сплошным цветом
	glShadeModel(GL_FLAT);
	//установка лицевой стороны при рисовании против часовой стрелки
	glFrontFace(GL_CW);
	// Включаем отбор, если установлена метка
	if (bCull)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
	// Если установлена метка, активизируем проверку глубины
	if (bDepth)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
	// Если установлена метка, рисуем заднюю сторону
	// в форме каркаса
	if (bOutline)
		glPolygonMode(GL_BACK, GL_LINE);
	else
		glPolygonMode(GL_BACK, GL_FILL);
	// Записываем состояние матрицы и выполняем пово-рот
	
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	glEnable(GL_CULL_FACE);
	GLfloat l = 25.f;
	// рисуем два треугольника, как основание пирамиды
	glColor3f(1, 0, 0);
	glFrontFace(GL_CCW);
	glBegin(GL_TRIANGLES);
	glVertex3f(l, l, l);
	glVertex3f(-l, l, l);
	glVertex3f(l, l, -l);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-l, l, l);
	glVertex3f(-l, l, -l);
	glVertex3f(l, l, -l);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-l, l, l);
	glVertex3f(-l, -l, l);
	glVertex3f(-l, -l, -l);

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(-l, l, -l);
	glVertex3f(-l, l, l);
	glVertex3f(-l, -l, -l);


	glColor3f(0.5f, .5f, 0.5f);
	glVertex3f(l, l, -l);
	glVertex3f(-l, l, -l);
	glVertex3f(-l, -l, -l);

	

	glEnd();

	glFrontFace(GL_CW);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f(l, -l, l);
	glVertex3f(-l, -l, l);
	glVertex3f(l, -l, -l);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-l, -l, l);
	glVertex3f(-l, -l, -l);
	glVertex3f(l, -l, -l);


	glColor3f(1.0f, .5f, 0.5f);
	glVertex3f(l, l, l);
	glVertex3f(l, -l, l);
	glVertex3f(l, -l, -l);

	glColor3f(0.5f, 1.0f, 0.5f);
	glVertex3f(l, l, -l);
	glVertex3f(l, l, l);
	glVertex3f(l, -l, -l);

	glColor3f(1.f, .5f, 0.f);
	glVertex3f(l, -l, -l);
	glVertex3f(-l, -l, -l);
	glVertex3f(l, l, -l);
	glEnd();
	// Восстанавливаем преобразования
	glPopMatrix();
	// Очищаем стек команд рисования
	glutSwapBuffers();
}

void Render7()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	GLfloat x, y, angle; // Здесь хранятся координаты и углы
	int iPivot = 1; // Используется, чтобы от-мечать
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glFrontFace(GL_CW);
	if (bShadeModel)
		glShadeModel(GL_FLAT);
	else
		glShadeModel(GL_SMOOTH);
	if (bCull)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
	if (bDepth)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
	if (bOutline)
		glPolygonMode(GL_BACK, GL_LINE);
	else
		glPolygonMode(GL_BACK, GL_FILL);
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	glBegin(GL_TRIANGLE_STRIP);
	for (float angle = -M_PI; angle < M_PI; angle += 0.1f)
	{
		//чередуем цвета
		if ((iPivot % 2) == 0)
			glColor3f(1.0f, 0.5f, .5f);
		else
			glColor3f(.6f, 0.3f, .5f);
		x = 50 * cos(angle);
		y = 50 * sin(angle);
		glVertex3f(x, y, 0.f);

		x = 50 * cos(angle + 0.1f);
		y = 50 * sin(angle + 0.1f);
		glVertex3f(x, y, 0.f);

		glVertex3f(x, y, 50.f);
		iPivot++;
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(0.0f, 0.0f);
	for (angle = 0.0f; angle < (2.0f * M_PI)+1; angle += (M_PI / 8.0f))
	{
		x = 50.0f * sin(angle);
		y = 50.0f * cos(angle);
		if ((iPivot % 2) == 0)
			glColor3f(0.0f, 1.0f, 0.0f);
		else
			glColor3f(1.0f, 0.0f, 0.0f);
		iPivot++;
		glVertex2f(x, y);
	}

	glEnd();
	glPopMatrix();
	glutSwapBuffers();
}


void(*renderFuncs[])() = { Render1, Render2, Render3, Render4, Render5, Render6, Render7, Render3 };
void menuHandler(int value)
{
	glutDisplayFunc(renderFuncs[value - 1]);
	glutPostRedisplay();
}

void effectsMenuHandler(int value)
{
	switch (value)
	{
	case 1:
		approxAspect *= 2.f;
		break;
	case 2:
		approxAspect /= 2.f;
		break;
	case 3:
		bCull = !bCull;
		break;
	case 4:
		bDepth = !bDepth;
		break;
	case 5:
		bOutline = !bOutline;
		break;
	case 6:
		bShadeModel = !bShadeModel;
		break;
	}
	glutPostRedisplay();
}

void setup()
{
	// заполнение изначальных псевдослучайных параметров отображения линий и размеров точек, цветов
	GLfloat sizes[2];
	GLfloat step;
	glGetFloatv(GL_POINT_SIZE_RANGE, sizes);
	glGetFloatv(GL_POINT_SIZE_GRANULARITY, &step);
	int i = 0;
	for (double angle = -(2.0f * M_PI) * 3.0f; angle <= (2.0f * M_PI) * 3.0f; angle += 0.1f)
	{
		sizesV.push_back( sizes[0] + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (sizes[1]/4 - sizes[0]))) );
		colorsV.push_back(RGB(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX)), 
			static_cast <float> (rand()) / (static_cast <float> (RAND_MAX)), 
			static_cast <float> (rand()) / (static_cast <float> (RAND_MAX))));
	}
	GLfloat fSizes[2];
	glGetFloatv(GL_LINE_WIDTH_RANGE, fSizes);
	i = 0;
	for (double angle = -(2.0f * M_PI) * 3.0f; angle <= (2.0f * M_PI) * 3.0f; angle += 0.1f)
	{
		sizesLV.push_back(fSizes[0] + i);
		i += 1.f;
	}

	const int count = 100;

	for (int i = 0; i < count; i++)
	{
		float x = -50.f + std::rand() % 101;
		float y = -50.f + std::rand() % 101;
		float z = -50.f + std::rand() % 101;
		if (std::sqrt(std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2) < 50.f * 50.f))
		{
			coords.push_back(std::vector<GLfloat>(3));
			coords[coords.size() - 1][0] = x;
			coords[coords.size() - 1][1] = y;
			coords[coords.size() - 1][2] = z;
		}
	}
}

int main(int argc, char* argv[])
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("Tasks");
	int effectsMenu = glutCreateMenu(effectsMenuHandler);
	glutAddMenuEntry("-10 Ед. аппроксимации", 1);
	glutAddMenuEntry("+10 Ед. аппроксимации", 2);
	glutAddMenuEntry("Cull", 3);
	glutAddMenuEntry("Depth", 4);
	glutAddMenuEntry("Outline", 5);
	glutAddMenuEntry("Shade", 6);
	int menu = glutCreateMenu(menuHandler);
	for (int i = 1; i < 9; i++)
	{
		glutAddMenuEntry((std::string("Задание") + std::to_string(i)).c_str(), i);
	}
	glutAddSubMenu("Effects", effectsMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutReshapeFunc(changeSizeHandler);
	glutDisplayFunc(Render1);
	glutSpecialFunc(SpecialKeys);
	setup();
	glutMainLoop();
	glutDestroyMenu(effectsMenu);
	glutDestroyMenu(menu);
	return 0;
}