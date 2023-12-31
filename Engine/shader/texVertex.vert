#version 460
#extension GL_EXT_nonuniform_qualifier : enable
precision mediump float;

#define MAX_MATRICES 500

layout(location = 0) in vec4 i_pos;
layout(location = 1) in vec4 i_col;
layout(location = 2) in int index;
layout(location = 3) in float isTex;

layout(location = 0) out vec2 o_uv;
layout(location = 1) out vec4 o_col;
layout(location = 2) out int outTexIndex;
layout(location = 3) out float outIsTex;

struct Matrix
{
    mat4 model;
    mat4 view;
    mat4 projection;
    int texIndex;
};

layout(set = 0, binding = 0) uniform uniformMatrix
{
    Matrix matrix[MAX_MATRICES];
};

void main()
{
    o_uv.x = ((i_pos.x + 1) / 2);
    o_uv.y = ((i_pos.y + 1) / 2);

    outTexIndex = matrix[index].texIndex;
    outIsTex = isTex;
    o_col = i_col;

    gl_Position = matrix[index].projection * matrix[index].view * matrix[index].model * i_pos;
}