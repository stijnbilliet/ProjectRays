#define EPSILON 0.001f

typedef struct _Ray
{
	float4 o;
	float4 d;
	int2 extra;
	int dobackface;
	int padding;
} Ray;

__kernel void GenerateShadowRays(int width,int height,
float4 lightPos, __global Ray* rays,
__read_only image2d_t worldPosBuffer, __read_only image2d_t normalBuffer)
{
	int2 imgCoord;
	imgCoord.x = get_global_id(0);
	imgCoord.y = get_global_id(1);
	
	float4 worldPos = read_imagef(worldPosBuffer, imgCoord);
	float4 Normal = read_imagef(normalBuffer, imgCoord);
	Normal = normalize(Normal);
	
	int k = imgCoord.y * width + imgCoord.x;
	float4 dir = lightPos - worldPos;
	
	__global Ray* newRay = rays + k;
	newRay->d = normalize(dir);
	newRay->o = worldPos + Normal * EPSILON;
	newRay->o.w = length(dir);
	
	newRay->extra.x = 0xFFFFFFFF;
	newRay->extra.y = 0xFFFFFFFF;
	newRay->dobackface = 0x00000000;
}