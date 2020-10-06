#ifndef GUICOMPONENT_H
#define GUICOMPONENT_H

class GuiObject;
class Camera;

class GuiComponent
{
public:
	GuiComponent() : parent(nullptr) {};
	virtual ~GuiComponent() { parent = nullptr; }
	virtual void Draw(Camera* camera_, glm::vec2 position_) = 0;
	virtual bool FindContainingPoint(glm::vec2 position_) = 0;
protected:
	 GuiComponent* parent;
};

#endif
