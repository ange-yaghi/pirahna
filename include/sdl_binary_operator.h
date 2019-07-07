#ifndef SDL_BINARY_OPERATOR_H
#define SDL_BINARY_OPERATOR_H

#include <sdl_value.h>

namespace piranha {

	class SdlCompilationError;

	class SdlBinaryOperator : public SdlValue {
	public:
		enum OPERATOR {
			DOT,
			POINTER,
			ADD,
			SUB,
			MUL,
			DIV
		};

	public:
		SdlBinaryOperator(OPERATOR op, SdlValue *leftOperand, SdlValue *rightOperand);
		~SdlBinaryOperator();

		OPERATOR getOperator() const { return m_operator; }
		SdlValue *getLeft() const { return m_leftOperand; }
		SdlValue *getRight() const { return m_rightOperand; }

		virtual SdlParserStructure *getImmediateReference(const SdlReferenceQuery &query, SdlReferenceInfo *output);

	protected:
		virtual NodeOutput *_generateNodeOutput(SdlContextTree *context, NodeProgram *program);
		virtual Node *_generateNode(SdlContextTree *context, NodeProgram *program);

	protected:
		OPERATOR m_operator;
		SdlValue *m_leftOperand;
		SdlValue *m_rightOperand;
	};

} /* namespace piranha */

#endif /* SDL_BINARY_OPERATOR_H */
