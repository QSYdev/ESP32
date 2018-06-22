#pragma once
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
		static const int AND_INT_VALUE;
		static const int OR_INT_VALUE;
		static const int OPEN_PARENTHESIS_INT_VALUE;
		static const int CLOSE_PARENTHESIS_INT_VALUE;

		static void fromInfixToPostfix(const std::string& expression, std::list<int>& queue);
		static int getPrior(int c);
		static void fromStringToIntArray(const std::string& expression, std::list<int>& result);
		static int fromIntBufferToInt(std::list<int>& buffer);

	};

private:
	ExpressionNode* mRoot;

private:
	bool evaluateExpression(const ExpressionNode* node, const bool* touchedNodes);

public:
	ExpressionTree(const std::string& expression);
	~ExpressionTree();

	bool evaluateExpression(const bool* touchedNodes);

};