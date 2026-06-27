#include "pch.h"
#include "spacefight.h"

// EXCEPTION CLASS *******************************

dll::EXCEPTION::EXCEPTION(int what_happened) :_what{ what_happened } {};

const wchar_t* dll::EXCEPTION::eGet() const
{
	switch (_what)
	{
	case ERR_INDEX:
		return L"Error in BAG<T> container index !";

	case ERR_PARAM:
		return L"Error in BAG<T> container parameter !";

	case ERR_PTR:
		return L"Error in BAG<T> container pointer !";

	case ERR_UNK:
		return L"Unknown error occurred in BAG<T> container !";
	}

	return L"Unknown error occurred in BAG<T> container !";
}

//////////////////////////////////////////////////

// RANDIT CLASS **********************************

dll::RANDIT::RANDIT()
{
	std::random_device rd{};
	std::seed_seq sq{ rd(),rd(), rd(), rd(), rd(), rd(), rd(), rd() };

	twister = new std::mt19937(sq);
}
dll::RANDIT::~RANDIT()
{
	if (twister)delete twister;
}

int dll::RANDIT::operator()(int min, int max)
{
	if (max < min)return 0;

	std::uniform_int_distribution distrib(min, max);

	return distrib(*twister);
}
float dll::RANDIT::operator()(float min, float max)
{
	if (max < min)return 0;

	std::uniform_real_distribution<float> distrib(min, max);

	return distrib(*twister);
}

//////////////////////////////////////////////////