#version 460
#extension GL_EXT_nonuniform_qualifier : enable
precision mediump float;

#define MAX_MATRICES 1000

layout(location = 0) in vec4 i_pos;
layout(location = 1) in float texIndex;

layout(location = 2) out vec2 o_uv;
layout(location = 3) out float outTexIndex;

struct Matrix
{
    mat4 model;
    mat4 view;
    mat4 projection;
};

layout(set = 0, binding = 0) uniform uniformMatrix
{
    Matrix matrix[MAX_MATRICES];
};

void main()
{
    o_uv.x = ((i_pos.x + 1) / 2);
    o_uv.y = ((i_pos.y + 1) / 2);
    outTexIndex = texIndex;

    gl_Position =  matrix[int(texIndex)].model * i_pos;
}