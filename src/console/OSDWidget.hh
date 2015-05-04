#ifndef OSDWIDGET_HH
#define OSDWIDGET_HH

#include "StringMap.hh"
#include "string_ref.hh"
#include <vector>
#include <memory>

namespace openmsx {

class OutputRectangle;
class OutputSurface;
class TclObject;
class Interpreter;

class OSDWidget
{
public:
	virtual ~OSDWidget();

	const std::string& getName() const { return name; }
	float getX()    const { return x; }
	float getY()    const { return y; }
	float getZ()    const { return z; }
	float getRelX() const { return relx; }
	float getRelY() const { return rely; }

	      OSDWidget* getParent()       { return parent; }
	const OSDWidget* getParent() const { return parent; }
	OSDWidget* findSubWidget(string_ref name);
	const OSDWidget* findSubWidget(string_ref name) const;
	void addWidget(std::unique_ptr<OSDWidget> widget);
	void deleteWidget(OSDWidget& widget);

	virtual std::vector<string_ref> getProperties() const;
	virtual void setProperty(Interpreter& interp,
	                         string_ref name, const TclObject& value);
	virtual void getProperty(string_ref name, TclObject& result) const;
	virtual float getRecursiveFadeValue() const;
	virtual string_ref getType() const = 0;

	void invalidateRecursive();
	void paintSDLRecursive(OutputSurface& output);
	void paintGLRecursive (OutputSurface& output);

	int getScaleFactor(const OutputRectangle& surface) const;
	void transformXY(const OutputRectangle& output,
	                 float x, float y, float relx, float rely,
	                 float& outx, float& outy) const;
	void getBoundingBox(const OutputRectangle& output,
	                    int& x, int& y, int& w, int& h);
	virtual void getWidthHeight(const OutputRectangle& output,
	                            float& width, float& height) const = 0;

	// for OSDGUI::OSDCommand
	void listWidgetNames(const std::string& parentName,
	                     std::vector<std::string>& result) const;

protected:
	explicit OSDWidget(const std::string& name);
	void invalidateChildren();
	bool needSuppressErrors() const;

	virtual void invalidateLocal() = 0;
	virtual void paintSDL(OutputSurface& output) = 0;
	virtual void paintGL (OutputSurface& output) = 0;

private:
	void getMouseCoord(float& outx, float& outy) const;
	void transformReverse(const OutputRectangle& output,
	                      float x, float y,
	                      float& outx, float& outy) const;
	void setParent(OSDWidget* parent_) { parent = parent_; }
	void resortUp  (OSDWidget* elem);
	void resortDown(OSDWidget* elem);

	/** Direct child widgets of this widget, sorted by z-coordinate.
	  */
	std::vector<std::unique_ptr<OSDWidget>> subWidgets;

	/** Contains the same widgets as "subWidgets", but stored with their name
	  * the key, so lookup by name is fast.
	  */
	StringMap<OSDWidget*> subWidgetsMap;

	OSDWidget* parent;

	const std::string name;
	float x, y, z;
	float relx, rely;
	bool scaled;
	bool clip;
	bool suppressErrors;
};

} // namespace openmsx

#endif
