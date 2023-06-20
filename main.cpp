#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}




static unsigned int compileShader(unsigned int type, const std::string& source)
{
  unsigned int id = glCreateShader(type);
  const char* src = source.c_str();
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  // TODO: Error Handling
  int result = 0;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE)
  {
    int length = 0;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char* message = (char*)alloca(length * sizeof(char));
    glGetShaderInfoLog(id, length, &length, message);
    std::cout << "Failed to compile shader!" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
    std::cout << message << std::endl;
    glDeleteShader(id);
    return 0;

  }

  return id;
}


static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
  unsigned int program = glCreateProgram();
  unsigned int vs = compileShader ( GL_VERTEX_SHADER, vertexShader );
  unsigned int fs = compileShader ( GL_FRAGMENT_SHADER, fragmentShader );

  glAttachShader ( program, vs );
  glAttachShader ( program, fs );
  glLinkProgram ( program );
  glValidateProgram ( program );

  glDeleteShader ( vs );
  glDeleteShader ( fs );

  return program;
}

int main(void) {
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);

    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    // in documentation it is stated that glewInit must come after glfwMakeContextCurrent in order for GLEW to work
    if (glewInit() != GLEW_OK) {
      std::cout << "Error, glewInit is being weird" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    glfwSetKeyCallback(window, key_callback);



    //
    unsigned int buff1;
    glGenBuffers(1, &buff1);
    glBindBuffer(GL_ARRAY_BUFFER, buff1);

    float positions[6] = {
      -0.5f, -0.5f,
       0.0f,  0.5f,
       0.5f, -0.5f
    };

    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    glEnableVertexAttribArray(0);


    std::string vertexShader =
      "#version 330 core\n"
      "\n"
      "layout(location = 0) in vec4 position;"
      "\n"
      "void main()"
      "{\n"
      "   gl_Position = position;\n"
      "}\n";

    std::string fragmentShader =
      "#version 330 core\n"
      "\n"
      "layout(location = 0) out vec4 color;"
      "\n"
      "void main()"
      "{\n"
      "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
      "}\n";

    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);




    while (!glfwWindowShouldClose(window)) {
      /*
        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
        glBegin(GL_TRIANGLES);
        glColor3f(1.f, 0.f, 0.f);
        glVertex3f(-0.6f, -0.4f, 0.f);
        glColor3f(0.f, 1.f, 0.f);
        glVertex3f(0.6f, -0.4f, 0.f);
        glColor3f(0.f, 0.f, 1.f);
        glVertex3f(0.f, 0.6f, 0.f);
        glEnd();
        glfwSwapBuffers(window);
        glfwPollEvents();
        */


        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays( GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteShader(shader);
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
