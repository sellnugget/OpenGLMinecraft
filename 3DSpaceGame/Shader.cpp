#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "AssetLoader.h"

namespace GL {
    void Shader::CompileShader(const char* VertexPath, const char* FragPath)
    {
        std::string VertexSource = Asset::LoadTextAsset(VertexPath);
        std::string FragSource = Asset::LoadTextAsset(FragPath);
        unsigned int vertexShader;
        unsigned int fragShader;

        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        fragShader = glCreateShader(GL_FRAGMENT_SHADER);

        const char* VS = VertexSource.c_str();
        const char* FS = FragSource.c_str();


        glShaderSource(vertexShader, 1, &VS, NULL);
        glShaderSource(fragShader, 1, &FS, NULL);

        //next compile vertex then fragment shader, printing any errors or warnings

        glCompileShader(vertexShader);
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << VertexPath << " ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
        }

        glCompileShader(fragShader);
        glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
            std::cout << FragPath << " ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << "\n";
        }
        InstanceID = glCreateProgram();

        glAttachShader(InstanceID, vertexShader);
        glAttachShader(InstanceID, fragShader);
        glLinkProgram(InstanceID);

        glGetProgramiv(InstanceID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(InstanceID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << "\n";
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragShader);
    }

    void Shader::LoadMainSource(const char* VertexPath, const char* FragPath)
    {
        VertSourceBuffer = Asset::LoadTextAsset(VertexPath);
        FragSourceBuffer = Asset::LoadTextAsset(FragPath);
    }

    void Shader::AddVertShader(const char* VertPath)
    {
        //first get the source data
        std::string VertSource = Asset::LoadTextAsset(VertPath);

        std::string buffer;

        std::string Target = "void main";

        //first change void main to void main<number>
        for (int i = 0; i < VertSource.size(); i++) {
            buffer += VertSource[i];
            if (buffer == Target.substr(0, buffer.size())) {

                if (buffer == Target) {

                    std::string number = std::to_string(ShaderCounts);
                    for (int x = 0; x < number.size(); x++) {
                        VertSource.insert(VertSource.begin() + i + 1, number[x]);
                    }
                    ShaderCounts++;
                    break;
                }
            }
            else {
                buffer = "";
            }
        }
        //find the last # line in shader
        int LastPound = 0;
        for (int i = 0; i < VertSourceBuffer.size(); i++) {
            if (VertSourceBuffer[i] == '#') {
                for (i; VertSourceBuffer[i] != '\n'; i++) {
                }
                LastPound = i + 1;
            }
        }

        //then add shader toggle

        std::string ToggleName = std::string(VertPath);

        std::size_t Begin = ToggleName.find_last_of('\\');
        std::size_t End = ToggleName.find_last_of(".");
        ToggleName = ToggleName.substr(Begin + 1, End - Begin - 1);


        std::string ShaderToggle = "uniform bool " + ToggleName + ";\n";

        for (int i = 0; i < ShaderToggle.size(); i++) {
            VertSourceBuffer.insert(VertSourceBuffer.begin() + LastPound + i, ShaderToggle[i]);
        }

        //then add the main definition after pound

        std::string ShaderDef = "void main" + std::to_string(ShaderCounts - 1) +  "();\n";
        for (int i = 0; i < ShaderDef.size(); i++) {
            VertSourceBuffer.insert(VertSourceBuffer.begin() + LastPound + i, ShaderDef[i]);
        }
       
        //after that add call to shader
        for (int i = 0; i < VertSourceBuffer.size(); i++) {
            buffer += VertSourceBuffer[i];
            if (buffer == Target.substr(0, buffer.size())) {

                if (buffer == Target) {

                    if (VertSourceBuffer[i + 1] != '(' && VertSourceBuffer[i + 1] != ' ') {
                        continue;
                    }

                    int x = 0;
                    for (x = 0; VertSourceBuffer[x + i] != '{'; x++) {

                    }
                    x++;
                    std::string functioncall = "\n\tif(" + ToggleName + ") {\n\t" + "main" + std::to_string(ShaderCounts - 1) + "(); " + "\n\t}\n";
                    for (int j = 0; j < functioncall.size(); j++) {
                        VertSourceBuffer.insert(VertSourceBuffer.begin() + i + x + j, functioncall[j]);
                    }
                    ShaderCounts++;
                    break;
                }
            }
            else {
                buffer = "";
            }
        }
        //then add shader at the end of shader

        for (int i = 0; i < VertSource.size(); i++) {
            VertSourceBuffer.insert(VertSourceBuffer.end() - 1, VertSource[i]);
        }
       
    }

    void Shader::AddFragShader(const char* FragPath)
    {
    }

    void Shader::CompileBuffers()
    {
        unsigned int vertexShader;
        unsigned int fragShader;

        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        fragShader = glCreateShader(GL_FRAGMENT_SHADER);

        const char* VS = VertSourceBuffer.c_str();
        const char* FS = FragSourceBuffer.c_str();


        glShaderSource(vertexShader, 1, &VS, NULL);
        glShaderSource(fragShader, 1, &FS, NULL);

        //next compile vertex then fragment shader, printing any errors or warnings

        glCompileShader(vertexShader);
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
        }

        glCompileShader(fragShader);
        glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << "\n";
        }
        InstanceID = glCreateProgram();

        glAttachShader(InstanceID, vertexShader);
        glAttachShader(InstanceID, fragShader);
        glLinkProgram(InstanceID);

        glGetProgramiv(InstanceID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(InstanceID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << "\n";
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragShader);


    }

    void Shader::Use()
    {
        glUseProgram(InstanceID);
    }


    void Shader::setUniform(const std::string& name, float value)
    {
        glUseProgram(InstanceID);
        glUniform1f(glGetUniformLocation(InstanceID, name.c_str()), value);
    }

    void Shader::setUniform(const std::string& name, int value)
    {
        glUseProgram(InstanceID);
        glUniform1i(glGetUniformLocation(InstanceID, name.c_str()), value);
    }

    void Shader::setUniform(const std::string& name, bool value)
    {
        glUseProgram(InstanceID);
        glUniform1i(glGetUniformLocation(InstanceID, name.c_str()), value);
    }


    void Shader::setUniform(const std::string& name, glm::vec2 value)
    {
        glUseProgram(InstanceID);
        glUniform2f(glGetUniformLocation(InstanceID, name.c_str()), value.x, value.y);
    }

    void Shader::setUniform(const std::string& name, glm::vec3 value)
    {
        glUseProgram(InstanceID);
        glUniform3f(glGetUniformLocation(InstanceID, name.c_str()), value.x, value.y, value.z);
    }

    void Shader::setUniform(const std::string& name, glm::vec4 value)
    {
        glUseProgram(InstanceID);
        glUniform4f(glGetUniformLocation(InstanceID, name.c_str()), value.x, value.y, value.z, value.w);
    }

    void Shader::setUniform(const std::string& name, glm::mat4 value)
    {
        glUseProgram(InstanceID);
        glUniformMatrix4fv(glGetUniformLocation(InstanceID, name.c_str()), 1, GL_FALSE,glm::value_ptr(value));
    }

}

