#ifndef STRING_NODE_OUTPUT_H
#define STRING_NODE_OUTPUT_H

#include <node_output.h>

namespace piranha {

	class StringNodeOutput : public NodeOutput {
	public:
		static const NodeType StringType;

	public:
		StringNodeOutput();
		virtual ~StringNodeOutput();
	};

} /* namespace piranha */

#endif /* STRING_NODE_OUTPUT_H */