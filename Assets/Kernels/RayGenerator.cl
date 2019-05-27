typedef struct _Ray
{
	//xyz origin
	float4 o;
	//xyz direction
	float4 d;
	//x-ray mask, y-activity flag
	int2 extra;
	//padding (backface and padding)
	int2 padding;
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
	float4 fragmentColor = (float4)(1.0f, 1.0f, 1.0f, 1.0f);
	
	//Check if ray did occlude (-1 no hit; 1 hit)
	//Shade respectively (-1 light; 1 dark)
	if(occlusions[k] == 1)
	{
		fragmentColor = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
	}
	
	//Write info to lightmask
	write_imagef(lightmask, imgCoord, fragmentColor);
}

__kernel void GenerateShadowRay(float4 endPos, float angularExtent, uint tileSize, __global Ray* rays,
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
	float4 dir = endPos - worldPos;
	
	//Generate random direction in angularExtent
	int2 vectorModulus;
	vectorModulus.x = imgCoord.x % tileSize;
	vectorModulus.y = imgCoord.y % tileSize;
	uint rnmb = vectorModulus.x * vectorModulus.y;
	
    rnmb ^= (rnmb << 13); //xorshift algorithm
    rnmb ^= (rnmb >> 17);
    rnmb ^= (rnmb << 5);
	
	float f0 = rnmb * (1.0 / 4294967296.0); //random between 0 and 1
	f0 *= angularExtent;
	
	float r = angularExtent * sqrt(f0); //radius
	float t = 2 * 3.14 * f0; //theta
	
	dir.x += r * cos(t);
	dir.z += r * sin(t);
	
	dir = normalize(dir);
	
	//Get 1D global index
	int k = imgCoord.y * dimensions.x + imgCoord.x;
	
	//Create rays
	__global Ray* my_ray = rays + k;
	my_ray->o = worldPos + Normal * 0.005f;
	my_ray->d = dir;
	my_ray->o.w = 1000.0f;
	my_ray->extra.x = 0xFFFFFFFF;
	my_ray->extra.y = ceil(dot(dir, Normal));
}