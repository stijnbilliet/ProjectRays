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

typedef struct _Intersection
{
    // id of a shape
    int shapeid;
    // Primitive index
    int primid;
    // Padding elements
    int padding0;
    int padding1;
        
    // uv - hit barycentrics, w - ray distance
    float4 uvwt;
} Intersection;

__kernel void SamplePenumbra(float lightSize, __global float4* temporallightmask, __write_only image2d_t lightmask, __read_only image2d_depth_t depthbuffer)
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
	
	float occlusionresult = temporallightmask[k].x;
	float shadowValue = occlusionresult;
	float depth = read_imagef(depthbuffer, imgCoord) + 1.0f;
	int sampleSize = ((1.0f-occlusionresult)*lightSize)/depth;
	int halfSampleSize = sampleSize/2.0f;
	int maxSampleDistance = sqrt(pow((float)sampleSize, 2)+pow((float)sampleSize, 2));
	int sampleDistance = maxSampleDistance;
	
	int2 sampleCoord;
	sampleCoord.y = imgCoord.y;
	for(int c = -halfSampleSize; c < halfSampleSize; ++c)
	{
		//Skip ourself
		if(c == 0) continue;
		
		//Break if out of bounds
		sampleCoord.x = imgCoord.x + c;
		if(sampleCoord.x < 0 || sampleCoord.x >= dimensions.x) break;
		
		int sampleCoord1D = sampleCoord.y * dimensions.x + sampleCoord.x;
		if(temporallightmask[sampleCoord1D].x > 0.98f && c < sampleDistance)
		{
			sampleDistance = c;	
		}
	}
	
	sampleCoord.x = imgCoord.x;
	for(int r = -halfSampleSize; r < halfSampleSize; ++r)
	{
		//Skip ourself
		if(r == 0) continue;
		
		//Break if out of bounds
		sampleCoord.y = imgCoord.y + r;
		if(sampleCoord.y < 0 || sampleCoord.y >= dimensions.y) break;
		
		int sampleCoord1D = sampleCoord.y * dimensions.x + sampleCoord.x;
		if(temporallightmask[sampleCoord1D].x > 0.98f && r < sampleDistance)
		{
			sampleDistance = r;	
		}
	}
	
	shadowValue = sampleDistance / maxSampleDistance;
	if(occlusionresult < 1.0f) shadowValue = 3*pow((float)shadowValue, 2)-2*pow((float)shadowValue, 3);
	shadowValue = 1.0f - shadowValue;
	
	write_imagef(lightmask, imgCoord, (float4)shadowValue);
}

__kernel void GenerateLightingMask(__global Intersection* intersections, __global Ray* rays, __write_only image2d_t lightmask)
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
	float occlusionresult = 1.0f;
	//Check if ray did occlude (-1 no hit; any other value hit)
	if(intersections[k].shapeid != -1)
	{
		occlusionresult = intersections[k].uvwt.z/rays[k].o.w;
	}
	
	//Write info to lightmask
	write_imagef(lightmask, imgCoord, (float4)occlusionresult);
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
	my_ray->o.w = length(dir);
	my_ray->extra.x = 0xFFFFFFFF;
	my_ray->extra.y = 0xFFFFFFFF;
}