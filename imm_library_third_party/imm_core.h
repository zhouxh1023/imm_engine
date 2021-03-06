////////////////
// imm_core.h
// miscellaneous basic utilities
// from 3D Game Programming with DirectX 11 by Frank Luna:
// GameTimer.h
// d3dUtil.h
// MathHelper.h
// Helper math class.
// LightHelper.h
// Helper classes for lighting.
// Effects.h
// Defines lightweight effect wrappers to group an effect and its variables.
// Also defines a static Effects class from which we can access all of our effects.
// by Frank Luna (C) 2011 All Rights Reserved.
////////////////
////////////////
#ifndef IMM_UTIL_H
#define IMM_UTIL_H
#include <cassert>
#include <windows.h>
#include <cfloat>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <d3d11.h>
#include "d3dx11effect.h"
#include <DirectXMath.h>
#include "debug_console.h"
using namespace DirectX;
////////////////
// timer
////////////////
////////////////
namespace imm
{
class timer
{
public:
	timer();
	float total_time()const;
	float delta_time()const {return (float)m_DeltaTime;}
	void reset();
	void start();
	void stop();
	void tick();
private:
	double m_SecPerCount;
	double m_DeltaTime;
	__int64 m_BaseTime;
	__int64 m_PausedTime;
	__int64 m_StopTime;
	__int64 m_PrevTime;
	__int64 m_CurrTime;
	bool m_Stopped;
};
// inial all!
timer::timer():
	m_SecPerCount(0.0),
	m_DeltaTime(-1.0),
	m_BaseTime(0),
	m_PausedTime(0),
	m_StopTime(0),
	m_PrevTime(0),
	m_CurrTime(0),
	m_Stopped(false)
{
	__int64 counts_per_sec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&counts_per_sec);
	m_SecPerCount = 1.0/(double)counts_per_sec;
}
// Returns the total time elapsed since reset() was called, NOT counting any
// time when the clock is stopped.
float timer::total_time() const
{
	if (m_Stopped) return (float)(((m_StopTime-m_PausedTime)-m_BaseTime)*m_SecPerCount);
	else return (float)(((m_CurrTime-m_PausedTime)-m_BaseTime)*m_SecPerCount);
}
//
void timer::reset()
{
	__int64 curr_time;
	QueryPerformanceCounter((LARGE_INTEGER*)&curr_time);
	m_BaseTime = curr_time;
	m_PrevTime = curr_time;
	m_StopTime = 0;
	m_Stopped  = false;
}
//
void timer::start()
{
	__int64 start_time;
	QueryPerformanceCounter((LARGE_INTEGER*)&start_time);
	if (m_Stopped) {
		m_PausedTime += (start_time-m_StopTime);	
		m_PrevTime = start_time;
		m_StopTime = 0;
		m_Stopped  = false;
	}
}
//
void timer::stop()
{
	if (!m_Stopped) {
		__int64 curr_time;
		QueryPerformanceCounter((LARGE_INTEGER*)&curr_time);
		m_StopTime = curr_time;
		m_Stopped  = true;
	}
}
//
void timer::tick()
{
	if (m_Stopped) {m_DeltaTime = 0.0; return;}
	QueryPerformanceCounter((LARGE_INTEGER*)&m_CurrTime);
	// Time difference between this frame and the previous.
	m_DeltaTime = (m_CurrTime-m_PrevTime)*m_SecPerCount;
	// Prepare for next frame.
	m_PrevTime = m_CurrTime;
	// Force nonnegative.  The DXSDK's CDXUTTimer mentions that if the 
	// processor goes into a power save mode or we get shuffled to another
	// processor, then m_DeltaTime can be negative.
	if (m_DeltaTime < 0.0) m_DeltaTime = 0.0;
}
}
////////////////
// DEBUG, IUnknown
////////////////
////////////////
#if defined(DEBUG) | defined(_DEBUG)
	#ifndef HR 
	#define HR(x) {HRESULT hr = (x); if (FAILED(hr))\
		{std::wostringstream os; os << hr; MessageBox(0, os.str().c_str(), L"HRESULT", 0); assert(false); abort();}}
	#endif
#else
	#ifndef HR
	#define HR(x) (x)
	#endif
#endif
#define ReleaseCOM(x) {if (x) {x->Release(); x = nullptr;}}
#define SafeDelete(x) {delete x; x = nullptr;}
// do nothing
#define DUMMY(x) (x)
// error messagebox
#define ERROR_MES(x) \
	{std::wstring w = (x); \
	std::wostringstream os; os << w; MessageBox(0, os.str().c_str(), L"ERROR", 0); assert(false); abort();}
namespace imm
{
////////////////
// math
////////////////
////////////////
// Returns random float in [0, 1).
float calc_randf() {return (float)(rand())/(float)RAND_MAX;}
// Returns random float in [a, b).
float calc_randf(float a, float b) {return a+calc_randf()*(b-a);}
template <typename T>
T calc_min(const T &a, const T &b) {return a < b ? a : b;}
template <typename T>
T calc_max(const T &a, const T &b) {return a > b ? a : b;}
template <typename T>
T calc_lerp(const T &a, const T &b, float t) {return a+(b-a)*t;}
template <typename T>
T calc_clamp(const T &x, const T &low, const T &high) {return x < low ? low : (x > high ? high : x);}
// Returns the polar angle of the point (x,y) in [0, 2*PI).
float angle_from_xy(float x, float y)
{
	float theta = 0.0f;
	// Quadrant I or IV
	if (x >= 0.0f) {
		// If x = 0, then atanf(y/x) = +pi/2 if y > 0
		//                atanf(y/x) = -pi/2 if y < 0
		theta = atanf(y / x); // in [-pi/2, +pi/2]
		if (theta < 0.0f) theta += 2.0f*XM_PI; // in [0, 2*pi).
	}
	// Quadrant II or III
	else theta = atanf(y/x)+XM_PI; // in [0, 2*pi).
	return theta;
}
//
XMMATRIX inverse_transpose(CXMMATRIX m) {
	// Inverse-transpose is just applied to normals.  So zero out
	// translation row so that it doesn't get into our inverse-transpose
	// calculation--we don't want the inverse-transpose of the translation.
	XMMATRIX a = m;
	a.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR det = XMMatrixDeterminant(a);
	return XMMatrixTranspose(XMMatrixInverse(&det, a));
}
//
XMVECTOR rand_unit_vec3()
{
	XMVECTOR one  = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	// Keep trying until we get a point on/in the hemisphere.
	bool conditional = true;
	while (conditional) {
		// Generate random point in the cube [-1,1]^3.
		XMVECTOR v = XMVectorSet(calc_randf(-1.0f, 1.0f), calc_randf(-1.0f, 1.0f), calc_randf(-1.0f, 1.0f), 0.0f);
		// Ignore points outside the unit sphere in order to get an even distribution
		// over the unit sphere.  Otherwise points will clump more on the sphere near
		// the corners of the cube.
		if (XMVector3Greater(XMVector3LengthSq(v), one)) continue;
		return XMVector3Normalize(v);
	}
	return one;
}
//
XMVECTOR rand_hemisphere_unit_vec3(XMVECTOR n)
{
	XMVECTOR one  = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	XMVECTOR zero = XMVectorZero();
	// Keep trying until we get a point on/in the hemisphere.
	bool conditional = true;
	while (conditional) {
		// Generate random point in the cube [-1,1]^3.
		XMVECTOR v = XMVectorSet(calc_randf(-1.0f, 1.0f), calc_randf(-1.0f, 1.0f), calc_randf(-1.0f, 1.0f), 0.0f);
		// Ignore points outside the unit sphere in order to get an even distribution
		// over the unit sphere.  Otherwise points will clump more on the sphere near
		// the corners of the cube.
		if (XMVector3Greater(XMVector3LengthSq(v), one)) continue;
		// Ignore points in the bottom hemisphere.
		if (XMVector3Less(XMVector3Dot(n, v), zero)) continue;
		return XMVector3Normalize(v);
	}
	return one;
}
////////////////
// light
// Note: Make sure structure alignment agrees with HLSL structure padding rules.
// Elements are packed into 4D vectors with the restriction that an element
// cannot straddle a 4D vector boundary.
////////////////
////////////////
struct lit_dir
{
	lit_dir() {ZeroMemory(this, sizeof(this));}
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;
	XMFLOAT3 direction;
	float pad; // Pad the last float so we can set an array of lights if we wanted.
};
//
struct lit_point
{
	lit_point() {ZeroMemory(this, sizeof(this));}
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;
	// Packed into 4D vector: (Position, Range)
	XMFLOAT3 position;
	float range;
	// Packed into 4D vector: (A0, A1, A2, Pad)
	XMFLOAT3 att;
	float pad; // Pad the last float so we can set an array of lights if we wanted.
};
//
struct lit_spot
{
	lit_spot() {ZeroMemory(this, sizeof(this));}
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;
	// Packed into 4D vector: (Position, Range)
	XMFLOAT3 position;
	float range;
	// Packed into 4D vector: (Direction, Spot)
	XMFLOAT3 direction;
	float spot;
	// Packed into 4D vector: (Att, Pad)
	XMFLOAT3 att;
	float pad; // Pad the last float so we can set an array of lights if we wanted.
};
//
struct material
{
	material() {ZeroMemory(this, sizeof(this));}
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular; // w = SpecPower
	XMFLOAT4 reflect;
};
////////////////
// effect
////////////////
////////////////
class effect
{
public:
	effect(ID3D11Device *device, const std::wstring &filename);
	virtual ~effect() {ReleaseCOM(m_FX);}
private:
	effect(const effect &rhs);
	effect &operator=(const effect &rhs);
protected:
	ID3DX11Effect *m_FX;
};
//
effect::effect(ID3D11Device *device, const std::wstring &filename):
	m_FX(0)
{
	std::ifstream fin(filename, std::ios::binary);
	if (!fin.is_open()) {
		std::wostringstream os;
		os << "Effect file load error: " << filename;
		ERROR_MES(os.str().c_str());
	}
	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compiled_shader(size);
	fin.read(&compiled_shader[0], size);
	fin.close();
	HR(D3DX11CreateEffectFromMemory(&compiled_shader[0], size, 0, device, &m_FX));
}
////////////////
// GLOBAL
////////////////
////////////////
static std::map<std::string, std::string> GLOBAL {
	{"path_fx", "ass_shader\\"},
	{"path_mod", "ass_model\\"},
	{"path_tex", "ass_texture\\"},
	{"path_lua", "script\\"},
	{"path_out", "misc\\output\\"},
	{"path_inp", "misc\\input\\"},
};
}
#endif