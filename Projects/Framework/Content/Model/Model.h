#pragma once
class Model : public BaseObject
{
public:
	Model(const char* modelPath);
	virtual ~Model();

	virtual void OnInit() override;
private:
	std::string m_ModelPath;
};