// $Id$

#ifndef __SDLLORENDERER_HH__
#define __SDLLORENDERER_HH__

#include <SDL/SDL.h>
#include "openmsx.hh"
#include "Renderer.hh"


class MSXTMS9928a;

/** Factory method to create SDLLoRenderer objects.
  */
Renderer *createSDLLoRenderer(MSXTMS9928a *vdp, bool fullScreen, const EmuTime &time);

/** Low-res (320x240) renderer on SDL.
  */
template <class Pixel> class SDLLoRenderer : public Renderer
{
public:
	/** Constructor.
	  * It is suggested to use the createSDLLoRenderer factory
	  * function instead, which automatically selects a colour depth.
	  */
	SDLLoRenderer(MSXTMS9928a *vdp, SDL_Surface *screen, const EmuTime &time);

	/** Destructor.
	  */
	virtual ~SDLLoRenderer();

	// Renderer interface:

	void putImage();
	void setFullScreen(bool);
	void updateForegroundColour(const EmuTime &time);
	void updateBackgroundColour(const EmuTime &time);
	void updateDisplayEnabled(const EmuTime &time);
	void updateDisplayMode(const EmuTime &time);
	void updateNameBase(const EmuTime &time);
	void updatePatternBase(const EmuTime &time);
	void updateColourBase(const EmuTime &time);
	void updateSpriteAttributeBase(const EmuTime &time);
	void updateSpritePatternBase(const EmuTime &time);
	void updateVRAM(int addr, byte data, const EmuTime &time);

private:
	typedef void (SDLLoRenderer::*RenderMethod)(int line);
	typedef void (SDLLoRenderer::*PhaseHandler)(int limit);

	void renderUntil(int limit);

	void mode0(int line);
	void mode1(int line);
	void mode2(int line);
	void mode12(int line);
	void mode3(int line);
	void modebogus(int line);
	void mode23(int line);

	void offPhase(int limit);
	void blankPhase(int limit);
	void displayPhase(int limit);

	/** Draw sprites on this line over the background.
	  */
	void drawSprites(Pixel *linePtr, int line);

	/** Set all dirty / clean.
	  */
	void setDirty(bool);

	/** RenderMethods for each screen mode.
	  */
	static RenderMethod modeToRenderMethod[];

	/** The VDP of which the video output is being rendered.
	  */
	MSXTMS9928a *vdp;

	/** SDL colours corresponding to each VDP palette entry.
	  * XPalFg has entry 0 set to the current background colour,
	  * XPalBg has entry 0 set to black.
	  */
	Pixel XPalFg[16], XPalBg[16];

	/** Rendering method for the current display mode.
	  */
	RenderMethod renderMethod;

	/** Phase handler: current drawing mode (off, blank, display).
	  */
	PhaseHandler currPhase;

	/** Number of the next line to render.
	  * Absolute NTSC line number: [0..262).
	  */
	int currLine;

	/** The surface which is visible to the user.
	  */
	SDL_Surface *screen;

	/** The surface which the image is rendered on.
	  */
	SDL_Surface *displayCache;

	/** Pointers to the start of each display line in the cache.
	  */
	Pixel *cacheLinePtrs[192];

	/** Pointers to the start of each display line on the screen.
	  */
	Pixel *screenLinePtrs[192];

	/** Dirty tables indicate which character blocks must be repainted.
	  * The anyDirty variables are true when there is at least one
	  * element in the dirty table that is true.
	  */
	bool anyDirtyColour, dirtyColour[1 << 10];
	bool anyDirtyPattern, dirtyPattern[1 << 10];
	bool anyDirtyName, dirtyName[1 << 10];

	/** Did foreground colour change since last screen update?
	  */
	bool dirtyForeground;

	/** Did background colour change since last screen update?
	  */
	bool dirtyBackground;

};

#endif //__SDLLORENDERER_HH__

