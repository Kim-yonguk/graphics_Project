// Defines the entry point for the console application.
//
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <iostream>
#include <string>
#include <fstream>
#include <chrono>

#include "Object.h"
#include "Camera.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "SOIL.h"

void init();
void display();
void reshape(int, int);
void idle();
void keyboard(unsigned char, int, int);
void special(int, int, int);
void mycreatemenu(int value);
void optionmenu(int value);

GLuint		program;

GLint		loc_a_vertex;
GLint       loc_a_normal;

GLint		loc_u_pvm_matrix;
GLint       loc_u_vm_matrix;
GLint       loc_u_normal_matrix;

GLint       loc_u_light_vector_vc;
GLint       loc_u_light_ambient;
GLint       loc_u_light_diffuse;
GLint       loc_u_light_specular;

GLint       loc_u_material_ambient;
GLint       loc_u_material_diffuse;
GLint       loc_u_material_specular;
GLint       loc_u_material_shininess;

GLint       loc_a_texcoord;
GLint       loc_u_texid[2];
GLuint      texid[2];


glm::mat4 mat_PVM, mat_VM;
glm::mat3 mat_Normal;

float angle=0;
float t=0.0f;

glm::vec4 light_vector(10.0f, 10.0f, 10.0f, 0.0f);
glm::vec4 light_ambient(1.0f, 1.0f, 1.0f, 1.0f);
glm::vec4 light_diffuse(1.0f, 1.0f, 1.0f, 1.0f);
glm::vec4 light_specular(1.0f, 1.0f, 1.0f, 1.0f);

glm::vec4 material_ambient(0.393f, 0.464f, 0.094f, 1.0f);
glm::vec4 material_diffuse(0.393f, 0.464f, 0.094f, 1.0f);
glm::vec4 material_specular(1.0f, 1.0f, 1.0f, 1.0f);
float     material_shininess=50.0f;

std::string g_filename1;
std::string g_filename2;
std::string g_filename3;
std::string g_filename4;
std::string g_filename5;
std::string g_filename6;
std::string g_filename7;
std::string g_filename8;

std::string g_mymodel;

Object		 g_model1,g_model2,g_model3,g_model4,g_model5,g_model6,g_model7,g_model8, g_desk, g_fan, g_sofa, g_tv;           // object
Camera		 g_camera;          // viewer (you)

std::chrono::time_point<std::chrono::system_clock> prev, curr;

int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        g_filename1 = argv[2];
    }
    else
    {
        g_filename1 = "./data/Captain.obj";
        //g_filename2 = "./data/megatron.obj";
        g_filename3 = "./data/dead_123456.obj";
        g_filename4 = "./data/Spiderman.obj";
        g_filename5 = "./data/Batman.obj";
        g_filename6 = "./data/Hulk.obj";
        g_filename7="./data/optimus.obj";
        g_filename8="./data/thors_hammerout";
    }


    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(640, 640);
    glutCreateWindow("Modeling & Navigating Your Studio");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);

    glutCreateMenu(mycreatemenu);
    glutAddMenuEntry("Captain",1);
    glutAddMenuEntry("Deadpool",3);
    glutAddMenuEntry("Spiderman",4);
    glutAddMenuEntry("Batman",5);
    glutAddMenuEntry("Hulk",6);
    glutAddMenuEntry("Optimus",7);
    glutAddMenuEntry("Thros",8);
    glutAttachMenu(GLUT_RIGHT_BUTTON);



    glutIdleFunc(idle);

	if (glewInit() != GLEW_OK)
	{
		std::cerr << "failed to initialize glew" << std::endl;
		return -1;
	}

    init();

    glutMainLoop();

    return 0;
}

void init()
{
    g_model1.load_simple_obj(g_filename1);
    //g_model2.load_simple_obj(g_filename2);
    g_model3.load_simple_obj(g_filename3);
    g_model4.load_simple_obj(g_filename4);

    g_model5.load_simple_obj(g_filename5);
    g_model6.load_simple_obj(g_filename6);
    g_model7.load_simple_obj(g_filename7);


    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glFrontFace(GL_CCW);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    // for filled polygon rendering

    // TODO: 1. Implement vertex/fragment shader codes for Phong shading model
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    program = Shader::create_program("./shader/simple.vert", "./shader/simple.frag");


    // TODO: 2. Get locations of shader variables in shader program
	loc_u_pvm_matrix         = glGetUniformLocation(program, "u_pvm_matrix");
    loc_u_vm_matrix          = glGetUniformLocation(program, "u_vm_matrix");
    loc_u_normal_matrix      = glGetUniformLocation(program, "u_normal_matrix");

    loc_u_light_vector_vc    = glGetUniformLocation(program, "u_light_vector_vc");
    loc_u_light_ambient      = glGetUniformLocation(program, "u_light_ambient");
    loc_u_light_diffuse      = glGetUniformLocation(program, "u_light_diffuse");
    loc_u_light_specular     = glGetUniformLocation(program, "u_light_specular");

    loc_u_material_ambient   = glGetUniformLocation(program, "u_material_ambient");
    loc_u_material_diffuse   = glGetUniformLocation(program, "u_material_diffuse");
    loc_u_material_specular  = glGetUniformLocation(program, "u_material_specular");
    loc_u_material_shininess = glGetUniformLocation(program, "u_material_shininess");

	loc_a_vertex             = glGetAttribLocation(program, "a_vertex");
    loc_a_normal             = glGetAttribLocation(program, "a_normal");

    loc_a_texcoord           = glGetAttribLocation(program, "a_texcoord");
    *loc_u_texid              = glGetUniformLocation(program, "u_texid");


    // int width, height, channels;
    // unsigned char* image = SOIL_load_image("optimus.bmp",
    //     &width, &height, &channels, SOIL_LOAD_RGB);
    // // unsigned char* image = SOIL_load_image("tex.jpg",
    // //     &width, &height, &channels, SOIL_LOAD_RGB);
    //
    // // TODO: generate texture
    // glGenTextures(1, &texid[0]);
    //
    // glBindTexture(GL_TEXTURE_2D, texid[0]);
    //
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
    //           GL_RGB, GL_UNSIGNED_BYTE, image);
    //
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
    //
    // SOIL_free_image_data(image);
    //
    //
    // // image1
    // image = SOIL_load_image("OptimusPrime1bytrento.bmp",
    // &width, &height, &channels, SOIL_LOAD_RGB);
    //
    // glGenTextures(1, &texid[1]);
    //
    // glBindTexture(GL_TEXTURE_2D, texid[1]);
    //
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
    //           GL_RGB, GL_UNSIGNED_BYTE, image);
    //
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
    //
    // SOIL_free_image_data(image);
    //
    prev = curr = std::chrono::system_clock::now();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);

	// Camera setting
	glm::mat4   mat_Proj, mat_View, mat_Model;
    glm::mat4   mat_trans, mat_scale, mat_rotate;

	// camera extrinsic param
	mat_View = glm::lookAt(
		g_camera.position(),				// eye position
		g_camera.center_position(), // center position
		g_camera.up_direction()			// up direction
		);

	// camera intrinsic param
	mat_Proj  = glm::perspective(g_camera.fovy(), 1.0f, 0.001f, 10000.0f);

    mat_Model = glm::mat4(1.0);
    mat_trans = glm::translate(glm::mat4(1.0), glm::vec3(0,0,0));
    mat_scale = glm::scale(glm::mat4(1.0), glm::vec3(0.03f));
    mat_rotate = glm::rotate(glm::mat4(1.0), angle-89.5f, glm::vec3(0,1,0));
    mat_Model = mat_trans * mat_rotate * mat_scale * mat_Model ;


    mat_PVM   = mat_Proj*mat_View*mat_Model;

    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, texid[0]);
    // glActiveTexture(GL_TEXTURE1);
    // glBindTexture(GL_TEXTURE_2D, texid[1]);
    //
    // glUniform1i(loc_u_texid[0], 0);
    // glUniform1i(loc_u_texid[1], 1);

    // TODO: 3. Extend Object::draw function to pass variables to the shader program
    mat_VM = mat_View * mat_Model;
    mat_Normal = glm::mat3(mat_VM);

    // glm::vec4 light_vector_vc = mat_View * light_vector;
    glm::vec4 light_vector_vc = light_vector;

    glUniformMatrix4fv(loc_u_pvm_matrix,    1, false, glm::value_ptr(mat_PVM));
    glUniformMatrix4fv(loc_u_vm_matrix,     1, false, glm::value_ptr(mat_VM));
    glUniformMatrix3fv(loc_u_normal_matrix, 1, false, glm::value_ptr(mat_Normal));

    glUniform3fv(loc_u_light_vector_vc,   1, glm::value_ptr(light_vector_vc));
    glUniform4fv(loc_u_light_ambient,     1, glm::value_ptr(light_ambient));
    glUniform4fv(loc_u_light_diffuse,     1, glm::value_ptr(light_diffuse));
    glUniform4fv(loc_u_light_specular,    1, glm::value_ptr(light_specular));

    glUniform4fv(loc_u_material_ambient,  1, glm::value_ptr(material_ambient));
    glUniform4fv(loc_u_material_diffuse,  1, glm::value_ptr(material_diffuse));
    glUniform4fv(loc_u_material_specular, 1, glm::value_ptr(material_specular));
    glUniform1f(loc_u_material_shininess, material_shininess);

    g_model1.draw(loc_a_vertex, loc_a_normal,
    loc_u_material_ambient, loc_u_material_diffuse,
    loc_u_material_specular, loc_u_material_shininess);



    mat_Model = glm::mat4(1.0);
    mat_trans = glm::translate(glm::mat4(1.0), glm::vec3(3,0,0));
    mat_scale = glm::scale(glm::mat4(1.0), glm::vec3(0.01f));
    mat_rotate = glm::rotate(glm::mat4(1.0), angle-360, glm::vec3(0,1,0));
    mat_Model = mat_trans * mat_rotate * mat_scale * mat_Model ;


    mat_PVM   = mat_Proj*mat_View*mat_Model;



    // TODO: 3. Extend Object::draw function to pass variables to the shader program
    mat_VM = mat_View * mat_Model;
    mat_Normal = glm::mat3(mat_VM);

    // glm::vec4 light_vector_vc = mat_View * light_vector;
    light_vector_vc = light_vector;

    glUniformMatrix4fv(loc_u_pvm_matrix,    1, false, glm::value_ptr(mat_PVM));
    glUniformMatrix4fv(loc_u_vm_matrix,     1, false, glm::value_ptr(mat_VM));
    glUniformMatrix3fv(loc_u_normal_matrix, 1, false, glm::value_ptr(mat_Normal));

    glUniform3fv(loc_u_light_vector_vc,   1, glm::value_ptr(light_vector_vc));
    glUniform4fv(loc_u_light_ambient,     1, glm::value_ptr(light_ambient));
    glUniform4fv(loc_u_light_diffuse,     1, glm::value_ptr(light_diffuse));
    glUniform4fv(loc_u_light_specular,    1, glm::value_ptr(light_specular));

    glUniform4fv(loc_u_material_ambient,  1, glm::value_ptr(material_ambient));
    glUniform4fv(loc_u_material_diffuse,  1, glm::value_ptr(material_diffuse));
    glUniform4fv(loc_u_material_specular, 1, glm::value_ptr(material_specular));
    glUniform1f(loc_u_material_shininess, material_shininess);




    g_model3.draw(loc_a_vertex, loc_a_normal,
    loc_u_material_ambient, loc_u_material_diffuse,
    loc_u_material_specular, loc_u_material_shininess);


    mat_Model = glm::mat4(1.0);
    mat_trans = glm::translate(glm::mat4(1.0), glm::vec3(-3,0,0));
    mat_scale = glm::scale(glm::mat4(1.0), glm::vec3(0.03f));
    mat_rotate = glm::rotate(glm::mat4(1.0), angle-89.5f, glm::vec3(0,1,0));
    mat_Model = mat_trans * mat_rotate * mat_scale * mat_Model ;


    mat_PVM   = mat_Proj*mat_View*mat_Model;



    // TODO: 3. Extend Object::draw function to pass variables to the shader program
    mat_VM = mat_View * mat_Model;
    mat_Normal = glm::mat3(mat_VM);

    // glm::vec4 light_vector_vc = mat_View * light_vector;
    light_vector_vc = light_vector;

    glUniformMatrix4fv(loc_u_pvm_matrix,    1, false, glm::value_ptr(mat_PVM));
    glUniformMatrix4fv(loc_u_vm_matrix,     1, false, glm::value_ptr(mat_VM));
    glUniformMatrix3fv(loc_u_normal_matrix, 1, false, glm::value_ptr(mat_Normal));

    glUniform3fv(loc_u_light_vector_vc,   1, glm::value_ptr(light_vector_vc));
    glUniform4fv(loc_u_light_ambient,     1, glm::value_ptr(light_ambient));
    glUniform4fv(loc_u_light_diffuse,     1, glm::value_ptr(light_diffuse));
    glUniform4fv(loc_u_light_specular,    1, glm::value_ptr(light_specular));

    glUniform4fv(loc_u_material_ambient,  1, glm::value_ptr(material_ambient));
    glUniform4fv(loc_u_material_diffuse,  1, glm::value_ptr(material_diffuse));
    glUniform4fv(loc_u_material_specular, 1, glm::value_ptr(material_specular));
    glUniform1f(loc_u_material_shininess, material_shininess);





    g_model4.draw(loc_a_vertex, loc_a_normal,
    loc_u_material_ambient, loc_u_material_diffuse,
    loc_u_material_specular, loc_u_material_shininess);





    mat_Model = glm::mat4(1.0);
    mat_trans = glm::translate(glm::mat4(1.0), glm::vec3(-6,0,0));
    mat_scale = glm::scale(glm::mat4(1.0), glm::vec3(0.013f));
    mat_rotate = glm::rotate(glm::mat4(1.0), angle, glm::vec3(0,1,0));
    mat_Model = mat_trans * mat_rotate * mat_scale * mat_Model ;


    mat_PVM   = mat_Proj*mat_View*mat_Model;



    // TODO: 3. Extend Object::draw function to pass variables to the shader program
    mat_VM = mat_View * mat_Model;
    mat_Normal = glm::mat3(mat_VM);

    // glm::vec4 light_vector_vc = mat_View * light_vector;
    light_vector_vc = light_vector;

    glUniformMatrix4fv(loc_u_pvm_matrix,    1, false, glm::value_ptr(mat_PVM));
    glUniformMatrix4fv(loc_u_vm_matrix,     1, false, glm::value_ptr(mat_VM));
    glUniformMatrix3fv(loc_u_normal_matrix, 1, false, glm::value_ptr(mat_Normal));

    glUniform3fv(loc_u_light_vector_vc,   1, glm::value_ptr(light_vector_vc));
    glUniform4fv(loc_u_light_ambient,     1, glm::value_ptr(light_ambient));
    glUniform4fv(loc_u_light_diffuse,     1, glm::value_ptr(light_diffuse));
    glUniform4fv(loc_u_light_specular,    1, glm::value_ptr(light_specular));

    glUniform4fv(loc_u_material_ambient,  1, glm::value_ptr(material_ambient));
    glUniform4fv(loc_u_material_diffuse,  1, glm::value_ptr(material_diffuse));
    glUniform4fv(loc_u_material_specular, 1, glm::value_ptr(material_specular));
    glUniform1f(loc_u_material_shininess, material_shininess);





    g_model5.draw(loc_a_vertex, loc_a_normal,
    loc_u_material_ambient, loc_u_material_diffuse,
    loc_u_material_specular, loc_u_material_shininess);




    mat_Model = glm::mat4(1.0);
    mat_trans = glm::translate(glm::mat4(1.0), glm::vec3(-1.5f,0,3));
    mat_scale = glm::scale(glm::mat4(1.0), glm::vec3(0.9f));
    mat_rotate = glm::rotate(glm::mat4(1.0), angle, glm::vec3(0,1,0));
    mat_Model = mat_trans * mat_rotate * mat_scale * mat_Model ;


    mat_PVM   = mat_Proj*mat_View*mat_Model;



    // TODO: 3. Extend Object::draw function to pass variables to the shader program
    mat_VM = mat_View * mat_Model;
    mat_Normal = glm::mat3(mat_VM);

    // glm::vec4 light_vector_vc = mat_View * light_vector;
    light_vector_vc = light_vector;

    glUniformMatrix4fv(loc_u_pvm_matrix,    1, false, glm::value_ptr(mat_PVM));
    glUniformMatrix4fv(loc_u_vm_matrix,     1, false, glm::value_ptr(mat_VM));
    glUniformMatrix3fv(loc_u_normal_matrix, 1, false, glm::value_ptr(mat_Normal));

    glUniform3fv(loc_u_light_vector_vc,   1, glm::value_ptr(light_vector_vc));
    glUniform4fv(loc_u_light_ambient,     1, glm::value_ptr(light_ambient));
    glUniform4fv(loc_u_light_diffuse,     1, glm::value_ptr(light_diffuse));
    glUniform4fv(loc_u_light_specular,    1, glm::value_ptr(light_specular));

    glUniform4fv(loc_u_material_ambient,  1, glm::value_ptr(material_ambient));
    glUniform4fv(loc_u_material_diffuse,  1, glm::value_ptr(material_diffuse));
    glUniform4fv(loc_u_material_specular, 1, glm::value_ptr(material_specular));
    glUniform1f(loc_u_material_shininess, material_shininess);





    g_model6.draw(loc_a_vertex, loc_a_normal,
    loc_u_material_ambient, loc_u_material_diffuse,
    loc_u_material_specular, loc_u_material_shininess);




    mat_Model = glm::mat4(1.0);
    mat_trans = glm::translate(glm::mat4(1.0), glm::vec3(0,2.8f,15));
    mat_scale = glm::scale(glm::mat4(1.0), glm::vec3(5.0f));
    mat_rotate = glm::rotate(glm::mat4(1.0), angle+179  , glm::vec3(0,1,0));
    mat_Model = mat_trans * mat_rotate * mat_scale * mat_Model ;


    mat_PVM   = mat_Proj*mat_View*mat_Model;



    // TODO: 3. Extend Object::draw function to pass variables to the shader program
    mat_VM = mat_View * mat_Model;
    mat_Normal = glm::mat3(mat_VM);

    // glm::vec4 light_vector_vc = mat_View * light_vector;
    light_vector_vc = light_vector;

    glUniformMatrix4fv(loc_u_pvm_matrix,    1, false, glm::value_ptr(mat_PVM));
    glUniformMatrix4fv(loc_u_vm_matrix,     1, false, glm::value_ptr(mat_VM));
    glUniformMatrix3fv(loc_u_normal_matrix, 1, false, glm::value_ptr(mat_Normal));

    glUniform3fv(loc_u_light_vector_vc,   1, glm::value_ptr(light_vector_vc));
    glUniform4fv(loc_u_light_ambient,     1, glm::value_ptr(light_ambient));
    glUniform4fv(loc_u_light_diffuse,     1, glm::value_ptr(light_diffuse));
    glUniform4fv(loc_u_light_specular,    1, glm::value_ptr(light_specular));

    glUniform4fv(loc_u_material_ambient,  1, glm::value_ptr(material_ambient));
    glUniform4fv(loc_u_material_diffuse,  1, glm::value_ptr(material_diffuse));
    glUniform4fv(loc_u_material_specular, 1, glm::value_ptr(material_specular));
    glUniform1f(loc_u_material_shininess, material_shininess);





    g_model7.draw(loc_a_vertex, loc_a_normal,
    loc_u_material_ambient, loc_u_material_diffuse,
    loc_u_material_specular, loc_u_material_shininess);

    // mat_Model = glm::translate(glm::mat4(1.0),glm::vec3(0, 5, 0));
    // mat_PVM = mat_Proj * mat_View * mat_Model;
    // glUniformMatrix4fv(loc_u_pvm_matrix,    1, false, glm::value_ptr(mat_PVM));
    // g_fan.draw(loc_a_vertex, loc_a_normal,
    //         loc_u_material_ambient, loc_u_material_diffuse,
    //         loc_u_material_specular, loc_u_material_shininess);






	glUseProgram(0);

	Shader::check_gl_error("draw");

    glutSwapBuffers();
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
}

void keyboard(unsigned char key, int x, int y)
{
	if ('a' == key || 'A' == key)
	{
        g_camera.move_left(0.1);
	}
	else if ('d' == key || 'D' == key)
	{
        g_camera.move_right(0.1);
	}
    else if('w'==key || 'W' == key){

    	g_camera.move_forward(0.1);
    }
    else if('s'==key || 'S' == key){
		g_camera.move_backward(0.1);
    }
    else if('q'==key || 'Q' == key){
		g_camera.move_up(0.1);
    }
    else if('e'==key || 'E' == key){
		g_camera.move_down(0.1);
    }
    glutPostRedisplay();
}

void special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		g_camera.rotate_left(0.1);
		break;
	case GLUT_KEY_RIGHT:
		g_camera.rotate_right(0.1);
		break;

    // case GLUT_KEY_UP:
    //     g_camera.rotate_up(0.01);
    //     break;
    //
    // case GLUT_KEY_DOWN:
    //     g_camera.rotate_down(0.01);
    //     break;


	default:
		break;
	}

	glutPostRedisplay();
}

void mycreatemenu(int value) ///우클릭메뉴 카운트고르기
{
    

    glutPostRedisplay();
}

void optionmenu(int value){

    glutPostRedisplay();
}

void idle()
{
    curr = std::chrono::system_clock::now();

    std::chrono::duration<float> elaped_seconds = (curr - prev);

    //angle += 1* elaped_seconds.count();

    prev = curr;

    glutPostRedisplay();
}
