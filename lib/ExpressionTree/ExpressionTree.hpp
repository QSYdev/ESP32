#include <list>
#include <math.h>
#include <string>

class ExpressionTree
{
private:
	class ExpressionNode
	{
	public:
		int mValue;
		ExpressionNode* mLeftNode;
		ExpressionNode* mRightNode;

		ExpressionNode(int value);
		ExpressionNode(int value, ExpressionNode* leftNode, ExpressionNode* rightNode);
		~ExpressionNode();

		inline bool isLeaf() const { return mLeftNode == nullptr && mRightNode == nullptr;  }

	};

	class Utils
	{
	public:
		static constexpr int AND_INT_VALUE = -1;
		static constexpr int OR_INT_VALUE = -2;
		static constexpr int OPEN_PARENTHESIS_INT_VALUE = -3;
		static constexpr int CLOSE_PARENTHESIS_INT_VALUE = -4;

		static void fromInfixToPostfix(std::string& expression, std::list<int>& queue);
		static int getPrior(int c);
		static void fromStringToIntArray(std::string& expression, std::list<int>& result);
		static int fromIntBufferToInt(std::list<int>& buffer);

	};

private:
	ExpressionNode* mRoot;

private:
	bool evaluateExpression(ExpressionNode* node, bool* touchedNodes);

public:
	ExpressionTree(std::string& expression);
	~ExpressionTree();

	bool evaluateExpression(bool* touchedNodes);

};