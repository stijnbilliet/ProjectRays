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

typedef struct _Intersection
{
	// Shape ID
	int shapeid;
	// Primitve ID
	int primid;

	int2 padding;
	
	// UV parametrization
	float4 uvwt;
} Intersection;

__kernel void SoftShadowSample(__global float4* iLightmask, __write_only image2d_t oLightmask, __read_only image2d_t depthBuffer)
{
	//Get image dimensions
	int2 dimensions;
	dimensions.x = get_image_width(oLightmask);
	dimensions.y = get_image_height(oLightmask);
	
	//Get texture coords
	int2 imgCoord;
	imgCoord.x = get_global_id(0);
	imgCoord.y = get_global_id(1);
	
	//Get neighborhood size
	int neighborhood;
	neighborhood = get_local_size(0);
	
	int k = imgCoord.y * dimensions.x + imgCoord.x;

	int halfSampleSize = (int)(neighborhood/2);
	int tileSizeSquared = (int)(neighborhood*neighborhood);
	float shadowValue = iLightmask[k].x;
	
	int2 sampleCoord = imgCoord;
	for(int c = -halfSampleSize; c < halfSampleSize; ++c)
	{
		sampleCoord.x = imgCoord.x + c;
		if(sampleCoord.x < 0 || sampleCoord.x >= dimensions.x) break;
		for(int r = -halfSampleSize; r < halfSampleSize; ++r)
		{
			sampleCoord.y = imgCoord.y + r;
			if(sampleCoord.y < 0 || sampleCoord.y >= dimensions.y) break;
			int sampleCoord1D = sampleCoord.y * dimensions.x + sampleCoord.x;
			shadowValue += iLightmask[sampleCoord1D].x;
		}
	}
	
	shadowValue /= tileSizeSquared;
	
	write_imagef(oLightmask, imgCoord, (float4)shadowValue);
}

__kernel void GenerateLightingMask(__global Intersection* intersections, __write_only image2d_t lightmask, __global Ray* rays)
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
	fragmentColor = (float4)(abs(min(intersections[k].shapeid, 0)), intersections[k].uvwt.w, rays[k].o.w, 0.0f);
	
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
	float maxRayDistance = length(dir)*1.1f;
	
	//Generate random direction in angularExtent
	uint rndState = (imgCoord.x % tileSize) * (imgCoord.y % tileSize);
	
	//Hash our random seed
	rndState = (rndState ^ 61) ^ (rndState >> 16);
    rndState *= 9;
    rndState = rndState ^ (rndState >> 4);
    rndState *= 0x27d4eb2d;
    rndState = rndState ^ (rndState >> 15);
	
	rndState ^= (rndState << 13); //xorshift algorithm
	rndState ^= (rndState >> 17);
	rndState ^= (rndState << 5);
	
	float f0 = rndState * (1.0 / 4294967296.0); //random between 0 and 1

	rndState ^= (rndState << 13); //xorshift algorithm
	rndState ^= (rndState >> 17);
	rndState ^= (rndState << 5);
	
	float f1 = rndState * (1.0 / 4294967296.0); //random between 0 and 1
	
	float r = tan(angularExtent * 3.14f/180.0f) * length(dir); //radius from angle
	float t = 2 * 3.14f * f0; //theta
	float d = sqrt(f1) * r; //distance from centre
	
	dir.x += d * cos(t);
	dir.z += d * sin(t);
	
	dir = normalize(dir);
	
	//Get 1D global index
	int k = imgCoord.y * dimensions.x + imgCoord.x;
	
	//Create rays
	__global Ray* my_ray = rays + k;
	my_ray->o = worldPos + Normal * 0.005f;
	my_ray->d = dir;
	my_ray->o.w = maxRayDistance;
	my_ray->extra.x = 0xFFFFFFFF;
	my_ray->extra.y = ceil(dot(dir, Normal));
}