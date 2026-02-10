#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "glm/fwd.hpp"
#include "stb_image.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "rect/rect.h"
#include "shader/shader.h"
#include "player/player.h"
#include "animSprite/anim.h"
#include <random>
#include "spriteClass/sprite.h"

float vert[] = {
    -0.5f,  0.5f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.0f,  0.5f, 0.0f,
    -0.5f, -0.5f,  0.0f,  0.0f, 0.5f,
     0.5f, -0.5f,  0.0f,  0.5f, 0.5f
};


unsigned int frag[] = {
    0,1,2,
    1,2,3
};


double getRandomDouble(double min, double max) {
    static std::mt19937 gen{std::random_device{}()};
    std::uniform_real_distribution<double> dis(min, max);
    return dis(gen);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {

    }

}


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);    

    GLFWwindow* window = glfwCreateWindow(1200, 800, "C++ game", NULL, NULL);
    if (window == NULL){std::cout << "ERROR WINDOW CREATE";glfwTerminate();}

    glfwSetWindowPos(window,500,500);
    glfwMakeContextCurrent(window);    

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {   
        std::cout << "ERROR GLAD INIT";
        return -1;
    }

    //stbi_set_flip_vertically_on_load(true);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    
    unsigned int EBO;
    glGenBuffers(1,&EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(frag), frag, GL_STATIC_DRAW);
    

    shaderProgram = CreateShader();
    glUseProgram(shaderProgram);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    initUniforms();

    Sprite FonTexture;
    FonTexture.Texture = CreateTexture("tex/Background/Background2.png");
    FonTexture.rectSize = glm::vec2(2.0,1.0);
    FonTexture.scale = glm::vec2(4.0,2.0);
    unsigned int starTexture = CreateTexture("tex/star.png");

    glfwSetMouseButtonCallback(window, mouse_button_callback);

    //player

    player bird;

    //star

    anim star;
    star.Texture = CreateTexture("tex/star.png");
    star.currentUv = glm::vec2(1.0,1.0);
    star.framesCount = 5;
    star.frameTime = 0.15;
    star.cutX = 5;
    star.cutY = 1;

    //tube

    Sprite tube;
    tube.Texture = CreateTexture("tex/Tiles/PipeStyle1.png");
    tube.cutX = 4;tube.cutY = 1;tube.isCollide = true;
    tube.scale = glm::vec2(0.3,1.5);tube.uvFrame = glm::vec2(1.0,1.0);

    Sprite tube1 = tube;tube1.position = glm::vec2(2.0,-(static_cast <float>(getRandomDouble(1.0,1.35))));
    Sprite tube2 = tube;tube2.position = glm::vec2(3.5,-(static_cast <float>(getRandomDouble(1.0,1.35))));
    Sprite tube3 = tube;tube3.position = glm::vec2(5.0,-(static_cast <float>(getRandomDouble(1.0,1.35))));

    Sprite tubesDown[] = {tube1, tube2, tube3};

    tube.angle = 180;
    Sprite tube4 = tube;tube4.position = glm::vec2(2.0,(static_cast <float>(getRandomDouble(1.0,1.35))));
    Sprite tube5 = tube;tube5.position = glm::vec2(3.5,(static_cast <float>(getRandomDouble(1.0,1.35))));
    Sprite tube6 = tube;tube6.position = glm::vec2(5.0,(static_cast <float>(getRandomDouble(1.0,1.35))));

    Sprite tubesUp[] = {tube4, tube5, tube6};

    //while
    float lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {   
        //delta time
        float now = glfwGetTime();
        float deltaTime = now - lastTime;
        lastTime = now;

        //exit
        if (glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }
        

        //Draw Object
        glClearColor(30.0f/255, 28.0f/255, 52.0f/255, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        areaCollide.clear();

        //fon
        FonTexture.draw();

        areaCollide.clear();
        
        //tube
        for (int i =0;i<3;i++)
        {   
            float &tubePosX = tubesDown[i].position.x;
            float &tubePosY = tubesDown[i].position.y;
            if (tubePosX > -2.0f) {if (bird.active) tubePosX -= 0.02f;} else {tubePosX = 2.5f;tubePosY = -getRandomDouble(1.0,1.35);}
            tubesDown[i].draw();
        }
        for (int i =0;i<3;i++)
        {   
            float &tubePosX = tubesUp[i].position.x;
            float &tubePosY = tubesUp[i].position.y;
            if (tubePosX > -2.0f) {if (bird.active) tubePosX -= 0.02f;} else {tubePosX = 2.5f;tubePosY = getRandomDouble(1.0,1.35);}
            tubesUp[i].draw();
        }

        //player
        bird.move(window,deltaTime);

        //stars
        star.play(deltaTime, glm::vec2(0.2), glm::vec2(0.0), 0, false);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    
    return 0;
}
