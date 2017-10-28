// snake.cpp : Defines the entry point for the console application.
//
#include <windows.h>
#include <stdio.h>

#include "../core_include/api.h"
#include "../core_include/rect.h"
#include "../core_include/cmd_target.h"
#include "../core_include/wnd.h"
#include "../core_include/msg.h"
#include "../core_include/surface.h"
#include "../core_include/display.h"
#include "../gui_include/slide_group.h"
#include "../gui_include/gesture.h"

#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	600
#define BG_COLOR		GLT_RGB(0, 0, 0)

static c_slide_root	s_root;
static c_surface*	s_surface;
void* s_phy_fb;

static void get_window_size(short *top, short *left, short *bottom, short *right)
{
	*top = *left = 0;
	*right = SCREEN_WIDTH - 1;
	*bottom = SCREEN_HEIGHT - 1;
}

#define MAX_LENGTH			256
#define INI_SNAKE_LENGTH	10
#define BODY_BLOCK			20
#define INVALIDE_POS		0

enum DIRECTION
{
	RIGHT,
	LEFT,
	DOWN,
	UP,
	MAX_DIRECTION
};
static DIRECTION s_player_direction = RIGHT;

enum SNAKE_STATUS
{
	RUNNING,
	BITTEN,
	CRASHED
};
class c_snake
{
public:
	c_snake();
	~c_snake() { ms_id--; }
	void run();
	bool is_be_bitten(short x, short y);
	void set_status(enum SNAKE_STATUS status) { m_status = status; }
	enum SNAKE_STATUS get_status() { return m_status; }
	int test_crash();
	int set_length(unsigned int len);
	unsigned int get_length() { return m_length; }
	bool got_food(unsigned int food_x, unsigned food_y);
	void get_head_pos(short *x, short *y) { *x = m_x[0]; *y = m_y[0]; }
	static unsigned int ms_id;
protected:
	void draw();
	virtual void move();
	void set_direct(enum DIRECTION direct);
	void update_direction();

	enum SNAKE_STATUS m_status;
	enum DIRECTION m_direction;
	unsigned int m_length;
	short m_x[MAX_LENGTH];
	short m_y[MAX_LENGTH];
	short m_x_footprint;
	short m_y_footprint;
	unsigned int m_id;
};

class c_super_snake : public c_snake
{
public:
	c_super_snake() {};
	~c_super_snake() {};
protected:
	virtual void move();
};

unsigned int c_snake::ms_id;

c_snake::c_snake()
{
	m_id = ms_id++;
	m_status = RUNNING;
	set_length(INI_SNAKE_LENGTH);
	
	short top, left, bottom, right;
	get_window_size(&top, &left, &bottom, &right);
	
	m_direction = RIGHT;
	memset(m_x, 0, sizeof(m_x));
	memset(m_y, 0, sizeof(m_y));
	m_x[0] = left + 1;
	m_y[0] = (bottom / 2);
	m_x_footprint = m_y_footprint = INVALIDE_POS;
}

int c_snake::set_length(unsigned int len)
{
	if (len >= MAX_LENGTH)
	{
		return -1;
	}

	for (unsigned int i = (m_length - 1); i < len; i++)
	{
		m_x[i] = m_x[m_length - 1];
		m_y[i] = m_y[m_length - 1];
	}
	m_length = len;
	return 0;
}

void c_snake::set_direct(enum DIRECTION direct)
{//no backwrad
	switch (m_direction)
	{
	case LEFT:
		if (direct != RIGHT) { m_direction = direct; } break;
	case RIGHT:
		if (direct != LEFT) { m_direction = direct; } break;
	case UP:
		if (direct != DOWN) { m_direction = direct; } break;
	case DOWN:
		if (direct != UP) { m_direction = direct; } break;
	default:
		break;
	}
}

static void draw_body(int x, int y, unsigned int rgb)
{
	s_surface->fill_rect(x, y, (x + BODY_BLOCK), (y + BODY_BLOCK), rgb, Z_ORDER_LEVEL_0);
}

void c_snake::draw()
{	//clear foot print
	if ((INVALIDE_POS != m_x_footprint) && (INVALIDE_POS != m_y_footprint))
	{
		draw_body(m_x_footprint, m_y_footprint, BG_COLOR);
	}

	//draw body
	if (m_length > 1)
	{
		draw_body(m_x[1], m_y[1], GLT_RGB(255, 0, 0));
	}

	//draw head
	if ((INVALIDE_POS == m_x[0]) || (INVALIDE_POS == m_y[0]))
	{
		return;
	}
	
	switch (m_direction)
	{
	case RIGHT:
		printf("<");
		draw_body(m_x[0], m_y[0], GLT_RGB(0, 255, 0));
		break;
	case LEFT:
		printf(">");
		draw_body(m_x[0], m_y[0], GLT_RGB(0, 255, 0));
		break;
	case UP:
		printf("v");
		draw_body(m_x[0], m_y[0], GLT_RGB(0, 255, 0));
		break;
	case DOWN:
		printf("^");
		draw_body(m_x[0], m_y[0], GLT_RGB(0, 255, 0));
		break;
	default:
		break;
	}
}

void c_snake::move()
{
	m_x_footprint = m_x[(m_length - 1)];
	m_y_footprint = m_y[(m_length - 1)];
	for (size_t i = (m_length - 1); i > 0; i--)
	{
		m_x[i] = m_x[i - 1];
		m_y[i] = m_y[i - 1];
	}

	if (m_direction == RIGHT)
	{
		m_x[0] += BODY_BLOCK;
	}
	else if (m_direction == DOWN)
	{
		m_y[0] += BODY_BLOCK;
	}
	else if (m_direction == UP)
	{
		m_y[0] -= BODY_BLOCK;
	}
	else if (m_direction == LEFT)
	{
		m_x[0] -= BODY_BLOCK;
	}
}

void c_snake::update_direction()
{
	set_direct(s_player_direction);
}

bool c_snake::is_be_bitten(short x, short y)
{
	for (size_t i = 1; i < m_length; i++)
	{
		if ((x == m_x[i]) && (y == m_y[i]))
		{
			return true;
		}
	}
	return false;
}

int c_snake::test_crash()
{
	//test crash wall
	short top, left, bottom, right;
	get_window_size(&top, &left, &bottom, &right);
	if ((m_x[0] >= right) ||
		(m_x[0] <= left) ||
		(m_y[0] <= top) ||
		(m_y[0] >= bottom))
	{
		m_status = CRASHED;
		return 1;
	}

	//test be bitten
	if (is_be_bitten(m_x[0], m_y[0]))
	{
		m_status = BITTEN;
		return 2;
	}
	return 0;
}

bool c_snake::got_food(unsigned int food_x, unsigned food_y)
{
	if ((m_x[0] == food_x) && (m_y[0] == food_y))
	{
		return true;
	}
	return false;
}

void c_snake::run()
{
	update_direction();
	move();
	draw();
	test_crash();
}

void c_super_snake::move()
{
	c_snake::move();
	short top, left, bottom, right;
	get_window_size(&top, &left, &bottom, &right);
	if (m_x[0] >= right)
	{
		m_x[0] = left + 1;
	}
	if (m_x[0] <= left)
	{
		m_x[0] = right - 1;
	}
	if (m_y[0] <= top)
	{
		m_y[0] = bottom - 1;
	}
	if (m_y[0] >= bottom)
	{
		m_y[0] = top + 1;
	}
}

class c_food
{
public:
	c_food();
	~c_food() {};
	void draw();
	void new_food();
	unsigned int m_x;
	unsigned int m_y;
};

c_food::c_food()
{
	new_food();
}

void c_food::new_food()
{
	short top, left, bottom, right;
	get_window_size(&top, &left, &bottom, &right);
	m_x = (rand() % (right - left - 1)) + left + 1;
	if (0 == m_x)
	{
		m_x = 1;
	}
	m_y = (rand() % (bottom - top - 1)) + top + 1;
	if (0 == m_y)
	{
		m_y = 1;
	}
}

void c_food::draw()
{
	draw_body(m_x, m_y, GLT_RGB(255, 255, 0));
}

class c_wall
{
public:
	c_wall() {};
	~c_wall() {};
	void draw();
private:

};

void c_wall::draw()
{
	short top, left, bottom, right;
	get_window_size(&top, &left, &bottom, &right);
	s_surface->draw_rect(top + 1, left + 1, right - 1, bottom - 1, GLT_RGB(255, 0, 0), Z_ORDER_LEVEL_0);
	s_surface->draw_rect(top + BODY_BLOCK, left + BODY_BLOCK, right - BODY_BLOCK, bottom - BODY_BLOCK, GLT_RGB(255, 0, 0), Z_ORDER_LEVEL_0);
}

enum GAME_STATUS
{
	GAME_RUNNING,
	GAME_PAUSE
};
enum GAME_STATUS update_game_status(unsigned feed_total)
{	
	static enum GAME_STATUS s_status = GAME_RUNNING;	
	//if (' ' == *key)
	{
		s_status = (GAME_PAUSE == s_status) ? GAME_RUNNING : GAME_PAUSE;
		return s_status;
	}

	static unsigned s_feed_total = 0;
	if (s_feed_total != feed_total)
	{
		s_feed_total = feed_total;
		short top, left, bottom, right;
		get_window_size(&top, &left, &bottom, &right);
		//set_cursor_pos(left, top);
		printf("Your score: %d ", 10 * feed_total * c_snake::ms_id);

		//set_cursor_pos(left, bottom);
		printf("Your score: %d ", 10 * feed_total * c_snake::ms_id);
	}
	return s_status;
}

void single_game()
{
	c_super_snake the_snake;
	c_food the_food;
	c_wall the_wall;
	unsigned int feed_total = 0;

	the_wall.draw();
	while (1)
	{
		Sleep(100);
		if (GAME_PAUSE == update_game_status(feed_total))
		{
			continue;
		}
		the_snake.run();
		the_food.draw();
		if (the_snake.got_food(the_food.m_x, the_food.m_y))
		{
			the_food.new_food();
			the_snake.set_length((the_snake.get_length() + 1));
			feed_total++;
		}
		
		if (RUNNING != the_snake.get_status())
		{
			return;
		}
	}
}

void create_ui()
{
	void* s_phy_fb = (void*)malloc(SCREEN_WIDTH * SCREEN_HEIGHT * 2);
	c_display* display = new c_display(s_phy_fb, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	s_surface = display->create_surface(&s_root, Z_ORDER_LEVEL_0);
	s_surface->set_active(true);
	s_surface->fill_rect(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, BG_COLOR, Z_ORDER_LEVEL_0);
}

extern "C" int run_native(int main_cnt, int sub_cnt)
{
	create_ui();
	single_game();
	return 0;
}

extern "C" void* get_frame_buffer(int display_id, int* width, int* height)
{
	return c_display::get_frame_buffer(0, width, height);
}

extern "C" int send_hid_msg(void* buf, int len, int display_id)
{
	static MSG_INFO s_press_msg;

	if (len != sizeof(MSG_INFO))
	{
		ASSERT(FALSE);
	}

	MSG_INFO* msg = (MSG_INFO*)buf;
	if (msg->dwMsgId == 0x4700 && (s_press_msg.dwMsgId == 0))
	{//presss
		memcpy(&s_press_msg, msg, sizeof(MSG_INFO));
	}
	else if (msg->dwMsgId == 0x4600)
	{//release
		int dx = msg->dwParam1 - s_press_msg.dwParam1;
		int dy = msg->dwParam2 - s_press_msg.dwParam2;
		if (abs(dx) > abs(dy))
		{
			s_player_direction = (dx > 0) ? RIGHT : LEFT;
		}
		else
		{
			s_player_direction = (dy > 0) ? DOWN : UP;
		}
		s_press_msg.dwMsgId = 0;
	}
	return 0;
}