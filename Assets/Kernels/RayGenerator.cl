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
	
	//Get tile size
	int tileSize;
	tileSize = get_local_size(0);
	
	int k = imgCoord.y * dimensions.x + imgCoord.x;
	float distanceToLight = iLightmask[k].z;
	float distanceToOccluder = iLightmask[k].y;
	float occlusionValue = distanceToOccluder / distanceToLight;
	//float4 depthValue = read_imagef(depthBuffer, imgCoord);
	float penumbraSize = (occlusionValue * tileSize) / 1.0f;
	
	int halfSampleSize = (int)(penumbraSize/2);
	int tileSizeSquared = (int)(penumbraSize*penumbraSize);
	float shadowValue = iLightmask[k].x;
	
	if(shadowValue != 0.0f)
	{
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
	if(intersections[k].shapeid != -1)
	{
		fragmentColor = (float4)(0.0f, intersections[k].uvwt.w, rays[k].o.w, 0.0f);
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
	float maxRayDistance = length(dir)*1.1f;
	
	//Generate random direction in angularExtent
	int2 vectorModulus;
	vectorModulus.x = imgCoord.x % tileSize;
	vectorModulus.y = imgCoord.y % tileSize;
	uint rnmb = vectorModulus.x * vectorModulus.y;
	
	{
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
	}
	
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