#include "F_Shader.h"

F_Shader::F_Shader()
{
	m_shader = Shader();
}

F_Shader::F_Shader(const Shader& shader)
{
	m_shader = shader;
}

void F_Shader::LoadFromFile(const char* vs,const char* fs)
{
	m_shader = LoadShader(vs, fs);
}
void F_Shader::LoadFromCode(const char* vs, const char* fs) {
	m_shader = LoadShaderFromMemory(vs, fs);
}

void F_Shader::SetValue(const char* name,const void* value, int type)
{
	SetShaderValue(m_shader, GetLoc( name), value, type);
}

void F_Shader::SetValueTexture(const char* name, Texture texture)
{
	SetShaderValueTexture(m_shader, GetLoc(name), texture);
}

void F_Shader::SetrValueMatrix(const char* name, Matrix mat)
{
	SetShaderValueMatrix(m_shader, GetLoc(name),mat);
}

int F_Shader::GetLoc(const char* name)
{
	return GetShaderLocation(m_shader, name);
}

void F_Shader::Unload()
{
	UnloadShader(m_shader);
}

void F_Shader::Begin()
{
	BeginShaderMode(m_shader);
}

void F_Shader::End()
{
	EndShaderMode();
}

