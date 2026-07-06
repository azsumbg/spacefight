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

// PROTON CLASS **********************************

dll::PROTON::PROTON()
{
	_width = 1.0f;
	_height = 1.0f;

	x_rad = _width / 2.0f;
	y_rad = _height / 2.0f;

	end.x = start.x + _width;
	end.y = start.y + _height;

	center.x = start.x + x_rad;
	center.y = start.y + y_rad;

	my_rect.left = start.x;
	my_rect.right = end.x;
	my_rect.top = start.y;
	my_rect.bottom = end.y;
}
dll::PROTON::PROTON(D2D1_POINT_2F start_point)
{
	start.x = start_point.x;
	start.y = start_point.y;

	_width = 1.0f;
	_height = 1.0f;

	x_rad = _width / 2.0f;
	y_rad = _height / 2.0f;

	end.x = start.x + _width;
	end.y = start.y + _height;

	center.x = start.x + x_rad;
	center.y = start.y + y_rad;

	my_rect.left = start.x;
	my_rect.right = end.x;
	my_rect.top = start.y;
	my_rect.bottom = end.y;
}
dll::PROTON::PROTON(D2D1_POINT_2F start_point, float first_width, float first_height)
{
	start.x = start_point.x;
	start.y = start_point.y;

	_width = first_width;
	_height = first_height;

	x_rad = _width / 2.0f;
	y_rad = _height / 2.0f;

	end.x = start.x + _width;
	end.y = start.y + _height;

	center.x = start.x + x_rad;
	center.y = start.y + y_rad;

	my_rect.left = start.x;
	my_rect.right = end.x;
	my_rect.top = start.y;
	my_rect.bottom = end.y;
}
dll::PROTON::PROTON(float first_x, float first_y, float first_width, float first_height)
{
	start.x = first_x;
	start.y = first_y;

	_width = first_width;
	_height = first_height;

	x_rad = _width / 2.0f;
	y_rad = _height / 2.0f;

	end.x = start.x + _width;
	end.y = start.y + _height;

	center.x = start.x + x_rad;
	center.y = start.y + y_rad;

	my_rect.left = start.x;
	my_rect.right = end.x;
	my_rect.top = start.y;
	my_rect.bottom = end.y;
}

dll::PROTON::~PROTON()
{
	if (in_heap)delete this;
}

float dll::PROTON::get_width() const
{
	return _width;
}
float dll::PROTON::get_height() const
{
	return _height;
}

void dll::PROTON::set_edges()
{
	end.x = start.x + _width;
	end.y = start.y + _height;

	center.x = start.x + x_rad;
	center.y = start.y + y_rad;

	my_rect.left = start.x;
	my_rect.right = end.x;
	my_rect.top = start.y;
	my_rect.bottom = end.y;
}

void dll::PROTON::set_width(float new_width)
{
	_width = new_width;
	
	x_rad = _width / 2.0f;
	
	end.x = start.x + _width;
	
	center.x = start.x + x_rad;
	
	my_rect.left = start.x;
	my_rect.right = end.x;
}
void dll::PROTON::set_height(float new_height)
{
	_height = new_height;

	y_rad = _height / 2.0f;

	end.y = start.y + _height;

	center.y = start.y + y_rad;

	my_rect.top = start.y;
	my_rect.bottom = end.y;
}
void dll::PROTON::new_dims(float new_width, float new_height)
{
	_width = new_width;
	_height = new_height;

	x_rad = _width / 2.0f;
	y_rad = _height / 2.0f;

	end.x = start.x + _width;
	end.y = start.y + _height;

	center.x = start.x + x_rad;
	center.y = start.y + y_rad;

	my_rect.left = start.x;
	my_rect.right = end.x;
	my_rect.top = start.y;
	my_rect.bottom = end.y;
}

dll::PROTON* dll::PROTON::create(float first_x, float first_y, float first_width, float first_height)
{
	PROTON* ret = new PROTON(first_x, first_y, first_width, first_height);
	
	ret->in_heap = true;

	return ret;
}

//////////////////////////////////////////////////

// BACKGROUND CLASS ******************************

dll::BACKGROUND::BACKGROUND(background what) :PROTON(D2D1_POINT_2F(0, 0), scr_width, scr_height)
{
	type = what;

	max_frames = 48;
	
	if (type == background::field)
	{
		start.y = 50.0f;
		set_edges();
		max_frames = 49;
	}
}

int dll::BACKGROUND::frame()
{
	--frame_delay;

	if (frame_delay <= 0)
	{
		frame_delay = max_frame_delay;
		++current_frame;
		if (current_frame >= max_frames)current_frame = 0;
	}

	return current_frame;
}

//////////////////////////////////////////////////


















// FUNCTIONS **********************************

float dll::distance(D2D1_POINT_2F first, D2D1_POINT_2F second)
{
	float a = (float)(pow(abs(second.x - first.x), 2));
	float b = (float)(pow(abs(second.y - first.y), 2));

	return (float)(sqrt(a + b));
}

float dll::rotate_angle(dirs to_where, float current_angle)
{
	float rad_angle{ current_angle * 3.14f / 180.0f };

	if (to_where == dirs::up && rad_angle <= 45.0f) ++rad_angle;
	else --rad_angle;

	return rad_angle;
}

bool dll::intersect(D2D1_RECT_F first, D2D1_RECT_F second)
{
	if (!(first.left >= second.right || first.right <= second.left || first.top >= second.bottom || first.bottom <= second.top))
		return true;

	return false;
}

bool dll::intersect(D2D1_POINT_2F first_center, D2D1_POINT_2F second_center,
	float first_xrad, float second_xrad, float first_yrad, float second_yrad)
{
	if (abs(second_center.x - first_center.x) <= first_xrad + second_xrad &&
		abs(second_center.y - first_center.y) <= first_yrad + second_yrad)return true;

	return false;
}