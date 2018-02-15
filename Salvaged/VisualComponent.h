#ifndef VISUALCOMPONENT_H
#define VISUALCOMPONENT_H

enum VisualComponentType {
	IMAGE,
	SPRITE,
	BUTTON,
	SWITCH,
	LABEL,
	TEXT_INPUT,
	RECT,
	INVISIBLE
};

class VisualComponent {
public:
  virtual ~VisualComponent() {}

  virtual VisualComponentType getType() = 0;
};

#endif
