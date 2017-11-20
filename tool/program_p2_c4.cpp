#include "program_p2_c4.h"

PROGRAM_P2_C4::PROGRAM_P2_C4()
{
    _pos        =   -1;
    _mvp        =   -1;
    _uv         =   -1;
    _texture    =   -1;
    _offset     =   -1;
}

bool PROGRAM_P2_C4::initialize()
{
#if !USE_SHADER
    const char* vs  =
    {

        "uniform    mat4    _mvp;"
        "attribute  vec3    _pos;"
        "attribute  vec2    _uv;"
        "varying    vec2    _outUV;"
        "void main()"
        "{"
        "   _outUV      = _uv;"
        "   gl_Position = _mvp * vec4(_pos.x,_pos.y,_pos.z,1.0);"
        "}"
    };
    const char* ps  =
    {
        "precision  mediump     float;"
        "uniform    sampler2D   _texture;"
        "uniform    vec2        _offset;"
        "varying    vec2        _outUV;"
        "void main()"
        "{"
        "  vec4 texColor    =   texture2D( _texture,_outUV + _offset);"
        "  gl_FragColor     =   texColor;"
        "}"
    };
#else

    const char* vs  =
    {

        "uniform    mat4    _mvp;"
        "uniform    vec3    _offset;"
        "attribute  vec3    _pos;"
        "attribute  vec2    _uv;"
        "varying    vec2    _outUV;"
        "void main()"
        "{"
        "   float us    = 1.0/_offset.x;"
        "   float vs    = 1.0/_offset.y;"
        "   int col     = int(_offset.z)%int(_offset.x);"
        "   int row     = int(_offset.z)/int(_offset.y);"
        "   _outUV      = vec2(_uv.x * us,_uv.y * vs);"
        "   _outUV.x    +=  float(col) * us;"
        "   _outUV.y    +=  float(row) * vs;"
        "   gl_Position = _mvp * vec4(_pos.x,_pos.y,_pos.z,1.0);"
        "}"
    };
    const char* ps  =
    {
        "precision  mediump     float;"
        "uniform    sampler2D   _texture;"
        "varying    vec2        _outUV;"
        "void main()"
        "{"
        "  vec4 texColor    =   texture2D( _texture,_outUV);"
        "  gl_FragColor     =   texColor;"
        "}"
    };
#endif

    bool    res =   createProgram(vs,ps);
    if(res)
    {
        _pos    =   glGetAttribLocation ( _programId, "_pos" );
        _uv     =   glGetAttribLocation ( _programId, "_uv" );

        // Get the uniform locations
        _mvp    =   glGetUniformLocation ( _programId, "_mvp" );
        _texture=   glGetUniformLocation ( _programId, "_texture" );
        _offset =   glGetUniformLocation ( _programId, "_offset" );

    }
    return  res;
}

void PROGRAM_P2_C4::begin()
{
    glUseProgram(_programId);
    glEnableVertexAttribArray ( _pos );
    glEnableVertexAttribArray ( _uv );
}

void PROGRAM_P2_C4::end()
{
    glDisableVertexAttribArray ( _pos );
    glDisableVertexAttribArray ( _uv );
    glUseProgram(0);
}
