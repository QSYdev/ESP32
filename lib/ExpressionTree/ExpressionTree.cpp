#include <ExpressionTree.hpp>

const int ExpressionTree::Utils::AND_INT_VALUE = -1;
const int ExpressionTree::Utils::OR_INT_VALUE = -2;
const int ExpressionTree::Utils::OPEN_PARENTHESIS_INT_VALUE = -3;
const int ExpressionTree::Utils::CLOSE_PARENTHESIS_INT_VALUE = -4;

ExpressionTree::ExpressionNode::ExpressionNode(int value)
	:ExpressionNode(value, nullptr, nullptr)
{
}

ExpressionTree::ExpressionNode::ExpressionNode(int value, ExpressionNode* leftNode, ExpressionNode* rightNode)
	:mValue(value), mLeftNode(leftNode), mRightNode(rightNode)
{
}

ExpressionTree::ExpressionNode::~ExpressionNode()
{
	delete mLeftNode;
	delete mRightNode;
}

void ExpressionTree::Utils::fromInfixToPostfix(const std::string& expression, std::list<int>& queue)
{
	std::list<int> stack;
	std::list<int> exp;

	fromStringToIntArray(expression, exp);

	for (int& c : exp)
	{
		if (c >= 0)
		{
			queue.push_back(c);
		}
		else if (c == AND_INT_VALUE || c == OR_INT_VALUE)
		{
			while (!stack.empty() && getPrior(c) <= getPrior(stack.front()))
			{
				queue.push_back(stack.front());
				stack.pop_front();
			}
			stack.push_front(c);
		}
		else if (c == OPEN_PARENTHESIS_INT_VALUE)
		{
			stack.push_front(c);
		}
		else if (c == CLOSE_PARENTHESIS_INT_VALUE)
		{
			int elem = 0;
			while (!stack.empty() && (elem = stack.front()) != OPEN_PARENTHESIS_INT_VALUE)
			{
				queue.push_back(elem);
				stack.pop_front();
			}

			if (!stack.empty())
				stack.pop_front();

			if (elem != Utils::OPEN_PARENTHESIS_INT_VALUE)
				throw "La expresion es invalida";
		}
		else
		{
			throw "La expresion es invalida";
		}
	}

	for (int& elem : stack)
	{
		if (elem != OPEN_PARENTHESIS_INT_VALUE)
			queue.push_back(elem);
		else
			throw "La expresion es invalida";
	}
}

int ExpressionTree::Utils::getPrior(int c)
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

void ExpressionTree::Utils::fromStringToIntArray(const std::string& expression, std::list<int>& result)
{
	std::list<int> number;
	for (const char& value : expression)
	{
		if (value >= '0' && value <= '9')
		{
			number.push_back(value - 48);
		}
		else
		{
			if (!number.empty())
			{
				int num = fromIntBufferToInt(number);
				result.push_back(num);
				number.clear();
			}

			if (value == '&')
			{
				result.push_back(AND_INT_VALUE);
			}
			else if (value == '|')
			{
				result.push_back(OR_INT_VALUE);
			}
			else if (value == '(')
			{
				result.push_back(OPEN_PARENTHESIS_INT_VALUE);
			}
			else if (value == ')')
			{
				result.push_back(CLOSE_PARENTHESIS_INT_VALUE);
			}
			else if (value == ' ')
			{

			}
			else
			{
				throw "Expresion invalida";
			}
		}
	}

	if (!number.empty())
	{
		int num = fromIntBufferToInt(number);
		result.push_back(num);
		number.clear();
	}
}

int ExpressionTree::Utils::fromIntBufferToInt(std::list<int>& buffer)
{
	int result = 0;
	int p = buffer.size() - 1;
	for (int& n : buffer)
		result += pow(10, p--) * n;

	return result;
}

ExpressionTree::ExpressionTree(const std::string& expression)
{
	std::list<ExpressionNode*> stack;
	std::list<int> exp;
	Utils::fromInfixToPostfix(expression, exp);

	for (int& value : exp)
	{
		if (value == Utils::AND_INT_VALUE || value == Utils::OR_INT_VALUE)
		{
			ExpressionNode* expN1 = stack.front();
			stack.pop_front();

			ExpressionNode* expN2 = stack.front();
			stack.pop_front();

			ExpressionNode* node = new ExpressionNode(value, expN1, expN2);
			stack.push_front(node);
		}
		else
		{
			stack.push_front(new ExpressionNode(value));
		}
	}

	mRoot = stack.front();
}

ExpressionTree::~ExpressionTree()
{
	delete mRoot;
}

bool ExpressionTree::evaluateExpression(const bool* touchedNodes)
{
	return evaluateExpression(mRoot, touchedNodes);
}

bool ExpressionTree::evaluateExpression(const ExpressionNode* node, const bool* touchedNodes)
{
	if (node->isLeaf())
	{
		uint16_t nodeId = node->mValue;
		return touchedNodes[nodeId];
	}
	else
	{
		switch (node->mValue) {
			case Utils::AND_INT_VALUE: 
			{
				return evaluateExpression(node->mLeftNode, touchedNodes) && evaluateExpression(node->mRightNode, touchedNodes);
			}
			case Utils::OR_INT_VALUE:
			{
				return evaluateExpression(node->mLeftNode, touchedNodes) || evaluateExpression(node->mRightNode, touchedNodes);
			}
			default:
			{
				return false;
			}
		}
	}
}

