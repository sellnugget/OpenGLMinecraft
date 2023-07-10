#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Game.h"
int main(void)
{
 
    Game game;

    game.Run();

    glfwTerminate();
}
