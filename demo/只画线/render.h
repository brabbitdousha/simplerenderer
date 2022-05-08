#pragma once
//=====================================================================
// ��Ⱦ�豸
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
	IUINT32** texture;
	int tex_width;           // ������
	int tex_height;          // ����߶�
	float max_u;             // ��������ȣ�tex_width - 1
	float max_v;             // �������߶ȣ�tex_height - 1
}s_texture;

typedef struct
{
	float shininess;
	s_texture diffuse_texture;
	s_texture specular_texture;
	s_texture normal_texture;
	bool have_diffuse;
	bool have_specular;
	bool have_normal;
}s_material;
typedef struct
{
	float shininess;
	s_vector ambient;
	s_vector diffuse;
	s_vector specular;
}v_material;

typedef struct
{
	s_vector albedo;
    float metallic;
	float roughness;
	float ao;
}s_PBR;
typedef struct
{
	s_texture albedo_texture;
	s_texture metallic_texture;
	s_texture roughness_texture;
	s_texture ao_texture;
}t_PBR;
typedef struct
{
	s_transform transform;   // ����任��
	int width;               // ���ڿ��
	int height;              // ���ڸ߶�
	IUINT32** framebuffer;   // ���ػ��棺framebuffer[y] ����� y��
	float** zbuffer;         // ��Ȼ��棺zbuffer[y] Ϊ�� y��ָ��
	IUINT32** texture;       // ����ͬ����ÿ������
	int tex_width;           // ������
	int tex_height;          // ����߶�
	float max_u;             // ��������ȣ�tex_width - 1
	float max_v;             // �������߶ȣ�tex_height - 1
	int render_state;        // ��Ⱦ״̬
	IUINT32 background;      // ������ɫ
	IUINT32 foreground;      // �߿���ɫ
	point_light pointlight[20];
	s_material material[100];
	v_material vmaterial[100];
	int v_m_num;
	s_camera camera;
	IUINT32** texture_di;
	IUINT32** texture_spe;
	int is_cull;
	s_PBR PBR;
	t_PBR tPBR[30];//texture PBR
	int now_state;
	float m1;
	float m2;

}device_t;

#define RENDER_STATE_WIREFRAME 1  //��Ⱦ�߿� 
#define RENDER_STATE_TEXTURE    2  //��Ⱦ���� 
#define RENDER_STATE_COLOR      4  //��Ⱦ��ɫ
//�豸��ʼ����fbΪ�ⲿ֡���棬��NULL�������ⲿ֡���� 
void device_init(device_t* device, int width, int height, void* fb);

// ɾ���豸
void device_destory(device_t* device);
//���õ�ǰ���� 
void device_set_texture(device_t* device, void* bits, long pitch, int w, int h);

void device_set_texture_by_photo(device_t* device, IUINT32** texture, long pitch, int w, int h);
//���ò��ʵ���������ͼ
void device_set_texture_by_diffuse(device_t* device, IUINT32** texture, long pitch, int w, int h, int count);
//���þ���߹���ͼ
void device_set_texture_by_specular(device_t* device, IUINT32** texture, long pitch, int w, int h, int count);

void device_set_texture_by_normal(device_t* device, IUINT32** texture, long pitch, int w, int h, int count);

void device_set_texture_by_albedo(device_t* device, IUINT32** texture, long pitch, int w, int h, int count);

void device_set_texture_by_metallic(device_t* device, IUINT32** texture, long pitch, int w, int h, int count);

void device_set_texture_by_roughness(device_t* device, IUINT32** texture, long pitch, int w, int h, int count);

void device_set_texture_by_ao(device_t* device, IUINT32** texture, long pitch, int w, int h, int count);

//���õ��Դ
void device_set_pointlight(device_t* device, s_vector& pos, s_vector& color,s_vector& am,s_vector& di,s_vector& spe,int cnt);

void device_set_pointlight(device_t* device, s_vector& pos, s_vector& color, int cnt);

//���ò���

//void device_set_material(device_t* device, s_vector& am, s_vector& di,s_vector& spe, float shi,int cnt);
// ��� framebuffer �� zbuffer
void device_clear(device_t* device, int mode);

void device_pixel(device_t* device, int x, int y, IUINT32 color);

void device_draw_line(device_t* device, int x1, int y1, int x2, int y2, IUINT32 c);

// ���������ȡ����
IUINT32 device_texture_read(const device_t* device, float u, float v);

void read_the_texture(s_vector& tmp, const  s_texture* t_texture, float u, float v);

void read_the_texture_dir(s_vector& tmp, const  s_texture* t_texture, int x, int y);


//=====================================================================
// ��Ⱦʵ��
//=====================================================================

// ����ɨ����
void device_draw_scanline(device_t* device, scanline_t* scanline,int count);

//����Ⱦ���� 
void device_render_trap(device_t* device, trapezoid_t* trap,int count);

// ���� render_state ����ԭʼ������
void device_draw_primitive(device_t* device, vertex_t* v1,
	vertex_t* v2, vertex_t* v3,int count);

typedef struct
{
	s_vector pos;//���� 
	s_color color;// ��ɫ 
	s_vector normal; //������ 
	s_vector tangent;// ���� 
	s_vector binormal;//�������� 
	s_vector2f texcoord;//�������� 
}for_vs;

typedef struct
{
	s_vector pos;//���� 
	s_vector2f texcoord;//�������� 
	s_color color; //��ɫ 
	s_vector normal;//������ 
	s_vector tangent;
	s_vector binormal;
	s_vector storage0;// ��ֵ�Ĵ���
	s_vector storage1;
	s_vector storage2;
}for_fs;

void v_shader(device_t* device, for_vs* vv, for_fs* ff);

void f_shader(device_t* device, for_fs* ff, s_color& color,int count,bool& is_ban,s_vector& ori_co);

void draw_line(device_t* device, int num, vertex_t* mesh);

void draw_line(device_t* device, int num, vector<vertex_t>& mesh);

void draw_plane(device_t* device, int num, vertex_t* mesh, int count);

void draw_plane(device_t* device, int num, vector<vertex_t>& mesh, int count);//����

void draw_plane_STRIP(device_t* device, vector<vertex_t>& mesh, vector<int>& indices, int count);

void camera_at_zero(device_t* device, s_vector eye, s_vector at, s_vector up);

void init_texture(device_t* device);

void init_texture_by_photo(device_t* device, char const* path);

void init_texture_by_diffuse(device_t* device, char const* path, int count);

void init_texture_by_specular(device_t* device, char const* path, int count);

void init_texture_by_normal(device_t* device, char const* path, int count);

void init_texture_by_albedo(device_t* device, char const* path, int count);

void init_texture_by_metallic(device_t* device, char const* path, int count);

void init_texture_by_roughness(device_t* device, char const* path, int count);

void init_texture_by_ao(device_t* device, char const* path, int count);

bool load_obj(std::vector<vertex_t>& tot_vertex, device_t* device, const char* obj_path, const char* pre_mtl_path, int start, bool filp_y);

bool load_pbrt_curve(string file_path, std::vector<vertex_t>& tot_vertex);

#endif
