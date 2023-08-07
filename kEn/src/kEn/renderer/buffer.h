#pragma once

#define SHADER_FLOAT_SIZE 4
#define SHADER_INT_SIZE 4
#define SHADER_BOOL_SIZE 1

// X-macro of form X(type, size, component count)
#define DATA_TYPES(X)						\
	X(float_,         SHADER_FLOAT_SIZE, 1)	\
	X(float2,     2 * SHADER_FLOAT_SIZE, 2)	\
	X(float3,     3 * SHADER_FLOAT_SIZE, 3)	\
	X(float4,     4 * SHADER_FLOAT_SIZE, 4)	\
	X(mat3,   3 * 3 * SHADER_FLOAT_SIZE, 3)	\
	X(mat4,   4 * 4 * SHADER_FLOAT_SIZE, 4)	\
	X(int_,           SHADER_INT_SIZE,   1)	\
	X(int2,       2 * SHADER_INT_SIZE,   2)	\
	X(int3,       3 * SHADER_INT_SIZE,   3)	\
	X(int4,       4 * SHADER_INT_SIZE,   4) \
	X(bool_,		  SHADER_BOOL_SIZE,  1)			

#include "kEn/core/assert.h"

namespace kEn
{
	using shader_data_type = uint8_t;

	namespace shader_data_types
	{
#		define ENUM_ENTRY(id, size, comps)	id, 
#		define SIZE_ENTRY(id, size, comps)	case id: return size;
#		define COMPS_ENTRY(id, size, comps)	case id: return comps;

		enum : shader_data_type
		{
			none = 0,
			DATA_TYPES(ENUM_ENTRY)
		};

		inline uint32_t get_size(shader_data_type type)
		{
			switch(type)
			{
				DATA_TYPES(SIZE_ENTRY)
			}

			KEN_CORE_ASSERT(false, "Unknown shader data type!");
			return 0;
		}

		inline uint32_t get_component_count(shader_data_type type)
		{
			switch (type)
			{
				DATA_TYPES(COMPS_ENTRY)
			}

			KEN_CORE_ASSERT(false, "Unknown shader data type!");
			return 0;
		}

#		undef ENUM_ENTRY
#		undef SIZE_ENTRY
#		undef COMPS_ENTRY
	}

	struct buffer_element
	{
		std::string name;
		shader_data_type type;
		uint32_t size;
		size_t offset;
		bool normalized;

		buffer_element() = default;

		buffer_element(shader_data_type type, std::string name, bool normalized = false)
			: name(std::move(name)), type(type), size(shader_data_types::get_size(type)), offset(0), normalized(normalized)
		{
		}
	};

	class buffer_layout
	{
	public:
		buffer_layout() = default;

		buffer_layout(std::initializer_list<buffer_element> elements)
			: elements_(elements)
		{
			size_t offset = 0;
			for(auto& element : elements_)
			{
				element.offset = offset;
				offset += element.size;
			}

			stride_ = static_cast<uint32_t>(offset);
		}

		uint32_t stride() const { return stride_; }
		const std::vector<buffer_element>& elements() const	{ return elements_;	}

		std::vector<buffer_element>::iterator begin() { return elements_.begin(); }
		std::vector<buffer_element>::iterator end() { return elements_.end(); }
		std::vector<buffer_element>::const_iterator begin() const { return elements_.begin(); }
		std::vector<buffer_element>::const_iterator end() const { return elements_.end(); }

	private:
		std::vector<buffer_element> elements_;
		uint32_t stride_ = 0;
	};

	class vertex_buffer
	{
	public:
		virtual ~vertex_buffer() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual const buffer_layout& layout() const = 0;
		virtual void set_layout(const buffer_layout& layout) = 0;

		static std::shared_ptr<vertex_buffer> create(float* vertices, uint32_t size);
	};

	class index_buffer
	{
	public:
		virtual ~index_buffer() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual uint32_t get_count() const = 0;

		static std::shared_ptr<index_buffer> create(uint32_t* indices, uint32_t size);
	};
}