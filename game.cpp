#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

float positionX = 0;
float positionY = 0;
float size = 1;
float speed = 50;
unsigned int color = 100;


void render()
{
	clear_screen();
	positionX > 50 ? positionX = -50 : positionX;
	positionY > 50 ? positionY = -50 : positionY;
	positionX < -50 ? positionX = 50 : positionX;
	positionY < -50 ? positionY = 50 : positionY;
	draw_rectangle(positionX,positionY, size, 1.777778 * size, 1);


	// Koristi O^2 - promijeniti u Renderer.cpp 
	//draw_circle(positionX, positionY, 1.5, color);
}

void simulate_game(Input *input, float dt) 
{
	if (is_down(BUTTON_UP))
	{
		positionY += (speed * dt) * 1.777778f;
	}
	if (is_down(BUTTON_DOWN))
	{
		positionY -= (speed * dt) * 1.777778f;
	}
	if (is_down(BUTTON_LEFT))
	{
		positionX -= speed * dt;
	}
	if (is_down(BUTTON_RIGHT)) 
	{
		positionX += speed * dt;
	}

	render();
}

