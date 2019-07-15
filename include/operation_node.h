#ifndef PIRANHA_OPERATION_NODE_H
#define PIRANHA_OPERATION_NODE_H

#include "node.h"

#include "add_operation_output.h"
#include "fundamental_output.h"

namespace piranha {

	class OperationNode : public Node {
	public:
		OperationNode() {}
		~OperationNode() {}

		virtual void connectLeft(pNodeInput input) {
			(*m_inputs[0].input) = input;
		}

		virtual void connectRight(pNodeInput input) {
			(*m_inputs[1].input) = input;
		}
	};

	template <typename FundamentalType>
	class OperationNodeSpecialized : public OperationNode {
	public:
		OperationNodeSpecialized() {
			/* void */
		}

		~OperationNodeSpecialized() {
			/* void */
		}

		virtual void _initialize() {
			m_output.initialize();
		}

		virtual void _evaluate() {
			/* void */
		}

		virtual void _destroy() {
			/* void */
		}

		virtual void registerOutputs() {
			setPrimaryOutput(&m_output);
			registerOutput(&m_output, "$primary");
		}

		virtual void registerInputs() {
			const ChannelType *type = NativeTypeLookup<FundamentalType>();

			registerInput(
				m_output.getLeftConnection(), "$left", type);
			registerInput(
				m_output.getRightConnection(), "$right", type);
		}

	protected:
		AddOperationNodeOutput<FundamentalType> m_output;
	};

} /* namespace piranha */

#endif /* PIRANHA_OPERATION_NODE_H */
