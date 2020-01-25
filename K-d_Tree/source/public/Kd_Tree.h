#pragma once
#include <cstdint>
#include <stdarg.h>

typedef int32_t int32;

template<typename t>
struct Kd_Point
{
	Kd_Point(const int32& dimensions)
		:m_dimensions(dimensions)
	{
		m_values = new t[m_dimensions];
	}

	Kd_Point(const int32& dimensions, t args, ...)
		:m_dimensions(dimensions)
	{
		m_values = new t[m_dimensions];

		va_list values;
		va_start(values, args);

		for (int32 i = 0; i < dimensions; ++i)
		{
			t nextArg = va_arg(values, t);
			m_values[i] = nextArg;
		}
		va_end(values);
	}

	void ChangeValue(const int32& dimension, const t& value)
	{
		if (dimension >= m_dimensions)
		{
			static_assert("The dimension you want to change is higher than the dimension of the point");
			return;
		}
		m_values[dimension] = value;
	}

	t* m_values;
	int32 m_dimensions;
};

template <typename t>
struct Kd_Node
{
	Kd_Node(const int32& dimensions, const int32& alignment, const Kd_Point<t>& value)
		:m_dimensions(dimensions)
		, m_alignment(alignment)
		,m_smallerChildNode(nullptr)
		,m_greaterChildNode(nullptr)
		,m_point(value)
	{
	}

	Kd_Node(const Kd_Point<t>& initVector, const int32& alignment)
		:m_dimensions(initVector.m_dimensions)
		,m_alignment(alignment)
		,m_smallerChildNode(nullptr)
		,m_greaterChildNode(nullptr)
		,m_point(initVector)
	{

	}

	int32 GetNextAlignment() const
	{
		if (m_alignment + 1 < m_dimensions)
		{
			return m_alignment + 1;
		}
		return 0;
	}

	void Create(const Kd_Point<t>& nextPoint, const int32& alignment)
	{
		int32 nextAlignment = GetNextAlignment();
		if(nextPoint.m_values[alignment] < m_point.m_values[alignment])
		{
			if (m_smallerChildNode)
			{
				m_smallerChildNode->Create(nextPoint, nextAlignment);
				return;
			}
			m_smallerChildNode = new Kd_Node(nextPoint, nextAlignment);
			return;
		}
		if (m_greaterChildNode)
		{
			m_greaterChildNode->Create(nextPoint, nextAlignment);
			return;
		}
		m_greaterChildNode = new Kd_Node(nextPoint, nextAlignment);
	}

	~Kd_Node()
	{
		if (m_smallerChildNode)
		{
			delete m_smallerChildNode;
		}
		if (m_greaterChildNode)
		{
			delete m_greaterChildNode;
		}
	}

	int32 m_dimensions;
	int32 m_alignment;
	Kd_Node<t>* m_smallerChildNode;
	Kd_Node<t>* m_greaterChildNode;
	Kd_Point<t> m_point;
};

template <typename t>
class Kd_Tree
{
public:
	Kd_Tree() = delete;
	Kd_Tree(const int32& dimensions)
		:m_dimensions(dimensions)
		,m_baseNode(nullptr)
	{

	}

	Kd_Tree(const Kd_Point<t>& initVector)
		:m_dimensions(initVector.m_dimensions)
		,m_baseNode(nullptr)
	{
		m_baseNode = new Kd_Node(initVector);
	}

	Kd_Tree(const Kd_Tree& other)
		:m_dimensions(other.m_dimensions)
		,m_baseNode(other.m_baseNode)
	{

	}

	void Add(const Kd_Point<t>& nextPoint)
	{
		if (m_baseNode)
		{
			m_baseNode->Create(nextPoint, m_baseNode->GetNextAlignment());
			return;
		}
		m_baseNode = new Kd_Node<t>(m_dimensions, 0, nextPoint);
	}

	Kd_Tree(Kd_Tree<t>&& other) noexcept
		:m_dimensions(0)
		,m_baseNode(nullptr)
	{
		if (this == &other)
		{
			return;
		}
		this->m_baseNode = other.m_baseNode;
		this->m_dimensions = other.m_dimensions;
		other.m_baseNode = nullptr;
	}
	~Kd_Tree()
	{
		delete m_baseNode;
	}
private:
	int32 m_dimensions;
	Kd_Node<t>* m_baseNode;
};