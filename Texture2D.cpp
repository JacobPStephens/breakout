#include <GLFW/glfw3.h>
#include "Texture2D.h"

Texture2D::Texture2D() {
    glGenTextures(1, &this->ID);
}