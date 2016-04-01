float dot_product(float x[2], float y[2])
{
	float result = 0;
	result = x[0]*y[0] + x[1]*y[1];
	return (result);
}

int barycentric(float a[2], float b[2], float c[2], float p[2])
{
	float v0[2], v1[2], v2[2];	//vectors between the points
	v0[0] = c[0] - a[0];
	v0[1] = c[1] - a[1];
	v1[0] = b[0] - a[0];
	v1[1] = b[1] - a[1];
	v2[0] = p[0] - a[0];
	v2[1] = p[1] - a[1];

	float dot00, dot01, dot02, dot11, dot12;	//dot products
	dot00 = dot_product(v0, v0);
	dot01 = dot_product(v0, v1);
	dot02 = dot_product(v0, v2);
	dot11 = dot_product(v1, v1);
	dot12 = dot_product(v1, v2);

	float denominator = 1;
	denominator = 1/(dot00 * dot11 - dot01*dot01);
	float u,v;	//barycentric co-ordinates
	u = (dot11 * dot02 - dot01 * dot12)*denominator;
	v = (dot00 * dot12 - dot01 * dot02)*denominator;

	return (u>=0 && v>=0 && (u+v)<1);
}
