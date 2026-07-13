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

// CREATURES CLASS *******************************

dll::CREATURES::CREATURES(creatures _what, float _sx, float _sy) :PROTON(D2D1_POINT_2F(_sx, _sy))
{
	type = _what;

	switch (type)
	{
	case creatures::hero:
		new_dims(64.0f, 80.0f);
		speed = 3.0f;
		max_frames = 5;
		frame_delay = 13;
		armor = 1;
		lifes = 150;
		strenght = 20;
		break;

	case creatures::shot:
		new_dims(13.0f, 30.0f);
		speed = 8.0f;
		max_frames = 11;
		frame_delay = 6;
		break;

	case creatures::fighter:
		new_dims(100.0f, 40.0f);
		speed = 1.0f;
		armor = 1;
		lifes = 100;
		strenght = 10;
		attack_delay = 80;
		view_range = 300;
		break;

	case creatures::cruiser:
		new_dims(102.0f, 100.0f);
		speed = 0.7f;
		armor = 4;
		lifes = 200;
		strenght = 20;
		attack_delay = 100;
		view_range = 350;
		break;

	case creatures::ship:
		new_dims(100.0f, 83.0f);
		speed = 0.8f;
		armor = 3;
		lifes = 150;
		strenght = 15;
		attack_delay = 90;
		view_range = 250;
		break;

	case creatures::shuttle:
		new_dims(95.0f, 67.0f);
		speed = 1.4f;
		armor = 1;
		lifes = 80;
		strenght = 8;
		attack_delay = 70;
		view_range = 200;
		break;
	}

	max_frame_delay = frame_delay;
}

void dll::CREATURES::set_path(float targ_x, float targ_y)
{
	hor_dir = false;
	ver_dir = false;

	move_sx = start.x;
	move_ex = targ_x;

	move_sy = start.y;
	move_ey = targ_y;

	if ((move_sx == move_ex) || (move_ex > start.x && move_ex <= end.x))
	{
		ver_dir = true;
		return;
	}
	if ((move_sy == move_ey) || (move_ey > start.y && move_ey <= end.y))
	{
		hor_dir = true;
		return;
	}

	slope = (move_ey - move_sy) / (move_ex - move_sx);
	intercept = start.y - slope * start.x;
}

void dll::CREATURES::move(float gear)
{
	if (type == creatures::shot)return;

	float my_speed = speed + gear / 10.0f;

	if (ver_dir)
	{
		if (move_ey < move_sy)
		{
			dir = dirs::up;

			if (start.y - my_speed >= sky)
			{
				start.y -= my_speed;
				set_edges();
				if (start.y <= move_ey)hero_moving = false;
			}
			else hero_moving = false;
			
		}
		else if (move_ey > move_sy)
		{
			dir = dirs::down;
			
			if (end.y + my_speed <= ground)
			{
				start.y += my_speed;
				set_edges();
				if (end.y >= move_ey)hero_moving = false;
			}
			else hero_moving = false;
		}
		else hero_moving = false;
	}
	else if (hor_dir)
	{
		if (move_ex < move_sx)
		{
			dir = dirs::left;

			if (start.x - my_speed >= 0)
			{
				start.x -= my_speed;
				set_edges();
				if (start.x <= move_ex)hero_moving = false;
			}
			else hero_moving = false;
		}
		else if (move_ex > move_sx)
		{
			dir = dirs::right;

			if (end.x + my_speed <= scr_width)
			{
				start.x += my_speed;
				set_edges();
				if (end.x >= move_ex)hero_moving = false;
			}
			else hero_moving = false;
		}
		else hero_moving = false;
	}
	else
	{
		if (move_ex < move_sx)
		{
			dir = dirs::left;

			if (start.x - my_speed >= 0)
			{
				start.x -= my_speed;
				start.y = start.x * slope + intercept;
				set_edges();

				if (start.x <= move_ex)hero_moving = false;

				if (start.x < 0)
				{
					start.x = 0;
					set_edges();
					hero_moving = false;
				}
				if (start.y < sky)
				{
					start.y = sky;
					set_edges();
					hero_moving = false;
				}
				if (end.y > ground)
				{
					start.y = ground - get_height();
					set_edges();
					hero_moving = false;
				}
			}
			else hero_moving = false;
		}
		else if (move_ex > move_sx)
		{
			dir = dirs::right;

			if (end.x + my_speed <= scr_width)
			{
				start.x += my_speed;
				start.y = start.x * slope + intercept;
				set_edges();

				if (end.x >= move_ex)hero_moving = false;

				if (end.x > scr_width)
				{
					start.x = scr_width - get_width();
					set_edges();
					hero_moving = false;
				}
				if (start.y < sky)
				{
					start.y = sky;
					set_edges();
					hero_moving = false;
				}
				if (end.y > ground)
				{
					start.y = ground - get_height();
					set_edges();
					hero_moving = false;
				}
			}
			else hero_moving = false;
		}
		else hero_moving = false;
	}
}

bool dll::CREATURES::shot_move(float gear)
{
	if (type != creatures::shot)return false;

	float my_speed = speed + gear / 10.0f;

	if (ver_dir)
	{
		if (move_ey < move_sy)
		{
			if (start.y - my_speed >= sky)
			{
				start.y -= my_speed;
				set_edges();
			}
			else return false;
		}
		else if (move_ey > move_sy)
		{
			if (end.y + my_speed <= ground)
			{
				start.y += my_speed;
				set_edges();
			}
			else return false;
		}
		else return false;
	}
	else if (hor_dir)
	{
		if (move_ex < move_sx)
		{
			if (start.x - my_speed >= 0)
			{
				start.x -= my_speed;
				set_edges();
			}
			else return false;
		}
		else if (move_ex > move_sx)
		{
			if (end.x + my_speed <= scr_width)
			{
				start.x += my_speed;
				set_edges();
			}
			else return false;
		}
		else return false;
	}
	else
	{
		if (move_ex < move_sx)
		{
			if (start.x - my_speed >= 0)
			{
				start.x -= my_speed;
				start.y = start.x * slope + intercept;
				set_edges();

				if (start.x < 0 || start.y < sky || end.y > ground || end.x > scr_width)return false;
			}
			else return false;
		}
		else if (move_ex > move_sx)
		{
			if (end.x + my_speed <= scr_width)
			{
				start.x += my_speed;
				start.y = start.x * slope + intercept;
				set_edges();

				if (start.x < 0 || start.y < sky || end.y > ground || end.x > scr_width)return false;
			}
			else return false;
		}
		else return false;
	}

	return true;
}

int dll::CREATURES::attack()
{
	--attack_delay;

	if (attack_delay <= 0)
	{
		attack_delay = max_attack_delay;
		return strenght;
	}

	return 0;
}

int dll::CREATURES::get_frame()
{
	--frame_delay;
	if (frame_delay <= 0)
	{
		frame_delay = max_frame_delay;
		++current_frame;
		if (current_frame > max_frames)current_frame = 0;
	}

	return current_frame;
}

float dll::CREATURES::get_move_target_x() const
{
	return move_ex;
}

float dll::CREATURES::get_move_target_y() const
{
	return move_ey;
}

float dll::CREATURES::get_view_range() const
{
	return view_range;

}

void dll::CREATURES::Release()
{
	delete this;
}

dll::CREATURES* dll::CREATURES::create(creatures what, float sx, float sy)
{
	CREATURES* ret{ new CREATURES(what, sx, sy) };

	return ret;
}

float dll::CREATURES::rotate_angle(float oppos, float adjanced)
{
	float ret_angle = std::atan2(oppos, adjanced) * 180.0f / 3.14f;

	if (move_ey < center.y)
	{
		if (move_ex < center.x)ret_angle = 360.0f - ret_angle;
	}
	else if (move_ey > center.y)
	{
		if (move_ex > center.x)ret_angle = 180.0f - ret_angle;
		else if (move_ex < center.x)ret_angle = 180.0f + ret_angle;
	}

	return ret_angle;
}

//////////////////////////////////////////////////

// METEORS CLASS**********************************

dll::METEORS::METEORS(meteors _what, float _sx, float _sy) :PROTON(D2D1_POINT_2F(_sx, _sy))
{
	type = _what;

	switch (type)
	{
	case meteors::meteor1:
		new_dims(250.0f, 273.0f);
		speed = 0.3f;
		lifes = 500;
		max_frames = 9;
		frame_delay = 7;
		break;

	case meteors::meteor2:
		new_dims(150.0f, 71.0f);
		lifes = 250;
		max_frames = 4;
		frame_delay = 14;
		break;

	case meteors::meteor3:
		new_dims(192.0f, 190.0f);
		speed = 0.4f;
		lifes = 320;
		max_frames = 19;
		frame_delay = 3;
		break;
	}

	max_frame_delay = frame_delay;
}

int dll::METEORS::get_frame()
{
	--frame_delay;
	if (frame_delay <= 0)
	{
		frame_delay = max_frame_delay;
		++frame;
		if (frame > max_frames)frame = 0;
	}

	return frame;
}

void dll::METEORS::set_path(float targ_x, float targ_y)
{
	hor_dir = false;
	ver_dir = false;

	move_sx = start.x;
	move_ex = targ_x;

	move_sy = start.y;
	move_ey = targ_y;

	if ((move_sx == move_ex) || (move_ex > start.x && move_ex <= end.x))
	{
		ver_dir = true;
		return;
	}
	if ((move_sy == move_ey) || (move_ey > start.y && move_ey <= end.y))
	{
		hor_dir = true;
		return;
	}

	slope = (move_ey - move_sy) / (move_ex - move_sx);
	intercept = start.y - slope * start.x;
}
bool dll::METEORS::move(float gear)
{
	float my_speed = speed + gear / 10.0f;

	if (ver_dir)
	{
		if (move_ey < move_sy)
		{
			if (start.y - my_speed >= sky)
			{
				start.y -= my_speed;
				set_edges();
			}
			else return false;
		}
		else if (move_ey > move_sy)
		{
			if (end.y + my_speed <= ground)
			{
				start.y += my_speed;
				set_edges();
			}
			else return false;
		}
		else return false;
	}
	else if (hor_dir)
	{
		if (move_ex < move_sx)
		{
			if (start.x - my_speed >= 0)
			{
				start.x -= my_speed;
				set_edges();
			}
			else return false;
		}
		else if (move_ex > move_sx)
		{
			if (end.x + my_speed <= scr_width)
			{
				start.x += my_speed;
				set_edges();
			}
			else return false;
		}
		else return false;
	}
	else
	{
		if (move_ex < move_sx)
		{
			if (start.x - my_speed >= 0)
			{
				start.x -= my_speed;
				start.y = start.x * slope + intercept;
				set_edges();

				if (start.x < 0 || start.y < sky || end.y > ground || end.x > scr_width)return false;
			}
			else return false;
		}
		else if (move_ex > move_sx)
		{
			if (end.x + my_speed <= scr_width)
			{
				start.x += my_speed;
				start.y = start.x * slope + intercept;
				set_edges();

				if (start.x < 0 || start.y < sky || end.y > ground || end.x > scr_width)return false;
			}
			else return false;
		}
		else return false;
	}

	return true;
}

void dll::METEORS::Release()
{
	delete this;
}

dll::METEORS* dll::METEORS::create(meteors what, float sx, float sy, float ex, float ey, float oppos, float adjanced)
{
	METEORS* ret = new METEORS(what, sx, sy);

	if (ret)
	{
		ret->set_path(ex, ey);

		ret->angle = std::atan2(oppos, adjanced) * 180.0f / 3.14f;

		if (ret->move_ey < ret->center.y)
		{
			if (ret->move_ex < ret->center.x)ret->angle = 360.0f - ret->angle;
		}
		else if (ret->move_ey > ret->center.y)
		{
			if (ret->move_ex > ret->center.x)ret->angle = 180.0f - ret->angle;
			else if (ret->move_ex < ret->center.x)ret->angle = 180.0f + ret->angle;
		}
	}
	
	return ret;
}

//////////////////////////////////////////////////

// FUNCTIONS **********************************

float dll::distance(D2D1_POINT_2F first, D2D1_POINT_2F second)
{
	float a = (float)(pow(abs(second.x - first.x), 2));
	float b = (float)(pow(abs(second.y - first.y), 2));

	return (float)(sqrt(a + b));
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

void dll::sort(BAG<D2D1_POINT_2F>& bag, D2D1_POINT_2F target)
{
	if (bag.size() < 2)return;
	else
	{
		bool ok = false;

		while (!ok)
		{
			ok = true;

			for (size_t i = 0; i < bag.size() - 1; ++i)
			{
				if (distance(bag[i], target) > distance(bag[i + 1], target))
				{
					ok = false;
					D2D1_POINT_2F temp = bag[i];
					bag[i] = bag[i + 1];
					bag[i + 1] = temp;
				}
			}
		}
	}
}

void dll::AIMove(CREATURES*& evil, BAG<D2D1_POINT_2F>& assets_centeres, BAG<D2D1_POINT_2F>& meteor_centeres,
	D2D1_POINT_2F hero_center)
{
	if (!assets_centeres.empty())sort(assets_centeres, evil->center);
	if (!meteor_centeres.empty())sort(meteor_centeres, evil->center);

	if (!meteor_centeres.empty())
	{
		if (distance(evil->center, meteor_centeres[0]) <= 250.0f)
		{
			float targ_x = 0;
			float targ_y = sky;

			if (meteor_centeres[0].x < evil->center.x)targ_x = scr_width;
			else if (meteor_centeres[0].x == evil->center.x)targ_x = evil->center.x;

			if (meteor_centeres[0].y < evil->center.y)targ_y = ground;
			else if (meteor_centeres[0].y == evil->center.y)targ_y = evil->center.y;

			
			evil->set_path(targ_x, targ_y);
			evil->action = actions::move;
			
			return;
		}
	}
	else if (evil->action == actions::attack)
	{
		if (distance(evil->center, hero_center) <= evil->get_view_range())
		{
			evil->action = actions::attack;
			return;
		}
	}
	else if (evil->action != actions::attack)
	{
		if (distance(evil->center, hero_center) <= evil->get_view_range())
		{
			evil->action = actions::attack;
			return;
		}
		else if (evil->action == actions::patrol)
		{
			if (distance(evil->center, hero_center) <= evil->get_view_range() * 1.5f)
			{
				evil->set_path(hero_center.x, hero_center.y);
				evil->action = actions::move;
				return;
			}
			else
			{
				if (evil->dir == dirs::left)
				{
					if (evil->center.x <= evil->get_move_target_x())
					{
						evil->set_path(scr_width - 200.0f, evil->center.y);
						return;
					}
				}
				else if (evil->dir == dirs::right)
				{
					if (evil->center.x >= evil->get_move_target_x())
					{
						evil->set_path(200.0f, evil->center.y);
						return;
					}
				}
				else if (evil->dir == dirs::up)
				{
					if (evil->center.y <= evil->get_move_target_y())
					{
						evil->set_path(evil->center.x, sky + 100.0f);
						return;
					}
				}
				else if (evil->dir == dirs::down)
				{
					if (evil->center.y >= evil->get_move_target_y())
					{
						evil->set_path(evil->center.x, ground - 100.0f);
						return;
					}
				}
			}
		}
		else if (evil->action == actions::move)
		{
			if((evil->dir == dirs::left&& evil->center.x <= evil->get_move_target_x()) 
				|| (evil->dir == dirs::right && evil->center.x >= evil->get_move_target_x())
				|| (evil->dir == dirs::up && evil->center.y <= evil->get_move_target_y())
				|| (evil->dir == dirs::down && evil->center.y >= evil->get_move_target_y()))
			{
				evil->action = actions::patrol;
				return;
			}
		}
	}
}