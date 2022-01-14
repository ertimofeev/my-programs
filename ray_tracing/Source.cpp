#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

#define INF 999999999.9

const double Vw = 1;
const double Vh = 1;
const double D = 1;
const double Cw = 720;
const double Ch = 720;
const int recursion_depth = 5;

int clamp(double v, double mn, double mx)
{
	return (max(mn, min(v, mx))) / 1;
}

struct vec
{
	vec()
	{
		x = y = z = 0;
	}

	vec(double x_, double y_, double z_)
	{
		x = x_;
		y = y_;
		z = z_;
	}

	double dot(const vec& other) const
	{
		return x * other.x + y * other.y + z * other.z;
	}

	double length() const
	{
		return sqrt(x * x + y * y + z * z);
	}

	vec add(double k) const
	{
		return vec(k + x, k + y, k + z);
	}

	vec add(const vec& other) const
	{
		return vec(x + other.x, y + other.y, z + other.z);
	}

	vec sub(const vec& other) const
	{
		return vec(x - other.x, y - other.y, z - other.z);
	}

	vec operator-() const
	{
		return vec(-x, -y, -z);
	}

	vec mult(double k) const
	{
		return vec(k * x, k * y, k * z);
	}

	vec mult(const vec& other) const
	{
		return vec(x * other.x, y * other.y, z * other.z);
	}
	vec normalized() {
		return vec(x / length(), y / length(), z / length());
	}

	double x, y, z;
};
vec ReflectRay(vec R, vec N) {
	return N.mult(N.dot(R) * 2).sub(R);
}


std::ostream& operator <<(std::ostream& os, const vec& v)
{
	return os << "vec(" << v.x << ", " << v.y << ", " << v.z << ")";
}

struct color_t
{
	color_t()
	{
		r = g = b = 255;
	}

	color_t(double r_, double g_, double b_)
	{
		r = r_;
		g = g_;
		b = b_;
	}

	color_t add(double k) const
	{
		return color_t(k + r, k + g, k + b);
	}

	color_t add(const color_t& other) const
	{
		return color_t(r + other.r, g + other.g, b + other.b);
	}

	color_t mult(double k) const
	{
		return color_t(k * r, k * g, k * b);
	}

	color_t normalized() const
	{
		return color_t(clamp(r, 0, 255), clamp(g, 0, 255), clamp(b, 0, 255));
	}

	double r, g, b;
};

struct sphere_t
{
	sphere_t()
	{
		radius = 0;
		color = color_t();
		center = vec();
		specular = -1.0;
		reflective = 0;
	}

	sphere_t(const vec& center_, double radius_, const color_t& color_, double specular_, double reflective_)
	{
		radius = radius_;
		center = center_;
		color = color_;
		specular = specular_;
		reflective = reflective_;
	}

	pair<double, double> getIntersectionsWith(const vec& O, const vec& D)
	{
		vec OC = O.sub(center);

		double k1 = D.dot(D);
		double k2 = 2 * OC.dot(D);
		double k3 = OC.dot(OC) - radius * radius;

		double d = k2 * k2 - 4 * k1 * k3;
		if (d < 0)
			return pair<double, double>(INF + 2.0, INF + 2.0);

		double t1 = (-k2 + sqrt(d)) / (2.0 * k1);
		double t2 = (-k2 - sqrt(d)) / (2.0 * k1);

		return pair<double, double>(t1, t2);
	}

	color_t color;
	vec center;
	double radius;
	double specular;
	double reflective;
};
struct light_t
{
	enum type_t { AMBIENT, POINT, DIRECTIONAL };
	light_t(double i, type_t type_, vec A = vec())
	{
		intensity = i;
		type = type_;
		v = A;
	}
	type_t type;
	vec v;
	double intensity;
};
vector<light_t>lights;
pair<int, double> ClosestIntersection(vec O, vec D, double t_min, double t_max);
double ComputeLightning(const vec& P, const vec& N, const vec& V, double s)
{
	double i = 0.0;
	for (int j = 0; j < lights.size(); ++j)
	{
		if (lights[j].type == light_t::type_t::AMBIENT) {
			i += lights[j].intensity;
		}
		else
		{
			vec L;
			if (lights[j].type == light_t::type_t::POINT) {
				L = lights[j].v.sub(P);
			}
			else
			{
				L = lights[j].v;
			}
			pair<int, double> p = ClosestIntersection(P, L, 0.001, INF);
			if (p.first >= 0)
				continue;

			double NDL = N.dot(L);
			if (NDL > 0)
			{
				i += lights[j].intensity * NDL / (N.length() * L.length());
			}

			if (s > 0)
			{
				vec R = ReflectRay(N, L);
				double RDV = R.dot(V);
				if (RDV > 0)
				{
					i += lights[j].intensity * pow(RDV / (R.length() * V.length()), s);
				}
			}
		}
	}

	return i;
}


vector<sphere_t> spheres;

typedef vector<vector<color_t>> data_t;

data_t normalized(const data_t& v)
{
	data_t r(v.size(), vector<color_t>(v[0].size()));
	for (size_t i = 0; i < v.size(); ++i)
	{
		for (size_t j = 0; j < v[i].size(); ++j)
		{
			r[i][j] = v[i][j].normalized();
		}
	}

	return r;
}

void writeImage(const string& filename, const data_t& data_)
{
	data_t data = normalized(data_);
	int h = data.size();
	int w = data[0].size();

	FILE* f;
	int filesize = 54 + 3 * w * h;  //w is your image width, h is image height, both int

	vector<unsigned char> img(3 * w * h, 0);

	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			int x = i, y = (h - 1) - j;
			int r = data[i][j].r;
			int g = data[i][j].g;
			int b = data[i][j].b;
			img[(x + y * w) * 3 + 2] = (unsigned char)(r);
			img[(x + y * w) * 3 + 1] = (unsigned char)(g);
			img[(x + y * w) * 3 + 0] = (unsigned char)(b);
		}
	}

	unsigned char bmpfileheader[14] = { 'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0 };
	unsigned char bmpinfoheader[40] = { 40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0 };
	unsigned char bmppad[3] = { 0,0,0 };

	bmpfileheader[2] = (unsigned char)(filesize);
	bmpfileheader[3] = (unsigned char)(filesize >> 8);
	bmpfileheader[4] = (unsigned char)(filesize >> 16);
	bmpfileheader[5] = (unsigned char)(filesize >> 24);

	bmpinfoheader[4] = (unsigned char)(w);
	bmpinfoheader[5] = (unsigned char)(w >> 8);
	bmpinfoheader[6] = (unsigned char)(w >> 16);
	bmpinfoheader[7] = (unsigned char)(w >> 24);
	bmpinfoheader[8] = (unsigned char)(h);
	bmpinfoheader[9] = (unsigned char)(h >> 8);
	bmpinfoheader[10] = (unsigned char)(h >> 16);
	bmpinfoheader[11] = (unsigned char)(h >> 24);

	f = fopen(filename.c_str(), "wb");
	fwrite(bmpfileheader, 1, 14, f);
	fwrite(bmpinfoheader, 1, 40, f);

	const unsigned char* pimg = img.data();
	for (int i = 0; i < h; i++)
	{
		fwrite(pimg + (w * (h - i - 1) * 3), 3, w, f);
		fwrite(bmppad, 1, (4 - (w * 3) % 4) % 4, f);
	}

	fclose(f);
}

vec canvasToViewport(double x, double y)
{
	return vec(x * Vw / Cw, y * Vh / Ch, D);
}//
pair<int, double> ClosestIntersection(vec O, vec D, double t_min, double t_max)
{
	double min_d = -1;
	int closest_id = -1;
	for (int i = 0; i < spheres.size(); ++i)
	{
		pair<double, double> p;
		p = spheres[i].getIntersectionsWith(O, D);
		if ((min_d < 0 || min_d >= p.first) && t_min <= p.first && p.first <= t_max)
		{
			min_d = p.first;
			closest_id = i;
		}

		if ((min_d < 0 || min_d >= p.second) && t_min <= p.second && p.second <= t_max)
		{
			min_d = p.second;
			closest_id = i;
		}
	}
	return make_pair(closest_id, min_d);
}
//
color_t traceRay(const vec& O, const vec& D, double t_min, double t_max, int recursion_depth)
{
	int closest_id = -1;
	double min_d = -1;
	pair<int, double> p = make_pair(-1, -1);
	p = ClosestIntersection(O, D, t_min, t_max);
	if (p.first < 0)
	{
		return color_t(0, 0, 0); // BACKGROUND_COLOR
	}
	vec P = O.add(D.mult(p.second));
	vec N = P.sub(spheres[p.first].center).normalized();
	color_t local_color = spheres[p.first].color.mult(ComputeLightning(P, N, -D, spheres[p.first].specular));
	double r = spheres[p.first].reflective;
	color_t reflected_color = color_t();

	if (r <= 0 || recursion_depth <= 0)
		return local_color;
	else {
		vec R = ReflectRay(-D, N);
		reflected_color = traceRay(P, R, 0.001, INF, recursion_depth - 1);
		return local_color.mult(1 - r).add(reflected_color.mult(r));
	}
}
int main()
{
	vec O(0, 0, 0);
	spheres.push_back(sphere_t(vec(0, -1, 3), 1, color_t(255, 0, 0), 500, 0.2));
	spheres.push_back(sphere_t(vec(2, 0, 4), 1, color_t(0, 0, 255), 500, 0.3));
	spheres.push_back(sphere_t(vec(-2, 0, 3), 1, color_t(0, 255, 0), 10, 0.4));
	spheres.push_back(sphere_t(vec(0, -5001, 0), 5000, color_t(255, 255, 0), 1000, 1));
	lights.push_back(light_t(0.2, light_t::AMBIENT));
	lights.push_back(light_t(0.6, light_t::POINT, vec(2, 1, 0)));
	lights.push_back(light_t(0.2, light_t::DIRECTIONAL, vec(1, 4, 4)));
	data_t image(Ch, vector<color_t>(Cw));

	for (int y = -Ch / 2; y < Ch / 2; ++y)
	{
		for (int x = -Cw / 2; x < Cw / 2; ++x)
		{
			vec D = canvasToViewport(y, x);
			color_t color = traceRay(O, D, 1, INF, 3);
			image[y + Ch / 2][x + Cw / 2] = color;
		}
	}

	writeImage("result.bmp", image);

	return 0;
}