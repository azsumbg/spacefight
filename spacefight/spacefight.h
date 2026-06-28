#pragma once

#ifdef SPACEFIGHT_EXPORTS
#define SPACEFIGHT_API __declspec(dllexport)
#else
#define SPACEFIGHT_API __declspec(dllimport)
#endif

#include <d2d1.h>
#include <random>

#pragma comment(lib, "d2d1.lib")

constexpr float scr_width{ 1000.0f };
constexpr float scr_height{ 800.0f };

constexpr float sky{ 50.0f };
constexpr float ground{ 750.0f };

constexpr int ERR_PTR{ 5001 };
constexpr int ERR_INDEX{ 5002 };
constexpr int ERR_PARAM{ 5003 };
constexpr int ERR_UNK{ 5004 };

enum class dirs { up = 0, down = 1, left = 2, right = 3, stop = 4 };
enum class evils { fighter = 0, cruiser = 1, shuttle = 2, ship = 3 };

namespace dll
{
	class SPACEFIGHT_API EXCEPTION
	{
	private:
		int _what{ ERR_UNK };

	public:
		EXCEPTION(int what_happened);

		const wchar_t* eGet() const;
	};

	class SPACEFIGHT_API RANDIT
	{
	private:
		std::mt19937* twister{ nullptr };

	public:
		RANDIT();
		~RANDIT();

		int operator()(int min, int max);
		float operator()(float min, float max);
	};

	template<typename T> class BAG
	{
	private:
		size_t max_size{ 1 };
		size_t next_pos{ 0 };

		T* main_ptr{ nullptr };

	public:
		BAG()
		{
			main_ptr = reinterpret_cast<T*>(calloc(max_size, sizeof(T)));
			if (!main_ptr)throw EXCEPTION(ERR_PTR);
		}
		BAG(size_t lenght)
		{
			max_size = lenght;
			main_ptr = reinterpret_cast<T*>(calloc(lenght, sizeof(T)));
			if (!main_ptr)throw EXCEPTION(ERR_PTR);
		}
		BAG(BAG& other)
		{
			if (!other.main_ptr)throw EXCEPTION(ERR_PTR);

			free(main_ptr);

			max_size = other.max_size;
			next_pos = other.next_pos;

			main_ptr = reinterpret_cast<T*>(calloc(max_size, sizeof(T)));

			if (!main_ptr)throw EXCEPTION(ERR_PTR);

			if (next_pos > 0)for (size_t i = 0; i < next_pos; ++i)main_ptr[i] = other.main_ptr[i];
		}
		BAG(BAG&& other)
		{
			if (!other.main_ptr)throw EXCEPTION(ERR_PTR);

			free(main_ptr);

			max_size = other.max_size;
			next_pos = other.next_pos;
			main_ptr = other.main_ptr;

			other.main_ptr = nullptr;
		}

		~BAG()
		{
			free(main_ptr);
		}

		size_t size() const
		{
			return next_pos;
		}
		size_t capacity() const
		{
			return max_size;
		}

		bool empty()const
		{
			return(next_pos == 0);
		}

		void clear()
		{
			free(main_ptr);

			main_ptr = reinterpret_cast<T*>(calloc(1, sizeof(T)));
			max_size = 1;
			next_pos = 0;

			if (!main_ptr)throw EXCEPTION(ERR_PTR);
		}

		T& operator[](size_t index)
		{
			if (!main_ptr)throw EXCEPTION(ERR_PTR);
			else if (index >= next_pos)return main_ptr[next_pos - 1];
			
			return main_ptr[index];
		}

		BAG& operator=(BAG& other)
		{
			if (!other.main_ptr)throw EXCEPTION(ERR_PTR);
			else
			{
				free(main_ptr);

				max_size = other.max_size;
				next_pos = other.next_pos;

				main_ptr = reinterpret_cast<T*>(calloc(max_size, sizeof(T)));
				if (!main_ptr)throw EXCEPTION(ERR_PTR);
				else  if (next_pos > 0)for (size_t i = 0; i < next_pos; ++i)main_ptr[i] = other.main_ptr[i];
			}

			return *this;
		}
		BAG& operator=(BAG&& other)
		{
			if (!other.main_ptr)throw EXCEPTION(ERR_PTR);
			else
			{
				free(main_ptr);

				main_ptr = other.main_ptr;
				max_size = other.max_size;
				next_pos = other.next_pos;

				other.main_ptr = nullptr;
			}

			return *this;
		}

		void push_back(T element)
		{
			if (!main_ptr)throw EXCEPTION(ERR_PTR);
			else
			{
				if (next_pos + 1 <= max_size)
				{
					main_ptr[next_pos] = element;
					++next_pos;
				}
				else
				{
					++max_size;
					T* temp_ptr = reinterpret_cast<T*>(realloc(main_ptr, max_size * sizeof(T)));

					if (!temp_ptr)throw EXCEPTION(ERR_PTR);
					else
					{
						main_ptr = temp_ptr;
						temp_ptr = nullptr;

						main_ptr[next_pos] = element;
						++next_pos;
					}
				}
			}
		}
		void push_back(T* element)
		{
			if (!main_ptr)throw EXCEPTION(ERR_PTR);
			else
			{
				if (next_pos + 1 <= max_size)
				{
					main_ptr[next_pos] = *element;
					++next_pos;
				}
				else
				{
					++max_size;
					T* temp_ptr = reinterpret_cast<T*>(realloc(main_ptr, max_size * sizeof(T)));

					if (!temp_ptr)throw EXCEPTION(ERR_PTR);
					else
					{
						main_ptr = temp_ptr;
						temp_ptr = nullptr;

						main_ptr[next_pos] = *element;
						++next_pos;
					}
				}
			}
		}

		void insert(T element, size_t index)
		{
			if (!main_ptr)throw EXCEPTION(ERR_PTR);
			else
			{
				if (index >= next_pos)throw EXCEPTION(ERR_INDEX);
				else
				{
					if (next_pos + 1 <= max_size)
					{
						for (size_t i = next_pos; i > index; --i)main_ptr[index] = main_ptr[index - 1];
						main_ptr[index] = element;
						++next_pos;
					}
					else
					{
						++max_size;
						T* temp_ptr = reinterpret_cast<T*>(realloc(main_ptr, max_size * sizeof(T)));
						if (!temp_ptr)throw EXCEPTION(ERR_PTR);
						else
						{
							main_ptr = temp_ptr;
							temp_ptr = nullptr;

							for (size_t i = next_pos; i > index; --i)main_ptr[index] = main_ptr[index - 1];
							main_ptr[index] = element;
							++next_pos;
						}
					}
				}
			}
		}
		void insert(T* element, size_t index)
		{
			if (!main_ptr)throw EXCEPTION(ERR_PTR);
			else
			{
				if (index >= next_pos)throw EXCEPTION(ERR_INDEX);
				else
				{
					if (next_pos + 1 <= max_size)
					{
						for (size_t i = next_pos; i > index; --i)main_ptr[index] = main_ptr[index - 1];
						main_ptr[index] = *element;
						++next_pos;
					}
					else
					{
						++max_size;
						T* temp_ptr = reinterpret_cast<T*>(realloc(main_ptr, max_size * sizeof(T)));
						if (!temp_ptr)throw EXCEPTION(ERR_PTR);
						else
						{
							main_ptr = temp_ptr;
							temp_ptr = nullptr;

							for (size_t i = next_pos; i > index; --i)main_ptr[index] = main_ptr[index - 1];
							main_ptr[index] = *element;
							++next_pos;
						}
					}
				}
			}
		}
		void erase(size_t index)
		{
			if (!main_ptr)throw EXCEPTION(ERR_PTR);
			else
			{
				if (next_pos < 2)clear();
				else
				{
					if (index >= next_pos)throw EXCEPTION(ERR_INDEX);
					else
					{
						for (size_t i = index; i < next_pos - 1; ++i)main_ptr[index] = main_ptr[index + 1];
						--next_pos;
					}	
				}
			}
		}
	
		void push_front(T element)
		{
			if (!main_ptr)throw EXCEPTION(ERR_PTR);
			else
			{
				if (empty())*main_ptr = element;
				else
				{
					if (next_pos + 1 <= max_size)
					{
						for (size_t i = next_pos; i > 0; --i)main_ptr[index] = main_ptr[index - 1];
						*main_ptr = element;
						++next_pos;
					}
					else
					{
						++max_size;
						T* temp_ptr = reinterpret_cast<T*>(realloc(main_ptr, max_size * sizeof(T)));
						if (!temp_ptr)throw EXCEPTION(ERR_PTR);
						else
						{
							main_ptr = temp_ptr;
							temp_ptr = nullptr;
							for (size_t i = next_pos; i > 0; --i)main_ptr[index] = main_ptr[index - 1];
							*main_ptr = element;
							++next_pos;
						}
					}
				}
			}
		}
		void push_front(T* element)
		{
			if (!main_ptr)throw EXCEPTION(ERR_PTR);
			else
			{
				if (empty())*main_ptr = element;
				else
				{
					if (next_pos + 1 <= max_size)
					{
						for (size_t i = next_pos; i > 0; --i)main_ptr[index] = main_ptr[index - 1];
						*main_ptr = *element;
						++next_pos;
					}
					else
					{
						++max_size;
						T* temp_ptr = reinterpret_cast<T*>(realloc(main_ptr, max_size * sizeof(T)));
						if (!temp_ptr)throw EXCEPTION(ERR_PTR);
						else
						{
							main_ptr = temp_ptr;
							temp_ptr = nullptr;
							for (size_t i = next_pos; i > 0; --i)main_ptr[index] = main_ptr[index - 1];
							*main_ptr = *element;
							++next_pos;
						}
					}
				}
			}
		}

		T& front()
		{
			if (empty())throw EXCEPTION(ERR_INDEX);

			return *main_ptr;
		}
		T& back()
		{
			if (empty())throw EXCEPTION(ERR_INDEX);

			return main_ptr[next_pos - 1];
		}
	};




}