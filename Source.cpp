#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GLuint program;
GLint attribute_coord2d;

const char *vs_source =
"#version 410\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec4 color;\n"
"out vec4 ourColor;\n"
"uniform mat4 model;"
"uniform mat4 view; "
"uniform mat4 projection;"
"void main(void){"
"gl_Position = projection * view * model *vec4(position, 1.0);\n"
"ourColor = color;\n"
"}";

const char *fs_cource =
"#version 410\n"
"in vec4 ourColor;\n"
"out vec4 color;\n"
"void main(void){"
"color = vec4(ourColor);"
"}";

int init_resources()
{
	
	GLint compile_ok = GL_FALSE, link_ok = GL_FALSE;

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vs, 1, &vs_source, NULL);
	glCompileShader(vs);
	glGetShaderiv(vs, GL_COMPILE_STATUS, &compile_ok);
	if (compile_ok == GL_FALSE)
	{

		GLint infoLogLength;
		glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(vs, infoLogLength, NULL, strInfoLog);

		fprintf(stderr, "Compile failure in shader:\n%s\n", strInfoLog);
		delete[] strInfoLog;
		fprintf(stderr, "error in vertex error\n");
		return 0;
	}

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fs, 1, &fs_cource, NULL);
	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &compile_ok);
	if (compile_ok == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(fs, infoLogLength, NULL, strInfoLog);

		fprintf(stderr, "Compile failure in shader:\n%s\n", strInfoLog);
		delete[] strInfoLog;

		fprintf(stderr, "error in fragment shader\n");
		return 0;
	}

	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		fprintf(stderr, "glinkprogram\n");
		return 0;
	}

	const char* attribute_name = "position";
	attribute_coord2d = glGetAttribLocation(program, attribute_name);
	if (attribute_coord2d == -1)
	{
		fprintf(stderr, "cannot bind attribute");
		return 0;
	}

	return 1;
}

void onDisplay()
{	
	
	glm::vec3 cubePositions[] = {
		glm::vec3(-1.0f, -0.2f, -0.3f),
		glm::vec3(0.5f, -0.4f, 1.2f)		
	};
	

	/*
	GLfloat points[] = {
	0.0, 0.8, 1.5,
	0.8, -0.8, 0.0,
	-0.8, -0.8, 0.0,
	};
	*/
	// cube ///////////////////////////////////////////////////////////////////////
	//    v6----- v5
	//   /|      /|
	//  v1------v0|
	//  | |     | |
	//  | |v7---|-|v4
	//  |/      |/
	//  v2------v3
	//glDrawArrays����cube
	
	GLfloat points[] = {
		
		/*0.5, 0.5, 0.5, 1.0f, 0.0f, 0.0f, 0.0f, -0.5, 0.5, 0.5, 0.0f, 1.0f, 0.0f, 0.0f, -0.5, -0.5, 0.5, 0.0f, 0.0f, 1.0f, 0.0f,    // v0-v1-v2 (front)
		-0.5, -0.5, 0.5, 0.0f, 0.0f, 1.0f, 0.0f, 0.5, -0.5, 0.5, 0.0f, 0.0f, 1.0f, 0.0f, 0.5, 0.5, 0.5, 1.0f, 0.0f, 0.0f, 0.0f,     // v2-v3-v0

		0.5, 0.5, 0.5, 1.0f, 0.0f, 0.0f, 0.0f, 0.5, -0.5, 0.5, 0.0f, 0.0f, 1.0f, 0.0f, 0.5, -0.5, -0.5, 1.0f, 1.0f, 0.0f, 0.0f,    // v0-v3-v4 (right)
		0.5, -0.5, -0.5, 1.0f, 1.0f, 0.0f, 0.0f, 0.5, 0.5, -0.5, 0.0f, 1.0f, 1.0f, 0.0f, 0.5, 0.5, 0.5, 1.0f, 0.0f, 0.0f, 0.0f,       // v4-v5-v0
		
		0.5, 0.5, 0.5, 1.0f, 0.0f, 0.0f, 0.0f, 0.5, 0.5, -0.5, 0.0f, 1.0f, 1.0f, 0.0f, -0.5, 0.5, -0.5, 1.0f, 0.5f, 1.0f, 0.0f,      // v0-v5-v6 (top)
		-0.5, 0.5, -0.5, 1.0f, 0.5f, 1.0f, 0.0f, -0.5, 0.5, 0.5, 0.0f, 1.0f, 0.0f, 0.0f, 0.5, 0.5, 0.5, 1.0f, 0.0f, 0.0f, 0.0f,      // v6-v1-v0
		
		-0.5, 0.5, 0.5, 0.0f, 1.0f, 0.0f, 0.0f, -0.5, 0.5, -0.5, 1.0f, 0.5f, 1.0f, 0.0f, -0.5, -0.5, -0.5, 0.5f, 0.0f, 1.0f, 0.0f,       // v1-v6-v7 (left)
		-0.5, -0.5, -0.5, 0.5f, 0.0f, 1.0f, 0.0f, -0.5, -0.5, 0.5, 0.0f, 0.0f, 1.0f, 0.0f, -0.5, 0.5, 0.5, 0.0f, 1.0f, 0.0f, 0.0f,     // v7-v2-v1
		
		-0.5, -0.5, -0.5, 0.5f, 0.0f, 1.0f, 0.0f,	0.5, -0.5, -0.5, 1.0f, 1.0f, 0.0f, 0.0f,	 0.5, -0.5, 0.5, 0.0f, 0.0f, 1.0f, 0.0f,     // v7-v4-v3 (bottom)
		0.5, -0.5, 0.5, 0.0f, 0.0f, 1.0f, 0.0f, -0.5, -0.5, 0.5, 0.0f, 0.0f, 1.0f, 0.0f, -0.5,		-0.5, -0.5, 0.5f, 0.0f, 1.0f, 0.0f,       // v3-v2-v7
		
		-0.5, -0.5, -0.5, 0.5f, 0.0f, 1.0f, 0.0f, 0.5, -0.5, -0.5, 1.0f, 1.0f, 0.0f, 0.0f, 0.5, 0.5, -0.5, 0.0f, 1.0f, 1.0f, 0.0f,     // V7-v4-v5 (back)
		0.5, 0.5, -0.5, 0.0f, 1.0f, 1.0f, 0.0f, -0.5, 0.5, -0.5, 1.0f, 0.5f, 1.0f, 0.0f, -0.5, -0.5, -0.5, 0.5f, 0.0f, 1.0f, 0.0f	// v5-v6-v7
		*/ 
		 0.5, 0.5, 0.5,    1.0f, 0.1f, 0.2f,1.0f,	//0
		 -0.5, 0.5, 0.5, 0.0f, 1.0f, 0.0f, 1.0f,	//1            
		 -0.5, -0.5, 0.5, 0.0f, 0.0f, 1.0f, 1.0f,	//2           
		 0.5, -0.5, 0.5, 0.0f, 0.0f, 1.0f, 1.0f,	//3           
		 0.5, -0.5, -0.5, 1.0f, 1.0f, 0.0f, 1.0f,	//4           
		 0.5, 0.5, -0.5, 0.0f, 1.0f, 1.0f, 1.0f,	//5           
		 -0.5, 0.5, -0.5, 1.0f, 0.5f, 1.0f, 1.0f,	//6         
		 -0.5, -0.5, -0.5, 0.5f, 0.0f, 1.0f, 1.0f//7            
		
	};
	GLfloat points1[] = { 
		0.5, 0.5, 0.5, 1.0f, 0.1f, 0.2f, 0.5f,	//0
		-0.5, 0.5, 0.5, 0.0f, 1.0f, 0.0f, 0.5f,	//1            
		-0.5, -0.5, 0.5, 0.0f, 0.0f, 1.0f, 0.5f,	//2           
		0.5, -0.5, 0.5, 0.0f, 0.0f, 1.0f, 0.5f,	//3           
		0.5, -0.5, -0.5, 1.0f, 1.0f, 0.0f, 0.5f,	//4           
		0.5, 0.5, -0.5, 0.0f, 1.0f, 1.0f, 0.5f,	//5           
		-0.5, 0.5, -0.5, 1.0f, 0.5f, 1.0f, 0.5f,	//6         
		-0.5, -0.5, -0.5, 0.5f, 0.0f, 1.0f, 0.5f//7       
	};
	GLuint indices[] = {
		0, 1, 2, 2, 3, 0,
		0, 3, 4, 4, 5, 0,
		0, 5, 6, 6, 1, 0,
		1, 6, 7, 7, 2, 1,
		7, 4, 3, 3, 2, 7,
		7, 4, 5, 5, 6, 7
	};
	GLuint indices1[] = {
		0, 1, 2, 2, 3, 0,
		0, 3, 4, 4, 5, 0,
		0, 5, 6, 6, 1, 0,
		1, 6, 7, 7, 2, 1,
		7, 4, 3, 3, 2, 7,
		7, 4, 5, 5, 6, 7
	};

//������һ��cube�Ļ�����
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);//����VBO(���㻺��������)��Ҫ3�����裺1.ʹ��glGenBuffers()�����»������2.ʹ��glBindBuffer()�󶨻������3.ʹ��glBufferData()���������ݿ�������������С�
	glBindBuffer(GL_ARRAY_BUFFER, vbo);//��������������
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);//����size���洢��λ��ͨ�����ֽڣ���OpenGL�������ڴ棬���ڴ洢�������ݻ���������ǰ�����뵱ǰ�󶨶�������������ݶ���ɾ��
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);//�ٵ�buffer��һ���״�ʹ�õķ����޷�������ʱ�����ʹ���һ���µĻ��������󣬲���buffer��������������������Ϊ�������ơ��ڵ��󶨵�һ����ǰ�����Ļ���������ʱ�����������������Ϊ��Ļ��������󡣢۵��󶨵�һ��ֵΪ���bufferʱ��OpenGL�ͻ�ֹͣʹ�û���������
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7* sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindVertexArray(0);
//�����ڶ���cube�Ļ�����	
	GLuint cubevbo = 0;
	glGenBuffers(1, &cubevbo);//����VBO(���㻺��������)��Ҫ3�����裺1.ʹ��glGenBuffers()�����»������2.ʹ��glBindBuffer()�󶨻������3.ʹ��glBufferData()���������ݿ�������������С�
	glBindBuffer(GL_ARRAY_BUFFER, cubevbo);//��������������
	glBufferData(GL_ARRAY_BUFFER, sizeof(points1), points1, GL_STATIC_DRAW);//����size���洢��λ��ͨ�����ֽڣ���OpenGL�������ڴ棬���ڴ洢�������ݻ���������ǰ�����뵱ǰ�󶨶�������������ݶ���ɾ��
	GLuint cubevao = 0;
	glGenVertexArrays(1, &cubevao);
	glBindVertexArray(cubevao);
	glBindBuffer(GL_ARRAY_BUFFER, cubevbo);//�ٵ�buffer��һ���״�ʹ�õķ����޷�������ʱ�����ʹ���һ���µĻ��������󣬲���buffer��������������������Ϊ�������ơ��ڵ��󶨵�һ����ǰ�����Ļ���������ʱ�����������������Ϊ��Ļ��������󡣢۵��󶨵�һ��ֵΪ���bufferʱ��OpenGL�ͻ�ֹͣʹ�û���������
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindVertexArray(0);

	
	
	//����һ��cube
	glBindVertexArray(vao);
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	model = glm::rotate(model, (GLfloat)glfwGetTime() * 50.0f, glm::vec3(0.3f, 1.0f, 0.0f));
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	projection = glm::perspective(45.0f, (GLfloat)640 / (GLfloat)480, 0.1f, 100.0f);
	GLint modelLoc = glGetUniformLocation(program, "model");
	GLint viewLoc = glGetUniformLocation(program, "view");
	GLint projLoc = glGetUniformLocation(program, "projection");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, indices);
	glBindVertexArray(0);
	//���ڶ���cube
	glBindVertexArray(cubevao);
	
	for (GLuint i = 0; i < 2; i++)
	{
		glm::mat4 model;
		model = glm::translate(model, cubePositions[i]);
		GLfloat angle = 20.0f * i;
		//model = glm::rotate(model, -(GLfloat)glfwGetTime() * 1.0f, glm::vec3(0.3f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, indices1);
	}
	
	
	
	glBindVertexArray(0);
	glUseProgram(program);
	//glDrawArrays(GL_TRIANGLES, 0, 36);

	
	
	
}
void free_resources()
{
	glDeleteProgram(program);
}

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
int main(void)
{



	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		//      throw std::runtime_error("glew fails to start.");
		fprintf(stderr, "glew error\n");
	}

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	//��Ȳ���
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	
	//���
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//������
	glEnable(GL_CULL_FACE);
	//ǰ������
//	glCullFace(GL_FRONT);
	//��������
	//glCullFace(GL_BACK);
	glfwSetKeyCallback(window, key_callback);

	GLint maxV;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxV);
	fprintf(stderr, "maxv: %d\n", maxV);

	init_resources();

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		onDisplay();
		

		glfwSwapBuffers(window);
		

	}
	free_resources();

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
