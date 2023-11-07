#pragma once
#include <glad/glad.h>

class IndexBuffer
{
    public:
        IndexBuffer(const GLuint* data, GLuint count);
        ~IndexBuffer();

        void Bind() const;
        void Unbind() const;

        inline GLuint GetCount() const { return m_Count; }

    private:
        GLuint m_RendererID;
        GLuint m_Count;

};
