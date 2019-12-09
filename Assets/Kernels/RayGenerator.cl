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

const sampler_t samplerLinearClamped = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP | CLK_FILTER_LINEAR;
const sampler_t samplerLinearRepeat = CLK_NORMALIZED_COORDS_TRUE | CLK_ADDRESS_REPEAT | CLK_FILTER_LINEAR;

__kernel void SoftShadowSample(__read_only image2d_t iLightmask, __write_only image2d_t oLightmask, __read_only image2d_t normalBuffer, __read_only image2d_t depthBuffer, float depthEpsilon, float normalEpsilon, float angularExtent)
{
	//Get image dimensions
	int2 dimensions;
	dimensions.x = get_image_width(oLightmask);
	dimensions.y = get_image_height(oLightmask);
	
	//Get texture coords
	int2 imgCoord;
	imgCoord.x = get_global_id(0);
	imgCoord.y = get_global_id(1);
	
	int k = imgCoord.y * dimensions.x + imgCoord.x;
	float4 lightMask = read_imagef(iLightmask, imgCoord);
	float shadowValue = lightMask.x;
	float distanceToOccluder = lightMask.y;
	float distanceToLight = lightMask.z;
	float occlusionValue = (distanceToLight - distanceToOccluder) / distanceToLight;
	float lightSize = angularExtent * 2.0f;
	float penumbraSize = occlusionValue * lightSize;
	
	float sampleSize = 10;
	int halfSampleSize = (int)(sampleSize/2);
	int tileSizeSquared = (int)(sampleSize*sampleSize);

	float2 sampleCoord;
	sampleCoord.x = imgCoord.x * 1.0f;
	sampleCoord.y = imgCoord.y * 1.0f;

	for(int c = -halfSampleSize; c < halfSampleSize; ++c)
	{
		sampleCoord.x = imgCoord.x + (c * penumbraSize);
		for(int r = -halfSampleSize; r < halfSampleSize; ++r)
		{
			sampleCoord.y = imgCoord.y + (r * penumbraSize);
			shadowValue += read_imagef(iLightmask, samplerLinearClamped, sampleCoord).x;
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
	
	//Check if ray did occlude (-1 no hit; anything else = hit)
	//Shade respectively (-1 should be 1 ; anything else should be 0)	
	fragmentColor = (float4)(abs(min(intersections[k].shapeid, 0)), intersections[k].uvwt.w, rays[k].o.w, intersections[k].shapeid);
	
	//Write info to lightmask
	write_imagef(lightmask, imgCoord, fragmentColor);
}

__kernel void GenerateShadowRay(float4 endPos, float angularExtent, uint tileSize, __global Ray* rays,
__read_only image2d_t worldPosBuffer, __read_only image2d_t normalBuffer, __read_only image2d_t inputNoise)
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
	
	float2 sampleCoord;
	sampleCoord.x = imgCoord.x / (float)dimensions.x;
	sampleCoord.y = imgCoord.y / (float)dimensions.y;
	float randVal = read_imagef(inputNoise, samplerLinearRepeat, sampleCoord).x;
	
	float r = tan(angularExtent * 3.14f/180.0f) * length(dir); //radius from angle
	float t = 2 * 3.14f * randVal; //theta
	float d = sqrt(randVal) * r; //distance from centre
	
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
	my_ray->extra.y = 0xFFFFFFFF;
}