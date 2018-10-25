typedef struct _Ray
{
	//xyz origin
	float4 o;
	//xyz direction
	float4 d;
	//x-ray mask, y-activity flag
	int2 extra;
	//padding (backface and padding)
	float2 padding;
} Ray;

__kernel void GenerateLightingMask(__global int* occlusions, __write_only image2d_t lightmask)
{
	//Get image dimensions
	int2 dimensions;
	dimensions.x = get_image_width(lightmask);
	dimensions.y = get_image_height(lightmask);
	
	//Get texture coords
	int2 imgCoord;
	imgCoord.x = get_global_id(0);
	imgCoord.y = get_global_id(1);
	
	//Get 1D global index
	int k = imgCoord.y * dimensions.x + imgCoord.x;
	float4 fragmentColor = (0.0f, 0.0f, 0.0f, 1.0f);
	
	//Check if ray did occlude (-1 no hit; 1 hit)
	//Shade respectively (-1 light; 1 dark)
	if(occlusions[k] == 1)
	{
		fragmentColor = (0.0f, 0.0f, 0.0f, 0.0f);
	}
	
	//Write info to lightmask
	write_imagef(lightmask, imgCoord, fragmentColor);
}

__kernel void GenerateShadowRays(float4 lightPos, __global Ray* rays,
__read_only image2d_t worldPosBuffer, __read_only image2d_t normalBuffer)
{
	//Get image dimensions
	int2 dimensions;
	dimensions.x = get_image_width(worldPosBuffer);
	dimensions.y = get_image_height(worldPosBuffer);
	
	//Get texture coords
	int2 imgCoord;
	imgCoord.x = get_global_id(0);
	imgCoord.y = get_global_id(1);
	
	//Sample worldposition and normal information from g-Buffertargets
	float4 worldPos = read_imagef(worldPosBuffer, imgCoord);
	float4 Normal = normalize(read_imagef(normalBuffer, imgCoord));
	float4 dir = lightPos - worldPos;
	
	//Get 1D global index
	int k = imgCoord.y * dimensions.x + imgCoord.x;
	
	//Create rays
	__global Ray* my_ray = rays + k;
	
	my_ray->o = worldPos + Normal * 0.005f;
	my_ray->d = normalize(dir);
	my_ray->o.w = length(dir.xyz);	
	my_ray->extra.x = 0xFFFFFFFF;
	my_ray->extra.y = 0xFFFFFFFF;
}