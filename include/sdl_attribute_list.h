#ifndef SDL_ATTRIBUTE_LIST_H
#define SDL_ATTRIBUTE_LIST_H

#include <sdl_parser_structure.h>

#include <vector>

namespace piranha {

	class SdlAttribute;
	class SdlAttributeDefinition;

	class SdlAttributeList : public SdlParserStructure {
	public:
		SdlAttributeList();
		~SdlAttributeList();

		void addAttribute(SdlAttribute *attribute);
		SdlAttribute *getAttribute(int index) const { return m_attributes[index]; }
		SdlAttribute *getAttribute(SdlAttributeDefinition *definition) const;
		int getAttributeCount() const { return (int)m_attributes.size(); }

	protected:
		std::vector<SdlAttribute *> m_attributes;
	};

} /* namespace piranha */

#endif /* SDL_ATTRIBUTE_LIST_H */