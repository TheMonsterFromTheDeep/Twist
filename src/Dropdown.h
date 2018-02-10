#ifndef TWIST_DROPDOWN_H_
#define TWIST_DROPDOWN_H_

#include "Widget.h"
#include "Reference.h"
#include "Image.h"

namespace Twist {
	class DropdownItem {
	public:
		Reference<Image> icon;
		std::string text;

		DropdownItem(std::string text_, Reference<Image> icon_=nullptr);
	};

	class DropdownItems {
	public:
		std::string title;
		std::vector<DropdownItem> items;

		DropdownItems(std::string title_ = "");

		float width();
	};

	class Dropdown : public Widget {
		bool highlighted = false;
		bool open = false;

		int selectedSection = 0;
		int selectedIndex = 0;
		int highlightedSection;
		int highlightedIndex;
	public:
		std::vector<DropdownItems> sections;

		void paint();
		virtual Vector getPreferredBounds();

		void onMouseEnter();
		void onMouseLeave();
		void onMouseDown(MouseEvent&);
		void onMouseMove(MouseEvent&);
	};
}

#endif