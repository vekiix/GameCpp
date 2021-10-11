#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)
#define margina 47.5f

float positionX = 0, positionY = -48.75f, size = 10, speed = 20, acceleration = 0;
float positionBallX = rand() % 50, positionBallY = rand() % 50;
int score;
bool up = true, left = true;

void render()
{
	clear_screen();
	draw_rectangle(0, -3.5, 47.5, 49, 0xffccff);

	
	draw_rectangle(positionBallX, positionBallY, 1, 1.77778f, 1);
	draw_rectangle(positionX, positionY, size, .5f, 0x666666);
	//draw_circle(0, 0, 10, 1);
}

void simulate_game(Input *input, float dt) 
{
	positionBallX > margina - 1 ? left = true : left;
	positionBallX < -margina + 1 ? left = false : left;
	positionBallY > margina - 3 ? up = false : up;
	if ((positionBallY <= -margina + 1 && positionBallY >= -margina) && (positionBallX <= positionX + size && positionBallX >= positionX - size))
	{
		if (positionBallY > -margina) 
		{
			score++;
			up = true;
			left = !left;
			speed += 5;
			size < 5 ? size : size -= 0.5;
		}
	}
	if(positionBallY < -margina -5)
	{
		throw score;
	}


	if (up && left)
	{
		positionBallY += speed / (16 / 9) * dt;
		positionBallX -= speed / (16 / 9) * dt;
	}
	else if (up && !left)
	{
		positionBallY += speed / (16 / 9) * dt;
		positionBallX += speed / (16 / 9) * dt;
	}
	else if (!up && left)
	{
		positionBallY -= speed / (16 / 9) * dt;
		positionBallX -= speed / (16 / 9) * dt;
	}
	else if (!up && !left)
	{
		positionBallY -= speed / (16 / 9) * dt;
		positionBallX += speed / (16 / 9) * dt;
	}

	if (is_down(BUTTON_LEFT))
	{
		if (positionX - size > -margina)positionX -= speed / (16 / 9) * dt;
	}
	if (is_down(BUTTON_RIGHT)) 
	{
		if(positionX + size < margina)positionX += speed / (16 / 9) * dt;
	}
	render();
}

