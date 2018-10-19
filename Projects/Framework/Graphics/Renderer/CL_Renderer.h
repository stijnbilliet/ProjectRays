#pragma once
class GL_Renderer;
class SceneManager;
class BaseGamemode;
class GameContext;
class CL_Renderer : public SingleInstance<CL_Renderer>
{
public:
	CL_Renderer();
	virtual ~CL_Renderer();

	virtual void OnInit(GameContext* pGameContext) override;
	virtual void OnUpdate(GameContext* pContext) override;

	void RaytracedShadows(GameContext* pGameContext);
	RadeonRays::IntersectionApi* GetRaysAPI() const;
private:
	void InitCL(GameContext* pGameContext);
	void InitRadeonRays(GameContext* pGameContext);
	void InitKernels(GameContext* pGameContext);
	void GenerateShadowRays(GameContext* pGameContext);

	void InitShadowKernel(GameContext* pGameContext);

	//Radeon rays context
	RadeonRays::IntersectionApi* m_pRRContext;
	
	//OpenCL context
	CLWContext m_CLContext; //runtime interface between cpu and gpu
	CLWDevice m_CLDevice;

	//Kernels
	CLWProgram m_RayGenerator; //one or more kernels bundled in one
	CLWKernel m_ShadowRayGenerator;

	//KernelData
	cl_mem m_CLGLWorldPosBuffer;
	cl_mem m_CLGLNormalBuffer;
	cl_mem m_CLGLRaysBuffer;

	//Radeon rays data
	RadeonRays::Buffer* m_RRaysBuffer;
	RadeonRays::Buffer* m_OcclusionBuffer;

	int m_ScreenWidth;
	int m_ScreenHeight;
	std::string m_AssetPath;
};