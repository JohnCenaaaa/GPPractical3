
#include <Windows.h>
#include <gl/GL.h>
#include <math.h>
#include <time.h>
#pragma comment (lib, "OpenGL32.lib")

#define WINDOW_TITLE "OpenGL Window"
#define PIE 3.1415926

float color = 0.1f;
float color2 = 0.0f;
float changeX = 0.1f;
float changeY = 0.1f;
float changeZ = 0.1f;

float changeX2 = 0.1f;
float changeY2 = 0.1f;
float changeZ2 = 0.1f;

float changeX3 = 0.1f;
float changeY3 = 0.1f;
float changeZ3 = 0.1f;

int questionToShow;
const float numOfTriangle = 100;
const float angle = PIE * 2.f / numOfTriangle;
float rotationSpeed = 0.0f;         
float maxSpeed = 5.0f;              
float minSpeed = -5.0f;            
float speedIncrement = 0.18f;       
float bladeRotationAngle = 0.0f;    
bool rotateClockwise = false;       
bool rotateCounterClockwise = false;
float windmillY = 0.0f;          
float maxRise = 2.0f;            
float riseSpeed = 0.005f;         
float baseY = 0.0f;         
float ufoX = -1.5f; 
float ufoTargetX = 0.0f;
float ufoY = 0.7f;      
float ufoSpeed = 0.01f; 
bool startUfoMovement = false;
bool ufoStopped = false;
bool ufoRising = false;
float ufoTargetY = 2.0f;
bool ufoRaised = false;
bool lightBeamActive = false;
bool windmillFullyRaised = false;

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam) {
		case VK_ESCAPE:
				PostQuitMessage(0);
				break;
		case VK_UP:
			changeY += 0.1f;
			changeY2 -= 0.1f;
				break;
			case VK_DOWN:
				changeY -= 0.1f;
				changeY2 += 0.1f;
				break;
			case VK_LEFT:
				changeX -= 0.1f;
				changeX2 += 0.1f;
				break;
			case VK_RIGHT:
				changeX += 0.1f;
				changeX2 -= 0.1f;
				break;
			case 'W':
				changeY2 += 0.1f;
				break;
			case 'A':
				changeX2 -= 0.1f;
				break;
			case 'S':
				changeY2 -= 0.1f;
				break;
			case 'D':
				changeX2 += 0.1f;
				break;
			case 'I':
				changeY3 += 0.1f;
				break;
			case 'J':
				changeX3 -= 0.1f;
				break;
			case 'K':
				changeY3 -= 0.1f;
				break;
			case 'L':
				changeX3 += 0.1f;
				break;
			case 'Q':  
				if (rotationSpeed > minSpeed) {  
					rotationSpeed -= speedIncrement;
				}
				break;

			case 'E':  
				if (rotationSpeed < maxSpeed) { 
					rotationSpeed += speedIncrement;
				}
				break;

			case 'R':  
				rotationSpeed = 0.0f;
				break;
			case 'Z':
				startUfoMovement = true;
				break;
			case 'X': 
				if (ufoStopped && !ufoRising && !ufoRaised) {
					ufoRising = true;
				}
				break;
			case 'C':
				if (lightBeamActive) {
					lightBeamActive = false;
					windmillFullyRaised = false;
				}
				break;
			case VK_SPACE:
				changeX = 0.0f;
				changeY = 0.0f;
				changeZ = 0.0f;
				changeX2 = 0.0f;
				changeY2 = 0.0f;
				changeZ2 = 0.0f;
				changeX3 = 0.0f;
				changeY3 = 0.0f;
				changeZ3 = 0.0f;
				startUfoMovement = false;
				ufoStopped = false;
				ufoRising = false;
				ufoRaised = false;
				lightBeamActive = false;
				windmillFullyRaised = false;
				windmillY = baseY;  
				ufoX = -1.5f;
				ufoY = 0.7f;
				break;
			case 0x31:
				questionToShow = 1;
				break;
			case 0x32:
				questionToShow = 2;
				break;
			case 0x33:
				questionToShow = 3;
				break;
			case 0x34:
				questionToShow = 4;
				break;
			case 0x35:
				questionToShow = 5;
				break;
			case 0x36:
				questionToShow = 6;
				break;
			case 0x37:
				questionToShow = 7;
				break;
			case 0x38:
				questionToShow = 8;
				break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------

bool initPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.cAlphaBits = 8;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 0;

	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	// choose pixel format returns the number most similar pixel format available
	int n = ChoosePixelFormat(hdc, &pfd);

	// set pixel format returns whether it sucessfully set the pixel format
	if (SetPixelFormat(hdc, n, &pfd))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------------------------------------
void updateUFOPosition() {
	if (startUfoMovement && ufoX < ufoTargetX) {
		ufoX += ufoSpeed;
		if (ufoX >= ufoTargetX) {
			ufoX = ufoTargetX;
			ufoStopped = true;
		}
	}

	// New rise mechanism
	if (ufoRising) {
		ufoY += ufoSpeed; // Use the same speed for vertical movement
		if (ufoY >= ufoTargetY) {
			ufoY = ufoTargetY;
			ufoRising = false;
			ufoRaised = true;
			lightBeamActive = true; // Activate light beam when UFO reaches target height
		}
	}
}

void updateRotation() {
	bladeRotationAngle += rotationSpeed;
	// Keep the angle within 0-360 degrees for simplicity
	if (bladeRotationAngle >= 360.0f) {
		bladeRotationAngle -= 360.0f;
	}
	else if (bladeRotationAngle < 0.0f) {
		bladeRotationAngle += 360.0f;
	}
}




void drawFanBlade(float x, float y, float radius, float rotationAngle) {
	glPushMatrix();
	glTranslatef(x, y, 0.0f); // Translate to the specified position
	glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f); // Rotate the blade

	// Draw the fan blade as a white quad
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1); // White color for the blade
	glVertex2f(-0.05f, 0.0f);
	glVertex2f(0.05f, 0.0f);
	glVertex2f(0.05f, radius);
	glVertex2f(-0.05f, radius);
	glEnd();

	// Draw the black outline using GL_LINE_LOOP
	glBegin(GL_LINE_LOOP);
	glColor3f(0, 0, 0); // Black color for the outline
	glVertex2f(-0.05f, 0.0f);
	glVertex2f(0.05f, 0.0f);
	glVertex2f(0.05f, radius);
	glVertex2f(-0.05f, radius);
	glEnd();

	// Draw the horizontal stripes (10 evenly spaced stripes)
	float stripeHeight = 0.005f; // Thickness of the stripe
	int numStripes = 20; // Number of stripes to draw

	for (int i = 0; i < numStripes; i++) {
		float stripeYPosition = (radius / numStripes) * (i + 1); // Spacing between the stripes

		glBegin(GL_QUADS);
		glColor3f(0.0f, 0.0f, 0.0f); // Black color for the horizontal stripe
		glVertex2f(-0.05f, stripeYPosition - stripeHeight / 2); // Bottom-left of the stripe
		glVertex2f(0.05f, stripeYPosition - stripeHeight / 2);  // Bottom-right of the stripe
		glVertex2f(0.05f, stripeYPosition + stripeHeight / 2); // Top-right of the stripe
		glVertex2f(-0.05f, stripeYPosition + stripeHeight / 2); // Top-left of the stripe
		glEnd();
	}

	// Draw the vertical stripe at the center
	float verticalStripeWidth = 0.005f; // Thickness of the stripe
	float verticalStripeXPosition = 0.0f; // Center of the blade

	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.0f, 0.0f); // Black color for the vertical stripe
	glVertex2f(verticalStripeXPosition - verticalStripeWidth / 2, 0.0f); // Bottom-left of the stripe
	glVertex2f(verticalStripeXPosition + verticalStripeWidth / 2, 0.0f); // Bottom-right of the stripe
	glVertex2f(verticalStripeXPosition + verticalStripeWidth / 2, radius); // Top-right of the stripe
	glVertex2f(verticalStripeXPosition - verticalStripeWidth / 2, radius); // Top-left of the stripe
	glEnd();

	glPopMatrix();
}




void drawFan(float x, float y, float radius, float numOfTriangle, float angle, float rotationAngle) {
	glPushMatrix();
	glTranslatef(x, y, 0.0f);
	glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f);

	// Draw the blades
	drawFanBlade(0.0f, 0.0f, radius, rotationAngle);
	drawFanBlade(0.0f, 0.0f, radius, rotationAngle + 90.0f);
	drawFanBlade(0.0f, 0.0f, radius, rotationAngle + 180.0f);
	drawFanBlade(0.0f, 0.0f, radius, rotationAngle + 270.0f);

	glPopMatrix();
}

void drawIrregularCrater(float x, float y, float baseRadius, float irregularity) {
	// Use a fixed random seed based on crater position for consistent shape
	unsigned int seed = (unsigned int)(x * 1000 + y * 1000);

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.5f, 0.5f, 0.5f); // Dark gray for crater interior

	// Center of the crater
	glVertex2f(x, y);

	// Create an irregular crater outline
	for (int i = 0; i <= numOfTriangle; i++) {
		float angle = i * 2.0f * PIE / numOfTriangle;

		// Deterministic random factor
		seed = seed * 1103515245 + 12345;
		float randomFactor = 1.0f + ((seed % 100 / 100.0f) - 0.5f) * irregularity;

		// Calculate irregular radius
		float currentRadius = baseRadius * randomFactor;

		// Calculate vertex position
		float dx = cos(angle) * currentRadius;
		float dy = sin(angle) * currentRadius;

		glVertex2f(x + dx, y + dy);
	}
	glEnd();

	// Add a slightly lighter rim to simulate depth
	glBegin(GL_LINE_LOOP);
	glColor3f(0.5f, 0.5f, 0.5f); // Slightly lighter color for rim
	seed = (unsigned int)(x * 1000 + y * 1000); // Reset seed
	for (int i = 0; i <= numOfTriangle; i++) {
		float angle = i * 2.0f * PIE / numOfTriangle;

		// Use same seed for consistent rim
		seed = seed * 1103515245 + 12345;
		float randomFactor = 1.0f + ((seed % 100 / 100.0f) - 0.5f) * irregularity;

		// Slightly larger radius for the rim
		float currentRadius = baseRadius * (1.0f + 0.1f) * randomFactor;

		float dx = cos(angle) * currentRadius;
		float dy = sin(angle) * currentRadius;

		glVertex2f(x + dx, y + dy);
	}
	glEnd();
}

void drawMoon(float x, float y, float moonRadius) {
	// Static arrays to store crater properties
	static bool cratersInitialized = false;
	static const int MAX_CRATERS = 100;
	static float craterX[MAX_CRATERS];
	static float craterY[MAX_CRATERS];
	static float craterRadius[MAX_CRATERS];
	static float craterIrregularity[MAX_CRATERS];
	static int numCraters;

	// Initialize craters only once
	if (!cratersInitialized) {
		// Use a fixed seed for consistent randomness
		srand(42);

		// Determine number of craters
		numCraters = 40 + rand() % 5; 

		// Generate crater properties
		for (int crater = 0; crater < numCraters; crater++) {
			// Random position within moon's surface
			float craterAngle = (rand() % 360) * PIE / 180.0f;
			float craterDistance = (rand() % 100 / 100.0f) * moonRadius * 0.8f;

			// Calculate crater position
			craterX[crater] = cos(craterAngle) * craterDistance;
			craterY[crater] = sin(craterAngle) * craterDistance - moonRadius;

			// Random crater size
			craterRadius[crater] = moonRadius * (0.05f + (rand() % 100 / 100.0f) * 0.1f);

			// Random irregularity
			craterIrregularity[crater] = 0.2f + (rand() % 100 / 100.0f) * 0.3f;
		}

		cratersInitialized = true;
	}

	glPushMatrix();
	glTranslatef(x, y, 0.0f); // Translate to the specified position

	// Draw moon base
	float moonCenterY = -moonRadius;
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.9f, 0.9f, 0.9f); // Light gray color (bright side)
	glVertex2f(0.0f, moonCenterY); // Center of the moon
	for (int i = 0; i <= numOfTriangle; i++) {
		float angle = i * 2.0f * PIE / numOfTriangle;
		float dx = cos(angle) * moonRadius;
		float dy = sin(angle) * moonRadius;

		// Add shadow effect by varying color based on distance from the center
		float dist = sqrt(dx * dx + dy * dy);
		float shadeFactor = dist / moonRadius;

		float red = 0.9f - 0.3f * shadeFactor;
		float green = 0.9f - 0.3f * shadeFactor;
		float blue = 0.9f - 0.3f * shadeFactor;

		glColor3f(red, green, blue);
		glVertex2f(dx, dy + moonCenterY);
	}
	glEnd();

	// Draw the pre-generated craters
	for (int crater = 0; crater < numCraters; crater++) {
		drawIrregularCrater(craterX[crater], craterY[crater],
			craterRadius[crater],
			craterIrregularity[crater]);
	}

	glPopMatrix();
}

void drawStars(float sceneWidth, float sceneHeight, int numStars) {
	// Use a static seed for consistent star placement
	static int seed = 42;
	srand(seed);

	// Draw small dots representing stars
	for (int i = 0; i < numStars; i++) {
		// Random x position within scene width
		float x = (rand() % 1000 / 1000.0f) * sceneWidth - (sceneWidth / 2.0f);

		// Random y position above y = 0 (bottom of the scene)
		float y = (rand() % 1000 / 1000.0f) * sceneHeight;

		// Varying star sizes
		float starSize = 1.0f + (rand() % 3); // Small dot sizes

		// Varying star brightness/color
		float brightness = 0.6f + (rand() % 100 / 500.0f);
		glPointSize(starSize);

		glBegin(GL_POINTS);
		// Slightly varied white/gray colors to simulate star twinkle
		glColor3f(brightness, brightness, brightness);
		glVertex2f(x, y);
		glEnd();
	}
}

// For multiple star fields with different characteristics
void drawStarField(float sceneWidth, float sceneHeight, int numStars, int seed) {
	srand(seed);

	for (int i = 0; i < numStars; i++) {
		float x = (rand() % 1000 / 1000.0f) * sceneWidth - (sceneWidth / 2.0f);
		float y = (rand() % 1000 / 1000.0f) * sceneHeight;

		float starSize = 1.0f + (rand() % 3);
		float brightness = 0.6f + (rand() % 100 / 500.0f);

		glPointSize(starSize);
		glBegin(GL_POINTS);
		glColor3f(brightness, brightness, brightness);
		glVertex2f(x, y);
		glEnd();
	}
}

void updateWindmillPosition() {
	if (rotationSpeed >= maxSpeed) {
		// Rising behavior
		if (windmillY < maxRise) {
			windmillY += riseSpeed; // Move upwards
		}
	}
	else if (lightBeamActive) {
		if (windmillY < maxRise) {
			windmillY += riseSpeed; // Move upwards

			// Check if windmill has reached maximum height
			if (windmillY >= maxRise) {
				windmillY = maxRise;
				windmillFullyRaised = true;
			}
		}
	}
	else if (!lightBeamActive && windmillFullyRaised) {
		// Dropping behavior when light beam is deactivated
		if (windmillY > baseY) {
			windmillY -= riseSpeed; // Move downwards

			// When windmill reaches base, stop the drop and deactivate light beam
			if (windmillY <= baseY) {
				windmillY = baseY;
				windmillFullyRaised = false;
				lightBeamActive = false; // Deactivate the light beam once windmill is back at base
			}
		}
	}
	else if (rotationSpeed <= minSpeed) {
		// Rising behavior
		if (windmillY < maxRise) {
			windmillY += riseSpeed; // Move upwards
		}
	}
	else {
		// Falling behavior
		if (windmillY > baseY) {
			windmillY -= riseSpeed; // Move downwards
		}
	}
}


void drawLightBeam(float ufoX, float ufoY) {
	glPushMatrix();
	glTranslatef(ufoX, ufoY, 0.0f);
	// Draw the light beam using a triangle fan
	glBegin(GL_TRIANGLE_FAN);

	// Start with a bluish-white color at the source with moderate transparency
	glColor4f(0.7f, 0.8f, 1.0f, 0.4f); // Soft blue-white, more moderate transparency
	glVertex2f(0.0f, -0.1f);           // Originating from the UFO's bottom center

	// Gradually fade to an even lighter, more transparent version
	glColor4f(0.9f, 0.95f, 1.0f, 0.1f); // Very light blue-white, very transparent
	glVertex2f(-0.5f, -1.5f);          // Left edge of the light beam
	glVertex2f(0.5f, -1.5f);           // Right edge of the light beam
	glEnd();
	glPopMatrix();
}

void drawUFO(float x, float y, float scale) {
	glPushMatrix();
	glTranslatef(x, y, 0.0f);
	glScalef(scale, scale, 1.0f);

	// UFO body (main disk)
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.7f, 0.7f, 0.7f); // Light gray body
	glVertex2f(0.0f, 0.0f); // Center point
	for (int i = 0; i <= 20; i++) {
		float angle = i * 2.0f * PIE / 20;
		// Slightly flatten the bottom of the disk
		float yFactor = (i > 10) ? 0.5f : 1.0f;
		float dx = cos(angle) * 0.5f;
		float dy = sin(angle) * 0.3f * yFactor;
		glVertex2f(dx, dy);
	}
	glEnd();

	// Metallic bottom rim
	glBegin(GL_LINE_STRIP);
	glColor3f(0.4f, 0.4f, 0.4f); // Darker gray for rim
	for (int i = 10; i <= 20; i++) {
		float angle = i * 2.0f * PIE / 20;
		float dx = cos(angle) * 0.5f;
		float dy = sin(angle) * 0.3f * 0.5f;
		glVertex2f(dx, dy);
	}
	glEnd();

	// Cockpit / Dome
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.6f, 0.8f, 1.0f); // Light blue for cockpit
	glVertex2f(0.0f, 0.2f); // Cockpit center
	for (int i = 0; i <= 20; i++) {
		float angle = i * 2.0f * PIE / 20;
		float dx = cos(angle) * 0.3f;
		float dy = sin(angle) * 0.1f + 0.2f;
		glVertex2f(dx, dy);
	}
	glEnd();

	// Cockpit rim
	glBegin(GL_LINE_LOOP);
	glColor3f(0.3f, 0.3f, 0.3f); // Dark gray rim
	for (int i = 0; i <= 20; i++) {
		float angle = i * 2.0f * PIE / 20;
		float dx = cos(angle) * 0.3f;
		float dy = sin(angle) * 0.1f + 0.2f;
		glVertex2f(dx, dy);
	}
	glEnd();

	// Landing lights / Protrusions
	glPointSize(3.0f);
	glBegin(GL_POINTS);
	glColor3f(1.0f, 1.0f, 0.0f); // Yellow lights
	glVertex2f(-0.4f, -0.1f);
	glVertex2f(0.4f, -0.1f);
	glEnd();

	glPopMatrix();
}


// Usage with multiple star fields
void renderScene() {
	// Background 
	drawMoon(0, 0, 1.2f);
	drawStarField(4.0f, 3.0f, 100, 42);
	updateUFOPosition();
	if (ufoStopped && ufoRaised && lightBeamActive && !windmillFullyRaised) {
        drawLightBeam(0, 1.2f);
    }
	drawUFO(ufoX, ufoY, 0.5f);
	// Foreground stars (denser, smaller)
	drawStarField(4.0f, 3.0f, 200, 123);
}

void display()
{
	switch (questionToShow) {
	case 1:

	case 2:

	case 3:

	case 4:

	case 5:

	case 6:
		glClearColor(color, 0.1f, 0.1f, 0.1f);
		glClear(GL_COLOR_BUFFER_BIT);
		glLoadIdentity();

		glPushMatrix();
		glTranslatef(changeX3, changeY3, changeZ3);

		glPushMatrix();
		glTranslatef(changeX, changeY, changeZ);
		glBegin(GL_QUADS);
		glColor3f(0, 0, 1.0f);
		glVertex2f(0.0, 0.5);
		glVertex2f(0.5, 0.5);
		glVertex2f(0.5, -0.5);
		glVertex2f(0, -0.5);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(changeX2, changeY2, changeZ2);
		glBegin(GL_QUADS);
		glColor3f(1.0f, 0, 0);
		glVertex2f(-0.5, 0.5);
		glVertex2f(-0.5, -0.5);
		glVertex2f(0, -0.5);
		glVertex2f(0, 0.5);
		glEnd();
		glPopMatrix();

		glPopMatrix();
		break;

	case 7:
		glClearColor(color, 0.1f, 0.1f, 0.1f);
		glClear(GL_COLOR_BUFFER_BIT);
		glLoadIdentity();

		glPushMatrix();
		glTranslatef(changeX3, changeY3, changeZ3);

		glPushMatrix();
		glTranslatef(changeX, changeY, changeZ);
		glBegin(GL_QUADS);
		glColor3f(1.0f, 0, 0);
		glVertex2f(0.3, 0.3);
		glVertex2f(-0.3, 0.3);
		glVertex2f(-0.3, -0.3);
		glVertex2f(0.3, -0.3);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(changeX2, changeY2, changeZ2);
		glBegin(GL_QUADS);
		glColor3f(1.0, 0, 0);
		glVertex2f(0.3, 0.3);
		glVertex2f(-0.3, 0.3);
		glVertex2f(-0.3, -0.3);
		glVertex2f(0.3, -0.3);
		glEnd();
		glPopMatrix();

		glPopMatrix();
		break;

	case 8:
		glClearColor(color2, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		glLoadIdentity();
		renderScene();
		updateWindmillPosition();
		glPushMatrix();
		glTranslatef(0.0f, windmillY, 0.0f);
		glBegin(GL_QUADS);
		glColor3f(1, 1, 1);
		glVertex2f(-0.5, -0.7);
		glVertex2f(0.5, -0.7);
		glVertex2f(0.5, -0.65);
		glVertex2f(-0.5, -0.65);
		glEnd();

		glBegin(GL_LINE_LOOP);
		glColor3f(0, 0, 0);
		glVertex2f(-0.5, -0.7);
		glVertex2f(0.5, -0.7);
		glVertex2f(0.5, -0.65);
		glVertex2f(-0.5, -0.65);
		glEnd();

		glBegin(GL_QUADS);
		glColor3f(1, 1, 1);
		glVertex2f(-0.3, -0.7);
		glVertex2f(0.3, -0.7);
		glVertex2f(0.3, 0);
		glVertex2f(-0.3, 0);
		glEnd();

		glBegin(GL_LINE_LOOP);
		glColor3f(0, 0, 0);
		glVertex2f(-0.3, -0.7);
		glVertex2f(0.3, -0.7);
		glVertex2f(0.3, 0);
		glVertex2f(-0.3, 0);
		glEnd();

		glBegin(GL_QUADS);
		glColor3f(0, 0, 0);
		glVertex2f(-0.25, -0.67);
		glVertex2f(0.25, -0.67);
		glVertex2f(0.25, -0.3);
		glVertex2f(-0.25, -0.3);
		glEnd();

		glBegin(GL_LINE_LOOP);
		glColor3f(0, 0, 0);
		glVertex2f(-0.25, -0.67);
		glVertex2f(0.25, -0.67);
		glVertex2f(0.25, -0.3);
		glVertex2f(-0.25, -0.3);
		glEnd();

		glBegin(GL_TRIANGLES);
		glColor3f(1, 1, 1);
		glVertex2f(-0.5, 0);
		glVertex2f(0.5, 0);
		glVertex2f(0, 0.5);
		glEnd();

		glBegin(GL_LINE_LOOP);
		glColor3f(0, 0, 0);
		glVertex2f(-0.5, 0);
		glVertex2f(0.5, 0);
		glVertex2f(0, 0.5);
		glEnd();

		float x = 0;
		float y = 0;
		float radius = 0.2f;
		glBegin(GL_TRIANGLE_FAN);
		glColor3f(0, 0, 0);
		glVertex2f(x, y);
		for (int i = 0; i <= numOfTriangle; i++) {
			float newX = x + radius * cos(angle * i);
			float newY = y + radius * sin(angle * i);
			glVertex2f(newX, newY);
		}
		glEnd();

		float radius2 = 0.15f;
		glBegin(GL_TRIANGLE_FAN);
		glColor3f(1, 1, 1);
		glVertex2f(x, y);
		for (int i = 0; i <= numOfTriangle; i++) {
			float newX = x + radius2 * cos(angle * i);
			float newY = y + radius2 * sin(angle * i);
			glVertex2f(newX, newY);
		}
		glEnd();
		// Draw the fan with rotating blades
		updateRotation();
		drawFan(0.0f, 0.3f, 0.6f, numOfTriangle, angle, bladeRotationAngle);

		glPopMatrix();
		break;
	}
	//--------------------------------
	//	OpenGL drawing
	//--------------------------------
	

	

	//--------------------------------
	//	End of OpenGL drawing
	//--------------------------------
}
//--------------------------------------------------------------------


// hinstance hinst is something like provide an ID, like maybe i need to open 5 times the program, but they are in same program, then each of the program has an id
// histance hprevinst rarely use it, it is for security purpose
// lpstr cmdlineparam is command line parameter,OS pass command to program
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR cmdlineparam, int nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX)); //zeromemory find address, get the size, and fill zero 

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = GetModuleHandle(NULL); //or put hInst, same result
	wc.lpfnWndProc = WindowProcedure; //window procedure incharge on capture input and process it, example is when user wan to close the program, then it will capture it and process
	wc.lpszClassName = WINDOW_TITLE; //classname, can put whatever u wan
	wc.style = CS_HREDRAW | CS_VREDRAW; // horizontal size and vertical size

	if (!RegisterClassEx(&wc)) return false; //define the class, check whether it is correct created or not, the above one

	HWND hWnd = CreateWindow(WINDOW_TITLE, // this one is ur window class name,
		WINDOW_TITLE, // this one is your window title, at left corner
		WS_OVERLAPPEDWINDOW, //style
		CW_USEDEFAULT, //size of the window location
		CW_USEDEFAULT, //size of the window location
		800, //width
		800, //height
		NULL,
		NULL, //menu paramenter, something like this vs 2022 top one punya menu, file,edit.....
		wc.hInstance, //the id
		NULL);

	//--------------------------------
	//	Initialize window for OpenGL
	//--------------------------------

	HDC hdc = GetDC(hWnd);

	//	initialize pixel format for the window
	initPixelFormat(hdc);

	//	get an openGL context
	HGLRC hglrc = wglCreateContext(hdc);

	//	make context current
	if (!wglMakeCurrent(hdc, hglrc)) return false;

	//--------------------------------
	//	End initialization
	//--------------------------------

	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (true) // infinite loop, keep track user event
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) // check if got message or not,
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		display();

		SwapBuffers(hdc);
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);

	return true;
}
//--------------------------------------------------------------------