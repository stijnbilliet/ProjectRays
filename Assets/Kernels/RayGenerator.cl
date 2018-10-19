#define EPSILON 0.001f

const sampler_t SMPL_PREF = CLK_NORMALIZED_COORDS_TRUE |
                            CLK_FILTER_LINEAR |
                            CLK_ADDRESS_NONE ;

typedef struct _Ray
{
	float4 o;
	float4 d;
	int2 extra;
	int dobackface;
	int padding;
} Ray;

__kernel void GenerateShadowRays(int width, int height, float4 lightPos, __global Ray* rays, __read_only image2d_t worldPosBuffer, __read_only image2d_t normalBuffer)
{
	const int2 texCoords = {get_global_id(0), get_global_id(1)};
	float4 worldPos = read_imagef(worldPosBuffer, SMPL_PREF, (int2)texCoords);
	float4 Normal = read_imagef(normalBuffer, SMPL_PREF, (int2)texCoords);
	Normal = normalize(Normal);
	
    if (texCoords.x < width && texCoords.y < height)
	{
		int k = texCoords.y * width + texCoords.x;
		float4 dir = lightPos - worldPos;
		rays[k].d = normalize(dir);
		rays[k].o = worldPos + Normal * EPSILON;
		rays[k].o.w = length(dir);
		
		rays[k].extra.x = 0xFFFFFFFF;
		rays[k].extra.y = 0xFFFFFFFF;
	}
}