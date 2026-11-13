#include"VBO.h"
#include<vector>

// Constructor that generates a Verrtex Buffer Object and llinks it to vertices
VBO::VBO(const std::vector<Vertex>& verteices)
{
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, verteices.size() * sizeof(Vertex), verteices.data(), GL_STATIC_DRAW);
}

// Binds the VBO
void VBO::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

// Unbinds the VBO
void VBO::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Deletes the VBO
void VBO::Delete()
{
    glDeleteBuffers(1, &ID);
}