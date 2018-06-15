#pragma once

template<class T>
class ExpressionTree
{
private:
	class ExpressionNode
	{
	public:
		T mData;
		ExpressionNode* mLeftNode;
		ExpressionNode* mRightNode;

		inline ExpressionNode(T data)
			:mData(data), mLeftNode(nullptr), mRightNode(nullptr)
		{
		}

		inline ExpressionNode(T data, ExpressionNode* leftNode, ExpressionNode* rightNode)
			: mData(data), mLeftNode(leftNode), mRightNode(rightNode)
		{
		}

		inline ~ExpressionNode()
		{
			delete mLeftNode;
			delete mRightNode;
		}

		inline bool isLeaf() const { return mLeftNode == nullptr && mRightNode == nullptr;  }
	};

	class Utils
	{
	public:
		static constexpr int AND_INT_VALUE = -1;
		static constexpr int OR_INT_VALUE = -2;
		static constexpr int OPEN_PARENTHESIS_INT_VALUE = -3;
		static constexpr int CLOSE_PARENTHESIS_INT_VALUE = -4;

		static uint16_t* fromInfixToPostfix(char* expression, int expressionLength, int* resultLength)
		{
			List<int> stack;
			List<int> queue;

			uint16_t* exp = fromStringToIntArray(expression);

			byte index = 0;
			while (index < exp.length) {
				int c = exp[index++];
				if (c >= QSYPacket.MIN_ID_SIZE && c <= QSYPacket.MAX_ID_SIZE) {
					queue.add(c);
				}
				else if (c == AND_INT_VALUE || c == OR_INT_VALUE) {
					while (!stack.isEmpty() && getPrior(c) <= getPrior(stack.peek())) {
						queue.add(stack.pop());
					}
					stack.push(c);
				}
				else if (c == OPEN_PARENTHESIS_INT_VALUE) {
					stack.push(c);
				}
				else if (c == CLOSE_PARENTHESIS_INT_VALUE) {
					int elem = 0;
					while (!stack.isEmpty() && (elem = stack.pop()) != OPEN_PARENTHESIS_INT_VALUE) {
						queue.add(elem);
					}
					if (elem != OPEN_PARENTHESIS_INT_VALUE) {
						throw new IllegalArgumentException("La expresion '" + expression + "' es invalida.");
					}
				}
				else {
					throw new IllegalArgumentException("La expresion '" + expression + "' es invalida.");
				}
			}
			while (!stack.isEmpty()) {
				int elem;
				if ((elem = stack.pop()) != OPEN_PARENTHESIS_INT_VALUE) {
					queue.add(elem);
				}
				else {
					throw new IllegalArgumentException("La expresion '" + expression + "' es invalida.");
				}
			}

			return toIntArray(queue);
		}

		static int getPrior(int c)
		{
			switch (c)
			{
				case OR_INT_VALUE:
				{
					return 1;
				}
				case AND_INT_VALUE:
				{
					return 2;
				}
				case OPEN_PARENTHESIS_INT_VALUE:
				{
					return 0;
				}
				default:
				{
					return -1;
				}
			}
		}

		static int* fromStringToIntArray(char* expression, int expressionLength)
		{
			List<int> queue;
			List<int> number;
			int index = 0;
			while (index < expressionLength) {
				char value = expression[index++];
				if (value >= '0' && value <= '9')
				{
					number.add(value - 48);
				}
				else
				{
					if (!number.isEmpty()) {
						int num = fromIntBufferToInt(number);
						queue.add(num);
						number.clear();
					}
					if (value == '&')
					{
						queue.add(AND_INT_VALUE);
					}
					else if (value == '|')
					{
						queue.add(OR_INT_VALUE);
					}
					else if (value == '(')
					{
						queue.add(OPEN_PARENTHESIS_INT_VALUE);
					}
					else if (value == ')')
					{
						queue.add(CLOSE_PARENTHESIS_INT_VALUE);
					}
				}
			}
			if (!number.isEmpty())
			{
				int num = fromIntBufferToInt(number);
				queue.add(num);
				number.clear();
			}

			return toIntArray(queue);
		}

		static int fromIntBufferToInt(List<int>& number)
		{
			int result = 0;
			int pow = number.size() - 1;
			number.begin();
			while (!number.end())
				result += pow(10, pow--) * number.next();

			return result;
		}

		static uint16_t* toIntArray(List<int>& list)
		{
			uint16_t* result = new uint16_t[list.size()];
			int i = 0;
			list.begin();
			while (!list.end())
				result[i++] = list.next();

			return result;
		}


	};
};