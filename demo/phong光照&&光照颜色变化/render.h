#pragma once
//=====================================================================
// 渲染设备
//=====================================================================
#ifndef render_h
#define render_h
#include "s_math.h"
#include "rasterize.h"
#include<iostream>
#include<vector>
using namespace std;
typedef struct
{
	s_vector lightpos;
	s_vector lightcolor;
	
	s_vector ambient;
	s_vector diffuse;
	s_vector specular;
}point_light;
typedef struct
{
	s_vector viewpos;
}s_camera;

typedef struct
{
	s_vector ambient;
	s_vector diffuse;
	s_vector specular;
	float shininess;
}s_material;
typedef struct
{
	s_transform transform;   // 坐标变换器
	int width;               // 窗口宽度
	int height;              // 窗口高度
	IUINT32** framebuffer;   // 像素缓存：framebuffer[y] 代表第 y行
	float** zbuffer;         // 深度缓存：zbuffer[y] 为第 y行指针
	IUINT32** texture;       // 纹理：同样是每行索引
	int tex_width;           // 纹理宽度
	int tex_height;          // 纹理高度
	float max_u;             // 纹理最大宽度：tex_width - 1
	float max_v;             // 纹理最大高度：tex_height - 1
	int render_state;        // 渲染状态
	IUINT32 background;      // 背景颜色
	IUINT32 foreground;      // 线框颜色
	point_light pointlight[20];
	s_material material[100];
	s_camera camera;

}device_t;

#define RENDER_STATE_WIREFRAME 1  //渲染线框 
#define RENDER_STATE_TEXTURE    2  //渲染纹理 
#define RENDER_STATE_COLOR      4  //渲染颜色
//设备初始化，fb为外部帧缓存，非NULL将引用外部帧缓存 
void device_init(device_t* device, int width, int height, void* fb);

// 删除设备
void device_destory(device_t* device);
//设置当前纹理 
void device_set_texture(device_t* device, void* bits, long pitch, int w, int h);

//设置点光源
void device_set_pointlight(device_t* device, s_vector& pos, s_vector& color,s_vector& am,s_vector& di,s_vector& spe,int cnt);

//设置材质

void device_set_material(device_t* device, s_vector& am, s_vector& di,s_vector& spe, float shi,int cnt);
// 清空 framebuffer 和 zbuffer
void device_clear(device_t* device, int mode);

void device_pixel(device_t* device, int x, int y, IUINT32 color);

void device_draw_line(device_t* device, int x1, int y1, int x2, int y2, IUINT32 c);

// 根据坐标读取纹理
IUINT32 device_texture_read(const device_t* device, float u, float v);

//=====================================================================
// 渲染实现
//=====================================================================

// 绘制扫描线
void device_draw_scanline(device_t* device, scanline_t* scanline,int count);

//主渲染函数 
void device_render_trap(device_t* device, trapezoid_t* trap,int count);

// 根据 render_state 绘制原始三角形
void device_draw_primitive(device_t* device, vertex_t* v1,
	vertex_t* v2, vertex_t* v3,int count);

typedef struct
{
	s_vector pos;//坐标 
	s_color color;// 颜色 
	s_vector normal; //法向量 
	s_vector tangent;// 切线 
	s_vector binormal;//副法向量 
	s_vector2f texcoord;//纹理坐标 
}for_vs;

typedef struct
{
	s_vector pos;//坐标 
	s_vector2f texcoord;//纹理坐标 
	s_color color; //颜色 
	s_vector normal;//法向量 
	s_vector storage0;// 插值寄存器
	s_vector storage1;
	s_vector storage2;
}for_fs;

void v_shader(device_t* device, for_vs* vv, for_fs* ff);

void f_shader(device_t* device, for_fs* ff, s_color& color,int count);

#endif
