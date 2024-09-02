#include <raylib.h>
#include <stdio.h>

#define W_WIDTH 800 //window width
#define W_HEIGHT 800 //window height
#define W_TITLE "Flappy Bird" //window title
#define W_BACKGROUND (Color){0x19, 0x19, 0x19, 0xFF} //background color
#define GRAVITY 3000 //bird gravity
#define B_COLOR (Color){0xFF, 0xFF, 0x0, 0xFF} //bird color
#define B_MAX_SPEED 10000
#define B_MIN_SPEED -1000
#define B_JUMP_FORCE -2000
#define W_FPS 60
#define P_BASE_SPEED 500 //pipe base speed
#define P_COLOR (Color){0x0, 0xFF, 0x0, 0xFF} //pipe color
#define P_SPEED_MOD 1.01 //pipe speed modifier
#define T_COLOR (Color){0xFF, 0xFF, 0xFF, 0xFF} //score text color
#define KEY_SPACE 32 //keycode for spacebar

typedef struct Bird {
	float x;
	float y;
	float width;
	float height;
	float speed;
	float gravity;
	Color color;
} bird;

typedef struct pipe {
	float x;
	float yTop;
	float yBottom;
	float width;
	float gapY;
	float gapSize;
	float speed;
	Color color;
} pipe;

bird b;
pipe p;
int score = 0;

// forward declare functions
bird createBird(float size);
pipe createPipe(float width, float gapSize);
void drawBird(bird b);
void drawPipe(pipe p);
int moveBird(bird *b);
void jump(bird *b);
int movePipe(pipe *p);
void resetPipe(pipe *p);
int checkCollision(bird b, pipe p);
int gameFrame();


int main(int argc, char *argv[]) {
	InitWindow(W_WIDTH, W_HEIGHT, W_TITLE);
	SetTargetFPS(W_FPS);
	b = createBird(50);
	p = createPipe(50, 300);

	while (!WindowShouldClose()) {
		if (gameFrame()) break;
	}

	CloseWindow();
	return 0;
}

bird createBird(float size) {
	float windowHeight = GetScreenHeight();
	float verticalCenter = windowHeight / 2;
	float x = size;
	float width = size;
	float height = size;
	float y = verticalCenter - height / 2;

	bird b = {
		x, y, width, height, 0, GRAVITY, B_COLOR
	};
	return b;
}

pipe createPipe(float width, float gapSize) {
	float windowHeight = GetScreenHeight();
	float verticalCenter = windowHeight / 2;
	float windowWidth = GetScreenWidth();
	float x = windowWidth;
	float yTop = 0;
	float gapY = verticalCenter - gapSize / 2;
	float yBottom = gapY + gapSize;
	float speed = P_BASE_SPEED;
	pipe p = {
		x, yTop, yBottom, width, gapY, gapSize, speed, P_COLOR
	};
	return p;
}

void drawBird(bird b) {
	DrawRectangle(b.x, b.y, b.width, b.height, b.color);
}

void drawPipe(pipe p) {
	float windowHeight = GetScreenHeight();
	DrawRectangle(p.x, p.yTop, p.width, p.gapY, p.color);
	DrawRectangle(p.x, p.yBottom, p.width, windowHeight - p.yBottom, p.color);
}

int moveBird(bird *b) {
	float dt = GetFrameTime();
	
	if (b->speed < B_MAX_SPEED) b->speed += b->gravity * dt;

	float ny = b->y + b->speed * dt;
	
	if (ny + b->height >= W_HEIGHT) return 1;
	if (ny <= 0) return 1;

	b->y = ny;
	
	return 0;
}

int movePipe(pipe *p) {
	float dt = GetFrameTime();
	float nx = p->x - p->speed * dt;

	if (nx + p->width <= 0) {
		resetPipe(p);
		return 1;
	}
	
	p->x = nx;
	return 0;
}

void resetPipe(pipe *p) {
	float windowWidth = GetScreenWidth();
	float windowHeight = GetScreenHeight();
	float min = windowHeight * 0.1;
	float max = windowHeight * 0.9 - p->gapSize;
	p->x = windowWidth;
	p->gapY = GetRandomValue(min, max);
	p->yBottom = p->gapY + p->gapSize;
	p->speed *= P_SPEED_MOD;
}

void jump(bird *b) {
	float newSpeed = b->speed + B_JUMP_FORCE;
	if (newSpeed >= B_MIN_SPEED) {
		b->speed = newSpeed;
	} else {
		b->speed = B_MIN_SPEED;
	}
}

int checkCollision(bird b, pipe p) {
	if (b.x + b.width >= p.x && b.x <= p.x + p.width) {
		if (b.y <= p.gapY || b.y + b.height >= p.yBottom) {
			return 1;
		}
	}
	return 0;
}

int gameFrame() {
	int gameOver = 0;
	char scoreText[16]; //buffer for sprintf
	sprintf(scoreText, "Score: %d", score); //convert score to string
	BeginDrawing();
	ClearBackground(W_BACKGROUND);
	
	if (IsKeyReleased(KEY_SPACE)) jump(&b);
		
	if (moveBird(&b)) gameOver = 1;
	if (movePipe(&p)) score++;
	if (checkCollision(b, p)) gameOver = 1;
	
	drawPipe(p);
	drawBird(b);
	DrawText(scoreText, 5, 5, 20, T_COLOR);
	EndDrawing();
	return gameOver;
}
